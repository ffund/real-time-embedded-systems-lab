Lab 6: RTOS
====================

The aim of this lab is for you to:

 * Practice using threads in an RTOS.

 * Learn about task scheduling in the context of an RTOS.


Please read this *entire document* once before beginning.

## Collaboration policy

Lab exercises are to be completed individually. You must keep your Bitbucket repository private,
and you may not share source code files with another student.

You may ask other students for help by email, but if you do so,

1. You should not share source code
2. You should include the entire email chain - the request for help, the response, and any
futher communication about the lab - in an appendix to your lab report.

You are encouraged to make use of the [TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours) for help with the lab exercises.




## Lab procedure

### Bring your working copy up to date

Start by updating your local git repository to include the latest code and instructions from the course repository. 
You should do this regularly before any activity on the lab, in case there have been 
any corrections or clarifications to the instructions.

You should already have a copy of your repository checked out in your development environment. Navigate to that directory. Then run:

    git remote add --track master upstream https://bitbucket.org/el6483s2015/el6483-lab.git

Pull changes (if any) from your own copy of the repository:

    git pull origin

Then merge in changes from the public course repository:

    git fetch origin -v
    git fetch upstream -v
    git merge upstream/master


### ChibiOS

The operating system we're using in this lab is called [ChibiOS](http://www.chibios.org/dokuwiki/doku.php).
It's a popular RTOS that has been used in a diverse range of applications, 
including a [real-time sailboat](http://link.springer.com/chapter/10.1007/978-3-319-10076-0_7), 
a [self-driving vehicle](https://hal.archives-ouvertes.fr/hal-00848101/),
and an [open-source UAV platform](https://hal-enac.archives-ouvertes.fr/hal-01059642/).

Some of the features of ChibiOS include:


 * Efficient context switch performance, efficient memory usage

 * Provides all the essential operating system primitives: threads, virtual timers, semaphores, mutexes

 * Hardware abstraction layer (HAL) supporting a variety of abstract device drivers: GPIO Port, Serial, ADC, CAN, EXT, GPT, I2C, ICU, MAC, MMC, PWM, RTC, SDC, SPI, UART, USB, USB-CDC

ChibiOS supports a 
[wide range of microcontrollers](http://www.chibios.org/dokuwiki/doku.php?id=chibios:architectures)
and compilers. However, in order to keep the git repository small, I have removed
the parts of the ChibiOS source code that deal exclusively with other platforms, 
leaving you with a "minified" version of the RTOS source to work with.
If you use ChibiOS in future projects with another supported microcontroller, you can get 
the complete source from the [ChibiOS git repository](https://github.com/ChibiOS/ChibiOS).

### Build system

Because we're building an entire operating system (as well as our own application-specific 
source code), the build system is slightly more complex in this lab.


Inside the `rtos-blinky` folder, open the Makefile.
The Makefile template for this lab is different than the ones we've used in previous labs,
when we were working in a bare metal environment.


First, look at the "Compiler options" and "Architecture or project specific options" sections.
Here, you'll note that instead of explicitly specifying compiler and linker flags
(e.g. like the ones I asked you about in lab 1), we set variables related to 
compiler and linker flags. That's because this build system includes multiple 
makefiles - if you open `ChibiOS_2.6.7_minified/os/ports/GCC/ARMCMx/rules.mk`, 
you'll see that the compiler and linker flags are set there based on the value
of the variables and arguments set in other makefiles.
Later in the project Makefile, you'll see the whole series of makefiles:

```
# Imported source files and paths
CHIBIOS = ../ChibiOS_2.6.7_minified
include $(CHIBIOS)/boards/ST_STM32F4_DISCOVERY/board.mk
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
```


Another notable change involves the debugging targets, which you'll find
at the end of the Makefile. In this lab, when we run `make openocd`, 
we're running the command

```
openocd -f board/stm32f4discovery.cfg -c "stm32f4x.cpu configure -rtos auto;"
```

which instructs `openocd` to try and detect a RTOS on the target system,
and to provide debugging facilities for that RTOS.
In this lab, whenever you run `make openocd` and `make debug`, 
you should see the following lines in the 
`openocd` output:

```
Info : Auto-detected RTOS: ChibiOS
Info : Successfully loaded memory map of ChibiOS/RT target running version 2.6.7
```

We need RTOS support in `openocd` in order to debug multiple threads in `gdb`, as 
we'll do in the next section.

### Button press to light LEDs

In the `rtos-blinky` project, we're going to run a simple program 
to light the LEDs on the board whenever the (blue) User button is pressed, 
You may recall that we have already done this in lab 2 (using GPIO registers 
directly and using the standard peripheral library, with polling) and in lab 5 
(using interrupts). Read the `main.c` source file to see how we implement 
this inside ChibiOS.


Compile the `rtos-blinky` program and load it onto your board with 
`make openocd` and `make gdb`. Run `continue` and verify that the program
works as expected, and answer the first question in the lab report.


Set a breakpoint at the top of the `Blink` function. When you press the User button 
on the Discovery board, the program will enter the `Blink` function and execution
should halt for debugging. However, you *may* find that the code you see at the top 
of the `gdb` screen is unfamiliar - not the `Blink` function, as you would expect.


This is because `gdb` focuses debug information on one thread at a time.
To learn more, run `info threads` inside `gdb`. The output should look something like this:

```
(gdb) info threads
  Id   Target Id         Frame 
* 2    Thread 536873032 (idle :  : READY) _port_switch_from_isr () at ../ChibiOS_2.6.7_minified/os/ports/GCC/ARMCMx/chcore_v7m.c:207
  1    Thread 536872960 (main :  : CURRENT) Blink () at main.c:15
```

(If you don't see multiple threads here, quit `gdb` - leave `openocd` running - and try it again.
Sometimes `gdb` doesn't recognize the RTOS the first time you run it.)

For each thread, `gdb` displays:

 * the thread number assigned by `gdb`
 * the thread number on the target system (the Discovery board), which also coincides with the thread's location in memory on the Discovery board
 * the current stack frame summary for that thread. Note that at the beginning of the "Frame" field it gives 
the thread's name (if defined) and current state.

The asterisk marks the thread that is the current focus of debugging. 
To display another thread's source code, e.g. thread number 1, use `thread 1` and note that the source code at the 
top of the window changes.
You can also see a *backtrace*, or a summary of how the thread arrived where it is; try 
stepping to the middle of the `Blink()` function and then run

```
thread apply 1 backtrace
```

(substituting the number assigned by `gdb` to the main thread in *your* application.)
Show the output of this command in your report.

You may be surprised to see two threads, since we didn't seem to create any threads
in this application. Both threads in this application are automatically created by the 
operating system. From the [ChibiOS documentation](http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:createthread):

> After initializing ChibiOS/RT using `chSysInit()` two threads are spawned by default:
> 
> * Idle thread. This thread has the lowest priority in the system so it runs only when the other threads in the system are sleeping. This threads usually switches the system in a low power mode and does nothing else.
> * Main thread. This thread executes your main() function at startup. The main thread is created at the NORMALPRIO level but it can change its own priority if required. It is from the main thread that the other threads are usually created.

Most operating systems have an idle thread. Its main purpose is to make sure that the scheduler 
always has a thread in the Ready state to schedule. Without the idle thread, there could be cases 
when the current thread leaves the CPU and no other threads are Ready. Since the idle thread is always Ready 
(or Running), the scheduler can always find a thread to run.


In the previous two implementations of this application (in lab 2), I asked you to check the size 
of the ELF file, and compare the code size with and without the standard peripheral libraries.
Check the size of the RTOS implementation:

    size build/rtos-blinky.elf

and note the results in your report (question 3).

How does it compare to the size of the ELF in lab 2? You may be surprised by the 
small size of this ELF, given that we're including an entire operating system in it. 
ChibiOS, like many other RTOS designed for small embedded platforms, 
has a modular build process that allows you to only include the parts of the operating
system that you want to use. In the `chconf.h`, `halconf.`, and `mcuconf.h` header files, 
we describe which parts of the target system we intend to use (as well as other settings, 
like system clock settings). The parts of the system that we have marked as unused
in those header files are automatically left out of the final build.

Answer question 4 in your report.

### ChibiOS shell

In the next part of this lab, we'll take our OS project a little bit farther 
by adding a [shell](http://en.wikipedia.org/wiki/Shell_%28computing%29), 
which you will access using a USB serial interface and use to run tasks on demand.

Build and load the `rtos-shell` application onto your Discovery board. Use a micro 
USB cable to connect the micro USB port of the Discovery board to your computer. 
You'll still need to power the board through the mini USB port (either from your computer, 
or from a USB power adapter). Reset the board (with the black Reset button).

On your laptop, install a serial console (if you don't already have one):

```
sudo apt-get install screen
```

Then run

```
sudo screen /dev/ttyACM0
```

Hit enter a few times, and you should see a command prompt like this:

```
ch> 
```

This is the ChibiOS command prompt on your Discovery board. You 
can execute commands on your board by entering them at this prompt.

Run `help` to find out what commands are available. Try out each of the commands, 
and find out what they do. Answer question 5 in your report.

**Warning**: Don't try to run `make flash` while the micro USB port is connected!
If you do, you'll probably have to reset the board (as described in an 
email announcement via NYU Classes around lab 2).

### Experiment with the RTOS scheduler

Now we're ready to start creating some threads and see how they are scheduled.
Inside the remaining projects, we've defined several new, additional commands
for our ChibiOS shell. We'll use them to explore RTOS scheduler functionality.

For the rest of this lab, you may find the following references helpful:

 * [Create a thread in ChibiOS](http://www.chibios.org/dokuwiki/doku.php?id=chibios%3Ahowtos%3Acreatethread)
 * [Priority levels in ChibiOS](http://www.chibios.org/dokuwiki/doku.php?id=chibios%3Akb%3Apriority)
 * [Round robin scheduling in ChibiOS](http://www.chibios.org/dokuwiki/doku.php?id=chibios%3Akb%3Around_robin)
 * [Reliable timing using threads in ChibiOS](http://www.chibios.org/dokuwiki/doku.php?id=chibios%3Akb%3Atiming)

The first scheduler we'll look at will be a simpler priority scheduler. In the 
`rtos-priority` folder, open `main.c`, 
and look for the `cmd_priority` function. This function is simple: it creates
five threads from heap memory, each of which runs a different function (`thread1`, `thread2`, 
`thread2`, etc.). The individual thread functions print their priority and a letter
to the ChibiOS terminal.

Connect to the ChibiOS terminal and run the `priority` command. What is the output?

ChibiOS uses a round-robin scheduler for threads at the same 
priority level. Since we created all five of these threads at normal priority, 
they are executed in their order of arrival, and the output should look like this:

```
Creating 5 threads: 
------------------------------------------ 
Thread 1, priority 64: E
Thread 2, priority 64: D
Thread 3, priority 64: A
Thread 4, priority 64: B
Thread 5, priority 64: C
```

Your first task is to get the output of the `priority` command to print
the letters A-E in alphabetical order, by changing the priority levels of the threads.
(In other words, the thread that prints A - thread 3, should execute first, then the
thread that prints B, etc.) Don't change the order in which the threads are created, 
or the letter printed in the individual thread functions - just change the priority level. 
Keep all priorities below the normal priority (i.e., `NORMALPRIO - 1`, `NORMALPRIO - 3`). 
Answer question 6 in your lab report.

**NOTE**: If your serial terminal is acting strange, the `exit` command in this ChibiOS
shell can sometimes help reset it to a better state.

The next scheduler we'll look at is the round robin scheduler.
In `rtos-round-robin`, open `main.c`. You'll see that this time, 
we've defined a `roundrobin` command that we can execute in the ChibiOS shell.
It will create 5 threads, each of which does some "work." At four times during 
the task execution (0%, 25%, 50%, and 100%), each thread
will print its name, priority, and the CPU time it has used thus far (measured in 
system ticks).

Run `roundrobin` and answer question 7 in the lab report.

Modify thread 3 to have a higher priority than the other threads. What is the output
of the `roundrobin` command now? Answer question 8 in the lab report.

The third and final scheduler we'll consider is a rate monotonic scheduler, 
in the `rtos-rms` project. In this project, we've defined a `rms` command.
When you run `rms` in the ChibiOS shell, you'll see many lines of output 
like this:

```
t3  N  64   532896   533176  870
t3  X  64   532953   533176  927
t2  N  64   532954   533077  972
t2  X  64   532992   533077  1010
```

(Some lines may be interleaved with one another, since we're not writing to 
the USB serial output in a thread-safe way. You can just ignore those lines.)

Each line of output tells you (in order):

 * The task name (there are four tasks altogether)
 * Whether the message is printed when the task eNters its execution or eXits it
 * The task's priority number
 * The current system time (in system ticks)
 * The absolute deadline (measured in system time) for the current task execution
 * The total number of system ticks allocated to this task so far (cumulative, over all executions of this task)


Use the output to find the computation time and period of each of the four tasks. (These are 
periodic tasks, with deadline equal to period.) Answer question 9 in the lab report.


Note that there is no preemption in this schedule. (Every N line is immediately followed 
by the X line.) In the `rtos-rms` project, you'll find that I have set `CH_TIME_QUANTUM` in `chconf.h`
to zero. This effectively disables round-robin scheduling among tasks of the same priority.


To implement a basic rate monotonic scheduler, modify the priorities assigned 
to each thread according to the rate monotonic scheduling rule. 
What priorities did you assign? Answer question 10 in the lab report.


## Submit your work

To submit your work, please open a terminal, navigate to the `6-rtos` folder, and run:

    git add REPORT.md
    git add rtos-round-robin/main.c
    git add rtos-priority/main.c
    git add rtos-rms/main.c
    git commit -m "Please grade this lab6-rtos submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
