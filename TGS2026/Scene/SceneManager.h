#pragma once
#include "SceneBase.h"

class SceneManager
{
private:
	SceneBase* current_scene; // 現在のシーン
	bool loop_flag;           // ループするか？

public:
	SceneManager();
	~SceneManager();

public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 終了時処理
	void Finalize();

public:
	// loop_flgの値を返す
	bool LoopCheck() const;

private:
	// 引数(new_scene_type)シーンに切り替え処理
	void ChangeScene(eSceneType new_scene_type);
	// 引数(new_scene_type)のシーン生成処理
	SceneBase* CreateScene(eSceneType new_scene_type);
};