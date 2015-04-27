Lab 7: Internet of things
===================================

In this lab, you will develop a product for the Internet of Things:
a night light and lullaby player marketed to parents of babies
and small children. The volume of the lullaby, the color of the 
night light, and the device state (on/off) is adjustable via Bluetooth.


This lab is very important, as it incorporates many of the lessons
you've learned in previous labs and lectures as well as some new concepts:

 * Use of GPIO pins in various modes: output, input, analog input, alternate function

 * Use of communication peripherals (UART)

 * Use of ADC (for reading from light sensor)

 * Use of PWM output (for adjusting light color)

 * Use of interrupts and interrupt priorities (for incoming Bluetooth data, light sensor ADC trigger, wake from low power state via button press or Bluetooth connection)

 * Use of special data structures (circular buffer, in this lab) to avoid data corruption when multitasking or using interrupts

 * Internet of Things concepts: wireless connectivity, design of low-power systems

 * Use of incremental, test-driven process to implement a complex design


Please read this *entire document* once before beginning.

### What to submit

This lab exercise is meant to emulate the kind of work you may do in a 
professional embedded systems environment. Therefore, there is no lab report. 
Instead, you will be graded on your code and programming habits:

 * Whether you have engaged in incremental development, dividing the system 
 into small components and testing at each step.
 * Whether your code is reasonably clean, without remnants of previous failed tries.
 * Whether the style of your code is reasonable and consistent. Put every statement on its own line, avoid inconsistent indentation that could mislead 
 the reader into seeing blocks where there are none, etc.
 * Whether your code is well documented. You do not need to document every line, but you should at least record your thought process for design decisions
 and non-obvious choices.
 * Whether you record your ongoing progress in source control (git) as you work, instead of pushing all your code at once at the end.


### Collaboration policy

Lab exercises are to be completed individually. You must keep your Bitbucket repository private,
and you may not share source code files with another student.

You may ask other students for help by email, but if you do so,

1. You should not share source code
2. You should include the entire email chain - the request for help, the response, and any
futher communication about the lab - in an appendix to your lab report.

You are encouraged to make use of the [TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours) for help with the lab exercises.

### Bring your working copy up to date

Start by updating your local git repository to include the latest code and instructions from the course repository. 
You should do this regularly before any activity on the lab, in case there have been 
any corrections or clarifications to the instructions.

You should already have a copy of your repository checked out in your development environment. Navigate to that directory. Then run:

    git remote add --track master upstream https://bitbucket.org/el6483s2015/el6483-lab.git

(If you've already done this to your working copy in the last lab, 
you don't have to do it again - but it won't break anything if you do.)

Pull changes (if any) from your own copy of the repository:

    git pull origin

Then merge in changes from the public course repository:

    git fetch origin -v
    git fetch upstream -v
    git merge upstream/master

Finally, push your updated repository to Bitbucket:

    git push -u origin master


## The scenario

You have just been hired as an embedded systems engineer at a small company 
that designs products for home automation. The first project you are 
assigned to is the development of a Bluetooth-controlled nightlight and 
lullaby player, marketed to parents of very young children.

The product is to have several features that distinguish it from the competition:

 * The device has an auto light setting, in which the light turns on automatically in a dark room, and turns off again when the room becomes bright. This setting can be disabled.
 *  The color of the light, the volume of the lullaby, and the device state (on/off) can be adjusted over Bluetooth using a smartphone app
 * The device may be put into a low power mode to save energy. However, it wakes automatically if a Bluetooth connection is established, or if the button on the device is pressed.

The user interface designers have shown you a mockup
of the smartphone app, to help guide your design, and the industrial design team have shown you this rendering of what 
the final product might look like:


