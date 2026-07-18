#include "DxLib.h"
#include <cmath>

#define WINDOW_X 720
#define WINDOW_Y 480

double ScreenFlipCount=120;
double RAIL_V=300.0;
double RAIL_Y = 300.0;

double x1 = -1000;
double x2 = -1000;
double x3 = -1000;
double x4 = -1000;
double b = -1000;

class Object {
public:
	double f, fd, fm;
	double ax, vx, x;
	double past_vx,past_ax;
	bool slip;
	Object(double A, double V, double X) {
		past_ax = A;
		ax = A;
		past_vx = V;
		vx = V;
		x = X;
		f = 0;
		fd = 0;
		fm = 0;
		slip = 0;
		Update();
	}
	void Update() {
		fd = -3 * x;
		if (fd < -500) {
			if (slip == 0) {
				x1 = x;
			}
			slip = 1;
		}
		if (slip) {
			fm = 200;
		}
		else {
			fm = -fd;
		}
		f = fd + fm;

		ax = f;
		vx += ax / ScreenFlipCount;
		if (vx > RAIL_V) {
			vx = RAIL_V;
			slip = 0;
			x4 = x;
		}
		x += vx / ScreenFlipCount;

		if (past_vx > 0 && vx <= 0) {
			b = x;
		}
		if (past_vx < 0 && vx >= 0) {
			x3 = x;
		}
		if (past_ax < 0 && ax >= 0) {
			x2 = x;
		}

		past_vx = vx;
		past_ax = ax;
	}
	void disp() {
		double box_x = 48;
		double box_y = 96;
		int color[5];
		color[0] = GetColor(0, 0, 0);
		color[1] = GetColor(192, 128, 128);
		color[2] = GetColor(255, 128, 128);
		color[3] = GetColor(128, 255, 128);
		color[4] = GetColor(128, 128, 255);
		SetFontThickness(8);
		char string[2][64] = {"すべり（等速運動）","単振動"};
		DrawString(8, 8, string[slip], color[slip]);
		DrawString(8, WINDOW_Y-25, "[Enter]座標リセット", GetColor(0,0,0));

		DrawString(8, 60, "合力", color[2]);
		DrawString(8, 80, "電磁力", color[3]);
		if (slip) {
			DrawString(8, 100, "動摩擦力", color[4]);
		}
		else {
			DrawString(8, 100, "静止摩擦力", color[4]);
		}
		

		DrawBoxAA(x - box_x / 2+WINDOW_X/2, RAIL_Y - box_y, x + box_x / 2+WINDOW_X/2, RAIL_Y , color[slip], 0, 8.0);
		DrawCircleAA(x + WINDOW_X / 2, RAIL_Y - box_y / 2, 3.0, 100, color[slip], 1);

		DrawLineAA(x + WINDOW_X / 2, RAIL_Y - box_y / 2-3, x + WINDOW_X / 2 + f/3, RAIL_Y - box_y / 2-3, color[2], 5.0);
		DrawLineAA(x + WINDOW_X / 2, RAIL_Y - box_y / 2+3, x + WINDOW_X / 2 + fd / 3, RAIL_Y - box_y / 2+3, color[3], 5.0);
		DrawLineAA(x + WINDOW_X / 2, RAIL_Y, x + WINDOW_X / 2 + fm / 3, RAIL_Y, color[4], 5.0);

		if (f > 0) {
			DrawLineAA(x + WINDOW_X / 2 + f / 3, RAIL_Y - box_y / 2 - 3, x + WINDOW_X / 2 + f / 3 - 15, RAIL_Y - box_y / 2 - 3 - 8, color[2], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + f / 3, RAIL_Y - box_y / 2 - 3, x + WINDOW_X / 2 + f / 3 - 15, RAIL_Y - box_y / 2 - 3 + 8, color[2], 5.0);
		}
		else if (f < 0) {
			DrawLineAA(x + WINDOW_X / 2 + f / 3, RAIL_Y - box_y / 2 - 3, x + WINDOW_X / 2 + f / 3 + 15, RAIL_Y - box_y / 2 - 3 - 8, color[2], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + f / 3, RAIL_Y - box_y / 2 - 3, x + WINDOW_X / 2 + f / 3 + 15, RAIL_Y - box_y / 2 - 3 + 8, color[2], 5.0);
		}
		if (fd > 0) {
			DrawLineAA(x + WINDOW_X / 2 + fd / 3, RAIL_Y - box_y / 2 + 3, x + WINDOW_X / 2 + fd / 3 - 15, RAIL_Y - box_y / 2 + 3 - 8, color[3], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + fd / 3, RAIL_Y - box_y / 2 + 3, x + WINDOW_X / 2 + fd / 3 - 15, RAIL_Y - box_y / 2 + 3 + 8, color[3], 5.0);

		}
		else if (fd < 0) {
			DrawLineAA(x + WINDOW_X / 2 + fd / 3, RAIL_Y - box_y / 2 + 3, x + WINDOW_X / 2 + fd / 3 + 15, RAIL_Y - box_y / 2 + 3 - 8, color[3], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + fd / 3, RAIL_Y - box_y / 2 + 3, x + WINDOW_X / 2 + fd / 3 + 15, RAIL_Y - box_y / 2 + 3 + 8, color[3], 5.0);
		}
		if (fm > 0) {
			DrawLineAA(x + WINDOW_X / 2 + fm / 3, RAIL_Y, x + WINDOW_X / 2 + fm / 3-15, RAIL_Y-8, color[4], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + fm / 3, RAIL_Y, x + WINDOW_X / 2 + fm / 3 - 15, RAIL_Y + 8, color[4], 5.0);
		}
		else if (fm < 0) {
			DrawLineAA(x + WINDOW_X / 2 + fm / 3, RAIL_Y, x + WINDOW_X / 2 + fm / 3 + 15, RAIL_Y - 8, color[4], 5.0);
			DrawLineAA(x + WINDOW_X / 2 + fm / 3, RAIL_Y, x + WINDOW_X / 2 + fm / 3 + 15, RAIL_Y + 8, color[4], 5.0);
		}
	}
};

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Object obj(0,RAIL_V,0);
	double rail_tani = 64.0;
	double rail_edge = -rail_tani;

	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);		//常時実行

	SetOutApplicationLogValidFlag(FALSE);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetMainWindowText("名門の森 力学・波動編第13問 シミュレーション ");
	ChangeFont("ＭＳ　ゴシック");
	SetFontThickness(2);
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);	//ウインドウ構築
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面描写
	SetMouseDispFlag(TRUE);

	DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	ScreenFlip();

	while (!ProcessMessage()) {
		rail_edge += RAIL_V / ScreenFlipCount;
		if (rail_edge >= 0) {
			rail_edge -= rail_tani;
		}
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			x1 = x2 = x3 = x4 = b = -1000;
		}


		DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(255, 255, 255), 1);
		for (int i = 0; i < 100; i++) {
			DrawLine(WINDOW_X/2, i * 16, WINDOW_X/2, i * 16 + 8, GetColor(128, 128, 128),1);
			DrawLine(x1 + WINDOW_X / 2, i * 16, x1 + WINDOW_X / 2, i * 16 + 8, GetColor(128, 128, 128), 1);
			DrawLine(x2 + WINDOW_X / 2, i * 16, x2 + WINDOW_X / 2, i * 16 + 8, GetColor(128, 128, 128), 1);
			DrawLine(x3 + WINDOW_X / 2, i * 16, x3 + WINDOW_X / 2, i * 16 + 8, GetColor(128, 128, 128), 1);
			DrawLine(x4 + WINDOW_X / 2, i * 16, x4 + WINDOW_X / 2, i * 16 + 8, GetColor(128, 128, 128), 1);
			DrawLine(b + WINDOW_X / 2, i * 16, b + WINDOW_X / 2, i * 16 + 8, GetColor(128, 128, 128), 1);
		}
		SetFontThickness(2);
		DrawString(WINDOW_X / 2 - 20, RAIL_Y + 8, " O", GetColor(0, 0, 0));
		DrawString(x1 + WINDOW_X / 2 - 20, RAIL_Y + 8, "x1", GetColor(0, 0, 0));
		DrawString(x2 + WINDOW_X / 2 - 20, RAIL_Y + 8, "x2", GetColor(0, 0, 0));
		DrawString(x3 + WINDOW_X / 2 - 20, RAIL_Y + 8, "x3", GetColor(0, 0, 0));
		DrawString(x4 + WINDOW_X / 2 - 20, RAIL_Y + 8, "x4", GetColor(0, 0, 0));
		DrawString(b  + WINDOW_X / 2 - 20, RAIL_Y + 8, " b", GetColor(0, 0, 0));
		for (double i = 0; i < 100; i+=1.0) {
			DrawLineAA(rail_edge + i * rail_tani, RAIL_Y, rail_edge + (i + 0.7) * rail_tani, RAIL_Y, GetColor(96,96, 96), 5.0);
		}

		obj.Update();
		obj.disp();

		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}