#include "Config.h"
#include "AnimationCore.h"
#include "Hash.h"
#include <avr/wdt.h>
#include <EEPROM.h>
#ifdef USE_BLUETOOTH_SERIAL
#include "ATCommand.h"
#include <AltSoftSerial.h>
#endif

// Animations
#include "FixedAnimation.h"
#include "BreatheAnimation.h"

#ifdef USE_SERIAL
TypedStream g_serialStream(&Serial, SERIAL_TIMEOUT);
#endif

#ifdef USE_BLUETOOTH_SERIAL
AltSoftSerial g_btSerial(BLUETOOTH_SERIAL_RX_PIN, BLUETOOTH_SERIAL_TX_PIN);
TypedStream g_btSerialStream(&g_btSerial, BLUETOOTH_SERIAL_TIMEOUT);
#endif

#if defined(USE_SERIAL) || defined(USE_BLUETOOTH_SERIAL)
enum Command : uint8_t {
	kCommand_GetVersion = 0,
	kCommand_GetID,
	kCommand_SetID,
	kCommand_Reset,
	kCommand_GetCapabilities,
	kCommand_GetAnimations,

	kCommand_SetChannelBrightness,
	kCommand_SetChannelLedCount,
	kCommand_SetChannelAnimation,
	kCommand_SetChannelAnimationEnabled,
	kCommand_SetChannelAnimationSpeed,
	kCommand_SetChannelAnimationColorCount,
	kCommand_SetChannelAnimationColor,
	kCommand_SetChannelAnimationData,

	kCommand_Max,

#ifdef USE_BLUETOOTH_SERIAL
	kCommand_Bluetooth_GetPassword = 50,
	kCommand_Bluetooth_SetPassword,
	kCommand_Bluetooth_UnpairAll,
	kCommand_Bluetooth_GetStatus,
	kCommand_Bluetooth_GetConnectionInfo,

	kCommand_Bluetooth_Max
#endif
};

enum Result : uint8_t {
	kResult_Ok,
	kResult_Error,

#ifdef USE_BLUETOOTH_SERIAL
	kResult_Bluetooth_Unavailable = 50,
#endif
};
#endif

Channel g_channels[CHANNEL_COUNT];
Animation *g_animations[ANIMATION_MAX_COUNT]{ 0 };
AnimationCore g_animationCore(g_channels);

// Animations
FixedAnimation g_anim0;
BreatheAnimation g_anim1;

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
#ifdef USE_SERIAL
	Serial.begin(SERIAL_BAUD_RATE);
#endif

#ifdef USE_BLUETOOTH_SERIAL
	initializeBluetooth();
#endif

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
#ifdef USE_SERIAL
	handleStream(g_serialStream);
#endif

#ifdef USE_BLUETOOTH_SERIAL
	handleStream(g_btSerialStream);
#endif

	// Update animations
	g_animationCore.Animate();
}

#ifdef USE_BLUETOOTH_SERIAL
void initializeBluetooth() {
	pinMode(BLUETOOTH_SERIAL_KEY_PIN, OUTPUT);

	// Start bluetooth serial connection
	g_btSerial.begin(BLUETOOTH_SERIAL_BAUD_RATE);

	// Raise key to high to entire AT mode on HC-05
	digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

	// Wait to ensure HC-05 entered AT mode
	delay(BLUETOOTH_SERIAL_AT_DELAY);

	// Read checksum and ID from EEPROM
	uint16_t checkSum = 0;
	EEPROM.get<uint16_t>(EEPROM_ID_LOCATION, checkSum);
	uint32_t id = 0;
	EEPROM.get<uint32_t>(EEPROM_ID_LOCATION + sizeof(checkSum), id);

	uint32_t hash = FNV1A32((uint8_t *)id, sizeof(id));
	char name[3 + sizeof(id) * 2 + 1] = "BC-";
	if (checkSum == EEPROM_ID_CHECKSUM) {
		snprintf(name + 3, sizeof(name) - 3, "%08X", id);
	} else {
		strcpy(name, "BC-NULL");
	}

	// Set module name
	ATCommand::Write(g_btSerialStream, "AT", "NAME", 1, name);

	// Return key to low
	digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

	// Wait to ensure HC-05 exited AT mode
	delay(BLUETOOTH_SERIAL_AT_DELAY);
}
#endif

