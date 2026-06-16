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
    ChangeVolumeSoundMem(55, title_main_bgm);

    //// スタート時音声
    //title_start_se = LoadSoundMem("Resource/Sounds/SE/object/cam/cam3.mp3");
    //// 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
    //ChangeVolumeSoundMem(70, title_start_se);
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
	menu_cursor = 0;

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

	InputManager* input = InputManager::GetInstance();


    // 決定（コントローラー）
    if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress)
    {
         return eSceneType::eInGame;
    }

    // 決定（キーボード）
    if (input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
    {
        return eSceneType::eInGame;
    }

    return eSceneType::eTitle;
}

// 描画処理
void TitleScene::Draw() const
{
    // 背景画像描画
    DrawGraph(0, 0, background, TRUE);

    // タイトル
    DrawStringToHandle(545,250,"TITLE", 0xffffff, font[0]);
    
    // スタート
    DrawStringToHandle(556, 550, "GAME START", 0xffffff, font[1]);


    // カーソル表示
    DrawStringToHandle(515, 550, ">", 0xff0000, font[1]);
    DrawStringToHandle(778, 550, "<", 0xff0000, font[1]);
}

// 終了時処理
void TitleScene::Finalize()
{
    DeleteGraph(background);
}

// 現在のシーン情報を返す
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
