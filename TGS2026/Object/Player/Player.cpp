#include "Player.h"
#include "../../Utility/InputManager.h"

// 初期化処理
void Player::Initialize()
{
    x = 1180;
    y = 120;

    radius = 30;
    speed = 150;

    state = State::Normal;

    // 矩形コリジョンサイズ設定
    collisionWidth = radius * 2.0f;
    collisionHeight = radius * 2.0f;

    UpdateColor();
}

// 更新処理
void Player::Update()
{
    ChangeState();
    Move();
    UpdateColor();
}

// 移動処理
void Player::Move()
{
    InputManager* input = InputManager::GetInstance();

    int moveX = 0;
    int moveY = 0;

    // コントローラー
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress)
        moveY = -speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress)
        moveY = speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress)
        moveX = -speed;

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress)
        moveX = speed;

    // キーボード（デバッグ）
    if (input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress)
        moveY = -speed;

    if (input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress)
        moveY = speed;

    if (input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress)
        moveX = -speed;

    if (input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress)
        moveX = speed;

    // 座標更新
    x += moveX;
    y += moveY;

    // 画面制限
    if (x < radius) x = radius;
    if (x > 1280 - radius) x = 1280 - radius;

    if (y < radius) y = radius;
    if (y > 720 - radius) y = 720 - radius;
}

// 状態切り替え
void Player::ChangeState()
{
    InputManager* input = InputManager::GetInstance();

    if (input->GetButtonInputState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::ePress ||
        input->GetButtonInputState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_RETURN) == eInputState::ePress)
    {
        state = (state == State::Normal) ? State::Shadow : State::Normal;
    }
}

// 色更新
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

// 描画
void Player::Draw() const
{
    DrawCircle(x, y, radius, color, TRUE);

    // デバッグ：コリジョン矩形表示
    int left = (int)(x - collisionWidth / 2);
    int right = (int)(x + collisionWidth / 2);
    int top = (int)(y - collisionHeight / 2);
    int bottom = (int)(y + collisionHeight / 2);

    DrawBox(left, top, right, bottom, GetColor(0, 255, 0), FALSE);
}

// =========================================
// コリジョン取得
// =========================================
Vector2D Player::GetCollisionPos() const
{
    return Vector2D{ (float)x, (float)y };
}

float Player::GetCollisionWidth() const
{
    return collisionWidth;
}

float Player::GetCollisionHeight() const
{
    return collisionHeight;
}

// 座標取得
void Player::GetLocation(int& outX, int& outY) const
{
    outX = x;
    outY = y;
}