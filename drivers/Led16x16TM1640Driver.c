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

#include "wirish/io.h"


#if 0
//映射分布
extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] =
{
    PMAP_ROW(GPIOB,   0, TIMER3,  3, ADC1,    8), /* D0/PB0 */
    PMAP_ROW(GPIOB,   1, TIMER3,  4, ADC1,    9), /* D1/PB1 */
    PMAP_ROW(GPIOB,   2,   NULL,  0, NULL, ADCx), /* D2/PB2 */
    PMAP_ROW(GPIOB,   3,   NULL,  0, NULL, ADCx), /* D3/PB3 */
    PMAP_ROW(GPIOB,   4,   NULL,  0, NULL, ADCx), /* D4/PB4 */
    PMAP_ROW(GPIOB,   5,   NULL,  0, NULL, ADCx), /* D5/PB5 */
    PMAP_ROW(GPIOB,   6, TIMER4,  1, NULL, ADCx), /* D6/PB6 */
    PMAP_ROW(GPIOB,   7, TIMER4,  2, NULL, ADCx), /* D7/PB7 */
    PMAP_ROW(GPIOA,   0, TIMER2,  1, ADC1,    0), /* D8/PA0 */
    PMAP_ROW(GPIOA,   1, TIMER2,  2, ADC1,    1), /* D9/PA1 */
    PMAP_ROW(GPIOA,   2, TIMER2,  3, ADC1,    2), /* D10/PA2 */
    PMAP_ROW(GPIOA,   3, TIMER2,  4, ADC1,    3), /* D11/PA3 */
    PMAP_ROW(GPIOA,   8, TIMER1,  1, NULL, ADCx), /* D12/PA8 */
    PMAP_ROW(GPIOA,   9, TIMER1,  2, NULL, ADCx), /* D13/PA9 */
    PMAP_ROW(GPIOA,  10, TIMER1,  3, NULL, ADCx), /* D14/PA10 */
    PMAP_ROW(GPIOA,  11, TIMER1,  4, NULL, ADCx), /* D15/PA11 */
    PMAP_ROW(GPIOA,  12,   NULL,  0, NULL, ADCx), /* D16/PA12 */
    PMAP_ROW(GPIOA,  13,   NULL,  0, NULL, ADCx), /* D17/PA13 */
    PMAP_ROW(GPIOA,  14,   NULL,  0, NULL, ADCx), /* D18/PA14 */
    PMAP_ROW(GPIOA,  15,   NULL,  0, NULL, ADCx), /* D19/PA15 */
    PMAP_ROW(GPIOA,   4,   NULL,  0, NULL, ADCx), /* D20/PA4 */
    PMAP_ROW(GPIOA,   5,   NULL,  0, NULL, ADCx), /* D21/PA5 */
    PMAP_ROW(GPIOA,   6,   NULL,  0, NULL, ADCx), /* D22/PA6 */
    PMAP_ROW(GPIOA,   7,   NULL,  0, NULL, ADCx), /* D23/PA7 */
};

#endif


#define _ON    (1)
#define _OFF    (0)




#if 0
//TM1640接口定义
#define PORT_TM_DIN                 0
#define PORT_TM_SCK4                    1
#define PORT_TM_SCK3                    2
#define PORT_TM_SCK2                    3
#define PORT_TM_SCK1                    4
#else
//TM1640接口定义
#define PIN_TM_DIN                  16     //PA12  ---D16
#define PIN_TM_SCK4                 0   //PB0   ---D0
#define PIN_TM_SCK3                 1   //PB1   ---D1
#define PIN_TM_SCK2                 2   //PB2   ---D2
#define PIN_TM_SCK1                 5   //PB5  ---D5
//测试运行时间脚脉冲高电平代表一个循环
#define PIN_TEST                 15   //PA11  ---D15  /* D15/PA11 */


#endif

#if 0
// pinMode((uint8_t)PIN_TM_SCK1, (int)OUTPUT);
#define PORT_TM_SCK1=0        do{   digitalWrite(PIN_TM_SCK1, 0); }while(0)
#define PORT_TM_SCK2=0        do{   digitalWrite(PIN_TM_SCK2, 0); }while(0)
#define PORT_TM_SCK3=0        do{   digitalWrite(PIN_TM_SCK3, 0); }while(0)
#define PORT_TM_SCK4=0        do{   digitalWrite(PIN_TM_SCK4, 0); }while(0)
#define PORT_TM_DIN=0        do{   digitalWrite(PIN_TM_DIN, 0); }while(0)

//
#define PORT_TM_SCK1=1        do{   digitalWrite(PIN_TM_SCK1, 1); }while(0)
#define PORT_TM_SCK2=1        do{   digitalWrite(PIN_TM_SCK2, 1); }while(0)
#define PORT_TM_SCK3=1        do{   digitalWrite(PIN_TM_SCK3, 1); }while(0)
#define PORT_TM_SCK4=1        do{   digitalWrite(PIN_TM_SCK4, 1); }while(0)
#define  PORT_TM_DIN=1        do{   digitalWrite(PIN_TM_DIN, 1); }while(0)
#endif

void Led16x16PortInit()
{
    pinMode((uint8_t)PIN_TM_SCK1, (int)0);
    pinMode((uint8_t)PIN_TM_SCK2, (int)0);
    pinMode((uint8_t)PIN_TM_SCK3, (int)0);
    pinMode((uint8_t)PIN_TM_SCK4, (int)0);
    pinMode((uint8_t)PIN_TM_DIN, (int)0);//OUTPUT
#ifdef PIN_TEST
    pinMode((uint8_t)PIN_TEST, (int)0);//OUTPUT
#endif
	
}




#define MAX_DISP_BUFFER    (2*8)


