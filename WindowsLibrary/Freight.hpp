//! @file	Freight.hpp
//! @brief	�v���~�e�B�u�ȕϐ����Ǘ�����R���e�i�N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#pragma once

#include "HeapMemory.h"

namespace alt
{
	namespace skeleton
	{
		//! @class	Freight
		//! @brief	�v���~�e�B�u�ȕϐ����Ǘ�����R���e�i�N���X
		template<class T>
		class Freight
		{
		public:
			//! @brief �R���X�g���N�^
			Freight<T> ()
			{
				_memory = new alt::BasicMemory ();
				_lpData = NULL;
				_size = 0;
			};

			//! @brief �R���X�g���N�^
			//! @param memory IMemory�I�u�W�F�N�g
			Freight<T> (IMemory* memory)
			{
				if (memory == NULL)
					_memory = new alt::BasicMemory ();
				else
					_memory = memory;
				_lpData = NULL;
				_size = 0;
			};

			//! @brief �R���X�g���N�^
			//! @param lpData �i�[�f�[�^
			//! @param size �i�[�f�[�^�T�C�Y�iT�P�ʂ̌��j
			//! @param memory HeapMemoryManager
			Freight<T> (T* lpData, size_t size, IMemory* memory = NULL)
				:Freight (memory)
			{
				SetData (lpData, size);
			};

			//! @brief �R�s�[�R���X�g���N�^
			//! @param base �R�s�[���I�u�W�F�N�g
			Freight<T> (const Freight<T>& base)
			{
				if (dynamic_cast<alt::BasicMemory*>(base._memory))
					_memory = new alt::BasicMemory ();
				else
					_memory = base._memory;
				_lpData = NULL;
				_size = 0;
				SetData (base._lpData, base._size);
			};

			//! @brief �f�X�g���N�^
			APIENTRY ~Freight ()
			{
				Clear ();
				auto basicMemory = dynamic_cast<alt::BasicMemory*>(_memory);
				if (basicMemory)
				{
					delete basicMemory;
				}
				_memory = NULL;
			};

			//! @brief �l�̐ݒ�
			//! @param lpData �i�[�f�[�^
			//! @param size �i�[�f�[�^�T�C�Y�iT�P�ʂ̌��j
			VOID APIENTRY SetData (T* lpData, size_t size)
			{
				New (size);
				// �����Ő��m�ɏ����R�s�[�ł���̂́A
				// char��unsigned char �Ȃǂ̃v���~�e�B�u��
				// �^�����ɂȂ�B�R���X�g���N�^��f�X�g���N�^��
				// �����̂́A�Ή��s�Boperator = ()�������Ȃ��B
				CopyMemory (_lpData, lpData, size * sizeof (T));
				_size = size;
			};

			//! @brief ���O�̃������擾
			//! @param size �擾�T�C�Y
			VOID APIENTRY New (size_t size)
			{
				Clear ();
				_lpData = static_cast<T*>(_memory->Allocate (sizeof (T) * size));
				_size = size;
			};

			//! @brief �i�[�f�[�^�̎擾
			//! @return T* �i�[�f�[�^
			T* APIENTRY GetData () const { return _lpData; };

			//! @brief �i�[�f�[�^�̃T�C�Y�擾
			//! @return size_t �i�[�f�[�^�T�C�Y�iT�P�ʁj
			size_t APIENTRY GetSize () const { return _size; }

			//! @brief �i�[�̈�̊J��
			VOID APIENTRY Clear ()
			{
				if (_lpData != NULL)
				{
					_memory->Free (_lpData);
					_lpData = NULL;
					_size = 0;
				}
			};

			//! @brief ���S�ȃI�u�W�F�N�g�̃R�s�[
			//! @param base �R�s�[���I�u�W�F�N�g
			VOID APIENTRY operator = (const Freight<T>& base)
			{
				if (dynamic_cast<alt::BasicMemory*>(base._memory))
					_memory = new alt::BasicMemory ();
				else
					_memory = base._memory;
				SetData (base._lpData, base._size);
			};

		private:
			IMemory* _memory;
			T* _lpData;
			size_t _size;
		};
	}
}