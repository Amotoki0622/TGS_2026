#pragma once
#include "../GameObject.h"
#include <vector>

class Player;
class EffectManager;

class SignBoard : public GameObject
{
public:
    enum class State
    {
        None,       // 遠くにいる
        Near,       // 近くにいる（Aボタンエフェクト表示中）
        Showing     // 全画面説明を表示中（ゲーム一時停止）
    };

private:
    State currentState;         // 看板の状態
    float detectRadius;         // プレイヤーを検知する半径

    std::vector<int> explanationImageHandles; // 複数枚の画像ハンドルを入れる配列
    int currentPageIndex;                     // いま何ページ目か (0からスタート)

    int nextIconHandle;         // ▼画像のハンドル（仮描画中は未使用）
    float animTimer;            // ▼をフワフワ揺らすアニメ用タイマー

    Player* targetPlayer;
    EffectManager* effectManager;

public:
    // 💡 変更点：引数から Player* と EffectManager* を削除し、Warp と同じ感覚で呼べるように修正
    SignBoard(float x, float y, float width, float height,
        int signboardId, int signboardGraphic);

    virtual ~SignBoard() override;

    virtual void Initialize() override;
    virtual void Update(float delta_second) override;
    virtual void Draw() const override;
    virtual void Finalize() override;

    // シーン側から一時停止チェックをするための関数
    bool IsShowing() const { return currentState == State::Showing; }

    // 💡 【新設】生成後に外部（InGameSceneなど）から情報を登録するためのセット関数
    void SetPlayer(Player* player) { targetPlayer = player; }
    void SetEffectManager(EffectManager* effectMgr) { effectManager = effectMgr; }
};