#pragma once
#include <vector>
#include <string>

class Player;

enum class EffectType {
    FollowItem,
    ActionUI
};

class EffectManager {
private:
    struct EffectData {
        float x, y;
        EffectType type;
        int imageHandle;
        int alpha;
        float targetX, targetY;
    };

    std::vector<EffectData> effects;

public:
    // コンストラクタとデストラクタをシンプルに
    EffectManager();
    ~EffectManager();

    void AddEffect(float x, float y, EffectType type, const std::string& imagePath);

    void Update(const Player& player, float delta_second);
    void Draw() const;
    void ClearAll();
};