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
        float alpha;
        float targetX, targetY;
        float scale;
        float angle;
        bool isReversedX; // 左右反転フラグ（true: 反転あり）
    };

    std::vector<EffectData> effects;

public:
    // コンストラクタとデストラクタをシンプルに
    EffectManager();
    ~EffectManager();

    void AddEffect(float x, float y, EffectType type, const std::string& imagePath, float scale = 1.0f, float angle = 0.0f, bool isReversedX = false);

    void Update(const Player& player, float delta_second);
    void Update(float delta_second);
    void Draw() const;
    void ClearAll();
};