// ----------------------------------------------------------------------------
//! @file	TString.h
//! @brief	������Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "Array.hpp"

namespace alt
{
	//! @class TString
	//! @brief ������Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC TString : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY TString ();

		//! @brief		�R���X�g���N�^
		//! @param[in]	lpctszString �������Ɋi�[���镶����
		APIENTRY TString (LPCTSTR lpctszString);

		//! @brief		�R���X�g���N�^
		//! @param[in]	string �������Ɋi�[���镶����
		APIENTRY TString (const TString& string);

		//! @brief		�R���X�g���N�^
		//! @param[in]	length �������Ɋm�ۂ��郁�����T�C�Y(������)
		//! @note		API�ȂǂŁATCHAR[256]���K�v�ȏꍇ�Ɏg�p���܂��B
		//!             Ptr()���g�p����ƁA�������m�ۍς݂̃|�C���^��
		//!             �g�p�ł��܂��B
		APIENTRY TString (INT length);

		//! @brief �f�X�g���N�^
		APIENTRY ~TString ();

		//! @brief �����Ŋm�ۂ��Ă��镶����|�C���^���擾���܂��B
		//! @return �ێ����Ă��镶����̃|�C���^
		LPTSTR APIENTRY Ptr () const { return _lptszString; };

		//! @brief �����Ŋm�ۂ��Ă��镶����|�C���^���擾���܂��B
		//! @return �ێ����Ă��镶����̃|�C���^
		LPCTSTR APIENTRY Ctr () const { return (LPCTSTR)_lptszString; };

		//! @brief �����Ŋm�ۂ��Ă��镶���񐔂��擾���܂��B
		//! @return ������
		INT APIENTRY Len () const;

		//! @brief �����Ŋm�ۂ��Ă��镶����(�o�C�g��)���擾���܂��B
		//! @return �o�C�g��
		INT APIENTRY Byte () const;

		//! @brief ������Ɣ�r���܂��B
		//1 @param[in]	lpctszString ��r���镶����
		//! @return ��v���邩�ۂ�
		//! @retval TRUE ��v����
		//! @retval FALSE ��v���Ȃ�
		BOOL APIENTRY operator == (LPCTSTR lpctszString) const;

		//! @brief TString�ƕ�������r���܂��B
		//! @param[in]	string ��r����TString�I�u�W�F�N�g
		//! @return ��v���邩�ۂ�
		//! @retval TRUE ��v����
		//! @retval FALSE ��v���Ȃ�
		BOOL APIENTRY operator == (const TString& string) const;

		//! @brief ������Ɣ�r���܂��B
		//1 @param[in]	lpctszString ��r���镶����
		//! @return ��v���Ȃ����ۂ�
		//! @retval TRUE ��v����
		//! @retval FALSE ��v���Ȃ�
		BOOL APIENTRY operator != (LPCTSTR lpctszString) const;

		//! @brief TString�ƕ�������r���܂��B
		//! @param[in]	string ��r����TString�I�u�W�F�N�g
		//! @return ��v���Ȃ����ۂ�
		//! @retval TRUE ��v����
		//! @retval FALSE ��v���Ȃ�
		BOOL APIENTRY operator != (const TString& string) const;

		//! @brief ������������܂��B
		//! @param[in]	lpctszString ������
		VOID APIENTRY operator = (LPCTSTR lpctszString);

		//! @brief ������������܂��B
		//! @param[in]	string ������
		VOID APIENTRY operator = (const TString& string);

		//! @brief ������������܂��B
		//! @param[in]	lpctszString ������
		//! @return ������ꂽ�������g
		TString& APIENTRY operator << (LPCTSTR lpctszString);

		//! @brief ������������܂��B
		//! @param[in]	string ������
		//! @return ������ꂽ�������g
		TString& APIENTRY operator << (const TString& string);

		//! @brief ���l�������܂��B
		//! @param[in]	iValue ���l
		//! @return ������ꂽ�������g
		TString& APIENTRY operator << (const int iValue);

		//! @brief �������ǉ����܂��B
		//! @param[in]	lpctszString ������
		//! @return �ǉ����ꂽ������̌���
		TString APIENTRY operator + (LPCTSTR lpctszString);

		//! @brief �������ǉ����܂��B
		//! @param[in]	string ������
		//! @return �ǉ����ꂽ������̌���
		TString APIENTRY operator + (TString& string);

		//! @brief �������ǉ����܂��B
		//! @param[in]	lpctszString ������
		//! @return ������ꂽ�������g
		TString& APIENTRY operator += (LPCTSTR lpctszString);

