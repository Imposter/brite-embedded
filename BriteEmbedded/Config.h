#pragma once

// NOTE: This does NOT work correctly on the Arduino Nano so do NOT use it
// the device will be stuck in an infinite boot loop where you will not be 
// able to upload any code
//#define USE_WATCHDOG

#define STATE_LED_PIN 13

#define EEPROM_ID_LOCATION 256
#define EEPROM_ID_CHECKSUM 0xBC1D

#define USE_SERIAL
#ifdef USE_SERIAL
#define SERIAL_BAUD_RATE 115200 // bps
#define SERIAL_TIMEOUT 50 // ms
#endif

#define USE_BLUETOOTH_SERIAL
#ifdef USE_BLUETOOTH_SERIAL
#define BLUETOOTH_SERIAL_KEY_PIN 10
#define BLUETOOTH_SERIAL_RX_PIN 8
#define BLUETOOTH_SERIAL_TX_PIN 9
#define BLUETOOTH_SERIAL_BAUD_RATE 38400 // bps
#define BLUETOOTH_SERIAL_TIMEOUT 250 // ms
#define BLUETOOTH_SERIAL_AT_DELAY 50 // ms
#define BLUETOOTH_SERIAL_BUFFER_SIZE 32
#endif

#define CHANNEL_START_PIN 2
#define CHANNEL_COUNT 2
#define CHANNEL_MAX_BRIGHTNESS 255 // 0-255
#define CHANNEL_MAX_SIZE 80

#define ANIMATION_MAX_COUNT 8
#define ANIMATION_MIN_SPEED 0.1f // 0.0-1.0
#define ANIMATION_MAX_SPEED 0.75f // 0.0-1.0
#define ANIMATION_UPDATE_SPEED 50
#define ANIMATION_MAX_COLORS 8

#if !defined(USE_SERIAL) && !defined(USE_BLUETOOTH_SERIAL)
#error "No serial communication is enabled!"
#endif