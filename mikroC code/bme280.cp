#line 1 "C:/Code/MMT koji ne radi/bme280.c"
#line 1 "c:/code/mmt koji ne radi/bme280.h"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for arm/include/stdint.h"





typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;
typedef signed long long int64_t;


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef signed int int_least16_t;
typedef signed long int int_least32_t;
typedef signed long long int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned int uint_least16_t;
typedef unsigned long int uint_least32_t;
typedef unsigned long long uint_least64_t;



typedef signed long int int_fast8_t;
typedef signed long int int_fast16_t;
typedef signed long int int_fast32_t;
typedef signed long long int_fast64_t;


typedef unsigned long int uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long long uint_fast64_t;


typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;


typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
#line 40 "c:/code/mmt koji ne radi/bme280.h"
void BME280_Init();
double getPressure();
double getTemperature();
#line 3 "C:/Code/MMT koji ne radi/bme280.c"
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;

uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

int32_t t_fine;

sbit LD11 at ODR12_GPIOE_ODR_bit;
sbit LD22 at ODR15_GPIOE_ODR_bit;

uint8_t data_[10], conv[20];
uint8_t output[10];

void BME_Write( int len) {
 I2C3_Start();
 I2C3_Write( 0x76 , data_, len, END_MODE_STOP);
}

void BME_Read( uint32_t len, uint32_t readlen) {
 LD11=1; LD22=0;
 I2C3_Start();
 LD11=1; LD22=1;
 I2C3_Write( 0x76 , data_, len, END_MODE_RESTART);
 LD11=0; LD22=1;
 I2C3_Read( 0x76 , output, readlen, END_MODE_STOP);
}

int16_t BME_Read_DigS(uint8_t reg) {
 int16_t result;
 data_[0] = reg;
 I2C3_Start();
 I2C3_Write( 0x76 , data_, 1, END_MODE_RESTART);
 I2C3_Read( 0x76 , output, 2, END_MODE_STOP);
 result = output[1];
 result <<= 8;
 result |= output[0];
 return result;
}

uint16_t BME_Read_DigU(uint8_t reg) {
 uint16_t result;
 data_[0] = reg;
 I2C3_Start();
 I2C3_Write( 0x76 , data_, 1, END_MODE_RESTART);
 I2C3_Read( 0x76 , output, 2, END_MODE_STOP);
 result = output[1];
 result <<= 8;
 result |= output[0];
 return result;
}



void BME280_Init() {

 I2C3_Init_Advanced(100000, &_GPIO_MODULE_I2C3_PA8_PC9);
 NVIC_SetIntPriority(IVT_INT_I2C3_EV, _NVIC_INT_PRIORITY_LVL0);
 NVIC_SetIntPriority(IVT_INT_I2C3_ER, _NVIC_INT_PRIORITY_LVL0);



 dig_T1 = BME_Read_DigU( 0x88 );
 dig_T2 = BME_Read_DigS( 0x8A );
 dig_T3 = BME_Read_DigS( 0x8C );

 dig_P1 = BME_Read_DigU( 0x8E );
 dig_P2 = BME_Read_DigS( 0x90 );
 dig_P3 = BME_Read_DigS( 0x92 );
 dig_P4 = BME_Read_DigS( 0x94 );
 dig_P5 = BME_Read_DigS( 0x96 );
 dig_P6 = BME_Read_DigS( 0x98 );
 dig_P7 = BME_Read_DigS( 0x9A );
 dig_P8 = BME_Read_DigS( 0x9C );
 dig_P9 = BME_Read_DigS( 0x9E );

 data_[0] =  0xF4 ;
 data_[1] =  0xB7 ;
 BME_Write(2);
}

int32_t BME280_compensate_T_int32(int32_t adc_T) {
 int32_t var1, var2, T;
 long out;
 adc_T >>= 4;
 var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
 var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) *
 ((int32_t)dig_T3)) >> 14;
 t_fine = var1 + var2;
 T = (t_fine * 5 + 128) >> 8;
 return T;
}

double getTemperature() {

 int32_t result;

 data_[0] =  0xFA ;

 BME_Read( 1,3);

 result = output[0];
 result <<= 8;
 result |= output[1];
 result <<= 8;
 result |= output[2];
 return BME280_compensate_T_int32(result)/100.0;
}

void BME280_Forced_Mode() {

 uint8_t dataReady = 1;
 data_[0] =  0xF4 ;
 data_[1] =  0xB5 ;

 BME_Write(2);


 while(dataReady != 0) {
 data_[0] =  0xF3 ;
 BME_Read( 1, 1);
 dataReady = output[0] &  0x08 ;
 }


 while (dataReady == 0) {
 data_[0] =  0xF3 ;
 BME_Read(1,1);
 dataReady = output[0] &  0x08 ;
 }
}

uint32_t BME280_compensate_P_int32(int32_t adc_P)
{
 int32_t var1, var2;
 uint32_t p;
 adc_P >>= 4;
 var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
 var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
 var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
 var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
 var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) * var1)>>1))>>18;
 var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
 if (var1 == 0) {
 return 0;
 }
 p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
 if (p < 0x80000000) {
 p = (p << 1) / ((uint32_t)var1);
 }
 else {
 p = (p / (uint32_t)var1) * 2;
 }
 var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
 var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
 p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
 return p;
}


double getPressure() {
 int32_t result;
 double ret;


 getTemperature();

 data_[0] =  0xF7 ;
 BME_Read( 1, 3);
 result = output[0];
 result <<= 8;
 result |= output[1];
 result <<= 8;
 result |= output[2];
 return BME280_compensate_P_int32(result)/100.0;
}