		//! @brief �������ǉ����܂��B
		//! @param[in]	string ������
		//! @return ������ꂽ�������g
		TString& APIENTRY operator += (const TString& string);

		//! @brief ������̈ꕔ�����o���܂��B
		//! @param[in]	index �ꏊ�̎w��
		//! @return �w�肳�ꂽ�ꏊ�ɂ��镶��
		TCHAR APIENTRY operator [] (int index) const;

		//! @brief ������̑召���r���܂��B
		//! @param[in]	string ��r�Ώ�
		//! @return ��r����
		//! @retval true ��r���鑤��������
		//! @retval false ��r���鑤���傫��
		bool APIENTRY operator < (const TString& string) const;

		//! @brief �����̗]����؂���܂��B
		//! @return �����]����؂������������g
		TString& APIENTRY TrimLeft ();

		//! @brief �E���̗]����؂���܂��B
		//! @return �E���]����؂������������g
		TString& APIENTRY TrimRight ();

		//! @brief ���E�̗]����؂���܂��B
		//! @return ���E�̗]����؂������������g
		TString& APIENTRY Trim ();

		//! @brief �������؂�o���܂��B
		//! @param[in] startPos �؂�o���J�n�ʒu
		//! @param[in] length �؂�o���T�C�Y
		//! @return �؂�o����������
		TString APIENTRY Substring (int startPos, int length = 0);

		//! @brief �������u�����܂��B
		//! @param[in] charsBefore �������镶����
		//! @param[in] charsAfter �u�����镶����
		//! @return �������ʂ�u������������
		TString APIENTRY Replace (TString& charsBefore, TString& charsAfter);

		//! @brief �������u�����܂��B
		//! @param[in] charsBefore �������镶����
		//! @param[in] charsAfter �u�����镶����
		//! @return �������ʂ����ׂĒu������������
		TString APIENTRY ReplaceAll (TString& charsBefore, TString& charsAfter);

		//! @brief ��������w�肵��������ŕ������܂��B
		//! @param[in] delimiters �������镶���̏W��
		//! @return delimiters�P�ʂŕ�������������Q
		//! @note delimiter�ɓ����Ă��镶���͂��ׂĕ����Ώۂł��B
		skeleton::Array<TString> APIENTRY Split (const TString& delimiters);

		//! @brief ��������������܂��B
		//! @param[in] keyword �������镶����
		//! @param[in] position �����J�n�ʒu
		//! @return ��������������̊J�n�ʒu
		int APIENTRY Find (TString keyword, int position = 0);

		//! @brief �t�H�[�}�b�g�ɏ]���ăp�����[�^�𕶎��񉻂��܂��B
		//! @param[in] format ������t�H�[�}�b�g
		//! @return �t�H�[�}�b�g�ɏ]���ď����������������g
		TString& APIENTRY Format (LPCTSTR format, ...);

		//! @brief �}���`�o�C�g���������荞�݂܂��B
		//! @param[in] lpcszSJIS �k���^�[�~�l�[�g���ꂽ������
		//! @param[in] codePage �}���`�o�C�g�̕����R�[�h
		//! @return BOOL �ϊ��ɐ������ۂ�
		//! @retval true ����
		//! @retval false ���s
		//! @note SJIS -> CP_ACP (����l)
		//!       UTF-8 -> CP_UTF8
		BOOL APIENTRY FromMultiByte (LPCSTR lpcszSJIS, UINT codePage = CP_ACP);

		//! @brief SJIS��������o�͂��܂��B
		//! @param[out] lpszSJIS SJIS������
		//! @param[in] dwLen SJIS��������i�[����o�b�t�@�T�C�Y
		//! @return �������񂾃o�C�g��
		int APIENTRY ToSJIS (LPSTR lpszSJIS, DWORD dwLen);

		//! @brief ������̐��l��
		//! @return ���l(INT)
		int APIENTRY ParseInt () const;

	protected:
		VOID APIENTRY Clear ();
		LPTSTR APIENTRY CreateMemory (ULONGLONG size);
		VOID APIENTRY Copy (LPCTSTR lpctszString);
		VOID APIENTRY Add (LPCTSTR lpctszString);
		int APIENTRY Compare (
			LPCTSTR lpctszString1, LPCTSTR lpctszString2, int size) const;
		INT APIENTRY Length (LPCTSTR lpctszString) const;

		LPTSTR _lptszString;
	};
}