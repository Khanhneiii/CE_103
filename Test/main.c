/*
 * Test.c
 *
 * Created: 3/17/2023 11:23:11 PM
 * Author : nhakh
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define sbit(port,bit) (port) |= (1<<(bit))
#define cbit(port,bit) (port) &= (1<<(bit))
#define tbit(port,bit) (port) ^= (1<<(bit))

#define RED_LIGHT 0x01
#define YELLOW_LIGHT 0x02
#define GREEN_LIGHT 0x04

#define RED_TIME1 TimeConfig_1[2]
#define YELLOW_TIME1 TimeConfig_1[1]
#define GREEN_TIME1 TimeConfig_1[0]

#define RED_TIME2 TimeConfig_2[0]
#define YELLOW_TIME2 TimeConfig_2[1]
#define GREEN_TIME2 TimeConfig_2[2]

uint8_t MatrixLed[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x98,0x00};// Dem tu 0 -> 9.
int RedTime_Config = 10, GreenTime_Config = 5, YellowTime_Config = 5;
	
int TimeConfig_1[3] = {5,5,10};
int TimeConfig_2[3] = {10,5,5};
int LightTime1 = 0, LightTime2 = 0;

int index1 = 0, index2 = 0;

void HienThiMatranLed(int time1, int time2) {
	int chuc = time1 / 10;
	int donvi = time1 % 10;
	PORTB = 0X01;
	PORTA = (MatrixLed[chuc]);
	_delay_ms(10);
	PORTB = 0x02;
	PORTA = (MatrixLed[donvi]);
	_delay_ms(10);
	
	chuc = time2 / 10;
	donvi = time2 % 10;
	PORTB = 0X04;
	PORTA = (MatrixLed[chuc]);
	_delay_ms(10);
	PORTB = 0x08;
	PORTA = (MatrixLed[donvi]);
	_delay_ms(10);
	
	
}

void init_MatrixLed() {
	DDRA = 0xFF;
	DDRB = 0xFF;
}

void Traffic_Light(int light1, int light2) {
	PORTC = light1 | (light2 << 3) ;
}

void init_PORT() {
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0x00;
}

void CountDown_LedMatrix(unsigned int time) {
	while (time) {
		
	}
}


int main(void)
{
	init_PORT();
	init_MatrixLed();
	TCCR1B = (1<<CS12);
	TCNT1 = 34286;
	TIMSK = (1<<TOIE1);
	sei();
	LightTime1 = TimeConfig_1[index1];
	LightTime2 = TimeConfig_2[index2];
	int Light1 = 0, Light2 = 0;
	while (1) {
		if (index1 == 0) {
			Light1 = GREEN_LIGHT;
		}
		else if (index1 == 1) {
			Light1 = YELLOW_LIGHT;
		}
		else if (index1 == 2) {
			Light1 = RED_LIGHT;
		}
		
		if (index2 == 0) {
			Light2 = RED_LIGHT;
		}
		else if (index2 == 1) {
			Light2 = GREEN_LIGHT;
		}
		else if (index2 == 2) {
			Light2 = YELLOW_LIGHT;
		}
		
		HienThiMatranLed(LightTime1, LightTime2);
		Traffic_Light(Light1,Light2);
	}
	
}

ISR (TIMER1_OVF_vect ){
	TCNT1=34286; //gan gia tri khoi tao cho T/C1
	tbit(PORTB,0);
	tbit(PORTB,1);
	if (LightTime1 >= 0) {
		LightTime1--;
		if (LightTime1 == -1) {
			index1 = (++index1) % 3;
			LightTime1 = TimeConfig_1[index1];
		}
	}
	
	if (LightTime2 >= 0) {
		LightTime2--;
		if (LightTime2 == -1) {
			index2 = (++index2) % 3;
			LightTime2 = TimeConfig_2[index2];
		}
	}
}