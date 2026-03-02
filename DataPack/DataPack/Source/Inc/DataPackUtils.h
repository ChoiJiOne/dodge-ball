#pragma once

#include <string>
#include <vector>

class DataPackUtils
{
public:
	static bool ParseBool(const std::string& fieldValue);
	
	static std::vector<int32_t> ParseIntArray(const std::string& fieldValue);
	static std::vector<float> ParseFloatArray(const std::string& fieldValue);
	static std::vector<std::string> ParseStringArray(const std::string& fieldValue);
	static std::vector<bool> ParseBoolArray(const std::string& fieldValue);
};