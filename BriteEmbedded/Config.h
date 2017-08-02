#pragma once

#define FIRMWARE_VERSION 0xB0000003

// NOTE: This does NOT work correctly on the Arduino Nano so do NOT use it
// the device will be stuck in an infinite boot loop where you will not be 
// able to upload any code
//#define USE_WATCHDOG

#define STATE_LED_PIN 13

#define EEPROM_ID_LOCATION 256
#define EEPROM_ID_CHECKSUM 0xBC1D

#define SERIAL_BAUD_RATE 115200 // bps
#define SERIAL_TIMEOUT 100 // ms

#define CHANNEL_START_PIN 2
#define CHANNEL_COUNT 2
#define CHANNEL_MAX_BRIGHTNESS 255 // 0-255
#define CHANNEL_MAX_SIZE 40

#define ANIMATION_MAX_COUNT 8
#define ANIMATION_MIN_SPEED 0.01f // 0.0-1.0
#define ANIMATION_MAX_SPEED 1.00f // 0.0-1.0 (Max: 25 FPS)
#define ANIMATION_UPDATE_SPEED 250
#define ANIMATION_MAX_COLORS 8