//显示命令定义
/*
MSB LSB
B7 B6 B5 B4 B3 B2 B1 B0 功能说明
1 0 001 0 0 0 设置脉冲宽度为1/16
1 0 001 0 0 1 设置脉冲宽度为2/16
1 0 001 0 1 0 设置脉冲宽度为4/16
1 0 001 0 1 1 设置脉冲宽度为10/16
1 0 001 1 0 0 设置脉冲宽度为11/16
1 0 001 1 0 1 设置脉冲宽度为12/16
1 0 001 1 1 0 设置脉冲宽度为13/16
1 0 001 1 1 1设置脉冲宽度为14/16
1 0 0 X X X 显示关
BIT4 BIT5 无关项，填0
BIT3 显示开关
*/
//亮度
#define TM1640_CMD_DISPLAY  ((1<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0))
#define TM_DISP_CON_UNLOCK    ((1<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<3)|(1<<2)|(0<<1)|(1<<0))//0b10001101/  //平时的亮度 BIT3=1 开显示，BIT2-BIT0表示亮度
#define TM_DISP_CON_LOCK     ((1<<7)|(0<<6)|(0<<5)|(0<<4)|(1<<3)|(0<<2)|(0<<1)|(0<<0))//0B10001000 //锁屏时亮度 设置脉冲宽度为1/16


#define TM_DISP_CON_OFF     ((1<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))//0B10001000 //锁屏时亮度 设置脉冲宽度为1/16

static int  LedDispCtrl=TM_DISP_CON_UNLOCK;


static int LedDispSwitch= _ON;


//地址自动加
#define TM1640_CMD_SET_DATA ((0<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))
//设置首地址
#define TM1640_CMD_SET_ADDR ((1<<7)|(1<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|(0<<0))


//全亮测试
#define DBG_ALL_DISPLAY     _OFF  //    _ON//
//默认的全亮值
#define DBG_ALL_DISPLAY_VALUE1  (0Xff) //(0XFF)

#define DBG_ALL_DISPLAY_VALUE2  (0Xff) //(0XFF)
#define DBG_ALL_DISPLAY_VALUE3  (0Xff) //(0XFF)
#define DBG_ALL_DISPLAY_VALUE4  (0Xff) //(0XFF)




#define HZ_NUM_HENG 8

const uint8 hz_heng[][16*16/8] =
{
    {0x04,0x04,0x44,0x44,0x44,0x5F,0x44,0xF4,0x44,0x5F,0x44,0xC4,0x04,0x04,0x04,0x00,0x00,0x40,0x4C,0x27,0x10,0x0C,0x07,0x01,0x20,0x40,0x40,0x3F,0x00,0x02,0x0C,0x00},/*"苏",0*/
    {0x42,0x42,0xFE,0x42,0x42,0x00,0x40,0x5E,0x50,0xD0,0x5F,0x50,0x50,0x5E,0x40,0x00,0x10,0x10,0x0F,0x08,0x04,0x00,0xFF,0x01,0x01,0x7F,0x01,0x3F,0x41,0x81,0x7F,0x00},/*"瑞",1*/
    {0x10,0x12,0x92,0x72,0xFE,0x51,0x91,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,0x00,0x04,0x02,0x01,0x00,0xFF,0x00,0x04,0x04,0x04,0x02,0x02,0x02,0xFF,0x01,0x01,0x00},/*"科",2*/
    {0x08,0x08,0x88,0xFF,0x48,0x28,0x00,0xC8,0x48,0x48,0x7F,0x48,0xC8,0x48,0x08,0x00,0x01,0x41,0x80,0x7F,0x00,0x40,0x40,0x20,0x13,0x0C,0x0C,0x12,0x21,0x60,0x20,0x00},/*"技",3*/
    {0x14,0x24,0x44,0x84,0x64,0x1C,0x20,0x18,0x0F,0xE8,0x08,0x08,0x28,0x18,0x08,0x00,0x20,0x10,0x4C,0x43,0x43,0x2C,0x20,0x10,0x0C,0x03,0x06,0x18,0x30,0x60,0x20,0x00},/*"欢",4*/
    {0x40,0x41,0xCE,0x04,0x00,0xFC,0x04,0x02,0x02,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,0x40,0x20,0x1F,0x20,0x40,0x47,0x42,0x41,0x40,0x5F,0x40,0x42,0x44,0x43,0x40,0x00},/*"迎",5*/
    {0x80,0x40,0x30,0xFC,0x03,0x90,0x68,0x06,0x04,0xF4,0x04,0x24,0x44,0x8C,0x04,0x00,0x00,0x20,0x38,0x03,0x38,0x40,0x40,0x49,0x52,0x41,0x40,0x70,0x00,0x09,0x30,0x00},/*"您",6*/
    {0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"！",7*/

};


/////////////////////////////////////////////////////////////////////////////////////////////
static uint8 LedBuf1[MAX_DISP_BUFFER];
static uint8 LedBuf2[MAX_DISP_BUFFER];
static uint8 LedBuf3[MAX_DISP_BUFFER];
static uint8 LedBuf4[MAX_DISP_BUFFER];

//==1 刷显存
static uint8 RefreshLed16x16Flag =1;

//红色显存
static uint8 LedBufRamRed[16*16/8]=
{
    0x00,//0x00,0x00,0x7F,0x40,0x42,0x52,0x52,0x7E,0x52,0x52,0x42,0x40,0x7F,0x00,0x00,0x01,0x02,0x0C,0xF0,0x00,0x00,0xF8,0x90,0x90,0x90,0xF8,0x02,0x01,0xFE,0x00,0x00

//  0xff,0x0f,0xf0,0xf0,0x0,0x0,0x0,0x0,
//  0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,
//  0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,
//  0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0


};

//绿色显存
static uint8 LedBufRamGreen[16*16/8]=
{
    0x00,0x00,0x00,0xFE,0x02,0x42,0x4A,0x4A,0x7E,0x4A,0x4A,0x42,0x02,0xFE,0x00,0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x1F,0x09,0x09,0x09,0x1F,0x40,0x80,0x7F,0x00,0x00//0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,
//0x0,0x0,0xff,0x0,0x0,0x0,0x0,0x0,
//0x0,0x0,0x0,0xff,0x0,0x0,0x0,0x0,
//0x00,0x00,0x00,0x7F,0x40,0x42,0x52,0x52,0x7E,0x52,0x52,0x42,0x40,0x7F,0x00,0x00,0x01,0x02,0x0C,0xF0,0x00,0x00,0xF8,0x90,0x90,0x90,0xF8,0x02,0x01,0xFE,0x00,0x00
};



static void DispClearAll()
{
    uint8 i=0;
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {

        LedBuf1[i]=  0;
        LedBuf2[i]=  0;
        LedBuf3[i]=  0;
        LedBuf4[i]=  0;
    }


}

//16x16/8字节=
static void DispRamRed16x16_heng(uint8 * disp)
{
    uint8 i=0;
    RefreshLed16x16Flag = 1;
//    LedDispCtrl = TM_DISP_CON_UNLOCK;
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf3[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf4[i]= *disp++;
        }
    }

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf2[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf1[i]= *disp++;
        }
    }


}


