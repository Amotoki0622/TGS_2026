#pragma once

#include "../SceneBase.h"

class TitleScene : public SceneBase
{
private:
    int menu_cursor;
    eSceneType next_scene;   

    //’Ç‰Á
    int background;

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