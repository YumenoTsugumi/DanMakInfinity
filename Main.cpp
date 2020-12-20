#include "DxLib.h"
#include "math.h"

// test

class BulletBase {
public:
	double x; // �e�̍��WX
	double y;// �e�̍��WY
	double vx;// �e�̈ړ���X
	double vy;// �e�̈ړ���Y
	double angle; // �e�̊p�x
	double speed; // �e�̑��x

	bool use; // �e���g�p����Ă��邩
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
			int Cr = GetColor(0, 0, 255); // �F�̒l���擾
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

// �ҁ[�����
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
	int Cr = GetColor(255, 0, 0); // �F�̒l���擾
	DrawCircle(player.x, player.y, 3, Cr, TRUE);

}

void MovePlayer() {
	char Buf[256];
	GetHitKeyStateAll(Buf);

	/*  *
	14 *|
	  * | 10
	 *  |
	-----
	   10
	*/


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
		// �ړ���10
		moveValue = 10;
	}
	else if (count ==2) {
		// �ړ���14.14
		moveValue = 14.14/2; // �Ⴄ
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

			// �~�Ɖ~�̓����蔻��
			// �i�e�̍��W�ƃv���C���[�̍��W�̋����j�� (�e�̔��a�@-�@�v���C���̓����蔻��)�@�ȉ��ł�������
			//�@�������Ă���

			double x = bullet->x - player.x;
			double y = bullet->y - player.y;
			double r = 20.0 - 3.0;
			if (sqrt(x * x + y * y) < r) {
				// �������Ă���@�͂�
				// �v���C���[������
				// �����G�t�F�N�g�@50ms
				// �����ʒu�ɖ߂���
				// ���G���Ԃ�������
				player.x = 400;
				player.y = 500;
			}

		}
	}
}


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetGraphMode(800, 600, 32);	// ��ʃ��[�h�̐ݒ�
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h
	SetMainWindowText("DanMakInfinity"); // �Q�[���^�C�g��

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	InitBullet();
	InitPlayer();


	int count = 0;
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		if (count % 3 == 0) {
			AddBullet(400, 200, rand()%360, 5.0);
		}
		// �����蔻��
		Collision();

		// �v���O����������
		MoveBullet();
		DrawBullet();

		MovePlayer();
		DrawPlayer();

		count++;

		//60fps�@1�b�Ԃɉ�ʂ�60��X�V����
		// 1�b1000ms / 60 = 16.666ms�ɉ�ʂ��P��X�V����
		WaitTimer(16); // 16ms�҂�

		clsDx();
		ScreenFlip();
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
