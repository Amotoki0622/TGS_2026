#include "InGameScene.h"
#include "DxLib.h"

// コンストラクタ
InGameScene::InGameScene()
{
	isBgmStarted = false;

	// メインBGM
	mainBGM = LoadSoundMem("Resource/Sounds/BGM/main/main01.mp3");
	// 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
	ChangeVolumeSoundMem(85, mainBGM);

	// 警報音
	beepSE = LoadSoundMem("Resource/Sounds/SE/object/cam/cam3.mp3");
	freq = GetFrequencySoundMem(beepSE); // 元の周波数を取得
	// 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
	ChangeVolumeSoundMem(70, beepSE);
	// 倍速にする場合
	SetFrequencySoundMem((int)(freq * 0.6f), beepSE);


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
	/*warp = new Warp(120, 320, 80, 80, 900, 320);*/

	player.Initialize();  // ←追加
	goal.Initialize();
	/*warp.Initialize();*/
	goal.SetPlayer(&player);
	/*warp.SetPlayer(&player);*/

	// オブジェクトの生成
	allObjects.push_back(new Wall(640.0f, 130.0f, 1280.0f, 128.0f));
	allObjects.push_back(new Wall(640.0f, 645.0f, 1280.0f, 128.0f));

	allObjects.push_back(new Block(448.0f, 258.0f, 128.0f, 128.0f));
	allObjects.push_back(new Block(448.0f, 386.0f, 128.0f, 128.0f));
	allObjects.push_back(new Block(448.0f, 514.0f, 128.0f, 128.0f));

	allObjects.push_back(new Warp(900.0f, 320.0f, 128.0f, 128.0f, 328.0f, 300.0f));
	// 3. プレイヤーの初期化
	player.Initialize();

	if (!fade) fade = new Fade(); // 生成
	state = SceneState::Playing;
	detectionTimer = 0.0f;

		// プレイヤーをセット
	//for (auto& wall : walls)
	//{
	//	wall.SetPlayer(&player);
	//}

	//for (auto& block : blocks)
	//{
	//	block.SetPlayer(&player);
	//}


	background = LoadGraph("Resource/Images/GameMain/background2.png");   // 背景画像

	// 音源関連・読み込み
	dieSE = LoadSoundMem("Resource/Sounds/SE/object/player/light_die01.mp3");
	freq = GetFrequencySoundMem(dieSE); // 元の周波数を取得
	// 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
	ChangeVolumeSoundMem(70, dieSE);
	// 倍速にする場合
	SetFrequencySoundMem((int)(freq * 1.0f), dieSE);

	
	// --- 検知オブジェクトの配置 ---
	// 一旦リストを掃除（リセット時用）
	for (auto d : detectors) delete d;
	detectors.clear();

	// カメラ配置: (x, y, 角度, 距離, 視野角)
	// 向き(角度)はラジアン: 0=右, PI/2=下, PI=左, PI*1.5=上
	detectors.push_back(new Cam(400.0f, 150.0f, DX_PI_F / 2.0f, 350.0f, 0.8f));
	detectors.push_back(new Cam(950.0f, 600.0f, DX_PI_F * 1.5f, 400.0f, 0.7f));

	//// 照明配置: (x, y, 半径)
	//detectors.push_back(new Light(640.0f, 360.0f, 120.0f));
	//detectors.push_back(new Light(200.0f, 500.0f, 80.0f));


	// 画面中央付近に、半径80pxの判定を持つトラバサミを配置
	detectors.push_back(new SpikeTrap(640.0f, 400.0f, 80.0f));

	//出現位置設定↓
	//player.x = 500;
	//player.y = 200;
}

