/*
 * mega8A_Bluethoot_and_Gyro.c
 *
 * Created: 2018-06-28 오전 8:44:39
 * Author : juwon
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit));
#define F_CPU  8000000L;
#define BAUD_RATE 9600;

typedef unsigned char byte;
static int USART_putchar(char c, FILE *stream);
static FILE device = FDEV_SETUP_STREAM(USART_putchar, NULL, _FDEV_SETUP_WRITE);

void USART_set(void)
{
	UBRRH = 0;
	UBRRL = 51;//9600
	UCSRA = 0x00;
	UCSRB = 0x18;
	UCSRC = 0x87;
}

static int USART_putchar(char c, FILE *stream)
{
	if(c == '\n')
	{   loop_until_bit_is_set(UCSRA,UDRE);
		UDR = 0x0D;
		loop_until_bit_is_set(UCSRA,UDRE);
		UDR = 0x0A;
	}
	else
	{  loop_until_bit_is_set(UCSRA,UDRE);
		UDR = c;
	}
	return 0;
}




void MPU6050_write(byte addr, byte dat)
{
	
	TWCR = 0xa4; //Start
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x08)));
	TWDR=0xd0;  //AD+W
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x18)));
	TWDR=addr;  //  RA
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x28)));
	TWDR=dat;
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x28)));
	TWCR = 0x94;
	_delay_us(50);
}


byte MPU6050_read(byte addr)
{
	byte dat;
	
	TWCR = 0xa4; //Start
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x08)));
	TWDR=0xd0;  //AD+W
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x18)));
	TWDR=addr;  //  RA
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x28)));
	
	TWCR=0xa4;   // restart (S)
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x10)));
	TWDR=0xd1;   // AD+R
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x40)));
	
	TWCR=0x84;
	
	while(((TWCR & 0x80) == 0x00 || ((TWSR & 0xf8)!=0x58))); //NO ACK
	dat = TWDR;
	TWCR = 0x94;
	
	return dat;
}

void main(void)
{
	byte dat0, dat1,dat2, dat3,dat4, dat5,dat6, dat7,dat8, dat9,dat10, dat11;

	TWSR=0x00;
	TWBR=0x12;
	
	USART_set();
	MPU6050_write(0x6b,0x00);
	MPU6050_write(0x6c,0x00);
	MPU6050_write(0x1b,0x10);  // gyroscope 설정, 최대 1000도/s로 설정함
	MPU6050_write(0x1c,0x10);  // accelerometer 설정, 최대 4g로 설정함
	
	while (1)
	{

		dat0=MPU6050_read(0x43);
		dat1=MPU6050_read(0x44);
		dat2=MPU6050_read(0x45);
		dat3=MPU6050_read(0x46);
		dat4=MPU6050_read(0x47);
		dat5=MPU6050_read(0x48);
		
		
		dat6=MPU6050_read(0x3b);
		dat7=MPU6050_read(0x3c);
		dat8=MPU6050_read(0x3d);
		dat9=MPU6050_read(0x3e);
		dat10=MPU6050_read(0x3f);
		dat11=MPU6050_read(0x40);

		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",0xff,0xff,dat0,dat1,dat2,dat3,dat4,dat5,dat6,dat7,dat8,dat9,dat10,dat11,0);
		//printf("%c%c%c%c%c%c%c%c%c",0xff,0xff,dat0,dat1,dat2,dat3,dat4,dat5,0);
		
		
		_delay_ms(2);
	}
}


