//============================================================================
// Name        : pigpioLED.cpp
// Author      : CA ABID
// Version     :
// Copyright   : 2021
// Description : Test sample using LCD driver
//============================================================================

#include "LCD.h"
#include <iostream>
#include <unistd.h>
#include <pigpio.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (gpioInitialise() < 0) {
		cout << "Failure..." << endl;
		exit(-1);
	}
	// Initialize the LCD driver
	LCD lcd(1, 0x27);


	while (1) {
		lcd.setPosition(0, 0);
		lcd.putData(65);  // Put char 'A'
		sleep(1);
		lcd.setPosition(0, 1);
		lcd.putData(66); // Put char 'B'
		sleep(1);
		lcd.setPosition(0, 0);
		lcd.putData(67); // Put char 'C'
		sleep(1);
		lcd.setPosition(0, 1);
		lcd.putData(68); // Put char 'D'
		lcd.puts("ABC"); // Put string "ABC3
		sleep(1);
	}
	gpioTerminate();
	return 0;
}

