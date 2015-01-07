#include "ardunetcore/SpiFlash.h"

SpiFlash::SpiFlash() {
    
}

void SpiFlash::eraseSector(uint16 sector) {
    spi_flash_erase_sector(sector);
}

void SpiFlash::writeData(uint32 des_addr, uint32 *src_addr, uint32 size) {
    spi_flash_write(des_addr, src_addr, size);
}

void SpiFlash::readData(uint32 src_addr, uint32 *des_addr, uint32 size) {
    spi_flash_read(src_addr, des_addr, size);
}

SpiFlash EEPROM = SpiFlash();