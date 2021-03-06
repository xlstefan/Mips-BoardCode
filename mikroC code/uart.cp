#line 1 "C:/Code/MMT koji ne radi/uart.c"
#line 1 "c:/code/mmt koji ne radi/uart.h"
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
#line 1 "c:/code/mmt koji ne radi/timer.h"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for arm/include/stdint.h"
#line 9 "c:/code/mmt koji ne radi/timer.h"
void my_Delay_us(uint32_t num);
void InitTimerUs();

void my_Delay_ms(uint32_t num);
void InitTimerMs();

void RTCInit(void);
#line 37 "c:/code/mmt koji ne radi/uart.h"
typedef struct TransmitStructUART {
 uint8_t flag;
 uint16_t byteCount;
 uint16_t bufferPointer;
 uint8_t buffer[1000];
} TransmitStructUART;

typedef struct ReceiveStructUART {
 uint8_t flag;
 uint16_t msgCount;
 uint16_t bufferPointerWrite;
 uint16_t bufferPointerRead;
 uint8_t buffer[1000];
} ReceiveStructUART;

void USART2_Send_Text(uint8_t* input);
void USART2_Init();
uint8_t sendData(float temp, float hum, float pres, float dist);
void sendSMS(char* number, int val);
void checkSMS();
#line 1 "c:/code/mmt koji ne radi/lcd.h"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for arm/include/stdint.h"
#line 1 "c:/code/mmt koji ne radi/timer.h"
#line 9 "c:/code/mmt koji ne radi/lcd.h"
void showTempLCD(float temp, float hum);
void initLCD();

void showText(uint8_t* text);
#line 1 "c:/code/mmt koji ne radi/temperature.h"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for arm/include/stdint.h"
#line 1 "c:/code/mmt koji ne radi/timer.h"
#line 31 "c:/code/mmt koji ne radi/temperature.h"
uint8_t oneWireReset();
void oneWireWrite(uint8_t byte);
unsigned short oneWireRead();

float calcTemp();
void setPrecision(uint8_t precision_);
#line 5 "C:/Code/MMT koji ne radi/uart.c"
uint8_t receivedFlag = 0;
uint8_t receivedTxt[300];

volatile TransmitStructUART transmitUART;
volatile ReceiveStructUART receiveUART;

void USART2_Init()
{

 RCC_AHB1ENR |=  0x00000008UL ;
 RCC_APB1ENR |=  0x00020000UL ;
 my_Delay_ms( 10 );


 GPIOD_MODER |=  0x00002000UL  |  0x00000800UL ;
 GPIOD_OSPEEDR |=  0x00003000UL  |  0x00000C00UL ;
 GPIOD_AFRL |=  0x07000000UL  |  0x00700000UL ;
 NVIC_IPR9 |=  0x00000000UL ;
 NVIC_ISER1 |=  0x00000040UL ;
 USART2_BRR |=  0x00000C35UL ;
 USART2_CR1 |=  0x00002000UL  |  0x00000020UL  |  0x00000008UL  |  0x00000004UL ;
 my_Delay_ms( 10 );
 NVIC_SetIntPriority(IVT_INT_USART2, _NVIC_INT_PRIORITY_LVL0);


 receiveUART.flag = 0;
 receiveUART.bufferPointerWrite = 0;
 receiveUART.bufferPointerRead = 0;
 receiveUART.msgCount=0;
 transmitUART.flag = 0;
 transmitUART.byteCount = 0;
 transmitUART.bufferPointer = 0;

 USART2_Send_Text("AT+CLCC\r\n");
 my_Delay_ms( 3000 );
 USART2_Send_Text("AT+CMGF=1\r\n");
 my_Delay_ms( 3000 );
}

void interruptUART() iv IVT_INT_USART2 ics ICS_AUTO
{
 uint32_t usartStatusRegister, usartDataRegister;
 usartStatusRegister = USART2_SR;

 if(usartStatusRegister &  0x00000020UL )
 {
 receiveUART.flag = 1;
 usartDataRegister = USART2_DR;
 receiveUART.buffer[receiveUART.bufferPointerWrite] = usartDataRegister;
 receiveUART.bufferPointerWrite++;
 if(receiveUART.bufferPointerWrite==1000)
 receiveUART.bufferPointerWrite=0;
 if(usartDataRegister==0x0A)
 {
 receiveUART.msgCount++;
 receiveUART.flag = 0;
 }
 }

 if(usartStatusRegister &  0x00000080UL )
 {
 if(transmitUART.bufferPointer < transmitUART.byteCount)
 USART2_DR = transmitUART.buffer[transmitUART.bufferPointer++];
 else
 {
 USART2_CR1 &= ~( 0x00000080UL );
 transmitUART.byteCount=0;
 transmitUART.bufferPointer = 0;
 transmitUART.flag = 0;
 }
 }
}

