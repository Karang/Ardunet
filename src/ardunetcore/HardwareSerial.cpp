#include "ardunetcore/HardwareSerial.h"

HardwareSerial::HardwareSerial() {
    
}

void HardwareSerial::begin(long speed) {
    uart_config(speed, EIGHT_BITS, STICK_PARITY_DIS, NONE_BITS, ONE_STOP_BIT, NONE_CTRL);
}

void HardwareSerial::print(const char*s) {
    while(*s) {
        tx_one_char(*s++);
    }
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
        tx_one_char((char)n);
    } else if (base==DEC) {
        if (n<0) {
            tx_one_char('-');
            n = -n;
        }
        printNumber(n, base);
    } else {
        printNumber(n, base);
    }
}

void HardwareSerial::print(unsigned long n, int base) {
    if (base==BYTE) {
        tx_one_char((char)n);
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
    tx_one_char('\r');
    tx_one_char('\n');
}

// PRIVATE //

void HardwareSerial::tx_one_char(char c) {
     while (true) {
      uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(UART0)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
      if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
        break;
      }
    }

    WRITE_PERI_REG(UART_FIFO(UART0), c);
}

void HardwareSerial::uart_config(unsigned int baut_rate, UartBitsNum4Char data_bits, UartExistParity exist_parity, UartParityMode parity, UartStopBitsNum stop_bits, UartFlowCtrl flow_ctrl) {
    /* rcv_buff size if 0x100 */
    //ETS_UART_INTR_ATTACH(uart0_rx_intr_handler,  &(UartDev.rcv_buff));
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_U0RTS);
    
    uart_div_modify(UART0, UART_CLK_FREQ / baut_rate);
    
    WRITE_PERI_REG(UART_CONF0(UART0), exist_parity
                 | parity
                 | (stop_bits << UART_STOP_BIT_NUM_S)
                 | (data_bits << UART_BIT_NUM_S));
    
    // clear rx and tx fifo,not ready
    SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);
    
    // set rx fifo trigger
    // WRITE_PERI_REG(UART_CONF1(uart_no), ((UartDev.rcv_buff.TrigLvl & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) | ((96 & UART_TXFIFO_EMPTY_THRHD) << UART_TXFIFO_EMPTY_THRHD_S) | UART_RX_FLOW_EN);
    // set rx fifo trigger
    WRITE_PERI_REG(UART_CONF1(UART0),
                   ((0x10 & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) |
                   ((0x10 & UART_RX_FLOW_THRHD) << UART_RX_FLOW_THRHD_S) |
                   UART_RX_FLOW_EN |
                   (0x02 & UART_RX_TOUT_THRHD) << UART_RX_TOUT_THRHD_S |
                   UART_RX_TOUT_EN);
    SET_PERI_REG_MASK(UART_INT_ENA(UART0), UART_RXFIFO_TOUT_INT_ENA | UART_FRM_ERR_INT_ENA);
    
    // clear all interrupt
    WRITE_PERI_REG(UART_INT_CLR(UART0), 0xffff);
    // enable rx_interrupt
    SET_PERI_REG_MASK(UART_INT_ENA(UART0), UART_RXFIFO_FULL_INT_ENA);
}

void HardwareSerial::printNumber(unsigned long n, uint8_t base) {
    unsigned char nBuffer[8*sizeof(long)];
    unsigned int i = 0;
    while (n>0) {
        nBuffer[i++] = n % base;
        n /= base;
    }
    
    if (i==0) {
        tx_one_char('0');
    }
    
    for (; i>0 ; i--) {
        tx_one_char((char)((nBuffer[i-1]<10) ? ('0'+nBuffer[i-1]) : ('A'+nBuffer[i-1]-10)));
    }
}

void HardwareSerial::printFloat(double n, uint8_t digits) {
    if (n<0) {
        tx_one_char('-');
        n = -n;
    }
    
    double rounding = 0.5;
    for (uint8_t i=0 ; i<digits ; ++i) rounding /= 10.0;
    n += rounding;
    
    unsigned long int_part = (unsigned long) n;
    double rest = n - (double)int_part;
    printNumber(int_part, DEC);
    
    if (digits>0)
        tx_one_char('.');
        
    while (digits-- > 0) {
        rest *= 10.0;
        int toPrint = int(rest);
        tx_one_char((char)('0'+toPrint));
        rest -= toPrint;
    }
}

HardwareSerial Serial = HardwareSerial();