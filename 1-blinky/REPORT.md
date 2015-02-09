Lab 1: Blinky
====================

Lab Assignment | 1 - Blinky
-------------- | -------------
Name           |
Net ID         |
Report due     | Tuesday, 17 February


Please answer the following questions:

1) The Makefile for this lab exercise defines, among other things, the compiler to use. (`CC` is a common shorthand used in makefiles for the compiler.) What is the name of the compiler used for this lab exercise?



2) The Makefile also defines certain *flags* that define compiler behavior. Fill in the following table, matching the flag to the behavior. The first one is completed for you as an example:

Behavior                                  | Flag
----------------------------------------- | -------------
Include debug info for GDB                | `-ggdb`
Compile for a little-endian target        |
Use the Thumb instruction set             |
Compile for a target with a Cortex M4 CPU |


3) The Makefile also instructs the linker to link files for STM libraries from three locations. What are those locations? (Give the full file path, not the line with variables given in the Makefile.)


4) The Makefile includes information about a startup file, which will be added to the executable file. Startup code is run just after a microcontroller is reset, before the main program. It's usually implemented as universal code for all same microcontroller type, so you won't need to write one from scratch for each program.

What is the location of the startup code? (Give the full file path, not the line with variables given in the Makefile.)

What form is the startup code given in? (C code, assembly, binary?)


5) There are five commands in the .gdbinit file. Look up each command online or in the `gdb` manual. List all five commands here, and explain what each one does.

6) What is in each of the ARM registers before execution starts?  Give the output
of `info registers` in the code block below.

```

```


7) Copy the assembly code for the `SysTick_Handler` function as shown by `gdb`
into the code block below.


```

```

8) Include a screenshot showing the "Toggling LED" output in your `openocd` window.
You can use any image hosting service to put your screenshot online.

![](http://url/of/image)

9) Include a screenshot showing the "Toggling LED from on to off"
and "Toggling LED from off to on" output in your `openocd` window.
You can use any image hosting service to put your screenshot online.

![](http://url/of/image)


10) If you stop `openocd` and `gdb`, and disconnect and reconnect the board from
the host computer, will the program (as given) keep running?
Explain the behavior you observe. What about your modified 'nodebug' program?
