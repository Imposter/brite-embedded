#include "ManualAnimation.h"
#include "AnimationCore.h"

const char *ManualAnimation::onIdRequested() {
	return "Manual";
}

void ManualAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);

	// Display channel data
	channel->Show();
}

void ManualAnimation::onEnabled(uint8_t channelIndex) {

}

void ManualAnimation::onDisabled(uint8_t channelIndex) {

}

void ManualAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	// Read command
	uint8_t command = 0;
	if (stream.Read(command)) {
		if (command >= kCommand_Max)
			return;

		if (command == kCommand_SetColor) {
			uint16_t index;
			if (!stream.ReadUInt16(index))
				return;

			uint8_t r, g, b;
			if (!stream.ReadUInt8(r))
				return;
			if (!stream.ReadUInt8(g))
				return;
			if (!stream.ReadUInt8(b))
				return;

			// Check index
			if (index >= ledCount)
				return;

			// Set color
			channel->SetLedColor(index, r, g, b);
		} else if (command == kCommand_SetColorRange) {
			uint16_t start;
			if (!stream.ReadUInt16(start))
				return;

			uint16_t count;
			if (!stream.ReadUInt16(count))
				return;

			if (start + count > ledCount)
				return;

			uint8_t r, g, b;
			if (!stream.ReadUInt8(r))
				return;
			if (!stream.ReadUInt8(g))
				return;
			if (!stream.ReadUInt8(b))
				return;

			// Set colors
			for (uint16_t i = 0; i < count; i++)
				channel->SetLedColor(start + i, r, g, b);
		} else if (command == kCommand_SetColors) {
			uint16_t count;
			if (!stream.ReadUInt16(count))
				return;

			if (ledCount != count)
				return;

			uint8_t r, g, b;
			for (uint16_t i = 0; i < ledCount; i++) {
				if (!stream.ReadUInt8(r))
					return;
				if (!stream.ReadUInt8(g))
					return;
				if (!stream.ReadUInt8(b))
					return;

				channel->SetLedColor(i, r, g, b);
			}
		} else if (command == kCommand_SetColorsRange) {
			uint16_t start;
			if (!stream.ReadUInt16(start))
				return;

			uint16_t count;
			if (!stream.ReadUInt16(count))
				return;

			if (start + count > ledCount)
				return;

			uint8_t r, g, b;
			for (uint16_t i = 0; i < count; i++) {
				if (!stream.ReadUInt8(r))
					return;
				if (!stream.ReadUInt8(g))
					return;
				if (!stream.ReadUInt8(b))
					return;

				channel->SetLedColor(start + i, r, g, b);
			}
		}
	}
}