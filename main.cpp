
//�w�b�_�[�t�@�C���ǂݍ���
#include "game.h"
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�\����

//�L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;		//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];			//�摜�̏ꏊ�i�p�X�j
	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int speed = -1;			//�ړ����x

	RECT coll;				//�����蔻��̗̈�i�l�p�j
	BOOL IsDraw = FALSE;	//�摜���`��ł���H
};

//����̍\����
struct MOVIE
{
	int handle = -1;		//����̃n���h��
	char path[255];			//����̏ꏊ

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int Volume = 255;		//�{�����[���i�ŏ��j0�`255�i�ő�j
};

//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���V�[��

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//��ʂ̐ؑւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int fadeTimeMill = 2000;						//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = fadeTimeMax;				//�t�F�[�h�C���̃J�E���^MAX


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

VOID ChangeScene(GAME_SCENE scene);		//�V�[����؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);		//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);				//�����蔻��

BOOL Collision(RECT player, RECT Goal);

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@��(WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log,txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x�̐ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");	//�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);	//�����ǂݍ���

	//�摜���ǂݍ��܂�Ȃ������Ƃ��́A�G���[�i-1�j������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			playMovie.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return -1;		//�G���[�I��
	}

	//����̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(player.path, ".\\image\\Player.png");		//�p�X�̃R�s�[
	player.handle = LoadGraph(player.path);	//�摜��ǂݍ���

	//�摜���ǂݍ��܂�Ȃ������Ƃ��́A�G���[�i-1�j������
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			player.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return - 1;		//�G���[�I��
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(player.handle, &player.width, &player.height);

	//�v���C���[��������
	player.x = 0;		//������
	player.y = GAME_HEIGHT / 2 - player.height / 2;		//������
	player.speed = 500;
	player.IsDraw = TRUE;	//�`��ł���I

	//�����蔻����X�V����
	CollUpdatePlayer(&player);		//�v���C���[�̓����蔻��̃A�h���X

	//�S�[���̉摜��ǂݍ���
	strcpyDx(Goal.path, ".\\image\\goal.png");		//�p�X�̃R�s�[
	Goal.handle = LoadGraph(Goal.path);	//�摜��ǂݍ���

	//�摜���ǂݍ��܂�Ȃ������Ƃ��́A�G���[�i-1�j������
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			Goal.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return -1;		//�G���[�I��
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//�S�[����������
	Goal.x = GAME_WIDTH - Goal.width;		//������
	Goal.y = 0;		//������
	Goal.speed = 500;
	Goal.IsDraw = TRUE;	//�`��ł���I

	//�����蔻����X�V����
	CollUpdate(&Goal);		//�S�[���̓����蔻��̃A�h���X

	//�������[�v
	while (1)
	{
		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) { break; }		//�������[�v�𔲂���
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

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

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;			//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;		//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();


		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);	//�摜������������폜
	DeleteGraph(player.handle);		//�摜���������ォ��폜
	DeleteGraph(Goal.handle);		//�摜���������ォ��폜

		
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
		GameScene = scene;	//�V�[����؂�ւ�
		IsFadeIn = FALSE;				//�t�F�[�h�C�����Ȃ�
		IsFadeOut = TRUE;				//�t�F�[�h�A�E�g����
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
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}
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
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�v���C���[���S�[���ɓ��������Ƃ���
	if (Collision(player.coll,Goal.coll) == TRUE) {
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}
	
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V����
	CollUpdate(&Goal);

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//����w�i��`��

	//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//������Đ�
	}
	//�����`��i�摜���������΂��j
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�v���C���[��`��
	if (player.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//�f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�S�[����`��
		if (Goal.IsDraw == TRUE)
		{
			//�摜��`��
			DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

			//�f�o�b�N�̂Ƃ��́A�����蔻��̗̈��`��
			if (GAME_DEBUG == TRUE)
			{
				//�l�p��`��
				DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}

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
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}
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
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;		//�J�E���^������
			IsFadeIn = FALSE;				//�t�F�[�h�C�������I��
		}
	}
	
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;		//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();			//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="player">��`A</param>
/// <param name="Goal">��`B</param>
/// <returns>����������TRUE/������Ȃ��Ȃ�FALSE</returns>
BOOL Collision(RECT player,RECT Goal) {
	if (player.left < Goal.right
		&&player.right > Goal.left
		&&player.top < Goal.bottom
		&&player.bottom > Goal.top) 
	{
			return TRUE;
	}
	else
	{
		return FALSE;
	}
}