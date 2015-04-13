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
 * - in particular, to test priority scheduler
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
  chprintf(chp, "Thread 1, priority %d: E\r\n", chThdGetPriority());
  return 0;
}

static msg_t thread2(BaseSequentialStream *chp) {
  chprintf(chp, "Thread 2, priority %d: D\r\n", chThdGetPriority());
  return 0;
}

static msg_t thread3(BaseSequentialStream *chp) {
  chprintf(chp, "Thread 3, priority %d: A\r\n", chThdGetPriority());
  return 0;
}

static msg_t thread4(BaseSequentialStream *chp) {
  chprintf(chp, "Thread 4, priority %d: B\r\n", chThdGetPriority());
  return 0;
}

static msg_t thread5(BaseSequentialStream *chp) {
  chprintf(chp, "Thread 5, priority %d: C\r\n", chThdGetPriority());
  return 0;
}

static void cmd_priority(BaseSequentialStream *chp) {

  chprintf(chp, "Creating 5 threads: \r\n");
  chprintf(chp, "------------------------------------------ \r\n");

  /* 
   * Creating dynamic threads using the heap allocator
  */
  Thread *tp1 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread1, chp);
  Thread *tp2 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread2, chp);
  Thread *tp3 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread3, chp);
  Thread *tp4 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread4, chp);
  Thread *tp5 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread5, chp);

  /*
   * Now wait for the spawned thread to terminate (if it has not terminated
   * already) then gets the thread exit message (msg) and returns the
   * terminated thread memory to the heap.
   */
  msg_t msg = chThdWait(tp1);
  msg = chThdWait(tp2);
  msg = chThdWait(tp3);
  msg = chThdWait(tp4);
  msg = chThdWait(tp5);
}

static const ShellCommand commands[] = {
		{"mem", cmd_mem},
		{"threads", cmd_threads},
		{"priority", cmd_priority},
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
