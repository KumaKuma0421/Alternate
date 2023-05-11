// ----------------------------------------------------------------------------
//! @file	Array.hpp
//! @brief	�ϔz��Ɋւ���N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "HeapMemory.h"

namespace alt
{
	namespace skeleton
	{
		//! @class Array
		//! @brief �T�C�Y�ς̔z���������N���X
		//! @note�\���́A�N���X�ȊO�͓���s�ł��B
		template<class T>
		class Array
		{
		public:
			//! @brief �R���X�g���N�^
			Array<T> ()
			{
				_memory = new alt::BasicMemory ();
				_array = NULL;
				_size = 0;
				_reserve = 0;
			};

			//! @brief �R���X�g���N�^
			//! @param memory IMemory�I�u�W�F�N�g
			Array<T> (IMemory* memory)
			{
				if (memory == NULL)
					_memory = new alt::BasicMemory ();
				else
					_memory = memory;
				_array = NULL;
				_size = 0;
				_reserve = 0;
			}

			//! @brief �R�s�[�R���X�g���N�^
			//! @param base �R�s�[���I�u�W�F�N�g
			Array<T> (const Array<T>& base)
			{
				operator =(base);
			};

			//! @brief �f�X�g���N�^
			virtual APIENTRY ~Array<T> ()
			{
				Clear ();
				auto basicMemory = dynamic_cast<alt::BasicMemory*>(_memory);
				if (basicMemory)
				{
					delete basicMemory;
				}
				_memory = NULL;
			};

			//! @brief �w�肵���C���f�b�N�X�̒l���擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY Get (SIZE_T index) const
			{
				if (index >= _size) return (T*)NULL;
				return &(_array[index]);
			};

			//! @brief Array<T>��z�񑀍�Ŏ擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief Array<T>�֒l��ǉ�
			//! @param item T&
			//! @note �z��̍Ō��ɒǉ����܂��B
			VOID APIENTRY Add (T& item)
			{
				Extend ();

				CopyMemory (&_array[_size], &item, sizeof (LPVOID)); // ������
				_array[_size] = item;
				_size++;
			};

			//! @brief Array<T>�֒l��}��
			//! @param item T&
			//! @param position �}���C���f�b�N�X
			VOID APIENTRY Insert (T& item, SIZE_T position)
			{
				if (position > _size) return;

				Extend ();

				MoveMemory (
					&_array[position + 1],
					&_array[position],
					(_size - position) * sizeof (T));
				ZeroMemory (&_array[position], sizeof (T));
				CopyMemory (&_array[position], &item, sizeof (LPVOID)); // ������
				_array[position] = item;

				_size++;
			};

			//! @brief Array<T>����l���폜
			//! @param position �폜�C���f�b�N�X
			VOID APIENTRY Remove (SIZE_T position)
			{
				if (position >= _size) return;

				//void(**vtable)() = *(void(***)())(&_array[position]);
				//vtable[0] ();
				(&_array[position])->~T ();

				CopyMemory (
					&_array[position],
					&_array[position + 1],
					(_size - position) * sizeof (T));

				_size--;
				_reserve++;
			};

			//! @brief �폜���J��Ԃ����ƂŔ������郊�U�[�u�̈�����
			VOID APIENTRY Shurink ()
			{
				if (_reserve == 0) return;

				T* newArray = static_cast<T*>(_memory->Allocate (sizeof (T) * _size));
				CopyMemory (newArray, _array, (sizeof (T) * _size));
				_memory->Free (_array);
				// ������Ƌ����ȋC������B
				auto heapMemory = dynamic_cast<alt::HeapMemory*>(_memory);
				if (heapMemory != NULL) heapMemory->Compact ();
				_array = newArray;
				_reserve = 0;
			};

			//! @brief ���O�ɋ�̗̈���擾
			//! @param size �擾�T�C�Y�i�I�u�W�F�N�g�����j
			VOID APIENTRY Reserve (SIZE_T size)
			{
				Extend (size);
			};

			//! @brief �̈�̊J��
			VOID APIENTRY Clear ()
			{
				if (_size)
				{
					for (SIZE_T i = 0; i < Size (); i++)
					{
						(&_array[i])->~T ();
					}
					_memory->Free (_array);
					_array = NULL;
					_size = 0;
				}
			};

			//! @brief �����^��Array<T>���R�s�[
			//! @param base �R�s�[�� Array<T>
			VOID APIENTRY operator = (const Array<T>& base)
			{
				Clear ();

				if (dynamic_cast<alt::BasicMemory*>(base._memory))
					_memory = new alt::BasicMemory ();
				else
					_memory = base._memory;
				_size = base._size;
				_array = static_cast<T*>(_memory->Allocate (sizeof (T) * _size));
				for (SIZE_T i = 0; i < _size; i++)
				{
					CopyMemory (&_array[i], &base._array[i], sizeof (LPVOID)); // ������
					_array[i] = base._array[i];
				}
				_reserve = 0;
			};

			//! @brief �����^��Array<T>�𖖔��ɒǉ�
			//! @param base �ǉ��� Array<T>
			VOID APIENTRY operator += (const Array<T>& base)
			{
				Extend (base._size - _reserve);

				for (SIZE_T i = 0; i < base._size; i++)
				{
					CopyMemory (&_array[_size + i], &base._array[i], sizeof (LPVOID)); // ������
					_array[_size + i] = base._array[i];
				}
				_size += base._size;
				_reserve = 0;
			};

			//! @brief �T�C�Y���擾
			//! @return SIZE_T
			//! @retval ���݂̃��U�[�u�ʂ������z��T�C�Y
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief for(auto n : array){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return T* �擪�A�h���X
			T* APIENTRY begin ()
			{
				return &_array[0];
			};

			//! @brief for(auto n : array){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return T* �����A�h���X
			T* APIENTRY end ()
			{
				return &_array[_size];
			};

			//! @brief for(auto& n : array){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return T* �擪�A�h���X
			const T* APIENTRY begin () const
			{
				return &_array[0];
			};

			//! @brief for(auto& n : array){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return T* �����A�h���X
			const T* APIENTRY end () const
			{
				return &_array[_size];
			};

		private:
			VOID APIENTRY Extend (SIZE_T size = 1)
			{
				//     size  reserve
				// --+-----+--------+
				// #1|    0|       0|
				// #2|    n|       0|
				// #3|    0|       n| 
				// #4|    n|       n|

				if (_size == 0 && _reserve == 0)
				{
					// #1 �������
					_array = static_cast<T*>(_memory->Allocate (sizeof (T) * size));
				}
				else if (_size > 0 && _reserve == 0)
				{
					// #2 �m�ۍς݁A���U�[�u�Ȃ�
					_array = static_cast<T*>(
						_memory->Reallocate (_array, sizeof (T) * (_size + size)));
				}
				else if (_size == 0 && _reserve > 0)
				{
					// #3 �m�ۂȂ��A���U�[�u����
					_reserve--;
				}
				else
				{
					// #4 �m�ۍς݁A���U�[�u����
					_reserve--;
				}
			};

			IMemory* _memory;
			T* _array;
			SIZE_T _size;
			SIZE_T _reserve;
		};
	}
}