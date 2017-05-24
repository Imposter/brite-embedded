#pragma once

#include "Config.h"
#include "Animation.h"

class PulseAnimation : public Animation {
	static const float AlphaUpdateRate;

	float m_alpha[CHANNEL_COUNT]{ 0.0f };

protected:
	const char *onIdRequested() override;
	void onAnimate(uint8_t channelIndex) override;
	void onEnabled(uint8_t channelIndex) override;
	void onDisabled(uint8_t channelIndex) override;
	bool onDataReceived(uint8_t channelIndex, TypedStream &stream) override;

public:
	virtual ~PulseAnimation() = default;
};