//16x16/8字节=
static void DispRamGreen16x16_heng(uint8 * disp)
{
    uint8 i=0;
    RefreshLed16x16Flag = 1;
//   LedDispCtrl = TM_DISP_CON_UNLOCK;
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf3[i]= *disp++;
        }
    }

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf4[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf2[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf1[i]= *disp++;
        }
    }


}







//10MS一次
//显示同PCB板丝印
void DispAllheng()
{
    static uint8 index=0;
    static uint16  DelayCnt=0;
    DelayCnt++;
    if(DelayCnt>(800/10))
    {
        DelayCnt=0;
        DispClearAll();
        if(index/HZ_NUM_HENG == 0)
        {
            DispRamRed16x16_heng((uint8 *)hz_heng[index++]);
        }
        else if (index/HZ_NUM_HENG == 1)
        {
            DispRamGreen16x16_heng((uint8 *)hz_heng[index%HZ_NUM_HENG]);
            index++;
        }
        else if (index/HZ_NUM_HENG == 2)
        {
            DispRamGreen16x16_heng((uint8 *)hz_heng[index%HZ_NUM_HENG]);
            DispRamRed16x16_heng((uint8 *)hz_heng[index%HZ_NUM_HENG]);
            index++;
        }
        else
        {
            index = 0;
        }


    }


}



///////////////////////////////////////////////////////////////////////////////////////////////

//显示方向为7线在上面
//点阵为从左到右
#if 1
const uint8 hz_shu[][16*16/8] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
    {0x00,0x00,0xFC,0x05,0x85,0x4A,0x28,0x10,0x18,0x18,0x24,0x24,0x41,0x86,0x38,0x00,0x80,0x80,0x80,0xFE,0x04,0x48,0x40,0x40,0x40,0x60,0xA0,0x90,0x18,0x0E,0x04,0x00},/*"欢",0*/
    {0x40,0x21,0x36,0x24,0x04,0x04,0xE4,0x24,0x25,0x26,0x24,0x20,0x20,0x50,0x8F,0x00,0x00,0x80,0x7C,0x44,0x44,0x44,0x44,0x44,0x44,0x54,0x48,0x40,0x40,0x00,0xFE,0x00},/*"迎",0*/
    {0x01,0x21,0x19,0x0D,0x09,0x01,0x7F,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x40,0x00,0x10,0x18,0x10,0x20,0x04,0xFE,0x40,0x40,0x40,0x40,0x42,0x42,0x42,0x3E,0x00},/*"光",0*/
    {0x01,0x09,0x09,0x49,0x4A,0x4A,0x4C,0x49,0x49,0x49,0x49,0x49,0x49,0x09,0x09,0x00,0x00,0x80,0x00,0xFE,0x20,0x10,0x10,0x04,0xFE,0x24,0x24,0x24,0x24,0xFC,0x04,0x00},/*"临",0*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"，",0*/
    {0x01,0x01,0x02,0x04,0x08,0x12,0x21,0xC0,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x80,0x40,0x20,0x18,0x8E,0x84,0x00,0xF0,0x20,0x20,0x40,0x40,0x80,0x00},/*"今",0*/
    {0x00,0x3F,0x01,0x01,0x01,0x01,0x7F,0x01,0x01,0x02,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0xFC,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x80,0x40,0x20,0x18,0x0E,0x04,0x00},/*"天",0*/
    {0x00,0x0F,0x08,0x0F,0x08,0x0F,0x00,0xFF,0x01,0x09,0x09,0x09,0x15,0x23,0x40,0x00,0x00,0xF0,0x10,0xF0,0x10,0xF0,0x00,0xFE,0x00,0x00,0xF8,0x00,0x00,0x00,0xFE,0x00},/*"是",0*/
    {0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"20",*/
    {0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"15",*/
    {0x08,0x0F,0x10,0x10,0x20,0x4F,0x88,0x08,0x08,0x08,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x80,0x80,0x80,0xF8,0x80,0x80,0x80,0x80,0xFE,0x80,0x80,0x80,0x80,0x80},/*"年",0*/
    {0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},/*"1",0 "2",0*/
    {0x07,0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x07,0x04,0x08,0x08,0x10,0x20,0x40,0x00,0xF0,0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0x10,0x50,0x20,0x00},/*"月",0*/
    {0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},/*"1",0 "7",0*/
    {0x00,0x1F,0x10,0x10,0x10,0x10,0x1F,0x10,0x10,0x10,0x10,0x10,0x1F,0x10,0x00,0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0x10,0x10,0xF0,0x10,0x00,0x00},/*"日",0*/
    {0x00,0x1F,0x10,0x1F,0x10,0x1F,0x01,0x11,0x1F,0x21,0x21,0x4F,0x01,0x01,0x7F,0x00,0x00,0xF8,0x08,0xF8,0x08,0xF8,0x00,0x00,0xFC,0x00,0x00,0xF8,0x00,0x00,0xFE,0x00},/*"星",0*/
    {0x22,0x22,0x7F,0x22,0x3E,0x22,0x3E,0x22,0x22,0xFF,0x00,0x24,0x22,0x43,0x81,0x00,0x00,0x7C,0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x7C,0x44,0x84,0x84,0x14,0x08,0x00},/*"期",0*/
    {0x00,0x7F,0x44,0x44,0x44,0x44,0x44,0x44,0x48,0x48,0x50,0x60,0x40,0x7F,0x40,0x00,0x00,0xFC,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x7C,0x04,0x04,0x04,0xFC,0x04,0x00},/*"四",0*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",0*/
    {0x01,0x01,0x01,0x01,0x01,0x1F,0x10,0x10,0x10,0x1F,0x10,0x01,0x28,0x24,0x44,0x00,0x00,0x00,0xF8,0x00,0x10,0xF8,0x10,0x10,0x10,0xF0,0x10,0x10,0x88,0x44,0x44,0x00},/*"点",0*/
    {0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"5"  "6",0*/
    {0x08,0x0C,0x08,0x10,0x10,0x20,0x40,0x9F,0x04,0x04,0x04,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x30,0x18,0x0E,0xE4,0x20,0x20,0x20,0x20,0x20,0xA0,0x40,0x00},/*"分",0*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*" ",0*/
};

#else
const uint8 hz_shu[][16*16/8] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},/*"欢",0*/
    {0x00,0x00,0xFC,0x05,0x85,0x4A,0x28,0x10,0x18,0x18,0x24,0x24,0x41,0x86,0x38,0x00,0x80,0x80,0x80,0xFE,0x04,0x48,0x40,0x40,0x40,0x60,0xA0,0x90,0x18,0x0E,0x04,0x00},/*"欢",0*/
    {0x40,0x21,0x36,0x24,0x04,0x04,0xE4,0x24,0x25,0x26,0x24,0x20,0x20,0x50,0x8F,0x00,0x00,0x80,0x7C,0x44,0x44,0x44,0x44,0x44,0x44,0x54,0x48,0x40,0x40,0x00,0xFE,0x00},/*"迎",0*/
    {0x01,0x21,0x19,0x0D,0x09,0x01,0x7F,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x40,0x00,0x10,0x18,0x10,0x20,0x04,0xFE,0x40,0x40,0x40,0x40,0x42,0x42,0x42,0x3E,0x00},/*"光",0*/
    {0x01,0x09,0x09,0x49,0x4A,0x4A,0x4C,0x49,0x49,0x49,0x49,0x49,0x49,0x09,0x09,0x00,0x00,0x80,0x00,0xFE,0x20,0x10,0x10,0x04,0xFE,0x24,0x24,0x24,0x24,0xFC,0x04,0x00},/*"临",0*/
};


