#include "LCD.h"
#include "pigpio.h"
#include <cstdint>
#include <unistd.h>
#include <iostream>

// Commands
#define    LCD_CLEAR    0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20


// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00


// flags for backlight control
#define LCD_BACKLIGHT 0x08


const uint8_t lcdRowOffset[] = {0x80, 0xC0, 0x14, 0x54};

LCD::LCD(const uint8_t bus, uint8_t addr, uint8_t width, bool backlight_on, uint8_t RS, uint8_t RW, uint8_t E,
         uint8_t B4) {
    m_i2cHandle = i2cOpen(bus, addr, 0);
    m_backlight_on = backlight_on;
    m_RS = 1 << RS;
    m_E = 1 << E;
    m_B4 = B4;
    m_displayFunction = LCD_FUNCTIONSET | LCD_2LINE | LCD_4BITMODE | LCD_5x8DOTS;
    m_displayControl = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;;
    m_displayMode = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    init();
    clear();
}

LCD::~LCD() {
    i2cClose(m_i2cHandle);
}

/*
 * Initialise the LCD screen
 */
void LCD::init() const {
    write4bits(0x3 << 4);
    usleep(4500);
    write4bits(0x3 << 4);
    usleep(4500);
    write4bits(0x3 << 4);
    usleep(150);
    write4bits(0x2 << 4);
    sendCommand(m_displayFunction);
    sendCommand(m_displayControl);
    sendCommand(m_displayMode);
    goHome();
}

void LCD::sendCommand(const uint8_t cmd) const {
    uint8_t MSN = (cmd >> 4) & 0x0F;
    uint8_t LSN = cmd & 0x0F;
    uint8_t MSb = MSN << m_B4;
    uint8_t LSb = LSN << m_B4;
    sendByte(MSb, LSb);
}

/*
 * Display a char
 */
void LCD::putChar(const uint8_t bits) const {
    uint8_t MSN = (bits >> 4) & 0x0F;
    uint8_t LSN = bits & 0x0F;
    uint8_t MSb = (MSN << m_B4) | m_RS;
    uint8_t LSb = (LSN << m_B4) | m_RS;
    sendByte(MSb, LSb);
}

void LCD::sendByte(uint8_t msb, uint8_t lsb) const {
    if (m_backlight_on) {
        lsb = lsb | LCD_BACKLIGHT;
        msb = msb | LCD_BACKLIGHT;
    }
    write4bits(msb);
    write4bits(lsb);
}

void LCD::write4bits(uint8_t value) const {
    i2cWriteByte(m_i2cHandle, value | m_E);
    usleep(1);
    i2cWriteByte(m_i2cHandle, value & ~m_E);
    usleep(50);
}

/*
 * Move the cursor to a position
 */
void LCD::setPosition(const uint8_t x, const uint8_t y) const {
    sendCommand(lcdRowOffset[y] + x);
    usleep(2000);
}

/*
 * Clear the LCD screen
 */
void LCD::clear() const {
    sendCommand(LCD_CLEAR);
    usleep(2000);

}

/*
 * Write a string
 */
void LCD::puts(const char *str) {
    while (*str)
        putChar((uint8_t) *(str++));
}

/*
 * Set the cursor to position 0,0
 */
void LCD::goHome() const {
    sendCommand(LCD_RETURNHOME);
    usleep(2000);
}

/*
 * Enable/Disable backlight
 */
void LCD::enableBacklight(bool backlight_on) {
    m_backlight_on=backlight_on;
    i2cWriteByte(m_i2cHandle, m_backlight_on ? LCD_BACKLIGHT : 0);
}

/*
 * Return the status of backlight
 */
bool LCD::getBacklight() const {
    return m_backlight_on;
}

/*
 * Enable/Disable cursor
 */
void LCD::enableCursor(bool enable) {
    m_displayControl= enable ? (m_displayControl | LCD_CURSORON) : (m_displayControl & ~LCD_CURSORON);
    sendCommand(m_displayControl);
}

/*
 * Enable/Disable cursor blinking
 */
void LCD::enableBlinking(bool enable) {
    m_displayControl= enable ? (m_displayControl | LCD_BLINKON) : (m_displayControl & ~LCD_BLINKON);
    sendCommand(m_displayControl);
}

LCD&  LCD::operator<<(const char *chaine)  {
    puts(chaine);
    return *this;
}