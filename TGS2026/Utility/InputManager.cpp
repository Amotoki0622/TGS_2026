#include "InputManager.h"
#include "DxLib.h"

// 静的メンバ変数の初期化
InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
	: now_key{}
	, old_key{}
{
}

// インスタンスを取得
InputManager* InputManager::GetInstance()
{
	if (instance == nullptr)
	{
		// 最初の1回だけオブジェクトを生成する
		instance = new InputManager();
	}

	// 実体を返す
	return instance;
}

// インスタンスの削除
void InputManager::DeleteInstance()
{
	if (instance != nullptr)
	{
		// メモリの開放
		delete instance;
		instance = nullptr;
	}
}

void InputManager::Update()
{
	// ゲームパッドの情報
	XINPUT_STATE input;

	// キーボード
	memcpy(old_key, now_key, sizeof(char) * 256);
	GetHitKeyStateAll(now_key);

	// ゲームパッドの状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &input);

	// メモリ領域をコピー
	memcpy(old_button, now_button, (sizeof(char) * PAD_BUTTON_MAX));
	for (int i = 0; i < PAD_BUTTON_MAX; i++)
	{
		// 現在押しているボタンの更新
		now_button[i] = input.Buttons[i];
	}

	// 左スティックの更新
	left_stick.x = input.ThumbLX;
	left_stick.y = input.ThumbLY;

	// 右スティックの更新
	right_stick.x = input.ThumbRX;
	right_stick.y = input.ThumbRY;

	// 左トリガーの更新
	left_trigger = (int)input.LeftTrigger;

	// 右トリガーの更新
	right_trigger = (int)input.RightTrigger;
}

// ボタンの入力状態を取得
eInputState InputManager::GetButtonInputState(int button)
{
	// ボタン入力が有効な範囲だったら処理を行う
	if (CheckButtonRange(button))
	{
		if (old_button[button] == TRUE)
		{
			if (now_button[button] == TRUE)
			{
				// 押し続けている
				return eInputState::eHold;
			}
			else
			{
				// 離した瞬間
				return eInputState::eRelease;
			}
		}
		else
		{
			if (now_button[button] == TRUE)
			{
				// 押した瞬間
				return eInputState::ePress;
			}
			else
			{
				// 入力無し
				return eInputState::eNone;
			}
		}
	}

	return eInputState::eNone;
}

// 左スティックの値を取得
Vector2D InputManager::GetLeftStick() const
{
	return left_stick;
}

// 右スティックの値を取得
Vector2D InputManager::GetRightStick() const
{
	return right_stick;
}

// 左トリガーの値を取得
int InputManager::GetLeftTrigger() const
{
	return left_trigger;
}

// 右トリガーの値を取得
int InputManager::GetRightTrigger() const
{
	return right_trigger;
}

// 左スティックの傾きの割合を取得
Vector2D InputManager::GetLeftStickTiltPercentage()
{
	// 左スティックの傾き
	Vector2D left_stick_tilt;

	// 左スティックの傾きを-1.0f ～ 1.0fの間に変換
	// x軸値の変換
	left_stick_tilt.x = left_stick.x / PAD_STICK_MAX;
	if (left_stick_tilt.x < -1.0f)
	{
		left_stick_tilt.x = -1.0f;
	}
	// y軸値の変換
	left_stick_tilt.y = left_stick.y / PAD_STICK_MAX;
	if (left_stick_tilt.y < -1.0f)
	{
		left_stick_tilt.y = -1.0f;
	}

	return left_stick_tilt;
}

// 右スティックの傾きの割合を取得
Vector2D InputManager::GetRightStickTiltPercentage()
{
	// 右スティックの傾き
	Vector2D right_stick_tilt;

	// 右スティックの傾きを-1.0f ～ 1.0fの間に変換
	// x軸値の変換
	right_stick_tilt.x = right_stick.x / PAD_STICK_MAX;
	if (right_stick_tilt.x < -1.0f)
	{
		right_stick_tilt.x = -1.0f;
	}
	// y軸値の変換
	right_stick_tilt.y = right_stick.y / PAD_STICK_MAX;
	if (right_stick_tilt.y < -1.0f)
	{
		right_stick_tilt.y = -1.0f;
	}

	return right_stick_tilt;
}

// 入力が有効な範囲かチェック
bool InputManager::CheckButtonRange(int button)
{
	// 現在の入力値がtrueかfalseか返す
	return (0 <= button && button < PAD_BUTTON_MAX);
}


eInputState InputManager::GetKeyInputState(int key) const
{
	if (CheckKeycodeRange(key))
	{
		if (old_key[key] == TRUE)
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::eHold;
			}
			else
			{
				return eInputState::eRelease;
			}
		}
		else
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::ePress;
			}
		}
	}
	return eInputState::eNone;
}

bool InputManager::CheckKeycodeRange(int key) const
{
	return (0 <= key && key < 256);
}
