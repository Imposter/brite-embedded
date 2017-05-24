#include "Animation.h"
#include "Hash.h"
#include <string.h>

AnimationCore *Animation::getCore() const {
	return m_core;
}

uint32_t Animation::GetId() {
	const char *idStr = onIdRequested();
	return FNV1A32((uint8_t *)idStr, strlen(idStr));
}

void Animation::Initialize(AnimationCore *core) {
	m_core = core;
}

void Animation::Animate(uint8_t channel) {
	onAnimate(channel);
}

void Animation::SetEnabled(uint8_t channel, bool enabled) {
	if (enabled) {
		onEnabled(channel);
	} else {
		onDisabled(channel);
	}
}

bool Animation::HandleStream(uint8_t channel, TypedStream &stream) {
	return onDataReceived(channel, stream);
}
