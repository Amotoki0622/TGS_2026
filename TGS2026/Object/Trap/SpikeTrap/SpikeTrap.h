#pragma once
#include "../TrapObject/TrapObject.h"

class Player;

class SpikeTrap : public TrapObject {
private:
    int spikeImage;     // トゲの画像
    float radius;       // 当たり判定の半径

public:
    SpikeTrap(float x, float y, float radius);

    // 毎フレーム、純粋に「今プレイヤーが乗っているか」だけをチェックする
    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};