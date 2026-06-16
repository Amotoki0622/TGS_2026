#pragma once

#include "../SceneBase.h"

class TitleScene : public SceneBase
{
private:
    int menu_cursor;
    eSceneType next_scene;   

    // 背景画像
    int background;

    // タイトル音声
    int title_main_bgm;

    // スタート音声
    int title_start_se;

    bool is_selected = false;   // ボタンが押されたかどうか
    float transition_timer = 0.0f;  // 遷移するまでの時間を計るタイマー

    int font[2];        // font[1]だと、font[0]しか使えない

public:
    TitleScene();
    ~TitleScene();

public:
    virtual void Initialize() override;
    virtual eSceneType Update(const float& delta_second) override;
    virtual void Draw() const override;
    virtual void Finalize() override;

public:
    virtual eSceneType GetNowSceneType() const override;
};