#endif

#if 0
const uint8 hz_shu[][16*16/8] =
{
    {
        1,2,4,8,16,32,64,128,256,0
    },/*"欢",0*/
};
#endif


#define HZ_NUM_SHU  (sizeof(hz_shu)/32)


//16x16/8字节=
//经过验证.低位在右边
static void DispRamRed16x16_shu(uint8 * disp)
{
    uint8 i=0;
    RefreshLed16x16Flag = 1;
//   LedDispCtrl = TM_DISP_CON_UNLOCK;
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf2[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf1[i]= *disp++;
        }
    }

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf3[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==1)
        {
            LedBuf4[i]= *disp++;
        }
    }


}


//16x16/8字节=
static void DispRamGreen16x16_shu(uint8 * disp)
{
    uint8 i=0;

    RefreshLed16x16Flag = 1;
//    LedDispCtrl = TM_DISP_CON_UNLOCK;

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf2[i]= *disp++;
        }
    }

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf1[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf3[i]= *disp++;
        }
    }
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        if((i%2)==0)
        {
            LedBuf4[i]= *disp++;
        }
    }


}


//输入的内存是以从左到右,低位在右边
//从上到下分布


//
//输入字母取模分布
//每字节,低位在实际显示屏右边
//字模显示分布:
//BYTY 0        BYTY 16
//BYTY 1        BYTY 17
//BYTY 2        BYTY 18
//BYTY 3        BYTY 19
//BYTY 4        BYTY 20
//BYTY 5        BYTY 21
//BYTY 6        BYTY 22
//BYTY 7        BYTY23
//BYTY 8        BYTY 24
//BYTY 9        BYTY 25
//BYTY 10      BYTY 26
//BYTY 11      BYTY 27
//BYTY 12      BYTY 28
//BYTY 13      BYTY 29
//BYTY 14      BYTY 30
//BYTY 15      BYTY 31
//


void DispRamGreen16x16_shuMoveLeft(const uint8* current16x16,const uint8* Next16x16,uint8 NumMoveLeft)
{
//临时显存
    uint8 DispTmp[32]= {0};
    uint8 index=0;
//一行预先23*16里面的显存
//根据左移不同,从32bit里面选取对应的字节数据到显存
    uint32 tmp32=0;

    if (NumMoveLeft>15)
    {
        NumMoveLeft =0;
    }


    for(index=0; index<16; index++)
    {
#if 0
        tmp32 = current16x16[index]<<(7+8*2) |current16x16[index+16]<<(7+8*1) |Next16x16[index]<<(7+8*0) |Next16x16[index+16];
        tmp32 = tmp32<<NumMoveLeft;
        DispTmp[index] = tmp32>>(7+8*2);
        DispTmp[index+16] = tmp32>>(7+8*1);
#else
        // tmp32 = current16x16[index]<<(7+8*2) |current16x16[index+16]<<(7+8*1) |Next16x16[index]<<(7+8*0) |Next16x16[index+16];
        tmp32 =current16x16[index]; //BUG 不能用或|
        tmp32<<=8;
        tmp32 |=current16x16[index +16];
        tmp32<<=8;
        tmp32 |=Next16x16[index];
        tmp32<<=8;
        tmp32 |=Next16x16[index +16];


        tmp32 = tmp32<<NumMoveLeft;
        DispTmp[index] = tmp32>>(8+8*2);
        DispTmp[index+16] = tmp32>>(8+8*1);

#endif
    }

    DispRamGreen16x16_shu(DispTmp);
}



