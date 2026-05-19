#pragma once

#include "StageData.h"
#include "StageLoader.h"

#include <vector>
class StageManager
{
private:
	std::vector<StageData> m_allStages; // 全ステージデータ
	int m_currentLevel;                 // 現在のステージ番号
	int m_currentMoveLimit;             // 現在の残り手数

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


private:
	// 実際にオブジェクトを生成する内部関数
	void CreateStageObject();

	// 現在プレイ中のステージデータを参照(ポインタ)
	StageData* m_pCurrentData;
};

