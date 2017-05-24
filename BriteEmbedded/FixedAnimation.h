#pragma once

#include "Animation.h"

class FixedAnimation : public Animation {
protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~FixedAnimation() = default;
};