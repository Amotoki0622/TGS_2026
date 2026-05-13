#pragma once
#include "../TrapObject/TrapObject.h"

class SpikeTrap : public TrapObject {
private:
    float radius;    // “¥‚İ”»’è‚Ì”¼Œa
    int openImage;   // ‘Ò‹@’†‰æ‘œ
    int closedImage; // ”­“®Œã‰æ‘œ

public:
    // Light‚Æ“¯‚¶ˆø”\¬ (x, y, radius)
    SpikeTrap(float x, float y, float radius);

    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};