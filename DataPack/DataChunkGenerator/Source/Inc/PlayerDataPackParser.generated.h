#pragma once
#include <csv.hpp>

#include "PlayerDataPack.generated.h"
#include "DataPackUtils.h"

inline std::vector<PlayerDataPack> GeneratePlayerDataPacks(const std::string& filePath)
{
    csv::CSVReader reader(filePath);
    std::vector<PlayerDataPack> dataPacks;
    bool isTypeDefineRow = true;
    for (csv::CSVRow& row : reader)
    {
        if (isTypeDefineRow)
        {
            isTypeDefineRow = false;
            continue;
        }
        
        PlayerDataPack dataPack;
        dataPack.Level = row[0].get<int32_t>();
        dataPack.Speed = row[1].get<int32_t>();
        dataPack.LevelUpTime = row[2].get<int32_t>();
        dataPack.EnemySpawnTime = row[3].get<float>();

        dataPacks.push_back(dataPack);
    }
    return dataPacks;
}