void USART2_Receive()
{
 int32_t current;
 if(receiveUART.msgCount>0)
 {
 current=0;
 receiveUART.msgCount--;
 while(receiveUART.buffer[receiveUART.bufferPointerRead]!=0x0A)
 {
 receivedTxt[current]=receiveUART.buffer[receiveUART.bufferPointerRead++];
 current++;
 if(receiveUART.bufferPointerRead==1000)
 receiveUART.bufferPointerRead=0;
 }
 receiveUART.bufferPointerRead++;
 if(receiveUART.bufferPointerRead==1000)
 receiveUART.bufferPointerRead=0;
 receivedTxt[current]=0;
 receivedFlag=1;
 }
 else
 receivedFlag=0;
}


void receive_SMS()
{
 USART2_Receive();

 if(receivedFlag==1 && receivedTxt[0]=='+' && receivedTxt[1]=='C' && receivedTxt[2]=='M' && receivedTxt[3]=='G' && receivedTxt[4]=='L' && receivedTxt[5]==':')
 {
 int pos=0;
 int numQuote=0;
 char number[20];
 int posNum=0;
 uint8_t tt[6];
 uint8_t bufff[50];
 pos = 6;
 while(receivedTxt[pos] != ','){
 tt[pos-6] = receivedTxt[pos];
 pos++;
 }
 tt[pos-6] = 0;
 while(numQuote<3)
 {
 if(receivedTxt[pos]=='"')
 numQuote++;
 pos++;
 }
 while(receivedTxt[pos]!='"')
 {
 number[posNum]=receivedTxt[pos];
 posNum++;
 pos++;
 }
 numQuote++;
 pos++;
 number[posNum]=0;
 USART2_Receive();
 showText(receivedTxt);
 if(receivedFlag==1 && receivedTxt[0]=='p' && receivedTxt[1]=='r' && receivedTxt[2]=='e' && receivedTxt[3]=='c' && receivedTxt[4]==':')
 {
 int val=0;
 int p=5;
 while(receivedTxt[p]!='\r')
 val=val*10+receivedTxt[p++]-'0';
 setPrecision(val);
 sendSMS(number, val);

 }
 strcpy(bufff, "AT+CMGD=");
 strcpy(bufff+8, tt);
 strcpy(bufff+8+strlen(tt),",0\r\n");
 USART2_Send_Text(bufff);

 my_Delay_ms(5000);
 }

}

void checkSMS(){
 USART2_Send_Text("AT+CMGL=\"REC UNREAD\"\r\n");
 my_Delay_ms( 3000 );
 receive_SMS();
 while(receivedFlag==1)
 receive_SMS();

}

void USART2_Send_Text(uint8_t* input)
{
 uint8_t input_Char;
 int32_t current;
 while(transmitUART.flag == 1);

 transmitUART.byteCount = 0;
 input_Char = *input;
 while((input_Char>=0x20 && input_Char<=0x7E) || input_Char==0x09 || input_Char==0x0A || input_Char==0x0D)
 {
 transmitUART.buffer[transmitUART.byteCount] = input_Char;
 ++input;
 input_Char = *input;
 ++transmitUART.byteCount;
 }

 transmitUART.bufferPointer = 0;

 transmitUART.flag = 1;
 USART2_DR = transmitUART.buffer[transmitUART.bufferPointer++];
 USART2_CR1 |=  0x00000080UL ;
}

void USART2_Send(char input)
{
 while(transmitUART.flag == 1);

 transmitUART.byteCount = 0;
 transmitUART.buffer[transmitUART.byteCount] = input;
 ++transmitUART.byteCount;

 transmitUART.bufferPointer = 0;

 transmitUART.flag = 1;
 USART2_DR = transmitUART.buffer[transmitUART.bufferPointer++];
 USART2_CR1 |=  0x00000080UL ;
}


