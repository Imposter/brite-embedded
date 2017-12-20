/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#include "FixedAnimation.h"
#include "AnimationCore.h"

const char *FixedAnimation::onIdRequested() {
	return "Fixed";
}

void FixedAnimation::onAnimate(uint8_t channelIndex) {
	// Get data
	AnimationCore *core = getCore();
	Channel *channel = core->GetChannel(channelIndex);
	uint16_t ledCount = channel->GetLedCount();

	uint8_t currentColorIndex = core->GetChannelAnimationCurrentColor(channelIndex);
	const Color &currentColor = core->GetChannelAnimationColor(channelIndex, currentColorIndex);

	// Update lights
	for (uint16_t i = 0; i < ledCount; i++)
		channel->SetLedColor(i, currentColor.Red, currentColor.Green, currentColor.Blue);
	channel->Show();
}

void FixedAnimation::onEnabled(uint8_t channelIndex) {

}

void FixedAnimation::onDisabled(uint8_t channelIndex) {

}

void FixedAnimation::onRequestReceived(uint8_t channelIndex, TypedStream &stream) {
	
}
