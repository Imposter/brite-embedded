#pragma once

#include "Config.h"
#include "Channel.h"
#include "Animation.h"
#include "Color.h"

class AnimationCore {
	Channel *m_channels{ 0 };
	Animation *m_animations[CHANNEL_COUNT]{ 0 };
	bool m_animationEnabled[CHANNEL_COUNT]{ false };
	uint8_t m_colorCount[CHANNEL_COUNT]{ 0 };
	Color m_colors[CHANNEL_COUNT][ANIMATION_MAX_COLORS]{ { { 0, 0, 0 } } };
	uint8_t m_currentColor[CHANNEL_COUNT]{ 0 };
	float m_speeds[CHANNEL_COUNT]{ ANIMATION_MIN_SPEED };
	uint32_t m_lastAnimation[CHANNEL_COUNT]{ 0 };

public:
	AnimationCore(Channel *channels);

	void Animate();

	Channel *GetChannel(uint8_t channel) const;
	void ResetChannel(uint8_t channel);

	Animation *GetChannelAnimation(uint8_t channel) const;
	void SetChannelAnimation(uint8_t channel, Animation *animation);

	const bool &GetChannelAnimationEnabled(uint8_t channel) const;
	void SetChannelAnimationEnabled(uint8_t channel, bool enabled);

	const uint8_t &GetChannelColorCount(uint8_t channel) const;
	void SetChannelColorCount(uint8_t channel, uint8_t colorCount);

	const Color &GetChannelColor(uint8_t channel, uint8_t color) const;
	void SetChannelColor(uint8_t channel, uint8_t color, Color colorValue);

	const uint8_t &GetChannelCurrentColor(uint8_t channel) const;
	void SetChannelCurrentColor(uint8_t channel, uint8_t color);

	const float &GetChannelSpeed(uint8_t channel) const;
	void SetChannelSpeed(uint8_t channel, float speed);

	const uint32_t &GetChannelLastAnimation(uint8_t channel) const;
	void SetChannelLastAnimation(uint8_t channel, uint32_t time);
};