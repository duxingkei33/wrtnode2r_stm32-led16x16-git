/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <rtthread.h>
#include <stdint.h>
#include <string.h>
#include "board.h"

#ifdef RT_USING_FINSH
#include <shell.h>
#include <finsh.h>
#endif

void rt_init_thread_entry(void* parameter)
{
    int ch;

    rt_thread_delay(RT_TICK_PER_SECOND);

    {
        extern void rt_platform_init(void);
        rt_platform_init();
    }
#ifdef RT_USING_FINSH
    /* initialize finsh */
    finsh_system_init();
    finsh_set_device(RT_CONSOLE_DEVICE_NAME);
    rt_thread_delay(10); /* wait finsh thread init OK */
    if(rt_strncmp(RT_CONSOLE_DEVICE_NAME, "bridge", sizeof("bridge")) == 0)
        finsh_set_echo(0);
#endif
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
    rt_show_version();
}





#ifdef RT_USING_LED16X16_THREAD


extern int   Disp16x16All(void);


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t disp_stack[ 2048 ]; //512

static struct rt_thread disp_thread;

static void disp_thread_entry(void* parameter)
{
    unsigned int count=0;
    //      rt_thread_delay(5* RT_TICK_PER_SECOND/2 );
        rt_kprintf("disp_thread_entry(&disp_thread);\r\n");

    while (1)
    {
#if 1
        Disp16x16All();
#else
        rt_thread_delay(5* RT_TICK_PER_SECOND/2 );
        rt_kprintf("disp_thread_entry(&disp_thread);\r\n");
#endif

    }
}






#endif





int rt_application_init(void)
{
    rt_thread_t tid;




    tid = rt_thread_create("init",
                           rt_init_thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX/3, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);


#ifdef RT_USING_LED16X16_THREAD


    // rt_kprintf("rt_thread_create disp_thread \r\n");

    tid = rt_thread_create("display",
                           disp_thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX/3, 20);
    if (tid != RT_NULL) rt_thread_startup(tid);



#endif

#if 0
    rt_err_t result;    /* init disp thread */
    rt_kprintf("rt_thread_init(&disp_thread);\r\n");

    result = rt_thread_init(&disp_thread,
                            "disp",
                            disp_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&disp_stack[0],
                            sizeof(disp_stack),
                            20,
                            5);
    rt_kprintf("end rt_thread_init(&disp_thread);\r\n");


    if (result == RT_EOK)
    {
        rt_thread_startup(&disp_thread);
        rt_kprintf("rt_thread_startup(&disp_thread);\r\n");
    }
    else
    {
        rt_kprintf("rt_thread_init(&disp_thread) failed\r\n");

    }

#endif


    return 0;
}

int cmd_digitalRead(int argc, char** argv)
{
    int ret = 0;
    int pin;
    if(argc == 2)
    {
        sscanf(argv[1], "%d", &pin);
        ret = digitalRead((rt_uint8_t)pin);
    }
    rt_kprintf("%d\n", ret);

    return ret;
}
MSH_CMD_EXPORT_ALIAS(cmd_digitalRead, digitalRead, Read digital pin.);

int cmd_pinMode(int argc, char** argv)
{
    int pin;
    int mode;
    if(argc == 3)
    {
        sscanf(argv[1], "%d", &pin);
        sscanf(argv[2], "%d", &mode);
        pinMode((uint8_t)pin, (int)mode);
    }
    else
    {
        rt_kprintf("Not enough argc,example:pinMode 1 1 \r\n");
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_pinMode, pinMode, Setup pin mode.);

int cmd_digitalWrite(int argc, char** argv)
{
    int pin;
    int val;
    if(argc == 3)
    {
        sscanf(argv[1], "%d", &pin);
        sscanf(argv[2], "%d", &val);
        digitalWrite((uint8_t)pin, (uint8_t)val);
    }
    else
    {
        rt_kprintf("Not enough argc,example:digitalWrite 1 1 \r\n");
    }


    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_digitalWrite, digitalWrite, Write digital pin.);

extern void __set_PRIMASK(uint32_t priMask);
extern void __set_PSP(uint32_t topOfProcStack);
void cmd_reset(void)
{
    rt_uint32_t usrAddr = 0x08000000;
    typedef void (*funcPtr)(void);
    typedef volatile uint32_t vu32;
    __set_PRIMASK(0); //关闭所有中断，可以没有
    if(((*(vu32*)usrAddr)&0x2FFE0000)==0x20000000) //判断地址是不是在RAM之内
    {
        rt_uint32_t jumpAddr = *(vu32*) (usrAddr+0x04); /* reset ptr in vector table */
        funcPtr usrMain1 = (funcPtr)jumpAddr;
        __set_PSP(*(vu32*)usrAddr);  //设置堆栈指针
        usrMain1();                                /* go! */
    }
}
MSH_CMD_EXPORT_ALIAS(cmd_reset, reset, reset to bootloader.);

















