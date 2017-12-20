/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

 #pragma once

#include "Config.h"
#include "Channel.h"
#include "Animation.h"
#include "Color.h"

class AnimationCore {
	Channel *m_channels{ 0 };
	Animation *m_animations[CHANNEL_COUNT]{ 0 };
	bool m_animationEnabled[CHANNEL_COUNT]{ false };
	float m_speeds[CHANNEL_COUNT]{ ANIMATION_MIN_SPEED };
	uint8_t m_colorCount[CHANNEL_COUNT]{ 0 };
	Color m_colors[CHANNEL_COUNT][ANIMATION_MAX_COLORS]{ { { 0, 0, 0 } } };
	uint8_t m_currentColor[CHANNEL_COUNT]{ 0 };
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

	const float &GetChannelAnimationSpeed(uint8_t channel) const;
	void SetChannelAnimationSpeed(uint8_t channel, float speed);

	const uint8_t &GetChannelAnimationColorCount(uint8_t channel) const;
	void SetChannelAnimationColorCount(uint8_t channel, uint8_t colorCount);

	const Color &GetChannelAnimationColor(uint8_t channel, uint8_t color) const;
	void SetChannelAnimationColor(uint8_t channel, uint8_t color, Color colorValue);

	const uint8_t &GetChannelAnimationCurrentColor(uint8_t channel) const;
	void SetChannelAnimationCurrentColor(uint8_t channel, uint8_t color);

	const uint32_t &GetChannelLastAnimation(uint8_t channel) const;
	void SetChannelLastAnimation(uint8_t channel, uint32_t time);
};