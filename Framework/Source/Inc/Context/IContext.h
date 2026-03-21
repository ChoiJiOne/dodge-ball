#pragma once

#include "Macro/Macro.h"

class IContext
{
public:
	IContext() = default;
	virtual ~IContext() = default;

	DISALLOW_COPY_AND_ASSIGN(IContext);
};