#include "Player.h"
#include "../../Utility/InputManager.h"

// ===============================
// 初期化
// ===============================
void Player::Initialize()
{
    x = 640;  // 画面中央（1280想定）
    y = 360;

    radius = 30;
    speed = 1.0;

    color = GetColor(255, 0, 0); // 赤
}

// ===============================
// 更新
// ===============================
void Player::Update()
{
    InputManager* input = InputManager::GetInstance();

    // =========================
    // キーボード移動（十字キー）
    // =========================
    if (input->GetButton(XINPUT_BUTTON_DPAD_UP))
    {
        y -= speed;
    }

    if (input->GetButton(XINPUT_BUTTON_DPAD_DOWN))
    {
        y += speed;
    }

    if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))
    {
        x -= speed;
    }

    if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
    {
        x += speed;
    }

    // =========================
    // コントローラー移動（スティック）
    // =========================
    Vector2D stick = input->GetLeftStick();

    x += (int)(stick.x * speed);
    y -= (int)(stick.y * speed);

    // =========================
    // 色変更（LB / RB）
    // =========================
    if (input->GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER))
    {
        color = GetColor(0, 255, 0); // 緑
    }

    if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
    {
        color = GetColor(255, 0, 0); // 赤
    }

    // =========================
    // 画面外に出ないように制限
    // =========================
    if (x < radius) x = radius;
    if (x > 1280 - radius) x = 1280 - radius;

    if (y < radius) y = radius;
    if (y > 720 - radius) y = 720 - radius;
}

// ===============================
// 描画
// ===============================
void Player::Draw() const
{

    DrawCircle(x, y, radius, color, TRUE);
}