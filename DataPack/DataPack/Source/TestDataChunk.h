#pragma once

#include "TestDataPack.generated.h"

struct TestDataChunk
{
	std::vector<TestDataPack> DataPacks;

	MSGPACK_DEFINE(DataPacks);
};
