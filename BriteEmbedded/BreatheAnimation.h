#pragma once

#include "Config.h"
#include "Animation.h"

class BreatheAnimation : public Animation {
	static const float AlphaUpdateRate;

	bool m_increasing[CHANNEL_COUNT]{ true };
	float m_alpha[CHANNEL_COUNT]{ 0.0f };

protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~BreatheAnimation() = default;
};