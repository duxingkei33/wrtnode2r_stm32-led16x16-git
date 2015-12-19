/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2015 WRTnode.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file   wirish/boards/wrtnode2/include/board/board.h
 * @author Schumy Hao <schumy@wrtnode.com>
 * @brief  wrtnode2 board header.
 */

#ifndef _BOARD_WRTNODE2_H_
#define _BOARD_WRTNODE2_H_

/* 72 MHz -> 72 cycles per microsecond. */
#define CYCLES_PER_MICROSECOND  72

/* Pin number for the built-in button. 
#define BOARD_BUTTON_PIN        38*/

/* Pin number for the built-in LED. 
#define BOARD_LED_PIN           13*/

/* Number of USARTs/UARTs whose pins are broken out to headers. */
#define BOARD_NR_USARTS         2

/* USART pin numbers. */
#define BOARD_USART1_TX_PIN     13
#define BOARD_USART1_RX_PIN     14
#define BOARD_USART2_TX_PIN     10
#define BOARD_USART2_RX_PIN     11

/* Number of SPI ports broken out to headers. */
#define BOARD_NR_SPI            1

/* SPI pin numbers. */
#define BOARD_SPI1_NSS_PIN      20
#define BOARD_SPI1_MOSI_PIN     23
#define BOARD_SPI1_MISO_PIN     22
#define BOARD_SPI1_SCK_PIN      21

/* Total number of GPIO pins that are broken out to headers and
 * intended for use. This includes pins like the LED, button, and
 * debug port (JTAG/SWD) pins. */
#define BOARD_NR_GPIO_PINS      24

/* Number of pins capable of PWM output. */
#define BOARD_NR_PWM_PINS       12

/* Number of pins capable of ADC conversion. */
#define BOARD_NR_ADC_PINS       6

/* Number of pins already connected to external hardware.  For Maple,
 * these are just BOARD_LED_PIN, BOARD_BUTTON_PIN, and the debug port
 * pins (see below). */
#define BOARD_NR_USED_PINS       0

/* Debug port pins. 
#define BOARD_JTMS_SWDIO_PIN    39
#define BOARD_JTCK_SWCLK_PIN    40
#define BOARD_JTDI_PIN          41
#define BOARD_JTDO_PIN          42
#define BOARD_NJTRST_PIN        43*/

/* USB configuration.  BOARD_USB_DISC_DEV is the GPIO port containing
 * the USB_DISC pin, and BOARD_USB_DISC_BIT is that pin's bit. 
#define BOARD_USB_DISC_DEV      GPIOC
#define BOARD_USB_DISC_BIT      12*/

/* Pin aliases: these give the GPIO port/bit for each pin as an
 * enum. These are optional, but recommended. They make it easier to
 * write code using low-level GPIO functionality. */
enum {
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, 
    PA0, PA1, PA2, PA3, PA8, PA9, PB10, PA11,
    PA12, PA13, PA14, PA15, PA4, PA5, PA6, PA7,
};

#endif
