#pragma once

#include "../SceneBase.h"

class ResultScene : public SceneBase
{
private:

	//背景画像
	int background;

	int font[2];

	//追加
	bool is_selected = false;
	float transition_timer = 0.0f;

	float clearAlpha = 0.0f;
	float backAlpha = 0.0f;

public:
	// コンストラクタ
	ResultScene();
	// デストラクタ
	~ResultScene();

public:
	// 初期化処理
	virtual void Initialize() override;
	// 更新処理
	virtual eSceneType Update(const float& delta_second) override;
	// 描画処理
	virtual void Draw() const override;
	// 終了時処理
	virtual void Finalize() override;

public:
	// 現在のシーン情報を返す
	virtual eSceneType GetNowSceneType() const override;

};