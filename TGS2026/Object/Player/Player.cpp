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
    x = 20;
    y = 381;


    radius = 70;
    speed = 105; 

    state = State::Normal;
    currentImage = 0; // 最初は通常ポーズ

    // プレイヤーキャラ画像分割読み込み
    // 1. 通常状態の画像読み込み（player_01.png）
    // （前回の設定のまま：1536x1024を想定）
    LoadDivGraph(
        "Resource/Images/Player/player_01.png",
        2, 2, 1, 768, 1024, images
    );

    // =============================================================
    // 【重要】2. シャドウ状態の画像読み込み（shadow.png）の修正
    // =============================================================
    // プロパティで確認した 612x408 の画像に合わせて引数を修正します。
    int result = LoadDivGraph(
        "Resource/Images/Player/shadow2.png",
        2,      // 総枚数
        2, 1,   // 横2, 縦1
        768,    // 【ここを修正】1枚あたりの横幅 (612 / 2)
        1024,    // 【ここを修正】1枚あたりの縦幅 (そのまま)
        images2 // shadow用配列に格納
    );

    if (result == -1) {
        printfDx("画像読み込み失敗\n");
    }

    currentImage = 0;
    // コリジョンサイズ
    collisionWidth = radius * 1.5f;
    collisionHeight = radius * 1.5f;

    tekazu = 18;

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
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress) {
        moveY = -speed;
        tekazu--;
    }
        

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress) {
        moveY = speed;
        tekazu--;
    }
        

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress){
        revers = FALSE;
        moveX = -speed;
        tekazu--;
    }

    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress) {
        revers = TRUE;
        moveX = speed;
        tekazu--;
    }
        
    // キーボード
    if (input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress) {
        moveY = -speed;
        tekazu--;
    }
     

    if (input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress) {
        moveY = speed;
        tekazu--;
    }

    if (input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress) {
        revers = FALSE;
        moveX = -speed;
        tekazu--;
    }
        

    if (input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress) {
        revers = TRUE;
        moveX = speed;
        tekazu--;
    }
        

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
        tekazu--;
       
    }
}

// =========================
// アニメーション処理
// =========================
void Player::UpdateAnimation()
{
    InputManager* input = InputManager::GetInstance();

    // 1. 画像の切り替え判定（eOn：ボタンを押している間ずっと）
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::eHold ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::eHold)
    {
        currentImage = 1; // 押している間はアクション画像
    }
    else
    {
        currentImage = 0; // 離せば通常立ち
    }

    // 2. 手数の減算判定（ePress：押した瞬間だけ1回）
    if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
    {
        tekazu--; // 押した瞬間に1回だけマイナス
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
    if (state == State::Normal)
    {
        // 通常状態（player_01.png は大きいので 0.17倍）
        if (images[currentImage] != -1)
        {
            DrawRotaGraph(x, y, 0.2, 0.0, images[currentImage], TRUE, revers);
        }
    }
    else // state == State::Shadow
    {
        // シャドウ状態（shadow.png は小さいので、もっと大きくする）
        if (images2[currentImage] != -1)
        {
            // player_01.pngの横幅(768)とshadow.pngの横幅(306)の比率を考えると、
            // 0.17 * (768 / 306) = 約 0.42 倍くらい。
            // 0.4 ～ 0.45 あたりで、通常時と同じ大きさに見えるように調整してください。
            float shadowExRate = 0.2f;
            DrawRotaGraph(x, y, (double)shadowExRate, 0.0, images2[currentImage], TRUE, revers);
        }
    }

    // (x座標, y座標, 色, "書式文字列", 変数);
    DrawFormatString(0, 100, GetColor(255, 255, 255), "手数は %d です", tekazu);

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