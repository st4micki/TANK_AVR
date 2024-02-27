# TANK_AVR
ATmega 328p program for a wheeled robot.
Features:
 - L298N DC motor driver throuh GPIO ports control
 - servo motor fast 8-bit PWM control
 - 3 channel ADC support for 2 joysticks 
 - joystick button support through external interrupt

Pinout:
SERVO PWM -  PD6 FAST PWM    
WHEELS IN1-4: PB1 - PB4
LED TURRET - PB5 BUILT IN LED
JOY1 Y - PC0 ADC0
JOY1 X - PC1 ADC1
JOY1 BUTTON - PD2 INT0 DONE
JOY2 X - PC2 ADC2

source code is located in TANK_AVR/TANK/src/main.cpp
