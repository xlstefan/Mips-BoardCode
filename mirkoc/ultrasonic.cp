#line 1 "C:/Code/Tamara latest/ultrasonic.c"
#line 1 "c:/code/tamara latest/ultrasonic.h"



void initUltrasonic();
double getDistance();
#line 1 "c:/code/tamara latest/timer.h"
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
#line 7 "c:/code/tamara latest/timer.h"
void my_Delay_us(uint32_t num);
void InitTimerUs();

void my_Delay_ms(uint32_t num);
void InitTimerMs();

void RTCInit(void);
#line 4 "C:/Code/Tamara latest/ultrasonic.c"
sbit TRIG at ODR14_GPIOD_ODR_bit;
sbit ECHO at IDR15_GPIOD_IDR_bit;

void initUltrasonic() {
 GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_14);
 GPIO_Digital_Input(&GPIOD_BASE, _GPIO_PINMASK_15);
}
double getDistance() {
 long cnt, echoValue;
 double distance;


 TRIG = 0;
 TRIG = 1;
 my_Delay_us(10);
 TRIG = 0;


 cnt = 0;
 while (ECHO == 0);
 while (ECHO == 1) {
 cnt++;
 my_Delay_us(1);
 }


 distance = cnt / 58.0;
 return distance;
}