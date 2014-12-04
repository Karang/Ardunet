#ifndef __HARDWARE_SERIAL_H__
#define __HARDWARE_SERIAL_H__

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

class HardwareSerial {
    private:
        void printNumber(unsigned long, uint8_t);
        void printFloat(double, uint8_t);
    public:
        HardwareSerial();
        void begin(int speed);
        
        void print(const char*s);
        void print(char, int = BYTE);
        void print(unsigned char, int = BYTE);
        void print(int, int = DEC);
        void print(unsigned int, int = DEC);
        void print(long, int = DEC);
        void print(unsigned long, int = DEC);
        void print(double, int = 2);

        void println(const char*s);
        void println(char, int = BYTE);
        void println(unsigned char, int = BYTE);
        void println(int, int = DEC);
        void println(unsigned int, int = DEC);
        void println(long, int = DEC);
        void println(unsigned long, int = DEC);
        void println(double, int = 2);
        void println(void);
};

extern HardwareSerial Serial;

#endif