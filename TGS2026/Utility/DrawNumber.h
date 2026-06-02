#pragma once
#include <vector>
#include <string>

class DrawNumber
{
public:

    // 初期化（数字画像をセット）
    static void SetImage(const std::vector<int>& images);

    // 数値描画（右詰め）
    static void Draw(int x, int y, int number, float scale = 1.0f);

    // 数値描画（中央揃え）
    static void DrawCenter(int x, int y, int number, float scale = 1.0f);

private:

    static std::vector<int> num_image;
};