#pragma once

#include "IDataChunk.h"

#include "BallDataPack.generated.h"

struct BallDataChunk : public IDataChunk
{
	std::vector<BallDataPack> DataPacks;
	std::map<int32_t, int32_t> NumToIdx;

	MSGPACK_DEFINE(DataPacks, NumToIdx);
};