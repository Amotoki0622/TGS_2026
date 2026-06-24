#include "SpikeTrap.h"
#include "../../Player/Player.h" 
#include <cmath>

SpikeTrap::SpikeTrap()
    : TrapObject(0.0f, 0.0f, TrapType::SpikeTrap)
    , radius(0.0f)
    , spikeImage(-1)
{
    font[0] = CreateFontToHandle("廻想体 ネクスト UP B", 35, 6);
}

SpikeTrap::SpikeTrap(float x, float y, float radius)
    : TrapObject(x, y, TrapType::SpikeTrap), radius(radius)
{
    Initialize();

    // 画像の読み込み（右側のトゲあり画像だけを使用）
    // int handles[2];
    // LoadDivGraph("Resource/Images/Trap/SpikeTrap/SpikeTrap2.png", 2, 2, 1, 735, 930, handles);
    // spikeImage = handles[1];
}

void SpikeTrap::Initialize()
{
    this->x = this->location.x;
    this->y = this->location.y;

    this->detected = false;

    // 画像読み込み
    if (spikeImage == -1)
    {
        int handles[2];
        LoadDivGraph("Resource/Images/Trap/SpikeTrap/SpikeTrap2.png", 2, 2, 1, 735, 930, handles);
        spikeImage = handles[1];
    }

    spike_se = LoadSoundMem("Resource/Sounds/SE/object/spike_trap/spike_damage.mp3");
    ChangeVolumeSoundMem(70, spike_se);
}

void SpikeTrap::Update(const Player& player, float delta_second) {
    int px, py;
    player.GetLocation(px, py);

    float dist = sqrtf(powf((float)px - x, 2) + powf((float)py - y, 2));
    bool isTouching = (dist < radius);

    // トゲを踏んだ瞬間
    if (isTouching && !detected) {
        detected = true; // どちらの状態でも検知フラグは立てる

        // プレイヤーが影状態かどうかを取得
        bool isShadow = (player.GetState() == Player::State::Shadow);

        if (isShadow) {
            // 影状態の場合：手数は減らさず、黒い血しぶき（影のエフェクト）だけ出す
            OutputDebugString("トゲを踏んだが、影状態なのでノーダメージ！\n");

            // 文字エフェクトを生成（トラップの少し上からスタート）
            textEffects.push_back({ x, y - 60.0f, 1.0f, 255.0f });

            // 黒い血しぶき（粒）エフェクトを生成（四方八方へ）
            for (int i = 0; i < 12; i++) {
                float angle = (float)(rand() % 360) * 3.141592f / 180.0f;
                float speed = (float)(rand() % 150 + 50);

                BloodParticle p;
                p.x = x;
                p.y = y - 10.0f;
                p.vx = cosf(angle) * speed;
                p.vy = sinf(angle) * speed - 30.0f;
                p.size = (float)(rand() % 6 + 4);
                p.timer = (float)(rand() % 4 + 4) / 10.0f;
                p.alpha = 255.0f;
                // 完全に真っ黒(0,0,0)より少しグレーを入れると見栄えが良いです
                p.color = GetColor(30, 30, 30);

                bloodParticles.push_back(p);
            }
        }
        else {
            // 通常状態の場合：手数を減らし、文字と赤い血しぶきを出す
            Player& mutablePlayer = const_cast<Player&>(player);
            PlaySoundMem(spike_se, DX_PLAYTYPE_BACK);
            mutablePlayer.DecreaseMoveCount();
            OutputDebugString("トゲを踏んだ！手数が減りました。\n");

            // 文字エフェクトを生成（トラップの少し上からスタート）
            textEffects.push_back({ x, y - 60.0f, 1.0f, 255.0f });

            // 赤い血しぶき（粒）エフェクトを生成（四方八方へ）
            for (int i = 0; i < 12; i++) {
                float angle = (float)(rand() % 360) * 3.141592f / 180.0f;
                float speed = (float)(rand() % 150 + 50);

                BloodParticle p;
                p.x = x;
                p.y = y - 10.0f;
                p.vx = cosf(angle) * speed;
                p.vy = sinf(angle) * speed - 30.0f;
                p.size = (float)(rand() % 6 + 4);
                p.timer = (float)(rand() % 4 + 4) / 10.0f;
                p.alpha = 255.0f;
                p.color = GetColor(200, 10, 10); // 最初の時の赤色

                bloodParticles.push_back(p);
            }
        }
    }

    if (!isTouching) {
        detected = false;
    }

    // 文字エフェクトのアニメーション更新（上にフワッ）
    for (auto it = textEffects.begin(); it != textEffects.end(); ) {
        it->timer -= delta_second;
        it->y -= 70.0f * delta_second;

        if (it->timer < 0.5f) {
            it->alpha = (it->timer / 0.5f) * 255.0f;
        }

        if (it->timer <= 0.0f) {
            it = textEffects.erase(it);
        }
        else {
            ++it;
        }
    }

    // 血しぶき（粒）エフェクトのアニメーション更新（重力で落ちる）
    for (auto it = bloodParticles.begin(); it != bloodParticles.end(); ) {
        it->timer -= delta_second;
        it->x += it->vx * delta_second;
        it->y += it->vy * delta_second;
        it->vy += 200.0f * delta_second; // 重力

        it->alpha = (it->timer / 0.5f) * 255.0f;
        if (it->alpha > 255.0f) it->alpha = 255.0f;

        if (it->timer <= 0.0f || it->alpha <= 0.0f) {
            it = bloodParticles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void SpikeTrap::Draw() const {
    // 常にトゲ画像を定位置に描画
    if (spikeImage != -1) {
        float scale = (radius * 1.6f) / 740.0f;
        DrawRotaGraph((int)x, (int)y, (double)scale, 0.0, spikeImage, TRUE);
    }

    // 血しぶきの描画
    for (const auto& p : bloodParticles) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)p.alpha);
        // 粒が持っている固有の色（p.color）を使って描画する
        DrawCircle((int)p.x, (int)p.y, (int)p.size, p.color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    //  文字（-1）の描画
    for (const auto& e : textEffects) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)e.alpha);
        DrawStringToHandle((int)e.x - 15, (int)e.y, "-1", GetColor(255, 30, 30), font[0]);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void SpikeTrap::SetRadius(float r)
{
    this->radius = r;
}