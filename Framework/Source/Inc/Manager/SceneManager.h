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

private:
	friend class IManager<SceneManager>;

	SceneManager() = default;
	virtual ~SceneManager() = default;

private:
	std::map<std::string, std::unique_ptr<IScene>> _cacheSceneMap;
};