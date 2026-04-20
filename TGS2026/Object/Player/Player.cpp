#include "Player.h"
#include "../../Utility/InputManager.h"

#define IMAGE_NUM 2

// =========================
// 初期化処理
// =========================
void Player::Initialize()
{
    x = 1180;
    y = 120;

    radius = 30;
    speed = 150;

    state = State::Normal;

    // =========================
    // 画像読み込み（分割）
    // ※マルチバイトなのでそのまま文字列でOK
    // =========================
    int result = LoadDivGraph(
        "Resource/Images/Player/player_01.png",
        IMAGE_NUM,
        2, 1,
        256, 256,
        images
    );

    currentImage = 0;

    if (result == -1)
    {
        printfDx("画像読み込み失敗\n");

        // ★絶対に入れる（保険）
        images[0] = LoadGraph("Resource/Images/Player/player_01.png");
        images[1] = images[0];
    }
    else
    {
        printfDx("画像読み込み成功\n");
    }

    // =========================
    // コリジョンサイズ
    // =========================
    collisionWidth = radius * 2.0f;
    collisionHeight = radius * 2.0f;

    UpdateColor();
}

// =========================
// 更新処理
// =========================
void Player::Update()
{
    ChangeState();
    Move();
    UpdateAnimation();
}

// =========================
// 移動処理
// =========================
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

    // キーボード
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

// =========================
// 状態切り替え
// =========================
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

// =========================
// アニメーション処理
// =========================
void Player::UpdateAnimation()
{
    InputManager* input = InputManager::GetInstance();

    // 押してる間だけキック
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
    {
        currentImage = 1;
    }
    else
    {
        currentImage = 0;
    }
}

// =========================
// 色更新（使わなくてもOK）
// =========================
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

// =========================
// 描画処理
// =========================
void Player::Draw() const
{
    // 中心表示（256画像 → 半分128）
    DrawGraph(x - 128, y - 128, images[currentImage], TRUE);

    DrawGraph(50, 50, images[currentImage], TRUE);

    // コリジョン矩形
    int left = (int)(x - collisionWidth / 2);
    int right = (int)(x + collisionWidth / 2);
    int top = (int)(y - collisionHeight / 2);
    int bottom = (int)(y + collisionHeight / 2);

    DrawBox(left, top, right, bottom, GetColor(0, 255, 0), FALSE);
}

// =========================
// コリジョン取得
// =========================
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

// =========================
// 座標取得
// =========================
void Player::GetLocation(int& outX, int& outY) const
{
    outX = x;
    outY = y;
}