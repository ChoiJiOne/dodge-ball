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

struct ActorKey
{
	int32_t order;
	std::string key;

	bool operator<(const ActorKey& instance) const
	{
		if (order != instance.order)
		{
			return order < instance.order;
		}

		return key < instance.key;
	}
};

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
			{
				controller->OnTick(deltaSeconds);
			}
		}
	}

	virtual void Release()
	{
		for (auto& [key, controller] : _controllerMap)
		{
			if (controller)
			{
				controller->OnRelease();
			}
		}

		_controllerMap.clear();
		_modelMap.clear();
	}

	bool IsInitialized() const { return _isInitialized; }

	template<typename TModel, typename... Args>
	Result<void> AddModel(int32_t order, Args&&... args)
	{
		std::string key = typeid(TModel).name();
		for (const auto& [actorKey, model] : _modelMap)
		{
			if (actorKey.key == key)
			{
				return Result<void>::Fail(MAKE_ERROR(
					EErrorCode::ALREADY_EXIST_ACTOR_MODEL,
					std::format("ALREADY_EXIST_ACTOR_MODEL:{0}", key)
				));
			}
		}

		std::unique_ptr<TModel> model = std::make_unique<TModel>(std::forward<Args>(args)...);
		TModel* rawModelPtr = model.get();

		ActorKey actorKey{ order, key };
		_modelMap.emplace(actorKey, std::move(model));
		
		if (std::is_base_of_v<IRenderableModel, TModel>)
		{
			IRenderableModel* renderableModel = dynamic_cast<IRenderableModel*>(rawModelPtr);
			_renderableModelMap.emplace(actorKey, renderableModel);
		}

		return Result<void>::Success();
	}

	template<typename TModel>
	Result<TModel*> GetModel() const
	{
		std::string key = typeid(TModel).name();
		for (const auto& [actorKey, model] : _modelMap)
		{
			if (actorKey.key == key)
			{
				TModel* targetModel = reinterpret_cast<TModel*>(model.get());
				return Result<TModel*>::Success(targetModel);
			}
		}

		return Result<TModel*>::Fail(MAKE_ERROR(
			EErrorCode::NOT_FOUND_ACTOR_MODEL,
			std::format("NOT_FOUND_ACTOR_MODEL:{0}", key)
		));
	}

	const std::map<ActorKey, IRenderableModel*>& GetRenderableModelMap() const
	{
		return _renderableModelMap;
	}
	
	template<typename TController, typename... Args>
	Result<void> AddController(int32_t order, Args&&... args)
	{
		std::string key = typeid(TController).name();
		for (const auto& [actorKey, controller] : _controllerMap)
		{
			if (actorKey.key == key)
			{
				return Result<void>::Fail(MAKE_ERROR(
					EErrorCode::ALREADY_EXIST_ACTOR_CONTROLLER,
					std::format("ALREADY_EXIST_ACTOR_CONTROLLER:{0}", key)
				));
			}
		}
		
		std::unique_ptr<TController> controller = std::make_unique<TController>(std::forward<Args>(args)...);
		controller->OnInitialize(this);

		ActorKey actorKey{ order, key };
		_controllerMap.emplace(actorKey, std::move(controller));

		return Result<void>::Success();
	}

	template<typename TController>
	Result<TController*> GetController() const
	{
		std::string key = typeid(TController).name();
		for (const auto& [actorKey, controller] : _controllerMap)
		{
			if (actorKey.key == key)
			{
				TController* targetController = reinterpret_cast<TController*>(controller.get());
				return Result<TController*>::Success(targetController);
			}
		}

		return Result<TController*>::Fail(MAKE_ERROR(
			EErrorCode::NOT_FOUND_ACTOR_CONTROLLER,
			std::format("NOT_FOUND_ACTOR_CONTROLLER:{0}", key)
		));
	}

protected:
	bool _isInitialized = false;

private:
	std::map<ActorKey, std::unique_ptr<IActorModel>> _modelMap;
	std::map<ActorKey, std::unique_ptr<IActorController>> _controllerMap;
	std::map<ActorKey, IRenderableModel*> _renderableModelMap;
};