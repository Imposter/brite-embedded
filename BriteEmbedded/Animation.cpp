/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#include "Animation.h"
#include "Hash.h"
#include <string.h>

AnimationCore *Animation::getCore() const {
	return m_core;
}

uint32_t Animation::GetId() {
	const char *idStr = onIdRequested();
	return FNV1A32(reinterpret_cast<uint8_t *>(const_cast<char *>(idStr)), strlen(idStr));
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

void Animation::HandleRequest(uint8_t channel, TypedStream &stream) {
	onRequestReceived(channel, stream);
}