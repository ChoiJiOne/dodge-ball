#include <algorithm>
#include <string>
#include <sstream>
#include <functional>

#include "DataPackUtils.h"

bool DataPackUtils::ParseBool(const std::string& fieldValue)
{
	std::string boolValue = fieldValue;
	std::transform(fieldValue.begin(), fieldValue.end(), boolValue.begin(), [](uint8_t c) { return std::tolower(c); });

	if (fieldValue == STRING_VALUE_TRUE)
		return true;

	if (fieldValue == STRING_VALUE_FALSE)
		return false;

	std::cout << "Invalid bool type value: " << fieldValue << std::endl;
	return false;
}

template <typename T>
std::vector<T> ParseArray(const std::string& fieldValue, std::function<T(const std::string&)> converter)
{
	std::vector<T> result;
	std::stringstream ss(fieldValue);
	std::string item;
	while (std::getline(ss, item, ','))
	{
		if (!item.empty())
			result.push_back(converter(item));
	}

	return result;
}

std::vector<int32_t> DataPackUtils::ParseIntArray(const std::string& fieldValue)
{
	return ParseArray<int32_t>(fieldValue, [](const std::string& s) { return std::stoi(s); });
}

std::vector<float> DataPackUtils::ParseFloatArray(const std::string& fieldValue)
{
	return ParseArray<float>(fieldValue, [](const std::string& s) { return std::stof(s); });
}

std::vector<std::string> DataPackUtils::ParseStringArray(const std::string& fieldValue)
{
	return ParseArray<std::string>(fieldValue, [](const std::string& s) { return s; });
}

std::vector<bool> DataPackUtils::ParseBoolArray(const std::string& fieldValue)
{
	return ParseArray<bool>(fieldValue, ParseBool);
}
