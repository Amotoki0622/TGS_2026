#include "RankingScene.h"

// コンストラクタ
RankingScene::RankingScene()
{
}

// デストラクタ
RankingScene::~RankingScene()
{
}

// 初期化処理
void RankingScene::Initialize()
{
}

// 更新処理
eSceneType RankingScene::Update(const float& delta_second)
{
	return GetNowSceneType();
}

// 描画処理
void RankingScene::Draw() const
{
}

// 終了時処理
void RankingScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType RankingScene::GetNowSceneType() const
{
	return eSceneType::eRanking;
}
