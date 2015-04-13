Lab 5: Timing and Interrupts
============================

Lab Assignment | 5 - Timing
-------------- | -----------------------
Name             |
Net ID           |
Report due       | Thursday, 9 April 2015


Please answer the following questions:

1) The following lines of code can be found in the `main.c` source file for the `interrupt-button` exercise.

Comment *each* line of code, explaining its purpose. (Not just the semantics of what it does, but what its *purpose* is.)

```
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

// Configure the interrupt using library functions
EXTI_InitStructure.EXTI_Line = EXTI_Line0;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

// Enable and set priorities for the EXTI0 interrupt in NVIC
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);                                 
```

2) Show the output of `print risingCounter` and `print risingCounter` in `gdb` that demonstrates switch bouncing.


```

```

3) Explain a method of switch debouncing using extra circuitry.


4) Explain a method of switch debouncing in software.


5) Explain the circumstances that trigger the bug in the
`interrupt-button` program where the LEDs stay lit even
when the button is released. What conditions will cause
this bug to occur?

6) In the `interrupt-button` program, in the ISR, we
have the following lines of code:

```
if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
  GPIO_SetBits(GPIOD, LEDS);
}
if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
  GPIO_ResetBits(GPIOD, LEDS);
}
```

Will these two `if` blocks always be evaluated in the
order in which they are written in the code, or
may the compiler reorder them? Explain.

7) Calculate the speed of each of the peripheral buses
given the system clock speed and the prescalers
in the `system_stm32f4xx.c` file.

Bus   | Speed (MHz)
----- |------------
AHB   |
APB1  |
APB2  |



8) What line of code in `system_stm32f4xx.c` would you
change to clock the APB1 peripheral bus at 84 MHz? What would you change it to? Try changing this value and run the
`timing-pwm` program again. What is the effect of this change
on the program?


9) Write an `initTimer4()` and `initPwm()` function to pulse the green LED every 
20 ms, with a duty cycle of 80%.


10) What is the value of the `lr` at the first line of `TIM2_IRQHandler`? What
does this special value indicate?

11) Why are hardware-timer-controlled PWM channels more valuable than CPU-controlled PWM 
for outputs with very specific timing requirements? Explain, given what you've observed in your `timing-pwm` experiment.


