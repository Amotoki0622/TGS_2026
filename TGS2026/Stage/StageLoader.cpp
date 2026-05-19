#include "StageLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define DEBUG

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
		// throw(filePath + "が開けません (ステージCSVを格納するためのリスト)");
		return AllStage;		// デバック用 これで止まったら、パスの間違い
	}

	// ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::stringstream ss(line);
		std::string moveLimitStr, fileName;

		// カンマで分割して読み込む
		if (std::getline(ss, moveLimitStr, ','))
		{
			if (std::getline(ss, fileName))
			{
				StageData data;
				// 手数を数値に変換
				data.moveLimit = std::atoi(moveLimitStr.c_str());

				// 改行コードやスペースを削る処理
				fileName.erase(0, fileName.find_first_not_of(" \t\r\n"));
				size_t last = fileName.find_last_not_of(" \t\r\n");

				if (last != std::string::npos) 
				{
					data.MapFileName = fileName.substr(0, last + 1);
				}
				else
				{
					data.MapFileName = fileName;
				}
				if (data.MapFileName.empty() == false) 
				{
					AllStage.push_back(data);
				}
			}
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
		//m_debugMessage = "失敗:" + fullPath;		// 失敗時
		throw(fullPath + "が開けません。(マップ配置)");
	}

	data.map.clear();		// 描画がおかしくならないように既存のデータを削除

	// ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(ifs, line))
	{

#ifdef DEBUG
		std::vector<char> row;
		std::stringstream ss(line);
		std::string cell;

		// カンマ区切りで1要素ずつ取り出す
		while (std::getline(ss, cell, ','))
		{
			// 空白を除去 (必要だったら)
			cell.erase(0, cell.find_first_not_of(" "));
			cell.erase(cell.find_first_not_of(" ") + 1);

			if (cell.empty() == false)		// 中身が空かどうか(サイズが0か?)
			{
				row.push_back(cell[0]);		// 最初の1文字(P, B, G, Sなど)を格納
			}
			else
			{
				row.push_back('W');			// 空っぽを空白(床)とする
			}
		}

		// 1行分（row）のデータを、マップ全体（data.map）の末尾に格納している
		data.map.push_back(row);			
#else

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

#endif // DEBUG

	}

	// 幅と高さの更新
#ifdef DEBUG
	// 縦のサイズ(行数)を記録
	data.height = (int)data.map.size();

	// 横幅の最大値を0でリセットする
	data.width = 0;

	// 1行ずつ順番に確認していく
	for (int i = 0; i < (int)data.map.size(); i++)
	{
		// 今チェックしている行(i番目)の長さを取得する
		int currentRowWidth = (int)data.map[i].size();

		// もし「今の行」が「これまでの最大値」よりも大きかったら
		if (currentRowWidth > data.width)
		{
			// 最大値を更新する(一番長い行合わせる)
			data.width = currentRowWidth;
		}
	}
#else
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
#endif // DEBUG

	// 読み込み完了後にメッセージをセット
	//m_debugMessage = data.MapFileName + " を読み込みました。";
}