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
    EffectData newEffect;
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

    // ★初期の不透明度はMAX（255）にしておく
    newEffect.alpha = 255.0f;

    effects.push_back(newEffect);

    effects.push_back(newData);
}


void EffectManager::Update(float delta_second) {
    
    // 1. すべてのエフェクトの時間更新
    for (auto& effect : effects) {

        // ★ポイント：1秒間に「255（完全透明）」に向かって、delta_secondベースで減らす
        // これで処理落ちしても、どんなオブジェクトが呼んでも、同じ速度で綺麗に消える！
        float fadeSpeed = 500.0f; // 好みの消える速さに調整してください（数値が大きいほど早く消える）
        effect.alpha -= fadeSpeed * delta_second;

        if (effect.alpha < 0.0f) {
            effect.alpha = 0.0f;
        }
    }

    // 2. 透明になったエフェクトを自動でお掃除（一括削除）
    effects.erase(
        std::remove_if(effects.begin(), effects.end(), [](const EffectData& effect) {
            return effect.alpha <= 0.0f;
            }),
        effects.end()
    );
}

void EffectManager::Draw() const {
    for (const auto& effect : effects) {
        if (effect.imageHandle == -1 || effect.alpha == 0) continue;

        // 透明度を適用
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)effect.alpha);

        // Player側で完璧に制御された isReversedX を、第7引数（TurnFlag）にそのまま渡す！
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