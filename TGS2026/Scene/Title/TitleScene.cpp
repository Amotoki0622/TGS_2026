#include "TitleScene.h"
#include "DxLib.h"

// コンストラクタ
TitleScene::TitleScene()
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
}

// 更新処理
eSceneType TitleScene::Update(const float& delta_second)
{
	return GetNowSceneType();
}

// 描画処理
void TitleScene::Draw() const
{
	SetFontSize(20);
	DrawString(10, 10, "TITLE", 0xffffff);
}

// 終了時処理
void TitleScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
