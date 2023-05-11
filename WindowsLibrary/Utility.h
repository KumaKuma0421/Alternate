// ----------------------------------------------------------------------------
//! @file	Utility.h
//! @brief	�ėp�I�Ɏg���郆�[�e�B���e�B�N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "TString.h"

namespace alt
{
	//! @class	Utility
	//! @brief	�ėp�I�Ɏg���郆�[�e�B���e�B�N���X
	class DLL_DECLSPEC Utility
	{
	public:
		//! @brief GUID�̍쐬
		//! @return GUID(GUID�\����)
		static const GUID APIENTRY CreateGUID ();

		//! @brief GUID�̍쐬
		//! @return GUID�i������j
		static const TString APIENTRY MakeGUID ();

		//! @brief �V�X�e�����Ԃ̎擾
		//! @return SYSTEMTIME
		//! @note wDayOfWee�͓��j�����O�ł��B
		static SYSTEMTIME APIENTRY GetSystemTime ();

		//! @brief ���[�J�����Ԃ̎擾
		//! @return SYSTEMTIME
		//! @note wDayOfWee�͓��j�����O�ł��B
		static SYSTEMTIME APIENTRY GetLocalTime ();

		//! @brief SYSTEMTIME�\���̂̒l�𕶎���
		//! @param systemTime SYSTEMTIME�\����
		//! @return TString
		static TString APIENTRY GetFormatTime (const SYSTEMTIME& systemTime);

		//! @brief ���[�J�����Ԃ��U�S�r�b�g�l�Ŏ擾
		//! @return ULARGE_INTEGER
		static ULARGE_INTEGER APIENTRY GetLocalTimeQuad ();

		//! @brief FILETIME�^����SYSTEMTIME���擾
		//! @param fileTime FILETIME�\����
		//! @return SYSTEMTIME�\����
		//! @note ������fileTime�̃^�C���]�[����ύX���܂���B�t�@�C����
		//!       �^�C���X�^���v�́A�V�X�e�������Ŋi�[����Ă��܂��B
		static SYSTEMTIME APIENTRY GetTimeByFileTime (const FILETIME& fileTime);

		//! @brief Windows���N�����Ă���̎��Ԃ�Ԃ��܂��B
		//! @note 49.7���Ń[���ɖ߂�܂��B
#pragma warning (push)
#pragma warning (disable:28159)
		static DWORD APIENTRY GetPastTick () { return ::GetTickCount (); };
#pragma warning (pop)

		//! @brief Windows���N�����Ă���̎��Ԃ�Ԃ��܂��B
		//! @return ULONGLONG
		static ULONGLONG APIENTRY GetPastTick64 () { return ::GetTickCount64 (); };

		//! @brief �V�X�e�������擾���܂��B
		//! @param systemInfo SYSTEM_INFO�\����
		//! DWORD     dwPageSize;
		//! LPVOID    lpMinimumApplicationAddress;
		//! LPVOID    lpMaximumApplicationAddress;
		//! DWORD_PTR dwActiveProcessorMask;
		//! DWORD     dwNumberOfProcessors;
		//! DWORD     dwProcessorType;
		//! DWORD     dwAllocationGranularity;
		//! WORD      wProcessorLevel;
		//! WORD      wProcessorRevision;
		static VOID APIENTRY GetSystemInfo (SYSTEM_INFO& systemInfo);

		//! @brief BASE64�Ńf�[�^���G���R�[�h
		//! @param lpbyString ������
		//! @param dwLength ������
		//! @param response �ϊ�����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY Base64Encode (
			LPBYTE lpbyString, DWORD dwLength, TString& response);