void DispRamRed16x16_shuMoveLeft(const uint8* current16x16,const uint8* Next16x16,uint8 NumMoveLeft)
{
//临时显存
    uint8 DispTmp[32]= {0};
    uint8 index=0;
//一行预先23*16里面的显存
//根据左移不同,从32bit里面选取对应的字节数据到显存
    uint32 tmp32=0;

    if (NumMoveLeft>15)
    {
        NumMoveLeft =0;
    }


    for(index=0; index<16; index++)
    {
#if 0
        tmp32 = current16x16[index]<<(7+8*2) |current16x16[index+16]<<(7+8*1) |Next16x16[index]<<(7+8*0) |Next16x16[index+16];
        tmp32 = tmp32<<NumMoveLeft;
        DispTmp[index] = tmp32>>(7+8*2);
        DispTmp[index+16] = tmp32>>(7+8*1);
#else
        // tmp32 = current16x16[index]<<(7+8*2) |current16x16[index+16]<<(7+8*1) |Next16x16[index]<<(7+8*0) |Next16x16[index+16];
        tmp32 =current16x16[index]; //BUG 不能用或|
        tmp32<<=8;
        tmp32 |=current16x16[index +16];
        tmp32<<=8;
        tmp32 |=Next16x16[index];
        tmp32<<=8;
        tmp32 |=Next16x16[index +16];


        tmp32 = tmp32<<NumMoveLeft;
        DispTmp[index] = tmp32>>(8+8*2);
        DispTmp[index+16] = tmp32>>(8+8*1);

#endif
    }

    DispRamRed16x16_shu(DispTmp);
}

/*

int  cmd_disp16x16(int argc, char** argv)
{
    uint32  cnt;

    int i=0;
    rt_kprintf("Get argc=%d  \r\n", argc);

    while(argc>i)

    {
        rt_kprintf("Get   argv is: %s\r\n" , argv[i++]);

    }

    if(argc == 1)  // d
    {
        cnt=65535;

    }
    else if(argc == 2)   //d 100
    {
        sscanf(argv[1], "%d", &cnt);
    }

*/
static uint8 index=0;
static uint16  DelayCnt=0;
static uint8  indexMoveLeft=0;

static uint8 indexHz=0;



static uint8 ledMoveStep=2;

void get_led_move_step()
{
    rt_kprintf("ledMoveStep=%d  \r\n", ledMoveStep);

}
MSH_CMD_EXPORT_ALIAS(get_led_move_step, gm, get_led_move_step.);

void set_led_move_step(int argc, char** argv)
{
    if(argc == 2)   //d 100
    {
        sscanf(argv[1], "%d", &ledMoveStep);
        if((ledMoveStep >16) ||(ledMoveStep <=0) )
        {
            ledMoveStep =2;
        }

    }
    else
    {
        ledMoveStep =2;

    }
    rt_kprintf("ledMoveStep=%d  \r\n", ledMoveStep);

}

MSH_CMD_EXPORT_ALIAS(set_led_move_step, sm, set_led_move_step.);


//GetLedSpeed
static uint8  LedSpeed=5;
void get_led_speed()
{
    rt_kprintf("LedSpeed =%d  \r\n", LedSpeed);

}
MSH_CMD_EXPORT_ALIAS(get_led_speed, gs, get_led_speed.);




void set_led_speed(int argc, char** argv)
{

    if(argc == 2)   //d 100
    {
        sscanf(argv[1], "%d", &LedSpeed);
#if 0
        if(LedSpeed >=30)
        {
            LedSpeed =2;
        }
#endif
    }
    else
    {
        LedSpeed =2;

    }

    //重启
    index=0;
    DelayCnt=0;
    indexMoveLeft=0;

    indexHz=0;

    LedDispCtrl = TM_DISP_CON_UNLOCK;
    RefreshLed16x16Flag = 1;

    LedDispSwitch = _ON;

    rt_kprintf("LedSpeed =%d  \r\n", LedSpeed);

}

MSH_CMD_EXPORT_ALIAS(set_led_speed, ss, set_led_speed.);








void DispConctrl(int argc, char** argv)
{
    index=0;
    DelayCnt=0;
    indexMoveLeft=0;

    indexHz=0;

    LedDispCtrl = TM_DISP_CON_UNLOCK;
    RefreshLed16x16Flag = 1;

    LedDispSwitch = _ON;
    if(argc ==2)
    {
        sscanf(argv[1], "%d", &LedDispSwitch);

        rt_kprintf(" argv[1] = %s\r\n LedDispSwitch= %d \r\n", argv[1] , LedDispSwitch);

        if(LedDispSwitch ==0)
        {
            LedDispCtrl  =TM_DISP_CON_OFF;
        }
        else
        {
            LedDispCtrl  =TM_DISP_CON_UNLOCK;
        }

    }
    else if(argc ==3)
    {

        rt_kprintf(" argv[1] =%s \r\n argv[2] = %s\r\n ", argv[1], argv[2]);

        sscanf(argv[1], " %d", &LedDispSwitch);

        sscanf(argv[2], " %d", &LedDispCtrl);//取低3bit

        rt_kprintf("LedDispSwitch= %d\r\n LedDispCtrl= %d \r\n ", LedDispSwitch ,LedDispCtrl);

        LedDispCtrl = LedDispCtrl &(7);
        LedDispCtrl |=TM_DISP_CON_LOCK;
    }

}

//MSH_CMD_EXPORT_ALIAS(DispConctrl, dc, dc 0 is off dc 1is on restart default .);
MSH_CMD_EXPORT_ALIAS(DispConctrl, dc,dc_1_7_1on_7_brightmax);

#if 0
void DispOff()
{
    index=0;
    DelayCnt=0;
    indexMoveLeft=0;

    indexHz=0;

    LedDispCtrl = TM_DISP_CON_OFF;
    RefreshLed16x16Flag = 1;
    LedDispSwitch = _OFF;

}

MSH_CMD_EXPORT_ALIAS(DispOff, dof, DispOff.);
#endif



