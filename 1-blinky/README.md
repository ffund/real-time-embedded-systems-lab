Lab 1: Blinky
====================

The aim of this lab is for you to:

 * Set up a toolchain and development environment for completing this and future lab exercises

 * Run a simple experiment to blink the LED on your STM32 F4 Discovery board

 * Practice using debugging tools in your development environment


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

### Set up development environment


We will use an open-source toolchain for all of the lab exercises. Instructions are provided for Ubuntu 14.04. 
If you are running another operating system, you may install an Ubuntu partition or use an Ubuntu Live USB to run the lab exercises.

Using an Ubuntu virtual machine is *not* recommended, because setting up the USB connection between 
the host and virtual machine (in order to program the STM32 via USB) may be somewhat complicated.

If you aren't already using Ubuntu, you'll need to [download an ISO](http://www.ubuntu.com/download/desktop) 
from the Ubuntu website. Further instructions are available online:

 * for [creating a Live USB on Windows](http://www.ubuntu.com/download/desktop/create-a-usb-stick-on-windows)
 * for [installing Ubuntu in a separate partition alongside your existing operating system](http://www.ubuntu.com/download/desktop/install-ubuntu-desktop)

Make sure to leave at least 2GB of extra space on your Ubuntu partition or your USB device, for persistent files.
On the Windows Live USB creator, you need to use the "Set a persistent file size for storing changes" option.
This is very important - if you don't have enough extra space, you won't be able to download and work 
with the source code for the lab.

If you are using the Ubuntu Live USB approach and are having trouble, the TAs can help you create your Live USB. 
(Note: Mac OS X has traditionally had some problems with creating live USBs.)
Bring a 4GB or larger USB key to TA office hours for assistance.


You should be comfortable with the Linux shell (terminal). If you're not, there are plenty 
of materials online that can help you get up to speed. [Here is one tutorial you can try](http://cli.learncodethehardway.org/book/). 
You don't need to completely master the terminal, but you shouldn't be scared if you're expected to use it.


Once you are in your Ubuntu 14.04 environment and are ready to start, you'll need to install some tools.

#### Get comfortable with `git` and Markdown

First, you will install `git`. Git is the most widely adopted version control system for software development, 
and we're using it in this course to share code with you and to receive your submissions for grading.

To install `git`, open a terminal and run:

    sudo apt-get install git

We recommend setting up key-based authentication in order to use  `git` with private repositories on 
Bitbucket. You can find instructions for setting this up [here](https://confluence.atlassian.com/pages/viewpage.action?pageId=270827678).

Once that's done, you can get a local copy of your fork as follows. Open a terminal and run:

    git clone git@bitbucket.org:USERNAME/el6483-NETID.git

where `USERNAME` is your Bitbucket username and `NETID` is your NYU Net ID. 
(You should have named your fork `el6483-NETID`.) You can also find this address on the Bitbucket home page for your repository:

![Find SSH URL](http://i.imgur.com/VsbpZet.png)

You'll submit your work for grading by editing a Markdown file and then pushing a **commit** to your fork.

To practice this workflow now, find the file called `REPORT.md` inside the `1-blinky` folder.  You'll write your lab report inside this file.

The REPORT.md file is written in Markdown. A helpful tutorial on Markdown is available [here](https://bitbucket.org/tutorials/markdowndemo). You may also be interested in using a Markdown editor with live preview, such as [Atom](http://www.webupd8.org/2014/05/install-atom-text-editor-in-ubuntu-via-ppa.html). Here's what this page looks like inside the Atom editor:

![Atom editor](http://i.imgur.com/jTTwWCT.png)

However, even a basic text editor should be fine.


Whether you're using a Markdown editor or a regular text editor, open the REPORT.md file. At the top, you'll see a table, with a space for your name and Net ID. Fill in your own name and Net ID, and save the file.

Open a terminal, and navigate to the `1-blinky` folder. Type the following to **commit** the changes to this file and **push** them back to Bitbucket:

    git add REPORT.md
    git commit -m "Added name and netID to report for lab1"
    git push -u origin master

Now, if you visit your repository in Bitbucket and click on "Commits," you should see that your changes have been pushed back to Bitbucket. Your repository (and anything you've pushed to it) is also visible to the instructor and TAs, so we'll grade your work right here in Bitbucket.

Once you're comfortable with the workflow for submitting your lab report, you can continue installing the tools necessary for the lab.


#### Understanding the development tools

In past programming courses, you may have become accustomed
to using `printf()` or equivalent statements to print debug information. When programming for embedded systems, the development hardware often does not have the same output functionality as a typical desktop computer. In this situation, we can use a technique called **semi-hosted debugging**.

In semi-hosted debugging, we use a link between the development board ("target") and the host computer to display debug information from the target on the host.

In order to do this type of debugging you need several things: a board with an on-chip debugger, a program that controls the chip’s debugging mode, a way to interface with it, and a compiler that can compile programs with debugging information. Instead of a single tool to do all these things, we'll use many tools working together. Collectively, these tools are called a toolchain (since they are "chained" together to produce a final executable application).

Our lab toolchain will include:

* `gcc`, a popular open-source C compiler. The version of `gcc` that we'll use to **cross-compile** for the ARM platform is called with the command `arm-none-eabi-gcc`

* `gdb`, a popular open-source debugger that gives you visibility inside your programs while they are running. `gdb` can run in semi-hosting mode, where it interfaces with a server that communicates with the on-chip debugger. The version of `gdb` that is used to debug ARM programs is called with the command `arm-none-eabi-gdb`.

* `openocd` is open-source software that interfaces with a hardware on-chip debugger (e.g., like the one on the STM32F4 Discovery board). `openocd` runs a server that a `gdb` instance can attach to, and will pass messages between `gdb` and the hardware debugger on the STM32 F4 Discovery board.

* `stlink` is a family of tools for use with the STM32 F4 Discovery board. It includes `st-util` (which has a role similar to `openocd`) and `st-flash` (which can be used to load a program onto the development board). Most of the functionality included in `stlink` tools is also available in other tools, but sometimes it's more convenient to use the `stlink` tools.


#### Install ARM development tools

At this point, you should be ready to install some ARM development tools. To complete this section, you must be connected to the Internet.

Start by making sure your package manager knows about the most recent versions of available packages. Open a terminal and run:

   sudo apt-get update

First, you will need to install some prerequisites in order to build the later tools:

    sudo apt-get install build-essential pkg-config libusb-1.0-0-dev git autoconf

Install an up-to-date version of the `gcc` ARM toolchain. This includes the ARM version of the `gcc` compiler and the ARM version of the `gdb` debugger.

First, remove any old (incompatible) versions of this tool that might already be installed (don't worry if the output of this command says "Unable to locate packet"):

    sudo apt-get remove binutils-arm-none-eabi gcc-arm-none-eabi


Then, add a more recent source for these tools and install from this repositoyr:


    sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
    sudo apt-get update
    sudo apt-get install gcc-arm-none-eabi=4.9.3.2014q4-0trusty12

Also build and install the `stlink` utilities for communicating with the STM32 F4 Discover board:

    git clone https://github.com/texane/stlink.git
    cd stlink
    ./autogen.sh
    ./configure
    make
    sudo make install
    sudo cp 49-stlinkv*.rules /etc/udev/rules.d


Finally, build and install openocd:

    cd
    wget http://downloads.sourceforge.net/project/openocd/openocd/0.8.0/openocd-0.8.0.tar.bz2
    tar jxf openocd-0.8.0.tar.bz2
    cd openocd-0.8.0
    ./configure --enable-stlink
    make
    sudo make install

The final step is to tell the debugger that it's safe to use configuration files in any directory. Make sure to run the following command as an ordinary user, *not* as root:

    echo "set auto-load safe-path /" >> ~/.gdbinit


Then reboot your computer.


#### Plug in your STM32F4 Discover Board and test


At this point, we'll run a quick check to make sure the STM32F4 Discovery board is recognized by our system.

Plug your board into your computer using a USB mini cable.

The small power LED near the USB connection should be lit. The larger COM LED should be lit in red.

Then, open a terminal and run the following command:

    st-util

The COM LED on your board should begin to flash or show steady green, and you should see some output like the following:

```
2015-02-08T18:12:26 INFO src/stlink-common.c: Loading device parameters....
2015-02-08T18:12:26 INFO src/stlink-common.c: Device connected is: F4 device, id 0x10016413
2015-02-08T18:12:26 INFO src/stlink-common.c: SRAM size: 0x30000 bytes (192 KiB), Flash: 0x100000 bytes (1024 KiB) in pages of 16384 bytes
2015-02-08T18:12:26 INFO gdbserver/gdb-server.c: Chip ID is 00000413, Core ID is  2ba01477.
2015-02-08T18:12:26 INFO gdbserver/gdb-server.c: Target voltage is 2905 mV.
2015-02-08T18:12:26 INFO gdbserver/gdb-server.c: Listening at *:4242...
```

Press Ctrl+C to quit `st-util`. Let's also try `openocd` - run:

    openocd -f board/stm32f4discovery.cfg

and you should see similar behavior from the COM LED (either flashing or steady green), as well as output like:

```
Open On-Chip Debugger 0.7.0 (2013-10-22-08:31)
Licensed under GNU GPL v2
For bug reports, read
  http://openocd.sourceforge.net/doc/doxygen/bugs.html
srst_only separate srst_nogate srst_open_drain connect_deassert_srst
Info : This adapter doesn't support configurable speed
Info : STLINK v2 JTAG v14 API v2 SWIM v0 VID 0x0483 PID 0x3748
Info : Target voltage: 2.903844
Info : stm32f4x.cpu: hardware has 6 breakpoints, 4 watchpoints
```

Press Ctrl+C to quit `openocd`.

We've now confirmed that the utilities on your computer are able to communicate with the STM32 F4 Discovery's on-chip debugger.

#### Build, install, and run the lab example

Now you should be all set up to run the blinky experiment. This is just a simple experiment to blink an LED on the development board and to practice using the debugging tools.

Open a terminal and navigate to the `1-blinky/src` folder inside the copy of the repository you've cloned using `git` in a previous step.

This contains two folders: a `blinky` folder, which contains the main source code for this lab exercise, and an `STM32F4-Discovery_FW_V1.1.0` folder, which contains firmware libraries.


`cd` inside the `blinky` library and run

    ls -a

You should see (among others) a Makefile. Open this Makefile and examine it.


*Answer questions 1), 2), 3) and 4) in REPORT.md.*

Once you've finished answering the questions about the Makefile, run

    make

to build this code.

The output of this command should look something like this:

```
arm-none-eabi-gcc --specs=rdimon.specs -lc -lrdimon  -ggdb -O0 -Wall -Tstm32_flash.ld  -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I. -I../STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I../STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I../STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I../STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc main.c system_stm32f4xx.c ../STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s -o blinky.elf
arm-none-eabi-size blinky.elf
   text	   data	    bss	    dec	    hex	filename
  11276	   2420	   1288	  14984	   3a88	blinky.elf
```

and if you run `ls`, you should see a file `blinky.elf` in your directory. This is the binary executable file we'll copy to the STM32F4 Discovery board.

To run this example, we'll need two terminals. Open a second terminal and run

    openocd -f board/stm32f4discovery.cfg

in the new terminal. Leave this running.

In the original terminal, open the .gdbinit file in a text editor and examine it carefully.

We are going to use `gdb` and its connection to the development board via `openocd` to load the `blinky.elf` 
program onto the board. The .gdbinit file defines some initial commands that will run when we open `gdb`.

* Answer question 5) in REPORT.md.*

Note that you need to run `gdb` from inside this directory in order for these 
commands to run automatically.

Now we're ready to start. In the terminal, in the same location where you ran `make`, run

    arm-none-eabi-gdb -tui blinky.elf

Hit 'Enter' when prompted to "Type return to continue."

Leaving `gdb` running, look at the second terminal, where `openocd` is still running.
You should see some output in this window saying

    Info:    accepting 'gdb' connection from 3333

followed by some other output.

Leave `openocd` running and return to your `gdb` terminal.

At this stage, we have loaded the new program onto the board, but nothing is running.
The top of the screen should show you the current position in the program execution.
It should be in the startup file, since the program hasn't started running yet.

Run

    info registers

What is in each of the ARM registers at this stage?

* Answer question 6) in the REPORT.md.*

Before we continue running the program, we're going to set a **breakpoint**.
This will stop program execution at a certain point, so that we can examine the state of
the program, the contents of registers, etc.

Let's set a breakpoint at the beginning of the main.c file. In the `gdb` window,
run:

    break main.c:1

Hit Enter, then at the `gdb` prompt, run

    continue

and hit Enter again.

The program will begin to run, but will stop at the breakpoint we've defined.
This will occur at the beginning of the `initSystick()` function. Now, we should
see the main.c file in the top half of the screen.

Once again, type

    info registers

to show the values in each register at this stage.

The debugger can also **disassmble** any function for us, i.e., it can show
us the assembly code version of any function.


Let's look at the SysTick_Handler function. First, set a breakpoint there:

    break main.c:29

then run

    continue

When program execution stops again, you should be at the top of the SysTick_Handler
function.

Let's print the value of the millisecondCounter variable here:

    print millisecondCounter

Also show the value of each of the registers:

    info registers


Now, let's look at the assembly code for this function:

    disassemble SysTick_Handler

The arrow on the left side shows what line of code execution is currently stopped at.


Let's "step" forward one line of C code. Run

    step

Run

     disassemble SysTick_Handler

again. What line of code is execution stopped at now?

Now run

    print millisecondCounter

and

    info registers


*Answer question 7) in REPORT.md.*

To remove the breakpoint we've set, run

    clear main.c:29


Let's set another breakpoint just before toggling the LED and printing "Toggling LED".
Run

    break main.c:44

then

    continue

Step through lines 44-49 of main.c using the

    step


command. In your second terminal, with `openocd` watch for the "Toggling LED" output
to appear. Take a screenshot of this output, upload it to an image hosting service,
and include it in your report.

*Answer question 8) in REPORT.md.*


Next, let's remove the breakpoint:

    clear main.c:44

and run

    continue


Our LED should be flashing on and off, with one second in the on state
followed by one second in the off state.


#### Modify `printf` statement in the code


Modify `main.c` so that, in the main loop, you get the current value
of the LED before toggling it. Change the `printf` statement so that it prints
either

    Toggling LED state from off to on

or

    Toggling LED state from on to off
    

You should implement this by checking the current state of the LED, *not* by 
tracking a loop variable.

Build your project and run it on your board.
Take a screenshot of your `openocd` showing the
modified `printf` statement and upload it to an image hosting service so that you
can include it in your report.

*Answer question 9) in REPORT.md.*


#### Run without semi-hosted debugging

Stop `openocd` and `gdb` on your computer. Disconnect the board from the host
computer, then reconnect it. Does the blinky program run? Explain.

*Answer question 10) in REPORT.md.*

