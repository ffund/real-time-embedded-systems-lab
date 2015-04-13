Lab 5: Timing and Interrupts
============================

The aim of this lab is for you to:

 * See how to configure interrupts and how an interrupt service handler works.
 * Learn how to use timer peripherals on the STM32F4 Discovery board.
 * Implement a digital dice application using a state machine.


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

### Triggering interrupt on button press

The first experiment for this lab is in the `interrupt-button` folder.

In lab 2, we wrote a program that polled the status
of the User button and

 * turned on the LEDs if the User button was pressed, and
 * turned them off when the button was released.

In this lab, we'll write the same program using interrupts.

First, open the `main.c` file and copy in your `initLeds()` function from lab 2.

Then, read the `initButton()` function. Use this function and the library source files to answer the first question in the lab report.

Use `make flash` to compile the program and load it onto your board. Press the black Reset button. Then, try pressing the blue User button. Have you achieved the desired functionality?

#### Count button events and debouncing

Modify the interrupt service routine for the User button
press, so that the `risingCounter` is incremented each time
the button is pressed, and the `fallingCounter` is incremented each time the button is released.

Run the new program on your board with `openocd` and `gdb`.
Before pressing the button, use `print risingCounter` and
`print fallingCounter` in `gdb` and verify that both are zero:

```
(gdb) print risingCounter
$1 = 0
(gdb) print fallingCounter
$2 = 0
```

Then run `continue` to continue program execution.

Now, press the User button ten times in a row *as fast as you can*. Then, run `print risingCounter` and `print fallingCounter` again.

What value do you expect to see in `risingCounter` and `fallingCounter`? What value do you see instead?

If you get a 10 in each, restart program execution and try it again, pressing the button faster this time.

You should find that the program registers more than 10 button presses (and possibly, a different number of presses and releases). 
This is because of *switch bounce*.
Switch bounce is the non-ideal behavior of the contacts
in a button or switch that creates multiple electrical
transitions for a single user input.

When you press a button or a switch, the contacts inside don't
make a clean electrical connection, but will actually "bounce" slightly
before settling. When viewed on an oscilloscope,
it might look something like this:

