//! @file	PerformanceCheck.h
//! @brief	���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#include "TString.h"
#include "Console.h"
#include "Utility.h"

#define CONFIG_FILE _T(".\\PerformanceCheck.ini")

namespace PerfCheck
{
	//! @class	PerformanceCheck
	//! @brief	���\����p
	class PerformanceCheck
	{
	public:
		//! @brief �R���X�g���N�^
		PerformanceCheck (alt::Console& console);

		//! @brief �f�X�g���N�^
		virtual ~PerformanceCheck ();

		//! @brief	����������
		virtual VOID Init ();

		//! @brief  �����J�n
		//! @return ��������
		//! @retval TRUE:����
		//! @retval FALSE:���s
		//! @note   �������z�֐��ł��B
		virtual BOOL DoAction () = 0;

	protected:
		alt::Console& _console;

	private:
		PerformanceCheck () = delete;
	};
}