#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

void PlayHandProcessing();


Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

int PlayerHand = 0;
bool playHand = false;

int ComHand = 0;

int winner = 0;
const int RESULT_DRAW = 0;
const int RESULT_LOSE = 1;
const int RESULT_WIN = 2;

const int HAND_GU = 0;
const int HAND_CHI = 1;
const int HAND_PA = 2;


// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();


	//========================================================
	// キーボードの入力取得
	//========================================================

	//if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
	//{
	//	// キーが押された瞬間の処理
	//}

	//if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	//{
	//	// キーが押されている間の処理
	//}

	//if (Engine::IsKeyboardKeyReleased(DIK_A))
	//{
	//	// キーが離された瞬間の処理
	//}



	if (playHand == false) {
		if (Engine::IsKeyboardKeyPushed(DIK_1)) {
			PlayerHand = HAND_GU;
			PlayHandProcessing();
		}
		else if (Engine::IsKeyboardKeyPushed(DIK_2)) {
			PlayerHand = HAND_CHI;
			PlayHandProcessing();
		}
		else if (Engine::IsKeyboardKeyPushed(DIK_3)) {
			PlayerHand = HAND_PA;
			PlayHandProcessing();
		}
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);
	
	Engine::LoadTexture("BackGround", "Res/GB.png");
	Engine::DrawTexture(0, 0, "BackGround");

	if (playHand) {
		switch (PlayerHand)
		{
		case 0:
			Engine::LoadTexture("PlayerHandTexture", "Res/janken_gu.png");
			break;
		case 1:
			Engine::LoadTexture("PlayerHandTexture", "Res/janken_choki.png");
			break;
		case 2:
			Engine::LoadTexture("PlayerHandTexture", "Res/janken_pa.png");
			break;
		}

		switch (ComHand)
		{
		case 0:
			Engine::LoadTexture("ComHandTexture", "Res/janken_gu.png");
			break;
		case 1:
			Engine::LoadTexture("ComHandTexture", "Res/janken_choki.png");
			break;
		case 2:
			Engine::LoadTexture("ComHandTexture", "Res/janken_pa.png");
			break;
		}

		switch (winner)
		{
		case 0:
			Engine::DrawFont(320, 240, "DRAW", Large, Black);
			break;
		case 1:
			Engine::DrawFont(320, 240, "LOSE...", Large, Black);
			break;
		case 2:
			Engine::DrawFont(320, 240, "WIN!!", Large, Black);
			break;
		}

		Engine::DrawTexture(220, 260, "PlayerHandTexture", 255, 0, 0.8f, 0.8f);
		Engine::DrawTexture(220, 20, "ComHandTexture", 255, 180, 0.8f, 0.8f);
	}

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

void PlayHandProcessing() {
	int winnerTemp = 0;
	// 手を出したかのトリガー
	playHand = true;

	// コンピューターの手
	srand((unsigned)time(NULL));
	ComHand = rand() % 3;

	// 結果
	winner = (PlayerHand - ComHand + 3) % 3;
}