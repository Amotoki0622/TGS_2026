#pragma once

#include "StageData.h"
#include "StageLoader.h"
#include "../Object/GameObject.h"
#include "../Object/Trap/TrapObject/TrapObject.h"

#include <vector>
class StageManager
{
private:
	std::vector<StageData> m_allStages; // 全ステージデータ
	int m_currentLevel;                 // 現在のステージ番号
	int m_currentMoveLimit;             // 現在の残り手数

	std::vector<GameObject*> m_Object;

public:
	// インストラクタ
	StageManager();
	// デストラクタ
	~StageManager();

	// 初期化処理
	void Initialize();			// 起動時に1回呼び出す

	// 更新処理
	// void Updata();				//毎フレーム呼び出す

	// 指定したレベルのステージを開始する
	void LoadLevel(int levelIndex);			// ステージ切り替え時に呼び出す

	void NextLevel();		// 次のステージに行く関数

	// 現在の手数を取得
	int GetCurrentMoveLimit()const
	{
		return m_currentMoveLimit;
	}

	// 現在のレベルを取得
	int GetCurrentLevel() const
	{
		return m_currentLevel;
	}

	// ステージの総数を取得
	int GetTotalStages() const				// デバック用
	{
		return (int)m_allStages.size();
	}

	const std::vector<GameObject*>& GetGenerateObject() const
	{
		return m_Object;
	}

	// クリアする関数(ステージ切り替え時用)
	void ClearObjects();

private:
	// 実際にオブジェクトを生成する内部関数
	void CreateStageObject();

	// プレイヤーの初期位置を保存する変数
	Vector2D m_playerSpawnPos = Vector2D(-100.0f, -100.0f);

	// 現在プレイ中のステージデータを参照(ポインタ)
	StageData* m_pCurrentData;

	bool  m_debug_isB_Detected = false; // 'B' が見つかったら true にする
	float m_debug_bX = 0.0f;           // 見つかったX座標
	float m_debug_bY = 0.0f;

public:
	template <class T>
	T* CreateStageObjectInstance(const Vector2D& generate_location)
	{
		T* new_instance = new T();

		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		if (new_object == nullptr)
		{
			delete new_instance;
			throw std::string("ゲームオブジェクトを生成できませんでした。");
			return nullptr;
		}

		new_object->SetLocation(generate_location);

		new_object->Initialize();

		m_Object.push_back(new_object);

		return new_instance;
	}

	// InGameSceneにプレイヤーの位置を渡す関数
	Vector2D GetPlayerSpawnPosition() const;

	// デバック
	void DrawDebugInfo() const;
};

