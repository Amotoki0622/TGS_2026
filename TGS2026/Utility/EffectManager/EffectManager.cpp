#include "EffectManager.h"
#include "../../Object/Player/Player.h" 
#include "DxLib.h"
#include <cmath>

EffectManager::EffectManager() {
    // ここは空でOKです
}

EffectManager::~EffectManager() {
    ClearAll();
}

void EffectManager::AddEffect(float x, float y, EffectType type, const std::string& imagePath, float scale) {
    int handle = LoadGraph(imagePath.c_str());
    if (handle == -1) return;

    EffectData newData;
    newData.x = x;
    newData.y = y;
    newData.type = type;
    newData.imageHandle = handle;
    newData.alpha = (type == EffectType::ActionUI) ? 0 : 255;
    newData.targetX = x;
    newData.targetY = y;
    newData.scale = scale; // ★倍率を記憶する

    effects.push_back(newData);
}



void EffectManager::Update(const Player& player, float delta_second) {
    int px, py;
    player.GetLocation(px, py);

    for (auto& effect : effects) {
        if (effect.type == EffectType::FollowItem) {
            // プレイヤーの少し後ろをターゲットにする
            effect.targetX = (float)px - 40.0f;
            effect.targetY = (float)py;

            effect.x += (effect.targetX - effect.x) * 0.1f;
            effect.y += (effect.targetY - effect.y) * 0.1f;
        }
        else if (effect.type == EffectType::ActionUI) {
            float dist = sqrtf(powf((float)px - effect.x, 2) + powf((float)py - effect.y, 2));

            if (dist < 150.0f) {
                effect.alpha += 15;
                if (effect.alpha > 200) effect.alpha = 200;
            }
            else {
                effect.alpha -= 15;
                if (effect.alpha < 0) effect.alpha = 0;
            }
        }
        else if (effect.type == EffectType::Smoke) {
            // プレイヤーを追いかけず（x, y はそのまま）、毎フレーム透明度（alpha）を減らす
            effect.alpha -= 10; // 数値を大きくすると早く消え、小さくすると長く残ります
            if (effect.alpha < 0) effect.alpha = 0;
        }
    }
}

void EffectManager::Draw() const {
    for (const auto& effect : effects) {
        if (effect.imageHandle == -1 || effect.alpha == 0) continue;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect.alpha);

        // ★第3引数の「1.0」だった部分を「effect.scale」に書き換える！
        DrawRotaGraph((int)effect.x, (int)effect.y, (double)effect.scale, 0.0, effect.imageHandle, TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EffectManager::ClearAll() {
    for (auto& effect : effects) {
        if (effect.imageHandle != -1) {
            DeleteGraph(effect.imageHandle);
        }
    }
    effects.clear();
}