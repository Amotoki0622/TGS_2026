#include "EndScene.h"

// コンストラクタ
EndScene::EndScene()
{
}

// デストラクタ
EndScene::~EndScene()
{
}

// 初期化処理
void EndScene::Initialize()
{
}

// 更新処理
eSceneType EndScene::Update()
{
	return GetNowSceneType();
}

// 描画処理
void EndScene::Draw() const
{
}

// 終了時処理
void EndScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType EndScene::GetNowSceneType() const
{
	return eSceneType::eEnd;
}
