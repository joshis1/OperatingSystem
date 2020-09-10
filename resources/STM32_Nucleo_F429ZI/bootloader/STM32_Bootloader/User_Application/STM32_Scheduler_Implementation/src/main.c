/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "main.h"
#include "led.h"

// undef __DEBUG for disabling printmsg
//#define __DEBUG

uint8_t current_task = 1;  //Start from TASK1 and not from Idle Task

uint32_t g_tick_count = 0;  // Global Tick Count - maintained.

/** Some random value for the state **/
#define BLOCKING_STATE  0x10
#define READY_STATE   0x40
/*** Ends here -- wants enum rather than Macro **/

typedef struct
{
	uint32_t psp_value;
	uint32_t block_count;
	uint32_t current_state;
    void (*task_handler)(void);
}TCB_t;


TCB_t user_tasks[MAX_TASKS];
//uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};

void Idle_Task(void);      //Idle Task
void task1_handler(void);  //task1
void task2_handler(void);  //task2
void task3_handler(void);  //task3
void task4_handler(void);  //task4

void task_delay(uint32_t tick_count);


void prvSetupUart(void); // uart initialization
void printmsg(char *msg); // uart print msg
void printUrgent_msg(char *msg);

void  save_psp_value(uint32_t current_psp_value);

__attribute__ (( naked )) void switch_sp_to_psp();


void init_systick_timer(uint32_t tick_hz);
__attribute__ (( naked )) void init_scheduler_stack(uint32_t sched_top_of_stack_start);
__attribute__ (( naked )) void PendSV_Handler(void);
void SysTick_Handler(void);

void init_tasks_stack();
void enable_exception_handlers();
void update_global_tick_count();
void unblock_tasks();
void schedule();


int main(void)
{
	/* MSP as stack pointer */
	//enable_exception_handlers();
	RCC_DeInit();
	prvSetupUart();

	printUrgent_msg("User application started\r\n");

//#ifdef __DEBUG
	led_on(GREEN_LED_PIN);
	led_on(BLUE_LED_PIN);
    led_on(RED_LED_PIN);
//#endif

	init_scheduler_stack(SCHED_STACK_START);
	init_tasks_stack();  /*store dummy frames*/

	init_systick_timer(DESIRED_TICK); /* systick */
    /* MSP as Stack Pointer ends here */
	//task1_handler
	led_init();
	switch_sp_to_psp();
	task1_handler(); // now task1 handler will use PSP stack.

	for(;;);

	return 0;
}

//Stack Pointer
//PSP - thread mode
// scheduler -- MSP - handler mode
// scheduler -- Systick handler , PendSv handler.
//user task will use PSP

// Stack assessment

//RAM start -- 128Kb for RAM- SRAM1 + SRAM2
//RAM -- 1kb -- private stack scheduler
//RAM -1 1Kb -- private stack T4
//RAM -1Kb - private stack T3
//RAM - 1k - private stack T2
//RAM - 1kb- private stack T1
//private stack t1 - RAM END
//total stack  5Kb.

//Full descending mode.
// T1 -- stack -- RAM END
//stack grows from top to bottom.
//reserve areas for code.


/** Idle_Task function **/
/** This should run only when there are no other task to run in CPU **/
/** We can use this task to put CPU into the sleep mode or lower power mode **/
void Idle_Task()
{
	printmsg("Idle Task initialized \r\n");
	while(1)
	{

	}
}


/** task1_handler function **/
void task1_handler()
{
	printmsg("Task 1 initialized \r\n");
	while(1)
	{
		led_on(GREEN_LED_PIN);
		task_delay(500);
		led_off(GREEN_LED_PIN);
		task_delay(500);
		printmsg("Task 1 handler \r\n");
	}
}

/** task2_handler function **/

void task2_handler()
{
	printmsg("Task 2 initialized \r\n");
	while(1)
	{
		led_on(RED_LED_PIN);
		task_delay(500);
		led_off(RED_LED_PIN);
		task_delay(500);
		printmsg("Task 2 handler \r\n");
	}
}


/** task3_handler function **/

