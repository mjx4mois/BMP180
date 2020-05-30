/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: BMP085  pressure & temperature sensor
     File Name		: SENSOR_BMP180.h
     Function		: SENSOR_BMP180
     Create Date	: 2017/03/21
     
     HAL Layer : SWI2C function
---------------------------------------------------------------------- */
#ifndef __BMP180_HEADER__ 
#define __BMP180_HEADER__  

//--------------------------------- Define SLAVE ADDRESS -------------------------------------
#define BMP180_SLAVE_ADDRESS     	0XEE
//--------------------------------- Define SLAVE ADDRESS -------------------------------------

#define CAL_COEFF_NUMBER         		22
#define sensor_bmp180_debug      		0	/* open printf result value */
#define OVERSAMPLING					3	/* set the mode see spec page.10 , range 0~3 */
//********************************************* SYSTEM ************************************************
//--------------------------------------------------------------------------------------------------
/* Get the Calibration coefficients */
void BMP180_GET_CAL_COEFF(void);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Get the temperature & pressure raw data */
CHAR8S BMP180_GET_TEMP_PRESS(INT32S *temperature_data,FLOAT *pressure_data,CHAR8U oss);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Calculate the sea level pressure */
void BMP180_GET_SEALEVEL_PRESSURE(FLOAT altitude,FLOAT pressure_data,FLOAT *sea_level_pressure);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Calculate the altitude */
void BMP180_GET_ALTITUDE(FLOAT pressure_data,FLOAT *altitude_result);
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM ************************************************

#endif		 //#ifndef __BMP180_HEADER__  
