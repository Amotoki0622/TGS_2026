#include "Player.h"
#include "../../Utility/InputManager.h"

// 初期化処理
void Player::Initialize()
{
    // 初期出現位置
    x = 1180;
    y = 120;

    radius = 30;
    speed = 150; // 1マス分

    state = State::Normal;

    UpdateColor();
}

// 更新処理
void Player::Update()
{
    ChangeState(); // 状態切り替え
    Move();        // 移動
    UpdateColor(); // 見た目更新
}

// 移動処理
void Player::Move()
{
    InputManager* input = InputManager::GetInstance();

    int moveX = 0;
    int moveY = 0;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress)
        moveY = -speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress)
        moveY = speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress)
        moveX = -speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress)
        moveX = speed;

    // 状態ごとの処理（今は同じ）
    switch (state)
    {
    case State::Normal:
        x += moveX;
        y += moveY;
        break;

    case State::Shadow:
        x += moveX;
        y += moveY;
        break;
    }

    // 画面外制限
    if (x < radius) x = radius;
    if (x > 1280 - radius) x = 1280 - radius;

    if (y < radius) y = radius;
    if (y > 720 - radius) y = 720 - radius;
}

// 状態切り替え処理
void Player::ChangeState()
{
    InputManager* input = InputManager::GetInstance();


    // LBボタン
    if (input->GetButtonInputState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::ePress)
    {
        if (state == State::Normal)
        {
            state = State::Shadow;
        }
        else
        {
            state = State::Normal;
        }
    }

    // RBボタン
    if (input->GetButtonInputState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::ePress)
    {
        if (state == State::Normal)
        {
            state = State::Shadow;
        }
        else
        {
            state = State::Normal;
        }
    }
}

// 色更新処理
void Player::UpdateColor()
{
    switch (state)
    {
    case State::Normal:
        color = GetColor(255, 0, 0);
        break;

    case State::Shadow:
        color = GetColor(100, 100, 255);
        break;
    }
}

// 描画処理
void Player::Draw() const
{
    DrawCircle(x, y, radius, color, TRUE);
}