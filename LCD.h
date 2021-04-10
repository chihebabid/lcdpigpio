#ifndef LCD_H_
#define LCD_H_
#include <cstdint>

class LCD {
private:
	int16_t m_i2cHandle;
	bool m_backlight_on;
	uint8_t m_RS,m_E,m_BL,m_B4;
	void init() const;
	void write4bits(uint8_t value) const;
public:
	LCD(const uint8_t bus, uint8_t addr, uint8_t width = 16, bool backlight_on = true, uint8_t RS = 0, uint8_t RW = 1, uint8_t E = 2, uint8_t BL = 3,
			uint8_t B4 = 4);
	virtual ~LCD();
	void sendCommand(const uint8_t cmd) const;
	void sendByte(uint8_t lsb, uint8_t msb) const;
	void setPosition(const uint8_t x, const uint8_t y) const;
	void putData(const uint8_t bits) const;
	void puts (const char *str);
	void clear() const;
};

#endif /* LCD_H_ */



