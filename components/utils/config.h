// Copyright 2022 PWrInSpace

/*! 
 *  \file contains IO numbers and peripherals configuration specific
 *        to the AirBreaker PCB
*/

// SPI
#define PCB_MOSI 25
#define PCB_MISO 27
#define PCB_SCK 26

#define PCB_SD_CS 33

// I2C
#define PCB_SDA 21
#define PCB_SCL 22

// UART
#define PCB_RX 3
#define PCB_TX 1

// PWMs
#define PCB_SERVO1_PWM 17
#define PCB_SERVO2_PWM 18
#define PCB_SERVO3_PWM 19

// LEDs
#define PCB_LED_X 15
#define PCB_LED_Y 12
#define PCB_LED_Z 14

// INPUT_ONLY
#define PCB_BATT_CHECK_ADC 32
#define PCB_START_STATE 35

// ADC
#define BATT_ADC_CHANNEL ADC_CHANNEL_4

// OTHER
#define PCB_BUZZER 23
#define PCB_IGNITER1 5
#define PCB_ADDITIONAL1 16
#define PCB_ADDITIONAL2 4
#define PCB_ADDITIONAL3 2
