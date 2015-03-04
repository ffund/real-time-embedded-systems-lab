Lab 3: Analog to digital conversion
===================================

The aim of this lab is for you to:

 * Enable an ADC channel on your STM32F4 Discovery board

 * Use the ADC to convert analog values from a temperature sensor to digital values

 * See the effect of ADC precision on temperature values


Note that the temperature sensor on the STM32F4 Discovery board 
is measuring the thermal chip junction temperature (actual temperature of semiconductor surface),
not the ambient temperature.

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

(If you've already done this to your working copy in the last lab, 
you don't have to do it again - but it won't break anything if you do.)

Pull changes (if any) from your own copy of the repository:

    git pull origin

Then merge in changes from the public course repository:

    git fetch origin -v
    git fetch upstream -v
    git merge upstream/master


### Set up ADC

To complete this lab, you'll make some minor changes in `temperature/src/main.c`.

Most of the ADC code has already been set up for you. Look in the 
`initTempSensor()` function to see how it's done.

In that function, most of the ADC code is "boilerplate" code; we enable the clock 
signal to the ADC, set up an ADC peripheral with default values, 
enable a temperature sensor on it, and enable the ADC.

Refer to the ADC library header and source files
(`stm32f4xx_adc.h` and `stm32f4xx_adc.c`) for more details.

You do need to make some small modifications here: in between  `ADC_StructInit(&adc)`
and `ADC_Init(ADC1, &adc)`, add two lines of code using the `adc` struct to:

 * **Set the ADC resolution to 12 bits.** The ADC on the STM32F4 Discovery board
can operate at several different resolutions: 12 bits, 10 bits, 8 bits, and 6 bits.
12 bits is actually the default, so it's already set, but since we'll change 
it at a later step, it's useful to set it explicitly here.

 * **Enable continuous conversion mode.** This lets the the ADC work in the background,
and convert analog data without intervention from the CPU. See section 1.3 of this 
[application note](http://www.st.com/web/en/resource/technical/document/application_note/CD00258017.pdf)
for more details.


Verify that your code compiles (run `make`) before moving on to the next step.

### Set up temperature conversion function

In the `readTemp()` function, we're going to print the temperature reading 
in three ways:

 * **Digital value** returned by ADC. The digital value returned by the ADC 
is assigned to a variable `temperature`.
 
 * **Voltage**. This was (approximately) the input to the ADC.
In this function, convert the ADC output back to a voltage using the formula on page 20 in the 
[lecture slides on peripherals](http://witestlab.poly.edu/~ffund/el6483/files/lecture4-peripherals.pdf).
Make sure you use float constants so that you don't accidentally do integer division when 
you mean to do floating point division. Assign the value back to the `temperature` variable.

 * **Temperature value**. The formula for converting the voltage to a temperature value is given 
in the [Reference Manual](http://witestlab.poly.edu/~ffund/el6483/files/DM00031020.pdf), Section 13.10, 
as  `((V_sense – V_25) / Avg_Slope) + 25`. Refer to the 
[datasheet](http://witestlab.poly.edu/~ffund/el6483/files/DM00037051.pdf), Section 5.3.21, for 
the values of V_25 and Avg_Slope. Assign the value back to the `temperature` variable.


### Run your code

When you've finished, run `make debug` (and `make openocd` in a second terminal) to run `gdb`.
Once your code is loaded onto the board, type `continue` and look at the `openocd` window -
you should see lines being printed to the screen, like this:

```
1069.000000, 0.861465, 65.586090
1068.000000, 0.860659, 65.263748
1066.000000, 0.859048, 64.619064
1067.000000, 0.859854, 64.941406
```

In each line, the first value is the digital value returned by the ADC, the 
second value is the estimate of voltage, and the last is the temperature value.


Save about 30 (sequential) lines of this to a plaintext file, and name it `12b.csv`. 
(This file will be submitted.)


### Test effect of ADC resolution

Repeat the previous steps, but this time, set your ADC to use 10 bits 
of precision. You will need to change a value in `initTempSensor()`
and a value in `readTemp()`.

Save 30 (sequential) lines of output to a plaintext file, and name it `10b.csv`.

Repeat for 8 bits and 6 bits of resolution, saving 30 (sequential) lines of data 
to `8b.csv` and `6b.csv`.


## Submit your work

To submit your work, first restore your code to the 12-bit resolution version.
Complete the `REPORT.md`. Then, open a terminal, navigate to the `3-adc-temp` folder, and run:

    git add REPORT.md
    git add temperature/src/main.c
    git add 12b.csv
    git add 10b.csv
    git add 8b.csv
    git add 6b.csv
    git commit -m "Please grade this lab3-adc-temp submission"
    git push -u origin master


You are also required to visit one of the
[TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours)
with your laptop and development board, and show your work to a TA in person.
