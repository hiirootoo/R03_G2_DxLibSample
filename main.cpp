//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g���Ƃ��͕K�v
#include "keyboard.h"	//�L�[�{�[�h�̏���

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"		//�Q�[���^�C�g��
#define GAME_WIDTH 1280					//�Q�[����ʂ̕�(�E�B�h�X)
#define GAME_HEIGHT 720					//�Q�[����ʂ̍���(�n�C�g)
#define GAME_COLOR 32					//�Q�[���̐F��

#define GAME_ICON_ID 333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��

//�񋓌^
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};	//�Q�[���̃V�[��

//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//��ʂ̐ؑւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

//�v���g�^�C�v�錾
VOID Title(VOID);			//�^�C�g�����
VOID TitleProc(VOID);		//�^�C�g�����(����)
VOID TitleDraw(VOID);		//�^�C�g�����(�`��)

VOID Play(VOID);			//�v���C���
VOID PlayProc(VOID);		//�v���C���(����)
VOID PlayDraw(VOID);		//�v���C���(�`��)

VOID End(VOID);				//�G���h���
VOID EndProc(VOID);			//�G���h���(����)
VOID EndDraw(VOID);			//�G���h���(�`��)

VOID Change(VOID);			//�؂�ւ����
VOID ChangeProc(VOID);		//�؂�ւ����(����)
VOID ChangeDraw(VOID);		//�؂�ւ����(�`��)

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

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�~�̒��S�_
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//�~�̔��a
	int radius = 50;

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�������[�v
	while (1)
	{
		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) { break; }		//�������[�v�𔲂���
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�L�[����
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			Y--;	//��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			Y++;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			X--;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			X++;	//�E�Ɉړ�
		}

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

		
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}