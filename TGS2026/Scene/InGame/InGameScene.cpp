#include "InGameScene.h"
#include "DxLib.h"

// コンストラクタ
InGameScene::InGameScene()
{
}

// デストラクタ
InGameScene::~InGameScene()
{
	// メモリ解放
	Finalize();
}

// 初期化処理
void InGameScene::Initialize()
{
	player.Initialize();  // ←追加
	goal.Initialize();
	goal.SetPlayer(&player);

	// 壁を配置
	walls.emplace_back(640, 120, 1280, 200);
	walls.emplace_back(640, 635, 1280, 200);
	walls.emplace_back(618, 320, 150, 220);

	//wall.Initialize();
	//wall.SetPlayer(&player);

	if (!fade) fade = new Fade(); // 生成
	state = SceneState::Playing;
	detectionTimer = 0.0f;

		// プレイヤーをセット
	for (auto& wall : walls)
	{
		wall.SetPlayer(&player);
	}


	background = LoadGraph("Resource/Images/GameMain/background2.png");   // 背景画像

	
	// --- 検知オブジェクトの配置 ---
	// 一旦リストを掃除（リセット時用）
	for (auto d : detectors) delete d;
	detectors.clear();

	// カメラ配置: (x, y, 角度, 距離, 視野角)
	// 向き(角度)はラジアン: 0=右, PI/2=下, PI=左, PI*1.5=上
	detectors.push_back(new Cam(400.0f, 150.0f, DX_PI_F / 2.0f, 350.0f, 0.8f));
	detectors.push_back(new Cam(800.0f, 600.0f, DX_PI_F * 1.5f, 400.0f, 1.2f));

	// 照明配置: (x, y, 半径)
	detectors.push_back(new Light(640.0f, 360.0f, 120.0f));
	detectors.push_back(new Light(200.0f, 500.0f, 80.0f));

	//出現位置設定↓
	//player.x = 500;
	//player.y = 200;
}

// 更新処理
eSceneType InGameScene::Update(const float& delta_second)
{
	// フェードの更新を常に行う
	fade->Update();

	// フェードアウト中（リスタート待機中）の処理
	if (state == SceneState::Restarting)
	{
		if (fade->IsFinished())
		{
			Initialize(); // 暗転しきったら初期化
			fade->Start(FadeType::IrisOut, false, 0.0005f); // フェードイン開始
			state = SceneState::Playing;
		}
		return GetNowSceneType(); // リスタート中は以下の処理をスキップ
	}

	player.Update();  
	player.Move(walls);
	goal.Update(delta_second);

	for (auto& wall : walls)
	{
		wall.Update(delta_second);
	}

	// --- カメラ・照明の更新と検知判定 ---
	bool isDetected = false;
	//for (auto d : detectors)
	//{
	//	d->Update(player);
	//	if (d->IsDetected())
	//	{
	//		isDetected = true;
	//	}
	//}


	// カメラのみの判定
	for (auto d : detectors)
	{
		if (d->GetType() == DetectiveType::Camera)
		{
			d->Update(player);
			if (d->IsDetected()) {
				isDetected = true;
				// ここで「カメラに見つかった」固有のフラグを立てることも可能
			}
		}
	}

	// 照明のみの判定
	for (auto d : detectors)
	{
		if (d->GetType() == DetectiveType::Light)
		{
			d->Update(player);
			if (d->IsDetected()) {
				isDetected = true;
				// 照明は見つかった瞬間にタイマーを最大にする(即アウト)
				detectionTimer = LIMIT_TIME; 
			}
		}
	}

	// --- 検知ステート管理 ---
	if (isDetected)
	{
		if (state == SceneState::Playing)
		{
			state = SceneState::Detected;
			detectionTimer = 0.0f;
			// ここで検知開始SEを鳴らす予定
		}

		detectionTimer += delta_second;

		// 猶予時間を超えたらフェードアウトへ
		if (detectionTimer >= LIMIT_TIME)
		{
			state = SceneState::Restarting;
			fade->Start(FadeType::IrisOut, true, 0.02f);
			// ここで捕まったSEを鳴らす予定
		}
	}
	else
	{
		// 範囲外に出たら猶予状態を解除
		if (state == SceneState::Detected)
		{
			state = SceneState::Playing;
			detectionTimer = 0.0f;
			// ここで検知SEを止める予定
		}
	}

	// ゴール判定
	if (goal.IsGoal())
	{
		// シーンの遷移
		return eSceneType::eTitle;
	}

	return GetNowSceneType();
}

// 描画処理
void InGameScene::Draw() const
{
	SetFontSize(20);
	DrawString(10, 10, "INGAME", 0xffffff);
	
	// タイトル画像の描画
	DrawExtendGraph(0, 0, 1280, 720, background, TRUE);

	for (auto& wall : walls)
	{
		wall.Draw();
	}
	
	goal.Draw();
	player.Draw(); // ←追加

	//// --- カメラ・照明の描画 ---
	//// プレイヤーより後に描くことで、視界範囲をプレイヤーの上に重ねて確認しやすくする
	/*for (auto d : detectors)
	{
		d->Draw();
	}*/

	//// 照明のみ
	//for (auto d : detectors)
	//{
	//	// 自分のタイプが Light の時だけ Draw を呼ぶ
	//	if (d->GetType() == DetectiveType::Light)
	//	{
	//		d->Draw();
	//	}
	//}

	//// カメラのみ
	for (auto d : detectors)
	{
		// 自分のタイプが Light の時だけ Camera を呼ぶ
		if (d->GetType() == DetectiveType::Camera)
		{
			d->Draw();
		}
	}

	for (auto d : detectors) d->Draw();

	// 猶予期間中の演出（画面を少し赤くする等）
	if (state == SceneState::Detected) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
		DrawBox(0, 0, 1280, 720, GetColor(255, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// フェードを最前面に描画
	if (fade) fade->Draw();
}

// 終了時処理
void InGameScene::Finalize()
{
	// 動的に生成したオブジェクトを削除してメモリリークを防ぐ
	for (auto d : detectors)
	{
		delete d;
	}
	detectors.clear();

	if (background != -1)
	{
		DeleteGraph(background);
		background = -1;
	}
}

// 現在のシーン情報を返す
eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
