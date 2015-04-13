#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "USB.h"
#include "shell.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
 * Basic ChibiOS shell, with some extra commands 
 * - in particular, to learn rate monotonic scheduling
 * Based on code by https://github.com/vedderb
 *
*/


#define SHELL_WA_SIZE   THD_WA_SIZE(512)
#define WA_SIZE THD_WA_SIZE(512)


static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
	size_t n, size;

	(void)argv;
	if (argc > 0) {
		chprintf(chp, "Usage: mem\r\n");
		return;
	}
	n = chHeapStatus(NULL, &size);
	chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
	chprintf(chp, "heap fragments   : %u\r\n", n);
	chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
	static const char *states[] = {THD_STATE_NAMES};
	Thread *tp;

	(void)argv;
	if (argc > 0) {
		chprintf(chp, "Usage: threads\r\n");
		return;
	}
	chprintf(chp, "    addr    stack prio refs     state           name time    \r\n");
	chprintf(chp, "-------------------------------------------------------------\r\n");
	tp = chRegFirstThread();
	do {
		chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %14s %lu\r\n",
				(uint32_t)tp, (uint32_t)tp->p_ctx.r13,
				(uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
				states[tp->p_state], tp->p_name, (uint32_t)tp->p_time);
		tp = chRegNextThread(tp);
	} while (tp != NULL);
}

static msg_t thread1(BaseSequentialStream *chp) {
  systime_t time = chTimeNow();    
  /* 
   * Thread-specific parameters
  */
  chRegSetThreadName("t1");
  static int d = 300;
  static int w = 75000;
  volatile uint32_t i, n;
  while(1) {
  if(chThdShouldTerminate())
    return 0;
  palSetPad(GPIOD, GPIOD_LED3);  
  /* 
   * Deadline for current execution of this task
  */ 
  time += d;            
  chprintf(chp, "%s  N  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  /* 
   * Do some "work"
  */
  for(i = 0; i < w; i ++) { 
    n = i / 3;
  }
  chprintf(chp, "%s  X  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  palClearPad(GPIOD, GPIOD_LED3);   
  /* 
   * Yield control of CPU until the deadline (which is also beginning of next period)
  */
  chThdSleepUntil(time);
  } 
  return 0;
}

static msg_t thread2(BaseSequentialStream *chp) {
  systime_t time = chTimeNow();     
  static int d = 170;
  static int w = 40000;
  chRegSetThreadName("t2");
  volatile uint32_t i, n;
  while(1) {
  if(chThdShouldTerminate())
    return 0;
  palSetPad(GPIOD, GPIOD_LED4);      
  time += d;            
  chprintf(chp, "%s  N  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  for(i = 0; i < w; i ++) { 
    n = i / 3;
  }
  chprintf(chp, "%s  X  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  palClearPad(GPIOD, GPIOD_LED4);      
  chThdSleepUntil(time);
  } 
  return 0;
}

static msg_t thread3(BaseSequentialStream *chp) {
  systime_t time = chTimeNow();     
  chRegSetThreadName("t3");
  static int d = 280;
  static int w = 60000;
  volatile uint32_t i, n;
  while(1) {
  if(chThdShouldTerminate())
    return 0;
  palSetPad(GPIOD, GPIOD_LED5);      
  time += d;            
  chprintf(chp, "%s  N  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  for(i = 0; i < w; i ++) { 
    n = i / 3;
  }
  chprintf(chp, "%s  X  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  palClearPad(GPIOD, GPIOD_LED5);      
  chThdSleepUntil(time);
  } 
  return 0;
}

static msg_t thread4(BaseSequentialStream *chp) {
  systime_t time = chTimeNow();     
  chRegSetThreadName("t4");
  static int d = 380;
  static int w = 25000;
  volatile uint32_t i, n;
  while(1) {
  if(chThdShouldTerminate())
    return 0;
  palSetPad(GPIOD, GPIOD_LED6);      
  time += d;            
  chprintf(chp, "%s  N  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  for(i = 0; i < w; i ++) { 
    n = i / 3;
  }
  chprintf(chp, "%s  X  %d   %d   %d  %d\r\n",  chRegGetThreadName(chThdSelf()), chThdGetPriority(),
						chTimeNow(), time, chThdGetTicks(chThdSelf()));
  palClearPad(GPIOD, GPIOD_LED6);      
  chThdSleepUntil(time);
  } 
  return 0;
}



static void cmd_rms(BaseSequentialStream *chp) {

  /* 
   * Creating dynamic threads using the heap allocator
  */
  Thread *tp1 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread1, chp);
  Thread *tp2 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread2, chp);
  Thread *tp3 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread3, chp);
  Thread *tp4 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread4, chp);


  chThdSleepUntil(chTimeNow() + MS2ST(500));

  /*
   * Try to kill threads
  */
  chThdTerminate(tp1);
  chThdTerminate(tp2);
  chThdTerminate(tp3);
  chThdTerminate(tp4);

  /*
   * Wait for the thread to terminate (if it has not terminated
   * already) then get the thread exit message (msg) and returns the
   * terminated thread memory to the heap.
   */
  msg_t msg = chThdWait(tp1);
  msg = chThdWait(tp2);
  msg = chThdWait(tp3);
  msg = chThdWait(tp4);
}

static const ShellCommand commands[] = {
		{"mem", cmd_mem},
		{"threads", cmd_threads},
		{"rms", cmd_rms},
		{NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
		(BaseSequentialStream *)&SDU1,
		commands
};


int main(void) {
	halInit();
	chSysInit();
	USBinit();

	palSetPadMode(GPIOD, GPIOD_LED3, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_OUTPUT_PUSHPULL);

	static Thread *shelltp = NULL;

	while(1) {
		if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
			shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
		else if (chThdTerminated(shelltp)) {
			chThdRelease(shelltp);
			shelltp = NULL;
		}

		chThdSleepMilliseconds(100);
	}
}
