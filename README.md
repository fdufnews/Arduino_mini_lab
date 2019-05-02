# Arduino_mini_lab

## What it is
Small development/test platform using of the shelves items that were lying in my desk.
There is an Arduino mini pro, an 80x160 TFT display some buttons, a joystick, a battery charger and an FTDI tool

I have written a small library that inherit from UTFT in order to have a little GUI for rapid prototyping.
I have added to UTFT a few method to init the display, read the buttons and the joystick, display battery state

## Components
[TFT display](Doc/Components/small/IPS-0-96-pouce-7-p-SPI-HD-65-k-Couleur-1.jpg)
[Battery charger](Doc/Components/mall/sku_219454_1.jpg)
[FTDI tool]((Doc/Components/mall/criusFTDI1.jpg)
[Joystick](Doc/Components/mall/mini-joystick-slide-analogiqueXY.jpg)
[Tactile switch](Doc/Components/mall/tactileSwitches.jpg)

## Details
Test_miniArduino is a test software that only uses UTFT library it was used to test the board before the library was created.
Test_Arduino_mini_lab demonstrates what the library can do.

## Sidenotes
The joystick is not that reliable it should be replaced with a more robust one.
At startup, there is a calibration phase during which the value of the joystick is sampled in order to determine a rest value for both axis. After the calibration period, if you move the joystick it never returns to the rest value.

