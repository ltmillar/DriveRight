#ifndef __GPS_PARSING__
#define __GPS_PARSING__

#define GPS_BAUDRATE 9600


#define GPS_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define GPS_UART_CLK			RCC_APB1Periph_UART4
#define GPS_GPIO_TX				GPIO_Pin_10
#define GPS_GPIO_RX				GPIO_Pin_11
#define GPS_GPIO_PORT			GPIOC
#define GPS_GPIO_AF				GPIO_AF_UART4
#define GPS_AF_PinSource_TX		GPIO_PinSource10
#define GPS_AF_PinSource_RX		GPIO_PinSource11
#define GPS_UART_PORT			UART4



struct GPS_info
{
	  	char hour, minute, seconds, year, month, day;
		int milliseconds;
		float latitude, longitude;
		float speed, angle, magvariation, HDOP;
		char lat, lon, mag;
		char GPS_status;
	    int latitude_fixed, longitude_fixed;
		float latitudeDegrees, longitudeDegrees;
};

//initialize GPS UART port
int GPS_init();

int GPS_get_rawdata(char* nmea, int length);

int GPS_parsing(struct GPS_info* GPS, char* nmea);

#endif
