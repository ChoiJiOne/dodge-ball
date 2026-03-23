#pragma once

#include "IDataChunk.h"

#include "EnemyDataPack.generated.h"

struct EnemyDataChunk : public IDataChunk
{
	std::vector<EnemyDataPack> DataPacks;
	std::map<int32_t, int32_t> NumToIdx;

	MSGPACK_DEFINE(DataPacks, NumToIdx);
};