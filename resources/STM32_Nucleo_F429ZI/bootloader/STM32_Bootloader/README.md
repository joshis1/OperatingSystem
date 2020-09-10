# Bootloader project for STM32
## This project has the following features

* Bootloader will jump to user application if the button is not pressed i.e it will run the user application - reset handler.
* The user application is stored at flash sector 2 i.e. starting address - 0x0800_8000
* The bootloader application is stored at the flash base address starting from sector 0 i.e. 0x0800_0000
* We have modified the user application linker file to tell the ROM starting address to 0x0800_8000 i.e. sector 2 flash address
* In the user application change the VECT_TAB_OFFSET  to 0x8000 in the file system_stm32f4xx.c
* The vector table offset needs to be changed since the user application is stored from the flash address - 0x0800_8000 i.e 0x8000 from Flash Base Address


### Bootloader features when the blue button is pressed.

* In order the bootloader to enter into this mode, press the blue button and reset the STM32 micro.
* In this mode, the user application will not be executed.
* Bootloader will wait for any UART communication from the host i.e. PC.
* Command Format - Length to follow( 1 byte) -> Command Code ( 1 byte) -> CRC ( 4 bytes).



*How to create .gitignore file*

* Firstly, create the file name - .gitignore
* git config --global core.excludesFile ~/.gitignore
* Modify .gitignore file to mention what git should ignore.

*References*

* https://www.atlassian.com/git/tutorials/saving-changes/gitignore
* https://www.st.com/en/development-tools/stsw-link004.html
* https://www.python.org/downloads/release/python-3510/
* https://www.amazon.com/Mark-Hammond-Python-Programming-Win32/dp/B008EU7452
* https://www.keil.com/support/docs/3913.htm#:~:text=The%20bootloader%20and%20the%20user,user%20application%20to%20execute%20it.
* https://stackoverflow.com/questions/4754152/how-do-i-remove-version-tracking-from-a-project-cloned-from-git/36945328#36945328
* In Application Programming - bootloader - https://www.keil.com/support/docs/3913.htm#:~:text=The%20bootloader%20and%20the%20user,user%20application%20to%20execute%20it.

*Host commands to run the python script*
* python -m pip install pyserial

#### Bootloader commands 

* How to jump to a particular location?
* The option is 5 and you can give the address of Reset Handler - Open the Map file of user application and check the address of Reset Handler - 0x08009540

* How to write the entire binary file to the sector 2 of the flash?
* The option is 8. Now, rename the user application to user_app.bin. Place this file in the same folder where Python file is present.
* Give the address where this hex file needs to be written, in my case I am trying to write this at Sector 2 of the flash i.e. 0x08008000

* How to erase the flash?
* Before you write anything to the flash, the flash needs to be erased.
* The option is 7, if you want mass erase i.e. entire flash then give 0xff otherwise give the starting sector number  and the number of sectors from there. 
* So, to erase the user application, give sector number 2 and number of sectors 2.
* To erase entire flash i.e. even bootloader then give 0xff. 




