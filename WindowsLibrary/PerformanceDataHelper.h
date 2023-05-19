// ----------------------------------------------------------------------------
//! @file	PerformanceDataHelper.cpp
//! @brief	�p�t�H�[�}���X���j�^�[�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.15

#pragma once

#include <Pdh.h>
#include "WindowsLibrary.h"
#include "TString.h"
#include "Array.hpp"

namespace alt
{
	class DLL_DECLSPEC PerformanceDataItem : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		//! @param hQuery PerformanceDataHelper�ŊǗ�����HQUERY�l
		//! @param key �Ď��J�E���^�[��
		APIENTRY PerformanceDataItem (HQUERY hQuery, TString key);

		//! @brief �f�X�g���N�^
		APIENTRY ~PerformanceDataItem ();

		//! @brief �J�E���^�[�l�̎擾�ݒ�
		//! @return bool
		//! @retval true(����)
		//! @retval false(���s)
		bool APIENTRY AddCounter ();

		//! @brief �J�E���^�[���̎擾
		//! @return TString&
		TString& APIENTRY GetKey ();

		//! @brief �J�E���^�[�l�̎擾
		//! @param dwFormat �擾�l�̌`��
		//!                 PDH_FMT_LONG
		//!                 PHD_FMT_DOUBLE
		//!                 PHD_FMT_LARGE
		//! @param value PDH_FMT_COUNTERVALUE
		//!              .longValue : LONG
		//!              .doubleValue : double
		//!              .largeValue : LONGLONG
		//! @return bool
		//! @retval true(����)
		//! @retval false(���s)
		bool APIENTRY GetValue (DWORD dwFormat, PDH_FMT_COUNTERVALUE& value);

	private:
		PerformanceDataItem () = delete;

		HQUERY _hQuery;
		HCOUNTER _hCounter;
		TString _counterPath;
	};

	class DLL_DECLSPEC PerformanceDataHelper : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY PerformanceDataHelper ();

		//! @brief �f�X�g���N�^
		APIENTRY ~PerformanceDataHelper ();

		//! @brief �p�t�H�[�}���X���j�^�[�l�̎擾�J�n
		//! @return bool
		//! @retval true(����)
		//! @retval false(���s)
		bool APIENTRY Open ();

		//! @brief �p�t�H�[�}���X���j�^�[�l�̎擾�I��
		//! @return bool
		//! @retval true(����)
		//! @retval false(���s)
		bool APIENTRY Close ();

		//! @brief ���̃N���X�ɑ�����J�E���^�[�ݒ�N���X�̐���
		//! @param key �J�E���^�[��
		//! @return PerformanceDataItem&
		PerformanceDataItem& APIENTRY CreateItem (TString key);

		//! @brief �p�t�H�[�}���X�J�E���^�[�l�̖₢���킹
		//! @return bool
		//! @retval true(����)
		//! @retval false(���s)
		bool APIENTRY Query ();

	private:
		HQUERY _hQuery;
	};
}