#include "Cam.h"

Cam::Cam()
    : TrapObject(0.0f, 0.0f, TrapType::Camera)
    , range(0.0f)
    , angle(0.0f)
    , fov(0.0f)
    , baseAngle(0.0f)
{
    this->currentSwingAngle = 0.0f;
    this->swingAngle = 0.8f;
    this->swingSpeed = 0.4f;
    this->direction = 1;
    this->isPausing = false;
    this->pauseTimer = 0.0f;
}

// コンストラクタ
Cam::Cam(float x, float y, float angle, float range, float fov)
    : TrapObject(x, y, TrapType::Camera)
{
    this->x = x;
    this->y = y;

    // 💡【追加】カメラロボットの「壁としてのサイズ」を設定（横幅, 縦幅）
    // スクリーンショットのサイズ感に合わせて、ここでは仮に 64px × 64px にしています。
    // もしすき間が空いたりきつすぎたりしたら、この数値を調整してください！
    this->box_size.x = 64.0f;
    this->box_size.y = 64.0f;

    this->range = range;
    this->baseAngle = angle;
    this->angle = angle;
    this->fov = fov;

    this->currentSwingAngle = 0.0f;
    this->swingAngle = 0.8f;
    this->swingSpeed = 0.4f;
    this->direction = 1;
    this->isPausing = false;
    this->pauseTimer = 0.0f;
}


void Cam::Initialize()
{
    this->x = this->location.x;
    this->y = this->location.y;
    this->box_size.x = 64.0f;
    this->box_size.y = 64.0f;
    
    this->currentSwingAngle = 0.0f;
    this->swingAngle = 0.8f;
    this->swingSpeed = 0.4f;
    this->direction = 1;
    this->isPausing = false;
    this->pauseTimer = 0.0f;
    this->detected = false;

    cam_image = LoadGraph("Resource/Images/Gimmick/cam.png");

}

// 引数に delta_second を追加しましょう
void Cam::Update(const Player& player, float delta_second) {
    detected = false;

    detected = false;

    if (isPausing) {
        // --- 待機中の処理 ---
        pauseTimer += delta_second;
        if (pauseTimer >= 1.0f) { // 1.0f = 1秒待機（ここを調整）
            isPausing = false;
            pauseTimer = 0.0f;
            direction *= -1; // 向きを反転させる
        }
    }
    else {
        // --- 移動中の処理 ---
        // swingSpeed を 0.5f くらいにするとゆっくりになります
        currentSwingAngle += swingSpeed * delta_second * direction;

        // 端まで到達したかチェック
        if (fabsf(currentSwingAngle) >= swingAngle) {
            isPausing = true;  // 停止モードへ
            // 行き過ぎを補正
            currentSwingAngle = (direction > 0) ? swingAngle : -swingAngle;
        }
    }

    // 最終的な angle を決定
    angle = baseAngle + currentSwingAngle;


    // 影状態なら検知処理を行わない
    if (player.GetState() == Player::State::Shadow) return;

    int px, py;
    player.GetLocation(px, py);

    // 1. 距離判定
    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    if (dist < range) {
        // 2. 角度判定（ここで使う angle は、上で計算された最新の角度になります）
        float targetAngle = atan2f((float)py - y, (float)px - x);
        float diffAngle = targetAngle - angle;

        // 角度の差を -PI ～ PI の範囲に補正
        while (diffAngle > DX_PI_F)  diffAngle -= DX_PI_F * 2;
        while (diffAngle < -DX_PI_F) diffAngle += DX_PI_F * 2;

        if (fabsf(diffAngle) < fov / 2.0f) {
            detected = true;
        }
    }
}

void Cam::Draw() const {
    unsigned int color = detected ? GetColor(255, 0, 0) : GetColor(255, 255, 0);

    // 目の位置を微調整するためのオフセット（ズラし幅）
    float eyeOffsetX = 0.0f;   // 左右のズレ（基本0でOK）
    float eyeOffsetY = -40.0f; // 上下のズレ（マイナスにすると上へ移動します。-30.0f 〜 -50.0fあたりで調整！）

    // 本当の「目」の座標
    float eyeX = x + eyeOffsetX;
    float eyeY = y + eyeOffsetY;


    // カメラ本体（画像）の描画
    if (cam_image != -1) {
        // 設定スケール
        float displayScale = 0.15f; 

        // 画像を描画
        DrawRotaGraph((int)x, (int)y, displayScale, 0.0f, cam_image, TRUE);
    }
    else {
        DrawCircle((int)x, (int)y, 10, GetColor(0, 0, 255), TRUE);
    }


    // 視界の可視化（扇形）
    for (float a = angle - fov / 2; a <= angle + fov / 2; a += 0.05f) {
        DrawLine((int)eyeX, (int)eyeY, (int)(eyeX + cosf(a) * range), (int)(eyeY + sinf(a) * range), color);
    }
}

// カメラの当たり判定（Blockの処理をそのまま移植）
bool Cam::IsHit(int nextX, int nextY, int width, int height) const
{
    // プレイヤー（移動先）の四隅を計算
    float pLeft = (float)nextX - (float)width / 2.0f + 0.5f;
    float pRight = (float)nextX + (float)width / 2.0f - 0.5f;
    float pTop = (float)nextY - (float)height / 2.0f + 0.5f;
    float pBottom = (float)nextY + (float)height / 2.0f - 0.5f;

    // このカメラロボット自体の四隅を計算
    float bLeft = location.x - box_size.x / 2.0f;
    float bRight = location.x + box_size.x / 2.0f;
    float bTop = location.y - box_size.y / 2.0f;
    float bBottom = location.y + box_size.y / 2.0f;

    // 衝突判定（AABB方式）
    return (
        pLeft < bRight &&
        pRight > bLeft &&
        pTop < bBottom &&
        pBottom > bTop
        );
}