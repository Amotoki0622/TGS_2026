#pragma once

#include "../../Object/Player/Player.h" 
#include "../../Object/Goal/Goal.h"
#include "../../Object/Wall/Wall.h"
#include "../../Object/Warp/Warp.h"
#include "../SceneBase.h"
#include "../../Utility/ResourceManager.h"
#include "../../Object/Detective/DetectiveObject.h" // 親クラス
#include "../../Object/Cam/Cam.h"             // 子クラス
#include "../../Object/Light/Light.h"           // 子クラス
#include "../../Utility/Fade/Fade.h"
#include <vector>

class InGameScene : public SceneBase
{
	
	// インゲーム内状態用列挙型
	enum class SceneState {
		Playing,    // 通常時
		Detected,   // 検知猶予中
		Restarting  // フェード中・リセット待ち
	};

private:
	Player player;
	Goal goal;
	Warp* warp;
	/*Wall wall;*/
	std::vector<Wall> walls;

	// リソースマネージャー
	ResourceManager* resource;      

	// 検知オブジェクト（カメラ・照明）をまとめて管理
	std::vector<DetectiveObject*> detectors;


	int background;

	// 音源関連
	int freq;
	bool isBgmStarted = false; // これはメンバ変数
	int mainBGM;
	int beepSE;          // 警報音
	int dieSE;           // プレイヤー死亡時の音

	SceneState state = SceneState::Playing;
	Fade* fade = nullptr;          // フェードクラス
	float detectionTimer = 0.0f;   // 猶予タイマー
	const float LIMIT_TIME = 2.5f; // 見つかってからの制限時間

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