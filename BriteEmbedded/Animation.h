#pragma once

#include "TypedStream.h"
#include <string.h>
#include <limits.h>

class AnimationCore;

class Animation {
	AnimationCore *m_core;

protected:
	AnimationCore *getCore() const;

	virtual const char *onIdRequested() = 0;
	virtual void onAnimate(uint8_t channelIndex) = 0;
	virtual void onEnabled(uint8_t channelIndex) = 0;
	virtual void onDisabled(uint8_t channelIndex) = 0;
	virtual bool onDataReceived(uint8_t channelIndex, TypedStream &stream) = 0;

public:
	uint32_t GetId();

	void Initialize(AnimationCore *core);
  
	void Animate(uint8_t channel);

	void SetEnabled(uint8_t channel, bool enabled);

	bool HandleStream(uint8_t channel, TypedStream &stream);
};