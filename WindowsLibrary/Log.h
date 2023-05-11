// ----------------------------------------------------------------------------
//! @file	Log.h
//! @brief	File���x�[�X�Ƃ������O�Ɋւ���@�\���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.26

#pragma once

#include "File.h"
#include "FileUtility.h"
#include "TString.h"
#include "CriticalSection.h"

namespace alt
{
	//! @enum	LogLevel
	//! @brief	���O�̏o�̓��x��
	enum class LogLevel
	{
		//! (0)�ُ�I�����x��
		Fatal,
		//! (1)�G���[
		Error,
		//! (2)�x��
		Warn,
		//! (3)���
		Info,
		//! (4)�f�o�b�O
		Debug,
		//! (5)�g���[�X
		Trace
	};

	//! @class	Log
	//! @brief	File���x�[�X�Ƃ������O�Ɋւ���@�\���W�񂵂��N���X
	class DLL_DECLSPEC Log : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY Log ();

		//! @brief �R���X�g���N�^
		//! @param logLevel �o�͑Ώۃ��x���i����ȏ���o�́j
		//! @param fileName �o�̓t�@�C����
		//! @param sRotation ���[�e�[�V�����t�@�C����
		//! @param dwFileSize �o�̓t�@�C���T�C�Y
		APIENTRY Log (
			LogLevel logLevel, LPCTSTR fileName, SHORT sRotation,
			DWORD dwFileSize);

		//! @brief �f�X�g���N�^
		APIENTRY ~Log ();

		//! @brief ������
		//! @param logLevel �o�͑Ώۃ��x���i����ȏ���o�́j
		//! @param fileName �o�̓t�@�C����
		//! @param sRotation ���[�e�[�V�����t�@�C����
		//! @param dwFileSize �o�̓t�@�C���T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Init (
			LogLevel logLevel, LPCTSTR fileName, SHORT sRotation,
			DWORD dwFileSize);

		//! @brief �v���I�G���[���O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Fatal (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief �G���[���O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Error (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief �x�����O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Warn (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief ��񃍃O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Info (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief �f�o�b�O���O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Debug (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief �g���[�X���O�̏o��
		//! @param message �o�̓��b�Z�[�W
		//! @param dwThreadID �o�͑ΏۃX���b�hID
		VOID APIENTRY Trace (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

	protected:
		BOOL APIENTRY AppendLog ();
		BOOL APIENTRY CreateLog ();
		BOOL APIENTRY OpenLog ();
		VOID APIENTRY FindHistory ();
		VOID APIENTRY RotateLogFile ();
		LPCTSTR APIENTRY GetLogLevelString (LogLevel logLevel);

	private:
		VOID APIENTRY LogWrite (
			LogLevel logLevel, LPCTSTR pctszMessage, DWORD dwThreadID);

		File _file;					//! ���ݏo�͒��̃��O�t�@�C��
		LogLevel _level;			//! �o�̓��O���x��
		TString _baseFileName;		//! ���O�t�@�C�����[�U�[�ݒ薼
		TString _logFileName;		//! ���O�t�@�C�����S��
		SYSTEMTIME _localTime;		//! ���O�ɏo�͂���V�X�e������
		DWORD _dwCurrentWriteSize;	//! ���݂܂łɏ�������ł��郍�O�t�@�C���T�C�Y
		SHORT _sFileCount;			//! ���݂܂łɊm�F�ł��闚���t�@�C����
		SHORT _sRotation;			//! ���O���[�e�[�V������
		DWORD _dwFileSize;			//! ���O�t�@�C���T�C�Y
		CriticalSection _lock;		//! �������ݎ����b�N
		LPCTSTR _format = _T ("%04d/%02d/%02d %02d:%02d:%02d.%03d %5d %5s %s\r\n");

		LPCTSTR _lpctszExtension = _T (".log");
		LPCTSTR _lpctszLogFileName = _T ("%s.log");
		LPCTSTR _lpctszSearchKeyword = _T ("??.log");
		LPCTSTR _lpctszRotationFileName = _T ("%s%02d.log");

		LPCTSTR _lpctszLogLevelFatal = _T ("Fatal");
		LPCTSTR _lpctszLogLevelError = _T ("Error");
		LPCTSTR _lpctszLogLevelWarn = _T ("Warn ");
		LPCTSTR _lpctszLogLevelInfo = _T ("Info ");
		LPCTSTR _lpctszLoglevelDebug = _T ("Debug");
		LPCTSTR _lpctszLogLevelTrace = _T ("Trace");
	};
}