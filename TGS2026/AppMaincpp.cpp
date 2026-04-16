#include "DxLib.h"
#include "Utility/InputManager.h"
#include "Utility/ResourceManager.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* manager = nullptr;
	int result = 0;

	try
	{
		manager = new SceneManager();

		manager->Initialize();

		InputManager* input = InputManager::GetInstance();

		while (ProcessMessage() != -1 && manager->LoopCheck())
		{
			input->Update();

			ClearDrawScreen();

			manager->Update();

			ScreenFlip();

			if (input->GetKeyInputState(KEY_INPUT_ESCAPE) == eInputState::eRelease)
			{
				break;
			}

		}
	}
	catch (std::string& error_text)
	{
		OutputDebugString(error_text.c_str());
		result = -1;
	}

	InputManager::DeleteInstance();

	ResourceManager::DeleteInstance();

	if (manager != nullptr)
	{
		manager->Finalize();
		delete manager;
	}

	DxLib_End();

	return 0;
}