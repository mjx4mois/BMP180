/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: BMP085  pressure & temperature sensor
     File Name		: EXAMPLE_BMP180.c
     Function		: EXAMPLE_BMP180
     Create Date	: 2017/04/29
---------------------------------------------------------------------- */
#ifndef __BMP180_EXAMPLE__
#define __BMP180_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <i2c.h>
#include <alcd.h>
#include "SENSOR_BMP180.h"
#include "Porting_Layer.h"

void EXAMPLE_BMP180(void);


void EXAMPLE_BMP180(void)
{

        INT32S final_temperature; 
        FLOAT final_pressure,final_sealevel_press,final_altitude;
        CHAR8S check_flag_BMP180 = 0;
	 CHAR8U lcd_temp[3]={0},lcd_data[6]={0};

	 
		i2c_stop_hang();

		printf("--------------------BMP180--------------------\r\n");
			
		/* Read Calibration Coefficients & Calculating
			I2C register address 0xAA~0xBF */
		BMP180_GET_CAL_COEFF();

		/* Get the Temperature & Pressure */
		check_flag_BMP180 = BMP180_GET_TEMP_PRESS(&final_temperature,&final_pressure,OVERSAMPLING);
		
		/* Calculate the altitude */		
		BMP180_GET_ALTITUDE(final_pressure,&final_altitude);

		/* Calculate the sea level pressure */
		BMP180_GET_SEALEVEL_PRESSURE(final_altitude,final_pressure,&final_sealevel_press);


		/* print result */
		if(check_flag_BMP180==0)
		{
                    printf("Temperature : %d.%d C\r\n",final_temperature/10,final_temperature%10);
                    printf("Pressure    : %ld Pa (%ld.%ld hPa)\r\n",(INT32S)final_pressure,(INT32S)final_pressure/100,(INT32S)final_pressure%100);
                    printf("Altitude    : %f m\r\n",final_altitude);
                    printf("Sea leve press   : %f Pa\r\n",final_sealevel_press);
                    printf("--------------------BMP180--------------------\r\n\r\n");


            		 /* BMP180 temperature */                    
	            	lcd_gotoxy(0,0);
	            	lcd_putsf("Temp:");     

	           	if(final_temperature<0)
	            	{
	            		lcd_putchar(45);
			}                             
	            	else
			{
				lcd_putchar(32);
			}
					
			/* final_temperature unit : 0.1 C -> if  final_temperature == 245 , 24.5 C */
 			lcd_temp[0]=final_temperature/100;
			lcd_temp[1]=(final_temperature/10)%10;
			lcd_temp[2]=final_temperature%10;
			/* show final_temperature data on LCD */
			lcd_putchar(48+lcd_temp[0]);
		       lcd_putchar(48+lcd_temp[1]);
			lcd_putchar(46);	/* show "."*/	
		       lcd_putchar(48+lcd_temp[2]);
		       lcd_putchar(67);	/* show "C"*/	


            		 /* BMP085 Pressure */                    
	            	lcd_gotoxy(0,1);
	            	lcd_putsf("P:");    
			/* final_pressure unit : Pa -> if  final_pressure == 99687 Pa , 996.87hPa */
			lcd_data[0]=((INT32S)final_pressure/100000);
			lcd_data[1]=((INT32S)final_pressure/10000)%10;
			lcd_data[2]=((INT32S)final_pressure/1000)%10;
			lcd_data[3]=((INT32S)final_pressure/100)%10;	
			lcd_data[4]=((INT32S)final_pressure/10)%10;
			lcd_data[5]=((INT32S)final_pressure)%10;
			/* show final_pressure data on LCD */
			lcd_putchar(48+lcd_data[0]);
			lcd_putchar(48+lcd_data[1]);
			lcd_putchar(48+lcd_data[2]);
		       lcd_putchar(48+lcd_data[3]);
			lcd_putchar(46);	/* show "."*/	
		       lcd_putchar(48+lcd_data[4]);	
			lcd_putchar(48+lcd_data[5]);	   
		       lcd_putsf("hPa");/* show "hPa"*/	


            		 /* BMP085 Altitude */                    
	            	lcd_gotoxy(0,2);
	            	lcd_putsf("Alt:");    
			/* final_altitude unit : m -> if  final_altitude == 612 Pa , 612m */
			lcd_data[0]=((INT32S)final_altitude/1000)%10;
			lcd_data[1]=((INT32S)final_altitude/100)%10;	
			lcd_data[2]=((INT32S)final_altitude/10)%10;
			lcd_data[3]=((INT32S)final_altitude)%10;
			lcd_data[4]=(INT32S)(final_altitude*10)%10;
			lcd_data[5]=(INT32S)(final_altitude*100)%10;
			/* show final_altitude data on LCD */
			lcd_putchar(48+lcd_data[0]);
			lcd_putchar(48+lcd_data[1]);
			lcd_putchar(48+lcd_data[2]);
		       lcd_putchar(48+lcd_data[3]);
			lcd_putchar(46);	/* show "."*/	
		       lcd_putchar(48+lcd_data[4]);	
			lcd_putchar(48+lcd_data[5]);   
		       lcd_putsf("m");/* show "m"*/	


            		 /* BMP085 sea leve pressure*/                    
	            	lcd_gotoxy(0,3);
	            	lcd_putsf("P0:");    /* P0 -> Sea leve pressure */
			/* final_sealevel_press unit : Pa -> if  final_sealevel_press == 99687 Pa , 996.87hPa */
			lcd_data[0]=((INT32S)final_sealevel_press/100000);
			lcd_data[1]=((INT32S)final_sealevel_press/10000)%10;
			lcd_data[2]=((INT32S)final_sealevel_press/1000)%10;
			lcd_data[3]=((INT32S)final_sealevel_press/100)%10;	
			lcd_data[4]=((INT32S)final_sealevel_press/10)%10;
			lcd_data[5]=((INT32S)final_sealevel_press)%10;
			/* show final_sealevel_press data on LCD */
			lcd_putchar(48+lcd_data[0]);
			lcd_putchar(48+lcd_data[1]);
			lcd_putchar(48+lcd_data[2]);
		       lcd_putchar(48+lcd_data[3]);
			lcd_putchar(46);	/* show "."*/	
		       lcd_putchar(48+lcd_data[4]);	
			lcd_putchar(48+lcd_data[5]);	   
		       lcd_putsf("hPa");/* show "hPa"*/		
			
		}
		else
		{
                    printf("Read BMP180 error\r\n");
                    printf("--------------------BMP180--------------------\r\n\r\n");
		}
		
}
#endif		//#ifndef __BMP180_EXAMPLE__
