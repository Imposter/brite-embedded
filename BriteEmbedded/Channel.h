#pragma once

#include "Config.h"
#include "Color.h"
#include <Adafruit_NeoPixel.h>

class Channel {
	static const uint8_t DefaultType = NEO_GRB + NEO_KHZ800;
  
	Adafruit_NeoPixel m_pixels;
	uint16_t m_ledCount;

public:
	Channel();
	Channel(uint8_t pin, uint16_t maxSize, uint8_t type = DefaultType);

	void Initialize();

	void SetPin(uint8_t pin);
	void SetMaxLedCount(uint16_t count);
	void SetBrightness(uint8_t limit);

	const uint16_t &GetLedCount() const;
	void SetLedCount(uint16_t ledCount);

	Color GetLedColor(uint16_t i) const;
	void SetLedColor(uint16_t i, uint32_t color);
	void SetLedColor(uint16_t i, Color color);
	void SetLedColor(uint16_t i, uint8_t r, uint8_t g, uint8_t b);

	void Show();

	void Clear();
};
