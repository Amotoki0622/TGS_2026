#pragma once

#include "../../Object/Player/Player.h" 
#include "../../Object/Goal/Goal.h"
#include "../../Object/Wall/Wall.h"
#include "../../Object/Warp/Warp.h"
#include "../../Object/Block/Block.h"
#include "../SceneBase.h"
#include "../../Utility/ResourceManager.h"
#include "../../Object/Trap/TrapObject/TrapObject.h"     // 親クラス
#include "../../Object/Trap/Cam/Cam.h"                   // 子クラス(カメラ)
#include "../../Object/Trap/Light/Light.h"               // 子クラス(ライト)
#include "../../Object/Trap/LegTrap/LegTrap.h"           // 子クラス(トラバサミ)
#include "../../Object/Trap/SpikeTrap/SpikeTrap.h"       // 子クラス(トゲ床)
#include "../../Object/Key/Key.h"
#include "../../Utility/Fade/Fade.h"
#include "../../Stage/StageManager.h"
#include "../../Utility/DrawNumber.h"
#include <vector>

class InGameScene : public SceneBase
{
	
	// インゲーム内状態用列挙型
	enum class SceneState {
		Playing,    // 通常時
		Detected,   // 検知猶予中
		Restarting,  // フェード中・リセット待ち
		StageNotifier
	};

private:
	StageManager m_stageManager;		// StageManagerのクラスを持つ
	Player player;
	Goal goal;
	std::vector<Warp> warps;
	/*Wall wall;*/
	std::vector<Wall> walls;
	std::vector<Block> blocks;
	std::vector<Key> key;
	std::vector<GameObject*> allObjects;

	// フォント
	int font[3];

	float m_notifierTimer;			// ステージ表示の残り時間を削るタイマー
	int m_notifierAlpha;			// 現在の文字の透明度を保存する

	/// <summary>
	/// フラグがtrueになったらリスタート
	/// </summary>
	bool m_isRestartNotifier;		// リスタート判定フラグ

	int fontPosX;					// 計算済みのX座標
	int fontPosY;					// 計算済みのY座標
	char fontText[32];				// 作成済みの文字列
	char m_tekazuText[16];

	// リソースマネージャー
	ResourceManager* resource;      

	// 検知オブジェクト（カメラ・照明）をまとめて管理
	std::vector<TrapObject*> detectors;


	int background;



	static const int MAX_HELP_PAGES = 3;    // ヘルプ画像の総枚数
	int helpImageHandles[MAX_HELP_PAGES];   // 画像ハンドルを保存する配列
	int currentHelpPage = 0;                // 現在何枚目を表示しているか (0が1枚目)
	bool isHelpOpen = false;                // ヘルプ画面が開いているかフラグ
	float helpOffsetY = 720.0f;             // アニメーション用
	float pausePushTimer = 0.0f;            // ポーズ画面のボタン押し込みタイマー

	// 音源関連
	int freq;
	bool isBgmStarted = false; // これはメンバ変数
	int mainBGM;
	int beepSE;          // 警報音
	int dieSE;           // プレイヤー死亡時の音
	int pageSE;          //　ヘルプ音

	SceneState state = SceneState::Playing;
	Fade* fade = nullptr;          // フェードクラス
	float detectionTimer = 0.0f;   // 猶予タイマー
	const float LIMIT_TIME = 2.5f; // 見つかってからの制限時間

	int tekazu;
	int delay;



private:
	bool isPaused = false;            // ポーズ中かどうかのフラグ
	int pauseBackgroundHandle = -1;   // ボカシ背景画像を保存するハンドル
	int pauseSelectIndex = 0;         // メニューの選択位置（0: 戻る, 1: タイトル）



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

private:
	// 文字列を画面の中心に描画する処理
	void SetUpStageText(const char* text);

	// ステージ通知のアルファ値を計算する処理
	void UpdateNotifierAlpha(float fadeInTime, float fadeOutTime);
public:
	// 現在のシーン情報を返す
	virtual eSceneType GetNowSceneType() const override;

};