#pragma once

#include "Macro/Macro.h"

class IActor;

class IActorController
{
public:
	IActorController() = default;
	virtual ~IActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(IActorController);

	virtual void OnInitialize(IActor* owner)
	{
		_ownerActor = owner;
	}

	virtual void OnRelease() = 0;
	virtual void OnTick(float deltaSeconds) = 0;
	virtual void OnCollision(IActor* actor) = 0;

protected:
	IActor* _ownerActor = nullptr;
};