void task3_handler()
{
	printmsg("Task 3 initialized \r\n");
	while(1)
	{
		led_on(BLUE_LED_PIN);
		task_delay(500);
		led_off(BLUE_LED_PIN);
		task_delay(500);
		printmsg("Task 3 handler \r\n");
	}
}

/** task4_handler function **/

void task4_handler()
{
	printmsg("Task 4 initialized \r\n");
	while(1)
	{
		task_delay(500);
		printmsg("Task 4 handler \r\n");
		task_delay(500);
	}
}

/** prvSetupUart function **/

void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart3_init;

	//GPIO D is connected to AHB1 bus.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Enable the UART 3 peripheral clock -- connected to APB1 bus.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// zero the local variable
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins));
	memset(&uart3_init,0,sizeof(uart3_init));
	// GPIO port D - pin 8 like TX
	// GPIO port D - ping 9 like RX
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF; //AF - Alternate function i.e. TX and RX.
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP; //Pull up so that we see some default voltage.

	GPIO_Init(GPIOD, &gpio_uart_pins);

	// set the Pin 8 and Pin 9 to AF 7 i.e. GPIO_AF_USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8 , GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9 , GPIO_AF_USART3);

	uart3_init.USART_BaudRate = 115200;
	uart3_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart3_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart3_init.USART_Parity = USART_Parity_No;
	uart3_init.USART_StopBits = USART_StopBits_1;
	uart3_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3,&uart3_init);
	//Enable USART 3 peripheral.
	//Enable USART 3 interrupt

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	// lets set the priority in NVIC for the UART 3 interrupt.
	// we cannot set priority 4 or below since we are calling FreeRTOS API from there.
	NVIC_SetPriority(USART3_IRQn,6);
	NVIC_EnableIRQ(USART3_IRQn);

	USART_Cmd(USART3, ENABLE);

}

/** printmsg function **/
void printUrgent_msg(char *msg)
{
	for(uint32_t i = 0; i < strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
		USART_SendData(USART3, msg[i]);
	}
}

/** printmsg function **/
void printmsg(char *msg)
{
#ifdef __DEBUG
	for(uint32_t i = 0; i < strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
		USART_SendData(USART3, msg[i]);
	}
#else
	(void)(msg);
#endif
}

//Scheduling policy --
// round robin pre-emptive scheduling.
// No task priority
// use SysTick timer to generate
// exception for every 1 ms to run the scheduler code.
//T1 -- launch manually
//after 1 ms .. switch out T1 and switch in T2.

// Scheduling -- algorithm -
//which task should run in the CPU.
// decision -- such as system load, the priority of tasks,
// shared resource access or simple round robin.

// execution context or state of a task.
// Processor --Processor Core.
//State of a task -- General purpose register, ALU,
// Status register and special registers.
//T1 State-> General Purpose registers, Status register and special registers.
//Core register - R0 to R12 -- 13 General purpose register
//PC -- Program Counter - R15
//LR - R14
//SP - R13
//SP - R13 - PSP and MSP
// Program Status register - PSR
//The below are previliged state registers.
//User task will not touch the below registers.
//PRIMask, FaultMaks, BASEPRI, Control register -- special register.
//T1 - switching out and T2- switching in.
//Save the context of T1 to T1's private stack - PUSH - context saving
//Save the PSP value of T1 // PUSH i..e context saving

//Get current PSP value of T2  // POP - context retreiving
// Retrieve the context of T2's private stack // POP - context retreiving.
// Run T2.

//thread mode using PSP
// handler mode using MSP.
// Exception handler -- change the PSP value
// Stack frame - processor already saves the state of a task.
//processor already saves Ro to R3 and LR, PC, R12, PSR.
// part of exception entry..
//R4 to R11 needs to be saved -manually.

/**SysTick Count Value calculation **/
//Processor clock = 16Mhz.
// SysTick timer count clock = 16Mhz.
// 1ms is 1Khz in frequency domain.
// to bring down systick timer count clock from 16Mhz to 1kHZ
// use a divisor
// reload value = 16000
// 16MHz/ 16000 - output will be 1000 Hz. i.e. TICK HZ.
//1000 Hz means 1 milliseconds.
//16Mhz is derived from HSI
// 1ms delay is 16000 counts

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz) - 1;
	//generic user guide
	//Sytick Reload Value register.
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSystCSR = (uint32_t*)0xE000E010;
	/** 0 to 23 bit values **/
	//clear the vlaue of SVR
	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count_value;
	//enable the systick.

	//SysTick SYST_CSR register bit.
	//TICKINT, ClockSource, enable.bit
	*pSystCSR |= (1 << 1);
	*pSystCSR |= ( 1 << 2);
	*pSystCSR |= ( 1 << 0);
}

