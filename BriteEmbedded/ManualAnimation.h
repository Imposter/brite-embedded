/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#pragma once

#include "Animation.h"

class ManualAnimation : public Animation {
	enum Command : uint8_t {
		kCommand_SetColor,
		kCommand_SetColorRange,
		kCommand_SetColors,
		kCommand_SetColorsRange,

		kCommand_Max
	};

protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	void onRequestReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~ManualAnimation() = default;
};