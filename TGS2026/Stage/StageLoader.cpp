#include "StageLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

// ステージマップ.csvを読み込み
std::vector<StageData> StageLoader::LoadMapList(const std::string& filePath)
{
	// 全ステージの手数制限、ステージCSVを格納するためリスト
	std::vector<StageData> AllStage;

	// 指定ファイルを読み込む
	std::ifstream ifs(filePath);

	// ファイルチェック
	if (ifs.fail())
	{
		throw(filePath + "が開けません (ステージCSVを格納するためのリスト)");
	}

	// ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(ifs, line))
	{
		StageData data;
		char MapFileBuffer[256] = { 0 };

		// CSVから「手数」「ファイル名」を取得
		if (sscanf_s(line.c_str(), "%d, %s", &data.moveLimit, MapFileBuffer, (unsigned int)sizeof(MapFileBuffer)) >= 2)
		{
			data.MapFileName = MapFileBuffer;
			AllStage.push_back(data);
		}
	}

	// 8ステージ分のデータが入ったリストを返す
	return AllStage;
}

// マップファイルを読み込み、StageDataにマップ配置を書き込む処理
void StageLoader::LoadMapCSV(StageData& data)
{
	// StageDataに保存されているファイル名を使用し、実際のマップファイルを開く
	std::string fullPath = "Resource/Map/" + data.MapFileName;
	std::ifstream ifs(fullPath);

	// ファイルチェック
	if (ifs.fail())
	{
		throw(fullPath + "が開けません。(マップ配置)");
	}

	// ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(ifs, line))
	{
		// 文字列を書式指定で分解して値を各変数に格納する
		char mode = NULL;
		int x_start = 0;
		int x_last = 0;
		int y_start = 0;
		int y_last = 0;
		char state = 0;

		sscanf_s(
			line.c_str(),
			"%c, %d, %d, %d, %d, %c",
			&mode, (unsigned int)sizeof(mode),
			&x_start, &y_start,
			&x_last, &y_last,
			&state, (unsigned int)sizeof(state)
		);

		// StageDataの2次元配列(map)に情報を蓄積する

		//  配列サイズが足りない時は拡張する
		if (y_start >= (int)data.map.size())
		{
			data.map.resize(y_start + 1);
		}
		if (x_start >= (int)data.map[y_start].size())
		{
			data.map[y_start].resize(x_start + 1, '.');		// デフォルトは床'.'
		}

		data.map[y_start][x_start] = mode;
	}

	// 最終的な縦横サイズを記録する
	data.height = (int)data.map.size();
	data.width = 0;

	for (int i = 0; i < (int)data.map.size(); i++)
	{
		// 今チェックしている「1行」を取り出す
		int currentRowWidth = (int)data.map[i].size();

		// もし「今の行の幅」が「これまでの最大幅」より大きければ
		if (currentRowWidth > data.width)
		{
			// 最大値を更新する
			data.width = currentRowWidth;
		}
	}
	
}