#include "MarqueeAnimation.h"
#include "AnimationCore.h"

const char *MarqueeAnimation::onIdRequested() {
	return "Marquee";
}

void MarqueeAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint8_t colors = core->GetChannelAnimationColorCount(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	uint8_t currentColorIndex = core->GetChannelAnimationCurrentColor(channelIndex);
	const Color &currentColor = core->GetChannelAnimationColor(channelIndex, currentColorIndex);

	uint16_t &position = m_position[channelIndex];
	bool &forward = m_forward[channelIndex];

	// Animate
	channel->SetLedColor(position, currentColor);

	// Update position
	bool changeColor = false;
	if (forward) {
		if (position == ledCount - 1) {
			position = 0;
			changeColor = true;
		} else {
			position++;
		}
	} else {
		if (position == 0) {
			position = ledCount - 1;
			changeColor = true;
		} else {
			position--;
		}
	}

	if (changeColor) {
		if (++currentColorIndex >= colors) {
			currentColorIndex = 0;
		}

		core->SetChannelAnimationCurrentColor(channelIndex, currentColorIndex);
	}

	// Display channel data
	channel->Show();
}

void MarqueeAnimation::onEnabled(uint8_t channelIndex) {
	m_position[channelIndex] = 0;
}

void MarqueeAnimation::onDisabled(uint8_t channelIndex) {

}

void MarqueeAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);

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
		}
	}
}