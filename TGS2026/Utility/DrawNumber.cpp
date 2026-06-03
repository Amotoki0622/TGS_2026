#include "DrawNumber.h"
#include "DxLib.h"

std::vector<int> DrawNumber::num_image;

void DrawNumber::SetImage(const std::vector<int>& images)
{
    num_image = images;
}

void DrawNumber::Draw(int x, int y, int number, float scale)
{
    if (num_image.empty()) return;

    int image_width, image_height;
    GetGraphSize(num_image[0], &image_width, &image_height);

    int digit_width = image_width / 10;
    int digit_height = image_height;

    std::string numStr = std::to_string(number);

    int total_width = static_cast<int>(numStr.length() * digit_width * scale);

    int startX = x - total_width;

    for (size_t i = 0; i < numStr.length(); ++i)
    {
        int digit = numStr[i] - '0';

        int srcX = digit * digit_width;

        int drawX = startX + static_cast<int>(i * digit_width * scale);

        DrawRectExtendGraph(
            drawX, y,
            drawX + static_cast<int>(digit_width * scale),
            y + static_cast<int>(digit_height * scale),
            srcX, 0,
            digit_width, digit_height,
            num_image[0],
            TRUE
        );
    }
}

void DrawNumber::DrawCenter(int x, int y, int number, float scale)
{
    if (num_image.empty()) return;

    int image_width, image_height;
    GetGraphSize(num_image[0], &image_width, &image_height);

    int digit_width = image_width / 10;

    std::string numStr = std::to_string(number);

    int total_width = static_cast<int>(numStr.length() * digit_width * scale);

    Draw(x + total_width / 2, y, number, scale);
}