/**
 * @author moth
 * @date   2022.01.16
 * @version V1.0.0
 */
#include "coal_can.h"
#include "osal.h"
#include "options.h"
#include "osal_log.h"
#include "co_log.h"
#include "co_main.h"
#include "co_rtk.h"

//#include <drivers/can/can.h>
#include <fcntl.h>
#include <string.h>

static struct rt_semaphore rx_sem; /* 用于接收消息的信号量 */

/* 接收数据回调函数 */
static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* CAN 接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void can_rx_thread(void *parameter)
{
    os_channel_t * channel = parameter;
    rt_err_t res;

    /* 设置接收回调函数 */
    res = rt_device_set_rx_indicate(channel->handle, can_rx_call);
    CC_ASSERT(res == RT_EOK);

#ifdef RT_CAN_USING_HDR
    struct rt_can_filter_item items[5] =
    {
        /* std,match ID:0x100~0x1ff，hdr 为 - 1，设置默认过滤表 */
        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 0, 0x700, RT_NULL, RT_NULL),
    };
    struct rt_can_filter_config cfg =
    {   1, 1, items}; /* 一共有 1 个过滤表 */
    /* 设置硬件过滤表 */
    res = rt_device_control(channel->handle, RT_CAN_CMD_SET_FILTER, &cfg);
    CC_ASSERT(res == RT_EOK);
#endif

    while (1)
    {
        res = rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        if (channel->callback && (res == RT_EOK))
            channel->callback(channel->arg);
    }
}

os_channel_t * os_channel_open(const char * name, void * callback, void * arg)
{
    os_channel_t * channel = malloc(sizeof(*channel));
    rt_err_t res;

    channel->handle = rt_device_find(name);
    CC_ASSERT(channel->handle != NULL);

    res = rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (res != RT_EOK)
        goto err;

    res = rt_device_open(channel->handle, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
    if (res != RT_EOK)
        goto err;

    channel->callback = callback;
    channel->arg = arg;

    os_thread_create("co_rx", 5, 1024, can_rx_thread, channel);

    return channel;

    err:
    {
        rt_sem_detach(&rx_sem);
        free(channel);
        return NULL;
    }
}

int os_channel_send(os_channel_t * channel, uint32_t id, const void * data, size_t dlc)
{
    rt_size_t status;
    struct rt_can_msg msg = { 0 };

    msg.id = id;
    msg.ide = RT_CAN_STDID;
    msg.rtr = RT_CAN_DTR;
    msg.len = dlc;
    memcpy(msg.data, data, dlc);

    co_msg_log("Tx->", id, data, dlc);

    status = rt_device_write(channel->handle, 0, &msg, sizeof(msg));

    if (status <= 0)
        return -1;

    return 0;
}

int os_channel_receive(os_channel_t * channel, uint32_t * id, void * data, size_t * dlc)
{
    struct rt_can_msg rxmsg = { 0 };
    rt_device_read(channel->handle, 0, &rxmsg, sizeof(rxmsg));
    if (rxmsg.len <= 0)
        return -1;

    *id = rxmsg.id;
    *dlc = rxmsg.len;
    memcpy(data, rxmsg.data, rxmsg.len);

    co_msg_log("Rx<-", *id, data, *dlc);

    return 0;
}

/*
 __weak void co_can_get_cfg(int bitrate, can_cfg_t * cfg)
 {
 ASSERT(0);
 }
 */

int os_channel_set_bitrate(os_channel_t * channel, int bitrate)
{
    rt_err_t ret = RT_EOK;
    ret = rt_device_control(channel->handle, RT_CAN_CMD_SET_BAUD, (void *) bitrate);

    return ret;
}

int os_channel_set_filter(os_channel_t * channel, uint8_t * filter, size_t size)
{
//    rt_device_control(can_dev, RT_CAN_CMD_SET_FILTER, &filter);
    return 0;
}

int os_channel_bus_on(os_channel_t * channel)
{
    rt_err_t ret = RT_EOK;
    ret = rt_device_open(channel->handle, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
    CC_ASSERT(ret == RT_EOK);
    return ret;
}

int os_channel_bus_off(os_channel_t * channel)
{
    rt_err_t ret = RT_EOK;
    ret = rt_device_close(channel->handle);
    return ret;
}

int os_channel_get_state(os_channel_t * channel, os_channel_state_t * state)
{
    rt_err_t ret = RT_EOK;
    static struct rt_can_status can_status;
    rt_device_control(channel->handle, RT_CAN_CMD_GET_STATUS, &can_status);

	/* 暂时找不到rtthread这个status内容具体是什么所有这里没写 */
    state->overrun = 0;
    state->error_passive = 0;
    state->bus_off = 0;

    if (channel->handle == NULL)
        state->bus_off = 1;

//    state->overrun = can_status.flags & CAN_STATUS_BUFFER_OVERFLOW;
//    state->error_passive = can_status.flags & CAN_STATUS_ERROR_PASSIVE;
//    state->bus_off = can_status.flags & CAN_STATUS_BUS_OFF;

    return ret;
}
