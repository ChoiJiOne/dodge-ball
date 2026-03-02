#pragma once

#include <string>
#include <iostream>
#include <vector>

#include <msgpack.hpp>

class DataPackUtils
{
public:
	static bool ParseBool(const std::string& fieldValue);
	
	static std::vector<int32_t> ParseIntArray(const std::string& fieldValue);
	static std::vector<float> ParseFloatArray(const std::string& fieldValue);
	static std::vector<std::string> ParseStringArray(const std::string& fieldValue);
	static std::vector<bool> ParseBoolArray(const std::string& fieldValue);

	template <typename TDataChunk>
	static bool TrySaveDataChunk(const std::string& filePath, TDataChunk dataChunk)
	{
		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer, dataChunk);

		std::ofstream ofs(filePath, std::ios::binary);
		if (!ofs.is_open())
		{
			std::cout << "Failed to save data chunk file to path: '" << filePath << "'" << std::endl;
			return false;
		}
		
		ofs.write(sbuffer.data(), sbuffer.size());
		ofs.close();        
		
		std::cout << "Successfully saved data chunk file to path: '" << filePath << "'" << std::endl;
		return true;
	}

private:
	static constexpr std::string_view STRING_VALUE_TRUE = "true";
	static constexpr std::string_view STRING_VALUE_FALSE = "false";
};