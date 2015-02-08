Lab 1: Blinky
====================

The aim of this lab is for you to:

 * Set up a toolchain and development environment for completing this and future lab exercises

 * Run a simple experiment to blink the LED on your STM32 F4 Discovery board

 * Practice using debugging tools in your development environment


Please read this *entire document* once before beginning.

## Collaboration policy

Lab exercises are to be completed individually. You must keep your Bitbucket repository private, and you may not share a code file with another student.

You may ask other students for help by email, but if you do so,

1. You should not share source code
2. You should include the entire email chain - the request for help, the response, and any futher communication about the lab - in an appendix to your lab report.

You are encouraged to make use of the [TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours) for help with the lab exercises.


## Lab procedure

### Set up development environment


We will use an open-source toolchain for all of the lab exercises. Instructions are provided for Ubuntu 14.04. If you are running another operating system, you may install an Ubuntu partition or use an Ubuntu Live USB to run the lab exercises.

Using an Ubuntu virtual machine is *not* recommended, because setting up the USB connection between the host and virtual machine (in order to program the STM32 via USB) may be somewhat complicated.

Further instructions are available online:

 * for [creating a Live USB on Windows](http://www.ubuntu.com/download/desktop/create-a-usb-stick-on-windows)
 * for [creating a live USB on Mac OS X](http://www.ubuntu.com/download/desktop/create-a-usb-stick-on-mac-osx)
 * for [installing Ubuntu in a separate partition alongside your existing operating system](http://www.ubuntu.com/download/desktop/install-ubuntu-desktop)

Make sure to leave at least 2GB of extra space on your Ubuntu partition or your USB device, for persistent files.

If you are using the Ubuntu Live USB approach and are having trouble, the TAs can help you create your Live USB. Bring a 4GB or larger USB key to TA office hours for assistance.


You should be comfortable with the Linux shell (terminal). If you're not, there are plenty of materials online that can help you get up to speed. [Here is one tutorial you can try](http://cli.learncodethehardway.org/book/). You don't need to completely master the terminal, but you shouldn't be scared if you're expected to use it.


Once you are in your Ubuntu 14.04 environment and are ready to start, you'll need to install some tools.

#### Get comfortable with `git` and Markdown

First, you will install `git`. Git is the most widely adopted version control system for software development, and we're using it in this course to share code with you and receive your submissions for grading.

To install `git`, open a terminal and run:

    sudo apt-get install git

We recommend setting up key-based authentication in order to use  `git` with private repositories on Bitbucket. You can find instructions for setting this up [here](https://confluence.atlassian.com/pages/viewpage.action?pageId=270827678).

Once that's done, you can get a local copy of your fork as follows. Open a terminal and run:

    git clone git@bitbucket.org:USERNAME/el6483-NETID.git

where `USERNAME` is your Bitbucket username and `NETID` is your NYU Net ID. (You should have named your fork `el6483-NETID`.) You can also find this address on the Bitbucket home page for your repository:

![Find SSH URL](http://i.imgur.com/VsbpZet.png)

You'll submit your work for grading by editing a Markdown file and then pushing a **commit** to your fork.

To practice this workflow now, find the file called `REPORT.md` inside the `1-blinky` folder.  You'll write your lab report inside this file.

The REPORT.md file is written in Markdown. A helpful tutorial on Markdown is available [here](https://bitbucket.org/tutorials/markdowndemo). You may also be interested in using a Markdown editor with live preview, such as [Atom](http://www.webupd8.org/2014/05/install-atom-text-editor-in-ubuntu-via-ppa.html). Here's what this page looks like inside the Atom editor:

![Atom editor](http://i.imgur.com/jTTwWCT.png)

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

In order to do this type of debugging you need several things: a board with an on-chip debugger, a program that controls the chip’s debugging mode, a way to interface with it, and a compile that can compile programs with debugging information.


Instead of a single tool to do all these things, we'll use many tools working together. Collectively, these tools are called a toolchain (since they are "chained" together to produce a final executable application).

#### Install ARM development tools



First, you will need to install some prerequisites:

    sudo apt-get install build-essential libusb-1.0-0-dev

Install an up-to-date version of the `gcc` ARM toolchain:

    sudo apt-get remove binutils-arm-none-eabi gcc-arm-none-eabi
    sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
    sudo apt-get update
    sudo apt-get install gcc-arm-none-eabi=4.9.3.2014q4-0trusty12

Also build and install the `st-link` utilities for communicating with the STM32 F4 Discover board:

    git clone https://github.com/texane/stlink.git
    cd stlink
    ./autogen.sh
    ./configure
    make
    make install




## Submit your work

To submit your work, please open a terminal, navigate to the `1-blinky` folder, and run:

    git add REPORT.md
    git add
    git commit -m "Please grade this lab1-blinky submission"
    git push -u origin master


You are also required to visit one of the [TA lab hours](http://witestlab.poly.edu/~ffund/el6483/#hours) with your laptop and development board, and show your work to a TA in person.
