#pragma once

#include <functional>
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
	Result<void> RegisterScene(Args&&...args)
	{
		std::string key = typeid(TScene).name();
		auto iter = _cacheSceneMap.find(key);
		if (iter != _cacheSceneMap.end()) {
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_SCENE,
				std::format("ALREADY_EXIST_SCENE:{0}", key)
			));
		}

		std::unique_ptr<TScene> scene = std::make_unique<TScene>(std::forward<Args>(args)...);
		_cacheSceneMap.emplace(key, std::move(scene));

		return Result<void>::Success();
	}

	template <typename TScene>
	void UnregisterScene()
	{
		std::string key = typeid(TScene).name();
		auto iter = _cacheSceneMap.find(key);
		if (iter != _cacheSceneMap.end())
		{
			iter->second.reset();
			iter->second = nullptr;

			_cacheSceneMap.erase(iter);
		}
	}

	template <typename TScene>
	Result<TScene*> GetScene()
	{
		std::string key = typeid(TScene).name();
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

	template <typename TScene>
	Result<void> TransitionImmediate()
	{
		std::string key = typeid(TScene).name();
		auto iter = _cacheSceneMap.find(key);
		if (iter == _cacheSceneMap.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_SCENE,
				std::format("SetCurrentScene failed, NOT_FOUND_SCENE:{0}", key)
			));
		}

		IScene* prevScene = _currentScene;
		IScene* nextScene = iter->second.get();

		if (prevScene)
		{
			prevScene->OnExit();
		}

		if (nextScene)
		{
			if (Result<void> result = nextScene->OnEnter(); !result.IsSuccess())
			{
				_currentScene = nullptr;
				return result;
			}
		}

		_currentScene = nextScene;
		return Result<void>::Success();
	}

	template <typename TScene>
	void Transition()
	{
		_transitionEvent = [this]() { return TransitionImmediate<TScene>(); };
	}

	void FlushTransitionEvent()
	{
		if (!_transitionEvent)
		{
			return;
		}

		auto transitionEvent = std::move(_transitionEvent);
		_transitionEvent = nullptr;
		transitionEvent();
	}

	IScene* GetCurrentScene() const { return _currentScene; }

private:
	friend class IManager<SceneManager>;

	SceneManager() = default;
	virtual ~SceneManager() = default;

private:
	IScene* _currentScene = nullptr;
	std::map<std::string, std::unique_ptr<IScene>> _cacheSceneMap;
	std::function<Result<void>()> _transitionEvent = nullptr;
};