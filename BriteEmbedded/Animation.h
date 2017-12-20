/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#pragma once

#include "TypedStream.h"

class AnimationCore;

class Animation {
	AnimationCore *m_core{ 0 };

protected:
	~Animation() = default;
	AnimationCore *getCore() const;

	virtual const char *onIdRequested() = 0;
	virtual void onAnimate(uint8_t channelIndex) = 0;
	virtual void onEnabled(uint8_t channelIndex) = 0;
	virtual void onDisabled(uint8_t channelIndex) = 0;
	virtual void onRequestReceived(uint8_t channelIndex, TypedStream &stream) = 0;

public:
	uint32_t GetId();
	void Initialize(AnimationCore *core);  
	void Animate(uint8_t channel);
	void SetEnabled(uint8_t channel, bool enabled);

	void HandleRequest(uint8_t channel, TypedStream &stream);
};