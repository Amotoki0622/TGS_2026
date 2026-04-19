#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

// コンストラクタ
TitleScene::TitleScene()
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
	menu_cursor = 0;
}

// 更新処理
eSceneType TitleScene::Update(const float& delta_second)
{
	InputManager* input = InputManager::GetInstance();

    // 上
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress)
    {
        menu_cursor--;
        if (menu_cursor < 0) menu_cursor = 1;
    }

    // 下
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress)
    {
        menu_cursor++;
        if (menu_cursor > 1) menu_cursor = 0;
    }

    // 決定
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress)
    {
        switch (menu_cursor)
        {
        case 0: return eSceneType::eInGame;
        case 1: return eSceneType::eEnd;
        }
    }


    // ===============================
    // 上（キーボード）
    // ===============================
    if (input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress)
    {
        menu_cursor--;
        if (menu_cursor < 0) menu_cursor = 1;
    }

    // ===============================
    // 下（キーボード）
    // ===============================
    if (input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress)
    {
        menu_cursor++;
        if (menu_cursor > 1) menu_cursor = 0;
    }

    // ===============================
    // 決定（キーボード）
    // ===============================
    if (input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
    {
        switch (menu_cursor)
        {
        case 0: return eSceneType::eInGame;
        case 1: return eSceneType::eEnd;
        }
    }
    return eSceneType::eTitle;
}

// 描画処理
void TitleScene::Draw() const
{
	SetFontSize(20);
	DrawString(10, 10, "TITLE", 0xffffff);
    
    // メニュー
    DrawString(500, 250, "START", 0xffffff);
    DrawString(500, 300, "END", 0xffffff);

    // =========================
    // カーソル表示（→）
    // =========================
    int cursorY = 250 + (menu_cursor * 50);

    DrawString(450, cursorY, ">", 0xffff00); // 黄色カーソル
}

// 終了時処理
void TitleScene::Finalize()
{
}

// 現在のシーン情報を返す
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
