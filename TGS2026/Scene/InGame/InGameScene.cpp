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
	walls.emplace_back(640, 170, 1280, 100);
	walls.emplace_back(640, 590, 1280, 100);

	//wall.Initialize();
	//wall.SetPlayer(&player);

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
	player.Update();  
	goal.Update(delta_second);

	for (auto& wall : walls)
	{
		wall.Update(delta_second);
	}

	// --- カメラ・照明の更新と検知判定 ---
	//bool isDetected = false;
	//for (auto d : detectors)
	//{
	//	d->Update(player);
	//	if (d->IsDetected())
	//	{
	//		isDetected = true;
	//		break; // 誰かが見つけたらループ終了
	//	}
	//}

	// 見つかったらやり直し
	//if (isDetected)
	//{
	//	Initialize(); // 初期位置・初期状態にリセット
	//	return GetNowSceneType();
	//}

	// カメラのみ
	//for (auto d : detectors)
	//{
	//	// カメラの時だけ更新・判定を行う
	//	if (d->GetType() == DetectiveType::Camera)
	//	{
	//		d->Update(player);
	//		if (d->IsDetected()) {
	//			Initialize();
	//			break;
	//		}
	//	}
	//}

	//// 照明のみ
	//for (auto d : detectors)
	//{
	//	// カメラの時だけ更新・判定を行う
	//	if (d->GetType() == DetectiveType::Light)
	//	{
	//		d->Update(player);
	//		if (d->IsDetected()) {
	//			Initialize();
	//			break;
	//		}
	//	}
	//}

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
	for (auto d : detectors)
	{
		d->Draw();
	}

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
	//for (auto d : detectors)
	//{
	//	// 自分のタイプが Light の時だけ Camera を呼ぶ
	//	if (d->GetType() == DetectiveType::Camera)
	//	{
	//		d->Draw();
	//	}
	//}
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
