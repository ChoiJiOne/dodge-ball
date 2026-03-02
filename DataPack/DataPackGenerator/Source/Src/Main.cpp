#include <format>

#include "DataPackUtils.h"

#include "TestDataChunk.h"

#include "TestDataPackParser.generated.h"

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


void GenerateTestDataChunk(const std::string dataName)
{
	TestDataChunk testDataChunk;
	testDataChunk.DataPacks = GenerateTestDataPacks(std::format("Resource/{0}.csv", dataName));
	if (DataPackUtils::TrySaveDataChunk(testDataChunk))
	{
		std::cout << "Successed!";
		return;
	}
}

int main(int argc, char* argv[])
{
	GenerateTestDataChunk("Test");
	return 0;
}