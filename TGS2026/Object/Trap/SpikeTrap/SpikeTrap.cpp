#include "SpikeTrap.h"
#include <cmath>

SpikeTrap::SpikeTrap(float x, float y, float radius)
    : TrapObject(x, y, TrapType::SpikeTrap), radius(radius) {

    //// 画像読み込み
    //openImage = LoadGraph("Resource/Images/Object/trap_open.png");
    //closedImage = LoadGraph("Resource/Images/Object/trap_closed.png");
}

void SpikeTrap::Update(const Player& player, float delta_second) {
    // 一度踏んで detected が true になったら、以降の判定をスキップして状態を維持する
    if (detected) return;

    // Shadow状態（影）でも踏む仕様にするか考えておく
    // もし影なら避ける仕様にする場合は、ここに return 処理を追加してください

    int px, py;
    player.GetLocation(px, py);

    // 円形範囲判定（Lightと同一のロジック）
    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    if (dist < radius) {
        detected = true;
    }
}

void SpikeTrap::Draw() const {
    // 発動状況に応じて画像を選択
    int handle = detected ? closedImage : openImage;

    if (handle != -1) {
        // 画像の中心(x, y)で描画
        DrawRotaGraph((int)x, (int)y, 1.0, 0.0, handle, TRUE);
    }

    // --- デバッグ用表示（LightのDraw構成を流用） ---
    unsigned int color = detected ? GetColor(255, 0, 0) : GetColor(0, 255, 255);

    // 範囲の可視化（透過円）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
    DrawCircle((int)x, (int)y, (int)radius, color, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 範囲の縁
    DrawCircle((int)x, (int)y, (int)radius, color, FALSE);
}