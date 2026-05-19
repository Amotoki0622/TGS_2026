#include "Player.h"
#include "../Block/Block.h"
//#include "../Wall/Wall.h"
#include "../../Utility/InputManager.h"

// =========================
// 初期化処理
// =========================
void Player::Initialize()
{
    // 初期位置
    x = 64;
    y = 250;



    radius = 64;
    speed = 128; 

    state = State::Normal;
    currentImage = 0; // 最初は通常ポーズ

    currentImage = 0;
    // コリジョンサイズ
    /*collisionWidth = radius * 1.5f;
    collisionHeight = radius * 1.5f;*/

    collisionWidth = radius;
    collisionHeight = radius;

    tekazu = 18;

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
        768,    // 1枚あたりの横幅 (612 / 2)
        1024,    // 1枚あたりの縦幅 (そのまま)
        images2 // shadow用配列に格納
    );

    if (result == -1) {
        printfDx("画像読み込み失敗\n");
    }

  //  // =============================================================
  //// 移動用画像込みで考える場合　
  //// =============================================================
  //  int fullNormal = LoadGraph("Resource/Images/Player/player_03.png");
  //  int fullShadow = LoadGraph("Resource/Images/Player/shadow3.png");

  //  // --- 1. 通常状態（これは整列されているのでループでOK） ---
  //  for (int i = 0; i < 3; i++) {
  //      images[i] = DerivationGraph(i * 512, 0, 512, 1024, fullNormal);
  //  }

  //  for (int i = 0; i < 3; i++) {
  //      images2[i] = DerivationGraph(i * 512, 0, 480, 1024, fullShadow);
  //  }

  //  // 3. 親画像を解放（これで子画像だけがメモリに残る）
  //  DeleteGraph(fullNormal);
  //  DeleteGraph(fullShadow);

  //  // 4. エラーチェック
  //  if (images2[0] == -1 || images2[1] == -1) {
  //      printfDx("シャドウ画像の切り出しに失敗しました。\n");
  //  }

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
void Player::Move(const std::vector<GameObject*>& objects) {
    InputManager* input = InputManager::GetInstance();

    // Bボタン（蹴る）でブロックを動かす
    if (input->GetButtonInputState(XINPUT_BUTTON_B) == eInputState::ePress ||
        input->GetKeyInputState(KEY_INPUT_SPACE) == eInputState::ePress)
    {
        // --- 1. 蹴る方向（ベクトル）を決定する ---
        float kickX = 0.0f;
        float kickY = 0.0f;

        // 十字キーまたはキーボードの入力をチェック
        if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_LEFT) == eInputState::eHold ||
            input->GetKeyInputState(KEY_INPUT_LEFT) == eInputState::eHold) {
            kickX = -128.0f;
        }
        else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_RIGHT) == eInputState::eHold ||
            input->GetKeyInputState(KEY_INPUT_RIGHT) == eInputState::eHold) {
            kickX = 128.0f;
        }
        else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_UP) == eInputState::eHold ||
            input->GetKeyInputState(KEY_INPUT_UP) == eInputState::eHold) {
            kickY = -128.0f;
        }
        else if (input->GetButtonInputState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::eHold ||
            input->GetKeyInputState(KEY_INPUT_DOWN) == eInputState::eHold) {
            kickY = 128.0f;
        }
        else {
            // 何も方向が押されていない場合は、今のプレイヤーの向き(revers)に蹴る
            kickX = (revers == TRUE) ? 128.0f : -128.0f;
        }

        // --- 2. 判定を出す位置（check座標）を計算 ---
        // 実際に蹴りだす量(128)ではなく、目の前(64)にブロックがあるか調べる
        float checkX = (float)x + (kickX != 0 ? (kickX > 0 ? 64.0f : -64.0f) : 0.0f);
        float checkY = (float)y + (kickY != 0 ? (kickY > 0 ? 64.0f : -64.0f) : 0.0f);

        // --- 3. 衝突判定とPushの実行 ---
        for (auto& obj : objects) {
            if (obj == this) continue;

            if (obj->IsHit((int)checkX, (int)checkY, 60, 60)) { // 少し遊び(60)を持たせる
                Block* targetBlock = dynamic_cast<Block*>(obj);
                if (targetBlock != nullptr) {
                    printfDx("Blockを方向指定で蹴った！(X:%.0f, Y:%.0f)\n", kickX, kickY);
                    targetBlock->Push(kickX, kickY, objects);
                    return; // 成功したら終了
                }
            }
        }
    }


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
    if (nextX < radius || nextX > 1280 - radius || // 右端もradiusを考慮
        nextY < radius || nextY > 720 - radius)
    {
        return;
    }

    // =========================
    // オブジェクトとの当たり判定
    // =========================
    bool canMove = true;

    for (const auto& obj : objects)
    {
        // 自分自身（プレイヤー）との判定はスキップ
        if (obj == this) continue;

        // GameObjectクラスに追加したIsHitを呼び出す
        if (obj->IsHit(nextX, nextY, collisionWidth, collisionHeight))
        {
            // 何かにぶつかったので、基本は移動不可
            canMove = false;
            break;
            // --- 発展：オブジェクトごとの特殊処理 ---
            // もし「動かせるブロック」だった場合の処理をここに書く
            if (obj->IsMovable())
            {
                // ここでブロック側のMoveなどを呼び出し、
                // ブロックが移動に成功したら canMove = true にする、といった処理が可能
            }

            // ぶつかった時点でこのループは抜ける
            break;
        }

    }

    // =========================
    // 移動確定処理
    // =========================
    if (canMove)
    {
        // ★ 1. 動く前の現在の座標（x, y）を一時的にキープしておく
        float oldX = (float)x;
        float oldY = (float)y;

        x = nextX;
        y = nextY;

        // 移動が成功したときだけ手数減少
        tekazu--;

        // ★【ここに書く！】移動が確定した瞬間、動く「前」の床に煙を残す
        // nextX/Y に動く前の座標は「現在の x, y」なので、
        // 移動する直前の足元（x, y）にそのままエフェクトを生成すれば、
        // 綺麗に「一歩前の位置」に煙がポッと残ります！
        effectManager.AddEffect(oldX, oldY, EffectType::Smoke, "Resource/Images/Trap/SpikeTrap/SpikeTrap.png", 0.15f);
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

    // --- 3. 手数の減算 ---
    if (isPressed)
    {
        tekazu--;
    }
}


void Player::Draw() const
{
    if (state == State::Normal)
    {
        // 通常状態（player_01.png は大きいので 0.17倍）
        if (images[currentImage] != -1)
        {
                               //↓移動の画像の追加時はここを0.3
            DrawRotaGraph(x, y, 0.2, 0.0, images[currentImage], TRUE, revers);
        }
    }
    else // Shadow状態時
    {
        // シャドウ状態：少し透けさせる（アルファ値 150前後）
        // これにより「隠れている」「実体がない」感じに
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

        // シャドウ状態（shadow.png は小さいので、もっと大きくする）
        if (images2[currentImage] != -1)
        {
                              //↓移動の画像の追加時はここを0.3
            DrawRotaGraph(x, y, 0.2, 0.0, images2[currentImage], TRUE, revers);
        }
        // 他の描画に影響が出ないよう、最後に描画モードをリセットする
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // プレイヤーの背後に発生したエフェクトを描画
    effectManager.Draw();

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