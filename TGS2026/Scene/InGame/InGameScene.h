#pragma once

#include "../../Object/Player/Player.h" 
#include "../../Object/Goal/Goal.h"
#include "../../Object/Wall/Wall.h"
#include "../SceneBase.h"

class InGameScene : public SceneBase
{

private:
	Player player;
	Goal goal;
	Wall wall;

public:
	// コンストラクタ
	InGameScene();
	// デストラクタ
	~InGameScene();

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