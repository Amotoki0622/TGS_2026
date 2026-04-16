#include "ResultScene.h"

// コンストラクタ
ResultScene::ResultScene()
{
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 初期化処理
void ResultScene::Initialize()
{
}

// 更新処理
eSceneType ResultScene::Update()
{
	return GetNowSceneType();
}

// 描画処理
void ResultScene::Draw() const
{
}

// 終了時処理
void ResultScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
