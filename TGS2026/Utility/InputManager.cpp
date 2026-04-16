#include "InputManager.h"
#include "DxLib.h"

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::GetInstance()
{
	// 初回呼び出しの際に新しいオブジェクトを生成
	if (instance == nullptr)
	{
		instance = new InputManager();
	}

	// 常にインスタンスを返す
	return instance;
}

// instance削除処理
void InputManager::DeleteInstance()
{
	// インスタンス削除
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// 更新処理
void InputManager::Update()
{
	// キー入力値の更新
	memcpy(old_key, now_key, (sizeof(char) * D_KEYCODE_MAX));
	GetHitKeyStateAll(now_key);

	// XInputコントローラーの入力値を取得する
	XINPUT_STATE input_state = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input_state);

	// ボタン入力値の更新
	for (int i = 0; i < 16; i++)
	{
		old_button[i] = now_button[i];
		now_button[i] = (bool)input_state.Buttons[i];
	}

	// トリガー入力値の更新(0.0f～1.0fに範囲を制限する)
	trigger[0] = (float)input_state.LeftTrigger / (float)UCHAR_MAX;
	trigger[1] = (float)input_state.RightTrigger / (float)UCHAR_MAX;

	// 左スティック入力値の更新(-1.0f～1.0fに範囲を制限する)
	if (input_state.ThumbLX > 0.0f)
	{
		stick[0].x = (float)input_state.ThumbLX / (float)SHRT_MAX;
	}
	else
	{
		stick[0].x = -((float)input_state.ThumbLX / (float)SHRT_MIN);
	}
	if (input_state.ThumbLY > 0.0f)
	{
		stick[0].y = (float)input_state.ThumbLY / (float)SHRT_MAX;
	}
	else
	{
		stick[0].y = -((float)input_state.ThumbLY / (float)SHRT_MIN);
	}

	// 右スティック入力値の更新(-1.0f～1.0fに範囲を制限する)
	if (input_state.ThumbRX > 0.0f)
	{
		stick[1].x = (float)input_state.ThumbRX / (float)SHRT_MAX;
	}
	else
	{
		stick[1].x = -((float)input_state.ThumbRX / (float)SHRT_MIN);
	}
	if (input_state.ThumbRY > 0.0f)
	{
		stick[1].y = (float)input_state.ThumbRY / (float)SHRT_MAX;
	}
	else
	{
		stick[1].y = -((float)input_state.ThumbRY / (float)SHRT_MIN);
	}
}

// 引数(keycode)キーの状態を取得して返す
eInputState InputManager::GetKeyInputState(int keycode)
{
	if (CheckKeycodeRange(keycode))
	{
		if (old_key[keycode] == TRUE)
		{
			if (now_key[keycode] == TRUE)
			{
				return eInputState::eHeld;
			}
			else
			{
				return eInputState::eRelease;
			}
		}
		else
		{
			if (now_key[keycode] == TRUE)
			{
				return eInputState::ePress;
			}
			else
			{
				return eInputState::eNone;
			}
		}
	}

	return eInputState::eNone;
}

// ボタンの入力取得処理（押し続けている間）
bool InputManager::GetButton(int button)
{
	return CheckButtonRange(button) && (now_button[button] && old_button[button]);
}

// ボタンの入力取得処理（押した瞬間）
bool InputManager::GetButtonDown(int button)
{
	return CheckButtonRange(button) && (now_button[button] && !old_button[button]);
}

// ボタンの入力取得処理（離した瞬間）
bool InputManager::GetButtonUp(int button)
{
	return CheckButtonRange(button) && (!now_button[button] && old_button[button]);
}

// 左トリガー入力取得処理
float InputManager::GetLeftTrigger()
{
	return trigger[0];
}

// 右トリガー入力取得処理
float InputManager::GetRightTrigger()
{
	return trigger[1];
}

// 左スティック入力取得処理
Vector2D InputManager::GetLeftStick()
{
	return stick[0];
}

// 右スティック入力取得処理
Vector2D InputManager::GetRightStick()
{
	return stick[1];
}

// キー配列範囲チェック
bool InputManager::CheckKeycodeRange(int keycode)
{
	return (0 <= keycode && keycode < D_KEYCODE_MAX);
}

// ボタン配列範囲チェック
bool InputManager::CheckButtonRange(int button)
{
	return (0 <= button && button < 16);
}
