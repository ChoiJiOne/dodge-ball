#pragma once

#include "IDataChunk.h"

#include "PlayerDataPack.generated.h"

struct PlayerDataChunk : public IDataChunk
{
	std::vector<PlayerDataPack> DataPacks;

	MSGPACK_DEFINE(DataPacks);
};