#include "Question.h"

// コンストラクタ
Question::Question()
{
}

// デストラクタ
Question::~Question()
{
}

// 初期化処理
void Question::Initialize()
{
}

// 更新処理
eSceneType Question::Update(const float& delta_second)
{
	return GetNowSceneType();
}

// 描画処理
void Question::Draw() const
{
}

// 終了時処理
void Question::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType Question::GetNowSceneType() const
{
	return eSceneType::eEnd;
}
