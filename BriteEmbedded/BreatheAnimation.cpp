#include "BreatheAnimation.h"
#include "AnimationCore.h"

const float BreatheAnimation::AlphaUpdateRate = 0.025f;

const char *BreatheAnimation::onIdRequested() {
	return "Breathe";
}

void BreatheAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint8_t colors = core->GetChannelAnimationColorCount(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	uint8_t currentColorIndex = core->GetChannelAnimationCurrentColor(channelIndex);
	Color currentColor = core->GetChannelAnimationColor(channelIndex, currentColorIndex);
	
	bool &increasing = m_increasing[channelIndex];
	float &alpha = m_alpha[channelIndex];

	// Update lights
	for (uint16_t i = 0; i < ledCount; i++)
		channel->SetLedColor(i, currentColor.Red * alpha, currentColor.Green * alpha, currentColor.Blue * alpha);
	channel->Show();

	// Update alpha
	if (increasing) {
		alpha += AlphaUpdateRate;
	} else {
		alpha -= AlphaUpdateRate;
	}

	// Check if we need to increase or decrease
	bool changeColor = false;
	if (alpha <= 0.0f) {
		increasing = true;
		changeColor = true;
		alpha = 0.0f;
	} else if (alpha >= 1.0f) {
		increasing = false;
		alpha = 1.0f;
	}

	if (changeColor) {
		if (++currentColorIndex >= colors) {
			currentColorIndex = 0;
		}

		core->SetChannelAnimationCurrentColor(channelIndex, currentColorIndex);
	}
}

void BreatheAnimation::onEnabled(uint8_t channelIndex) {
	m_increasing[channelIndex] = true;
	m_alpha[channelIndex] = 0.0f;
}

void BreatheAnimation::onDisabled(uint8_t channelIndex) {

}

bool BreatheAnimation::onDataReceived(uint8_t channelIndex, TypedStream &stream) {
	return true;
}
