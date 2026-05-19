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

void EffectManager::AddEffect(float x, float y, EffectType type, const std::string& imagePath) {
    // c_str() を使って DXライブラリが読める形式に変換
    int handle = LoadGraph(imagePath.c_str());
    if (handle == -1) {
        // エラーログを出して追跡しやすくする
        OutputDebugString("エフェクト画像の読み込みに失敗しました。\n");
        return;
    }

    EffectData newData;
    newData.x = x;
    newData.y = y;
    newData.type = type;
    newData.imageHandle = handle;
    newData.alpha = (type == EffectType::ActionUI) ? 0 : 255;
    newData.targetX = x;
    newData.targetY = y;

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
    }
}

void EffectManager::Draw() const {
    for (const auto& effect : effects) {
        if (effect.imageHandle == -1 || effect.alpha == 0) continue;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect.alpha);
        DrawRotaGraph((int)effect.x, (int)effect.y, 1.0, 0.0, effect.imageHandle, TRUE);
    }
    // ループの外側で1回だけ戻す方が親切で安全です
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