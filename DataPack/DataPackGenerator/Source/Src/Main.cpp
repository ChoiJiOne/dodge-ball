#include <fstream>

#include <csv.hpp>

#include "TestDataChunk.h"

#include "DataPackUtils.h"

std::vector<TestDataPack> GenerateTestDataPacks(const std::string& filePath)
{
	csv::CSVReader reader(filePath);

	std::vector<TestDataPack> dataPacks;
	bool isTypeDefineRow = true;

	for (csv::CSVRow& row : reader)
	{
		if (isTypeDefineRow)
		{
			isTypeDefineRow = false;
			continue;
		}

		TestDataPack dataPack;

		// 일반 숫자와 문자열은 get<T>()로 바로 가져올 수 있습니다.
		dataPack.A = row[0].get<int32_t>();
		dataPack.B = row[1].get<float>();
		dataPack.C = row[2].get<std::string>();

		// bool 타입과 배열 타입은 직접 문자열로 가져온 후 파싱해야 합니다.
		dataPack.D = DataPackUtils::ParseBool(row[3].get<std::string>());
		dataPack.E = DataPackUtils::ParseIntArray(row[4].get<std::string>());

		dataPacks.push_back(dataPack);
	}

	return dataPacks;
}

int main(int argc, char* argv[])
{
	TestDataChunk testDataChunk;

	// CSV 파일이 위치한 경로를 명확하게 지정하세요.
	testDataChunk.DataPacks = GenerateTestDataPacks("Resource/Test.csv");

	// 1. msgpack 패커(sbuffer) 채우기: testDataChunk를 직렬화 합니다.
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, testDataChunk);
	// 2. 파일로 저장하기 (바이너리 모드)
	std::string outputFilePath = "Resource/TestData.bytes";
	std::ofstream ofs(outputFilePath, std::ios::binary);

	if (ofs.is_open())
	{
		ofs.write(sbuf.data(), sbuf.size());
		ofs.close();
		std::cout << "성공적으로 바이너리 파일을 생성했습니다: " << outputFilePath << "\n";
	}
	else
	{
		std::cerr << "파일 쓰기 실패: " << outputFilePath << "\n";
	}

	return 0;
}