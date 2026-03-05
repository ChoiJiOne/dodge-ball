#pragma once

#include <map>
#include <memory>
#include <string>
#include <typeinfo>

// NOTE: 일단, raylib에서 제공하는 파일 읽기 기능을 사용해야 해서 필요 (향후 없애는 것 고려.)
#include <raylib.h>
#include <msgpack.hpp>

#include "IDataChunk.h"

#include "Macro/Macro.h"
#include "Manager/IManager.h"

class DataChunkManager : public IManager<DataChunkManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(DataChunkManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	template <typename TDataChunk>
	Result<const TDataChunk*> LoadFromFile(const std::string& filePath)
	{
		std::string key = typeid(TDataChunk).name();
		auto iter = _cacheDataChunk.find(key);
		if (iter != _cacheDataChunk.end())
		{
			const TDataChunk* dataChunkPtr = static_cast<const TDataChunk*>(iter->second.get());
			return Result<const TDataChunk*>::Success(dataChunkPtr);
		}

		int32_t dataSize = 0;
		uint8_t* fileData = LoadFileData(filePath.c_str(), &dataSize);

		if (fileData == nullptr || dataSize == 0)
		{
			return Result<const TDataChunk*>::Fail(MAKE_ERROR(
				EErrorCode::FAILED_TO_LOAD_DATA_CHUNK,
				std::format("FAILED_TO_LOAD_DATA_CHUNK:{0}", filePath)
			));
		}

		TDataChunk* rawPtr = nullptr;
		// NOTE: msgpack에서 예외 던져서 여기서는 try-catch 사용)
		try
		{
			std::unique_ptr<TDataChunk> dataChunk = std::make_unique<TDataChunk>();

			msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(fileData), dataSize);
			msgpack::object deserialized = oh.get();

			deserialized.convert(*dataChunk);
			rawPtr = dataChunk.get();

			_cacheDataChunk.emplace(key, std::move(dataChunk));
		}
		catch (const msgpack::type_error& e)
		{
			UnloadFileData(fileData);
			fileData = nullptr;

			return Result<const TDataChunk*>::Fail(MAKE_ERROR(
				EErrorCode::FAILED_TO_PARSE_DATA_CHUNK,
				std::format("FAILED_TO_PARSE_DATA_CHUNK:{0}", filePath)
			));
		}
		catch (...)
		{
			UnloadFileData(fileData);
			fileData = nullptr;

			return Result<const TDataChunk*>::Fail(MAKE_ERROR(
				EErrorCode::UNKNOWN_DATA_CHUNK_ERROR,
				std::format("UNKNOWN_DATA_CHUNK_ERROR:{0}", filePath)
			));
		}

		UnloadFileData(fileData);
		fileData = nullptr;

		return Result<const TDataChunk*>::Success(reinterpret_cast<const TDataChunk*>(rawPtr));
	}
	
private:
	friend class IManager<DataChunkManager>;

	DataChunkManager() = default;
	virtual ~DataChunkManager() = default;

private:
	std::map<std::string, std::unique_ptr<IDataChunk>> _cacheDataChunk;
};