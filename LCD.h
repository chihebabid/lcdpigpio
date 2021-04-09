#ifndef LCD_H_
#define LCD_H_

class LCD {
private:
	int16_t m_i2cHandle;
	bool m_backlight_on;
	uint8_t m_RS,m_E,m_BL,m_B4;
	void init();
public:
	LCD(uint8_t bus,uint8_t addr,uint8_t width=16, bool backlight_on=true, uint8_t RS=0, uint8_t RW=1, uint8_t E=2,  uint8_t BL=3,uint8_t B4=4);
	virtual ~LCD();
	void sendCommand(uint8_t cmd);
	void sendByte(uint8_t lsb,uint8_t msb);
	void setPosition(uint8_t x,uint8_t y);
	void putData(uint8_t bits);
	void lcdPuts (const char *str);
	void clear();
};

#endif /* LCD_H_ */



