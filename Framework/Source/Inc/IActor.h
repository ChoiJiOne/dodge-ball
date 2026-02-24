#pragma once

#include "Macro/Macro.h"

class IActor
{
public:
	IActor() = default;
	virtual ~IActor() = default;

	DISALLOW_COPY_AND_ASSIGN(IActor);

	virtual void Tick(float deltaSeconds) = 0;
	virtual void Release() = 0;

	bool IsInitialized() { return _isInitialized; }

protected:
	bool _isInitialized = false;
};