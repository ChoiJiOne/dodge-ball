#include "Actor/IActor.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"
#include "Manager/SceneManager.h"
#include "Scene/IScene.h"

#include "EnemyActor.h"
#include "EnemySpawnActorController.h"
#include "EnemySpawnActorModel.h"

void EnemySpawnActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	Result<EnemySpawnActorModel*> result = _ownerActor->GetModel<EnemySpawnActorModel>();
	if (!result.IsSuccess()) // Get이 실패할 수 있을까...?
	{
		LOG_E("FAILED_TO_GET_ENEMY_SPAWN_MODEL"); // 일단 로그를 찍어보자.
		return;
	}
	else
	{
		_model = result.GetValue();
	}
}

void EnemySpawnActorController::OnRelease()
{
	_model = nullptr;
}

void EnemySpawnActorController::OnTick(float deltaSeconds)
{
	_stepTime += deltaSeconds;
	if (_stepTime < _spawnTime) 
	{
		return;
	}

	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	Result<void> result = currentScene->CreateAndAddActor<EnemyActor>(std::format("EnemyActor_{0}", _count));
	if (!result.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_AND_ADD_ENEMY_ACTOR:(count:{0})", _count);
		return;
	}

	_count++;
	_stepTime = 0.0f;

}
