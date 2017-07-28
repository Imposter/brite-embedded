#include "PulseAnimation.h"
#include "AnimationCore.h"

const float PulseAnimation::AlphaUpdateRate = 0.025f;

const char *PulseAnimation::onIdRequested() {
	return "Pulse";
}

void PulseAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint8_t colors = core->GetChannelAnimationColorCount(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	uint8_t currentColorIndex = core->GetChannelAnimationCurrentColor(channelIndex);
	const Color &currentColor = core->GetChannelAnimationColor(channelIndex, currentColorIndex);

	float &alpha = m_alpha[channelIndex];

	// Update lights
	for (uint16_t i = 0; i < ledCount; i++)
		channel->SetLedColor(i, currentColor.Red * alpha, currentColor.Green * alpha, currentColor.Blue * alpha);
	channel->Show();

	// Update alpha
	alpha -= AlphaUpdateRate;

	// Check if we need to increase or decrease
	bool changeColor = false;
	if (alpha <= 0.0f) {
		changeColor = true;
		alpha = 1.0f;
	}

	if (changeColor) {
		if (++currentColorIndex >= colors) {
			currentColorIndex = 0;
		}

		core->SetChannelAnimationCurrentColor(channelIndex, currentColorIndex);
	}
}

void PulseAnimation::onEnabled(uint8_t channelIndex) {
	m_alpha[channelIndex] = 0.0f;
}

void PulseAnimation::onDisabled(uint8_t channelIndex) {

}

void PulseAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	
}
