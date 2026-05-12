#pragma once
#include "../TrapObject/TrapObject.h"

class Light : public TrapObject {
private:
    float radius; // è∆ÇÁÇ∑îºåa

public:
    Light(float x, float y, float radius);
    void Update(const Player& player, float delta_second) override;
    void Draw() const override;
};