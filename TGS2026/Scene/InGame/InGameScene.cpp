#include "InGameScene.h"

// コンストラクタ
InGameScene::InGameScene()
{
}

// デストラクタ
InGameScene::~InGameScene()
{
}

// 初期化処理
void InGameScene::Initialize()
{
}

// 更新処理
eSceneType InGameScene::Update()
{
	return GetNowSceneType();
}

// 描画処理
void InGameScene::Draw() const
{
}

// 終了時処理
void InGameScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
