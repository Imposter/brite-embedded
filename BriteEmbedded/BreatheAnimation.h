#pragma once

#include "Animation.h"

class BreatheAnimation : public Animation {
	static const float AlphaUpdateRate;

	bool m_increasing[CHANNEL_COUNT]{ true };
	float m_alpha[CHANNEL_COUNT]{ 0.0f };

protected:
	virtual const char *onIdRequested() override;
	virtual void onAnimate(uint8_t channelIndex) override;
	virtual void onEnabled(uint8_t channelIndex) override;
	virtual void onDisabled(uint8_t channelIndex) override;
	virtual bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;
};