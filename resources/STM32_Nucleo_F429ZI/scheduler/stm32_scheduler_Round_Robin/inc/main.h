/*
 * main.h
 *
 *  Created on: 19Aug.,2020
 *      Author: shreyas.joshi
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

void task1_handler(void);  //task1
void task2_handler(void);  //task2
void task3_handler(void);  //task3
void task4_handler(void);  //task4

void prvSetupUart(void); // uart initialization
void printmsg(char *msg); // uart print msg

void  save_psp_value(uint32_t current_psp_value);

__attribute__ (( naked )) void switch_sp_to_psp();


void init_systick_timer(uint32_t tick_hz);
__attribute__ (( naked )) void init_scheduler_stack(uint32_t sched_top_of_stack_start);

void init_tasks_stack();


void enable_exception_handlers();

/** Task - Stack calculation Macros **/
#define SIZE_TASK_STACK                (1024U)
#define SIZE_SCHED_STACK               (1024U)
#define SRAM_START                     (0x20000000U)
#define SIZE_SRAM                      (128 *1024) //bytes

#define SRAM_END                       (SRAM_START + SIZE_SRAM)
//full descending Stack i.e. RAM starts from END and grows downwards.
#define T1_STACK_START                 (SRAM_END)
#define T2_STACK_START                 ((SRAM_END)  - (1 *SIZE_TASK_STACK))
#define T3_STACK_START                 ((SRAM_END)  - (2 *SIZE_TASK_STACK))
#define T4_STACK_START                 ((SRAM_END)  - (3 *SIZE_TASK_STACK))
#define IDLE_STACK_START               ((SRAM_END)  - (4 *SIZE_TASK_STACK))
#define SCHED_STACK_START              ((SRAM_END)  - (5 *SIZE_TASK_STACK))

/** Task - Stack calculation Ends here **/

#define HSI_CLOCK (16000000U)
#define DESIRED_TICK  (1000U)
#define SYSTICK_TIM_CLK HSI_CLOCK


#define MAX_TASKS (5)

#define DUMMY_XPSR   (0x1 << 24)  //should have T-BIT enable. i.e. 24th bit

#define INTERRUPT_ENABLE() do { __asm volatile("MOV R0, #0x0"); \
		__asm volatile("MSR PRIMASK, R0"); \
                            } while(0)

#define INTERRUPT_DISABLE() do { __asm volatile("MOV R0, #0x1"); \
		__asm volatile("MSR PRIMASK, R0"); \
                            } while(0)

#endif /* INC_MAIN_H_ */
