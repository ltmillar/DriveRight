void ADCConvert_Potentiometer(void);
int isPassenger(void);
int getVoltage(void);

extern ADC3ConvertedValue;
extern ADC3ConvertedVoltage;

int main(void)
{

	char passenger = 0;

	ADCConvert_Potentiometer();

	while(1)
    {
	    passenger = isPassenger();
    }
}

