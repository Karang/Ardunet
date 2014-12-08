extern "C" {
    #include "driver/uart.h"
}

#include "ardunetcore/HardwareSerial.h"

HardwareSerial::HardwareSerial() {
    
}

void HardwareSerial::begin(int speed) {
    switch (speed) {
        case 9600:
            uart_init(BIT_RATE_9600, BIT_RATE_9600);
            break;
        case 19200:
            uart_init(BIT_RATE_19200, BIT_RATE_19200);
            break;
        case 38400:
            uart_init(BIT_RATE_38400, BIT_RATE_38400);
            break;
        case 57600:
            uart_init(BIT_RATE_57600, BIT_RATE_57600);
            break;
        case 74880:
            uart_init(BIT_RATE_74880, BIT_RATE_74880);
            break;
        case 115200:
            uart_init(BIT_RATE_115200, BIT_RATE_115200);
            break;
        case 230400:
            uart_init(BIT_RATE_230400, BIT_RATE_230400);
            break;
        case 256000:
            uart_init(BIT_RATE_256000, BIT_RATE_256000);
            break;
        case 460800:
            uart_init(BIT_RATE_460800, BIT_RATE_460800);
            break;
        case 921600:
            uart_init(BIT_RATE_921600, BIT_RATE_921600);
            break;
        default:
            uart_init(BIT_RATE_115200, BIT_RATE_115200);
            break;
    }
}

void HardwareSerial::print(const char*s) {
    uart0_sendStr(s);
}

void HardwareSerial::print(char c, int base) {
    print((long) c, base);
}

void HardwareSerial::print(unsigned char c, int base) {
    print((unsigned long) c, base);
}

void HardwareSerial::print(int i, int base) {
    print((long) i, base);
}

void HardwareSerial::print(unsigned int i, int base) {
    print((unsigned long) i, base);
}

void HardwareSerial::print(long n, int base) {
    if (base==BYTE) {
        uart0_sendChar((char)n);
    } else if (base==DEC) {
        if (n<0) {
            uart0_sendChar('-');
            n = -n;
        }
        printNumber(n, base);
    } else {
        printNumber(n, base);
    }
}

void HardwareSerial::print(unsigned long n, int base) {
    if (base==BYTE) {
        uart0_sendChar((char)n);
    } else {
        printNumber(n, base);
    }
}

void HardwareSerial::print(double n, int digits) {
    printFloat(n, digits);
}

void HardwareSerial::println(const char*s) {
    print(s);
    println();
}

void HardwareSerial::println(char c, int base) {
    print(c, base);
    println();
}

void HardwareSerial::println(unsigned char c, int base) {
    print(c, base);
    println();
}

void HardwareSerial::println(int n, int base) {
    print(n, base);
    println();
}

void HardwareSerial::println(unsigned int n, int base) {
    print(n, base);
    println();
}

void HardwareSerial::println(long n, int base) {
    print(n, base);
    println();
}

void HardwareSerial::println(unsigned long n, int base) {
    print(n, base);
    println();
}

void HardwareSerial::println(double n, int digits) {
    print(n, digits);
    println();
}

void HardwareSerial::println(void) {
    uart0_sendChar('\r');
    uart0_sendChar('\n');
}

void HardwareSerial::printNumber(unsigned long n, uint8_t base) {
    unsigned char nBuffer[8*sizeof(long)];
    unsigned int i = 0;
    while (n>0) {
        nBuffer[i++] = n % base;
        n /= base;
    }
    
    if (i==0) {
        uart0_sendChar('0');
    }
    
    for (; i>0 ; i--) {
        uart0_sendChar((char)((nBuffer[i-1]<10) ? ('0'+nBuffer[i-1]) : ('A'+nBuffer[i-1]-10)));
    }
}

void HardwareSerial::printFloat(double n, uint8_t digits) {
    if (n<0) {
        uart0_sendChar('-');
        n = -n;
    }
    
    double rounding = 0.5;
    for (uint8_t i=0 ; i<digits ; ++i) rounding /= 10.0;
    n += rounding;
    
    unsigned long int_part = (unsigned long) n;
    double rest = n - (double)int_part;
    printNumber(int_part, DEC);
    
    if (digits>0)
        uart0_sendChar('.');
        
    while (digits-- > 0) {
        rest *= 10.0;
        int toPrint = int(rest);
        uart0_sendChar((char)('0'+toPrint));
        rest -= toPrint;
    }
}

HardwareSerial Serial = HardwareSerial();