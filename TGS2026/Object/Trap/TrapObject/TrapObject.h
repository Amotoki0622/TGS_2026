#pragma once
#include "DxLib.h"
#include "../../Player/Player.h"
#include "../../GameObject.h"
#include "../../../Utility/Fade/Fade.h"
#include <cmath>

// オブジェクトの種類を定義
enum class TrapType {
    Camera,            // カメラ
    Light,             // ライト
    LegTrap,           // トラバサミ
    SpikeTrap          // トゲ床トラップ
};

class TrapObject : public GameObject {
public:
    // インゲーム内の進行状況をギミック側でも操作できるように定義
    enum class SceneState {
        Playing,    // 通常時
        Detected,   // 検知猶予中
        Restarting  // フェード中・リセット待ち
    };

protected:
    float x, y;
    bool detected;
    TrapType type; // 自分のタイプを保持する変数

    // 見つかった時の処理を自己完結させるための共通変数
    float detectionTimer = 0.0f;   // 各自が持つ猶予タイマー
    const float LIMIT_TIME = 2.5f; // 制限時間
    int beepSE = -1;               // 警告音のサウンドハンドル
    int dieSE = -1;                // 死亡時のサウンドハンドル

    // InGameSceneから教えてもらう、シーンをコントロールするためのポインタ
    SceneState* sceneState = nullptr;
    Fade* sceneFade = nullptr;

public:
    TrapObject(float x, float y, TrapType type)
        : x(x), y(y), detected(false), type(type) {
        // 親クラス（GameObject）のメンバを使う場合はここで位置を同期しておくと安全です
        this->location.x = x;
        this->location.y = y;

        this->detectionTimer = 0.0f;
        this->sceneState = nullptr;
        this->sceneFade = nullptr;
    }

    virtual ~TrapObject() {}

    // 状態リセット時にタイマーも戻せるように、ここで基本のInitializeを定義しておきます
    void Initialize() override {
        this->detected = false;
        this->detectionTimer = 0.0f;
    }

    void Finalize() override {}

    virtual void Update(const Player& player, float delta_second) = 0;
    virtual void Draw() const = 0;

    bool IsDetected() const { return detected; }
    TrapType GetType() const { return type; }

    // InGameSceneから状態とフェードの場所を教えてもらうための登録関数
    void SetupSceneReferences(SceneState* state, Fade* fade) {
        this->sceneState = state;
        this->sceneFade = fade;
    }
};