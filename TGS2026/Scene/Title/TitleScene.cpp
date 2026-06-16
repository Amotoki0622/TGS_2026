#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

// コンストラクタ
TitleScene::TitleScene()
{
    font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 125, 6);
    font[1] = CreateFontToHandle("廻想体 ネクスト UP B", 75, 6);
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
	menu_cursor = 0;

    //追加
    background = LoadGraph("Resource/Images/Title.png");
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
    if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress)
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
    //追加
    DrawGraph(0, 0, background, TRUE);

	/*SetFontSize(20);*/
    DrawStringToHandle(545,250,"TITLE", 0xffffff, font[0]);
	/*DrawString(625, 180, "TITLE", 0xffffff);*/
    
    // メニュー
    DrawStringToHandle(575, 550, "START", 0xffffff, font[1]);
    /*DrawString(625, 250, "START", 0xffffff);*/
    /*DrawString(630, 300, "END", 0xffffff);*/

    // =========================
    // カーソル表示（→）
    // =========================
    int cursorY = 250 + (menu_cursor * 50);

    DrawStringToHandle(535, 550, ">", 0xff0000, font[1]);
    DrawStringToHandle(745, 550, "<", 0xff0000, font[1]);
    //DrawString(575, cursorY, ">", 0xffff00); // 黄色カーソル
    //DrawString(725, cursorY, "<", 0xffff00); // 黄色カーソル
}

// 終了時処理
void TitleScene::Finalize()
{
    //追加
    DeleteGraph(background);
}

// 現在のシーン情報を返す
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