![Bounce](http://upload.wikimedia.org/wikipedia/commons/a/ac/Bouncy_Switch.png)

Bounce differs depending on the force and speed with which a user presses a button.
This makes it tricky to debug - you might find that your product involving a switch
works OK when *you* test it in the lab. But when you release
your product, and your end users press buttons
with different force or speed than you typically
do, your product will suddenly have bounce-related bugs.

Switch debouncing techniques compensate for switch bounce, and can be done in an electrical circuit
or in software. Learn about one method of switch debouncing
of each type, and explain it in your report. Here are some useful references:

 * Jack Ganssle, [A Guide to Debouncing](http://www.eng.utah.edu/~cs5780/debouncing.pdf)
 * Slides from Oregon State University on [Debouncing Switches](http://web.engr.oregonstate.edu/~traylor/ece473/lectures/debounce.pdf)

In the last part of this lab exercise (the digital dice application), you'll need to use 
a debouncing technique. 

#### Polled interrupts

Next, we'll see what happens if we fail to poll
all possible triggers inside an ISR.

In the ISR, change the line

```
if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
```

to


```
else if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
```

We'll also make the ISR very long. Inside the ISR, add the code snippet

```
int x;
for ( x = 0; x < 10000; x++ ) {
   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}
```

after `GPIO_SetBits(GPIOD, LEDS);` AND after
`GPIO_ResetBits(GPIOD, LEDS);`.

Load this program onto the board and press the black Reset button.
Then, press the User button, with varying force and speed. Can you get the LEDs to
stay lit even when the button is released?
In your report, explain the circumstances that cause this
bug.

### Timer peripherals

Timer peripherals are among the most commonly used interrupt triggers. In this part of the lab, we'll learn
how to configure and use them.

Timers can be tricky to use because they depend on a
lot of configuration details related to the system
clock and bus speeds. For those of you using PWM in your projects, this 
lab exercise will be very important.

#### System clock

On the STM32F4 Discovery board, the system clock can operate at up to 168 MHz, 
using an external crystal oscillator with a frequency of 8 MHz followed by a PLL multiplier.
More specifically, when the HSE (high speed external oscillator) is active, the system clock is calculated from the 
value of the HSE and some PLL parameters, as:

`SYSCLK =  ((HSE_VALUE / PLL_M) * PLL_N) / PLL_P`

To operate at 168 MHz, we set

Parameter | Value
----------|--------
`PLL_M`   | 8
`PLL_N`   | 336
`PLL_P`   | 2

We also need to specify the HSE frequency, which is defined in a library header file. 
This header file is generic, not specific to the F4 Discovery board, and defines the HSE value as 25 MHz. Until now, this
didn't matter because we weren't using any peripherals that needed exact timing (like Timer or UART peripherals).
For this lab, to set the correct value for HSE, we will use the Makefile to pass a new
`#define` statement to the project.

In the Makefile, you should see a flag

`-DHSE_VALUE=8000000`

This is equivalent to adding

`#define HSE_VALUE 8000000`

to the project, and tells the code the frequency of the crystal oscillator (8 MHz).

#### Peripheral bus clocks

The system clock speed and the speed of individual peripheral
buses are set up in the `SystemInit()` function, which is called
from the startup file. To see the `SystemInit()` function,

 * Load the `timing-pwm` program onto your board with `openocd` and `gdb`
 * Before you run `continue` in `gdb`, set a breakpoint in the startup file: `break startup_stm32f4xx.S:97`
 * Run `continue` to resume program execution up to the breakpoint.
 * The program should break at the `bl  SystemInit` instruction. From this point on, step through the code line by line. Use `si` to step through assembly instructions and `s` to step through C code.

 Look for the lines of code that modify `RCC->CFGR`. By
 writing to this register, we set prescaler values for
 each of three peripheral buses, which then determines their speed as follows:


Bus   | Prescaler | Speed
----- | ----------|------------
AHB   | 1         | System clock / prescaler
APB1  | 4         | AHB speed / prescaler
APB2  | 2         | AHB speed / prescaler


In your report, fill out the table of peripheral bus speeds for this experiment.

On the STM32F4 Discovery board, some timers are on the APB1 bus and some timers 
are on the APB2 bus. The input clock to the timer will depend on the system clock 
and the bus clock prescalers.


#### Timers

In the `timing-pwm` program, we configure two 1-second timers. They'll both be general-purpose
timers, whose operation is described in Section 18 of the 
[STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf).

To know how to set timers for a desired period, we need to understand a little bit 
about how timers works. A timer typically consists of a prescaler, an N-bit counter 
register (typically 8, 16, or 32 bits), and one or more compare registers.

* The prescaler divides the input clock frequency, creating a timer clock at a desired rate. 
The timers on the Discovery board are very flexible, allowing arbitrary integer
prescalers up to 16 bits (many other microcontrollers only allow a few specific
prescaler values.)
* The timer then counts up to a certain period (on the Discovery board, some timers have 16-bit counters and some have 32-bit counters).
* When the counter reaches the desired value, it triggers an overflow, restarts from 0, and (if armed and enabled in NVIC), fires an interrupt.

Here's an example with the prescaler value of 4:

![timer](http://i.imgur.com/OilUhFm.png)


One quirk of the STM32F4 Discovery is the way the timer clock frequencies (input to the prescaler)
are set. From page 214 of the [STM32F4 Discovery Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf):

> The timer clock frequencies are automatically set by hardware. There are two cases:
> 
> 1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
> that of the APB domain to which the timers are connected.
> 
> 2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
> timers are connected


In our case, the APB prescalers are not 1, so the timer clocks will be set to double the bus clock.
Since we want to set 1-second timers, we'll set our prescaler and period as follows:
 
 * APB1 bus speed:    42 MHz
 * Timer clock speed: 84 MHz
 * Prescaler:         21000 (now timer frequency is 4 kHz)
 * Period:            4000 (now timer will overflow every 1 second)


#### Timer-triggered interrupts

Timers can trigger interrupts if set up in the usual manner:
arm the timer, enable the timer interrupt in the NVIC, and write an ISR.

In our program, we've written an ISR that toggles the blue LED every time 
the interrupt is triggered. The blue LED will therefore be lit for one second,
then off for one second, and toggle continuously between these two states.

#### PWM

Besides for triggering interrupts, timers on the STM32F4 Discovery board 
can also drive PWM outputs on GPIO pins. To use this functionality, 
we connect the timer to a PWM *output channel*, and set the pulse period.
The output will be pulsed high while the value in the timer's counter register
is less than the pulse period.

In our program, we used a timer period of 4000 to set 1-second timers.
We'll connect one of our timers to a PWM output with a pulse period of 3000.
Then we'll connect this PWM output to the green LED, which will be pulsed every 
second with a 75% duty cycle.


#### Running the `timing-pwm` program
 
Load the `timing-pwm` program onto your board with `openocd` and `gdb`.
Run the program and watch the LEDs. Use a stopwatch on your timer or smartphone
to count how long it takes for the green LED to pulse 20 times. 
Is the timer triggering every 1 second, as expected?


Set a breakpoint inside TIM2_IRQHandler. When the program execution breaks,
```
info registers
```

Pay special attention to the link register. What is its value? What does this special value indicate?
Answer the question in the report.

Does the green LED keep flashing when normal operation of the CPU is paused for the breakpoint 
inside the ISR? Why? In your report, explain how hardware-timer-controlled PWM channels
can be a useful tool (more so than CPU-controlled PWM) for outputs with very specific timing requirements.


### Digital dice application

In the last part of this lab exercise, we'll use the SysTick timer to control
the flow of a program implementing a finite state machine.
On ARM cores the SysTick timer is typically used as the RTOS tick timer,
but when we're not running an operating system, we can use it for other things
(like program flow).

We are going to implement a digital dice application. The dice will display a four-bit 
random number using the LEDs on the board. To generate a value, you must shake the board.
The number will be displayed when you stop shaking the board.

You can see a video of my digital dice implementation on [YouTube](http://youtu.be/38gIszeC3nY).

The state machine for this application is:

![SM](http://i.imgur.com/UpKbFln.png)

Your digital dice should meet the following specifications:

 * While shaking the dice, it should not keep changing the value shown on the LEDs. 
It may either show a zero value (all LEDs off) or show the value of the previous dice "throw."
 * While the dice is not moving, it should not keep changing the value shown on the LEDs.
 * In other words, the value on the LEDs should only change on the transition from "shaking" to 
"display."


In the `main.c` source file in the digital dice folder, fill in the following functions
which you have written in previous lab exercises:


 * `initLeds()` from lab 2
 * `readAxes()` from lab 4
 * `readSPI()` from lab 4
 * `writeSPI()` from lab 4

Functions related to the hardware random number generator have been provided for you:

 * `initRng()`
 * `showRandomNumber()`


You must fill in two functions:

 * `isShaking()` should return true while the board is shaking
 * `SysTick_Handler()` (which will be triggered when the system tick generates an interrupt)

Some global variables and `#define` statements have been set up that you may choose to use inside 
the `isShaking()` statement: a `BOUNCE_DELAY`, a `THRESHOLD`, and a `bounceCounter`. 
(You'll need to set  `BOUNCE_DELAY` and `THRESHOLD` to appropriate values.)
As you may have guessed, you will need to do some software debouncing inside your `isShaking` function
in order to detect when the board is still being actively shaken and when it is at rest.

Your `SysTickHandler()` should be responsible for checking and updating application state 
according to the finite state machine defined above. To help you out, we included this code in `main.c`:

```
#define STARTING 00
#define WAITING  01
#define SHAKING  10
#define DISPLAY  11

uint8_t state = STARTING;
```

and we've also set up a `switch` statement inside `SysTickHandler()`.

## Submit your work

To submit your work, please open a terminal, navigate to the `5-timing-interrupts` folder, and run:

    git add REPORT.md
    git add digital-dice-sm/src/main.c
    git commit -m "Please grade this lab5-timing-interrupts submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
