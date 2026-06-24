#pragma once

#include "../../Utility/Vector2D.h"
#include "../GameObject.h"

class Player;

class Warp : public GameObject
{
private:
    float toX;
    float toY;

    Player* player;

    int warpImage[3];
    int warpSE;
    int warp_in_se;
    int warp_out_se;

    int animFrame;
    int animTimer;

    float angle;

    static bool playerInWarp;

public:

    Warp();
    ~Warp();

    void SetPlayer(Player* p);

    void SetPosition(float px, float py);
    void SetSize(float w, float h);

    void SetTargetPosition(float x, float y);

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw() const override;
    void Finalize() override;

    bool IsHit(
        int nextX,
        int nextY,
        float pW,
        float pH
    ) const;
};