//10MS一次
void DispAllshu()
{

    DelayCnt++;
    if(DelayCnt>(LedSpeed))
    {
        DelayCnt=0;
        DispClearAll();
        if(index ==0)
        {
            DispRamGreen16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft);
            indexMoveLeft +=ledMoveStep;//左移步子
            if(indexMoveLeft >=16)
            {
                indexMoveLeft = 0;
                indexHz++;
                if(indexHz >=HZ_NUM_SHU)
                {
                    indexHz = 0;
                    index++;
                }
            }
        }
        else if(index ==1)
        {
            DispRamRed16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft);
            indexMoveLeft +=ledMoveStep;

            if(indexMoveLeft >=16)
            {
                indexMoveLeft = 0;
                indexHz++;
                if(indexHz >=HZ_NUM_SHU)
                {
                    indexHz = 0;
                    index++;
                }
            }
        }
        else if(index ==2)
        {
            DispRamGreen16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft);
            DispRamRed16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft);
            indexMoveLeft +=ledMoveStep;

            if(indexMoveLeft >=16)
            {
                indexMoveLeft = 0;
                indexHz++;
                if(indexHz >=HZ_NUM_SHU)
                {
                    indexHz = 0;
                    index++;
                }
            }
        }
        else if(index ==3)
        {
            DispRamGreen16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft++);
            DispRamRed16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft++);
            if(indexMoveLeft >=16)
            {
                indexMoveLeft = 0;
                indexHz++;
                if(indexHz >=HZ_NUM_SHU)
                {
                    indexHz = 0;
                    index++;
                }
            }
        }
        else if(index ==4)
        {
            DispRamRed16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft++);
            DispRamGreen16x16_shuMoveLeft( hz_shu[indexHz], hz_shu[indexHz+1],indexMoveLeft++);
            if(indexMoveLeft >=16)
            {
                indexMoveLeft = 0;
                indexHz++;
                if(indexHz >=HZ_NUM_SHU)
                {
                    indexHz = 0;
                    index=0;
                }
            }
        }
    }


}




//10MS一次
void DispAllshu1()
{
    static uint8 index=0;
    static uint16  DelayCnt=0;
    DelayCnt++;
    if(DelayCnt>(80/10))
    {

        DelayCnt=0;
        DispClearAll();
        if(index/HZ_NUM_SHU == 0)
        {
            DispRamRed16x16_shu((uint8 *)hz_shu[index++]);
        }
        else if (index/HZ_NUM_SHU == 1)
        {
            DispRamGreen16x16_shu((uint8 *)hz_shu[index%HZ_NUM_SHU]);
            index++;
        }
        else if (index/HZ_NUM_SHU == 2)
        {
            DispRamGreen16x16_shu((uint8 *)hz_shu[index%HZ_NUM_SHU]);
            DispRamRed16x16_shu((uint8 *)hz_shu[index%HZ_NUM_SHU]);
            index++;
        }
        else
        {
            index = 0;
        }


    }


}





/////////////////////////////////////////////////////////////////
/********************************************************************
 函           数:   static void TM1640Delay()
 入口参数:
 返 回     值:
 说            明:  延时,私有
 ***********************************************************************/
#if 0
static void TM1640Delay()
{
    uint8 n=9;
    while(--n);

}
#endif




#if 0
static void TM1640Delay()
{
//   uint8 n=9;
//   while(--n);

}
#endif

#define TM1640Delay()


/********************************************************************
 函           数:   void TM1640Start1()
 入口参数:
 返 回     值:
 说            明:   SCLK 为高,DIN从高变低表示启动
 ***********************************************************************/
#if 1
static void TM1640Start1()
{

    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK1, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    //TM1640Delay();
    //PORT_TM_SCK1=0;

}
#endif

/********************************************************************
 函           数:   void TM1640End1()
 入口参数:
 返 回     值:
 说            明:  SCLK 为高,DIN从低变高表示停止
 ***********************************************************************/
#if 1
static void TM1640End1()
{

    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK1, 0);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK1, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK1, 1);

}
#endif


/********************************************************************
 函           数:   static void TM1640OuteByte1(uint8 dat)
 入口参数:
 返 回     值:
 说            明:   输出1字节命令或数据
 ***********************************************************************/
#if 1
static void TM1640OuteByte1(uint8 dat)
{
    uint8 i=8;
    while(i--)
    {
        digitalWrite(PIN_TM_SCK1, 0);
        //时钟为低电平时允许数据脚改变
        //低电平先发送
        if(dat &1)      digitalWrite(PIN_TM_DIN, 1);
        else digitalWrite(PIN_TM_DIN, 0);
        dat >>=1;
        TM1640Delay();
        digitalWrite(PIN_TM_SCK1, 1);
        TM1640Delay();

    }
}
#endif


void TM1640DisplayProcess1()
{

    uint8 i=0;
#if 0

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        //LedRamMemory.LedBuf[i] =Ledtable[SysCntSecond%10];  //0;  //
        LedBuf1[i] =0;  //Ledtable[SysCntSecond%10];  //
    }


#endif




#if 1
    //TM1640_PORT_INIT();
#endif

#if 1
    TM1640Start1();
    TM1640OuteByte1(TM1640_CMD_SET_DATA);
    TM1640End1();
#endif

#if 1
    //
    TM1640Start1();
    TM1640OuteByte1(TM1640_CMD_SET_ADDR);
    //设置完地址就可以设置数据咯
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
#if (DBG_ALL_DISPLAY==_ON)
#if 1
        if((i%2)==0)
        {
            TM1640OuteByte1(DBG_ALL_DISPLAY_VALUE1);
        }
        else
        {

            {
                TM1640OuteByte1(0);  //DBG_ALL_DISPLAY_VALUE
            }

        }
#else
        TM1640OuteByte1(Ledtable[7]);
#endif
#else
        TM1640OuteByte1(LedBuf1[i]);
#endif
    }
    TM1640End1();
#endif

#if 1
    //
    //
    TM1640Start1();
    TM1640OuteByte1(LedDispCtrl);//TM1640_CMD_DISPLAY
    TM1640End1();
#endif

}




/********************************************************************
 函           数:   void TM1640Start1()
 入口参数:
 返 回     值:
 说            明:   SCLK 为高,DIN从高变低表示启动
 ***********************************************************************/
#if 1
static void TM1640Start2()
{

    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK2, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    //TM1640Delay();
    //PORT_TM_SCK2=0;

}
#endif

/********************************************************************
 函           数:   void TM1640End1()
 入口参数:
 返 回     值:
 说            明:  SCLK 为高,DIN从低变高表示停止
 ***********************************************************************/
#if 1
static void TM1640End2()
{

    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK2, 0);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK2, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK2, 1);

}
#endif


