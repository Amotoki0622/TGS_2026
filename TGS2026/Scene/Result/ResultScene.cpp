#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

#include <math.h>

// コンストラクタ
ResultScene::ResultScene()
{
	font[0] = -1;
	font[1] = -1;
	result_bgm = -1;
	result_se = -1;
	background = -1;

	//font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 125, 6);
	//font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 75, 6);

	////リザルトBGM
	//result_bgm = LoadSoundMem("Resource/Sounds/BGM/result/result_bgm.mp3");
	////音量調整
	//ChangeVolumeSoundMem(180, result_bgm);

	////リザルトSE
	//result_se = LoadSoundMem("Resource/Sounds/SE/result/result_se.mp3");
	////音量調整
	///*ChangeVolumeSoundMem(130, result_se);*/
}

// デストラクタ
ResultScene::~ResultScene()
{
	Finalize();
}

// 初期化処理
void ResultScene::Initialize()
{
	is_selected = false;
	transition_timer = 0.0f;

	animation_time = 0.0f;
	clearAlpha = 0.0f;
	backAlpha = 0.0f;

	font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 125, 6);
	font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 75, 6);

	// リザルトBGM
	result_bgm = LoadSoundMem("Resource/Sounds/BGM/result/result_bgm.mp3");
	ChangeVolumeSoundMem(180, result_bgm);

	// リザルトSE
	result_se = LoadSoundMem("Resource/Sounds/SE/result/result_se.mp3");
	/*ChangeVolumeSoundMem(130, result_se);*/

	background = LoadGraph("Resource/Images/result.png");
}

// 更新処理
eSceneType ResultScene::Update(const float& delta_second)
{
	//音の再生
	if (CheckSoundMem(result_bgm) == 0)
	{
		//ループ再生
		PlaySoundMem(result_bgm, DX_PLAYTYPE_LOOP);
	}

	animation_time += delta_second;

	if (clearAlpha < 255)
	{
		clearAlpha += 128 * delta_second;
	}
	else if (backAlpha < 255)
	{
		backAlpha += 128 * delta_second;
	}

	InputManager* input = InputManager::GetInstance();

	if (!is_selected)
	{
		//決定(コントローラーBまたはキーボードEnter)
		if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
			input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
		{
			PlaySoundMem(result_se, DX_PLAYTYPE_BACK); //音を鳴らす
			is_selected = true;
		}
	}
	else
	{
		transition_timer += delta_second;

		if (transition_timer >= 2.0f)
		{
			return eSceneType::eTitle;
		}
	}

	return eSceneType::eResult;

	return GetNowSceneType();
}

// 描画処理
void ResultScene::Draw() const
{
	// 背景画像
	DrawGraph(0, 0, background, TRUE);

	int titleX = 350;		// 元X座標
	int titleY = 100;		// 元Y座標

	// 左上に赤丸を上から描画
	/*if (fmod(animation_time, 1.0f) < 0.5f)
	{
		DrawCircle(112, 60, 6, GetColor(255, 0, 0), TRUE);
	}*/

	// フェードインが完了次第ノイズ演出
	if (clearAlpha >= 255.0f)
	{
		// 数秒に一瞬ノイズ演出
		if ((int)(animation_time * 2.5f) % 4 == 0 && (int)(animation_time * 20.0f) % 6 == 0)
		{
			titleX += (int)(sin(animation_time * 100) * 10.0f);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)clearAlpha);
	DrawStringToHandle(titleX, titleY, "GAME CLEAR", 0xffffff, font[0]);

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)clearAlpha);
	//DrawStringToHandle(350, 100, "GAME CLEAR", 0xffffff, font[0]);

	unsigned int resultTextColor = 0xffffff;
	unsigned int cursorColor = 0xff0000;
	int pushOffsetY = 0;

	// 色・座標判定
	if (is_selected && transition_timer < 0.6f)
	{
		resultTextColor = 0x888888;
		cursorColor = 0x990000;
		pushOffsetY = 4;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)backAlpha);
	DrawStringToHandle(456, 550, "BACK TO TITLE", resultTextColor, font[1]);

	int cursorPulse = 0;
	if (backAlpha >= 255.0f)
	{
		cursorPulse = (int)(sin(animation_time * 5.0f) * 8.0f);
	}

	// カーソル表示
	DrawStringToHandle(410 + cursorPulse, 550 + pushOffsetY, ">", cursorColor, font[1]);
	DrawStringToHandle(850 - cursorPulse, 550 + pushOffsetY, "<", cursorColor, font[1]);

	// DrawStringToHandle(410, 550, ">", cursorColor, font[1]);
	// DrawStringToHandle(850, 550, "<", cursorColor, font[1]);
}

// 終了時処理
void ResultScene::Finalize()
{
	if (background != -1) {
		DeleteGraph(background);
		background = -1;
	}

	if (font[0] != -1) {
		DeleteFontToHandle(font[0]);
		font[0] = -1;
	}
	if (font[1] != -1) {
		DeleteFontToHandle(font[1]);
		font[1] = -1;
	}

	if (result_bgm != -1) {
		StopSoundMem(result_bgm);
		DeleteSoundMem(result_bgm);
		result_bgm = -1;
	}

	if (result_se != -1) {
		StopSoundMem(result_se);
		DeleteSoundMem(result_se); 
		result_se = -1;
	}

}

// 現在のシーン情報を返す
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
