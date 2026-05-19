#pragma once
#include <vector>
#include <string>

class Player;

enum class EffectType {
    FollowItem,
    ActionUI,
    Smoke
};

class EffectManager {
private:
    struct EffectData {
        float x, y;
        EffectType type;
        int imageHandle;
        int alpha;
        float targetX, targetY;
        float scale;
    };

    std::vector<EffectData> effects;

public:
    // コンストラクタとデストラクタをシンプルに
    EffectManager();
    ~EffectManager();

    void AddEffect(float x, float y, EffectType type, const std::string& imagePath, float scale);

    void Update(const Player& player, float delta_second);
    void Draw() const;
    void ClearAll();
};