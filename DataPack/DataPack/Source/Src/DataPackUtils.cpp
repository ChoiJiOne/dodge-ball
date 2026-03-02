#include <sstream>

#include "DataPackUtils.h"

bool DataPackUtils::ParseBool(const std::string& fieldValue)
{
	if (fieldValue == "TRUE")
		return true;

	if (fieldValue == "FALSE")
		return false;

	return false;
}

std::vector<int32_t> DataPackUtils::ParseIntArray(const std::string& fieldValue)
{
	std::vector<int32_t> result;
	std::stringstream ss(fieldValue);
	std::string item;
	while (std::getline(ss, item, ','))
	{
		if (!item.empty())
			result.push_back(std::stoi(item));
	}

	return result;
}

std::vector<float> DataPackUtils::ParseFloatArray(const std::string& fieldValue)
{
	std::vector<float> result;
	std::stringstream ss(fieldValue);
	std::string item;
	while (std::getline(ss, item, ','))
	{
		if (!item.empty())
			result.push_back(std::stof(item));
	}

	return result;
}

std::vector<std::string> DataPackUtils::ParseStringArray(const std::string& fieldValue)
{
	std::vector<std::string> result;
	std::stringstream ss(fieldValue);
	std::string item;
	while (std::getline(ss, item, ','))
	{
		if (!item.empty())
			result.push_back(item);
	}

	return result;
}

std::vector<bool> DataPackUtils::ParseBoolArray(const std::string& fieldValue)
{
	std::vector<bool> result;
	std::stringstream ss(fieldValue);
	std::string item;
	while (std::getline(ss, item, ','))
	{
		if (!item.empty())
			result.push_back(ParseBool(item));
	}

	return result;
}
