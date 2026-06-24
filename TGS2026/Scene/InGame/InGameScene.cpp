#include "InGameScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

// コンストラクタ
InGameScene::InGameScene()
{
	isBgmStarted = false;

	// メインBGM
	mainBGM = LoadSoundMem("Resource/Sounds/BGM/main/main01.mp3");
	ChangeVolumeSoundMem(140, mainBGM);

	// 警報音
	beepSE = LoadSoundMem("Resource/Sounds/SE/object/cam/cam3.mp3");
	freq = GetFrequencySoundMem(beepSE);
	ChangeVolumeSoundMem(70, beepSE);
	SetFrequencySoundMem((int)(freq * 0.6f), beepSE); // 減速

	// ヘルプ音
	pageSE = LoadSoundMem("Resource/Sounds/SE/object/push/paper_push.mp3");
	ChangeVolumeSoundMem(60, pageSE);

	m_stageManager.Initialize();

	// フォントの設定
	font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 100, 6);
	font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 32, 6); // ポーズ画面に使用
	font[2] = CreateFontToHandle("廻想体 ネクスト UP B", 45, 6); // ポーズ画面に使用

	helpImageHandles[0] = LoadGraph("Resource/Images/Hint/shadow_hint.png");
	helpImageHandles[1] = LoadGraph("Resource/Images/Hint/kick_hint.png");
	helpImageHandles[2] = LoadGraph("Resource/Images/Hint/key_hint.png");
	helpImageHandles[3] = LoadGraph("Resource/Images/Hint/spike_trap_hint.png");
}

// デストラクタ
InGameScene::~InGameScene()
{
	Finalize();
}

// 初期化処理
void InGameScene::Initialize()
{

	allObjects.clear(); // 配列を空っぽにする
	detectors.clear();

	// StageManagerで読みこんだものを呼び出す
	m_stageManager.LoadLevel(m_stageManager.GetCurrentLevel());

	// プレイヤーを初期化
	player.Initialize();

	// 配列に入っているすべてのオブジェクトクラスの初期化処理
	for (GameObject* obj : allObjects)
	{
		if (obj != nullptr)
		{
			obj->Initialize();
		}
	}

	// 現在の階層（インデックス）を取得
	int currentLevel = m_stageManager.GetCurrentLevel();

	// デフォルトは 128.0f / ステージ3以降は一回り小さく
	float currentChipSize = 128.0f;
	if (currentLevel >= 4)
	{
		currentChipSize = 100.0f;
	}

	// プレイヤーのサイズ・移動量を自動計算してセット
	player.SetChipSize(currentChipSize);

	// 確定したレベルの手数をプレイヤーに渡す
	int currentLimit = m_stageManager.GetCurrentMoveLimit();
	player.SetTekazu(currentLimit);

	// StageManagerが生成したブロックをInGameSceneのリストに追加する
	for (auto* obj : m_stageManager.GetGenerateObject())
	{
		allObjects.push_back(obj);

		// もしオブジェクトの正体が「鍵」だったら、プレイヤーのアドレスをセットする
		Key* keyObj = dynamic_cast<Key*>(obj);
		if (keyObj != nullptr)
		{
			keyObj->SetPlayer(&player);
		}

		Goal* goalObj = dynamic_cast<Goal*>(obj);
		if (goalObj != nullptr)
		{
			goalObj->SetPlayer(&player);
		}

		Cam* camObj = dynamic_cast<Cam*>(obj);
		if (camObj != nullptr)
		{
			detectors.push_back(camObj);
		}

		SpikeTrap* spikeObj = dynamic_cast<SpikeTrap*>(obj);
		if (spikeObj != nullptr)
		{
			detectors.push_back(spikeObj);
		}
	}

	// StageManagerから、CSVに書かれたプレイヤーの初期座標をもらう
	Vector2D playerSpawnPos = m_stageManager.GetPlayerSpawnPosition();

	// プレイヤーにCSVから読み込んだ座標をセットする
	player.SetPosition(playerSpawnPos.x, playerSpawnPos.y);

	if (!fade) fade = new Fade(); // 生成
	state = SceneState::StageNotifier;
	detectionTimer = 0.0f;
	m_notifierTimer = 1.8f;
	m_notifierAlpha = 0;		// 透明
	m_isRestartNotifier = false;

	// 文字列の保存
	UpdateStageNameText();
	// 現在のレベル
	int level = m_stageManager.GetCurrentLevel() + 1;
	
	// char buf[32];
	// sprintf_s(buf, "STAGE %d", level);
	// SetUpStageText(buf);

	// チュートリアル判定と表示画像のセット
	isTutorialVisualOpen = false;
	currentTutorialPage = 0; // 常に1ページ目から開始

	// レベル(階層)によって出す画像を変える
	if (level == 1) // STAGE 1 (チュートリアル1：画像2枚 KickとKey)
	{
		isTutorialVisualOpen = true;
		maxTutorialPages = 2; // 2ページ構成
		tutorialImageIndices[0] = 1; // 1ページ目: Kick (helpImageHandles[1])
		tutorialImageIndices[1] = 2; // 2ページ目: Key  (helpImageHandles[2])
	}
	else if (level == 3) // STAGE 2 (チュートリアル2：画像1枚 Shadow)
	{
		isTutorialVisualOpen = true;
		maxTutorialPages = 1; // 1ページ構成
		tutorialImageIndices[0] = 0; // 1ページ目: Shadow (helpImageHandles[0])
	}
	else if (level == 6) // STAGE 3 (チュートリアル3：画像1枚 Trap)
	{
		isTutorialVisualOpen = true;
		maxTutorialPages = 1; // 1ページ構成
		tutorialImageIndices[0] = 3; // 1ページ目: Trap (helpImageHandles[3])
	}

	//sprintf_s(fontText, "STAGE %d", level);

	//// 文字の描画座標
	//int textWidth = GetDrawStringWidthToHandle(fontText, (int)strlen(fontText), font[0]);
	//fontPosX = (1280 - textWidth) / 2;
	//fontPosY = (720 - 100) / 2;

	for (GameObject* obj : allObjects)
	{
		if (obj == nullptr) continue;

		Warp* warpObj = dynamic_cast<Warp*>(obj);
		if (warpObj != nullptr)
		{
			warpObj->SetPlayer(&player); // ワープオブジェクトにプレイヤーを登録
		}
	}

	background = LoadGraph("Resource/Images/GameMain/background4.png");   // 背景画像

	// 音源関連・読み込み
	dieSE = LoadSoundMem("Resource/Sounds/SE/object/player/light_die01.mp3");
	freq = GetFrequencySoundMem(dieSE);
	ChangeVolumeSoundMem(70, dieSE);
	SetFrequencySoundMem((int)(freq * 1.0f), dieSE);

	delay = 0;
	tekazu = 0;

	//DrawNumber::SetImage(
	//	ResourceManager::GetInstance()->GetImages("Resource/Images/Number/number.png")
	//);
}

