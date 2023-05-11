// ----------------------------------------------------------------------------
//! @file	Map.hpp
//! @brief	�}�b�v�Ɋւ���N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.8.16

#pragma once

#include "Utility.h"

namespace alt
{
	namespace skeleton
	{
		//! @class MapNode
		//! @brief �o�����A���p�m�[�h�N���X 
		//! @tparam T �i�[�f�[�^�^
		template<class T>
		class MapNode
		{
		public:
			//! @brief �R���X�g���N�^
			MapNode<T> ()
			{
				_prev = nullptr;
				_next = nullptr;
				_value = nullptr;
				ZeroMemory (_byMD5, sizeof (_byMD5));
			};

			//! @brief �R���X�g���N�^
			//! @param value T&
			MapNode<T> (T& value)
				:MapNode<T> ()
			{
				_value = new T (value);
				alt::Utility::CreateMD5 ((LPBYTE)_value, sizeof (T), _byMD5);
			};

			//! @brief �f�X�g���N�^
			APIENTRY ~MapNode<T> ()
			{
				delete _value;
			};

			//! @brief �擪��ێ�����m�[�h
			MapNode<T>* _prev;

			//! @brief ������ێ�����m�[�h
			MapNode<T>* _next;

			//! @brief �Ǘ�����f�[�^
			T* _value;

			//! @brief �f�[�^��MD5
			BYTE _byMD5[32];
		};

		//! @class MapIterator
		//! @brief Node�擪�`�����܂ł��T�[�`����C�e���[�^
		template<class T>
		class MapIterator
		{
		public:
			//! @brief �R���X�g���N�^
			MapIterator<T> (MapNode<T>* node)
			{
				_node = node;
			};

			//! @brief �f�X�g���N�^
			APIENTRY ~MapIterator<T> ()
			{
				_node = nullptr;
			};

			//! @brief �f�[�^�A�N�Z�X�p�I�y���[�^
			//! @note �͈�for�p
			T& APIENTRY operator * ()
			{
				return *(_node->_value);
			};

			//! @brief �m�[�h�ړ��p�I�y���[�^
			//! @note �͈�for�p
			MapIterator<T>& APIENTRY operator ++ ()
			{
				_node = _node->_next;
				return *this;
			};

			//! @brief �m�[�h�����m�F�p�I�y���[�^
			//! @param comp ��r���m�[�h
			//! @return bool
			//! @retval true(�s��v)
			//! @retval false(��v)
			bool APIENTRY operator != (const MapIterator<T>& comp)
			{
				return _node != comp._node;
			};

		private:
			MapIterator<T> () = delete;
			MapNode<T>* _node;
		};

		//! @class Map
		//! @brief	�|�C���^�ɂ��}�b�v��������N���X
		template<class T>
		class Map
		{
		public:
			//! @brief �R���X�g���N�^
			Map<T> ()
			{
				_node = new MapNode<T> ();
				_index = nullptr;
				_size = 0;
			};

			//! @brief �R�s�[�R���X�g���N�^
			//! @param base �R�s�[���I�u�W�F�N�g
			Map<T> (const Map<T>& base)
				: Map<T> ()
			{
				operator =(base);
			};

			//! @brief �f�X�g���N�^
			virtual APIENTRY ~Map<T> ()
			{
				Clear ();
				delete _node;
			};

			//! @brief Map<T>�֒l��ǉ�
			//! @param item T*
			//! @note �}�b�v��MD5���ɒǉ����܂��B
			VOID APIENTRY Add (T& item)
			{
				MapNode<T>* newNode = new MapNode<T> (item);

				if (_size == 0)
				{
					_node->_prev = newNode;
					_node->_next = newNode;
				}
				else
				{
					MapNode<T>* current = _node;

					while(current != NULL)
					{
						MapNode<T>* next = current->_next;

						if (next == NULL || ::memcmp (newNode->_byMD5, next->_byMD5, 32) < 0)
						{
							if (next) next->_prev = newNode;
							current->_next = newNode;
							if (current) newNode->_prev = current;
							if (next) newNode->_next = next;
							break;
						}

						if (current->_next == NULL) break;
						
						current = current->_next;
					}
				}

				_size += 1;
			};

			//! @brief Map<T>����l���폜
			//! @param item �폜�I�u�W�F�N�g
			VOID APIENTRY Remove (T& item)
			{
				MapNode<T>* current = _node->_next;

				for (;current != NULL;)
				{
					if (*(current->_value) == item)
					{
						break;
					}
					current = current->_next;
				}

				if (current != NULL)
				{
					MapNode<T>* before = current->_prev;
					MapNode<T>* after = current->_next;
					if (before != NULL && before->_next != NULL) before->_next = after;
					if (after != NULL && after->_prev != NULL) after->_prev = before;

					delete current;
					_size--;
				}
			};

			// for DEBUG
			MapNode<T>* GetNode (SIZE_T index)
			{
				MapNode<T>* current = _node->_next;

				for (INT i = 0; i < index; i++)
				{
					if (current == nullptr)
					{
						break;
					}
					current = current->_next;
				}

				return current;
			};

			//! @brief �w�肵���C���f�b�N�X�̒l���擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY Get (SIZE_T index) const
			{
				MapNode<T>* current = _node->_next;

				for (INT i = 0; i < index; i++)
				{
					if (current == nullptr)
					{
						break;
					}
					current = current->_next;
				}

				return current != nullptr ? current->_value : nullptr;
			};

			//! @brief Map<T>��z�񑀍�Ŏ擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief �����^��Map<T>���R�s�[
			//! @param base �R�s�[�� Map<T>
			VOID APIENTRY operator = (const Map<T>& base)
			{
				Clear ();

				for (auto& itr : base)
				{
					Add (itr);
				}
			}

			//! @brief �T�C�Y���擾
			//! @return SIZE_T
			//! @retval ���݂̃}�b�v�̃T�C�Y
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief �̈�̊J��
			VOID APIENTRY Clear ()
			{
				while (_size > 0)
				{
					MapNode<T>* node = _node->_next;
					delete _node;
					_node = node;
					_size--;
				}
			};

			//! @brief for(auto n : map){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return MapIterator<T> �擪�C�e���[�^
			MapIterator<T> APIENTRY begin () const
			{
				return MapIterator<T> (_node->_next);
			};

			//! @brief for(auto n : list){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return MapIterator<T> �����C�e���[�^
			MapIterator<T> APIENTRY end () const
			{
				return MapIterator<T> (nullptr);
			};

			//! @brief Iterator����̃A�N�Z�X�������
			friend MapIterator<T>;

		private:
			void Balance (int left, int center, int right, MapNode<T> balance)
			{

			};

			void CreateIndex ()
			{
				SIZE_T center;

				if (_size % 2 == 0)
				{
					// even
					center = (_size + 1) / 2;
				}
				else
				{
					// odd
					center = _size / 2;
				}

				for (MapNode<T>* item = _node->_next; item != nullptr; _node = _node->_next)
				{
					center--;

					if (center == 0)
					{
						_index = item;
						break;
					}
				}
			};

			MapNode<T>* _node;
			MapNode<T>* _index;
			SIZE_T _size;
		};
	}
}