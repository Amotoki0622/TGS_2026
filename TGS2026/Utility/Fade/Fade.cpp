#include "Fade.h"

Fade::Fade()
    : type(FadeType::None)
    , ratio(0.0f)
    , speed(0.02f)
    , isFading(false)
    , isOut(true)
{
}

void Fade::Start(FadeType type, bool isOut, float speed) {
    this->type = type;
    this->isOut = isOut;
    this->speed = speed;
    this->isFading = true;

    // 暗くする場合(Out)は0から開始、明るくする場合(In)は1から開始
    this->ratio = isOut ? 0.0f : 1.0f;
}

void Fade::Update(float delta_second)
{
    if (!isFading) return;

    // speed に delta_second を掛けることで、FPSに関わらず一定時間で終わるようになります
    // speed を「1秒間に変化する量」として扱います
    float step = speed * delta_second;

    if (isOut) { // 暗くなる（フェードアウト）
        ratio += step;
        if (ratio >= 1.0f) {
            ratio = 1.0f;
            isFading = false;
        }
    }
    else { // 明るくなる（フェードイン）
        ratio -= step;
        if (ratio <= 0.0f) {
            ratio = 0.0f;
            isFading = false;
        }
    }
}

void Fade::Draw() {
    if (type == FadeType::None) return;
    if (!isFading && ratio <= 0.0f) return;

    unsigned int black = GetColor(0, 0, 0);
    int w = 1280; // 画面幅
    int h = 720;  // 画面高

    switch (type) {
    case FadeType::Normal:
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 * ratio));
        DrawBox(0, 0, w, h, black, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        break;

    case FadeType::TopToBottom:
        DrawBox(0, 0, w, (int)(h * ratio), black, TRUE);
        break;

    case FadeType::LeftToRight:
        DrawBox(0, 0, (int)(w * ratio), h, black, TRUE);
        break;

    case FadeType::RightToLeft:
        DrawBox((int)(w * (1.0f - ratio)), 0, w, h, black, TRUE);
        break;

    case FadeType::IrisOut:
        // 円形フェード（真ん中の穴を塞いでいく）
        // 画面中央から角までの最大距離を基準にする
        float maxRadius = 800.0f;
        float currentRadius = maxRadius * (1.0f - ratio);

        if (currentRadius <= 0) {
            DrawBox(0, 0, w, h, black, TRUE);
        }
        else {
            // 円そのものを描くのではなく、「太い円輪」で外側を塗りつぶす
            // 2000pxくらいの極太の線で円を描くと、画面外まで塗りつぶせます
            DrawCircle(w / 2, h / 2, (int)(currentRadius + 1000), black, TRUE, (int)2000);

            // もしガタつきが気になる場合は、DrawCircleの代わりに、
            // 上下左右に4つの大きなBoxを描いて穴を狭める処理に差し替えることも可能です。
        }
        break;
    }
}