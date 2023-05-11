// ----------------------------------------------------------------------------
//! @file	LinkedList.hpp
//! @brief	�A�����X�g�Ɋւ���N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.26

#pragma once

namespace alt
{
	namespace skeleton
	{
		//! @class LinkedListNode
		//! @brief �o�����A���p�m�[�h�N���X 
		//! @tparam T �i�[�f�[�^�^
		template<class T>
		class LinkedListNode
		{
		public:
			//! @brief �R���X�g���N�^
			LinkedListNode<T> ()
			{
				_prev = nullptr;
				_next = nullptr;
				_value = nullptr;
			};

			//! @brief �R���X�g���N�^
			//! @param value T&
			LinkedListNode<T> (T& value)
				:LinkedListNode<T> ()
			{
				_value = new T (value);
			};

			//! @brief �f�X�g���N�^
			APIENTRY ~LinkedListNode<T> ()
			{
				delete _value;
			};

			//! @brief �擪��ێ�����m�[�h
			LinkedListNode<T>* _prev;

			//! @brief ������ێ�����m�[�h
			LinkedListNode<T>* _next;

			//! @brief �Ǘ�����f�[�^
			T* _value;
		};

		//! @class LinkedListIterator
		//! @brief Node�擪�`�����܂ł��T�[�`����C�e���[�^
		template<class T>
		class LinkedListIterator
		{
		public:
			//! @brief �R���X�g���N�^
			LinkedListIterator<T> (LinkedListNode<T>* node)
			{
				_node = node;
			};

			//! @brief �f�X�g���N�^
			APIENTRY ~LinkedListIterator<T> ()
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
			LinkedListIterator<T>& APIENTRY operator ++ ()
			{
				_node = _node->_next;
				return *this;
			};

			//! @brief �m�[�h�����m�F�p�I�y���[�^
			//! @param comp ��r���m�[�h
			//! @return bool
			//! @retval true(�s��v)
			//! @retval false(��v)
			bool APIENTRY operator != (const LinkedListIterator<T>& comp)
			{
				return _node != comp._node;
			};

		private:
			LinkedListIterator<T> () = delete;
			LinkedListNode<T>* _node;
		};

		//! @class LinkedList
		//! @brief	�|�C���^�ɂ��A�����X�g��������N���X
		template<class T>
		class LinkedList
		{
		public:
			//! @brief �R���X�g���N�^
			LinkedList<T> ()
			{
				_node = new LinkedListNode<T> ();
				_size = 0;
			};

			//! @brief �R�s�[�R���X�g���N�^
			//! @param base �R�s�[���I�u�W�F�N�g
			LinkedList<T> (const LinkedList<T>& base)
				: LinkedList<T> ()
			{
				operator =(base);
			};

			//! @brief �f�X�g���N�^
			virtual APIENTRY ~LinkedList<T> ()
			{
				Clear ();
				delete _node;
			};

			//! @brief LinkedList<T>�֒l��ǉ�
			//! @param item T*
			//! @note �A�����X�g�̍őO��ɒǉ����܂��B
			VOID APIENTRY AddFirst (T& item)
			{
				LinkedListNode<T>* newNode = new LinkedListNode<T> (item);

				if (_size == 0)
				{
					_node->_prev = newNode;
					_node->_next = newNode;
				}
				else
				{
					LinkedListNode<T>* next = _node->_next;
					_node->_next = newNode;
					newNode->_next = next;
					next->_prev = newNode;
				}

				_size++;
			};

			//! @brief LinkedList<T>�֒l��ǉ�
			//! @param item T*
			//! @note �A�����X�g�̍Ō��ɒǉ����܂��B
			VOID APIENTRY AddLast (T& item)
			{
				LinkedListNode<T>* newNode = new LinkedListNode<T> (item);

				if (_size == 0)
				{
					_node->_prev = newNode;
					_node->_next = newNode;
				}
				else
				{
					LinkedListNode<T>* prev = _node->_prev;
					_node->_prev = newNode;
					newNode->_prev = prev;
					prev->_next = newNode;
				}

				_size += 1;
			};

