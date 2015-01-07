#ifndef __ARDUNET_SPI_FLASH_H__
#define __ARDUNET_SPI_FLASH_H__

extern "C" {
    #include "esp_common.h"
}

class SpiFlash {
    private:
    public:
        SpiFlash();
        void eraseSector(uint16 sector);
        void writeData(uint32 des_addr, uint32 *src_addr, uint32 size);
        void readData(uint32 src_addr, uint32 *des_addr, uint32 size);
};

extern SpiFlash Flash;

#endif