Make a copy of main.c, called main-nodebug.c:

    cp main.c main-nodebug.c


Edit main-nodebug.c, and remove the reference to the `<stdio.h>` header file
and the `setbuf` and `printf` statements. Also, change the pin to 15, which will
activate the blue LED instead of the orange one.

In order to build and compile this program, you'll edit the Makefile. This way, you'll be able 
to build either the original blinky or the version without semi-hosted debugging, 
just by running `make` with different arguments. 

In general, a Makefile includes stanzas in the form

```
target: dependencies
	system command to build
```

and then to build a certain target, you run `make target`. (If you run `make` without specifying
a target, it will run `make all` by default.)

If you look at the current Makefile, you'll see that the stanzas that define the rules for 
building the original blinky are


```
all: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(SEMIHOSTING_FLAGS) $(CFLAGS) $^ -o $@    
	$(SIZE) $@    
```
where the variables (`PROJ_NAME`, `SRCS`, `CC`, `SEMIHOSTING_FLAGS`, `CFLAGS`, and `SIZE`)
are defined previously in the Makefile.

Edit the makefile, and add a new target:

```
nodebug: $(PROJ_NAME)-nodebug.elf

$(PROJ_NAME)-nodebug.elf: dependencies
	system command to build
```

Fill in the "dependencies" and "system command to build" yourself, 
so that when you run

    make nodebug

it will compile main-nodebug.c with the necessary flags and library files, 
and generate a `blinky-nodebug.elf` program that will run without semi-hosted
debugging. (You can find lots of information on Makefile syntax online; 
[here](http://mrbook.org/blog/tutorials/make/) is a quick tutorial.)

Run `openocd' as before and run

    arm-none-eabi-gdb -tui blinky-nodebug.elf
    
to burn this `blinky-nodebug.elf` to your device. Then,
stop `openocd` and `gdb` on your computer. Disconnect the board from the host
computer, then reconnect it.
This program should flash the blue LED *without* `openocd` or `gdb` attached.
Show your work to a TA
during [lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours).


## Submit your work

To submit your work, please open a terminal, navigate to the `1-blinky` folder, and run:

    git add REPORT.md
    git add main.c
    git add main-nodebug.c
    git add Makefile
    git commit -m "Please grade this lab1-blinky submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
