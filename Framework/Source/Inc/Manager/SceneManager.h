#pragma once

#include <map>
#include <memory>
#include <string>
#include <typeinfo>

#include "Macro/Macro.h"
#include "Manager/IManager.h"
#include "Scene/IScene.h"

class SceneManager : public IManager<SceneManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(SceneManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	template <typename TScene, typename... Args>
	Result<TScene*> CreateScene(const std::string& key, Args &&...args)
	{
		auto iter = _cacheSceneMap.find(key);
		if (iter != _cacheSceneMap.end()) {
			return Result<TScene*>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_SCENE,
				std::format("ALREADY_EXIST_SCENE:{0}", key)
			));
		}

		std::unique_ptr<TScene> scene = std::make_unique<TScene>(std::forward<Args>(args)...);
		TScene* scenePtr = scene.get();

		_cacheSceneMap.emplace(key, std::move(scene));

		return Result<TScene*>::Success(scenePtr);
	}

	template <typename TScene>
	Result<TScene*> GetScene(const std::string& key)
	{
		auto iter = _cacheSceneMap.find(key);
		if (iter == _cacheSceneMap.end())
		{
			return Result<TScene*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_SCENE,
				std::format("NOT_FOUND_SCENE:{0}", key)
			));
		}

		TScene* scenePtr = reinterpret_cast<TScene*>(iter->second.get());
		return Result<TScene*>::Success(scenePtr);
	}

	void DestroyScene(const std::string& key);

private:
	friend class IManager<SceneManager>;

	SceneManager() = default;
	virtual ~SceneManager() = default;

private:
	std::map<std::string, std::unique_ptr<IScene>> _cacheSceneMap;
};