		//! @brief BASE64�f�[�^���f�R�[�h
		//! @param source BASE64������
		//! @param lpbyBuffer �o�b�t�@
		//! @param dwBuffer �o�b�t�@�T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY Base64Decode (
			TString& source, LPBYTE lpbyBuffer, DWORD& dwBuffer);

		//! @brief ���ϐ��̎擾
		//! @return Array<TString>
		//! @note �z��`���ł��ׂĂ̊��ϐ����擾���܂��B
		static skeleton::Array<TString> APIENTRY GetEnvironments ();

		//! @brief ���ϐ��̎擾
		//! @param lpctszKeyword �擾����L�[���[�h
		//! @return TString �Ώۂ̊��ϐ�
		static TString APIENTRY GetEnv (LPCTSTR lpctszKeyword);

		//! @brief �ݒ�t�B���̓ǂݍ���
		//! @param lpctszIniFile INI�t�@�C����
		//! @param lpctszSection �Z�N�V������
		//! @param lpctszKeyword �L�[���[�h��
		//! @param response �ǂݍ��݌���
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY ReadIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			TString& response);

		//! @brief �ݒ�t�B���̓ǂݍ���
		//! @param lpctszIniFile INI�t�@�C����
		//! @param lpctszSection �Z�N�V������
		//! @param lpctszKeyword �L�[���[�h��
		//! @param dwDefault ����l
		//! @return DWORD �擾��
		static DWORD APIENTRY ReadIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			DWORD dwDefault);

		//! @brief �ݒ�t�B���̏�������
		//! @param lpctszIniFile INI�t�@�C����
		//! @param lpctszSection �Z�N�V������
		//! @param lpctszKeyword �L�[���[�h��
		//! @param strValue �������ݓ��e
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY WriteIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			TString& strValue);

		//! @brief �ݒ�t�B���̏�������
		//! @param lpctszIniFile INI�t�@�C����
		//! @param lpctszSection �Z�N�V������
		//! @param lpctszKeyword �L�[���[�h��
		//! @param iValue �������ݓ��e
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY WriteIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			INT iValue);

		//! @brief �f�[�^����MD5���v�Z
		//! @param[in] lpbyData MD5���v�Z����f�[�^
		//! @param[in] dwSize MD5���v�Z����f�[�^�̃T�C�Y
		//! @param[out lpbyMD5 MD5�̌v�Z����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		//! @note MD5�͂P�Q�W�r�b�g�̒l�ł��B�o�͌��ʂ̊i�[�ɂR�Q�o�C�g�K�v�ł��B
		static BOOL APIENTRY CreateMD5 (LPBYTE lpbyData, DWORD dwSize, LPBYTE lpbyMD5);

		//! @brief �t�@�C������MD5���v�Z
		//! @param[in] lpctszFileName �t�@�C����
		//! @param[out] lpbyMD5 MD5�̌v�Z����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		//! @note MD5�͂P�Q�W�r�b�g�̒l�ł��B�o�͌��ʂ̊i�[�ɂR�Q�o�C�g�K�v�ł��B
		static BOOL APIENTRY CreateMD5 (LPCTSTR lpctszFileName, LPBYTE lpbyMD5);

		//! @brief �z��̓��e���N�C�b�N�\�[�g
		//! @tparam T �z��̌^
		//! @param array �z��
		//! @param rangeLeft �\�[�g�͈͂̍ŏ��Y��
		//! @param rangeRight �\�[�g�͈͂̍ő�Y��
		//! @note �e���v���[�g�͐錾�A�{�̂��w�b�_�[�ɏ����Ȃ���΂Ȃ�Ȃ��B
		//! @sa https://webpia.jp/quick_sort/
		template<typename T>
		static VOID APIENTRY QuickSort (T array[], int rangeLeft, int rangeRight)
		{
			// �ϐ���`
			int leftValue;
			int rightValue;
			T pivot;
			T temp;

			leftValue = rangeLeft; rightValue = rangeRight;

			// ��͐^�񒆂ɐݒ�i�l�̕��ϒl�i�����l�j������j
			pivot = array[(rangeLeft + rangeRight) / 2];

			while (1)
			{
				// ���菬�����l�������猩���Ă���
				// ������N���X�Ŕ�r���邽�߂ɁAoperator < ()�̎������K�v
				while (array[leftValue] < pivot) leftValue++;

				// ����傫���l���E���猩���Ă���
				// ������N���X�Ŕ�r���邽�߂ɁAoperator < ()�̎������K�v
				while (pivot < array[rightValue]) rightValue--;

				// ���������l�̏������t�ɂȂ�����I��
				if (leftValue >= rightValue) break;

				// ���S�ɃI�u�W�F�N�g����邽�߂ɁAoperator = ()�̎������K�v
				temp = array[leftValue];
				array[leftValue] = array[rightValue];
				array[rightValue] = temp;

				leftValue++;
				rightValue--;
			}

			//���̃f�[�^�Q��ΏۂƂ��ăN�C�b�N�\�[�g���ċA
			if (rangeLeft < leftValue - 1)
				QuickSort (array, rangeLeft, leftValue - 1);

			//�E�̃f�[�^�Q��ΏۂƂ��ăN�C�b�N�\�[�g���ċA
			if (rightValue + 1 < rangeRight)
				QuickSort (array, rightValue + 1, rangeRight);
		}

	private:
		Utility () = delete;
		~Utility () = delete;
	};
}