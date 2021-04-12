# A C++ 16x2 LCD library using i2c
An i2c 1602 LCD library written in C++ and it is based on pigpio library for Raspberry OS
It uses the i2c expander PCF8574.

## Usage
Assuming pigpio library is installed, simply add class files to your project.

After initialization of pigpio library, it is possible to create an instance of LCD. It is sufficient to precise the i2c bus and the address of PCF8574.

	if (gpioInitialise() < 0) {
		cout << "Failure..." << endl;
		exit(-1);
	}
	
	LCD lcd(1, 0x27); `` 

By default, the display will be on and no cursor is displayed

To enable cursor, use ``lcd.enableCursor()``, or ``lcd.enableCursor(false)``

To disable cursor, use ``lcd.enableCursor(false)``
