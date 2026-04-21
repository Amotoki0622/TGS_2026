#pragma once
#include "../Detective/DetectiveObject.h"

class Cam : public DetectiveObject {
private:
    float range;
    float angle;
    float fov;

public:
    // ˆø”‚ÌŒ^‚ğ float ‚É“ˆê‚µ‚Ä‚¨‚­‚ÆŒvZƒ~ƒX‚ªŒ¸‚è‚Ü‚·
    Cam(float x, float y, float angle, float range, float fov);

    void Update(const Player& player) override;
    void Draw() const override;
};