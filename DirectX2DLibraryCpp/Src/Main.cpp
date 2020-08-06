#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

enum GameMainProcessing
{
	CountDown,
	Playing,
	Result,
};

GameMainProcessing MainProcessing = CountDown;


Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

float timer = 0.0f;
char buf[24];

int countDownSwitch = 0;

int counter = 0;
char printCounter[24];


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

	// １秒の計測
	timer = clock() / CLOCKS_PER_SEC;
	snprintf(buf, 24, "%3f", timer);
	puts(buf);
	countDownSwitch = timer;
	
	snprintf(printCounter, 24, "%d", counter);
	puts(printCounter);

	//最初にカウントダウンを表示
	//カウントダウンが終わったらキーの入力の受付を開始
	//現在何発連打されているかを表示
	//一定時間経過したら終了 = > 終了した旨と結果を表示

	switch (MainProcessing)
	{
	case CountDown:
		break;
	case Playing:
		if (Engine::IsKeyboardKeyPushed(DIK_SPACE)) {
			counter++;
		}
		break;
	case Result:
		break;
	default:
		break;
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// 適当な背景
	Engine::LoadTexture("BackGround", "Res/GB.png");
	Engine::DrawTexture(0, 0, "BackGround");

	// 確認用のタイマー
	Engine::DrawFont(0.0f, 0.0f, buf, FontSize::Small, FontColor::White);

	// カウントダウン
	if (MainProcessing == CountDown) {
		switch (countDownSwitch)
		{
		case 1:
			Engine::DrawFont(300.0f, 100.0f, "３", FontSize::Large, FontColor::Red);
			break;
		case 2:
			Engine::DrawFont(300.0f, 100.0f, "２", FontSize::Large, FontColor::Red);
			break;
		case 3:
			Engine::DrawFont(300.0f, 100.0f, "１", FontSize::Large, FontColor::Red);
			break;
		case 4:
			Engine::DrawFont(300.0f, 100.0f, "GO!", FontSize::Large, FontColor::Red);
			break;
		case 5:
			MainProcessing = Playing;
		default:
			break;
		}
	}

	// カウンターの表示
	if (MainProcessing == Playing) {
		Engine::DrawFont(100.0f, 0.0f, printCounter, FontSize::Small, FontColor::Black);
	}

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
