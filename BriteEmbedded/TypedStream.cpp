#include "TypedStream.h"
#include <Arduino.h>
#include <string.h>
#include <limits.h>

const uint16_t TypedStream::DefaultReadTimeout = 100; // ms

int16_t TypedStream::Peek() {
	return m_stream->peek();
}

int16_t TypedStream::Read() {
	return m_stream->read();
}

int16_t TypedStream::Read(char *buffer, uint16_t size) {
	uint32_t initialTime = millis();
	uint32_t currentTime = initialTime;
	uint32_t deltaTime = 0;

	uint16_t origSize = size;

	do {
		// Read data
		int16_t b = Read();
		if (b != -1) {
			*(char *)(buffer++) = b;
			size--;
		}

		currentTime = millis();
		if (currentTime < initialTime)
			deltaTime = ULONG_MAX - initialTime + currentTime;
		else
			deltaTime = currentTime - initialTime;
	} while (deltaTime < m_timeout && size != 0);

	if (size == 0) {
		return origSize;
	} else if (size > 0 && size != origSize) {
		return origSize - size;
	} else {
		return -1;
	}
}

int16_t TypedStream::Write(const char *buffer, uint16_t size) {
	return m_stream->write(buffer, size);
}

int16_t TypedStream::Write(const char *string) {
	return Write(string, strlen(string));
}

bool TypedStream::readDataType(DataType type) {
	if (!m_typesEnabled)
		return true;

	uint32_t initialTime = millis();
	uint32_t currentTime = initialTime;
	uint32_t deltaTime = 0;

	do {
		if (m_stream->available() > 0) {
			int16_t b = m_stream->peek();
			if (b < 0 || b >= kDataType_Max)
				return false;

			b = m_stream->read();
			return b == type;
		}

		currentTime = millis();
		if (currentTime < initialTime)
			deltaTime = ULONG_MAX - initialTime + currentTime;
		else
			deltaTime = currentTime - initialTime;
	} while (deltaTime < m_timeout);

	return false;
}

void TypedStream::writeDataType(DataType type) {
	if (m_typesEnabled)
		Write<uint8_t>(type);
}

TypedStream::TypedStream(Stream *stream, uint16_t timeout)
	: m_stream(stream), m_timeout(timeout), m_typesEnabled(true) {}

Stream &TypedStream::GetStream() const {
	return *m_stream;
}

uint16_t TypedStream::Available() {
	return m_stream->available();
}

bool TypedStream::IsDataTypeEnabled() const {
	return m_typesEnabled;
}

void TypedStream::SetDataTypeEnabled(bool enabled) {
	m_typesEnabled = enabled;
}

const uint16_t &TypedStream::GetTimeout() const {
	return m_timeout;
}

void TypedStream::SetTimeout(uint16_t timeout) {
	m_timeout = timeout;
}

bool TypedStream::ReadBoolean(bool &obj) {
	if (!readDataType(kDataType_Boolean))
		return false;

	uint8_t b;
	if (!Read<uint8_t>(b))
		return false;

	obj = b == 1;

	return true;
}

bool TypedStream::ReadInt8(int8_t &obj) {
	if (!readDataType(kDataType_Int8))
		return false;

	return Read<int8_t>(obj);
}

bool TypedStream::ReadUInt8(uint8_t &obj) {
	if (!readDataType(kDataType_UInt8))
		return false;

	return Read<uint8_t>(obj);
}

bool TypedStream::ReadInt16(int16_t &obj) {
	if (!readDataType(kDataType_Int16))
		return false;

	return Read<int16_t>(obj);
}

bool TypedStream::ReadUInt16(uint16_t &obj) {
	if (!readDataType(kDataType_UInt16))
		return false;

	return Read<uint16_t>(obj);
}

bool TypedStream::ReadInt32(int32_t &obj) {
	if (!readDataType(kDataType_Int32))
		return false;

	return Read<int32_t>(obj);
}

bool TypedStream::ReadUInt32(uint32_t &obj) {
	if (!readDataType(kDataType_UInt32))
		return false;

	return Read<uint32_t>(obj);
}

bool TypedStream::ReadFloat(float &obj) {
	if (!readDataType(kDataType_Float))
		return false;

	return Read<float>(obj);
}

bool TypedStream::ReadString(char *obj, uint32_t size) {
	if (!readDataType(kDataType_String))
		return false;

	uint32_t length = 0;
	if (!Read<uint32_t>(length))
		return false;

	uint32_t toRead = min(size, length);
	if (!Read(obj, toRead))
		return false;

	// Read out the rest of the string to ensure that stream synchronization
	// is maintained
	while (toRead < length) {
		if (Read() == -1)
			return false;

		toRead++;
	}

	return true;
}

bool TypedStream::ReadBlob(uint8_t *obj, uint32_t size) {
	if (!readDataType(kDataType_Blob))
		return false;

	uint32_t length = 0;
	if (!Read<uint32_t>(length))
		return false;

	uint32_t toRead = min(size, length);
	if (!Read((char *)obj, toRead))
		return false;

	// Read out the rest of the blob to ensure that stream synchronization
	// is maintained
	while (toRead < length) {
		if (Read() == -1)
			return false;

		toRead++;
	}

	return true;
}

void TypedStream::WriteBoolean(bool obj) {
	writeDataType(kDataType_Boolean);
	Write<uint8_t>(obj ? 1 : 0);
}

void TypedStream::WriteInt8(int8_t obj) {
	writeDataType(kDataType_Int8);
	Write<int8_t>(obj);
}

void TypedStream::WriteUInt8(uint8_t obj) {
	writeDataType(kDataType_UInt8);
	Write<uint8_t>(obj);
}

void TypedStream::WriteInt16(int16_t obj) {
	writeDataType(kDataType_Int16);
	Write<int16_t>(obj);
}

void TypedStream::WriteUInt16(uint16_t obj) {
	writeDataType(kDataType_UInt16);
	Write<uint16_t>(obj);
}

void TypedStream::WriteInt32(int32_t obj) {
	writeDataType(kDataType_Int32);
	Write<int32_t>(obj);
}

void TypedStream::WriteUInt32(uint32_t obj) {
	writeDataType(kDataType_UInt32);
	Write<uint32_t>(obj);
}

void TypedStream::WriteFloat(float obj) {
	writeDataType(kDataType_Float);
	Write<float>(obj);
}

void TypedStream::WriteString(const char *obj, uint32_t size) {
	writeDataType(kDataType_String);
	Write<uint32_t>(size);
	Write(obj, size);
}

void TypedStream::WriteBlob(uint8_t *obj, uint32_t size) {
	writeDataType(kDataType_Blob);
	Write<uint32_t>(size);
	Write((const char *)obj, size);
}
