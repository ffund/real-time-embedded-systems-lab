Lab 6: RTOS
====================

Lab Assignment | 6 - RTOS
-------------- | -------------
Name           |
Net ID         |
Report due     | Thursday, 23 April


Please answer the following questions:

1) In the previous lab, we learned about debouncing inputs that may "bounce,"
such as buttons. In this lab, we light LEDs according to the status of a button input. 
Do we need to debounce the button input in this lab? Why or why not? If your
answer is "yes," explain what user-perceivable problem you would solve
by debouncing button input in this lab. If your answer is "no," explain what's different
about this lab and the previous lab, in which we demonstrated user-perceivable problems
due to failure to debounce button input.


2) Show the backtrace of the main thread while inside the `Blinky()` function, 
using thread debugging in `gdb`.


3) In lab 2, I asked you to check the size of your ELF file
in two cases: one without the peripheral library and one with the peripheral
library. What is the size of your ELF file for the RTOS? Use `size build/rtos-blinky.elf` 
to get its size, then use your work from lab 2 to fill out the following table:

Implementation                    | ELF size (hex)
--------------------------------- | -------------
Lab 2 - GPIO with no libraries    |
Lab 2 - with peripheral libraries |
Lab 6 - with RTOS                 | 



4) The ChibiOS build system lets us enable and disable parts of the OS and
device drivers on an individual basis. Of course, there are dependencies 
between the OS kernel and device subsystems. As an example, try enabling 
PWM in `halconf.h` in the `rtos-blinky` project. Then try to build the project with `make`. 
What error message do you get? Show this error message here. Then,
identify the line in `mcuconf.h` that you need to change in order 
to fix this error and successfully build the OS with PWM support. Explain.

5) List the commands available in the ChibiOS shell in `rtos-shell`, and describe
what each one appears to do.

6) Show the output of the `priority` command with the letters A-E in the correct order:

```

```

Also show the modifications you made to these five lines of code:

```
  Thread *tp1 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread1, chp);
  Thread *tp2 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread2, chp);
  Thread *tp3 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread3, chp);
  Thread *tp4 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread4, chp);
  Thread *tp5 = chThdCreateFromHeap(NULL, WA_SIZE, NORMALPRIO, thread5, chp);
```

7) Compute the average waiting time and average turnaround time for the tasks 
in the `rtos-round-robin` example.  
(See [Homework 8](http://witestlab.poly.edu/~ffund/el6483/files/homework8.pdf).) 
You can neglect CPU time spent on tasks other than the 5 "test" tasks created for the example.


8) What is the output of the `roundrobin` command in the `rtos-round-robin` example
when you modify thread 3 to have a higher priority than the other threads?

```

```

Draw a chart (like the one on page 40 of the lecture slides) showing the scheduling
of these five tasks using the round robin scheduler with priority. Note the
priority of each task on the chart. Upload it to any image hosting service and
show the image inline here:

![]()

9) What is the computation time and period of each of the tasks in the `rtos-rms` 
project? Fill in the following table.

Task          | Computation time (system ticks) | Period (system ticks)
------------- | --------------------------------|-----------------------
t1            |                                 |
t2            |                                 |
t3            |                                 |
t4            |                                 |


What is the CPU utilization? Is there a guaranteed RMS schedule that 
will meet the tasks' deadlines?


10) What priority did you assign to each task for rate monotonic scheduling?


Task          | Priority
------------- | --------------
t1            |               
t2            |               
t3            |               
t4            |               


