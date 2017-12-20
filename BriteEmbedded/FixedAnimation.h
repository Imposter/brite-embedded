/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#pragma once

#include "Animation.h"

class FixedAnimation : public Animation {
protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	void onRequestReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~FixedAnimation() = default;
};