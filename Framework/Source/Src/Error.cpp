#include "Error.h"

Error::Error(const EErrorCode& code, const std::string& message, const char* file, int32_t line, const char* func)
	: _code(code)
	, _message(message)
	, _file(file)
	, _line(line)
	, _function(func)
{
	_description = GetErrorCodeMessage(code);
}

std::string Error::GetErrorCodeMessage(const EErrorCode& errorCode)
{
	std::string errorCodeMessage;

	switch (errorCode)
	{
	case EErrorCode::INVALID_ARGUMENT:
		errorCodeMessage = "One or more arguments are invalid.";
		break;

	case EErrorCode::NOT_INITIALIZED:
		errorCodeMessage = "Not initialized.";
		break;

	case EErrorCode::ALREADY_INITIALIZED:
		errorCodeMessage = "Initialization already completed.";
		break;

	case EErrorCode::FAILED_TO_INITIALIZE:
		errorCodeMessage = "Initialization failed.";
		break;

	case EErrorCode::NOT_IMPLEMENTED:
		errorCodeMessage = "Feature not implemented.";
		break;

	case EErrorCode::INTERNAL_ERROR:
		errorCodeMessage = "An unexpected internal error occurred.";
		break;

	default:
		errorCodeMessage = "Unknown error.";
		break;
	}

	return errorCodeMessage;
}