#include "StageManager.h"

// インストラクタ
StageManager::StageManager()
	: m_currentLevel(0)
	, m_currentMoveLimit(0)
	, m_pCurrentData(nullptr)
{

}

StageManager::~StageManager()
{
}

// 初期化処理
void StageManager::Initialize()
{
	// 1. リストを読み込む
	m_allStages = StageLoader::LoadMapList("Resource/Map/StageData.csv");

	// 2. リストが空っぽならここで処理を止める（アクセス違反を防ぐ）
	if (m_allStages.empty())
	{
		// ここで止まる場合は StageData.csv の読み込み自体に失敗しています
		return;
	}

	// 3. 各ステージの中身(Stage1.csvなど)を読み込む
	for (int i = 0; i < (int)m_allStages.size(); i++)
	{
		StageLoader::LoadMapCSV(m_allStages[i]);
	}

	// 4. 準備ができたら最初のレベルをロード
	LoadLevel(0);
}

// 指定したレベルのステージを開始する
void StageManager::LoadLevel(int levelIndex)
{
	// ステージの総数を取得する
	int totalStage = (int)m_allStages.size();

	// ステージが0未満ではないか
	// ステージが総ステージ数を超えていないか
	if (levelIndex < 0 || levelIndex >= totalStage)
	{
		// 存在しないステージだから、処理を中断する
		return;
	}

	// 現在のステージ番号を記録する
	m_currentLevel = levelIndex;
	// 「今使うデータ」だけにピンを立てる
	m_pCurrentData = &m_allStages[m_currentLevel];

	if (m_pCurrentData->map.empty())
	{
		return;
	}

	// ステージの詳細(ステージ配置)をロード
	// StageLoader::LoadMapCSV(*m_pCurrentData);＊ワンちゃんいらないかもしれない(要らなかった)

	// 手数制限をセット
	m_currentMoveLimit = m_pCurrentData->moveLimit;


	// マップデータに基づいて、実際にゲームオブジェクトを生成する
	CreateStageObject();				// (オブジェクトを生成するときに使う処理)
}

void StageManager::NextLevel()
{
	int nextLevel = m_currentLevel + 1;

	// 次のステージがあるか確認
	if (nextLevel < (int)m_allStages.size())
	{
		LoadLevel(nextLevel);
	}
	else
	{
		// 全ステージクリアした時の処理(エンディング?)
	}

}

// 実際にオブジェクトを生成する内部関数
void StageManager::CreateStageObject()
{
	//if (!m_pCurrentData) return;
	if (m_pCurrentData == nullptr)
	{
		return;
	}

	for (int y = 0; y < m_pCurrentData->height; ++y) 
	{
		for (int x = 0; x < (int)m_pCurrentData->map[y].size(); ++x) 
		{
			char mode = m_pCurrentData->map[y][x];

			// 座標計算する処理

			// modeに応じた生成処理	(コメント文の中に詳細を書く)
			switch (mode)
			{
			case 'W':
				// 壁を生成する処理
				break;

			case 'P':
				// プレイヤーを生成する処理
				break;

			case 'C':
				// カメラを生成する処理
				break;

			case 'L':
				// ライトを生成する処理
				break;

			case 'T':
				// トラップを生成する処理
				break;

			case 'F':
				// 床を生成する処理
				break;

			case 'w':
				// ワープを生成する処理
				break;

			default:
				break;
			}
		}
	}
}