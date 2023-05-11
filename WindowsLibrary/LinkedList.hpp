// ----------------------------------------------------------------------------
//! @file	LinkedList.hpp
//! @brief	連結リストに関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.26

#pragma once

namespace alt
{
	namespace skeleton
	{
		//! @class LinkedListNode
		//! @brief 双方向連結用ノードクラス 
		//! @tparam T 格納データ型
		template<class T>
		class LinkedListNode
		{
		public:
			//! @brief コンストラクタ
			LinkedListNode<T> ()
			{
				_prev = nullptr;
				_next = nullptr;
				_value = nullptr;
			};

			//! @brief コンストラクタ
			//! @param value T&
			LinkedListNode<T> (T& value)
				:LinkedListNode<T> ()
			{
				_value = new T (value);
			};

			//! @brief デストラクタ
			APIENTRY ~LinkedListNode<T> ()
			{
				delete _value;
			};

			//! @brief 先頭を保持するノード
			LinkedListNode<T>* _prev;

			//! @brief 末尾を保持するノード
			LinkedListNode<T>* _next;

			//! @brief 管理するデータ
			T* _value;
		};

		//! @class LinkedListIterator
		//! @brief Node先頭～末尾までをサーチするイテレータ
		template<class T>
		class LinkedListIterator
		{
		public:
			//! @brief コンストラクタ
			LinkedListIterator<T> (LinkedListNode<T>* node)
			{
				_node = node;
			};

			//! @brief デストラクタ
			APIENTRY ~LinkedListIterator<T> ()
			{
				_node = nullptr;
			};

			//! @brief データアクセス用オペレータ
			//! @note 範囲for用
			T& APIENTRY operator * ()
			{
				return *(_node->_value);
			};

			//! @brief ノード移動用オペレータ
			//! @note 範囲for用
			LinkedListIterator<T>& APIENTRY operator ++ ()
			{
				_node = _node->_next;
				return *this;
			};

			//! @brief ノード末尾確認用オペレータ
			//! @param comp 比較元ノード
			//! @return bool
			//! @retval true(不一致)
			//! @retval false(一致)
			bool APIENTRY operator != (const LinkedListIterator<T>& comp)
			{
				return _node != comp._node;
			};

		private:
			LinkedListIterator<T> () = delete;
			LinkedListNode<T>* _node;
		};

		//! @class LinkedList
		//! @brief	ポインタによる連結リストを具現したクラス
		template<class T>
		class LinkedList
		{
		public:
			//! @brief コンストラクタ
			LinkedList<T> ()
			{
				_node = new LinkedListNode<T> ();
				_size = 0;
			};

			//! @brief コピーコンストラクタ
			//! @param base コピー元オブジェクト
			LinkedList<T> (const LinkedList<T>& base)
				: LinkedList<T> ()
			{
				operator =(base);
			};

			//! @brief デストラクタ
			virtual APIENTRY ~LinkedList<T> ()
			{
				Clear ();
				delete _node;
			};

			//! @brief LinkedList<T>へ値を追加
			//! @param item T*
			//! @note 連結リストの最前列に追加します。
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

			//! @brief LinkedList<T>へ値を追加
			//! @param item T*
			//! @note 連結リストの最後列に追加します。
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

			//! @brief 指定したインデックスの値を取得
			//! @param index 取得するインデックス
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

			//! @brief LinkedList<T>を配列操作で取得
			//! @param index 取得するインデックス
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief 同じ型のLinkedList<T>をコピー
			//! @param base コピー元 LinkedList<T>
			VOID APIENTRY operator = (const LinkedList<T>& base)
			{
				Clear ();

				for (auto& itr : base)
				{
					AddLast (itr);
				}
			}

			//! @brief LinkedList<T>の先頭アイテムを削除
			VOID APIENTRY RemoveFirst ()
			{
				if (_size == 0) return;

				LinkedListNode<T>* node1 = _node->_next;
				LinkedListNode<T>* node2 = node1->_next;
				delete node1;
				_node->_next = node2;

				_size--;
			};

			//! @brief LinkedList<T>の末尾アイテムを削除
			VOID APIENTRY RemoveLast ()
			{
				if (_size == 0) return;

				LinkedListNode<T>* node1 = _node->_prev;
				LinkedListNode<T>* node2 = node1->_prev;
				delete node1;
				_node->_prev = node2;

				_size--;
			};

			//! @brief LinkedList<T>から値を削除
			//! @param item 削除オブジェクト
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

			//! @brief LinkedList<T>へ値を追加
			//! @param item T&
			//! @note 連結リストの最後列に追加します。
			VOID APIENTRY Enqueue (T& item)
			{
				AddLast (item);
			};

			//! @brief LinkedList<T>から値を取得
			//! @param item T&
			//! @return bool
			//! @retval true(成功)
			//! @retval false(失敗)
			//! @note 連結リストの最前列から取得します。
			bool APIENTRY Dequeue (T& item)
			{
				bool ret = true;
				T* value = Get (0);

				if (value != nullptr) item = *value;
				else ret = false;

				RemoveFirst ();

				return ret;
			};

			//! @brief サイズを取得
			//! @return SIZE_T
			//! @retval 現在の連結リストのサイズ
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief 領域の開放
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

			//! @brief for(auto n : list){}を使用できるようにするインターフェース
			//! @return LinkedListIterator<T> 先頭イテレータ
			LinkedListIterator<T> APIENTRY begin () const
			{
				return LinkedListIterator<T> (_node->_next);
			};

			//! @brief for(auto n : list){}を使用できるようにするインターフェース
			//! @return LinkedListIterator<T> 末尾イテレータ
			LinkedListIterator<T> APIENTRY end () const
			{
				return LinkedListIterator<T> (nullptr);
			};

			//! @brief Iteratorからのアクセス権を解放
			friend LinkedListIterator<T>;

		private:
			LinkedListNode<T>* _node;
			SIZE_T _size;
		};
	}
}