			//! @brief �w�肵���C���f�b�N�X�̒l���擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY Get (SIZE_T index) const
			{
				LinkedListNode<T>* current = _node->_next;

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

			//! @brief LinkedList<T>��z�񑀍�Ŏ擾
			//! @param index �擾����C���f�b�N�X
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief �����^��LinkedList<T>���R�s�[
			//! @param base �R�s�[�� LinkedList<T>
			VOID APIENTRY operator = (const LinkedList<T>& base)
			{
				Clear ();

				for (auto& itr : base)
				{
					AddLast (itr);
				}
			}

			//! @brief LinkedList<T>�̐擪�A�C�e�����폜
			VOID APIENTRY RemoveFirst ()
			{
				if (_size == 0) return;

				LinkedListNode<T>* node1 = _node->_next;
				LinkedListNode<T>* node2 = node1->_next;
				delete node1;
				_node->_next = node2;

				_size--;
			};

			//! @brief LinkedList<T>�̖����A�C�e�����폜
			VOID APIENTRY RemoveLast ()
			{
				if (_size == 0) return;

				LinkedListNode<T>* node1 = _node->_prev;
				LinkedListNode<T>* node2 = node1->_prev;
				delete node1;
				_node->_prev = node2;

				_size--;
			};

			//! @brief LinkedList<T>����l���폜
			//! @param item �폜�I�u�W�F�N�g
			VOID APIENTRY Remove (T& item)
			{
				LinkedListNode<T>* current = _node->_next;
				LinkedListNode<T>* before = _node;
				LinkedListNode<T>* after = current->_next;

				for (INT i = 0; i < _size; i++)
				{
					if (*(current->_value) == item)
					{
						break;
					}

					current = current->_next;
					if (current == nullptr)
					{
						break;
					}
					before = current->_prev;
					after = current->_next;
				}

				if (current != nullptr)
				{
					if (before != nullptr && before->_next != nullptr)
						before->_next = after;
					if (after != nullptr && after->_prev != nullptr)
						after->_prev = before;

					delete current;
					_size--;
				}
			};

			//! @brief LinkedList<T>�֒l��ǉ�
			//! @param item T&
			//! @note �A�����X�g�̍Ō��ɒǉ����܂��B
			VOID APIENTRY Enqueue (T& item)
			{
				AddLast (item);
			};

			//! @brief LinkedList<T>����l���擾
			//! @param item T&
			//! @return bool
			//! @retval true(����)
			//! @retval false(���s)
			//! @note �A�����X�g�̍őO�񂩂�擾���܂��B
			bool APIENTRY Dequeue (T& item)
			{
				bool ret = true;
				T* value = Get (0);

				if (value != nullptr) item = *value;
				else ret = false;

				RemoveFirst ();

				return ret;
			};

			//! @brief �T�C�Y���擾
			//! @return SIZE_T
			//! @retval ���݂̘A�����X�g�̃T�C�Y
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief �̈�̊J��
			VOID APIENTRY Clear ()
			{
				while (_size > 0)
				{
					LinkedListNode<T>* node = _node->_next;
					delete _node;
					_node = node;
					_size--;
				}
			};

			//! @brief for(auto n : list){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return LinkedListIterator<T> �擪�C�e���[�^
			LinkedListIterator<T> APIENTRY begin () const
			{
				return LinkedListIterator<T> (_node->_next);
			};

			//! @brief for(auto n : list){}���g�p�ł���悤�ɂ���C���^�[�t�F�[�X
			//! @return LinkedListIterator<T> �����C�e���[�^
			LinkedListIterator<T> APIENTRY end () const
			{
				return LinkedListIterator<T> (nullptr);
			};

			//! @brief Iterator����̃A�N�Z�X�������
			friend LinkedListIterator<T>;

		private:
			LinkedListNode<T>* _node;
			SIZE_T _size;
		};
	}
}