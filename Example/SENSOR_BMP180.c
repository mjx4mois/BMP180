/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: pressure & temperature sensor
     File Name		: SENSOR_BMP180.c
     Function		: SENSOR_BMP180
     Create Date	: 2017/03/21
---------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_BMP180.h>

#define SENSOR_BMP180_DEBUG    			(0)	/* open printf result value */

BMP180_CAL_COFF BMP180_CAL_COFF_DATA;

/********************************************** SYSTEM *************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* Get the Calibration coefficients */
void BMP180_GET_CAL_COEFF(void)
{
       CHAR8U BMP_CAL_COFF_ARRAY[CAL_COEFF_NUMBER]={0};
       INT16U count;
       CHAR8S fail_flag=0;

	        for(count=0;count<CAL_COEFF_NUMBER;count++)
	        {

			fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,(0xAA+count),&BMP_CAL_COFF_ARRAY[count]);
			if(fail_flag<=0)
			{
				printf("read error in CAL_COEFF_NUMBER[%d] \r\n",count);
	                     break;
			}
	        }

	       /*calculating Calibration Coefficients*/
	        BMP180_CAL_COFF_DATA.AC1 = (INT16S) ( (BMP_CAL_COFF_ARRAY[0]<<8) + BMP_CAL_COFF_ARRAY[1]);
	        BMP180_CAL_COFF_DATA.AC2 = (INT16S) ( (BMP_CAL_COFF_ARRAY[2]<<8) + BMP_CAL_COFF_ARRAY[3]);
	        BMP180_CAL_COFF_DATA.AC3 = (INT16S) ( (BMP_CAL_COFF_ARRAY[4]<<8) + BMP_CAL_COFF_ARRAY[5]);
	        BMP180_CAL_COFF_DATA.AC4 = (INT16S) ( (BMP_CAL_COFF_ARRAY[6]<<8) + BMP_CAL_COFF_ARRAY[7]);
	        BMP180_CAL_COFF_DATA.AC5 = (INT16S) ( (BMP_CAL_COFF_ARRAY[8]<<8) + BMP_CAL_COFF_ARRAY[9]);
	        BMP180_CAL_COFF_DATA.AC6 = (INT16S) ( (BMP_CAL_COFF_ARRAY[10]<<8) + BMP_CAL_COFF_ARRAY[11]);
	        BMP180_CAL_COFF_DATA.B1 =  (INT16S) ( (BMP_CAL_COFF_ARRAY[12]<<8) + BMP_CAL_COFF_ARRAY[13]);
	        BMP180_CAL_COFF_DATA.B2 =  (INT16S) ( (BMP_CAL_COFF_ARRAY[14]<<8) + BMP_CAL_COFF_ARRAY[15]);
	        BMP180_CAL_COFF_DATA.MB =  (INT16S) ( (BMP_CAL_COFF_ARRAY[16]<<8) + BMP_CAL_COFF_ARRAY[17]);
	        BMP180_CAL_COFF_DATA.MC =  (INT16S) ( (BMP_CAL_COFF_ARRAY[18]<<8) + BMP_CAL_COFF_ARRAY[19]);
	        BMP180_CAL_COFF_DATA.MD =  (INT16S) ( (BMP_CAL_COFF_ARRAY[20]<<8) + BMP_CAL_COFF_ARRAY[21]);

