# Arduino_mini_lab

Small development/test plateform using of the shelves items that were lying in my drawers.
There is an Arduino mini pro, an 80x160 TFT display some buttons, a joystick, a battery charger and an FTDI tool

I have written a small library that inherit from UTFT in order to have a little GUI for rapid prototyping.
I have added to UTFT a few method to init the display, read the buttons and the joystick, display battery state

The joystick is not that reliable it should be replaced with a more robust one.
At startup, there is a calibration phase during which the value of the joystick is sampled in order to determine a rest value for both axis. After the calibration period, if you move the joystick it never returns to the rest value.
