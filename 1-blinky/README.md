Lab 1: Blinky
====================

The aim of this lab is for you to:

 * Set up a toolchain and development environment for completing this and future lab exercises
 * Run a simple experiment to blink the LED on your STM32 F4 Discovery board

Set up development environment
---------------------

We will use an open-source toolchain for all of the lab exercises. Instructions are provided for Ubuntu 14.04 and Mac operating systems. If you are running another operating, you may install an Ubuntu partition or use an Ubuntu Live USB to run the lab exercises.

If you are using the Ubuntu Live USB approach, the TAs can help you create your Live USB. Bring a 4GB or larger USB key to TA office hours for assistance.

### Ubuntu 14.04

These instructions are for Ubuntu 14.04. If you are running a different version of Ubuntu or a different version of Linux, you may need to modify these instructions.

You should be comfortable with the Linux shell (terminal). If you're not, there are plenty of materials online that can help you get up to speed, or you can visit the lab TA office hours for more help.

First, you will need to install some prerequisites:

    sudo apt-get install git build-essential libusb-1.0-0-dev

Install an up-to-date version of the <code>gcc</code> ARM toolchain:

    sudo apt-get remove binutils-arm-none-eabi gcc-arm-none-eabi
    sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
    sudo apt-get update
    sudo apt-get install gcc-arm-none-eabi=4.9.3.2014q4-0trusty12

Also build and install the <code>st-link</code> utilities for communicating with the STM32 F4 Discover board:

    git clone https://github.com/texane/stlink.git
    cd stlink
    ./autogen.sh
    ./configure
    make
    make install

At this point, you have all the software that is necessary for the blinky lab. Now you need to set up your development environment.




### Mac

Run blinky
---------------------
