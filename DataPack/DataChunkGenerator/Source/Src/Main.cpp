#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include "Macro.h"

#include "DataChunkUtils.h"
#include "DataPackUtils.h"

#include "EnemyDataChunk.h"
#include "PlayerDataChunk.h"

#include "EnemyDataPackParser.generated.h"
#include "PlayerDataPackParser.generated.h"

void GenerateEnemyDataChunk()
{
	std::string dataName = "Enemy";
	std::cout << std::format("[{0}] Generating data chunk...", dataName) << std::endl;

	EnemyDataChunk dataChunk;
	dataChunk.DataPacks = GenerateEnemyDataPacks(std::format("CSV/{0}.csv", dataName));
	
	for (std::size_t idx = 0; idx < dataChunk.DataPacks.size(); ++idx)
	{
		const EnemyDataPack& dataPack = dataChunk.DataPacks[idx];
		dataChunk.NumToIdx.emplace(dataPack.Num, idx);
	}
	
	if (!DataChunkUtils::TrySaveDataChunk(std::format("Resource/{0}.bytes", dataName), dataChunk))
	{
		std::cout << std::format("[{0}] Error: Failed to generate data chunk!", dataName) << std::endl;
		return;
	}

	std::cout << std::format("[{0}] Successfully generated data chunk.", dataName) << std::endl;
}

void GeneratePlayerDataChunk()
{
	std::string dataName = "Player";
	std::cout << std::format("[{0}] Generating data chunk...", dataName) << std::endl;

	PlayerDataChunk dataChunk;
	dataChunk.DataPacks = GeneratePlayerDataPacks(std::format("CSV/{0}.csv", dataName));

	if (!DataChunkUtils::TrySaveDataChunk(std::format("Resource/{0}.bytes", dataName), dataChunk))
	{
		std::cout << std::format("[{0}] Error: Failed to generate data chunk!", dataName) << std::endl;
		return;
	}

	std::cout << std::format("[{0}] Successfully generated data chunk.", dataName) << std::endl;
}

class DataChunkGenerator
{
public:
	DataChunkGenerator() = default;
	virtual ~DataChunkGenerator() = default;
	
	void Startup(int32_t argc, char* argv[])
	{
		_generatorMap =
		{
			{ "Enemy", GenerateEnemyDataChunk },
			{ "Player", GeneratePlayerDataChunk },
			// NOTE: 여기 아래에 계속해서 추가.
		};

		if (argc == COMMAND_TARGET_ALL_PARAM_COUNT)
		{
			const std::string_view command = argv[1];
			if (command != COMMAND_TARGET_ALL)
			{
				std::cout << "Invalid DataChunkGenerator command line argument: " << command << std::endl;
				return;
			}
		}
		else if (argc == COMMAND_TARGET_PARAM_COUNT)
		{
			const std::string_view command = argv[1];
			if (command != COMMAND_TARGET)
			{
				std::cout << "Invalid DataChunkGenerator command line argument: " << command << std::endl;
				return;
			}

			const std::string_view param = argv[2];
			const auto& iter = _generatorMap.find(param);
			if (iter == _generatorMap.end())
			{
				std::cout << "Invalid DataChunkGenerator command line param: " << param << std::endl;
				return;
			}

			_targetGenerator = iter->second;
		}
		else
		{
			std::cout << "Invalid DataChunkGenerator command line argument count: " << argc << std::endl;
			return;
		}

		_isInitialized = true;
	}

	void Run()
	{
		if (!_isInitialized)
		{
			std::cout << "Failed to Startup DataChunkGenerator." << std::endl;
			return;
		}

		std::cout << "--- Start Data Chunk Generation ---" << std::endl;

		if (_targetGenerator != nullptr)
		{
			_targetGenerator();
		}
		else
		{
			for (const auto& dataChunkGenerator : _generatorMap)
			{
				if (dataChunkGenerator.second != nullptr)
				{
					dataChunkGenerator.second();
				}
			}
		}

		std::cout << "--- Data Chunk Generation Finished ---" << std::endl;
	}

private:
	static constexpr std::string_view COMMAND_TARGET_ALL = "--target-all";
	static constexpr std::string_view COMMAND_TARGET = "--target";
	static constexpr int32_t COMMAND_TARGET_ALL_PARAM_COUNT = 2;
	static constexpr int32_t COMMAND_TARGET_PARAM_COUNT = 3;

private:
	bool _isInitialized = false;
	
	std::function<void()> _targetGenerator = nullptr;
	std::map<std::string_view, std::function<void()>> _generatorMap;
};

int main(int32_t argc, char* argv[])
{
	std::unique_ptr<DataChunkGenerator> dataChunkGenerator = std::make_unique<DataChunkGenerator>();
	dataChunkGenerator->Startup(argc, argv);
	dataChunkGenerator->Run();
	return 0;
}