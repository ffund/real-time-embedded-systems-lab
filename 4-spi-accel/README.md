Lab 4: Accelerometer and SPI
============================

The aim of this lab is for you to:

 * Learn how to use SPI to communicate with an accelerometer

 * Implement a common usage of accelerometer readings (detect device orientation)


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

### Check which accelerometer you have

This lab will use the accelerometer that is on the STM32F4 Discovery board.

The STM32 F4 Discovery board has been sold with two different accelerometer devices, 
the LIS302DL and the LIS3DSH. The first release had a LIS302DL (old board) and 
the new (current) release has a LIS3DSH.

This lab has been written for the LIS3DSH, because that is what's on the board I have.
If anybody has a board with a LIS302DL, please notify me ASAP and I will
create a modified version of the lab for that device. (I will need to 
borrow your board for a few hours to test it.)

To identify which device is on your board, navigate to the `accel-whoami` folder
and flash the binary file onto your device with the command:

```
st-flash write accel-whoami.bin 0x8000000
```

If your board has the LIS3DSH, the green LED will light up, 
and you can continue with the lab instructions.
If your device has the LIS302DL, the red LED will light up,
and you should let me know ASAP so that I can modify the lab instructions
for you.


### Configure SPI communication with accelerometer device

The accelerometer on the STM32F4 Discovery communications
with the processor using SPI. In this part of the lab, 
you'll set up that SPI connection.

SPI uses GPIO pins, like other peripherals. To use GPIO pins for SPI,
you'll configure them to use their *alternate function*.

Refer to page  40 of the 
[STM32F4 Discovery user manual](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/DM00039084.pdf),
and find out which GPIO pins (port letter and pin number) are connected
to each SPI line on the MEMS accelerometer. Fill out the table in the 
report.

Next, read the `initAccelerometer()` function. In your report, explain in your own words
how the SPI interface used for communication with the accelerometer 
is configured.

Once you've completed these steps, move on to the `readSPI()` and `writeSPI()` 
functions. Using `stm32f4xx_spi.c` for reference on how to use the library functions,
write the functions used by the microcontroller (master) to read and write over the SPI bus.
There are comments in these functions to guide you.

Finally, write the `readAxes()` function, in which you read in values from 
the data registers of the accelerometer using SPI. Again, 
there are comments in these functions to guide you.
Because data is sent one byte at a time and the accelerometer readings 
are 16 bits, you will need to do some bit shifting to 
set the full set of 16-bit readings in this function.
Also note that the accelerometer returns *signed* values (2's complement), 
so we cast the reading to a `int16_t`.

When you've finished writing these functions, load your code 
onto your device with `make openocd` and `make debug`.
In the `openocd` window, you should see readings from your device, 
in units of _mg_ (0.001 _g_ s of acceleration, where _g_ is gravitational force, 9.81 m/s^2).

Place your STM32F4 Discovery board on a flat surface. 
What are the readings? In this position, you should measure +1 _g_ (around 1000 _mg_) on 
the z-axis and 0 on the x-axis and y-axis. 
Fill in the zero-G offset table in the report (refer to the 
[LIS3DSH datasheet](http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf),
Section 3.5.2, for more details.)

Using `gdb`, set a breakpoint in `main.c`, right before you put the chip select line 
low in `readSPI()` (refer to lab 1 instructions for the syntax for setting a breakpoint).
Continue up until that breakpoint (with `continue`). Then, run `layout split` so that you can see both the C 
and assembly code at the same time in `gdb`. Step through the *assembly* code using 
the command `si` to run one assembly line at a time. (The `gdb` window will also show you where you 
are in the C code, using this "split" view.)

As you approach the line that sets the pin used for CS low, 
run `x` between every `si` step to show the contents of the output data register (ODR)
for that pin. (Look up the address of this register in the 
[STM32F4 Discovery reference manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf).)
In your report, include a screenshot of the `gdb` screen at the first step 
_after_ the pin is set low (i.e., the first time `x` shows a zero value in 
the output data register for this pin).


### Light LEDs based on board orientation

In this part of the lab, we will light LEDs on the board based on its rotation.

One common use of an accelerometer in smartphones is to change the screen 
orientation between portrait and landscape rotation, depending on how the 
user is holding the device. You will do something similar; 
in this part of the lab, you will use the accelerometer readings to light LEDs so 
that when you hold the STM32F4 Discovery board facing yourself, the lowest of the four LEDs 
(the one closest to the ground - and only this one) is lit, regardless of how the board 
is oriented.

In the `accel-led` folder, copy the contents of your `writeSPI()`, `readSPI()`, and 
`readAxes()` functions into `main.c`. Also copy the `initLeds()` function you wrote in the 
button press lab (lab 2 - library version) into `main.c`.

Next, modify the `main` function in `main.c` to calculate pitch and roll of the board 
using the accelerometer values. Then, add some logic to set the LEDs each time you read 
accelerometer values.

In your report, show the part of the code in which you set the LEDs based on the board orientation.

## Submit your work

To submit your work, please open a terminal, navigate to the `4-spi-accel` folder, and run:

    git add REPORT.md
    git add accel-spi/src/main.c
    git add accel-led/src/main.c
    git commit -m "Please grade this lab4-spi-accel submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
