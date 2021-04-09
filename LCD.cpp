#include <cstdint>
#include <unistd.h>
#include "pigpio.h"
#include "LCD.h"

#define	LCD_CLEAR	0x01
char lcdRowOffset[4]={0x80, 0xC0, 0x14, 0x54};

LCD::LCD(uint8_t bus, uint8_t addr, uint8_t width, bool backlight_on, uint8_t RS, uint8_t RW, uint8_t E, uint8_t BL, uint8_t B4) {
	m_i2cHandle = i2cOpen(bus, addr, 0);
	m_backlight_on = backlight_on;
	m_RS = 1<<RS;
	m_E = 1<<E;
	m_BL = 1<<BL;
	m_B4 = B4;
	init();
}

LCD::~LCD() {
	// TODO Auto-generated destructor stub
}
/*
 * Initialise the LCD screen
 */
void LCD::init() {
	sendCommand(0x33); //# Initialise 1
	usleep(2000);
	sendCommand(0x32); //# Initialise 2
	//# Cursor increment
	sendCommand(0x06);
	sendCommand(0x0F); //# Display on,move_to off, blink off
	sendCommand(0x28);
	sendCommand(LCD_CLEAR); //# 4-bits, 1 line, 5x8 font
	//sendCommand(0x08); //???
	 //# Clear display
	sendCommand(0x80);


}

void LCD::sendCommand(uint8_t cmd) {
	uint8_t MSN = (cmd >> 4) & 0x0F;
	uint8_t LSN = cmd & 0x0F;
	uint8_t MSb = MSN << m_B4;
	uint8_t LSb = LSN << m_B4;
	sendByte(MSb, LSb);
}

void LCD::putData(uint8_t bits) {
	uint8_t MSN = (bits>>4) & 0x0F;
	uint8_t LSN = bits & 0x0F;
	uint8_t MSb = (MSN << m_B4) | m_RS;
	uint8_t LSb = (LSN << m_B4) | m_RS;
    sendByte(MSb, LSb);
}

void LCD::sendByte(uint8_t msb, uint8_t lsb) {
	if (m_backlight_on) {
	lsb = lsb | m_BL;
	msb = msb | m_BL;
	}
	i2cWriteByte(m_i2cHandle, msb | m_E);
	usleep(1);
	i2cWriteByte(m_i2cHandle, msb & ~m_E);
	usleep(200);
	i2cWriteByte(m_i2cHandle, lsb | m_E);
	usleep(1);
	i2cWriteByte(m_i2cHandle, lsb & ~m_E);
	usleep(200);


}
void LCD::setPosition(uint8_t x,uint8_t y) {
	sendCommand(lcdRowOffset[y]+x);

}

void LCD::clear() {
	sendCommand(LCD_CLEAR);
}

void LCD::lcdPuts (const char *str)
{
  while (*str)
	  putData ( (uint8_t)*(str++)) ;
}
