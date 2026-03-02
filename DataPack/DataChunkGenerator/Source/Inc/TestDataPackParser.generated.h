#pragma once
#include <csv.hpp>

#include "TestDataPack.generated.h"
#include "DataPackUtils.h"

inline std::vector<TestDataPack> GenerateTestDataPacks(const std::string& filePath)
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
        dataPack.A = row[0].get<int32_t>();
        dataPack.B = row[1].get<float>();
        dataPack.C = row[2].get<std::string>();
        dataPack.D = DataPackUtils::ParseBool(row[3].get<std::string>());
        dataPack.E = DataPackUtils::ParseIntArray(row[4].get<std::string>());

        dataPacks.push_back(dataPack);
    }
    return dataPacks;
}