#ifndef __HARDWARE_SERIAL_H__
#define __HARDWARE_SERIAL_H__

extern "C" {
    #include "esp_common.h"
}

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

class HardwareSerial {
    private:
        void uart_config(unsigned int baut_rate, UartBitsNum4Char data_bits, UartExistParity exist_parity, UartParityMode parity, UartStopBitsNum stop_bits, UartFlowCtrl flow_ctrl);
        void printNumber(unsigned long, uint8_t);
        void printFloat(double, uint8_t);
        void tx_one_char(char);
    public:
        HardwareSerial();
        void begin(long);
        
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
        
        void printBSSID(uint8*bssid);
};

extern "C" void uart_div_modify(int no, unsigned int freq);

extern HardwareSerial Serial;

#endif