// 更新処理
eSceneType InGameScene::Update(const float& delta_second)
{
	// フェードの更新を常に行う
	fade->Update(delta_second);

	// 音の再生
	if (CheckSoundMem(mainBGM) == 0) {
		// ループ再生
		PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP);
	}

	// フェードアウト中（リスタート待機中）の処理
	if (state == SceneState::Restarting)
	{
		if (fade->IsFinished()) {
			Initialize(); // 暗転しきったら初期化
			fade->Start(FadeType::IrisOut, false, 1.0f);  // フェードイン開始
			state = SceneState::Playing;
		}
		return GetNowSceneType(); // リスタート中は以下の処理をスキップ
	}

	player.Update(delta_second);  
	player.Move(allObjects);
	goal.Update(delta_second);
	/*warp->Update(delta_second);*/

	for (auto& wall : walls)
	{
		wall.Update(delta_second);
	}

	for (auto& block : blocks)
	{
		block.Update(delta_second);
	}

	for (auto& warp : warps)
	{
		warp.Update(delta_second);
	}

	// --- 1. 検知判定フェーズ ---
    // 毎フレーム、まずは「見つかっていない」状態からチェックを開始する
	bool isCamDetected = false;          // カメラに検知
	bool isLightDetected = false;        // ライトに検知

	for (auto d : detectors) {
		// 設置物（カメラ・ライト）の状態を更新（プレイヤーとの距離計算など）
		d->Update(player,delta_second);

		// その設置物の検知範囲内にプレイヤーが入っているか判定
		if (d->IsDetected()) {
			// カメラに検知された場合
			if (d->GetType() == TrapType::Camera) {
				isCamDetected = true;
			}
			// ライトに検知され、かつプレイヤーが「影状態」だった場合
			else if (d->GetType() == TrapType::Light && player.GetState() == Player::State::Shadow) {
				isLightDetected = true;
			}
		}
	}

	// カメラの音源処理
	if (isCamDetected) {
		// 【見つかっている間】
		// まだ鳴っていなければ、ループ再生を開始
		if (CheckSoundMem(beepSE) == 0) {
			PlaySoundMem(beepSE, DX_PLAYTYPE_LOOP);
		}
	}
	else {
		// 【逃げ切った、または範囲外】
		// 鳴っていたら止める
		if (CheckSoundMem(beepSE) == 1) {
			StopSoundMem(beepSE);
		}

		if (state == SceneState::Detected) {
			state = SceneState::Playing;
			detectionTimer = 0.0f;
		}
	}


	// --- 2. ステート（進行状況）管理フェーズ ---
	// カメラかライト、どちらかに検知されている場合
	if (isCamDetected || isLightDetected) {

		// 通常プレイ中(Playing)に見つかったら、即座に「検知猶予状態(Detected)」へ移行
		if (state == SceneState::Playing) {
			state = SceneState::Detected;
			detectionTimer = 0.0f; // タイマーをリセットしてカウント開始
		}

		// 【重要】ライト（影で触れた）なら猶予なし、カメラなら時間経過でタイマーを進める
		if (isLightDetected) {
			// 状態が切り替わるタイミングで再生
			PlaySoundMem(dieSE, DX_PLAYTYPE_BACK);
			detectionTimer = LIMIT_TIME; // ライトの場合は強制的にタイムアップ状態にする
		}
		else {
			detectionTimer += delta_second; // カメラの場合は現実時間の経過秒数を加算
		}

		

		// --- 3. 失敗（リセット）確定判定 ---
		// タイマーが制限時間を超えた（＝捕まった）場合の処理
		if (detectionTimer >= LIMIT_TIME) {
			// 例：入れるかは別。ここで上記のループ音を止め、「ガシャーン！」などの失敗音を再生する
			// ここに入れる
			StopSoundMem(beepSE);
		
			// 「リスタート待機状態(Restarting)」へ移行し、画面演出を開始
			state = SceneState::Restarting;

			if (isLightDetected) {
				// 【ライト演出】
				// 画面全体が非常にゆっくり暗くなる(Normal)演出
				// 1.0f(完了) / 3.0(秒) = 約 0.33f
				fade->Start(FadeType::Normal, true, 0.8f);;
			}
			else {
				// 【カメラ演出】見つかって捕まったイメージ
				// 円形に画面が閉じていく(IrisOut)演出
				// パシッと1秒で暗くする場合
	            // 1.0f(完了) / 1.0(秒) = 1.0f
				fade->Start(FadeType::IrisOut, true, 1.0f);
			}
		}
	}
	// どちらにも検知されていない場合
	else {
		// もし「検知猶予中」に範囲外へ逃げ出せたら、通常プレイ状態に戻す
		if (state == SceneState::Detected) {
			state = SceneState::Playing;
			detectionTimer = 0.0f; // 猶予タイマーをリセット

			// ここでループ音を止める
		}
	}

	// ゴール判定
	if (goal.IsGoal())
	{
		// シーンの遷移
		return eSceneType::eGameOver;
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

	// 追加したオブジェクト（壁やブロック）を全て描画
	for (const auto& obj : allObjects) {
		if (obj != nullptr) {
			obj->Draw();
		}
	}

	/*for (auto& wall : walls)
	{
		wall.Draw();
	}

	for (auto& block : blocks)
	{
		block.Draw();
	}*/
	
	goal.Draw();
	player.Draw();
	/*warp->Draw();*/

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
		if (d->GetType() == TrapType::Camera)
		{
			d->Draw();
		}
	}

	for (auto d : detectors) d->Draw();

	// 猶予期間中の演出
	if (state == SceneState::Detected) {

		// カメラ検知があるか確認
		bool isCameraDetecting = false;
		for (auto d : detectors) {
			if (d->GetType() == TrapType::Camera && d->IsDetected()) {
				isCameraDetecting = true;
				break;
			}
		}

		// カメラ検知時のみ点滅させる
		if (isCameraDetecting) {
			// detectionTimerを使って点滅ロジックを作る 
			if ((int)(detectionTimer * 2) % 2 == 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
				DrawBox(0, 0, 1280, 720, GetColor(255, 0, 0), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
		// Lightの時は赤色を表示しないので、ここに else は書かない
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
