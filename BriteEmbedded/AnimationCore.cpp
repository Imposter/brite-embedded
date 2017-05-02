#include "AnimationCore.h"

AnimationCore::AnimationCore(Channel *channels)
	: m_channels(channels) {}

void AnimationCore::Animate() {
	for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
		if (m_animationEnabled[i]) {
			Animation *animation = m_animations[i];
			if (animation != 0) {
				uint32_t currentTime = millis();
				uint32_t lastAnimation = m_lastAnimation[i];
				uint32_t deltaTime;
				if (currentTime < lastAnimation)
					deltaTime = ULONG_MAX - lastAnimation + currentTime;
				else
					deltaTime = currentTime - lastAnimation;

				if (deltaTime < (uint8_t)(ANIMATION_UPDATE_SPEED * (1.0f - m_speeds[i])))
					return;

				// Animate
				animation->Animate(i);

				// Update last animation time
				m_lastAnimation[i] = currentTime;
			}
		}
	}
}

Channel *AnimationCore::GetChannel(uint8_t channel) const {
	return &m_channels[channel];
}

void AnimationCore::ResetChannel(uint8_t channel) {
	// Clear animation for channel
	m_animations[channel] = 0;

	// Ensure animation is disabled
	m_animationEnabled[channel] = false;

	// Clear colors for channel
	for (uint8_t i = 0; i < ANIMATION_MAX_COLORS; i++)
		m_colors[channel][i] = { 0, 0, 0 };

	// Reset current color
	m_currentColor[channel] = 0;

	// Reset speed
	m_speeds[channel] = ANIMATION_MIN_SPEED;

	// Reset last animation time
	m_lastAnimation[channel] = 0;
}

Animation *AnimationCore::GetChannelAnimation(uint8_t channel) const {
	return m_animations[channel];
}

void AnimationCore::SetChannelAnimation(uint8_t channel, Animation *animation) {
	m_animations[channel] = animation;
}

const bool &AnimationCore::GetChannelAnimationEnabled(uint8_t channel) const {
	return m_animationEnabled[channel];
}

void AnimationCore::SetChannelAnimationEnabled(uint8_t channelIndex, bool enabled) {
	m_animationEnabled[channelIndex] = enabled;

	Channel &channel = m_channels[channelIndex];
	if (!enabled) {
		channel.Clear();
	}

	Animation *animation = m_animations[channelIndex];
	if (animation != 0) {
		animation->SetEnabled(channelIndex, enabled);
	}
}

const uint8_t &AnimationCore::GetChannelColorCount(uint8_t channel) const {
	return m_colorCount[channel];
}

void AnimationCore::SetChannelColorCount(uint8_t channel, uint8_t colorCount) {
	m_colorCount[channel] = colorCount;
}

const Color &AnimationCore::GetChannelColor(uint8_t channel, uint8_t color) const {
	return m_colors[channel][color];
}

void AnimationCore::SetChannelColor(uint8_t channel, uint8_t color, Color colorValue) {
	m_colors[channel][color] = colorValue;
}

const uint8_t &AnimationCore::GetChannelCurrentColor(uint8_t channel) const {
	return m_currentColor[channel];
}

void AnimationCore::SetChannelCurrentColor(uint8_t channel, uint8_t color) {
	m_currentColor[channel] = color;
}

const float &AnimationCore::GetChannelSpeed(uint8_t channel) const {
	return m_speeds[channel];
}

void AnimationCore::SetChannelSpeed(uint8_t channel, float speed) {
	m_speeds[channel] = speed;
}

const uint32_t &AnimationCore::GetChannelLastAnimation(uint8_t channel) const {
	return m_lastAnimation[channel];
}

void AnimationCore::SetChannelLastAnimation(uint8_t channel, uint32_t time) {
	m_lastAnimation[channel] = time;
}