/********************************************************************
 函           数:   static void TM1640OuteByte1(uint8 dat)
 入口参数:
 返 回     值:
 说            明:   输出1字节命令或数据
 ***********************************************************************/
#if 1
static void TM1640OuteByte2(uint8 dat)
{
    uint8 i=8;
    while(i--)
    {
        digitalWrite(PIN_TM_SCK2, 0);
        //时钟为低电平时允许数据脚改变
        //低电平先发送
        if(dat &1)      digitalWrite(PIN_TM_DIN, 1);
        else digitalWrite(PIN_TM_DIN, 0);
        dat >>=1;
        TM1640Delay();
        digitalWrite(PIN_TM_SCK2, 1);
        TM1640Delay();

    }
}
#endif


void TM1640DisplayProcess2()
{


    uint8 i=0;
#if 0

    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        //LedRamMemory.LedBuf[i] =Ledtable[SysCntSecond%10];  //0;  //
        LedBuf2[i] =0;  //Ledtable[SysCntSecond%10];  //
    }
#endif






#if 1
    //TM1640_PORT_INIT();
#endif

#if 1
    TM1640Start2();
    TM1640OuteByte2(TM1640_CMD_SET_DATA);
    TM1640End2();
#endif

#if 1
    //
    TM1640Start2();
    TM1640OuteByte2(TM1640_CMD_SET_ADDR);
    //设置完地址就可以设置数据咯
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
#if (DBG_ALL_DISPLAY==_ON)
#if 1
        if((i%2)==0)
        {
            TM1640OuteByte2(DBG_ALL_DISPLAY_VALUE2);
        }
        else
        {

            {
                TM1640OuteByte2(0);  //DBG_ALL_DISPLAY_VALUE
            }

        }
#else
        TM1640OuteByte1(Ledtable[7]);
#endif
#else
        TM1640OuteByte2(LedBuf2[i]);
#endif
    }
    TM1640End2();
#endif

#if 1
    //
    //
    TM1640Start2();
    TM1640OuteByte2(LedDispCtrl);//TM1640_CMD_DISPLAY
    TM1640End2();
#endif

}




/********************************************************************
 函           数:   void TM1640Start1()
 入口参数:
 返 回     值:
 说            明:   SCLK 为高,DIN从高变低表示启动
 ***********************************************************************/
#if 1
static void TM1640Start3()
{

    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK3, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    //TM1640Delay();
    //PORT_TM_SCK3=0;

}
#endif

/********************************************************************
 函           数:   void TM1640End1()
 入口参数:
 返 回     值:
 说            明:  SCLK 为高,DIN从低变高表示停止
 ***********************************************************************/
#if 1
static void TM1640End3()
{

    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK3, 0);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK3, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK3, 1);

}
#endif


/********************************************************************
 函           数:   static void TM1640OuteByte1(uint8 dat)
 入口参数:
 返 回     值:
 说            明:   输出1字节命令或数据
 ***********************************************************************/
#if 1
static void TM1640OuteByte3(uint8 dat)
{
    uint8 i=8;
    while(i--)
    {
        digitalWrite(PIN_TM_SCK3, 0);
        //时钟为低电平时允许数据脚改变
        //低电平先发送
        if(dat &1)      digitalWrite(PIN_TM_DIN, 1);
        else digitalWrite(PIN_TM_DIN, 0);
        dat >>=1;
        TM1640Delay();
        digitalWrite(PIN_TM_SCK3, 1);
        TM1640Delay();

    }
}
#endif


void TM1640DisplayProcess3()
{
    uint8 i=0;


#if 0
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        //LedRamMemory.LedBuf[i] =Ledtable[SysCntSecond%10];  //0;  //
        LedBuf3[i] =0;  //Ledtable[SysCntSecond%10];  //
    }
#endif






#if 1
    //TM1640_PORT_INIT();
#endif

#if 1
    TM1640Start3();
    TM1640OuteByte3(TM1640_CMD_SET_DATA);
    TM1640End3();
#endif

#if 1
    //
    TM1640Start3();
    TM1640OuteByte3(TM1640_CMD_SET_ADDR);
    //设置完地址就可以设置数据咯
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
#if (DBG_ALL_DISPLAY==_ON)
#if 1
        if((i%2)==0)
        {
            TM1640OuteByte3(DBG_ALL_DISPLAY_VALUE3);
        }
        else
        {

            {
                TM1640OuteByte3(0);  //DBG_ALL_DISPLAY_VALUE
            }

        }
#else
        TM1640OuteByte1(Ledtable[7]);
#endif
#else
        TM1640OuteByte3(LedBuf3[i]);
#endif
    }
    TM1640End3();
#endif

#if 1
    //
    //
    TM1640Start3();
    TM1640OuteByte3(LedDispCtrl);//TM1640_CMD_DISPLAY
    TM1640End3();
#endif

}



/********************************************************************
 函           数:   void TM1640Start1()
 入口参数:
 返 回     值:
 说            明:   SCLK 为高,DIN从高变低表示启动
 ***********************************************************************/
#if 1
static void TM1640Start4()
{

    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK4, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    //TM1640Delay();
    //PORT_TM_SCK4=0;

}
#endif

/********************************************************************
 函           数:   void TM1640End1()
 入口参数:
 返 回     值:
 说            明:  SCLK 为高,DIN从低变高表示停止
 ***********************************************************************/
#if 1
static void TM1640End4()
{

    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK4, 0);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 0);
    digitalWrite(PIN_TM_SCK4, 1);
    TM1640Delay();
    digitalWrite(PIN_TM_DIN, 1);
    digitalWrite(PIN_TM_SCK4, 1);

}
#endif


/********************************************************************
 函           数:   static void TM1640OuteByte1(uint8 dat)
 入口参数:
 返 回     值:
 说            明:   输出1字节命令或数据
 ***********************************************************************/
#if 1
static void TM1640OuteByte4(uint8 dat)
{
    uint8 i=8;
    while(i--)
    {
        digitalWrite(PIN_TM_SCK4, 0);
        //时钟为低电平时允许数据脚改变
        //低电平先发送
        if(dat &1)      digitalWrite(PIN_TM_DIN, 1);
        else digitalWrite(PIN_TM_DIN, 0);
        dat >>=1;
        TM1640Delay();
        digitalWrite(PIN_TM_SCK4, 1);
        TM1640Delay();

    }
}
#endif


