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
	// StageData.csvを読み込む
	// 全ステージデータをロード
	m_allStages = StageLoader::LoadMapList("Resource/Map/StageData.csv");		// 格納するファイルの指定


	// StageData.csvを読み込み、全ステージのCSVファイルを順番に読み込む
	for (int i = 0; i < (int)m_allStages.size(); i++)
	{
		// i番目のステージデータを読み込み関数に渡す処理
		StageLoader::LoadMapCSV(m_allStages[i]);		// m_allStages.mapに2次元配列データが格納される
	}

	if (m_allStages.empty() == false)
	{
		LoadLevel(0);		// 最初のレベルを呼び出す
	}
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

	// ステージの詳細(ステージ配置)をロード
	StageLoader::LoadMapCSV(*m_pCurrentData);

	// 手数制限をセット
	m_currentMoveLimit = m_pCurrentData->moveLimit;


	// マップデータに基づいて、実際にゲームオブジェクトを生成する
	CreateStageObject();
}

void StageManager::NextLevel()
{
	int nextlevel = m_currentLevel + 1;

	// 次のステージがあるか確認
	if (nextlevel < (int)m_allStages.size())
	{
		LoadLevel(nextlevel);
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