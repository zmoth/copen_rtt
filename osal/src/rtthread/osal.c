/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is licensed under the terms of the BSD 3-clause
 * license. See the file LICENSE distributed with this software for
 * full license information.
 ********************************************************************/
/**
 * @author moth
 * @date   2022.01.16
 * @version V1.0.0
 */
 
#include "osal.h"

#include <stdlib.h>

#define TMO_TO_TICKS(ms) \
   ((ms == OS_WAIT_FOREVER) ? RT_WAITING_FOREVER : ((ms) * RT_TICK_PER_SECOND / 1000))

void * os_malloc(size_t size)
{
    return malloc(size);
}

void os_free(void * ptr)
{
    free(ptr);
}

os_thread_t * os_thread_create(const char * name, uint32_t priority, size_t stacksize, void (*entry)(void * arg),
        void * arg)
{
    rt_thread_t xHandle = NULL;

    /* stacksize in freertos is not in bytes but in stack depth, it should be
     * divided by the stack width */
    rt_uint32_t stackdepth = stacksize;

    xHandle = rt_thread_create(name, entry, arg, stackdepth, priority, 10);
    if (xHandle != RT_NULL)
    {
        rt_thread_startup(xHandle);
        return (os_thread_t *) xHandle;
    }
    else
    {
        return NULL;
    }
}

os_mutex_t * os_mutex_create(void)
{
    static os_mutex_t handle = RT_NULL;
    handle = rt_mutex_create("osmutex", RT_IPC_FLAG_PRIO);
    CC_ASSERT(handle != RT_NULL);
    return (os_mutex_t *) handle;
}

void os_mutex_lock(os_mutex_t * mutex)
{
    rt_mutex_take((rt_mutex_t) mutex, RT_WAITING_FOREVER);
}

void os_mutex_unlock(os_mutex_t * mutex)
{
    rt_mutex_release((rt_mutex_t) mutex);
}

void os_mutex_destroy(os_mutex_t * mutex)
{
    rt_mutex_delete((rt_mutex_t) mutex);
}

void os_usleep(uint32_t us)
{
    rt_thread_mdelay(us / 1000);
}

uint32_t os_get_current_time_us(void)
{
    return 1000000 * rt_tick_get() / RT_TICK_PER_SECOND;
}

os_sem_t * os_sem_create(size_t count)
{
    static os_sem_t handle = RT_NULL;
    handle = rt_sem_create("ossem", (rt_uint32_t) count, RT_IPC_FLAG_PRIO);
    CC_ASSERT(handle != RT_NULL);
    return (os_sem_t *) handle;
}

bool os_sem_wait(os_sem_t * sem, uint32_t time)
{
    static rt_err_t result;
    result = rt_sem_take((rt_sem_t) sem, TMO_TO_TICKS(time));
    if (result != RT_EOK)
    {
        /* Did not timeout */
        return false;
    }

    /* Timed out */
    return true;
}

void os_sem_signal(os_sem_t * sem)
{
    rt_sem_release((rt_sem_t) sem);
}

void os_sem_destroy(os_sem_t * sem)
{
    rt_sem_delete((rt_sem_t) sem);
}

os_event_t * os_event_create(void)
{
    rt_event_t handle = RT_NULL;
    handle = rt_event_create("osevent", RT_IPC_FLAG_PRIO);
    CC_ASSERT(handle != RT_NULL);
    return (os_event_t *) handle;
}

bool os_event_wait(os_event_t * event, uint32_t mask, uint32_t * value, uint32_t time)
{
    //TODO: value
    rt_event_recv((rt_event_t) event, (rt_uint32_t) mask, (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
            (rt_int32_t) TMO_TO_TICKS(time), (rt_uint32_t *) value);
    *value &= mask;
    return *value == 0;
}

void os_event_set(os_event_t * event, uint32_t value)
{
    rt_event_send((rt_event_t) event, value);
}

void os_event_clr(os_event_t * event, uint32_t value)
{
//    xEventGroupClearBits((EventGroupHandle_t) event, value);
}

void os_event_destroy(os_event_t * event)
{
    rt_event_delete((rt_event_t) event);
}

os_mbox_t * os_mbox_create(size_t size)
{
    os_mbox_t handle = RT_NULL;
    handle = rt_mq_create("osmq", sizeof(void *), size, RT_IPC_FLAG_PRIO);
    CC_ASSERT(handle != RT_NULL);
    return (os_mbox_t *) handle;
}

bool os_mbox_fetch(os_mbox_t * mbox, void ** msg, uint32_t time)
{
    rt_err_t success = RT_EOK;
    success = rt_mq_recv((rt_mq_t) mbox, msg, sizeof(void *), TMO_TO_TICKS(time));
    return (success != RT_EOK);
}

bool os_mbox_post(os_mbox_t * mbox, void * msg, uint32_t time)
{
    rt_err_t success = RT_EOK;
    success = rt_mq_send_wait((rt_mq_t) mbox, &msg, sizeof(void *), TMO_TO_TICKS(time));
//    success = rt_mq_send((rt_mq_t) mbox, &msg, sizeof(void *));
    return success != RT_EOK;
}

void os_mbox_destroy(os_mbox_t * mbox)
{
    rt_mq_delete((rt_mq_t) mbox);
}

static void os_timer_callback(void * xTimer)
{
    os_timer_t * timer = xTimer;

    if (timer->fn)
        timer->fn(timer, timer->arg);
}

os_timer_t * os_timer_create(uint32_t us, void (*fn)(os_timer_t *, void * arg), void * arg, bool oneshot)
{
    os_timer_t * timer;

    timer = malloc(sizeof(*timer));
    CC_ASSERT(timer != NULL);

    timer->fn = fn;
    timer->arg = arg;
    timer->us = us;

    timer->handle = rt_timer_create("os_timer", os_timer_callback, timer, (us * RT_TICK_PER_SECOND / 1000000),
            (oneshot ? RT_TIMER_FLAG_ONE_SHOT : RT_TIMER_FLAG_PERIODIC));
    CC_ASSERT(timer->handle != RT_NULL);
//    rt_timer_start(timer->handle);
    return timer;
}

void os_timer_set(os_timer_t * timer, uint32_t us)
{
    timer->us = us;
}

void os_timer_start(os_timer_t * timer)
{
    /* Start timer by updating the period */
    CC_ASSERT(timer->handle != RT_NULL);
    CC_ASSERT(rt_timer_start(timer->handle) == RT_EOK);

//    rt_err_t res;
//    res = rt_timer_control(timer->handle, RT_TIMER_CTRL_SET_TIME, (void *) (timer->us * RT_TICK_PER_SECOND / 1000000));
//    CC_ASSERT(res == RT_EOK);
}

void os_timer_stop(os_timer_t * timer)
{
    CC_ASSERT(rt_timer_stop(timer->handle) == RT_EOK);
}

void os_timer_destroy(os_timer_t * timer)
{
    CC_ASSERT(rt_timer_delete(timer->handle) == RT_EOK);
    free(timer);
}
