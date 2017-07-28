#include "FadeAnimation.h"
#include "AnimationCore.h"

const float FadeAnimation::AmountUpdateRate = 0.025f;

const char *FadeAnimation::onIdRequested() {
	return "Fade";
}

void FadeAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint8_t colors = core->GetChannelAnimationColorCount(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	uint8_t currentColorIndex = core->GetChannelAnimationCurrentColor(channelIndex);
	uint8_t nextColorIndex = currentColorIndex + 1;
	if (nextColorIndex >= colors) {
		nextColorIndex = 0;
	}

	const Color &currentColor = core->GetChannelAnimationColor(channelIndex, currentColorIndex);
	const Color &nextColor = core->GetChannelAnimationColor(channelIndex, nextColorIndex);

	float &amount = m_amount[channelIndex];

	// Blend colors
	uint8_t r = static_cast<uint8_t>(currentColor.Red * (1.0f - amount) + nextColor.Red * amount);
	uint8_t g = static_cast<uint8_t>(currentColor.Green * (1.0f - amount) + nextColor.Green * amount);
	uint8_t b = static_cast<uint8_t>(currentColor.Blue * (1.0f - amount) + nextColor.Blue * amount);
	
	// Update lights
	for (uint16_t i = 0; i < ledCount; i++)
		channel->SetLedColor(i, r, g, b);
	channel->Show();

	// Update amount
	amount += AmountUpdateRate;

	// Change next color if we've reached the color
	if (amount >= 1.0f) {
		core->SetChannelAnimationCurrentColor(channelIndex, nextColorIndex);
		amount = 0.0f;
	}
}

void FadeAnimation::onEnabled(uint8_t channelIndex) {
	m_amount[channelIndex] = 0.0f;
}

void FadeAnimation::onDisabled(uint8_t channelIndex) {

}

void FadeAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	
}