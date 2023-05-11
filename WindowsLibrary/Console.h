// ----------------------------------------------------------------------------
//! @file	Console.h
//! @brief	�R���\�[���Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @enum	ForeColor
	//! @brief	�R���\�[���o�͎��̕����F�ݒ�
	enum class ForeColor
	{
		//! ��
		Black = 0,
		//! ��
		Blue = FOREGROUND_BLUE,
		//! ��(����)
		BlueH = Blue | FOREGROUND_INTENSITY,
		//! ��
		Green = FOREGROUND_GREEN,
		//! ��(����)
		GreenH = Green | FOREGROUND_INTENSITY,
		//! ��
		Red = FOREGROUND_RED,
		//! ��(����)
		RedH = Red | FOREGROUND_INTENSITY,
		//! �V�A��
		Cyan = (Blue | Green),
		//! �V�A��(����)
		CyanH = Cyan | FOREGROUND_INTENSITY,
		//! �}�[���^
		Magenta = (Blue | Red),
		//! �}�[���^(����)
		MagentaH = Magenta | FOREGROUND_INTENSITY,
		//! ��
		Yellow = (Green | Red),
		//! ��(����)
		YellowH = Yellow | FOREGROUND_INTENSITY,
		//! ��
		White = (Blue | Green | Red),
		//! ��(����)
		WhiteH = White | FOREGROUND_INTENSITY
	};

	//! @enum	BackColor
	//! @brief	�R���\�[���o�͎��̔w�i�F�ݒ�
	enum class BackColor
	{
		//! ��
		Black = 0,
		//! ��
		Blue = BACKGROUND_BLUE,
		//! ��(����)
		BlueH = Blue | BACKGROUND_INTENSITY,
		//! ��
		Green = BACKGROUND_GREEN,
		//! ��(����)
		GreenH = Green | BACKGROUND_INTENSITY,
		//! ��
		Red = BACKGROUND_RED,
		//! ��(����)
		RedH = Red | BACKGROUND_INTENSITY,
		//! �V�A��
		Cyan = (Blue | Green),
		//! �V�A��(����)
		CyanH = Cyan | BACKGROUND_INTENSITY,
		//! �}�[���^
		Magenta = (Blue | Red),
		//! �}�[���^(����)
		MagentaH = Magenta | BACKGROUND_INTENSITY,
		//!	��
		Yellow = (Green | Red),
		//! ��(����)
		YellowH = Yellow | BACKGROUND_INTENSITY,
		//! ��
		White = (Blue | Green | Red),
		//! ��(����)
		WhiteH = White | BACKGROUND_INTENSITY
	};

	//! @class	Console
	//! @brief	�R���\�[���Ɋւ���N���X
	class DLL_DECLSPEC Console : public HandleLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY Console ();

		//! @brief �f�X�g���N�^
		APIENTRY ~Console ();

		//! @brief	�R���\�[���o�̓E�B���h�E���쐬���܂��B
		//! @return BOOL	���s����
		//! @retval TRUE	����
		//! @retval FALSE	���s
		BOOL APIENTRY Create () const;

		//! @brief Console�N���X�����������܂��B
		//! @return BOOL	���s����
		//! @retval TRUE	����
		//! @retval FALSE	���s
		BOOL APIENTRY Init ();

		//! @brief �����F�A�w�i�F��ݒ肵�܂��B
		//! @param[in] foreColor �����F
		//! @param[in] backColor �w�i�F
		//! @return bool
		//! @retval true ����
		//! @retval false ���s
		BOOL APIENTRY SetColor (ForeColor foreColor, BackColor backColor) const;

		//! @brief �R���\�[���ɕ������o�͂��܂��B
		DWORD APIENTRY Write (LPCTSTR lpctszString) const;

		//! @brief �R���\�[���ɕ������o�͂��܂��B
		VOID APIENTRY Format (LPCTSTR format, ...) const;

		//! @brief �R���\�[���^�C�g����ݒ肵�܂��B
		BOOL APIENTRY SetTitle (LPCTSTR lpctszTitle) const;

		//! @brief �o�͈ʒu��ݒ肵�܂��B
		BOOL APIENTRY SetCursorPosition (SHORT x, SHORT y) const;

		//! @brief �o�b�t�@�T�C�Y��ݒ肵�܂��B
		BOOL APIENTRY SetBufferSize (SHORT x, SHORT y) const;

		//! @brief �E�B���h�E�T�C�Y��ݒ肵�܂��B
		BOOL APIENTRY SetWindowInfo (SHORT width, SHORT height) const;

		//! @brief �R���\�[�����N���A���܂��B
		BOOL APIENTRY Clear () const;

	private:
		BOOL APIENTRY GetScreenBufferInfo (CONSOLE_SCREEN_BUFFER_INFO& csbi) const;

		//! @brief �R���\�[���E�B���h�E�o�͏��
		CONSOLE_SCREEN_BUFFER_INFO _prevConsoleScreenBufferInfo;
	};
}