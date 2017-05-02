#include "Hash.h"

uint32_t FNV1A32(uint8_t *obj, uint32_t size, uint32_t prime, uint32_t offset) {
	for (uint32_t i = 0; i < size; i++) {
		offset *= prime;
		offset ^= obj[i];
	}

	return offset;
}

constexpr uint32_t FNV1ACompileTime32(uint8_t *obj, uint32_t last) {
	return *obj ? FNV1A32(obj + 1, (*obj ^ last) *FNV1APrime32) : last;
}
