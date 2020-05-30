/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: BMP085  pressure & temperature sensor
     File Name		: SENSOR_BMP180.h
     Function		: SENSOR_BMP180
     Create Date	: 2017/03/21
---------------------------------------------------------------------- */
#ifndef __BMP180_HEADER__ 
#define __BMP180_HEADER__  

/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
#define BMP180_SLAVE_ADDRESS     		(0xEE)
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/

/*--------------------------------- Define BMP180 Parameter -------------------------------------*/
/* define BMP180 parameter */
typedef struct BMP180_CAL_COFF_S
{
        INT16S AC1;
        INT16S AC2;
        INT16S AC3;
        INT16U AC4;
        INT16U AC5;
        INT16U AC6;
        INT16S B1;
        INT16S B2;
        INT16S MB;
        INT16S MC;
        INT16S MD;
}BMP180_CAL_COFF;

#define CAL_COEFF_NUMBER         			(22)
#define OVERSAMPLING						(3)	/* set the mode see spec page.10 , range 0~3 */
/*--------------------------------- Define BMP180 Parameter -------------------------------------*/


/********************************************** SYSTEM *************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* Get the Calibration coefficients */
void BMP180_GET_CAL_COEFF(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Get the temperature & pressure raw data */
CHAR8S BMP180_GET_TEMP_PRESS(INT32S *temperature_data,FLOAT *pressure_data,CHAR8U oss);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Calculate the sea level pressure */
void BMP180_GET_SEALEVEL_PRESSURE(FLOAT altitude,FLOAT pressure_data,FLOAT *sea_level_pressure);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Calculate the altitude */
void BMP180_GET_ALTITUDE(FLOAT pressure_data,FLOAT *altitude_result);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM *************************************************/

#endif		 /*#ifndef __BMP180_HEADER__*/  
