//#include "AppHost.h"
//#include "App.h"
//
//int main(int argc, char* argv[])
//{
//	AppHost appHost;
//	App app;
//
//	// CHECKME: 여기 main 엔트리 포인트 이후에 점검 필요
//	Result<void> result = appHost.Startup();
//	if (!result.IsSuccess())
//	{
//		return 0; // NOTE: 여기에 뭔가 로그라도 찍어줘야 하지 않을까...?
//	}
//
//	result = appHost.Run(app);
//	if (!result.IsSuccess())
//	{
//		return 0; // NOTE: 여기에 뭔가 로그라도 찍어줘야 하지 않을까...?
//	}
//
//	result = appHost.Shutdown();
//	if (!result.IsSuccess())
//	{
//		return 0; // NOTE: 여기에 뭔가 로그라도 찍어줘야 하지 않을까...?
//	}
//
//	return 0;
//}


#include <raylib.h>

int main(void)
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		if (GetKeyStateEx(KEY_A) == STATE_NONE)
		{
			TraceLog(LOG_ERROR, "STATE_NONE");
		}
		else if (GetKeyStateEx(KEY_A) == STATE_PRESSED)
		{
			TraceLog(LOG_ERROR, "STATE_PRESSED");
		}
		else if (GetKeyStateEx(KEY_A) == STATE_HELD)
		{
			TraceLog(LOG_ERROR, "STATE_HELD");
		}
		else if (GetKeyStateEx(KEY_A) == STATE_RELEASED)
		{
			TraceLog(LOG_ERROR, "STATE_RELEASED");
		}

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}