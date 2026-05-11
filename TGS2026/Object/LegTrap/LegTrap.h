//#pragma once
//#include "../Detective/DetectiveObject.h"
//
//class LegTrap : public DetectiveObject {
//private:
//    int openImage;   // 開いている画像
//    int closedImage; // 閉じている画像
//
//    // 当たり判定の広さ（画像サイズに合わせて調整してください）
//    const float range = 30.0f;
//
//public:
//    LegTrap(float x, float y)
//        : DetectiveObject(x, y, DetectiveType::LegTrap) {
//        // 画像の読み込み
//        openImage = LoadGraph("Resource/Images/Object/trap_open.png");
//        closedImage = LoadGraph("Resource/Images/Object/trap_closed.png");
//    }
//
//    // デストラクタで画像を解放（必要に応じて）
//    virtual ~LegTrap() {
//        // DeleteGraph(openImage);
//        // DeleteGraph(closedImage);
//    }
//
//    void Update(const Player& player, float delta_second) override {
//        // すでに発動（発見状態）なら判定しない
//        if (detected) return;
//
//        // プレイヤーの中心座標を取得（仮に GetPositionX / Y としています）
//        float px = player.GetX();
//        float py = player.GetY();
//
//        // シンプルな距離判定（円判定）または矩形判定
//        // ここではカチッと動く操作感に合わせて、距離で判定します
//        float dx = x - px;
//        float dy = y - py;
//        float distSq = dx * dx + dy * dy;
//
//        if (distSq < range * range) {
//            detected = true;
//            // SEを鳴らすならここ
//            // PlaySoundMem(trapSE, DX_PLAYTYPE_BACK);
//        }
//    }
//
//    void Draw() const override {
//        // detected（発見フラグ）が立っていたら「閉じた画像」を表示
//        int handle = detected ? closedImage : openImage;
//
//        if (handle != -1) {
//            // 画像の中心が(x, y)になるように描画
//            DrawRotaGraph((int)x, (int)y, 1.0, 0.0, handle, TRUE);
//        }
//    }
//};