#pragma once
#include "../TrapObject/TrapObject.h"

class LegTrap : public TrapObject {
private:
    float radius;    // 踏み判定の半径（Lightの構成に合わせました）
    int openImage;   // 待機中画像
    int closedImage; // 発動後画像

public:
    // Lightと同じ引数構成 (x, y, radius)
    LegTrap(float x, float y, float radius);

    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};