#include "Config.h"
#include "AnimationCore.h"
#include "Hash.h"
#include <avr/wdt.h>
#include <EEPROM.h>

// Animations
#include "ManualAnimation.h"
#include "FixedAnimation.h"
#include "BreatheAnimation.h"
#include "PulseAnimation.h"
#include "FadeAnimation.h"
#include "SpiralAnimation.h"
#include "MarqueeAnimation.h"

TypedStream g_serialStream(&Serial, SERIAL_TIMEOUT);

enum Command : uint8_t {
	kCommand_GetVersion = 0,
	kCommand_GetID,
	kCommand_SetID,
	kCommand_Reset,
	kCommand_GetParameters,
	kCommand_GetAnimations,

	kCommand_SetChannelBrightness,
	kCommand_SetChannelLedCount,
	kCommand_SetChannelAnimation,
	kCommand_SetChannelAnimationEnabled,
	kCommand_SetChannelAnimationSpeed,
	kCommand_SetChannelAnimationColorCount,
	kCommand_SetChannelAnimationColor,
	kCommand_SendChannelAnimationRequest,

	kCommand_Max,
};

enum Result : uint8_t {
	kResult_Ok,
	kResult_Error,
};

Channel g_channels[CHANNEL_COUNT];
Animation *g_animations[ANIMATION_MAX_COUNT]{ 0 };
AnimationCore g_animationCore(g_channels);

// Animations
ManualAnimation g_anim0;
FixedAnimation g_anim1;
BreatheAnimation g_anim2;
PulseAnimation g_anim3;
FadeAnimation g_anim4;
SpiralAnimation g_anim5;
MarqueeAnimation g_anim6;

// Forward declarations
void handleStream(TypedStream &stream);

void setup() {
	// Set up state pin for output
	pinMode(STATE_LED_PIN, OUTPUT);
	digitalWrite(STATE_LED_PIN, LOW);

#ifdef USE_WATCHDOG
	// Set up watchdog timer, we'll use this in case we freeze during serial communication to reset
	// the device
	wdt_enable(WDTO_1S);
#endif

	// Initialize serial
	Serial.begin(SERIAL_BAUD_RATE);

	// Initialize channels
	for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
		Channel &channel = g_channels[i];
		channel.SetPin(CHANNEL_START_PIN + i);
		channel.SetMaxLedCount(CHANNEL_MAX_SIZE);
		channel.SetBrightness(CHANNEL_MAX_BRIGHTNESS);
		channel.Initialize();
	}
	
	// Initialize animations
	g_animations[0] = &g_anim0;
	g_animations[1] = &g_anim1;
	g_animations[2] = &g_anim2;
	g_animations[3] = &g_anim3;
	g_animations[4] = &g_anim4;
	g_animations[5] = &g_anim5;
	g_animations[6] = &g_anim6;

	for (uint8_t i = 0; i < ANIMATION_MAX_COUNT; i++) {
		Animation *animation = g_animations[i];
		if (animation != 0)
			animation->Initialize(&g_animationCore);
	}

	// Turn on state LED
	digitalWrite(STATE_LED_PIN, HIGH);
}

void loop() {
#ifdef USE_WATCHDOG
	// Notify watchdog that we're still running
	wdt_reset();
#endif

	// Handle incoming serial data
	handleStream(g_serialStream);

	// Update animations
	g_animationCore.Animate();
}