#if SENSOR_BMP180_DEBUG

		for(count=0;count<CAL_COEFF_NUMBER;count++)
		{
			printf("a[0x%X]=0x%X  ",(0xAA+count),BMP_CAL_COFF_ARRAY[count]);
			if(count %3==0 && count!=0 )printf("\r\n");
		}
		
		printf("\r\n");

         	printf("AC1 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC1,BMP180_CAL_COFF_DATA.AC1);
         	printf("AC2 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC2,BMP180_CAL_COFF_DATA.AC2);
         	printf("AC3 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC3,BMP180_CAL_COFF_DATA.AC3);
         	printf("AC4 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC4,BMP180_CAL_COFF_DATA.AC4);
         	printf("AC5 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC5,BMP180_CAL_COFF_DATA.AC5);
         	printf("AC6 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.AC6,BMP180_CAL_COFF_DATA.AC6);
         	printf("B1 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.B1,BMP180_CAL_COFF_DATA.B1);
        	printf("B2 = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.B2,BMP180_CAL_COFF_DATA.B2);
         	printf("MB = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.MB,BMP180_CAL_COFF_DATA.MB);
         	printf("MC = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.MC,BMP180_CAL_COFF_DATA.MC);
         	printf("MD = 0x%X(%d)\r\n", BMP180_CAL_COFF_DATA.MD,BMP180_CAL_COFF_DATA.MD);
#endif

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Get the temperature & pressure raw data */
CHAR8S BMP180_GET_TEMP_PRESS(INT32S *temperature_data,FLOAT *pressure_data,CHAR8U oss)
{
        CHAR8S fail_flag=0;
		
        INT32S X1,X2,X3,B5,REAL_TEMPUTRE;
        INT32S B3,B6;
        INT32U B4,B7;

        INT32S Temp_value;
        INT32S Press_value;

        CHAR8U high_byte_temp,low_byte_temp;
        CHAR8U high_byte_press,low_byte_press,x_low_byte_press;

        INT32S pr1,pr2,pr3;
        INT32S pr4,pr5,final_p;
        FLOAT  p_float;

      
		/* temperature measurement */
		fail_flag = i2c_write_1_byte_data(BMP180_SLAVE_ADDRESS,0XF4,0X2E);
		if(fail_flag == SWI2C_STATUS_FAIL) 
		{
			return -1;
		}
		
        	delay_ms(6);	/* wait for BMP180 conversion is finished  see spec page.17 */ 

		/* read tempreature data */
		fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,0XF6,&high_byte_temp);
		if(fail_flag == SWI2C_STATUS_FAIL)
		{
			return -1;
		}

		fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,0XF7,&low_byte_temp);
		if(fail_flag == SWI2C_STATUS_FAIL)
		{
			return -1;
		}

		delay_us(30);/* delay small time */
  
     
       	/* pressure measurement */
		fail_flag = i2c_write_1_byte_data(BMP180_SLAVE_ADDRESS,0XF4, 0X34 + (CHAR8U)(oss<< 6) );  /* oss -> set the BMP085 measurement pressure mode */
		if(fail_flag == SWI2C_STATUS_FAIL) 
		{
			return -1;
		}
		
       	switch(oss)	/* wait for BMP180 conversion is finished  see spec page.17 */ 
        	{
         		case 0:/* datasheet max time :4.5ms */
					delay_ms(6);
					break;
         		case 1:/*datasheet max time :7.5ms */
					delay_ms(9);
					break;
         		case 2:/*datasheet max time :13.5ms */
					delay_ms(15);
					break;
         		case 3:/*datasheet max time :25.5ms */
					delay_ms(27);
					break;
         		default:
					return -1;  /*wrong value*/
        	}

		/* read pressure data */
		fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,0XF6,&high_byte_press);
		if(fail_flag == SWI2C_STATUS_FAIL)  
		{
			return -1;
		}

		fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,0XF7,&low_byte_press);
		if(fail_flag == SWI2C_STATUS_FAIL)  
		{
			return -1;
		}

		fail_flag = i2c_read_1_byte_data(BMP180_SLAVE_ADDRESS,0XF8,&x_low_byte_press);
		if(fail_flag == SWI2C_STATUS_FAIL)
		{
			return -1;
		}

		/* Calculation Tempearture */
		Temp_value = (INT32S) ((high_byte_temp << 8) + low_byte_temp);

		X1 = ((INT32S)Temp_value - BMP180_CAL_COFF_DATA.AC6)*(BMP180_CAL_COFF_DATA.AC5)/32768;
		X2 =  (INT32S)((INT32S)BMP180_CAL_COFF_DATA.MC *2048)/(INT32S)(X1 + (INT32S)BMP180_CAL_COFF_DATA.MD);
		B5 = (INT32S)(X1 + X2);
		REAL_TEMPUTRE = (INT32S)(B5 + 8)/ 16;        /* Final Temperature , ex: 153 temp  setp in 0.1 , 15.3 oC */
 
		/* Calculation Pressuree */
		pr1 = (INT32S)high_byte_press << 16;
		pr2 = (INT32S)low_byte_press << 8;
		pr3 = (INT32S)x_low_byte_press;
		pr4 = (INT32S)(8-oss);
		Press_value = (INT32S)((pr1+pr2+pr3) >> pr4);


		B6 = (INT32S)(B5 - 4000);
		pr1 = (INT32S)(B6 * B6)/4096;

		X1 = ((BMP180_CAL_COFF_DATA.B2)*pr1)/2048 ;
		X2 = (INT32S) ((INT32S)BMP180_CAL_COFF_DATA.AC2) * B6 / 2048;
		X3 = X1 + X2;
		B3 =(INT32S)(((((INT32S)BMP180_CAL_COFF_DATA.AC1*4+X3)<< (INT32S)(oss) ) + 2)/4 );
		X1 =(INT32S)(((INT32S)BMP180_CAL_COFF_DATA.AC3 * B6)/8192);
		X2 =(INT32S)(((INT32S)BMP180_CAL_COFF_DATA.B1 * ((B6*B6)/4096))/65536);
		X3 =(INT32S)(((X1 + X2) + 2)/4);
		pr4 =(INT32S)(X3 + 32768);
		pr5 =(INT32S)BMP180_CAL_COFF_DATA.AC4;
		B4 = (INT32U) ((pr4 * pr5)/32768);
		pr1 = (INT32S)(50000 >> (INT32S)(oss));
		B7 = (INT32S)((INT32U)Press_value - B3) * pr1;
		

		if( B7 < (0x80000000) )
		{
			p_float = ((FLOAT)B7*2/(FLOAT)B4);
		}
		else
		{
			p_float = (((FLOAT)B7*(FLOAT)B4)/2);
		}


		X1 =(INT32S) ((p_float/256) *(p_float/256));
		X1 = ((X1 *3038)/65536);
        	X2 = (INT32S)((-7357 * p_float)/65536);
        	p_float = (p_float + ((FLOAT)X1 +(FLOAT)X2 + 3791)/16); /* Final pressure ,  press. in Pa */


#if SENSOR_BMP180_DEBUG
		printf("Temp_value = %ld\r\n",Temp_value);
		printf("X1 = %ld\r\n",(INT32S)X1);
		printf("X2 = %ld\r\n",(INT32S)X2);
        	printf("B5 = %lu\r\n",(INT32S)B5);
        	printf("REAL_TEMPUTRE = %ld.%ld\r\n",(INT32S)REAL_TEMPUTRE/10,(INT32S)REAL_TEMPUTRE%10);
#endif


		/* Final parameter Set */
		*pressure_data = (p_float);							/* Final Pressure , ex: 100053 Pa = 1000.53 hpa*/
		*temperature_data = (INT32S)REAL_TEMPUTRE;		/* Final Temperature*/

		return 0;

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Calculate the sea level pressure */
void BMP180_GET_ALTITUDE(FLOAT pressure_data,FLOAT *altitude_result)
{
	FLOAT alt_temp1,alt_temp2,alt_temp3,alt_temp4,alt_temp5;
	
	        /* Calculating absolute altitude  */
	        alt_temp1 = ((FLOAT)pressure_data/101325);
	        alt_temp2 = (1/5.255);
	        alt_temp3 = pow(alt_temp1,alt_temp2);
	        alt_temp4 = (1 - alt_temp3);
	        alt_temp5 = 44330.0 *(alt_temp4);	/*altitude result*/
	      
	 	*altitude_result = alt_temp5;		/* Final altitude*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Calculate the altitude */      
void BMP180_GET_SEALEVEL_PRESSURE(FLOAT altitude,FLOAT pressure_data,FLOAT *sea_level_pressure)
{
	FLOAT alt_temp6,alt_temp7,alt_temp8,alt_temp9,alt_temp10;
	
	        /* Calculating pressure at sea level  */
	        alt_temp6 = ((FLOAT)altitude/44330.0);
	        alt_temp7 = (1 - alt_temp6);
	        alt_temp8 = pow(alt_temp7,5.255);
	        alt_temp9 = (pressure_data /alt_temp8);
	        alt_temp10 = alt_temp9;      
	        
		*sea_level_pressure = alt_temp10;		/* Final sea level*/
}
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM *************************************************/