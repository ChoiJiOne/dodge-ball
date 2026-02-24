#pragma once

#include "Result.h"
#include "Macro.h"

template <typename T>
class IManager
{
public:
	DISALLOW_COPY_AND_ASSIGN(IManager);

	static T& Get() { return _instance; }
	static T* GetPtr() { return &_instance; } // NOTE: 하위에서 nullptr 체크 필요.
	
	bool IsInitialized() { return _isInitialized; }

	virtual Result<void> Startup() = 0;
	virtual Result<void> Shutdown() = 0;

protected:
	IManager() = default;
	virtual ~IManager() = default;

protected:
	bool _isInitialized = false;

private:
	static T _instance;
};

#include "IManager.inl"