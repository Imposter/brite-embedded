#pragma once

#include "TypedStream.h"

class ATCommand {
public:
	static int16_t Execute(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize);
	static int16_t Execute(TypedStream &stream, const char *type, const char *command);

	static int16_t Read(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize, uint8_t parameters, ...);
	static int16_t Read(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize);

	static int16_t Write(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize, uint8_t parameters, ...);
	static int16_t Write(TypedStream &stream, const char *type, const char *command, uint8_t parameters, ...);

	static int16_t GetParameter(const char *buffer, uint32_t bufferSize, char *parameterBuffer, uint16_t parameterBufferSize);
	static int16_t GetParameter(const char *buffer, uint32_t bufferSize, uint8_t parameter, char *parameterBuffer, uint16_t parameterBufferSize);
};