void save_psp_value(uint32_t current_psp_value)
{
	user_tasks[current_task].psp_value = current_psp_value;
}

void update_next_task()
{
	int state = BLOCKING_STATE;
	for(int i = 0; i < MAX_TASKS; i++)
	{
		current_task++;
		current_task =  (current_task % MAX_TASKS);
		state = user_tasks[current_task].current_state;

		//current_task != 0 means not and Idle task.
		if( (state == READY_STATE) && current_task != 0 )
		{
			break;
		}
	}

	//if all tasks are blocked then schedule the idle task
	if(state == BLOCKING_STATE)
	{
		current_task = 0;
	}
}

void schedule()
{
	uint32_t *pICSR = (uint32_t*)0xE000ED04;
	*pICSR |= (0x1 << 28); // Pend the PendSv handler.
}


//This will do our context switching.
__attribute__ (( naked )) void PendSV_Handler(void)
{
	// this is using MSP
	// exception handler.

	//save the context of current task.

	//1.  Get current running task's psp value.
	__asm volatile("MRS R0, PSP");
	// 2. using the psp value store SF2 ( R4 to R11)
	//store the register value to memory.
	//STMDB -- store multiple registers, decrement before.
	__asm volatile("STMDB R0!, {R4-R11}"); // RO will get updated

	//save LR value here

	__asm volatile("PUSH {LR}");
	// 3. Save the current value of PSP.
	__asm volatile("BL save_psp_value");



	//Retreive the context of next task **/
	//1. Decide the next task to run.
	__asm volatile("BL update_next_task");
	//get its past PSP value
	__asm volatile("BL get_psp_value");
	//using the psp value retrieve SF2 (R4 to R11)
	// memory to register
	__asm volatile("LDMIA R0!, {R4-R11}"); //load multiple register and increment
	// update PSP and exit.
	__asm volatile("MSR PSP, R0"); //psp is pointing to new task stack

	// Store back LR
	__asm volatile("POP {LR}");

	__asm volatile("BX LR");

}


void SysTick_Handler(void)
{
	uint32_t *pICSR = (uint32_t*)0xE000ED04;

	update_global_tick_count();
	unblock_tasks();

	*pICSR |= (0x1 << 28); // Pend the PendSv handler.

}

__attribute__ (( naked )) void init_scheduler_stack(uint32_t sched_top_of_stack_start)
{
   __asm volatile("MSR MSP, %0": : "r"(sched_top_of_stack_start));  // store sched top of stack in MSP - main stack pointer.
   __asm volatile("BX LR"); // BX is branch indirect to LR.
}
/** Stack organisation **/
/** <Stack END ----> Stack Start>
 * <R4, R5, R6,R7,R8,R9,R10, R11 - Stack frame 2 - manually needs to store
 * --> continue -- R0,R1, R2, R3, R12, LR, PC, XPSR - Stack Frame 1- automatically stored by the processor
 */
/** Top of Stack --------------------Start of Stack */
/* R4, R5, R6,R7,R8,R9,R10, R11  -- R0,R1, R2, R3, R12, LR, PC, XPSR*/
/** During initialization of stack - ensure xPSR has T-bit set i.e thumb mode
 * only thumb mode is supported by ARM cortex M4
 * LR - special exception value i.e. to use PSP as a SP and not MSP as a SP.
 * before you set the LR, you could initialize the PSP value by MOV instruction
 * PC - is the program counter where the control should go
 * For e.g. initially the control will go to the task2, task3, task4, task1 - function address
 */

