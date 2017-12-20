/*
 * Copyright (C) 2017 Eyaz Rehman. All Rights Reserved.
 *
 * This file is part of Brite.
 * Licensed under the GNU General Public License. See LICENSE file in the project
 * root for full license information.
 */

#pragma once

#include <stdint.h>

constexpr uint32_t FNV1APrime32 = 0x01000193;
constexpr uint32_t FNV1AOffset32 = 0x811C9DC5;

uint32_t FNV1A32(uint8_t *obj, uint32_t size, uint32_t prime = FNV1APrime32, uint32_t offset = FNV1AOffset32);
constexpr uint32_t FNV1ACompileTime32(uint8_t *obj, uint32_t last = FNV1AOffset32);