#include "SpiralAnimation.h"
#include "AnimationCore.h"

const char *SpiralAnimation::onIdRequested() {
	return "Spiral";
}

void SpiralAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint8_t colors = core->GetChannelAnimationColorCount(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	// Get background color
	const Color &backgroundColor = core->GetChannelAnimationColor(channelIndex, 0);

	// Clear
	for (uint16_t i = 0; i < ledCount; i++)
		channel->SetLedColor(i, backgroundColor);

	uint16_t &position = m_position[channelIndex];
	uint8_t &groupSize = m_groupSize[channelIndex];
	bool &forward = m_forward[channelIndex];

	// Animate
	for (uint8_t i = 0; i < colors - 1; i++) {
		int16_t start = position + i * groupSize;
		const Color &currentColor = core->GetChannelAnimationColor(channelIndex, colors - i - 1);

		for (int16_t j = 0; j < groupSize; j++) {
			int16_t k = forward ? start + j : start - j;
			if (k < 0) k = k + ledCount;
			if (k > ledCount) k = k - ledCount - 1;

			channel->SetLedColor(k, currentColor);
		}
	}

	// Update position
	if (forward) {
		if (position == ledCount - 1) position = 0;
		else position++;
	} else {
		if (position == 0) position = ledCount - 1;
		else position--;
	}

	// Display channel data
	channel->Show();
}

void SpiralAnimation::onEnabled(uint8_t channelIndex) {
	m_position[channelIndex] = 0;
}

void SpiralAnimation::onDisabled(uint8_t channelIndex) {

}

void SpiralAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	// Read command
	uint8_t command = 0;
	if (stream.Read(command)) {
		if (command >= kCommand_Max)
			return;

		if (command == kCommand_SetForwardEnabled) {
			bool forward;
			if (!stream.ReadBoolean(forward))
				return;

			m_forward[channelIndex] = forward;
		} else if (command == kCommand_SetGroupSize) {
			uint16_t size;
			if (!stream.ReadUInt16(size))
				return;

			if (size > ledCount)
				return;

			m_position[channelIndex] = 0;
			m_groupSize[channelIndex] = size;
		}
	}
}