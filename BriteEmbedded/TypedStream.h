#pragma once

#include <Stream.h>
#include <stdint.h>

/**
* TypedStream
*
* \brief Typed stream to read and write data to a stream, preserving and
* comparing data types to ensure data maintains its integrity.
**/
class TypedStream {
	static const uint16_t DefaultReadTimeout;

	/**
	* Enumerations
	**/
	enum DataType : uint8_t {
		kDataType_Boolean,
		kDataType_Int8,
		kDataType_UInt8,
		kDataType_Int16,
		kDataType_UInt16,
		kDataType_Int32,
		kDataType_UInt32,
		kDataType_Float,
		kDataType_String,
		kDataType_Blob,
		kDataType_Max,
	};

	/**
	* Private variables
	*/
	Stream *m_stream;
	uint16_t m_timeout;
	bool m_typesEnabled;

public:

	/**
	* Peek
	*
	* \brief Allows reading a byte ahead without increasing the position in the
	* underlying stream.
	*
	* \return Read byte
	**/
	int16_t Peek() const;

	/**
	* Read
	*
	* \brief Reads a byte from the underlying stream.
	*
	* \return Read byte
	**/
	int16_t Read() const;

	/**
	* Read
	*
	* \brief Reads specified amount of bytes from the underlying stream.
	*
	* \param buffer Buffer to read to
	* \param size Amount of bytes to read
	* \return Bytes read
	**/
	int16_t Read(char *buffer, uint16_t size) const;

	/**
	* Write
	*
	* \brief Writes specified amount of bytes to the underlying stream.
	*
	* \param buffer Buffer to write
	* \param size Amount of bytes to write
	* \return Bytes written
	**/
	int16_t Write(const char *buffer, uint16_t size) const;

	/**
	* Write
	*
	* \brief Writes specified amount of bytes to the underlying stream.
	*
	* \param string String to write
	* \return Bytes written
	**/
	int16_t Write(const char *string) const;

	/**
	* Read
	*
	* \brief Reads an object from the underlying stream. Endian order must match 
	* that of the host.
	*
	* \tparam TData Object type
	* \param obj Object to read to
	* \return Read successful or not
	**/
	template<typename TData>
	bool Read(TData &obj) {
		return Read((char *)&obj, sizeof(TData)) == sizeof(TData);
	}

	/**
	* Write
	*
	* \brief Writes an object to the underlying stream. Endian order must match 
	* that of the host.
	*
	* \tparam TData Object type
	* \param obj Object to write
	* \return Write successful or not
	**/
	template<typename TData>
	bool Write(const TData &obj) {
		return Write((const char *)&obj, sizeof(TData)) == sizeof(TData);
	}

private:

	/**
	* readDataType
	*
	* \brief If data types are enabled, this will peek a byte to compare if the
	* data type matches, and if it does then the byte is read, otherwise it is 
	* discarded so the stream integrity is maintained.
	*
	* \param type Data type to compare
	* \return Data type matches or not
	**/
	bool readDataType(DataType type) const;

	/**
	* writeDataType
	*
	* \brief Peeks a byte to compare if the data type matches, and if it does
	* the byte is read, otherwise it is discarded so the stream integrity is 
	* maintained.
	*
	* \param type Data type to compare
	* \return Data type matches or not
	**/
	void writeDataType(DataType type);

public:
	
	/**
	* TypedStream
	*
	* \brief Initializes with specified typed stream
	* 
	* \param stream Underlying data stream
	* \param timeout Read timeout in milliseconds
	**/
	TypedStream(Stream *stream, uint16_t timeout = DefaultReadTimeout);
	
	/**
	* GetStream
	* 
	* \brief Used to obtain underlying data stream
	* 
	* \return Reference to stream
	**/
	Stream &GetStream() const;

	/**
	* Available
	* 
	* \brief Returns how many bytes are available to read
	* 
	* \return Bytes available to read
	**/
	uint16_t Available() const;

	/**
	* IsDataTypeEnabled
	* 
	* \brief Returns whether data types are enabled or not
	* 
	* \return Data types enabled or disabled
	**/
	bool IsDataTypeEnabled() const;

	/**
	* SetDataTypeEnabled
	* 
	* \brief If set to true, the typed stream will first check if the stream
	* contains the type information for the data being read, otherwise the 
	* stream will try to be read without any guarantee the data is actually 
	* valid.
	* 
	* \param enabled Data types enabled or disabled
	**/
	void SetDataTypeEnabled(bool enabled);

