Lab 4: Accelerometer and SPI
============================

Lab Assignment | 4 - Accelerometer
-------------- | -----------------------
Name           | 
Net ID         | 
Report due     | Wednesday, 11 March 2015


Please answer the following questions:

1) Which GPIO pins are used by the MEMS accelerometer for each 
of the four SPI lines? (Refer to page 40 of the 
[user manual](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/user_manual/DM00039084.pdf) 
for the STM32F4 Discovery board.)

SPI Line         | Port and pin number
---------------- | --------------------
SCK (clock)      | 
MOSI             | 
MISO             | 
CS (chip select) |


2) Read the `initAccelerometer()` function. Explain in your own words
how the SPI interface is configured and used for communication with the accelerometer.


3) What are your zero-G offsets in the X, Y, and Z directions?
Show 10 lines of output from your board when it is 
sitting on a flat surface. Then fill in the table.


```
put output lines here
```

Axis         | Offset
------------ | --------------------
X            | 
Y            | 
Z            |

4) Show a screenshot of your `gdb` window at the first step 
_after_ the CS pin is set to low.


5) Show the code you use to light the LEDs according to the board's orientation.

```
fill in the relevant lines of code here.

```


6) In the `accel-whoami` program, we are reading from a register 
on the device that is configured to return a default value. Based on the value
that is returned, we can identify the device as a LIS3DSH or a LIS302DL.

The SPI Read protocol is described in Section 6.2.1 (Figure 7) of the 
[LIS3DSH datasheet](http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf).
Create a timing diagram showing exactly what happens (on all four lines) when you call
`readSPI(LIS3DSH_WHO_AM_I_ADDR)`. (The register addresses are defined in `main.h`
and are also given in Table 16 of the 
[LIS3DSH datasheet](http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf).)
Assume that the accelerometer returns the default value for this register, which is 
given in Section 8.3 of the datasheet.

Insert an image of your timing diagram here.

![timing diagram](http://path/to/image/file.png)







