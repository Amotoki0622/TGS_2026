#include "Player.h"
#include "../../Utility/InputManager.h"

// もしPlayer.hで定義していなければ、ここで定義してください
// 今回の画像は横2枚、縦1枚の構成です
#define IMAGE_NUM 2

// =========================
// 初期化処理
// =========================
void Player::Initialize()
{
    // 初期位置
    x = 1180;
    y = 120;


    radius = 70;
    speed = 150; 

    state = State::Normal;
    currentImage = 0; // 最初は通常ポーズ

    // プレイヤーキャラ画像分割読み込み
    int result = LoadDivGraph(
        "Resource/Images/Player/player_01.png",
        2,      // 総枚数
        2, 1,   // 横に2枚、縦に1枚
        768,    // 1枚あたりの横幅 (1536 / 2)
        1024,   // 1枚あたりの縦幅 (そのまま)
        images
    );

    if (result == -1) {
        printfDx("画像読み込み失敗\n");
    }

    currentImage = 0;
    // コリジョンサイズ
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

    // 判定を「ePress（押した瞬間）」から「eOn（押している間）」に変更してください。
    // ※ePressだと1フレーム（1/60秒）しか切り替わらないため、反映されていないように見えます。
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::eHold ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::eHold)
    {
        currentImage = 1; // キックなどのアクション画像
    }
    else
    {
        currentImage = 0; // 通常立ち
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

void Player::Draw() const
{
    if (images[currentImage] != -1)
    {
        // 0.1f (10%の大きさ) で描画。
        // キャラが小さい場合は 0.2f、大きい場合は 0.05f などに調整してください。
        DrawRotaGraph(x, y, 0.2, 0.0, images[currentImage], TRUE);
    }

    // 緑の当たり判定枠もそのまま表示しておくと、中心が合っているか確認しやすいです
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

void Player::SetPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}