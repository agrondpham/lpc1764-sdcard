/****************************************************************************
 *   $Id:: timer.c 5823 2010-12-07 19:01:00Z usb00423                       $
 *   Project: NXP LPC17xx Timer for PWM example
 *
 *   Description:
 *     This file contains timer code example which include timer 
 *     initialization, timer interrupt handler, and APIs for timer access.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc17xx.h"
#include "../lpc_types.h"
#include "timer.h"

volatile uint32_t timer0_m0_counter = 0;
volatile uint32_t timer1_m0_counter = 0;
volatile uint32_t timer2_m0_counter = 0;
volatile uint32_t timer3_m0_counter = 0;
volatile uint32_t timer0_m1_counter = 0;
volatile uint32_t timer1_m1_counter = 0;
volatile uint32_t timer2_m1_counter = 0;
volatile uint32_t timer3_m1_counter = 0;

volatile uint32_t timer0_capture0 = 0;
volatile uint32_t timer1_capture0 = 0;
volatile uint32_t timer2_capture0 = 0;
volatile uint32_t timer3_capture0 = 0;
volatile uint32_t timer0_capture1 = 0;
volatile uint32_t timer1_capture1 = 0;
volatile uint32_t timer2_capture1 = 0;
volatile uint32_t timer3_capture1 = 0;

volatile uint32_t timer0_counter = 0;
volatile uint32_t timer1_counter = 0;
volatile uint32_t timer2_counter = 0;
volatile uint32_t timer3_counter = 0;


/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0x02;		/* reset timer */
	LPC_TIM0->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = (SystemCoreClock / 4) / (1000/delayInMs);  //enter delay time
	LPC_TIM0->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM0->MCR = 0x04;		/* stop timer on match */
	LPC_TIM0->TCR = 0x01;		/* start timer */
  
	/* wait until delay time has elapsed */
	while (LPC_TIM0->TCR & 0x01);

  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 0x02;		/* reset timer */
	LPC_TIM1->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = (SystemCoreClock / 4) / (1000/delayInMs);  //enter delay time
	LPC_TIM1->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM1->MCR = 0x04;		/* stop timer on match */
	LPC_TIM1->TCR = 0x01;		/* start timer */
  
	/* wait until delay time has elapsed */
	while (LPC_TIM1->TCR & 0x01);
  }

  else if ( timer_num == 2 )
    {
  	LPC_TIM2->TCR = 0x02;		/* reset timer */
  	LPC_TIM2->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = (SystemCoreClock / 4) / (1000/delayInMs);  //enter delay time
  	LPC_TIM2->IR  = 0xff;		/* reset all interrrupts */
  	LPC_TIM2->MCR = 0x04;		/* stop timer on match */
  	LPC_TIM2->TCR = 0x01;		/* start timer */

  	/* wait until delay time has elapsed */
  	while (LPC_TIM2->TCR & 0x01);
    }

  else if ( timer_num == 3 )
    {
  	LPC_TIM3->TCR = 0x02;		/* reset timer */
  	LPC_TIM3->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = (SystemCoreClock / 4) / (1000/delayInMs);  //enter delay time
  	LPC_TIM3->IR  = 0xff;		/* reset all interrrupts */
  	LPC_TIM3->MCR = 0x04;		/* stop timer on match */
  	LPC_TIM3->TCR = 0x01;		/* start timer */

  	/* wait until delay time has elapsed */
  	while (LPC_TIM3->TCR & 0x01);
    }
  return;
}

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_interrupt (void)
{  
  if ( LPC_TIM0->IR & (0x1<<0) )
  {
    LPC_TIM0->IR = 0x1<<0;		/* clear interrupt flag */
    timer0_m0_counter++;
  }
  if ( LPC_TIM0->IR & (0x1<<1) )
  {
    LPC_TIM0->IR = 0x1<<1;		/* clear interrupt flag */
    timer0_m1_counter++;
  }
  if ( LPC_TIM0->IR & (0x1<<4) )
  {
	LPC_TIM0->IR = 0x1<<4;		/* clear interrupt flag */
	timer0_capture0++;
  }
  if ( LPC_TIM0->IR & (0x1<<5) )
  {
	LPC_TIM0->IR = 0x1<<5;		/* clear interrupt flag */
	timer0_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER1_interrupt (void)
{  
  if ( LPC_TIM1->IR & (0x1<<0) )
  {
    LPC_TIM1->IR = 0x1<<0;		/* clear interrupt flag */
    timer1_m0_counter++;
  }
  if ( LPC_TIM1->IR & (0x1<<1) )
  {
    LPC_TIM1->IR = 0x1<<1;		/* clear interrupt flag */
    timer1_m1_counter++;
  }
  if ( LPC_TIM1->IR & (0x1<<4) )
  {
	LPC_TIM1->IR = 0x1<<4;		/* clear interrupt flag */
	timer1_capture0++;
  }
  if ( LPC_TIM1->IR & (0x1<<5) )
  {
	LPC_TIM1->IR = 0x1<<5;		/* clear interrupt flag */
	timer1_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_interrupt (void)
{
  if ( LPC_TIM2->IR & (0x1<<0) )
  {
    LPC_TIM2->IR = 0x1<<0;		/* clear interrupt flag */
    timer2_m0_counter++;
  }
  if ( LPC_TIM2->IR & (0x1<<1) )
  {
    LPC_TIM2->IR = 0x1<<1;		/* clear interrupt flag */
    timer2_m1_counter++;
  }
  if ( LPC_TIM2->IR & (0x1<<4) )
  {
	LPC_TIM2->IR = 0x1<<4;		/* clear interrupt flag */
	timer2_capture0++;
  }
  if ( LPC_TIM2->IR & (0x1<<5) )
  {
	LPC_TIM2->IR = 0x1<<5;		/* clear interrupt flag */
	timer2_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_interrupt (void)
{
  if ( LPC_TIM3->IR & (0x1<<0) )
  {
    LPC_TIM3->IR = 0x1<<0;		/* clear interrupt flag */
    timer3_m0_counter++;
  }
  if ( LPC_TIM3->IR & (0x1<<1) )
  {
    LPC_TIM3->IR = 0x1<<1;		/* clear interrupt flag */
    timer3_m1_counter++;
  }
  if ( LPC_TIM3->IR & (0x1<<4) )
  {
	LPC_TIM3->IR = 0x1<<4;		/* clear interrupt flag */
	timer3_capture0++;
  }
  if ( LPC_TIM3->IR & (0x1<<5) )
  {
	LPC_TIM3->IR = 0x1<<5;		/* clear interrupt flag */
	timer3_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 1;
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 1;
  }
  else if ( timer_num == 2 )
  {
	LPC_TIM2->TCR = 1;
  }
  else if ( timer_num == 3 )
  {
	LPC_TIM3->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1 oe 2 or 3
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0;
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 0;
  }
  else if ( timer_num == 2 )
  {
	LPC_TIM2->TCR = 0;
  }
  else if ( timer_num == 3 )
  {
	LPC_TIM2->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
** 
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
	regVal = LPC_TIM0->TCR;
	regVal |= 0x02;
	LPC_TIM0->TCR = regVal;
  }
  else if ( timer_num == 1 )
  {
	regVal = LPC_TIM1->TCR;
	regVal |= 0x02;
	LPC_TIM1->TCR = regVal;
  }
  else if ( timer_num == 2 )
  {
	regVal = LPC_TIM2->TCR;
	regVal |= 0x02;
	LPC_TIM2->TCR = regVal;
  }
  else if ( timer_num == 3 )
  {
	regVal = LPC_TIM3->TCR;
	regVal |= 0x02;
	LPC_TIM3->TCR = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
uint32_t TimerInit( uint8_t timer_num, uint32_t TimerInterval )
{
 if ( timer_num == 0 )
	{
		timer0_counter = 0;
		LPC_TIM0->MR0 = TimerInterval;
		LPC_TIM0->MCR = 3;				/* Interrupt and Reset on MR0 */
		NVIC_EnableIRQ(TIMER0_IRQn);
	}
	else if ( timer_num == 1 )
	{
		timer1_counter = 0;
		LPC_TIM1->MR0 = TimerInterval;
		LPC_TIM1->MCR = 3;				/* Interrupt and Reset on MR1 */
		NVIC_EnableIRQ(TIMER1_IRQn);
	}
	else if ( timer_num == 2 )
	{
		timer2_counter = 0;
		LPC_TIM2->MR0 = TimerInterval;
		LPC_TIM2->MCR = 3;				/* Interrupt and Reset on MR1 */
		NVIC_EnableIRQ(TIMER2_IRQn);
	}
	else if ( timer_num == 3 )
	{
		timer3_counter = 0;
		LPC_TIM3->MR0 = TimerInterval;
		LPC_TIM3->MCR = 3;				/* Interrupt and Reset on MR1 */
		NVIC_EnableIRQ(TIMER3_IRQn);
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