// 更新処理
eSceneType InGameScene::Update(const float& delta_second)
{
	tekazu = player.GetTekazu();
	InputManager* input = InputManager::GetInstance();

	// 手数を保存する変数
	sprintf_s(m_tekazuText, "%d", tekazu);

	// -------------------------------------------------------------
	// ① 一時停止（ポーズ）の入力・制御フェーズ（最優先）
	// -------------------------------------------------------------
	if (input->GetKeyInputState(KEY_INPUT_P) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_START) == eInputState::ePress)
	{
		isPaused = !isPaused;
		if (isPaused)
		{
			if (pauseBackgroundHandle != -1) DeleteGraph(pauseBackgroundHandle);
			pauseBackgroundHandle = MakeScreen(1280, 720, FALSE);
			GetDrawScreenGraph(0, 0, 1280, 720, pauseBackgroundHandle);
			GraphFilter(pauseBackgroundHandle, DX_GRAPH_FILTER_GAUSS, 16, 800);
			pauseSelectIndex = 0;
		}
	}

	if (isPaused)
	{
		// ヘルプ画面が開いている時のページ切り替え操作
		if (isHelpOpen)
		{
			// ◀ 左キー（またはDパッド左）で前のページに戻る
			if (input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress)
			{
				PlaySoundMem(pageSE, DX_PLAYTYPE_BACK); // 音を鳴らす
				currentHelpPage--;
				if (currentHelpPage < 0) currentHelpPage = 0; // 1枚目より前には戻らない
			}
			// ▶ 右キー（またはDパッド右）で次のページに進む
			if (input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress)
			{
				PlaySoundMem(pageSE, DX_PLAYTYPE_BACK); // 音を鳴らす
				currentHelpPage++;
				if (currentHelpPage >= MAX_HELP_PAGES) currentHelpPage = MAX_HELP_PAGES - 1; // 最後のページで止まる
			}

			// BackSpaceキー または コントローラーのAボタン でヘルプを閉じる処理
			if (input->GetKeyInputState(KEY_INPUT_BACK) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress)
			{
				isHelpOpen = false;
			}
		}
		// ヘルプが開いていない時だけ、いつものポーズメニューを動かす
		else
		{
			// 決定ボタンが押されて「押し込みアニメーション中」は、上下入力を受け付けないようにする
			if (pausePushTimer <= 0.0f)
			{
				if (input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress)
				{
					pauseSelectIndex--;
					if (pauseSelectIndex < 0) pauseSelectIndex = 3;
				}
				if (input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress)
				{
					pauseSelectIndex++;
					if (pauseSelectIndex > 3) pauseSelectIndex = 0;
				}

				// 決定ボタンが押されたら、まずは「押し込みタイマー」だけをセット！
				if (input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress || input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress)
				{
					pausePushTimer = 0.2f; // 0.2秒のアニメーション時間を確保
				}
			}
		}

		// 押し込みタイマーのカウントダウンと、時間が切れた時の画面遷移処理
		if (pausePushTimer > 0.0f)
		{
			pausePushTimer -= delta_second; // 毎フレームタイマーを減らす

			// 0.2秒が経過して、タイマーが0以下になった瞬間に本来の処理をドカンと実行！
			if (pausePushTimer <= 0.0f)
			{
				if (pauseSelectIndex == 0) isPaused = false;
				if (pauseSelectIndex == 1)
				{
					StopSoundMem(beepSE);
					Initialize();
					state = SceneState::Restarting;
					fade->Start(FadeType::IrisOut, true, 1.5f);
					isPaused = false;
				}

				// ヘルプ画面
				if (pauseSelectIndex == 2)
				{
					isHelpOpen = true;
					currentHelpPage = 0;
					helpOffsetY = 720.0f; 
				}

				if (pauseSelectIndex == 3)
				{
					StopSoundMem(mainBGM);
					StopSoundMem(beepSE);
					isPaused = false;
					return eSceneType::eTitle;
				}
			}
		}

		// アニメーション
		float targetHelpY = isHelpOpen ? 0.0f : 720.0f;
		helpOffsetY += (targetHelpY - helpOffsetY) * 0.1f;

		return GetNowSceneType();
	}


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
			Initialize();
			state = SceneState::StageNotifier;
			m_notifierTimer = 1.8f;

		}
		return GetNowSceneType();
	}


	if (state == SceneState::StageNotifier)
	{
		// カウントダウン開始
		m_notifierTimer -= delta_second;

		UpdateNotifierAlpha(0.3f, 0.3f);


		// 時間が来たら、文字を消してフェードイン(徐々に明るくして)
		if (m_notifierTimer <= 0.0f)
		{
			// IrisOutの引数をFalseにする
			fade->Start(FadeType::IrisOut, false, 1.2f);

			state = SceneState::Playing;		// 通常プレイにする
		}

		return GetNowSceneType();
	}

	// -------------------------------------------------------------
	// ② シーン基盤・基礎演出の更新フェーズ
	// -------------------------------------------------------------
	fade->Update(delta_second);

	if (CheckSoundMem(mainBGM) == 0) {
		PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP);
	}

	// -------------------------------------------------------------
	// ③ 特殊ステート制御（リスタート中・ステージ開始演出中）
	// -------------------------------------------------------------
	// フェードアウト中（リスタート待機中）の処理
	//if (state == SceneState::Restarting)
	//{
	//	if (fade->IsFinished()) {
	//		Initialize();
	//		state = SceneState::StageNotifier;
	//		m_notifierTimer = 1.8f;

	//	}
	//	return GetNowSceneType();
	//}

	/*if (state == SceneState::StageNotifier)
	{
		m_notifierTimer -= delta_second;
		if (m_notifierTimer <= 0.0f)
		{
			fade->Start(FadeType::IrisOut, false, 1.2f);
			state = SceneState::Playing;
		}
		return GetNowSceneType();
	}*/

	if (isTutorialVisualOpen && state == SceneState::Playing && fade->IsFinished())
	{
		// ◀ 左キーで前のページへ
		if (input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress ||
			input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress)
		{
			if (currentTutorialPage > 0)
			{
				currentTutorialPage--;
				PlaySoundMem(pageSE, DX_PLAYTYPE_BACK);
			}
		}

		// ▶ 右キーで次のページへ
		if (input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress ||
			input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress)
		{
			if (currentTutorialPage < maxTutorialPages - 1)
			{
				currentTutorialPage++;
				PlaySoundMem(pageSE, DX_PLAYTYPE_BACK);
			}
		}

		// Bボタンで閉じる（ゲーム開始）
		if (input->GetKeyInputState(KEY_INPUT_BACK) == eInputState::ePress ||
			input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress ||
			input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress)
		{
			isTutorialVisualOpen = false;
			PlaySoundMem(pageSE, DX_PLAYTYPE_BACK);
		}

		return GetNowSceneType();
	}

	// -------------------------------------------------------------
	// ④ 通常ゲームプレイ（オブジェクト・プレイヤー更新）フェーズ
	// -------------------------------------------------------------
	player.Update(delta_second);
	player.Move(allObjects);

	for (const auto& obj : allObjects) {
		if (obj != nullptr) {
			obj->Update(delta_second);
		}
	}

	

	// ステージクリア判定（プレイヤーが開いたゴールの中心座標に完全に重なった時）
	int playerX, playerY;
	player.GetLocation(playerX, playerY);

	for (const auto& obj : allObjects)
	{
		Goal* goalObj = dynamic_cast<Goal*>(obj);
		if (goalObj != nullptr && goalObj->IsOpen())
		{
			int goalX, goalY;
			goalObj->GetLocation(goalX, goalY);


			if (playerX == goalX && playerY == goalY)
			{
				goalObj->PlayGoalSE();
				m_stageManager.NextLevel();
				StopSoundMem(beepSE);

				// 「5」から「チュートリアルを含めた総ステージ数」に変更
				if (m_stageManager.GetCurrentLevel() >= 8)
				{
					StopSoundMem(mainBGM);
					return eSceneType::eResult;
				}

				UpdateStageNameText();

			//if (playerX == goalX && playerY == goalY)
			//{
			//	goalObj->PlayGoalSE();
			//	m_stageManager.NextLevel();
			//	StopSoundMem(beepSE);

			//	if (m_stageManager.GetCurrentLevel() >= 5) // 全ステージクリアならタイトルへ
			//	{
			//		StopSoundMem(mainBGM);
			//		return eSceneType::eResult;
			//	}

				state = SceneState::Restarting;
				fade->Start(FadeType::IrisOut, true, 1.5f);
				return GetNowSceneType();
			}
		}
	}

	// =============================================================
	// デバッグ用：Nキーが押されたら強制的に次のステージへ
	// =============================================================
	if (input->GetKeyInputState(KEY_INPUT_N) == eInputState::ePress)
	{ 
		m_stageManager.NextLevel(); // ステージの内部進行度を次に進める
		StopSoundMem(beepSE);       

		if (m_stageManager.GetCurrentLevel() >= 8)
		{
			StopSoundMem(mainBGM);
			return eSceneType::eResult;
		}

		state = SceneState::Restarting;
		fade->Start(FadeType::IrisOut, true, 1.5f);
		return GetNowSceneType();
	}

	// -------------------------------------------------------------
	// ⑤ 罠（カメラ・ライト）の検知判定フェーズ
	// -------------------------------------------------------------
	bool isCamDetected = false;
	bool isLightDetected = false;

	for (auto d : detectors) {
		d->Update(player, delta_second);
		if (d->IsDetected()) {
			if (d->GetType() == TrapType::Camera) isCamDetected = true;
			else if (d->GetType() == TrapType::Light && player.GetState() == Player::State::Shadow) isLightDetected = true;
		}
	}

	if (isCamDetected) {
		if (CheckSoundMem(beepSE) == 0) PlaySoundMem(beepSE, DX_PLAYTYPE_LOOP);
	}
	else {
		if (CheckSoundMem(beepSE) == 1) StopSoundMem(beepSE);
	}

	// -------------------------------------------------------------
	// ⑥ 状態管理＆ペナルティ（失敗確定）判定フェーズ
	// -------------------------------------------------------------
	if (isCamDetected || isLightDetected) {
		if (state == SceneState::Playing) {
			state = SceneState::Detected;
			detectionTimer = 0.0f;
		}
		if (isLightDetected) {
			PlaySoundMem(dieSE, DX_PLAYTYPE_BACK);
			detectionTimer = LIMIT_TIME;
		}
		else {
			detectionTimer += delta_second;
		}

		if (detectionTimer >= LIMIT_TIME) {
			StopSoundMem(beepSE);
			state = SceneState::Restarting;
			if (isLightDetected || player.GetTekazu() == 0) fade->Start(FadeType::Normal, true, 0.8f);
			else fade->Start(FadeType::IrisOut, true, 1.0f);
		}
	}
	else {
		if (state == SceneState::Detected) {
			state = SceneState::Playing;
			detectionTimer = 0.0f;
		}
	}

	// -------------------------------------------------------------
	// ⑦ 手数ゼロによるゲームオーバー判定
	// -------------------------------------------------------------
	if (player.GetTekazu() == 0 && state != SceneState::Restarting)
	{
		StopSoundMem(mainBGM);
		StopSoundMem(beepSE);
		state = SceneState::Restarting;
		fade->Start(FadeType::IrisOut, true, 1.0f);
	}

	return GetNowSceneType();
}

