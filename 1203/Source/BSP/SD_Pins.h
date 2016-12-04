#ifndef __SDPIN__
#define __SDPIN__
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define SPIx_SPI_CLK		RCC_APB2Periph_SPI1
#define	SPIx_GPIO_CLK		RCC_AHB1Periph_GPIOA

#define SPIx_SPI_SCLK_GPIO_PORT			GPIOA
#define SPIx_SPI_MISO_GPIO_PORT			GPIOA
#define SPIx_SPI_MOSI_GPIO_PORT			GPIOA

#define SPIx_SPI_SCLK_SOURCE			GPIO_PinSource5
#define SPIx_SPI_MISO_SOURCE			GPIO_PinSource6
#define SPIx_SPI_MOSI_SOURCE			GPIO_PinSource7

#define SPIx_SPI_SCLK_AF				GPIO_AF_SPI1
#define SPIx_SPI_MISO_AF				GPIO_AF_SPI1
#define SPIx_SPI_MOSI_AF				GPIO_AF_SPI1

#define SPIx_SPI_SCLK_PIN				GPIO_Pin_5
#define SPIx_SPI_MISO_PIN				GPIO_Pin_6
#define SPIx_SPI_MOSI_PIN				GPIO_Pin_7

#define SPIx_SPI				SPI1

#define SD_CS_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define SD_CS_GPIO_PORT 		GPIOA
#define SD_CS_PIN				GPIO_Pin_4






#endif
