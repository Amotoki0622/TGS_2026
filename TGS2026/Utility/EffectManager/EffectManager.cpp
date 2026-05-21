#include "EffectManager.h"
#include "../../Object/Player/Player.h" 
#include "DxLib.h"
#include <cmath>
#include <algorithm> 
#include <unordered_map>

EffectManager::EffectManager() {
  
}

EffectManager::~EffectManager() {
    ClearAll();
}

void EffectManager::AddEffect(float x, float y, EffectType type, const std::string& imagePath, float scale, float angle, bool isReversedX) {
    // ★【修正】毎回LoadGraphするのをやめ、すでに読み込み済みの画像があればそのハンドルを使い回す
    // これにより、DXライブラリの内部メモリがバグるのを防ぎます。
    static std::unordered_map<std::string, int> imageCache;
    int handle = -1;

    if (imageCache.count(imagePath) > 0) {
        handle = imageCache[imagePath];
    }
    else {
        handle = LoadGraph(imagePath.c_str());
        if (handle != -1) {
            imageCache[imagePath] = handle;
        }
    }

    if (handle == -1) return;

    EffectData newData;
    newData.x = x;
    newData.y = y;
    newData.type = type;
    newData.imageHandle = handle; // 安全に使い回されたハンドル
    newData.alpha = (type == EffectType::ActionUI) ? 0 : 255;
    newData.targetX = x;
    newData.targetY = y;
    newData.scale = scale;
    newData.angle = angle;
    newData.isReversedX = isReversedX;

    effects.push_back(newData);
}

void EffectManager::Update(const Player& player, float delta_second) {
    int px, py;
    player.GetLocation(px, py);

    // 1. 各エフェクトの数値を個別に更新
    for (auto& effect : effects) {

        // --- アイテムの処理 ---
        if (effect.type == EffectType::FollowItem) {
            effect.targetX = (float)px - 40.0f;
            effect.targetY = (float)py;
            effect.x += (effect.targetX - effect.x) * 0.1f;
            effect.y += (effect.targetY - effect.y) * 0.1f;
        }

        // --- UIの処理 ---
        if (effect.type == EffectType::ActionUI) {
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

        // --- 煙の処理 ---
        if (effect.type == EffectType::Smoke) {
            // ★ カウンター代わりに、現在の alpha の値が偶数のときだけ引く、
            // または、単純に「1」ずつ引く形にする
            effect.alpha -= 0.3; // 一番最遅の「1」にする

            if (effect.alpha < 0) effect.alpha = 0;
        }
    }

    // 2. 完全に透明になった煙をお掃除
    effects.erase(
        std::remove_if(effects.begin(), effects.end(), [](const EffectData& effect) {
            return (effect.type == EffectType::Smoke && effect.alpha <= 0);
            }),
        effects.end()
    );
}

void EffectManager::Draw() const {
    for (const auto& effect : effects) {
        if (effect.imageHandle == -1 || effect.alpha == 0) continue;

        // 透明度を適用
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)effect.alpha);

        // ★【修正】Player側で完璧に制御された isReversedX を、第7引数（TurnFlag）にそのまま渡す！
        // マイナス倍率などのトリッキーな計算を廃止し、DXライブラリ標準の機能で安全に描画します。
        DrawRotaGraph(
            (int)effect.x,
            (int)effect.y,
            (double)effect.scale,
            (double)effect.angle,
            effect.imageHandle,
            TRUE,
            effect.isReversedX // ★ここがDXライブラリ本来の反転フラグ位置です
        );

    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EffectManager::ClearAll() {
    // ※画像ハンドルは上のキャッシュで一元管理されるため、ここで毎回 DeleteGraph しなくて安全になります
    effects.clear();
}