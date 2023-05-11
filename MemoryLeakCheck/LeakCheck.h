//! @file	LeakCheck.h
//! @brief	TestProject�ł͌��o�ł��Ȃ��A�������[���[�N���m�F���܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.23

#pragma once

#define CONFIG_FILE _T(".\\MemoryLeakCheck.ini")

namespace LeakCheck
{
	//! @class	Data
	//! @brief	�f�[�^�i�[�N���X
	class Data
	{
	public:
		Data ()
		{
			number = -1;
		};

		Data (int i)
		{
			number = i;
		};

		virtual ~Data ()
		{

		};

		int Get ()
		{
			return number;
		};

		void Set (int i)
		{
			number = i;
		};

	private:
		int number;
	};

	//! @class	MemoryLeakCheck
	//! @brief	�������[���[�N�`�F�b�N�p
	class LeakCheck
	{
	public:
		//! @brief ����������
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		virtual BOOL Init () = 0;

		//! @brief �����J�n
		//! @return ��������
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		virtual BOOL DoAction () = 0;
	};

	//! @class LeakCheckTest
	//! @brief �������[���[�N���ʂ̏o�͊m�F�p
	class LeakCheckTest : public LeakCheck
	{
	public:
		BOOL Init ()
		{
			LPBYTE lpbyLeakCheck = new BYTE[16];
			ZeroMemory (lpbyLeakCheck, 16);
			return TRUE;
		};

		BOOL DoAction ()
		{
			LPBYTE lpbyLeakCheck = (LPBYTE)::malloc (16);
			ZeroMemory (lpbyLeakCheck, 16);
			return TRUE;
		};
	};
}