#include "SpikeTrap.h"
#include "../../Player/Player.h" // Playerの関数を呼ぶために必要
#include <cmath>

// コンストラクタ
SpikeTrap::SpikeTrap(float x, float y, float radius)
    : TrapObject(x, y, TrapType::SpikeTrap), // 親クラスの初期化
    radius(radius),                          // 半径の設定
    state(SpikeState::Off)                   // 最初はOff（安全）からスタート
{
    // 画像を分割して読み込む（横2枚、縦1枚）
    // 1枚あたりのサイズは 横768(1536/2), 縦1024
    int handles[2];
    LoadDivGraph("Resource/Images/Trap/SpikeTrap/SpikeTrap2.png", 2, 2, 1, 735, 930, handles);

    closedImage = handles[0];  // 左側：トゲなし
    openImage = handles[1];    // 右側：トゲあり
}

// 更新処理
void SpikeTrap::Update(const Player& player, float delta_second) {
    int px, py;
    player.GetLocation(px, py);

    // 1. 移動監視（Toggle）は常に実行
    if (lastPlayerX != -1 && (px != lastPlayerX || py != lastPlayerY)) {
        ToggleState();
        // 離れた後に戻ってきたらまたダメージを食らわせたいなら、ここで detected = false にする
        // 1回きりにしたいならこのままでOK
    }
    lastPlayerX = px;
    lastPlayerY = py;

    // 2. 「今」プレイヤーが範囲内にいるかをチェック
    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    bool isTouching = (dist < radius); // 範囲内ならtrue

    // 3. 当たり判定（手数を減らす処理）
    // 「トゲが出ていて」かつ「範囲内にいて」かつ「まだ手数を減らしていない」場合
    if (state == SpikeState::On && isTouching && !detected) {

        // 【重要】手数を減らす
        // playerはconst参照なので、中身を変えるためにキャストが必要です
        Player& mutablePlayer = const_cast<Player&>(player);
        mutablePlayer.DecreaseMoveCount();

        // 1回減らしたらdetectedをtrueにして、連続で減るのを防ぐ
        detected = true;

        // デバッグログ
        OutputDebugString("トゲに当たった！手数が減りました。\n");
    }
    // 4. 【重要】もし範囲外に出たら、detectedをリセットする（これで「離れた」とみなす）
    if (!isTouching) {
        detected = false;
    }
}

// 描画処理
void SpikeTrap::Draw() const {
    // 1. 状態に応じた画像を選択
    int handle = (state == SpikeState::On) ? openImage : closedImage;

    if (handle != -1) {
        // 画像の元の幅（768px）に対して、ゲーム上のサイズ（radius * 2）に合わせる倍率を計算
        // もし大きすぎる場合は、ここの倍率を 0.1 とか固定値にしてもOKです
        float scale = (radius * 1.6f) / 740.0f;

        // 画像の中心(x, y)で描画
        DrawRotaGraph((int)x, (int)y, (double)scale, 0.0, handle, TRUE);
    }

    // --- 動作確認用（後で消してOK） ---
    // 踏んでいる間だけ「HIT!」を画像の上に重ねて出すと分かりやすいです
    if (detected) {
        DrawFormatString((int)x - 20, (int)y - 50, GetColor(255, 255, 0), "HIT!");
    }

    //unsigned int color;

    //// 今まさに踏んでダメージが発生しているなら「黄色（HIT）」
    //if (detected) {
    //    color = GetColor(255, 255, 0);
    //}
    //else if (state == SpikeState::On) {
    //    color = GetColor(255, 0, 0); // 赤
    //}
    //else {
    //    color = GetColor(0, 0, 255); // 青
    //}

    //// --- 円の描画 ---
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
    //DrawCircle((int)x, (int)y, (int)radius, color, TRUE);
    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    //DrawCircle((int)x, (int)y, (int)radius, color, FALSE);

    //// --- テキスト表示 ---
    //if (detected) {
    //    DrawFormatString((int)x - 20, (int)y - 10, color, "HIT!");
    //}
    //else {
    //    // 離れたら ON / OFF 表示に戻る
    //    const char* statusText = (state == SpikeState::On) ? "ON" : "OFF";
    //    DrawFormatString((int)x - 20, (int)y - 10, color, statusText);
    //}
}

// 状態反転処理
void SpikeTrap::ToggleState() {
    if (state == SpikeState::On) {
        state = SpikeState::Off;
    }
    else {
        state = SpikeState::On;
    }
}