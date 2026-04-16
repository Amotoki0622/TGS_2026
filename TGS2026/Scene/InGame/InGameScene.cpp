#include "InGameScene.h"
#include "DxLib.h"

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
eSceneType InGameScene::Update(const float& delta_second)
{
	return GetNowSceneType();
}

// 描画処理
void InGameScene::Draw() const
{
	SetFontSize(20);
	DrawString(10, 10, "INGAME", 0xffffff);
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
