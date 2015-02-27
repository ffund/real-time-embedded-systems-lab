Lab 2: GPIO Peripherals
====================

The aim of this lab is for you to:

 * Implement an experiment that turns on the LEDs on the STM32 F4 Discovery board while the User button is pressed, using GPIO registers directly.

 * Implement the same experiment using the GPIO libraries, instead of writing directly to  GPIO registers.


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



### Button press using GPIO registers directly

For this part of the lab, your task is to program your
board to light the LEDs whenever the (blue) User button
is pressed.

Specifically,

 * If the button is pressed, light all four LEDs and keep them lit for as long as the button is pressed.
 * If the button is not pressed, turn all four LEDs off.

A template `main.c` has been created for you inside the `button-gpio/src` directory. Open this file
and read it to understand the control flow of the program. Then edit it as follows.

In the `initLeds` function:

 * **Enable clock on GPIOD port**: Check Section 7.3.10 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf) and find out which bit of the RCC AHB1 peripheral clock enable register is used to enable the clock to the GPIOD port. Set a value of 1 on this bit of `RCC->AHB1ENR`; make sure not to erase the other values in the register.

 * **Set mode of GPIOD pins 12-15 to output**: Using `GPIOD->MODER`, set the mode of GPIOD pins 12-15 to output. Refer to Section 8.4.1 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf) for more information on the MODER register.

In the `initButton` function:

 * **Enable clock on GPIOA port**: Check Section 7.3.10 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf) and find out which bit of the RCC AHB1 peripheral clock enable register is used to enable the clock to the GPIOA port. Set a value of 1 on this bit of `RCC->AHB1ENR`; make sure not to erase the other values in the register.

 * **Set mode of GPIOA pin 0 to input**: Using `GPIOA->MODER`, set the mode of GPIOA pin 0 to input. Refer to Section 8.4.1 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf) for more information on the MODER register.

In the `loop` function:

 * **Set first part of `if` to evaluate true if button is pressed**: Using the input data register (`GPIOA->IDR`),
 write a statement that evaluates true if the User button (which is connected to pin 0 on GPIO port A) is pressed.
 Replace the `1` in `if (1)` with this statement. The IDR  register is specified in Section 8.4.5 of the [STM32F4 Technical Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf).

 * **Turn on LEDs when button is pressed**: Use the Bit Set Reset Register to set pins 12-15 on GPIO port D when the button is pressed (use `GPIOD->BSRRL`). The BSSR register is specified in Section 8.4.7 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf).

 * **Turn off LEDs when button is not pressed**: Use the Bit Set Reset Register to reset pins 12-15 on GPIO port D when the button is not pressed (use `GPIOD->BSRRH`). The BSSR register is specified in Section 8.4.7 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf).

After you've made these changes, make sure you are in the `button-gpio` folder, then run

    make clean
    make

to build your project.

Run

    size build/button-gpio.elf

What is the size of the ELF file? Note this in your report.

Load the build onto your board using

    make flash

The output of this command should look like this:

```
st-flash write build/button-gpio.bin 0x8000000
2015-02-23T03:20:59 INFO src/stlink-common.c: Loading device parameters....
2015-02-23T03:20:59 INFO src/stlink-common.c: Device connected is: F4 device, id 0x10016413
2015-02-23T03:20:59 INFO src/stlink-common.c: SRAM size: 0x30000 bytes (192 KiB), Flash: 0x100000 bytes (1024 KiB) in pages of 16384 bytes
2015-02-23T03:20:59 INFO src/stlink-common.c: Attempting to write 1292 (0x50c) bytes to stm32 address: 134217728 (0x8000000)
EraseFlash - Sector:0x0 Size:0x4000
Flash page at addr: 0x08000000 erased
2015-02-23T03:20:59 INFO src/stlink-common.c: Finished erasing 1 pages of 16384 (0x4000) bytes
2015-02-23T03:20:59 INFO src/stlink-common.c: Starting Flash write for F2/F4
2015-02-23T03:20:59 INFO src/stlink-common.c: Successfully loaded flash loader in sram
size: 1292
2015-02-23T03:20:59 INFO src/stlink-common.c: Starting verification of write complete
2015-02-23T03:20:59 INFO src/stlink-common.c: Flash written and verified! jolly good!
```

Then press the black Reset button on your board. Now, when you press the blue User button on the board, you should see all four LEDs light up together; when you release the User button, the LEDs should turn off.

If you need to debug, you can run `make debug` (and `make openocd` in a second terminal) to run `gdb` and step through your code.

`x` is a useful gdb command, used to eXamine a memory address. For example, `x 0x40000000` 
prints the contents of the memory address `0x40000000`.

Use `x` to inspect the contents of the memory address corresponding 
to the input data register for the GPIO A port (to which the User button is connected).

* First, check the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf) to find the specific memory address associated with this register.
* Then, run your program on the board with gdb connected.
* Break program execution sometime after the User button peripheral is already set up.  
* Using `x`, show the contents of the input data register for the GPIO A port when the user button is pressed and when it is not pressed. Include a screenshot in your report.

### Button press using GPIO library

In this part of the lab, you'll use the RCC and the GPIO libraries to to repeat the same task. Now, instead of
writing to peripheral registers directly, you'll call functions that write to these registers.

The libraries are located in the `2-peripherals/common/STM32F4xx_StdPeriph_Driver/src/` directory.
The header files are located in the `2-peripherals/common/STM32F4xx_StdPeriph_Driver/inc/`
directory. Open the `stm32f4xx_gpio.c` and read the instructions at the top.


Edit the `blinky-library/src/main.c` file. Using the comments as a guide, 
fill in function calls to the RCC and GPIO library functions.
Refer to the library source and header files to find out the usage for each function.

Next, modify the Makefile to include
the `stm32f4xx_gpio.c` and `stm32f4xx_rcc.c` source files in the list of sources.


When you're ready, run

    make clean
    make

to build your project.

Run

    size build/button-library.elf

What is the size of the ELF file? How does it compare to the size of the ELF in the previous section of this 
lab? Note this in your report.

Load the build onto your board using

    make flash

Then press the black Reset button on your board. Now, when you press the blue User button on the board, you should see all four LEDs light up together; when you release the User button, the LEDs should turn off.

If you need to debug, you can run `make debug` (and `make openocd` in a second terminal) to run `gdb` and step through your code.


## Submit your work

To submit your work, please open a terminal, navigate to the `2-peripherals` folder, and run:

    git add REPORT.md
    git add blinky-gpio/src/main.c
    git add blinky-gpio/Makefile
    git add blinky-library/src/main.c
    git add blinky-library/Makefile
    git commit -m "Please grade this lab1-blinky submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
