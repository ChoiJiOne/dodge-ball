#pragma once

#include "IDataChunk.h"

#include "BallDataPack.generated.h"

struct BallDataChunk : public IDataChunk
{
	std::vector<BallDataPack> DataPacks;

	MSGPACK_DEFINE(DataPacks);
};