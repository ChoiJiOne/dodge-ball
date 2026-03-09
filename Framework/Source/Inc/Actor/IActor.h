#pragma once

#include <format>
#include <map>
#include <string>
#include <memory>
#include <typeinfo>

#include "Result.h"
#include "Macro/Macro.h"
#include "Actor/IActorModel.h"
#include "Actor/IActorController.h"
#include "Render/IRenderableModel.h"

class IActor
{
public:
	IActor() = default;
	virtual ~IActor() = default;

	DISALLOW_COPY_AND_ASSIGN(IActor);

	virtual void Tick(float deltaSeconds)
	{
		for (auto& [key, controller] : _controllerMap)
		{
			if (controller)
				controller->OnTick(deltaSeconds);
		}
	}

	virtual void Release()
	{
		for (auto& [key, controller] : _controllerMap)
		{
			if (controller) 
				controller->OnRelease();
		}

		_controllerMap.clear();
		_modelMap.clear();
	}

	bool IsInitialized() const { return _isInitialized; }

	template<typename TModel, typename... Args>
	Result<void> AddModel(Args&&... args)
	{
		std::string key = typeid(TModel).name();
		auto iter = _modelMap.find(key);
		if (iter != _modelMap.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_ACTOR_MODEL,
				std::format("ALREADY_EXIST_ACTOR_MODEL:{0}", key)
			));
		}

		std::unique_ptr<TModel> model = std::make_unique<TModel>(std::forward<Args>(args)...);
		TModel* rawModelPtr = model.get();

		_modelMap.emplace(key, std::move(model));
		
		if (std::is_base_of_v<IRenderableModel, TModel>)
		{
			IRenderableModel* renderableModel = dynamic_cast<IRenderableModel*>(rawModelPtr);
			_renderableModelMap.emplace(key, renderableModel);
		}

		return Result<void>::Success();
	}

	template<typename TModel>
	Result<TModel*> GetModel() const
	{
		std::string key = typeid(TModel).name();
		auto iter = _modelMap.find(key);
		if (iter == _modelMap.end())
		{
			return Result<TModel*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_ACTOR_MODEL,
				std::format("NOT_FOUND_ACTOR_MODEL:{0}", key)
			));
		}

		TModel* model = reinterpret_cast<TModel*>(iter->second.get());
		return Result<TModel*>::Success(model);
	}

	const std::map<std::string, IRenderableModel*>& GetRenderableModelMap() const
	{
		return _renderableModelMap;
	}
	
	template<typename TController, typename... Args>
	Result<void> AddController(Args&&... args)
	{
		std::string key = typeid(TController).name();
		auto iter = _controllerMap.find(key);
		if (iter != _controllerMap.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_ACTOR_CONTROLLER,
				std::format("ALREADY_EXIST_ACTOR_CONTROLLER:{0}", key)
			));
		}

		std::unique_ptr<TController> controller = std::make_unique<TController>(std::forward<Args>(args)...);
		controller->OnInitialize(this);

		_controllerMap.emplace(key, std::move(controller));

		return Result<void>::Success();
	}

	template<typename TController>
	Result<TController*> GetController() const
	{
		std::string key = typeid(TController).name();
		auto iter = _controllerMap.find(key);
		if (iter == _controllerMap.end())
		{
			return Result<TController*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_ACTOR_CONTROLLER,
				std::format("NOT_FOUND_ACTOR_CONTROLLER:{0}", key)
			));
		}

		TController* controller = reinterpret_cast<TController*>(iter->second.get());
		return Result<TController*>::Success(controller);
	}

protected:
	bool _isInitialized = false;

private:
	std::map<std::string, std::unique_ptr<IActorModel>> _modelMap;
	std::map<std::string, std::unique_ptr<IActorController>> _controllerMap;
	std::map<std::string, IRenderableModel*> _renderableModelMap;
};