	/**
	* GetTimeout
	*
	* \brief Returns the read timeout
	*
	* \return Read timeout in milliseconds (ms)
	**/
	const uint16_t &GetTimeout() const;

	/**
	* SetTimeout
	*
	* \brief Sets read timeout
	*
	* \param timeout Read timeout in milliseconds (ms)
	**/
	void SetTimeout(uint16_t timeout);

	/**
	* ReadBoolean
	* 
	* \brief Read a boolean from the stream
	* 
	* \param obj Output boolean
	* \returns Whether the boolean was read successfully or not
	**/
	bool ReadBoolean(bool &obj);

	/**
	* ReadInt8
	*
	* \brief Read an 8-bit integer from the stream
	*
	* \param obj Output 8-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadInt8(int8_t &obj);

	/**
	* ReadUInt8
	*
	* \brief Read an unsigned 8-bit integer from the stream
	*
	* \param obj Output unsigned 8-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadUInt8(uint8_t &obj);

	/**
	* ReadInt16
	*
	* \brief Read an 16-bit integer from the stream
	*
	* \param obj Output 16-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadInt16(int16_t &obj);

	/**
	* ReadUInt16
	*
	* \brief Read an unsigned 16-bit integer from the stream
	*
	* \param obj Output unsigned 16-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadUInt16(uint16_t &obj);

	/**
	* ReadInt32
	*
	* \brief Read an 32-bit integer from the stream
	*
	* \param obj Output 32-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadInt32(int32_t &obj);

	/**
	* ReadUInt32
	*
	* \brief Read an unsigned 32-bit integer from the stream
	*
	* \param obj Output unsigned 32-bit integer
	* \returns Whether the integer was read successfully or not
	**/
	bool ReadUInt32(uint32_t &obj);

	/**
	* ReadFloat
	*
	* \brief Read a 32-bit float from the stream
	*
	* \param obj Output 32-bit float
	* \returns Whether the float was read successfully or not
	**/
	bool ReadFloat(float &obj);

	/**
	* ReadString
	*
	* \brief Reads a string from the stream, with a 32-bit length
	*
	* \param obj Output string buffer
	* \param size Maximum string length
	* \returns Whether the string was read successfully or not
	**/
	bool ReadString(char *obj, uint32_t size);

	/**
	* ReadBlob
	*
	* \brief Reads a binary blob from the stream, with a 32-bit length
	*
	* \param obj Output buffer
	* \param size Maximum buffer length
	* \returns Whether the blob was read successfully or not
	**/
	bool ReadBlob(uint8_t *obj, uint32_t size);

	/**
	* WriteBoolean
	*
	* \brief Writes a boolean to the stream
	*
	* \param obj Boolean to write
	**/
	void WriteBoolean(bool obj);

	/**
	* WriteInt8
	*
	* \brief Writes a 8-bit integer to the stream
	*
	* \param obj 8-bit integer to write
	**/
	void WriteInt8(int8_t obj);

	/**
	* WriteUInt8
	*
	* \brief Writes an unsigned 8-bit integer to the stream
	*
	* \param obj Unsigned 8-bit integer to write
	**/
	void WriteUInt8(uint8_t obj);

	/**
	* WriteInt16
	*
	* \brief Writes a 16-bit integer to the stream
	*
	* \param obj 16-bit integer to write
	**/
	void WriteInt16(int16_t obj);

	/**
	* WriteUInt16
	*
	* \brief Writes an unsigned 16-bit integer to the stream
	*
	* \param obj Unsigned 16-bit integer to write
	**/
	void WriteUInt16(uint16_t obj);

	/**
	* WriteInt32
	*
	* \brief Writes a 32-bit integer to the stream
	*
	* \param obj 32-bit integer to write
	**/
	void WriteInt32(int32_t obj);

	/**
	* WriteUInt32
	*
	* \brief Writes an unsigned 32-bit integer to the stream
	*
	* \param obj Unsigned 32-bit integer to write
	**/
	void WriteUInt32(uint32_t obj);

	/**
	* WriteFloat
	*
	* \brief Writes a 32-bit float to the stream
	*
	* \param obj 32-bit float to write
	**/
	void WriteFloat(float obj);

	/**
	* WriteString
	*
	* \brief Writes a string to the stream
	*
	* \param obj String buffer
	* \param size Size of string buffer
	**/
	void WriteString(const char *obj, uint32_t size);

	/**
	* WriteBlob
	*
	* \brief Writes a binary blob to the stream
	*
	* \param obj Blob buffer
	* \param size Size of buffer
	**/
	void WriteBlob(uint8_t *obj, uint32_t size);
};