#pragma once

#include "Config.h"
#include "Animation.h"

class FixedAnimation : public Animation {
protected:
	virtual const char *onIdRequested() override;
	virtual void onAnimate(uint8_t channelIndex) override;
	virtual void onEnabled(uint8_t channelIndex) override;
	virtual void onDisabled(uint8_t channelIndex) override;
	virtual bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;
};