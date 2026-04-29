#pragma once
#include "../Detective/DetectiveObject.h"

class Light : public DetectiveObject {
private:
    float radius; // è∆ÇÁÇ∑îºåa

public:
    Light(float x, float y, float radius);
    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};