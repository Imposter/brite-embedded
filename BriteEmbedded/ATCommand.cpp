#include "ATCommand.h"
#include <stdarg.h>

#ifndef min
#define min(a, b) a < b ? a : b
#endif

int16_t ATCommand::Execute(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize) {
	// Send command
	stream.Write(type);
	if (command != 0) {
		stream.Write('+');
		stream.Write(command);
	}
	stream.Write("\r\n");

	// Read response
	return stream.Read(responseBuffer, responseBufferSize);
}

int16_t ATCommand::Execute(TypedStream &stream, const char *type, const char *command) {
	// Send command
	stream.Write(type);
	if (command != 0) {
		stream.Write('+');
		stream.Write(command);
	}
	stream.Write("\r\n");

	// Ignore response
	char c = 0;
	int16_t response = 0;
	while (stream.Read(&c, sizeof c) > 0)
		response++;
	
	return response;
}

int16_t ATCommand::Read(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize, uint8_t parameters, ...) {
	// Start variable arguments
	va_list args;
	va_start(args, parameters);

	// Send command
	stream.Write(type);
	stream.Write('+');
	stream.Write(command);
	stream.Write('?');
	for (uint8_t i = 0; i < parameters; i++) {
		const char *arg = va_arg(args, const char *);
		stream.Write(arg);
		if (i < parameters - 1)
			stream.Write(',');
	}
	stream.Write("\r\n");

	// End variable arguments
	va_end(args);

	// Read response
	return stream.Read(responseBuffer, responseBufferSize);
}

int16_t ATCommand::Read(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize) {
	// Send command
	stream.Write(type);
	stream.Write('+');
	stream.Write(command);
	stream.Write("?\r\n");

	// Read response
	return stream.Read(responseBuffer, responseBufferSize);
}

int16_t ATCommand::Write(TypedStream &stream, const char *type, const char *command, char *responseBuffer, uint16_t responseBufferSize, uint8_t parameters, ...) {
	// Start variable arguments
	va_list args;
	va_start(args, parameters);

	// Send command
	stream.Write(type);
	stream.Write('+');
	stream.Write(command);
	stream.Write('=');
	for (uint8_t i = 0; i < parameters; i++) {
		const char *arg = va_arg(args, const char *);
		stream.Write(arg);
		if (i < parameters - 1)
			stream.Write(',');
	}
	stream.Write("\r\n");

	// End variable arguments
	va_end(args);

	// Read response
	return stream.Read(responseBuffer, responseBufferSize);
}

int16_t ATCommand::Write(TypedStream &stream, const char *type, const char *command, uint8_t parameters, ...) {
	// Start variable arguments
	va_list args;
	va_start(args, parameters);

	// Send command
	stream.Write(type);
	stream.Write('+');
	stream.Write(command);
	stream.Write('=');
	for (uint8_t i = 0; i < parameters; i++) {
		const char *arg = va_arg(args, const char *);
		stream.Write(arg);
		if (i < parameters - 1)
			stream.Write(',');
	}
	stream.Write("\r\n");

	// End variable arguments
	va_end(args);

	// Ignore response
	char c = 0;
	int16_t response = 0;
	while (stream.Read(&c, sizeof c) > 0)
		response++;
	
	return response;
}

int16_t ATCommand::GetParameter(const char *buffer, uint32_t bufferSize, char *parameterBuffer, uint16_t parameterBufferSize) {
	const char *paramStart = strstr(buffer, ":");
	if (paramStart != nullptr) {
		const char *paramEnd = strstr(++paramStart, "\r\n");
		int16_t toCopy = min(paramEnd - paramStart, parameterBufferSize);
		strncpy(parameterBuffer, paramStart, toCopy);

		return toCopy;
	}
	return -1;
}

// Untested
int16_t ATCommand::GetParameter(const char *buffer, uint32_t bufferSize, uint8_t parameter, char *parameterBuffer, uint16_t parameterBufferSize) {
	const char *paramStart = strstr(buffer, "=");
	if (paramStart != nullptr) {
		for (uint8_t i = 0; i < parameter; i++)
			paramStart = strstr(paramStart, ",");

		const char *paramEnd = strstr(++paramStart, ",");
		if (paramEnd == nullptr)
			paramEnd = strstr(paramStart, "\r\n");

		int16_t toCopy = min(paramEnd - paramStart, parameterBufferSize);
		strncpy(parameterBuffer, paramStart, toCopy);

		return toCopy;
	}
	return -1;
}
