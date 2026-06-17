#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

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
	background = LoadGraph("Resource/Images/result.png");
}

// 更新処理
eSceneType ResultScene::Update(const float& delta_second)
{
	return GetNowSceneType();
}

// 描画処理
void ResultScene::Draw() const
{
	DrawGraph(0, 0, background, TRUE);
}

// 終了時処理
void ResultScene::Finalize()
{
	DeleteGraph(background);
}

// 現在のシーン情報を返す
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
