#include "SignBoard.h"
#include "../Player/Player.h"
#include "../../Utility/EffectManager/EffectManager.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"
#include <cmath>

// 💡 変更点：コンストラクタの引数をヘッダーに合わせて変更
SignBoard::SignBoard(float x, float y, float width, float height,
    int signboardId, int signboardGraphic)
{
    // 親クラス（GameObject）のメンバにセット
    this->location.x = x;
    this->location.y = y;
    this->box_size.x = width;
    this->box_size.y = height;

    this->graphic_images = signboardGraphic;
    this->flip_flag = FALSE;

    this->currentState = State::None;
    this->detectRadius = 0.0f;
    this->animTimer = 0.0f;
    this->currentPageIndex = 0;

    // 💡 外部からセットされるまでは一旦安全に nullptr で初期化
    this->targetPlayer = nullptr;
    this->effectManager = nullptr;

    // ID（ステージ番号）ごとにページを登録
    switch (signboardId)
    {
    case 1:
        this->explanationImageHandles.push_back(LoadGraph("Resource/Images/Description/stage01_01.png"));
        this->explanationImageHandles.push_back(LoadGraph("Resource/Images/Description/stage01_02.png"));
        break;
    default:
        this->explanationImageHandles.push_back(LoadGraph("Resource/Images/Description/page_default.png"));
        break;
    }

    this->nextIconHandle = -1;
}

SignBoard::~SignBoard()
{
    this->Finalize();
}

void SignBoard::Initialize()
{
    this->currentState = State::None;
    this->currentPageIndex = 0;
}

void SignBoard::Update(float delta_second)
{
    // 💡 ポインタがセットされていない安全対策（エラー落ち防止）
    if (targetPlayer == nullptr || effectManager == nullptr) return;

    InputManager* input = InputManager::GetInstance();
    animTimer += 5.0f * delta_second;

    int px, py;
    targetPlayer->GetLocation(px, py);
    float dx = (float)px - location.x;
    float dy = (float)py - location.y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (currentState != State::Showing)
    {
        if (distance <= detectRadius)
        {
            currentState = State::Near;

            // 近くにいる間、Aボタンエフェクトを発注
            effectManager->AddEffect(location.x, location.y - 50.0f,
                EffectType::ActionUI,
                "Resource/Images/Object/ui_a_btn.png");
        }
        else
        {
            currentState = State::None;
        }

        // Aボタンかスペースキーで説明画面を開く
        if (currentState == State::Near &&
            (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress ||
                input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress))
        {
            currentState = State::Showing;
            currentPageIndex = 0;
        }
    }
    else
    {
        // 説明画面表示中：下キー、S、A、スペースで次のページへ！
        if (input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress ||
            input->GetKeyInputState(KEY_INPUT_S) == eInputState::ePress ||
            input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress ||
            input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
        {
            currentPageIndex++;

            // もし登録されている最大ページ数を超えたら、説明を終了してゲームに戻る
            if (currentPageIndex >= explanationImageHandles.size())
            {
                currentState = State::Near;
            }
        }
    }
}

void SignBoard::Draw() const
{
    // 1. 看板自体の描画
    if (graphic_images != -1)
    {
        GameObject::Draw();
    }
    else
    {
        int left = (int)(location.x - box_size.x * 0.5f);
        int top = (int)(location.y - box_size.y * 0.5f);
        int right = (int)(location.x + box_size.x * 0.5f);
        int bottom = (int)(location.y + box_size.y * 0.5f);
        DrawBox(left, top, right, bottom, GetColor(139, 69, 19), TRUE);
        DrawBox(left, top, right, bottom, GetColor(255, 255, 255), FALSE);

        SetFontSize(16);
        DrawString(left + 10, top + 20, "SIGN", GetColor(255, 255, 255));
    }

    // 2. 近くにいる時の「仮AボタンUI」
    if (currentState == State::Near)
    {
        int offsetY = (int)(sinf(animTimer) * 5.0f) - 50;
        DrawCircle((int)location.x, (int)location.y + offsetY, 15, GetColor(255, 220, 0), TRUE);
        DrawCircle((int)location.x, (int)location.y + offsetY, 15, GetColor(0, 0, 0), FALSE);
        SetFontSize(16);
        DrawString((int)location.x - 5, (int)location.y + offsetY - 8, "A", GetColor(0, 0, 0));
    }

    // 3. 説明表示中の描画（全画面）
    if (currentState == State::Showing)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        int currentHandle = explanationImageHandles[currentPageIndex];

        if (currentHandle != -1)
        {
            DrawExtendGraph(140, 60, 1140, 620, currentHandle, TRUE);
        }
        else
        {
            DrawBox(140, 60, 1140, 620, GetColor(30, 50, 100), TRUE);
            DrawBox(140, 60, 1140, 620, GetColor(255, 255, 255), FALSE);

            SetFontSize(40);
            DrawFormatString(430, 300, GetColor(255, 255, 255), "【 せつめい画像 (ページ %d) 】", currentPageIndex + 1);

            SetFontSize(20);
            DrawString(460, 380, "ボタンを押すと次のページ、または閉じます", GetColor(200, 200, 200));
        }

        // 4. ▼アイコンを図形（三角形）で描画
        int arrowOffsetY = (int)(sinf(animTimer) * 5.0f);
        int centerX = 640;
        int centerY = 650 + arrowOffsetY;

        int x1 = centerX - 20, y1 = centerY - 10;
        int x2 = centerX + 20, y2 = centerY - 10;
        int x3 = centerX, y3 = centerY + 15;

        DrawTriangle(x1, y1, x2, y2, x3, y3, GetColor(255, 255, 255), TRUE);
        DrawTriangle(x1, y1, x2, y2, x3, y3, GetColor(0, 0, 0), FALSE);
    }
}

void SignBoard::Finalize()
{
    for (int handle : explanationImageHandles)
    {
        if (handle != -1) DeleteGraph(handle);
    }
    explanationImageHandles.clear();

    if (nextIconHandle != -1) DeleteGraph(nextIconHandle);
}