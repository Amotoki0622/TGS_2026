#pragma once
#include "DxLib.h"

enum class FadeType {
    None,
    Normal,      // 通常の暗転（アルファ）
    TopToBottom, // 上から下へ
    RightToLeft, // 右から左へ
    LeftToRight, // 左から右へ
    IrisOut      // 円形に閉じる（ギャグアニメ風）
};

class Fade {
private:
    FadeType type;
    float ratio;    // 進捗率 (0.0: 透明 ～ 1.0: 真っ暗)
    float speed;    // 変化速度
    bool isFading;  // フェード動作中か
    bool isOut;     // true: 暗くなる(Out), false: 明るくなる(In)

public:
    Fade();
    void Start(FadeType type, bool isOut, float speed = 0.02f);
    void Update();
    void Draw();

    bool IsFinished() const { return !isFading; }
};