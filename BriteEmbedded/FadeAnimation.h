#pragma once

#include "Config.h"
#include "Animation.h"

class FadeAnimation : public Animation {
	static const float AmountUpdateRate;

	float m_amount[CHANNEL_COUNT]{ 0.0f };

protected:
	virtual const char *onIdRequested() override;
	virtual void onAnimate(uint8_t channelIndex) override;
	virtual void onEnabled(uint8_t channelIndex) override;
	virtual void onDisabled(uint8_t channelIndex) override;
	virtual bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;
};