#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

// コンストラクタ
TitleScene::TitleScene()
{
    font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 125, 6);
    font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 45, 6);

    // タイトルメインBGM
    title_main_bgm = LoadSoundMem("Resource/Sounds/BGM/title/title_main_bgm.mp3");
    // 音量を設定
    ChangeVolumeSoundMem(180, title_main_bgm);

    // スタート時音声
    title_start_se = LoadSoundMem("Resource/Sounds/SE/title/title_start_se.mp3");
    // 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
    ChangeVolumeSoundMem(130, title_start_se);
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
    is_selected = false;
    transition_timer = 0.0f;

    animation_time = 0.0f;

    // 背景画像
    background = LoadGraph("Resource/Images/Title.png");
}

// 更新処理
eSceneType TitleScene::Update(const float& delta_second)
{
    // 音の再生
    if (CheckSoundMem(title_main_bgm) == 0) {
        // ループ再生
        PlaySoundMem(title_main_bgm, DX_PLAYTYPE_LOOP);
    }

    // アニメーション用のタイマーを毎フレーム進める
    animation_time += delta_second;

	InputManager* input = InputManager::GetInstance();

    if (!is_selected)
    {
        // 決定（コントローラーB または キーボードEnter）
        if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
            input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
        {
            PlaySoundMem(title_start_se, DX_PLAYTYPE_BACK); // 音を鳴らす
            is_selected = true;                             // 「ボタン押したよ」状態にする
        }
    }
    // ボタンが押された後の処理
    else
    {
        transition_timer += delta_second; // 毎フレーム、秒数を足していく

        // 2.0秒 経ったら、ついに画面をインゲームに切り替える！
        //（好みの長さに数値を調整してください。1.5f や 3.0f など）
        if (transition_timer >= 2.0f)
        {
            return eSceneType::eInGame;
        }
    }

    // 2.0秒経つまでは、ずっとタイトルシーンを維持する
    return eSceneType::eTitle;
}


// 描画処理
void TitleScene::Draw() const
{
    // 背景画像描画
    DrawGraph(0, 0, background, TRUE);

    // タイトル
    DrawStringToHandle(360,250,"DUAL ESCAPE", 0xffffff, font[0]);
    
    //  通常時の色（STARTとカーソル）
    unsigned int startTextColor = 0xffffff;   // 通常は白
    unsigned int cursorColor = 0xff0000;      // 通常は鮮やかな赤
    int pushOffsetY = 0;                      // 通常はズレなし

    if (is_selected == false)
    {
        int alpha = 177 + (int)(sin(animation_time * 3.0f) * 78.0f);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    }
    else if (is_selected == true && transition_timer < 0.6f)
    {
        startTextColor = 0x888888;
        cursorColor = 0x990000;
        pushOffsetY = 4;
    }

    // スタート
    DrawStringToHandle(550, 550 + pushOffsetY, "GAME START", startTextColor, font[1]);

    int cursorPulse = (int)(sin(animation_time * 5.0f) * 8.0f);

    // カーソル表示
    DrawStringToHandle(515 + cursorPulse, 550 + pushOffsetY, ">", cursorColor, font[1]);
    DrawStringToHandle(778 - cursorPulse, 550 + pushOffsetY, "<", cursorColor, font[1]);

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //// ボタンが押されてからの色（STARTとカーソル）
    //if (is_selected && transition_timer < 0.6f) // 押し込まれた時の時間調整
    //{
    //    startTextColor = 0x888888;    // 押し込まれた暗いグレー
    //    cursorColor = 0x990000;       // 押し込まれた暗い赤
    //    pushOffsetY = 4;              // 4ピクセル下に下げる
    //}

    //// スタート
    //DrawStringToHandle(556, 550, "GAME START", startTextColor, font[1]);


    //// カーソル表示
    //DrawStringToHandle(515, 550, ">", cursorColor, font[1]);
    //DrawStringToHandle(778, 550, "<", cursorColor, font[1]);
}

// 終了時処理
void TitleScene::Finalize()
{
    DeleteGraph(background);

    //DeleteFontToHandle(font[0]);
    //DeleteFontToHandle(font[1]);

    StopSoundMem(title_main_bgm);
}

// 現在のシーン情報を返す
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
