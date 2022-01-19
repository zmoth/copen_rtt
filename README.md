# copen_rtt
> 能力有限，使用canfestival-rtt一直失败，写SDO失败。故搜索其它canopen协议栈自己简单移植了一个。

# 源码下载
1. 协议栈本身(改协议包含了DS301、305大部分内容)
[https://github.com/rtlabs-com/c-open](https://github.com/rtlabs-com/c-open)
2. 操作系统抽象层
[https://github.com/rtlabs-com/osal](https://github.com/rtlabs-com/osal)

# 操作系统抽象层修改(osal)
只需要修改osal_sys.h和osal.c即可，主要就是将Freertos改成rtthread。

# 协议栈接口修改(c-open)

找到ports，主要就是吧coal_can.c改成rthread的can的程序就ok了。(根据rtt官方的can的例子很好移植)。

虽然说着简单但是实际还是花费了一点时间，而且还没有支持DS402子协议。
在此我就直接贴出所有程序，直接添加到您所在的工程中就能使用了。

[https://github.com/QianMoth/copen_rtt](https://github.com/QianMoth/copen_rtt)

# 效果展示
```bash
[09:45:21.377]收←◆
 \ | /
- RT -     Thread Operating System
 / | \     4.0.3 build Jan 17 2022
 2006 - 2020 Copyright by rt-thread team
[INFO ] state = STATE_INIT_PWRON
[INFO ] state = STATE_INIT_APP
[INFO ] state = STATE_INIT_COMM
[INFO ] state = STATE_PREOP
[DEBUG] Tx-> 0702: 00
[INFO ] state = STATE_INIT_COMM
[INFO ] state = STATE_PREOP
[DEBUG] Tx-> 0702: 00
[DEBUG] Tx-> 0000: 82 00
msh >
[09:45:21.439]收←◆[DEBUG] Rx<- 0702: 00
[DEBUG] Rx<- 0082: 00 00 00 00 00 00 00 00

[09:45:21.918]收←◆[DEBUG] sdo read 2:1008:00
[DEBUG] Tx-> 0602: 40 08 10 00 00 00 00 00
[DEBUG] Rx<- 0582: 41 08 10 00 0a 00 00 00
[DEBUG] Tx-> 0602: 60 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 00 43 44 48 44 20 64 72
[DEBUG] Tx-> 0602: 70 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 19 69 76 65 00 00 00 00
(2) 0x1008 CDHD drive
[DEBUG] sdo read 2:1009:00
[DEBUG] Tx-> 0602: 40 09 10 00 00 00 00 00
[DEBUG] Rx<- 0582: 41 09 10 00 13 00 00 00
[DEBUG] Tx-> 0602: 60 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 00 43 6f 6e 74 72 6f 6c
[DEBUG] Tx-> 0602: 70 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 10 3a 42 31 20 50 6f 77
[DEBUG] Tx-> 0602: 60 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 05 65 72 3a 31 30 00 00
(2) 0x1009 Control:B1 Power:10
[DEBUG] sdo read 2:100A:00
[DEBUG] Tx-> 0602: 40 0a 10 00 00 00 00 00
[DEBUG] Rx<- 0582: 41 0a 10 00 07 00 00 00
[DEBUG] Tx-> 0602: 60 00 00 00 00 00 00 00
[DEBUG] Rx<- 0582: 01 31 2e 34 31 2e 31 34
(2) 0x100a 1.41.14
[INFO ] state = STATE_PREOP
[DEBUG] Tx-> 0000: 80 00
[INFO ] state = STATE_OP
[DEBUG] Tx-> 0000: 01 00
[DEBUG] Tx-> 0080:
end[DEBUG] Rx<- 0182: d0 02 01 00 00
[DEBUG] Rx<- 0482: 84 aa 03 00 00 00 00 00
[BDEBUG] Rx<- 0382: 84 aa 03 00 00 00 00 00
[DEBUG] Rx<- 0282: 84 aa 03 00 00 00 00 00
```