#pragma once

#include "IDataChunk.h"

#include "PlayerDataPack.generated.h"

struct PlayerDataChunk : public IDataChunk
{
	std::vector<PlayerDataPack> DataPacks;
	std::map<int32_t, int32_t> LevelToIdx;

	MSGPACK_DEFINE(DataPacks, LevelToIdx);
};