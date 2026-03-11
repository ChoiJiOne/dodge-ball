#include "Manager/DataChunkManager.h"

Result<void> DataChunkManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_DATA_CHUNK_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> DataChunkManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_DATA_CHUNK_MANAGER"));
	}

	for (auto& [key, dataChunk] : _cacheDataChunk)
	{
		dataChunk.reset();
		dataChunk = nullptr;
	}

	_isInitialized = false;
	return Result<void>::Success();
}