#include "Player.h"
//#include "../Wall/Wall.h"
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
    x = 70;
    y = 381;


    radius = 60;
    speed = 90; 

    // 音源読み込み・関連
    //moveSE = LoadSoundMem("Resource/Sounds/SE/object/player/");


    // 変身時
    changeStateSE = LoadSoundMem("Resource/Sounds/SE/object/player/change.mp3");
    // 読み込み直後に設定
    // 基本の周波数は 44100 が一般的です
    freq = GetFrequencySoundMem(changeStateSE); // 元の周波数を取得
    // 音量を設定（例：半分の 128 や、かなり控えめな 80 など）
    ChangeVolumeSoundMem(70, changeStateSE);
    // 倍速にする場合
    SetFrequencySoundMem((int)(freq * 2.0f), changeStateSE);




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
    /*collisionWidth = radius * 1.5f;
    collisionHeight = radius * 1.5f;*/

    collisionWidth = radius;
    collisionHeight = radius;

    tekazu = 18;

    UpdateColor();
}

// =========================
// 更新処理
// =========================
void Player::Update(const float& delta_second)
{
    ChangeState();
    //Move(walls);
    UpdateAnimation(delta_second);
}

// =========================
// 移動処理
// =========================
void Player::Move(const std::vector<Wall>& walls) {
    InputManager* input = InputManager::GetInstance();

    int moveX = 0;
    int moveY = 0;

    // =========================
    // 入力（1方向だけに制限）
    // =========================
    if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::ePress)
    {
        moveX = -speed;
        revers = FALSE;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::ePress)
    {
        moveX = speed;
        revers = TRUE;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_UP) == eInputState::ePress)
    {
        moveY = -speed;
    }
    else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::ePress)
    {
        moveY = speed;
    }

    // 入力がなければ何もしない
    if (moveX == 0 && moveY == 0) return;

    // =========================
    // 次の座標を計算
    // =========================
    int nextX = x + moveX;
    int nextY = y + moveY;

    // =========================
    // 画面外チェック
    // =========================
    if (nextX < radius || nextX > 1280 ||
        nextY < radius || nextY > 720 - radius)
    {
        return;
    }

    // =========================
    // 壁との当たり判定（移動前チェック）
    // =========================
    bool hitWall = false;

    for (const auto& wall : walls)
    {
        if (wall.IsHit(nextX, nextY, collisionWidth, collisionHeight))
        {
            hitWall = true;
            break;
        }
    }

    // =========================
    // 移動処理
    // =========================
    if (!hitWall)
    {
        x = nextX;
        y = nextY;

        // 成功したときだけ手数減少
        tekazu--;
    }
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
        // 状態が切り替わるタイミングで再生
        PlaySoundMem(changeStateSE, DX_PLAYTYPE_BACK);

        state = (state == State::Normal) ? State::Shadow : State::Normal;
        tekazu--;
       
    }
}

// =========================
// アニメーション処理
// =========================
void Player::UpdateAnimation(float delta_second)
{
    InputManager* input = InputManager::GetInstance();

    //// 1. 画像の切り替え判定（eHold：ボタンを押している間ずっと）
    //if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::eHold ||
    //    input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::eHold)
    //{
    //    currentImage = 1; // 押している間はアクション画像
    //}
    //else
    //{
    //    currentImage = 0; // 離せば通常立ち
    //}

    //// 2. 手数の減算判定（ePress：押した瞬間だけ1回）
    //if (input->GetButtonInputState(XINPUT_BUTTON_A) == eInputState::ePress ||
    //    input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
    //{
    //    tekazu--; // 押した瞬間に1回だけマイナス
    //}

    // --- 1. 入力判定 ---
    bool isPushing = (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::eHold ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::eHold);

    bool isPressed = (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress);

    // --- 2. タイマーと画像の制御 ---
    if (isPushing)
    {
        currentImage = 1;      // 押している間は画像1
        actionTimer = 0.25f;    // 常に「余韻タイマー」を最大値（例：0.25秒）にリセットし続ける
    }
    else
    {
        // ボタンを離しているとき
        if (actionTimer > 0.0f)
        {
            currentImage = 1;           // タイマーが残っていれば画像1を維持
            actionTimer -= delta_second; // タイマーを減らしていく
        }
        else
        {
            currentImage = 0;           // タイマーが切れたら通常画像に戻る
        }
    }

    // --- 3. 手数の減算（変更なし） ---
    if (isPressed)
    {
        tekazu--;
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
        // シャドウ状態：少し透けさせる（アルファ値 150前後）
        // これにより「隠れている」「実体がない」感じに
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

        // シャドウ状態（shadow.png は小さいので、もっと大きくする）
        if (images2[currentImage] != -1)
        {
            // player_01.pngの横幅(768)とshadow.pngの横幅(306)の比率を考えると、
            // 0.17 * (768 / 306) = 約 0.42 倍くらい。
            // 0.4 ～ 0.45 あたりで、通常時と同じ大きさに見えるように調整してください。
            float shadowExRate = 0.2f;
            DrawRotaGraph(x, y, (double)shadowExRate, 0.0, images2[currentImage], TRUE, revers);
        }
        // 他の描画に影響が出ないよう、最後に描画モードをリセットする
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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