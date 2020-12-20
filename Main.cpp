#include "DxLib.h"
#include "math.h"

#include "Func.h"
#include "DXFunc.h"
#include "Game.h"

class BulletBase {
public:
	double x; // 弾の座標X
	double y;// 弾の座標Y
	double vx;// 弾の移動量X
	double vy;// 弾の移動量Y
	double angle; // 弾の角度
	double speed; // 弾の速度

	bool use; // 弾が使用されているか
};

const int BulletMax = 1024;
BulletBase bullets[BulletMax];



void AddBullet(double x, double y, double angle, double speed) 
{
	for (int ii = 0; ii < BulletMax; ii++) {
		if (!bullets[ii].use) {
			BulletBase* bullet = &bullets[ii];
			bullet->x = x;
			bullet->y = y;
			bullet->angle = angle / 57.29;
			bullet->speed = speed;

			bullet->vx = cos(bullet->angle) * speed;
			bullet->vy = sin(bullet->angle) * speed;

			bullet->use = true;
			return;
		}
	}
}


void InitBullet() {
	for (int ii = 0; ii < BulletMax; ii++) {
		BulletBase* bullet = &bullets[ii];
		memset(bullet, 0, sizeof(BulletBase));
	}
}

void DrawBullet() {
	for (int ii = 0; ii < BulletMax; ii++) {
		if (bullets[ii].use) {
			BulletBase* bullet = &bullets[ii];
			int Cr = GetColor(0, 0, 255); // 青色の値を取得
			DrawCircle(bullet->x, bullet->y, 20, Cr, TRUE);
		}
	}
}

void MoveBullet() {
	for (int ii = 0; ii < BulletMax; ii++) {
		if (bullets[ii].use) {
			BulletBase* bullet = &bullets[ii];

			bullet->x += bullet->vx;
			bullet->y += bullet->vy;
		}
	}
}

// ぴーちゃん
class Player {
public:
	double x;
	double y;
	int image;
};

Player player;

void InitPlayer() {
	player.x = 400;
	player.y = 400;
	player.image = LoadGraph("Resource\\pChan.png");
}
void DrawPlayer() {
	//DrawGraph(player.x, player.y, player.image, TRUE);

	DrawRotaGraph(player.x, player.y,
		0.25, 0.0,
		player.image, TRUE, FALSE);
	int Cr = GetColor(255, 0, 0); // 青色の値を取得
	DrawCircle(player.x, player.y, 3, Cr, TRUE);

}

void MovePlayer() {
	char Buf[256];
	GetHitKeyStateAll(Buf);

	int count = 0;
	if (Buf[KEY_INPUT_LEFT] == 1) {
		count++;
	}
	else if(Buf[KEY_INPUT_RIGHT] == 1) {
		count++;
	}
	
	if (Buf[KEY_INPUT_UP] == 1) {
		count++;
	} 
	else if (Buf[KEY_INPUT_DOWN] == 1) {
		count++;
	}

	double moveValue = 0;
	if (count == 1) {
		// 移動量10
		moveValue = 10;
	}
	else if (count ==2) {
		// 移動量14.14
		moveValue = 14.14/2; // 違う
	}

	if (Buf[KEY_INPUT_LEFT] == 1) {
		player.x += -moveValue;
	}
	else if (Buf[KEY_INPUT_RIGHT] == 1) {
		player.x += moveValue;
	}

	if (Buf[KEY_INPUT_UP] == 1) {
		player.y += -moveValue;
	}
	else if (Buf[KEY_INPUT_DOWN] == 1) {
		player.y += moveValue;
	}
}

void Collision() 
{
	for (int ii = 0; ii < BulletMax; ii++) {
		if (bullets[ii].use) {
			BulletBase* bullet = &bullets[ii];

			bullet->x;
			bullet->y;

			player.x;
			player.y;

			// 円と円の当たり判定
			// （弾の座標とプレイヤーの座標の距離）が (弾の半径　-　プレイヤの当たり判定)　以下であったら
			//　当たっている

			double x = bullet->x - player.x;
			double y = bullet->y - player.y;
			double r = 20.0 - 3.0;
			if (sqrt(x * x + y * y) < r) {
				// 当たっている　はず
				// プレイヤーが死ぬ
				// 爆発エフェクト　50ms
				// 初期位置に戻って
				// 無敵時間があって
				player.x = 400;
				player.y = 500;
			}

		}
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int windowX = 800;
	int windowY = 600;
	SetGraphMode(windowX, windowY, 32); // ウィンドウサイズ
	ChangeWindowMode(true); //ウィンドウモード
	SetOutApplicationLogValidFlag(false); //ログを吐かない
	SetAlwaysRunFlag(TRUE); //アクティブでなくても処理を行う
	SetMainWindowText("DanMakInfinity"); // ウィンドウタイトル
	
	if (DxLib_Init() == -1) {//DXライブラリ初期化
		return -1;
	}
	
	SetDrawScreen(DX_SCREEN_BACK); //ダブルバッファを有効に
	
	srand((unsigned)time(NULL)); //乱数初期化
	
	SetDrawMode(DX_DRAWMODE_BILINEAR);//バイナリア描画を使用する

	CGame* game = new CGame();
	game->Init();

	//クライアント領域矩形
	//game->setRect(CRect(0, 0, windowX, windowY));

	//game->createBattleScene();
	game->Main();

	DxLib_End(); // 終了処理

	return 0;				// ソフトの終了 
}
