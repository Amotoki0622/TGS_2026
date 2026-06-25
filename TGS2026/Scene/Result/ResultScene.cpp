#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

// コンストラクタ
ResultScene::ResultScene()
{
	font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 125, 6);
	font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 75, 6);

	//リザルトBGM
	result_bgm = LoadSoundMem("Resource/Sounds/BGM/result/result_bgm.mp3");
	//音量調整
	ChangeVolumeSoundMem(180, result_bgm);

	//リザルトSE
	result_se = LoadSoundMem("Resource/Sounds/SE/result/result_se.mp3");
	//音量調整
	/*ChangeVolumeSoundMem(130, result_se);*/
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 初期化処理
void ResultScene::Initialize()
{
	is_selected = false;
	transition_timer = 0.0f;

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

	/*return GetNowSceneType();*/
}

// 描画処理
void ResultScene::Draw() const
{
	DrawGraph(0, 0, background, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)clearAlpha);
	DrawStringToHandle(350, 100, "GAME CLEAR", 0xffffff, font[0]);

	unsigned int resultTextColor = 0xffffff;
	unsigned int cursorColor = 0xff0000;
	int pushOffsetY = 0;

	if (is_selected && transition_timer < 0.6f)
	{
		resultTextColor = 0x888888;
		cursorColor = 0x990000;
		pushOffsetY = 4;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)backAlpha);
	DrawStringToHandle(456, 550, "BACK TO TITLE", resultTextColor, font[1]);

	DrawStringToHandle(410, 550, ">", cursorColor, font[1]);
	DrawStringToHandle(850, 550, "<", cursorColor, font[1]);
}

// 終了時処理
void ResultScene::Finalize()
{
	DeleteGraph(background);

	DeleteFontToHandle(font[0]);
	DeleteFontToHandle(font[1]);

	StopSoundMem(result_bgm);
}

// 現在のシーン情報を返す
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
