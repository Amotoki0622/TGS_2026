#pragma once

#include "../../Object/Player/Player.h" 
#include "../../Object/Goal/Goal.h"
#include "../../Object/Wall/Wall.h"
#include "../SceneBase.h"
#include "../../Utility/ResourceManager.h"
#include "../../Object/Detective/DetectiveObject.h" // 親クラス
#include "../../Object/Cam/Cam.h"             // 子クラス
#include "../../Object/Light/Light.h"           // 子クラス

class InGameScene : public SceneBase
{

private:
	Player player;
	Goal goal;
	Wall wall;

	// リソースマネージャー
	ResourceManager* resource;      

	// 検知オブジェクト（カメラ・照明）をまとめて管理
	std::vector<DetectiveObject*> detectors;


	int background;

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