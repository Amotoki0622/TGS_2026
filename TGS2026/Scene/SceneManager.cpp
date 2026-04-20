#include "SceneManager.h"
#include "../Utility/InputManager.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/ProjectConfig.h"
#include "DxLib.h"

#include "Title/TitleScene.h"
#include "InGame/InGameScene.h"
#include "Ranking/RankingScene.h"
#include "Result/ResultScene.h"
#include "End/EndScene.h"


//#define DEBUG

SceneManager::SceneManager() : current_scene(nullptr), loop_flag(true)
{

}

SceneManager::~SceneManager()
{
	this->Finalize();
}

void SceneManager::Initialize()
{
#ifdef DEBUG

	ChangeScene(eSceneType::eTitle);

#else
	// ウィンドウモードで起動する
	ChangeWindowMode(TRUE);

	// ウィンドウサイズの設定
	SetGraphMode(D_WIN_MAX_X, D_WIN_MAX_Y, D_COLOR_BIT);

	// ウィンドウタイトルの設定
	SetWindowText("タイトルが分からないから(仮)と命名");

	// 垂直同期を行わない
	SetWaitVSyncFlag(FALSE);

	// Log.txtファイルの生成制御（Debugモードのみ生成する）
#if _DEBUG
	SetOutApplicationLogValidFlag(TRUE);
#else
	SetOutApplicationLogValidFlag(FALSE);
#endif // _DEBUG

	// Dxライブラリの初期化
	if (DxLib_Init() == D_FAILURE)
	{
		throw std::string("Dxライブラリの初期化に失敗しました！\n");
	}

	// 描画先を表画面に反映する
	SetDrawScreen(DX_SCREEN_BACK);

	// 最初のシーンをタイトル画面にする
	ChangeScene(eSceneType::eTitle);

	// 非アクティブ状態でも動作させる
	SetAlwaysRunFlag(TRUE);
#endif // DEBUG



}

void SceneManager::Update()
{
#ifdef DEBUG

	eSceneType next_scene_type = current_scene->Update();

	current_scene->Draw();

	if (next_scene_type != current_scene->GetNowSceneType())
	{
		ChangeScene(next_scene_type);
	}

#else
	// 入力情報を取得する
	InputManager* input = InputManager::GetInstance();

	// メインループ
	while (ProcessMessage() == D_SUCCESS)
	{
		// 入力情報の更新
		input->Update();


		// フレームレートの制御
		FreamControl();

		// シーンの更新
		eSceneType next_scene_type = current_scene->Update(GetDeltaSecond());

		// 描画処理
		Draw();

		//// ゲームを終了するか確認する
		//if ((next_scene_type == eSceneType::eEnd) ||
		//	(input->GetButtonInputState(XINPUT_BUTTON_BACK)) ||
		//	(input->GetButtonInputState(KEY_INPUT_ESCAPE) == ePadInputState::ePress))
		//{
		//	break;
		//}

		// 現在のシーンタイプが次のシーンタイプと違っているか？
		if (current_scene->GetNowSceneType() != next_scene_type)
		{
			// シーン切り替え処理
			ChangeScene(next_scene_type);
		}
	}


#endif // DEBUG

}

void SceneManager::Finalize()
{
	if (current_scene != nullptr)
	{
		// current_sceneの終了処理
		current_scene->Finalize();
		// current_sceneが確保したメモリを解放
		delete current_scene;
		// current_sceneのポインタを無効化
		current_scene = nullptr;
	}
}

void SceneManager::Draw() const
{
	// 画面の初期化
	ClearDrawScreen();

	// シーンの描画処理
	current_scene->Draw();

	// 裏画面の内容を表画面に反映する
	ScreenFlip();
}

bool SceneManager::LoopCheck() const
{
	return loop_flag;
}

void SceneManager::ChangeScene(eSceneType new_scene_type)
{
	if (new_scene_type == eSceneType::eEnd)
	{
		loop_flag = false;
		return;
	}

	SceneBase* new_scene = CreateScene(new_scene_type);

	if (new_scene == nullptr)
	{
		throw("\n新しいシーンの生成ができませんでした。\n");
	}

	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	new_scene->Initialize();

	current_scene = new_scene;
}


// dynamiccast(SceneBaseを継承しいているか確認)
SceneBase* SceneManager::CreateScene(eSceneType new_scene_type)
{
	switch (new_scene_type)
	{
	case eSceneType::eTitle:
		return dynamic_cast<SceneBase*>(new TitleScene());

	case eSceneType::eQuestion
		return dynamic_cast<SceneBase*>(new Question());

	case eSceneType::eInGame:
		return dynamic_cast<SceneBase*>(new InGameScene());

	case eSceneType::eRanking:
		return dynamic_cast<SceneBase*>(new RankingScene());

	case eSceneType::eResult:
		return dynamic_cast<SceneBase*>(new ResultScene());

	case eSceneType::eEnd:
		return dynamic_cast<SceneBase*>(new EndScene());

	default:
		return nullptr;
	}
}