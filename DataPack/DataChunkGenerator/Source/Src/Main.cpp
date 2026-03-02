#include <format>
#include <iostream>

#include "DataPackUtils.h"

#include "TestDataChunk.h"

#include "TestDataPackParser.generated.h"

void GenerateTestDataChunk(const std::string dataName)
{
	TestDataChunk testDataChunk;
	testDataChunk.DataPacks = GenerateTestDataPacks(std::format("Resource/{0}.csv", dataName));
	if (DataPackUtils::TrySaveDataChunk(std::format("Resource/{0}.bytes", dataName), testDataChunk))
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