void sendSMS(char* number, int val) {
 int cz = 0x1A;
 int pos=9;
 int posNum=0;
 char txtNum[50];
 uint8_t buffer[3];
 USART2_Send_Text("AT+CMGF=1\r\n");
 Delay_ms(1000);
 txtNum[0]='A'; txtNum[1]='T'; txtNum[2]='+'; txtNum[3]='C'; txtNum[4]='M'; txtNum[5]='G'; txtNum[6]='S'; txtNum[7]='='; txtNum[8]='"';
 while(number[posNum]!=0)
 {
 txtNum[pos]=number[posNum];
 pos++;
 posNum++;
 }
 txtNum[pos++]='"';
 txtNum[pos++]='\r';
 txtNum[pos++]='\n';
 txtNum[pos++]=0;
 USART2_Send_Text(txtNum);

 Delay_ms(1000);


 switch(val) {
 case  2 :
 USART2_Send_Text("Promenjena preciznost na: 1/2 stepena");
 break;
 case  4 :
 USART2_Send_Text("Promenjena preciznost na: 1/4 stepena");
 break;
 case  8 :
 USART2_Send_Text("Promenjena preciznost na: 1/8 stepena");
 break;
 case  16 :
 USART2_Send_Text("Promenjena preciznost na: 1/16 stepena");
 break;
 default:
 USART2_Send_Text("Greska: podesavanje nije moguce promeniti,\n postavljeno je podrazumevano podesavanje na 1/16 stepana");

 }




 USART2_Send(cz);
 Delay_ms(1000);
}

uint8_t getReceiveTxt()
{
 USART2_Receive();
 while(receivedFlag==1)
 USART2_Receive();
}

uint8_t checkReceiveTxt()
{
 uint8_t ok=0;
 uint32_t current;
 USART2_Receive();
 while(receivedFlag==1)
 {
 for(current=1; receivedTxt[current]!=0; current++)
 if((receivedTxt[current-1]=='O')&& (receivedTxt[current]=='K'))
 ok=1;
 USART2_Receive();
 }
 return ok;
}

uint8_t sendData(float temp, float hum, float pres, float dist) {
 uint32_t len, i;
 uint8_t txtTemp[10], txtHum[10], txtPres[10], txtDist[10];
 uint8_t url[150] = "AT+HTTPPARA=\"URL\",\"http://azaric.asuscomm.com:9998/mips/log?temp=";
 len = strlen(url);
 FloatToStr(temp, txtTemp);
 FloatToStr(hum, txtHum);
 FloatToStr(pres, txtPres);
 FloatToStr(dist, txtDist);
 for (i = 0; i < strlen(txtTemp); i++) {
 if (txtTemp[i] == '\0')
 break;
 url[len++] = txtTemp[i];
 }

 url[len++] = '&';url[len++] = 'h';url[len++] = 'u';url[len++] = 'm';url[len++] = '=';

 for (i = 0; i < strlen(txtHum); i++) {
 if (txtHum[i] == '\0')
 break;
 url[len++] = txtHum[i];
 }

 url[len++] = '&';url[len++] = 'p';url[len++] = 'r';url[len++] = 'e';url[len++] = 's';url[len++] = '=';

 for (i = 0; i < strlen(txtPres); i++) {
 if (txtPres[i] == '\0')
 break;
 url[len++] = txtPres[i];
 }

 url[len++] = '&';url[len++] = 'd';url[len++] = 'i';url[len++] = 's';url[len++] = 't';url[len++] = '=';

 for (i = 0; i < strlen(txtDist); i++) {
 if (txtDist[i] == '\0')
 break;
 url[len++] = txtDist[i];
 }
 url[len++] = '\"';url[len++] = '\r';url[len++] = '\n';url[len++] = '\0';

 USART2_Send_Text("AT+CPIN?\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;
 USART2_Send_Text("AT+CIPSHUT\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+CGATT=1\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+SAPBR=3,1,\"APN\",\"internet\"\r\n");

 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+SAPBR=3,1,\"PWD\",\"gprs\"\r\n");

 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+SAPBR=1,1\r\n");
 my_Delay_ms(3* 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+HTTPTERM\r\n");
 my_Delay_ms( 3000 );
 getReceiveTxt();

 USART2_Send_Text("AT+HTTPINIT\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+HTTPPARA=\"CID\",1\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text(url);
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+HTTPACTION=1\r\n");
 my_Delay_ms( 3000 );
 if(checkReceiveTxt()==0) return 0;

 USART2_Send_Text("AT+CIPSHUT\r\n");
 my_Delay_ms( 3000 );
 getReceiveTxt();

 USART2_Send_Text("AT+SAPBR=0,1\r\n");
 my_Delay_ms( 3000 );
 getReceiveTxt();

 USART2_Send_Text("AT+CGATT=0\r\n");
 my_Delay_ms(3* 3000 );
 getReceiveTxt();

 return 1;
}
