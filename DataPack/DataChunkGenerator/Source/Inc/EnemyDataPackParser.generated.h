#pragma once
#include <csv.hpp>

#include "EnemyDataPack.generated.h"
#include "DataPackUtils.h"

inline std::vector<EnemyDataPack> GenerateEnemyDataPacks(const std::string& filePath)
{
    csv::CSVReader reader(filePath);
    std::vector<EnemyDataPack> dataPacks;
    bool isTypeDefineRow = true;
    for (csv::CSVRow& row : reader)
    {
        if (isTypeDefineRow)
        {
            isTypeDefineRow = false;
            continue;
        }
        
        EnemyDataPack dataPack;
        dataPack.Num = row[0].get<int32_t>();
        dataPack.MoveSpeed = row[1].get<int32_t>();
        dataPack.RotationSpeed = row[2].get<int32_t>();
        dataPack.Color = DataPackUtils::ParseFloatArray(row[3].get<std::string>());

        dataPacks.push_back(dataPack);
    }
    return dataPacks;
}