// 描画処理
void InGameScene::Draw() const
{
	// ベースの黒背景
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

	// ステージ床タイルを敷き詰める
	m_stageManager.DrawFloorBackground();

	SetFontSize(20);

	// 追加したオブジェクト（壁やブロック、各種罠）を全て描画
	for (const auto& obj : allObjects) {
		if (obj != nullptr) {
			obj->Draw();
		}
	}

	// 手数（スコア）の描画
	// DrawNumber::Draw(230, 550, tekazu, 0.8f);
	DrawStringToHandle(230 + 2, 550 + 2, m_tekazuText, GetColor(30, 30, 30), font[0]); // 影
	DrawStringToHandle(230, 550, m_tekazuText, GetColor(255, 255, 255), font[0]);     // 本尊


	//DrawStringToHandle(40, 450, "STAGE1", GetColor(255, 255, 255), font[0]);

	/*for (auto& wall : walls)
	{
		wall.Draw();
	}

	for (auto& block : blocks)
	{
		block.Draw();
	}*/

	//goal.Draw();			// これはいらない
	// プレイヤーの描画
	player.Draw();

	// 罠（カメラ・ライト）の可視エリアなどの追加描画が必要な場合は、ここで obj->Draw() とは別に d->Draw() を回してください。

	// 猶予期間（検知中）の画面赤点滅演出
	if (state == SceneState::Detected) {
		bool isCameraDetecting = false;
		for (auto d : detectors) {
			if (d->GetType() == TrapType::Camera && d->IsDetected()) {
				isCameraDetecting = true;
				break;
			}
		}

		if (isCameraDetecting) {
			if ((int)(detectionTimer * 2) % 2 == 0) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
				DrawBox(0, 0, 1280, 720, GetColor(255, 0, 0), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}

	// フェードを最前面に描画
	if (fade) fade->Draw();

	// ステージ切り替え時の開始通知表示
	if (state == SceneState::StageNotifier)
	{
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_notifierAlpha);

		DrawStringToHandle(fontPosX + 4, fontPosY + 4, fontText, GetColor(30, 30, 30), font[0]); // 影
		DrawStringToHandle(fontPosX, fontPosY, fontText, GetColor(255, 255, 255), font[0]);      // 本尊

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// チュートリアル画像の描画
    // チュートリアル画像の描画（条件をUpdateと合わせる）
	if (isTutorialVisualOpen && state == SceneState::Playing && fade->IsFinished())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int helpWidth = 960;
		int helpHeight = 540;
		int x1 = (1280 - helpWidth) / 2 + 5;
		int y1 = (720 - helpHeight) / 2 - 20;

		int drawImageIndex = tutorialImageIndices[currentTutorialPage];

		int origW, origH;
		GetGraphSize(helpImageHandles[drawImageIndex], &origW, &origH);
		double scaleX = (double)helpWidth / origW;
		double scaleY = (double)helpHeight / origH;

		DrawRotaGraph3(x1 + helpWidth / 2, y1 + helpHeight / 2, origW / 2, origH / 2, scaleX, scaleY, 0.0, helpImageHandles[drawImageIndex], TRUE);

		if (maxTutorialPages > 1)
		{
			DrawFormatStringToHandle(665 - 45, y1 + helpHeight + 15, GetColor(255, 255, 255), font[2], "%d / %d", currentTutorialPage + 1, maxTutorialPages);

			const char* guideText = "十字ボタン: ページ切替 / Bボタン: ゲーム開始";
			int textW = GetDrawStringWidthToHandle(guideText, (int)strlen(guideText), font[1]);
			DrawStringToHandle((1280 - textW) / 2, y1 + helpHeight + 70, guideText, GetColor(255, 255, 255), font[1]);
		}
		else
		{
			const char* guideText = "Bボタンでゲーム開始";
			int textW = GetDrawStringWidthToHandle(guideText, (int)strlen(guideText), font[1]);
			DrawStringToHandle((1280 - textW) / 2, y1 + helpHeight + 15, guideText, GetColor(255, 255, 255), font[1]);
		}
	}

	// ポーズ画面の描画処理
	if (isPaused)
	{
		DrawGraph(0, 0, pauseBackgroundHandle, FALSE);

		int menuLeft = 640 - 200;
		int menuTop = 360 - 180;
		int menuBottom = 360 + 180;
		int menuRight = 640 + 200;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(menuLeft, menuTop, menuRight, menuBottom, GetColor(15, 15, 15), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(menuLeft, menuTop, menuRight, menuBottom, GetColor(255, 255, 255), FALSE);

		// ポーズのタイトルをカスタムフォント化
		DrawStringToHandle(660 - 80, 200, "- PAUSE -", GetColor(255, 255, 255), font[1]);

		// 既存の c0 〜 c3 の色判定の下に追加
		unsigned int c0 = (pauseSelectIndex == 0) ? GetColor(255, 220, 0) : GetColor(200, 200, 200);
		unsigned int c1 = (pauseSelectIndex == 1) ? GetColor(255, 220, 0) : GetColor(200, 200, 200);
		unsigned int c2 = (pauseSelectIndex == 2) ? GetColor(255, 220, 0) : GetColor(200, 200, 200);
		unsigned int c3 = (pauseSelectIndex == 3) ? GetColor(255, 220, 0) : GetColor(200, 200, 200);

		// 押し込みアニメーション用の変数を準備
		int pushOffsetY = 0; // 下にずらす量
		unsigned int activeCursorColor = GetColor(255, 220, 0);

		// ボタンが押されている間（タイマー作動中）は、見た目を変化させる
		if (pausePushTimer > 0.0f)
		{
			pushOffsetY = 4;                 // 項目とカーソルを 4ピクセル 下に沈ませる
			activeCursorColor = GetColor(120, 100, 0); // カーソルの黄色を暗くする

			// 現在選ばれている項目の文字色をグレーにして「消灯感」を出す
			if (pauseSelectIndex == 0) c0 = GetColor(100, 100, 100);
			if (pauseSelectIndex == 1) c1 = GetColor(100, 100, 100);
			if (pauseSelectIndex == 2) c2 = GetColor(100, 100, 100);
			if (pauseSelectIndex == 3) c3 = GetColor(100, 100, 100);
		}

		int textLeftX = 640 - 100;

		// 各項目のベースとなる文字を描画
		DrawStringToHandle(textLeftX, 270 + (pauseSelectIndex == 0 ? pushOffsetY : 0), "ゲームに戻る", c0, font[1]);
		DrawStringToHandle(textLeftX, 320 + (pauseSelectIndex == 1 ? pushOffsetY : 0), "リスタート", c1, font[1]);
		DrawStringToHandle(textLeftX, 370 + (pauseSelectIndex == 2 ? pushOffsetY : 0), "ヘルプ", c2, font[1]);
		DrawStringToHandle(textLeftX, 420 + (pauseSelectIndex == 3 ? pushOffsetY : 0), "タイトルに戻る", c3, font[1]);

		// 現在選んでいる項目の Y座標（pushOffsetY をプラス！）
		int cursorY = 270 + (pauseSelectIndex * 50) + pushOffsetY;

		// 今選ばれている項目の文字列と横幅を取得
		const char* currentMenuText = "";
		if (pauseSelectIndex == 0) currentMenuText = "ゲームに戻る";
		if (pauseSelectIndex == 1) currentMenuText = "リスタート";
		if (pauseSelectIndex == 2) currentMenuText = "ヘルプ";
		if (pauseSelectIndex == 3) currentMenuText = "タイトルに戻る";

		int textWidth = GetDrawStringWidthToHandle(currentMenuText, (int)strlen(currentMenuText), font[1]);

		// カーソル描画
		DrawStringToHandle(textLeftX - 30, cursorY, ">", activeCursorColor, font[1]);
		DrawStringToHandle(textLeftX + textWidth + 15, cursorY, "<", activeCursorColor, font[1]);
		
		// ヘルプ描画
		if (helpOffsetY < 720.0f)
		{
			// 画面サイズ
			int helpWidth = 960;
			int helpHeight = 540;

			// 画面中央に配置するための座標を計算
			int x1 = (1280 - helpWidth) / 2 + 5;
			int y1 = (720 - helpHeight) / 2 + (int)helpOffsetY -20;
			int x2 = x1 + helpWidth;
			int y2 = y1 + helpHeight;

			// 角度の計算
			float degree = -0.2f;
			double angle = degree * (3.1415926f / 180.0f);

			// 元の画像サイズを取得して倍率を出す
			int origW, origH;
			GetGraphSize(helpImageHandles[currentHelpPage], &origW, &origH);
			double scaleX = (double)helpWidth / origW;
			double scaleY = (double)helpHeight / origH;

			// 画像描画
			DrawRotaGraph3(x1 + helpWidth / 2, y1 + helpHeight / 2, origW / 2, origH / 2, scaleX, scaleY, angle, helpImageHandles[currentHelpPage], FALSE);

			// ページ数の描画
			DrawFormatStringToHandle(665 - 45, y2 + 15, GetColor(255, 255, 255), font[2], "%d / 4", currentHelpPage + 1);
		}
	} 
} 

// 終了時処理
void InGameScene::Finalize()
{
	if (pauseBackgroundHandle != -1)
	{
		DeleteGraph(pauseBackgroundHandle);
		pauseBackgroundHandle = -1;
	}

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

	allObjects.clear();
}

void InGameScene::UpdateStageNameText()
{
	int currentLevel = m_stageManager.GetCurrentLevel();
	char buf[32];

	switch (currentLevel)
	{
		case 0:
			sprintf_s(buf, "チュートリアル");
			break;
		case 1:
			sprintf_s(buf, "Stage 1");
			break;
		case 2:
			sprintf_s(buf, "チュートリアル");
			break;
		case 3:
			sprintf_s(buf, "Stage 2");
			break;
		case 4:
			sprintf_s(buf, "Stage 3");
			break;
		case 5:
			sprintf_s(buf, "Stage 4");
			break;
		case 6:
			sprintf_s(buf, "Stage 5");
			break;
		case 7:
			sprintf_s(buf, "Stage");
			break;
		case 8:
			break;
			sprintf_s(buf, "Stage %d", currentLevel + 1);
		default:
			break;
	}

	SetUpStageText(buf);
}

void InGameScene::SetUpStageText(const char* text)
{
	// 文字列をコピー
	sprintf_s(fontText, "%s", text);

	// 座標を計算(画面の中心)
	int textWidth = GetDrawStringWidthToHandle(fontText, (int)strlen(fontText), font[0]);
	fontPosX = (1280 - textWidth) / 2;
	fontPosY = (720 - 100) / 2;

	// 透明度をリセット
	m_notifierAlpha = 0;
}

void InGameScene::UpdateNotifierAlpha(float fadeInTime, float fadeOutTime)
{
	m_notifierAlpha = 255;		// 最初は不透明

	// フェードイン
	if (m_notifierTimer > (1.8f - fadeInTime))
	{
		// 1.8秒の時に0、(1.8 - fadeInTime)秒の時に1.0になる割合を計算
		float progress = (1.8f - m_notifierTimer) / fadeInTime;
		m_notifierAlpha = (int)(progress * 255.0f);
	}
	// フェードアウト
	else if (m_notifierTimer < fadeOutTime)
	{
		// fadeOutTime秒の時に1.0、0秒の時に0になる割合を計算
		float progress = m_notifierTimer / fadeOutTime;
		m_notifierAlpha = (int)(progress * 255.0f);
	}

	// 範囲外に行かないようにする
			// 範囲外に行かないようにする
	if (m_notifierAlpha < 0)
	{
		m_notifierAlpha = 0;
	}
	if (m_notifierAlpha > 255)
	{
		m_notifierAlpha = 255;
	}

}

// 現在のシーン情報を返す
eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}