void handleStream(TypedStream &stream) {
	uint16_t available = stream.Available();
	if (available > 0) {
		// Read command
		uint8_t command = 0;
		if (stream.Read(command)) {
			if (command >= kCommand_Max) 
				return;

			// Write command back to device
			stream.Write(command);

			if (command == kCommand_GetVersion) {
				stream.SetDataTypeEnabled(true);
				stream.WriteUInt32(FIRMWARE_VERSION);
			} else if (command == kCommand_GetID) {
				// Read checksum and ID from EEPROM
				uint16_t checkSum = 0;
				EEPROM.get<uint16_t>(EEPROM_ID_LOCATION, checkSum);
				uint32_t id = 0;
				EEPROM.get<uint32_t>(EEPROM_ID_LOCATION + sizeof checkSum, id);

				stream.SetDataTypeEnabled(true);
				if (checkSum == EEPROM_ID_CHECKSUM) {
					stream.WriteUInt8(kResult_Ok);
					stream.WriteUInt32(id);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetID) {
				// Read ID provided by user
				uint32_t id = 0;
				if (!stream.ReadUInt32(id))
					return;

				// Read checksum from EEPROM
				uint16_t checkSum = 0;
				EEPROM.get<uint16_t>(EEPROM_ID_LOCATION, checkSum);
				
				stream.SetDataTypeEnabled(true);
				if (checkSum != EEPROM_ID_CHECKSUM) {
					// Set ID
					EEPROM.put<uint16_t>(EEPROM_ID_LOCATION, static_cast<uint16_t>(EEPROM_ID_CHECKSUM));
					EEPROM.put<uint32_t>(EEPROM_ID_LOCATION + sizeof(uint16_t), id);
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_Reset) {
				for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
					Channel &channel = g_channels[i];

					// Reset channel size
					channel.SetMaxLedCount(CHANNEL_MAX_SIZE);

					// Reset brightness
					channel.SetBrightness(CHANNEL_MAX_BRIGHTNESS);

					// Clear channels
					channel.Clear();

					// Disable animations
					g_animationCore.SetChannelAnimationEnabled(i, false);

					// Reset core for channel
					g_animationCore.ResetChannel(i);
				}

				stream.SetDataTypeEnabled(true);
				stream.WriteUInt8(kResult_Ok);
			} else if (command == kCommand_GetParameters) {
				stream.SetDataTypeEnabled(true);
				stream.WriteUInt8(CHANNEL_COUNT);
				stream.WriteUInt16(CHANNEL_MAX_SIZE);
				stream.WriteUInt8(CHANNEL_MAX_BRIGHTNESS);
				stream.WriteUInt8(ANIMATION_MAX_COLORS);
				stream.WriteFloat(ANIMATION_MIN_SPEED);
				stream.WriteFloat(ANIMATION_MAX_SPEED);
			} else if (command == kCommand_GetAnimations) {
				uint8_t animCount = 0;
				for (; animCount < ANIMATION_MAX_COUNT; animCount++)
					if (g_animations[animCount] == 0)
						break;

				stream.SetDataTypeEnabled(true);
				stream.WriteUInt8(animCount);
				for (uint8_t i = 0; i < animCount; i++)
					stream.WriteUInt32(g_animations[i]->GetId());
			} else if (command == kCommand_SetChannelBrightness) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint8_t brightness = 0;
				if (!stream.ReadUInt8(brightness))
					return;

				if (channelIndex < CHANNEL_COUNT && brightness <= CHANNEL_MAX_BRIGHTNESS) {
					Channel &channel = g_channels[channelIndex];
					channel.SetBrightness(brightness);

					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelLedCount) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint16_t ledCount = 0;
				if (!stream.ReadUInt16(ledCount))
					return;

				if (channelIndex < CHANNEL_COUNT && ledCount >= 0 && ledCount < CHANNEL_MAX_SIZE) {
					Channel &channel = g_channels[channelIndex];
					channel.SetLedCount(ledCount);

					// Reset animations
					g_animationCore.SetChannelAnimationEnabled(channelIndex, false);

					// Reset channel
					g_animationCore.ResetChannel(channelIndex);
					
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelAnimation) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint32_t animId = 0;
				if (!stream.ReadUInt32(animId))
					return;

				// Find animation
				Animation *animation = 0;
				for (uint8_t i = 0; i < ANIMATION_MAX_COUNT; i++) {
					Animation *anim = g_animations[i];
					if (anim != 0 && anim->GetId() == animId) {
						animation = anim;
						break;
					}
				}

				// Set channel animation
				if (channelIndex < CHANNEL_COUNT && animation != 0) {
					// Disable previous animation, if any
					g_animationCore.SetChannelAnimationEnabled(channelIndex, false);

					// Reset channel
					g_animationCore.ResetChannel(channelIndex);

					// Change animation
					g_animationCore.SetChannelAnimation(channelIndex, animation);

					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelAnimationEnabled) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				bool enabled = 0;
				if (!stream.ReadBoolean(enabled))
					return;

				// Set channel animation enabled
				Animation *animation = g_animationCore.GetChannelAnimation(channelIndex);
				if (channelIndex < CHANNEL_COUNT && animation != 0) {
					g_animationCore.SetChannelAnimationEnabled(channelIndex, enabled);
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelAnimationSpeed) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				float speed = 0;
				if (!stream.ReadFloat(speed))
					return;

				if (channelIndex < CHANNEL_COUNT && speed >= ANIMATION_MIN_SPEED && speed <= ANIMATION_MAX_SPEED) {
					g_animationCore.SetChannelAnimationSpeed(channelIndex, speed);
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelAnimationColorCount) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint8_t count = 0;
				if (!stream.ReadUInt8(count))
					return;

				if (channelIndex < CHANNEL_COUNT && count <= ANIMATION_MAX_COLORS) {
					g_animationCore.SetChannelAnimationColorCount(channelIndex, count);
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SetChannelAnimationColor) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint8_t index = 0;
				if (!stream.ReadUInt8(index))
					return;

				uint8_t r = 0;
				if (!stream.ReadUInt8(r))
					return;

				uint8_t g = 0;
				if (!stream.ReadUInt8(g))
					return;

				uint8_t b = 0;
				if (!stream.ReadUInt8(b))
					return;

				if (channelIndex < CHANNEL_COUNT && index < CHANNEL_MAX_SIZE) {
					g_animationCore.SetChannelAnimationColor(channelIndex, index, { r, g, b });
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			} else if (command == kCommand_SendChannelAnimationRequest) {
				stream.SetDataTypeEnabled(true);

				uint8_t channelIndex = 0;
				if (!stream.ReadUInt8(channelIndex))
					return;

				uint32_t animId = 0;
				if (!stream.ReadUInt32(animId))
					return;

				// Find animation
				Animation *animation = 0;
				if (animId != 0) {
					for (uint8_t i = 0; i < ANIMATION_MAX_COUNT; i++) {
						Animation *anim = g_animations[i];
						if (anim != 0 && anim->GetId() == animId) {
							animation = anim;
							break;
						}
					}
				}

				// Set channel animation data and handle stream
				if (channelIndex < CHANNEL_COUNT && animation != 0) {
					stream.SetDataTypeEnabled(true);
					stream.WriteUInt8(kResult_Ok);

					stream.SetDataTypeEnabled(false);
					animation->HandleRequest(channelIndex, stream);
				} else {
					stream.SetDataTypeEnabled(true);
					stream.WriteUInt8(kResult_Error);
				}
			}
		}
	}
}