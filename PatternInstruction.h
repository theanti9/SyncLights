typedef struct color {
  byte r;
  byte g;
  byte b;
} Color;

class PatternInstruction {
  public:
    PatternInstruction(uint8_t _x, uint8_t _y, Color c) {
      x = _x;
      y = _y;
      setColor(c);
      time_delay = 0;
    }
    PatternInstruction(uint8_t _x, uint8_t _y, Color c, uint16_t timed) {
      x = _x;
      y = _y;
      setColor(c);
      time_delay = timed;
    }
    
    uint32_t getColor() {
      return color;
    }
    uint8_t getTime() {
      return time_delay;
    }
    
    uint8_t getX() {
      return x;
    }
  
    uint8_t getY() {
      return y;
    }  
  private:
    uint8_t x;
    uint8_t y;
    uint16_t time_delay;
    uint32_t color;
    uint32_t setColor(Color c) {
      color = c.r;
      color <<= 8;
      color |= c.g;
      color <<= 8;
      color |= c.b;
    }
};
