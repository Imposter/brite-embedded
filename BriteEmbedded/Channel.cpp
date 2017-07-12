#include "Channel.h"

Channel::Channel()
	: m_pixels(0, 0, DefaultType), m_ledCount(0) {}

Channel::Channel(uint8_t pin, uint16_t maxSize, uint8_t type)
	: m_pixels(maxSize, pin, type), m_ledCount(0) {}

void Channel::Initialize() {
	m_pixels.begin();
	m_pixels.show();
}

void Channel::SetPin(uint8_t pin) {
	m_pixels.setPin(pin);
}

void Channel::SetMaxLedCount(uint16_t count) {
	if (m_ledCount > 0)
		return;

	m_ledCount = 0;
	m_pixels.updateLength(count);
}

void Channel::SetBrightness(uint8_t limit) {
	m_pixels.setBrightness(limit);
}

const uint16_t &Channel::GetLedCount() const {
	return m_ledCount;
}

void Channel::SetLedCount(uint16_t ledCount) {
	m_ledCount = ledCount;
}

Color Channel::GetLedColor(uint16_t i) const {
	return reinterpret_cast<Color *>(m_pixels.getPixels())[i];
}

void Channel::SetLedColor(uint16_t i, uint32_t color) {
	if (i >= m_ledCount)
		return;

	m_pixels.setPixelColor(i, color);
}

void Channel::SetLedColor(uint16_t i, Color color) {
	if (i >= m_ledCount)
		return;

	m_pixels.setPixelColor(i, color.Red, color.Green, color.Blue);
}

void Channel::SetLedColor(uint16_t i, uint8_t r, uint8_t g, uint8_t b) {
	if (i >= m_ledCount)
		return;

	m_pixels.setPixelColor(i, r, g, b);
}

void Channel::Show() {
	m_pixels.show();
}

void Channel::Clear() {
	for (uint16_t i = 0; i < m_ledCount; i++)
		m_pixels.setPixelColor(i, 0);
	m_pixels.show();
}