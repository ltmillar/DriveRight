#ifndef __define
#define __define

/****************************************/
/*
 * SD card pin distribution
 */
/****************************************/

/* Use SPI communication with SDCard */
#define    FATFS_USE_SDIO                0

/* Select your SPI settings */
#define FATFS_SPI                SPI1
#define FATFS_SPI_PINSPACK        TM_SPI_PinsPack_1//PA7    PA6     PA5


/* Custom CS pin for SPI communication */
#define FATFS_CS_PORT        GPIOB
#define FATFS_CS_PIN        GPIO_Pin_0

#endif
