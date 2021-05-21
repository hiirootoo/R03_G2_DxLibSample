#pragma once

//�w�b�_�t�@�C����ǂݍ���
#include "game.h"

//�L�[�{�[�h�̃w�b�h�t�@�C��

//�}�N����`
#define KEY_KIND_MAX 256	//�擾����L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD {
	char TempKeyState[ KEY_KIND_MAX ];		//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];			//�S�ẴL�[�̓��͎��Ԃ��Ǘ�
	int OldAllKeyState[KEY_KIND_MAX];		//�ȑO�̑S�ẴL�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyborad;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);							//�S�ẴL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KEY_INPUT);						//����̃L�[�������Ă��邩�H
extern BOOL KeyUp(int KEY_INPUT);						//����̃L�[�������Ă��邩�H
extern BOOL KeyClick(int KEY_INPUT);					//����̃L�[���N���b�N�������H
extern BOOL KeyDownKeep(int KEY_INPUT_, int millSec);	//����̃L�[���w��̃~���b�����������Ă��邩�H