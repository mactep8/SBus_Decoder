#ifndef CONFIG
#define CONFIG
#include <Arduino.h>

#define CHANNELS_COUNT 16
volatile uint8_t ChannelsMapA[CHANNELS_COUNT] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
volatile uint8_t ChannelsMapB[CHANNELS_COUNT] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
volatile uint8_t PPM_A = 0;
volatile uint8_t PPM_B = 0;
volatile uint8_t ChannelsBankA = 0;
volatile uint8_t ChannelsBankB = 0;
volatile uint8_t OCR_BANK_A = 0;
volatile uint8_t OCR_BANK_B = 0;

#define  CH0_on PORTD |= (1<<2) //D2
#define  CH0_off PORTD &= 0xFB //D2

#define  CH1_on PORTD |= (1<<3) //D3
#define  CH1_off PORTD &= 0xF7 //D3

#define  CH2_on PORTD |= (1<<4) //D4
#define  CH2_off PORTD &= 0xEF //D4

#define  CH3_on PORTD |= (1<<5) //D5
#define  CH3_off PORTD &= 0xDF //D5

#define  CH4_on PORTD |= (1<<6) //D6
#define  CH4_off PORTD &= 0xBF //D6

#define  CH5_on PORTD |= (1<<7) //D7
#define  CH5_off PORTD &= 0x7F //D7

#define  CH6_on PORTB |= 1 //D8
#define  CH6_off PORTB &= 0xFE //D8

#define  CH7_on PORTB |= (1<<1) //D9
#define  CH7_off PORTB &= 0xFD //D9

#define  CH8_on PORTB |= (1<<2) //D10
#define  CH8_off PORTB &= 0xFB //D10

#define  CH9_on PORTB |= (1<<3) //D11
#define  CH9_off PORTB &= 0xF7 //D11

#define  CH10_on PORTB |= (1<<4) //D12
#define  CH10_off PORTB &= 0xED //D12

#define  CH11_on PORTB |= (1<<5) //D13
#define  CH11_off PORTB &= 0xDF //D13

#define  CH12_on PORTC |= 1 //A0
#define  CH12_off PORTC &= 0xFE //A0

#define  CH13_on PORTC |= (1<<1) //A1
#define  CH13_off PORTC &= 0xFD //A1

#define  CH14_on PORTC |= (1<<2) //A2
#define  CH14_off PORTC &= 0xFB //A2

#define  CH15_on PORTC |= (1<<3) //A3
#define  CH15_off PORTC &= 0xF7 //A3


//#define SET_TIME_A(x) Set_TimeA( x )
//#define SET_TIME_B(x) Set_TimeB( x )
#define SET_TIME_PPM_A(x) Set_Time_PPMA( x )
#define SET_TIME_PPM_B(x) Set_Time_PPMB( x )


// EEPROM Addr
// Bank A
#define ADDR_PPM_BANK_A 0
#define ADDR_TIMER_LOOPS_A 1
#define ADDR_CHANNELS_A 2
#define ADDR_CHANNEL_MAP_A 3
// Bank B
#define ADDR_PPM_BANK_B 19
#define ADDR_TIMER_LOOPS_B 20
#define ADDR_CHANNELS_B 21
#define ADDR_CHANNEL_MAP_B 22

#endif
