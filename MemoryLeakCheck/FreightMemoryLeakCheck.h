//! @file	FreightMemoryLeakCheck.h
//! @brief	Freight�̃������[���[�N���m�F���܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "LeakCheck.h"
#include "Freight.hpp"

namespace LeakCheck
{
	//! @class	FreightMemoryLeakCheck
	//! @brief	Array�̃������[���[�N�`�F�b�N�N���X
	class FreightMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief �R���X�g���N�^
		FreightMemoryLeakCheck ();

		//! @brief �f�X�g���N�^
		virtual ~FreightMemoryLeakCheck ();

		//! @brief ����������
		virtual BOOL Init ();

		//! @brief �������[���[�N����
		virtual BOOL DoAction ();
	
	private:
		BOOL DoAction1 ();
		BOOL DoAction2 ();
		BOOL DoAction3 ();
		BOOL DoAction4 ();
		BOOL DoAction5 ();
		BOOL DoAction6 ();
	};
}