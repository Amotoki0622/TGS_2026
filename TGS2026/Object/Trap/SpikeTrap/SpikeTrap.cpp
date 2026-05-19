#include "SpikeTrap.h"
#include "../../Player/Player.h" 
#include <cmath>

SpikeTrap::SpikeTrap(float x, float y, float radius)
    : TrapObject(x, y, TrapType::SpikeTrap), radius(radius)
{
    // 画像の読み込み（右側のトゲあり画像だけを使用）
    int handles[2];
    LoadDivGraph("Resource/Images/Trap/SpikeTrap/SpikeTrap2.png", 2, 2, 1, 735, 930, handles);
    spikeImage = handles[1];
}

void SpikeTrap::Update(const Player& player, float delta_second) {
    int px, py;
    player.GetLocation(px, py);

    // 1. 今、プレイヤーがトゲの範囲内にいるかをチェック
    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    bool isTouching = (dist < radius);

    // 2. 「乗っている」かつ「まだ手数を減らしていない」なら手数を減らす
    if (isTouching && !detected) {
        Player& mutablePlayer = const_cast<Player&>(player);
        mutablePlayer.DecreaseMoveCount();

        detected = true; // 「この侵入ではもう減らしたよ」というフラグ
        OutputDebugString("トゲを踏んだ！手数が減りました。\n");
    }

    // 3. トゲから足が離れたら、検知フラグをリセットする
    if (!isTouching) {
        detected = false;
    }
}

void SpikeTrap::Draw() const {
    // 常にトゲ画像を定位置に描画
    if (spikeImage != -1) {
        float scale = (radius * 1.6f) / 740.0f;
        DrawRotaGraph((int)x, (int)y, (double)scale, 0.0, spikeImage, TRUE);
    }

    // デバッグ用表示：踏んでいる間だけ文字を出す
    if (detected) {
        DrawFormatString((int)x - 20, (int)y - 50, GetColor(255, 255, 0), "HIT!");
    }
}