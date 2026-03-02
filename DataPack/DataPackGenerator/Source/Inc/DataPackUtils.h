#pragma once

#include <string>
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
	static bool TrySaveDataChunk(TDataChunk dataChunk)
	{
		msgpack::sbuffer sbuffer;
		msgpack::pack(sbuffer, dataChunk);

		std::string dataFilePath = "Resource/TestData.bytes";
		std::ofstream ofs(dataFilePath, std::ios::binary);

		if (ofs.is_open())
		{
			ofs.write(sbuffer.data(), sbuffer.size());
			ofs.close();
			return true;
		}

		return false;
	}
};