![light](http://i.imgur.com/U1UOCcT.png)


### Circuit prototype

You've been hired following the departure of the project's original engineer,  who was so successful on her last project that she was recruited into a better-paying job at a much more prominent company. She had only just started working 
on the night light project. She left you a prototype circuit on a breadboard, 
and a repository in the company's source control system.

Here's the circuit she left you:

![circuit](http://i.imgur.com/7QYM2rb.jpg)

The engineer had started coding the project, and her code is all 
available to you in the company's source control system. Specifically, inside 
the main source folder, you notice that the project seems to be organized into 
several submodules:

 * leds
 * lightsensor
 * rgbled
 * bluetooth
 * buffer
 * power

There are also two separate folders, named 'audio' and 'helix', each with some code in it. Here, the module of interest is the code in 'audioutils' in the 'audio' folder.

### Learn the code

Your first step on inheriting this code base should be to learn how to use 
the key functionality of the various modules.

For each module,

 * leds
 * lightsensor
 * rgbled
 * bluetooth
 * buffer
 * power
 * audioutils

write a function that uses the important functionality of the module, and verifies that it is working properly (independently of the other modules). Your test function should be declared in the module's header file
and defined in the module's source file. Make sure to document every function!

Try running each test function, one after the other, in `main` and verify 
that you know how to use each module.

When you have completed a new test function, commit the files you modified 
with `git` and push them to Bitbucket. (Don't wait until the end of the lab 
to push your files; you should do this as you go along.)

### Implement the system


Now, it's time to integrate these submodules and implement the actual system you've been assigned to build. Your final implementation should meet all the specifications described above.

There are some hints spread throughout `main` in comments on how the system should be implemented. For example,

 * Use flags to keep track of system state. Set flags in ISRs, then check their values and act accordingly in the main `while()` loop.
 * Use a circular buffer to keep incoming data from Bluetooth (received in the RX interrupt) and process it in the main `while()` loop.

You should also follow these design guidelines:

 * You should definitely debounce your User button press input!
 * The RGB LED will affect the reading of the light sensor. If you define
 a single threshold (`LIGHT_THRESHOLD_HIGH` == `LIGHT_THRESHOLD_LOW`), above which the LED will be off and below which the LED will be on, you will find that if the light reading is just below that threshold, then the LED will constantly blink on and off. This behavior is not desirable in a nightlight! Fortunately, the ADC we are using lets us define two thresholds, a low threshold and a high threshold. By keeping enough distance between these 
 thresholds, we can avoid the annoying oscillation.
 * When music is not playing, make sure the audio DAC is off, not playing 
 unpleasant noise.
 * When the system returns from a low power mode, you should restore its previous state (light status, light color, audio status and audio volume).
 * Use the on-board LEDs on the Discovery to show system status (e.g. system in normal power mode or not, Bluetooth connected or disconnected, music playing or not playing, etc.).
 * You will need to add your own "framing" to the data you send over the 
Bluetooth serial connection, in order to distinguish between color settings, volume settings, or other settings. For example, you may decide to first send a '#' character if you are planning to set a volume setting, and a '$' character if you are going to send a color setting. Then, in the part of your 
program where you read from the circular buffer, you can check the framing
character and act accordingly.
 * Ideally, your system should be somewhat resilient to dropped bytes over the air interface and/or Bluetooth connections that are dropped in middle of 
 sending a new setting.

As you work, document your code and push your work in progress to Bitbucket. 
(Don't wait until the end to document or push to git.)


### Test power usage

Power control is an important consideration in designing systems for the Internet of Things. Now that you've implemented your system, you 
should find out more about its power usage.

There are two major sources of power consumption in this system,
the MCU and the peripherals on the breadboard.

To measure the current on the MCU, you will remove the jumper JP1 
and attach multimeter leads across those pins:

![mcupower](http://i.imgur.com/h7TyB8R.jpg)

To measure the current to peripherals on the breadboard, you will put the multimeter in series with the 3V power supply to the breadboard. Remove the orange cable from the boardboard and connect it to one of the multimeter leads. Put a jumper cable in the hole where the orange cable would have been 
plugged in to the breadboard, and connect it to the other multimeter lead:

![peripheral](http://i.imgur.com/ojHJFiY.jpg)


Fill out the following table and include it in a multiline comment near the top of the `power.c` source file.

 
Measurement    |   State           |   Measured current  (mA) 
-------------- | ------------------|---------------------------
MCU            |  Normal           |
MCU            |  Sleep            |
MCU            |  Stop             |
Breadboard     |  BT discoverable  |
Breadboard     |  BT connected     |
Breadboard     |  BT Rx/TX         |


## Appendices

### Appendix A: Pins

This pins were selected because of their mappings to specific alternate functions. See Table 5, "MCU pin description versus board function," in the 
[User manual](http://www.st.com/st-web-ui/static/active/cn/resource/technical/document/user_manual/DM00039084.pdf) for more details.

Breadboard            | Discovery board | Function
--------------------- | ----------------|------------------------------
[Blk] Ground          | GND             | Ground
[Red] RGB LED red     | PC6             | PWM output, TIM3 PWM Channel 1 
[Grn] RGB LED green   | PC9             | PWM output, TIM3 PWM Channel 4 
[Blu] RGB LED blue    | PC8             | PWM output, TIM3 PWM Channel 3 
[Ylw] Photoresistor   | PC0             | Analog input, ADC1 Channel 10
[Wht] Bluetooth TX    | PA10            | USART1, RX 
[Gry] Bluetooth RX    | PA9             | USART1, TX 
[Prp] Bluetooth State | PD1             | Digital input
[Orn] 3V              | 3V              | High voltage
                | PA0             | User button (power control)
                | PD12            | On-board LED
                | PD13            | On-board LED
                | PD14            | On-board LED 
                | PD15            | On-board LED 
                | PD4             | CS43L22 (audio DAC) reset
                | PB6             | I2C1 SCL
                | PB9             | I2C1 SDA
                | PC7             | I2S MCK (master clock)
                | PC10            | I2S SCK (serial clock)
                | PC12            | I2S SD (serial data)
                | PA4             | I2S WS (word select/word clock)



### Appendix B: Resistive Sensors

In this lab, we are using a standard light sensor ([photoresistor](http://en.wikipedia.org/wiki/Photoresistor)) to measure ambient light levels. 
A photoresistor is an example of a *passive resistive sensor*--"passive" meaning that it **must** be powered by an external power source! 


The resistance of a photoresistor decreases as the incident light intensity increases.  Many sensors used in the real world, like our photoresistor, are resistive sensors - that is, their resistance varies according to ambient conditions. For example, a potentiometer varies its resistance according to position, and a force sensitive resistor varies its resistance according to the force that is applied to it. 


Having a variable resistance *on its own* is not sufficient to read in an analog value. The analog inputs on our board measure changes in voltage, 
not resistance. To translate the resistance into voltage, we must add another resistor and build a **voltage divider**.

In the image below, R2 represents a basic passive resistive sensor, and R1 represents a simple resistor. Notice that if we did not have this voltage divider in our circuit, we would not be able to measure variations in voltage at Vout.

![Voltage Divider](https://cdn.sparkfun.com/assets/7/2/a/7/5/511acd39ce395f6746000000.png)

In this circuit, if R2 increases in resistance, then Vout will increase in voltage. If R2 decreases in resistance, then Vout will decrease.
