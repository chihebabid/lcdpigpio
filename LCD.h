#ifndef LCD_H_
#define LCD_H_
#include <cstdint>
#include <ostream>

class LCD {
private:
	int16_t m_i2cHandle;
	bool m_backlight_on;
	uint8_t m_RS,m_E,m_BL,m_B4;
	uint8_t m_displayFunction; // Mode 4/8bits ; #lines ; font
    uint8_t m_displayControl;
    uint8_t m_displayMode;
	void init() const;
	void write4bits(uint8_t value) const;
    void sendCommand(const uint8_t cmd) const;
    void sendByte(uint8_t lsb, uint8_t msb) const;
public:
	LCD(const uint8_t bus, uint8_t addr, uint8_t width = 16, bool backlight_on = true, uint8_t RS = 0, uint8_t RW = 1, uint8_t E = 2,
			uint8_t B4 = 4);
	virtual ~LCD();
	void setPosition(const uint8_t x, const uint8_t y) const;
	void putChar(const uint8_t bits) const;
	void puts (const char *str);
	void clear() const;
	void goHome() const;
    void enableBacklight(bool backlight_on=true);
    bool getBacklight() const;
    void enableCursor(bool enable=true);
    void enableBlinking(bool enable=true);
    //void print(std::ostream &stream) const;
    LCD&  operator<<(const char *chaine) ;
};


#endif /* LCD_H_ */



