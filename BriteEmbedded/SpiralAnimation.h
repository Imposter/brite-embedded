/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#pragma once

#include "Config.h"
#include "Animation.h"

class SpiralAnimation : public Animation {
	enum Command : uint8_t {
		kCommand_SetForwardEnabled,
		kCommand_SetGroupSize,

		kCommand_Max
	};

	uint16_t m_position[CHANNEL_COUNT]{ 0 };
	uint8_t m_groupSize[CHANNEL_COUNT]{ 0 };
	bool m_forward[CHANNEL_COUNT]{ true };

protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	void onRequestReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~SpiralAnimation() = default;
};