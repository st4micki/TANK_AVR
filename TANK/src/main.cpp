// ARDUINO:                          _______________                          ARDUINO:
// RESET          (PCINT14/RESET)PC6|1            28|PC5(ADC5/SCL/PCINT13)         A5
// D0(RX)           (PCINT16/RXD)PD0|2            27|PC4(ADC4/SDA/PCINT12)         A4
// D1(TX)           (PCINT17/TXD)PD1|3            26|PC3(ADC3/PCINT11)             A3
// D2              (PCINT18/INT0)PD2|4            25|PC2(ADC2/PCINT10)             A2
// D3(PWM)    (PCINT19/OC2B/INT1)PD3|5            24|PC1(ADC1/PCINT9)              A1
// D4            (PCINT20/XCK/T0)PD4|6 ATmega328P 23|PC0(ADC0/PCINT8)              A0
//                               VCC|7            22|GND       
//                               GND|8            21|AREF
// CRYSTAL   (PCINT6/XTAL1/TOSC1)PB6|9            20|AVCC
// CRYSTAL   (PCINT7/XTAL2/TOSC2)PB7|10           19|PB5(SCK/PCINT15)             D13
// D5(PWM)      (PCINT21/OC0B/T1)PD5|11           18|PB4(MISO/PCINT4)             D12
// D6(PWM)    (PCINT22/OC0A/AIN0)PD6|12           17|PB3(MOSI/OC2A/PCINT3)   D11(PWM)
// D7              (PCINT23/AIN1)PD7|13           16|PB2(SS/OC1B/PCINT2)     D10(PWM)
// D8          (PCINT0/CLK0/ICP1)PB0|14           15|PB1(OC1A/PCINT1)         D9(PWM)
//                                  |_______________|   





//TANK DRIVER SOFTWARE
//SERVO PWM -  PD6 FAST PWM    
//WHEELS IN1-4: PB1 - PB4
//LED TURRET - PB5 BUILT IN LED
//JOY1 Y - PC0 ADC0
//JOY1 X - PC1 ADC1
//JOY1 BUTTON - PD2 INT0 DONE
//JOY2 X - PC2 ADC2
int duty_cycle;
int servo_stop_duty = 25;
int servo_left_duty = 1;
int servo_right_duty = 255;
int joy1_y = 0;
int joy1_x = 0;
int joy2_x = 0;


                    
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

void PWM_setup(){
  TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
  TIMSK0 |= _BV(TOIE0);
  TCCR0B |= _BV(CS00) | _BV(CS02);
  OCR0A = servo_stop_duty;
}

void ADC_start_conversion(){
  ADCSRA |= _BV(ADSC);
}

void ADC_setup(){
  ADMUX |= 0x40; 
  ADCSRA |= _BV(ADEN) | _BV(ADIE) |  _BV(ADPS1) | _BV(ADPS0);
  DIDR0 |= _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D); 
  ADC_start_conversion();
}

void forward(){
  PORTB |= _BV(PB1);
  PORTB |= _BV(PB3);
}

void backward(){
  PORTB |= _BV(PB2);
  PORTB |= _BV(PB4);
}

void left(){
  PORTB |= _BV(PB2);
  PORTB |= _BV(PB3);

}

void right(){
  PORTB |= _BV(PB1);
  PORTB |= _BV(PB4);
}

void stop(){
  PORTB &= ~(_BV(PB1));
  PORTB &= ~(_BV(PB2));
  PORTB &= ~(_BV(PB3));
  PORTB &= ~(_BV(PB4));
}

void servo_left(){
  duty_cycle = servo_left_duty;
  PORTB &= ~(_BV(PB1));
  PORTB &= ~(_BV(PB3));
}

void servo_right(){
  duty_cycle = servo_right_duty;
}

void servo_stop(){
  duty_cycle = servo_stop_duty;
}

ISR(INT0_vect){
  PORTB ^= _BV(PB0);
}

ISR(TIMER0_OVF_vect){
  OCR0A = duty_cycle;
}

ISR(ADC_vect){
  switch (ADMUX)
  {
  case 0x40:
    joy1_y = ADC;
    ADMUX = 0x41;
    break;
  case 0x41:
    joy1_x = ADC;
    ADMUX = 0x42;
    break;
  case 0x42:
    joy2_x = ADC;
    ADMUX = 0x43;
    break;
  case 0x43:
    ADMUX = 0x40;
    //EMPTY STATE SO JOY1_Y AND JOY_2X DONT OVERWRITE THEMSELVES
    break;

  }
  ADC_start_conversion();
}

int main(void){
  //SETTING UP WHEEL PINS
  DDRB = 0x1f;
  //SETTING UP BUTTON INPUT
  PORTD |= _BV(PD2); //PULL UP
  //SETTING UP EXTERNAL INTERRUPT INT0
  EICRA |= _BV(ISC01);
  sei();
  EIMSK |= _BV(INT0);
  //SETTING UP PWM
  DDRD = 0x40;
  PWM_setup();
  ADC_setup();




  while(1){
    if(400 < joy1_x && joy1_x < 600){
      if(joy1_y > 600){
        forward();
      }
      else if(joy1_y < 400){
        backward();
      }
      else{
        stop();
      }
    }


    if(400 < joy1_y && joy1_y < 600){
      if(joy1_x > 600){
        left();
      }
      else if(joy1_x < 400){
        right();
      }
      else{
        stop();
      }
    }
    if(joy1_y > 400 && joy1_y < 600){
      if(joy2_x > 600){
        servo_left();
      }
      else if(joy2_x < 400){
        servo_right();
      }
      else{
        servo_stop();
      }
    }
  }
}