void init_tasks_stack()
{
	uint32_t *pPSP;
	//initialize task handlers
	user_tasks[0].task_handler = (uint32_t)Idle_Task;
	user_tasks[1].task_handler = (uint32_t)task1_handler;
	user_tasks[2].task_handler = (uint32_t)task2_handler;
	user_tasks[3].task_handler = (uint32_t)task3_handler;
	user_tasks[4].task_handler = (uint32_t)task4_handler;

	user_tasks[0].current_state = READY_STATE;
	user_tasks[1].current_state = READY_STATE;
	user_tasks[2].current_state = READY_STATE;
	user_tasks[3].current_state = READY_STATE;
	user_tasks[4].current_state = READY_STATE;


	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
    user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;


	for(int i = 0; i < MAX_TASKS; i++)
	{
		pPSP = (uint32_t *)user_tasks[i].psp_value;
		pPSP--;
		*pPSP = DUMMY_XPSR;  //XPSR
		pPSP--; // PC
		*pPSP = (uint32_t)user_tasks[i].task_handler;
		pPSP--; //LR
		//This 0xFFFFFFFD will make the processor to use PSP as SP rather than MSP.
		*pPSP =  0xFFFFFFFD; //Table 2-17 Exception return behavior  - ARM Cortex M4 processor manual - return to PSP rather than MSP

		for(int j = 0; j < 13; j++)
		{
			pPSP--;
			*pPSP = 0; // R12 to R4 are all zeroes. basically it comprises of R0 to R12
		}

		user_tasks[i].psp_value = (uint32_t)pPSP; // top of Stack i.e. the updated PSP.
	}
}

uint32_t get_psp_value()
{
	return user_tasks[current_task].psp_value;
}

__attribute__ (( naked )) void switch_sp_to_psp()
{
   //1. Initialize the PSP with Task1 Stack start address.
	//get the value of psp of current task.
	// save LR
	__asm volatile ("PUSH {LR}");
	__asm volatile("BL get_psp_value"); //branch with link
	//return value is stored in RO - according to the procedure
	// doc - for ARM

	__asm volatile("MSR PSP, R0"); //initilize PSP
	__asm volatile("POP {LR}"); // pops back LR.

	// change MSP to PSP.
	//

   //2. Change SP to PSP using Control register.
	__asm volatile("MOV R0, #0x02"); // 2nd bit as - immediate value
	__asm volatile("MSR CONTROL, R0"); // push this R0 value to CONTROL register.
	__asm volatile("BX LR"); // this connects back to the main function.
}

void task_delay(uint32_t tick_count)
{
	INTERRUPT_DISABLE();
	if(current_task != 0) // not Idle task
	{
		user_tasks[current_task].current_state = BLOCKING_STATE;
		user_tasks[current_task].block_count = g_tick_count + tick_count;
		schedule();
	}
	INTERRUPT_ENABLE();
}

void update_global_tick_count()
{
	g_tick_count++;
}

void unblock_tasks()
{
	for(int i = 1; i < MAX_TASKS; i++)
	{
		if( user_tasks[i].current_state  != READY_STATE)
		{
			if(user_tasks[i].block_count == g_tick_count)
			{
				user_tasks[i].current_state = READY_STATE;
			}
		}
	}
}



void enable_exception_handlers()
{
	uint32_t *pAuxCR = (uint32_t*)0xE000E008;  //Auxiliary control register
	uint32_t *pSHCRS = (uint32_t *) 0xE000ED24; //system handler control and state register
	//bit 18 usage fault, bit 17 bus fault, bit 16 memory manage fault
	//Table 4-24 SHCSR bit assignments
	*pSHCRS |= ((0x1U <<18) | (0x1U << 17) | (0x1U << 16 ));
	//precise fault - bit 1 -- Auxiliary control register
	*pAuxCR |= (0x1U << 1);
}

void MemManage_Handler()
{
	while(1)
	{
		printmsg("MemManage_Handler Fault \r\n");
	}
}

void BusFault_Handler()
{
	while(1)
	{
		printmsg("BusFault_Handler Fault \r\n");
	}

}

void UsageFault_Handler()
{
	while(1)
	{
		printmsg("UsageFault_Handler Fault \r\n");
	}
}

void HardFault_Handler()
{
	while(1)
	{
		printmsg("HardFault_Handler Fault \r\n");
	}
}