void TM1640DisplayProcess4()
{
    uint8 i=0;

#if 0
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
        //LedRamMemory.LedBuf[i] =Ledtable[SysCntSecond%10];  //0;  //
        LedBuf4[i] =0;  //Ledtable[SysCntSecond%10];  //
    }



#endif



#if 1
    //TM1640_PORT_INIT();
#endif

#if 1
    TM1640Start4();
    TM1640OuteByte4(TM1640_CMD_SET_DATA);
    TM1640End4();
#endif

#if 1
    //
    TM1640Start4();
    TM1640OuteByte4(TM1640_CMD_SET_ADDR);
    //设置完地址就可以设置数据咯
    for(i=0; i<MAX_DISP_BUFFER; i++)
    {
#if (DBG_ALL_DISPLAY==_ON)
#if 1
        if((i%2)==1)
        {
            TM1640OuteByte4(DBG_ALL_DISPLAY_VALUE4);
        }
        else
        {

            {
                TM1640OuteByte4(0);  //DBG_ALL_DISPLAY_VALUE
            }

        }
#else
        TM1640OuteByte1(Ledtable[7]);
#endif
#else
        TM1640OuteByte4(LedBuf4[i]);
#endif
    }
    TM1640End4();
#endif

#if 1
    //
    //
    TM1640Start4();
    TM1640OuteByte4(LedDispCtrl);//TM1640_CMD_DISPLAY
    TM1640End4();
#endif

}

#if 0
void CheckLedDark()
{
    uint8 i=0;

    for(i=0; i<16; i++)
    {
        if(LedBuf1[i] != 0)return;
    }

    for(i=0; i<16; i++)
    {
        if(LedBuf2[i] != 0)return;
    }

    for(i=0; i<16; i++)
    {
        if(LedBuf3[i] != 0)return;
    }

    for(i=0; i<16; i++)
    {
        if(LedBuf4[i] != 0)return;
    }

    rt_kprintf(" disp Get Dark   delay starting \r\n");
    rt_thread_delay(510);


}

#endif


int  cmd_disp16x16(int argc, char** argv)
{
    uint32  cnt;

    int i=0;
    rt_kprintf("Get argc=%d  \r\n", argc);

    while(argc>i)

    {
        rt_kprintf("Get   argv is: %s\r\n" , argv[i++]);

    }

    if(argc == 1)  // d
    {
        cnt=65535;

    }
    else if(argc == 2)   //d 100
    {
        sscanf(argv[1], "%d", &cnt);
    }



    Led16x16PortInit();
    rt_kprintf("cmd_disp16x16 starting  cnt = %d \r\n",cnt);


    while(cnt--)//
//   while(1)
    {

#if 1
        //控制显存上的点
#if 0
        DispAllheng();
#else
        DispAllshu();
        rt_thread_delay(3);
        //  DispAllshu1();
#endif

        //  rt_kprintf("disping   cnt =%d\r\n" , cnt++ );

        //CheckLedDark();

        if(RefreshLed16x16Flag)
        {
            RefreshLed16x16Flag = 0;
            //刷显存
            TM1640DisplayProcess1();
            TM1640DisplayProcess2();
            TM1640DisplayProcess3();
            TM1640DisplayProcess4();
        }



#endif

#if 0
        digitalWrite(PIN_TM_DIN, 1);
        digitalWrite(PIN_TM_SCK1, 1);
        digitalWrite(PIN_TM_SCK2, 1);
        digitalWrite(PIN_TM_SCK3, 1);
        digitalWrite(PIN_TM_SCK4, 1);
        rt_kprintf("PIN_TM_SCK1 =1 cnt =%d\r\n" , cnt);

        rt_thread_delay(1*50);
        digitalWrite(PIN_TM_DIN, 0);
        digitalWrite(PIN_TM_SCK1, 0);
        digitalWrite(PIN_TM_SCK2, 0);
        digitalWrite(PIN_TM_SCK3, 0);
        digitalWrite(PIN_TM_SCK4, 0);
        rt_kprintf("PIN_TM_SCK1 =0 cnt =%d\r\n" , cnt);

        rt_thread_delay(1*50);
#endif

    }
}



MSH_CMD_EXPORT_ALIAS(cmd_disp16x16, disp16x16, disp16x16DotPanel.);


MSH_CMD_EXPORT_ALIAS(cmd_disp16x16, d, disp16x16DotPanel.);




#if 1

int   Disp16x16All(void)
{

    Led16x16PortInit();
    rt_kprintf("cmd_disp16x16 starting   \r\n" );


//   while(cnt--)//
    while(1)
    {

        //控制显存上的点
#if 0
        DispAllheng();
#else

#ifdef PIN_TEST
        digitalWrite(PIN_TEST, 1);

#endif


        if(LedDispSwitch)
        {
            DispAllshu();

        }
        //  DispAllshu1();
#endif

        //  rt_kprintf("disping   cnt =%d\r\n" , cnt++ );

        //CheckLedDark();

        if(RefreshLed16x16Flag)
        {
            RefreshLed16x16Flag = 0;
            //刷显存
            TM1640DisplayProcess1();
            TM1640DisplayProcess2();
            TM1640DisplayProcess3();
            TM1640DisplayProcess4();
        }
#ifdef PIN_TEST
        digitalWrite(PIN_TEST, 0);

#endif

        rt_thread_delay(16);




    }
}




#else

int   Disp16x16All(void)
{

    Led16x16PortInit();
    rt_kprintf("cmd_disp16x16 starting   \r\n" );


//   while(cnt--)//
    while(1)
    {

        //控制显存上的点
#if 0
        DispAllheng();
#else

#ifdef PIN_TEST
        digitalWrite(PIN_TEST, 1);

#endif
        rt_thread_delay(16);  //最小单位10ms

#ifdef PIN_TEST
        digitalWrite(PIN_TEST, 0);

#endif

        rt_thread_delay(16);


#endif


    }
}


#endif







































