#pragma once
#pragma once

#include "Vector2D.h"

#define   D_KEYCODE_MAX  (256)

enum class eInputState : unsigned char
{
	eNone,
	ePress,
	eRelease,
	eHeld,
};

class InputManager
{
private:
	static InputManager* instance;

	char now_key[D_KEYCODE_MAX] = {};  // 現在のフレーム入力値
	char old_key[D_KEYCODE_MAX] = {};  // 過去のフレーム入力値

	bool now_button[16]; // 現在のフレーム入力値
	bool old_button[16]; // 過去のフレーム入力値
	float trigger[2];    // 左右トリガーの入力値
	Vector2D stick[2];   // 左右スティックの入力値

private:
	// コンストラクタをprivateにする
	InputManager() = default;
	// コピーガードの作成
	// コピーコンストラクタ禁止
	InputManager(const InputManager&) = delete;
	// 代入演算子禁止
	InputManager& operator = (const InputManager&) = delete;

public:
	// デストラクタ
	~InputManager() = default;

public:
	// コンストラクタとデストラクタとして機能する静的メンバ関数を作成する
	// 初回呼び出しの際に新しいオブジェクトを生成し、関数は常にそのインスタンスを返すようにする
	static InputManager* GetInstance();
	static void DeleteInstance();

public:
	// 更新処理
	void Update();

public:
	// 引数(keycode)キーの状態を取得して返す
	eInputState GetKeyInputState(int keycode);

	// ボタンの入力取得処理
	bool GetButton(int button);     // 押し続けている間
	bool GetButtonDown(int button); // 押した瞬間
	bool GetButtonUp(int button);   // 離した瞬間

	// トリガー入力取得処理
	float GetLeftTrigger();  // 左トリガー
	float GetRightTrigger(); // 右トリガー

	// スティック入力取得処理
	Vector2D GetLeftStick();  // 左スティック
	Vector2D GetRightStick(); // 右スティック

private:
	// キー配列範囲チェック
	bool CheckKeycodeRange(int keycode);

	// ボタン配列範囲チェック
	bool CheckButtonRange(int button);
};

// 静的メンバ関数の中では、静的メンバ変数しか呼べない
// 普通のメンバ関数の中で、静的メンバを呼ぶのはOK