#if defined(USE_SERIAL) || defined(USE_BLUETOOTH_SERIAL)
void handleStream(TypedStream &stream) {
	uint16_t available = stream.Available();
	if (available > 0) {
		// Read command
		uint8_t command = 0;
		if (stream.Read(command)) {
			if (command >=
	#ifdef USE_BLUETOOTH_SERIAL
				kCommand_Bluetooth_Max
	#else
				kCommand_Max
	#endif
				) return;

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
				EEPROM.get<uint32_t>(EEPROM_ID_LOCATION + sizeof(checkSum), id);

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
					EEPROM.put<uint16_t>(EEPROM_ID_LOCATION, (uint16_t)EEPROM_ID_CHECKSUM);
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
			} else if (command == kCommand_GetCapabilities) {
				stream.SetDataTypeEnabled(true);
#ifdef USE_BLUETOOTH_SERIAL
				stream.WriteBoolean(true);
#else
				stream.WriteBoolean(false);
#endif
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
			} else if (command == kCommand_SetChannelAnimationData) {
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
				if (channelIndex < CHANNEL_COUNT && animation != 0 && animation->HandleStream(channelIndex, stream)) {
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Error);
				}
			}
#ifdef USE_BLUETOOTH_SERIAL
			else if (command == kCommand_Bluetooth_GetPassword) {
				stream.SetDataTypeEnabled(true);

				// Raise key to high to entire AT mode on HC-05
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

				// Wait to ensure HC-05 entered AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				char buffer[BLUETOOTH_SERIAL_BUFFER_SIZE]{ 0 };
				int16_t readSize = ATCommand::Read(g_btSerialStream, "AT", "PSWD", buffer, sizeof(buffer));

				// Return key to low
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

				// Wait to ensure HC-05 exited AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Send response to device
				if (readSize > 0) {
					char password[4]{ 0 };
					ATCommand::GetParameter(buffer, readSize, password, sizeof(password));

					stream.WriteUInt8(kResult_Ok);
					stream.WriteString(password, sizeof(password));
				} else {
					stream.WriteUInt8(kResult_Bluetooth_Unavailable);
				}
			} else if (command == kCommand_Bluetooth_SetPassword) {
				stream.SetDataTypeEnabled(true);

				// Read password
				char password[4 + 1]{ 0 };
				if (!stream.ReadString(password, 4))
					return;

				// Raise key to high to entire AT mode on HC-05
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

				// Wait to ensure HC-05 entered AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				char buffer[BLUETOOTH_SERIAL_BUFFER_SIZE]{ 0 };
				int16_t readSize = ATCommand::Write(g_btSerialStream, "AT", "PSWD", buffer, sizeof(buffer), 1, password);

				// Return key to low
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

				// Wait to ensure HC-05 exited AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Send response to device
				if (readSize > 0) {
					bool resultOk = strstr(buffer, "OK") != 0;
					stream.WriteUInt8(resultOk ? kResult_Ok : kResult_Error);
				} else {
					stream.WriteUInt8(kResult_Bluetooth_Unavailable);
				}
			} else if (command == kCommand_Bluetooth_UnpairAll) { // TODO/NOTE: When paired, this sometimes returns BluetoothUnavailable
				// Raise key to high to entire AT mode on HC-05
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

				// Wait to ensure HC-05 entered AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Unpair
				int16_t unpairReadSize = ATCommand::Execute(g_btSerialStream, "AT", "RMAAD");

				// Disconnect
				int16_t disconnectReadSize = ATCommand::Execute(g_btSerialStream, "AT", "DISC");

				// Return key to low
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

				// Wait to ensure HC-05 exited AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Send response to device
				stream.SetDataTypeEnabled(true);
				if (unpairReadSize > 0 && disconnectReadSize > 0) {
					stream.WriteUInt8(kResult_Ok);
				} else {
					stream.WriteUInt8(kResult_Bluetooth_Unavailable);
				}
			} else if (command == kCommand_Bluetooth_GetStatus) {
				// Raise key to high to entire AT mode on HC-05
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

				// Wait to ensure HC-05 entered AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				char buffer[BLUETOOTH_SERIAL_BUFFER_SIZE]{ 0 };
				int16_t readSize = ATCommand::Read(g_btSerialStream, "AT", "STATE", buffer, sizeof(buffer));

				// Return key to low
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

				// Wait to ensure HC-05 exited AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Send response to device
				stream.SetDataTypeEnabled(true);
				if (readSize > 0) {
					char state[12]{ 0 };
					ATCommand::GetParameter(buffer, readSize, state, sizeof(state));

					stream.WriteUInt8(kResult_Ok);
					stream.WriteBoolean(strcmp(state, "CONNECTED") == 0);
				} else {
					stream.WriteUInt8(kResult_Bluetooth_Unavailable);
				}
			} else if (command == kCommand_Bluetooth_GetConnectionInfo) {
				// Raise key to high to entire AT mode on HC-05
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, HIGH);

				// Wait to ensure HC-05 entered AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				char buffer[BLUETOOTH_SERIAL_BUFFER_SIZE]{ 0 };

				int16_t stateReadSize = ATCommand::Read(g_btSerialStream, "AT", "STATE", buffer, sizeof(buffer));
				char state[12]{ 0 };
				if (stateReadSize > 0) {
					ATCommand::GetParameter(buffer, stateReadSize, state, sizeof(state));
				}

				int16_t addressReadSize = ATCommand::Read(g_btSerialStream, "AT", "MRAD", buffer, sizeof(buffer));
				char address[14 + 1]{ 0 };
				if (addressReadSize > 0) {
					ATCommand::GetParameter(buffer, addressReadSize, address, sizeof(address));
				}

				// Replace colons in address into commas
				for (uint8_t i = 0; i < sizeof(address); i++)
					if (address[i] == ':')
						address[i] = ',';

				int16_t nameReadSize = ATCommand::Read(g_btSerialStream, "AT", "RNAME", buffer, sizeof(buffer), 1, address);
				char name[32]{ 0 };
				uint8_t nameLength = 0;
				if (nameReadSize > 0) {
					nameLength = ATCommand::GetParameter(buffer, nameReadSize, name, sizeof(name));
				}

				// Return key to low
				digitalWrite(BLUETOOTH_SERIAL_KEY_PIN, LOW);

				// Wait to ensure HC-05 exited AT mode
				delay(BLUETOOTH_SERIAL_AT_DELAY);

				// Send response to device
				stream.SetDataTypeEnabled(true);
				if (stateReadSize > 0 && addressReadSize > 0) {
					stream.WriteUInt8(kResult_Ok);
					if (strcmp(state, "CONNECTED") == 0 && nameReadSize > 0) {
						stream.WriteBoolean(true);
						stream.WriteString(address, 14);
						stream.WriteString(name, nameLength);
					} else {
						stream.WriteBoolean(false);
					}
				} else {
					stream.WriteUInt8(kResult_Bluetooth_Unavailable);
				}
			}
#endif
		}
	}
}
#endif
