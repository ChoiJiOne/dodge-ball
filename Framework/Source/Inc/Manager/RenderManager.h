#pragma once

#include "IManager.h"

class RenderManager : public IManager<RenderManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(RenderManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

private:
	friend class IManager<RenderManager>;

	RenderManager() = default;
	virtual ~RenderManager() = default;
};