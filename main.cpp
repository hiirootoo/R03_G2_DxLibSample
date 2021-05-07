//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"//DxLib���g���Ƃ��͕K�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"		//�Q�[���^�C�g��
#define GAME_WIDTH 1280					//�Q�[����ʂ̕�(�E�B�h�X)
#define GAME_HEIGHT 720					//�Q�[����ʂ̍���(�n�C�g)
#define GAME_COLOR 32					//�Q�[���̐F��

#define GAME_ICON_ID 333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@��(WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log,txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetWindowStyleMode(GAME_WINDOW_BAR);								//�E�B���h�E�o�[�̏��
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x�̐ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetAlwaysRunFlag(TRUE);


	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�l�p�̈ʒu�����߂�
	int x = 0;
	int y = 0;
	/*int x = GAME_WIDTH / 2;*/
	/*int y = GAME_HEIGHT / 2;*/
	
	//�l�p�̑傫�������߂�
	int width = 32;		//��
	int height = 32;	//����

	int AL = 0;
	int BL = 0;

	//���x
	int xSpeed = 1;
	int ySpeed = 1;

	//�~�̔��a�����߂�
	int radius = 100;

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		//��������L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
		{
			break;	//�������[�v�𔲂���
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ�Ƃ�
		{
			break;	//�������[�v�𔲂���
		}

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`��
		DrawBox(
			x, y, width, height,
			GetColor(255, 0, 0),
			TRUE
		);

		//�~��`��
		DrawCircle(
			x, y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);

		if (AL == 0) {
			x++; width++;
		}
		else if (AL == 1) {
			x-- + width--;
		}
		if (BL == 0) {
		y++; height++;
		}
		else if (BL == 1) {
			y--; height--;
		}

		if (x < 0)
			AL = 0;
		else if (x + 32 > GAME_WIDTH)
			AL = 1;

		if (y < 0)
			BL = 0;
		else if (y + 32 > GAME_HEIGHT)
			BL = 1;

		/*x += xSpeed;
		y += ySpeed;
		if (x<0 || x + width>GAME_WIDTH)
			xSpeed = -xSpeed;
		if (y<0 || y + height>GAME_HEIGHT)
			ySpeed - ySpeed;*/

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

		
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}