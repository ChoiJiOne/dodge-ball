#pragma once

#include <map>
#include <memory>
#include <string>
#include <typeinfo>

// NOTE: �ϴ�, raylib���� �����ϴ� ���� �б� ����� ����ؾ� �ؼ� �ʿ� (���� ���ִ� �� ����.)
#include <msgpack.hpp>
#include <raylib.h>

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
	Result<void> LoadDataChunk(const std::string& filePath)
	{
		std::string key = typeid(TDataChunk).name();
		auto iter = _cacheDataChunk.find(key);
		if (iter != _cacheDataChunk.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_LOAD_DATA_CHUNK,
				std::format("ALREADY_LOAD_DATA_CHUNK:{0}", filePath)
			));
		}

		int32_t dataSize = 0;
		uint8_t* fileData = LoadFileData(filePath.c_str(), &dataSize);

		if (fileData == nullptr || dataSize == 0)
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::FAILED_TO_LOAD_DATA_CHUNK,
				std::format("FAILED_TO_LOAD_DATA_CHUNK:{0}", filePath)
			));
		}

		// NOTE: msgpack���� ���� ������ ���⼭�� try-catch ���)
		try
		{
			std::unique_ptr<TDataChunk> dataChunk = std::make_unique<TDataChunk>();

			msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(fileData), dataSize);
			msgpack::object deserialized = oh.get();
			deserialized.convert(*dataChunk);

			_cacheDataChunk.emplace(key, std::move(dataChunk));
		}
		catch (const msgpack::type_error& e)
		{
			UnloadFileData(fileData);
			fileData = nullptr;

			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::FAILED_TO_PARSE_DATA_CHUNK,
				std::format("FAILED_TO_PARSE_DATA_CHUNK:{0}", filePath)
			));
		}
		catch (...)
		{
			UnloadFileData(fileData);
			fileData = nullptr;

			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::UNKNOWN_DATA_CHUNK_ERROR,
				std::format("UNKNOWN_DATA_CHUNK_ERROR:{0}", filePath)
			));
		}

		UnloadFileData(fileData);
		fileData = nullptr;

		return Result<void>::Success();
	}

	template <typename TDataChunk>
	Result<const TDataChunk*> GetDataChunk()
	{
		std::string key = typeid(TDataChunk).name();
		auto iter = _cacheDataChunk.find(key);
		if (iter == _cacheDataChunk.end())
		{
			return Result<const TDataChunk*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_DATA_CHUNK,
				std::format("NOT_FOUND_DATA_CHUNK:{0}", key)
			));
		}

		const TDataChunk* dataChunkPtr = reinterpret_cast<const TDataChunk*>(iter->second.get());
		return Result<const TDataChunk*>::Success(dataChunkPtr);
	}
	
private:
	friend class IManager<DataChunkManager>;

	DataChunkManager() = default;
	virtual ~DataChunkManager() = default;

private:
	std::map<std::string, std::unique_ptr<IDataChunk>> _cacheDataChunk;
};