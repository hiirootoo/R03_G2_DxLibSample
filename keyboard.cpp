//�L�[�{�[�h�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "keyboard.h"

//�O���[�o���ϐ�
KEYBOARD keyboard;

//�֐�

/// <summary>
/// �L�[�̓��͏�Ԃ��X�V����
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//���O�̃L�[���͂��Ƃ��Ă���
	for (int i = 0; i < KEY_KIND_MAX; i++) {
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];
	}

	//���ׂẴL�[�̓��͏�Ԃ𓾂�
	GetHitKeyStateAll(keyboard.TempKeyState);

	for (int i = 0; i < KEY_KIND_MAX; i++) {
		//�L�[�R�[�h�������Ă���Ƃ�
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;		//�����Ă��鎞�Ԃ����Z
		}
		else
		{
			keyboard.AllKeyState[i] = 0;	//�����Ă��鎞�Ԃ�0�ɖ߂�
		}
	}

	return;
}

/// <summary>
/// �L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�����Ă�����TRUE</returns>
BOOL KeyDown(int KEY_INPUT)
{
	if (keyboard.AllKeyState[KEY_INPUT] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[���グ�Ă��邩�A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�グ�Ă�����TRUE</returns>
BOOL KeyUp(int KEY_INPUT)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT] != 0			//�ȑO�͉����Ă���
		&& keyboard.AllKeyState[KEY_INPUT] == 0			//���݂͉����Ă��Ȃ�
		) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[���N���b�N�������A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT">�L�[�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL KeyClick(int KEY_INPUT)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT] != 0			//�ȑO�͉����Ă���
		&& keyboard.AllKeyState[KEY_INPUT] == 0			//���݂͉����Ă��Ȃ�
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f���� 
/// </summary>
/// <param name="MillTime">�L�[�R�[�h</param>
/// <returns>�L�[�����������Ă���~���b</returns>
BOOL KeyDownKeep(int KEY_INPUT, int MilliTime)
{
	//�P�b�͂P�O�O�O�~���b
	float MilliSec = 1000.0f;

	//���������Ă��鎞�Ԃ́A�~���b�����e�o�r�l
	//��j�P�T�O�O�~���b����/�P�O�O�O�~�����P�D�T�b x �U�O�e�o�r���X�O
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT] > UpdateTime)
	{
		return TRUE;		//���������Ă���
	}
	else
	{
		return FALSE;		//���������Ă��Ȃ�
	}
}