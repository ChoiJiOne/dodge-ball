#pragma once

#include <string>

enum class EErrorCode
{
	SUCCESS = 0x0000, // 성공

	INVALID_ARGUMENT = 0x0001,
	NOT_INITIALIZED = 0x0002,
	ALREADY_INITIALIZED = 0x0003,
	FAILED_TO_INITIALIZE = 0x0004,
	FAILED_TO_STARTUP = 0x0005,
	NOT_IMPLEMENTED = 0x0010, // 미구현

	// Config
	FAILED_TO_LOAD_CONFIG = 0x1001,
	FAILED_TO_PARSE_CONFIG = 0x1002,
	NOT_FOUND_CONFIG = 0x1003,
	ALREADY_LOAD_CONFIG = 0x1004,
	
	// Data Chunk
	FAILED_TO_LOAD_DATA_CHUNK = 0x2000,
	FAILED_TO_PARSE_DATA_CHUNK = 0x2001,
	UNKNOWN_DATA_CHUNK_ERROR = 0x2002,
	NOT_FOUND_DATA_CHUNK = 0x2003,
	ALREADY_LOAD_DATA_CHUNK = 0x2004,

	// Actor
	ALREADY_EXIST_ACTOR = 0x3001,

	INTERNAL_ERROR = 0xFFFF, // 정의되지 않은 에러
};

class Error
{
public:
	explicit Error() = default;
	explicit Error(
		const EErrorCode& code,
		const std::string& message,
		const char* file,
		int32_t line,
		const char* func)
		: _code(code)
		, _message(message)
		, _file(file)
		, _line(line)
		, _function(func) {}

	Error(Error&& instance) noexcept
		: _code(instance._code)
		, _message(instance._message)
		, _file(instance._file)
		, _line(instance._line)
		, _function(instance._function) {}

	Error(const Error& instance) noexcept
		: _code(instance._code)
		, _message(instance._message)
		, _file(instance._file)
		, _line(instance._line)
		, _function(instance._function) {}

	Error& operator=(Error&& instance) noexcept
	{
		if (this == &instance)
		{
			return *this;
		}

		_code = instance._code;
		_message = instance._message;
		_file = instance._file;
		_line = instance._line;
		_function = instance._function;

		return *this;
	}

	Error& operator=(const Error& instance) noexcept
	{
		if (this == &instance)
		{
			return *this;
		}

		_code = instance._code;
		_message = instance._message;
		_file = instance._file;
		_line = instance._line;
		_function = instance._function;

		return *this;
	}

	const EErrorCode& GetErrorCode() const { return _code; }
	const std::string& GetMessage() const { return _message; }
	const std::string& GetFile() const { return _file; }
	const int32_t& GetLine() const { return _line; }
	const std::string& GetFunction() const { return _function; }

private:
	EErrorCode _code = EErrorCode::SUCCESS;
	std::string _description;
	std::string _message;
	std::string _file;
	int32_t _line = 0;
	std::string _function;
};

#define MAKE_ERROR(code, message) Error((code), (message), __FILE__, __LINE__, __func__)