// ----------------------------------------------------------------------------
//! @file	Map.hpp
//! @brief	マップに関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.8.16

#pragma once

#include "Utility.h"

namespace alt
{
	namespace skeleton
	{
		//! @class MapNode
		//! @brief 双方向連結用ノードクラス 
		//! @tparam T 格納データ型
		template<class T>
		class MapNode
		{
		public:
			//! @brief コンストラクタ
			MapNode<T> ()
			{
				_prev = nullptr;
				_next = nullptr;
				_value = nullptr;
				ZeroMemory (_byMD5, sizeof (_byMD5));
			};

			//! @brief コンストラクタ
			//! @param value T&
			MapNode<T> (T& value)
				:MapNode<T> ()
			{
				_value = new T (value);
				alt::Utility::CreateMD5 ((LPBYTE)_value, sizeof (T), _byMD5);
			};

			//! @brief デストラクタ
			APIENTRY ~MapNode<T> ()
			{
				delete _value;
			};

			//! @brief 先頭を保持するノード
			MapNode<T>* _prev;

			//! @brief 末尾を保持するノード
			MapNode<T>* _next;

			//! @brief 管理するデータ
			T* _value;

			//! @brief データのMD5
			BYTE _byMD5[32];
		};

		//! @class MapIterator
		//! @brief Node先頭～末尾までをサーチするイテレータ
		template<class T>
		class MapIterator
		{
		public:
			//! @brief コンストラクタ
			MapIterator<T> (MapNode<T>* node)
			{
				_node = node;
			};

			//! @brief デストラクタ
			APIENTRY ~MapIterator<T> ()
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
			MapIterator<T>& APIENTRY operator ++ ()
			{
				_node = _node->_next;
				return *this;
			};

			//! @brief ノード末尾確認用オペレータ
			//! @param comp 比較元ノード
			//! @return bool
			//! @retval true(不一致)
			//! @retval false(一致)
			bool APIENTRY operator != (const MapIterator<T>& comp)
			{
				return _node != comp._node;
			};

		private:
			MapIterator<T> () = delete;
			MapNode<T>* _node;
		};

		//! @class Map
		//! @brief	ポインタによるマップを具現したクラス
		template<class T>
		class Map
		{
		public:
			//! @brief コンストラクタ
			Map<T> ()
			{
				_node = new MapNode<T> ();
				_index = nullptr;
				_size = 0;
			};

			//! @brief コピーコンストラクタ
			//! @param base コピー元オブジェクト
			Map<T> (const Map<T>& base)
				: Map<T> ()
			{
				operator =(base);
			};

			//! @brief デストラクタ
			virtual APIENTRY ~Map<T> ()
			{
				Clear ();
				delete _node;
			};

			//! @brief Map<T>へ値を追加
			//! @param item T*
			//! @note マップのMD5順に追加します。
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

			//! @brief Map<T>から値を削除
			//! @param item 削除オブジェクト
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

			//! @brief 指定したインデックスの値を取得
			//! @param index 取得するインデックス
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

			//! @brief Map<T>を配列操作で取得
			//! @param index 取得するインデックス
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief 同じ型のMap<T>をコピー
			//! @param base コピー元 Map<T>
			VOID APIENTRY operator = (const Map<T>& base)
			{
				Clear ();

				for (auto& itr : base)
				{
					Add (itr);
				}
			}

			//! @brief サイズを取得
			//! @return SIZE_T
			//! @retval 現在のマップのサイズ
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief 領域の開放
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

			//! @brief for(auto n : map){}を使用できるようにするインターフェース
			//! @return MapIterator<T> 先頭イテレータ
			MapIterator<T> APIENTRY begin () const
			{
				return MapIterator<T> (_node->_next);
			};

			//! @brief for(auto n : list){}を使用できるようにするインターフェース
			//! @return MapIterator<T> 末尾イテレータ
			MapIterator<T> APIENTRY end () const
			{
				return MapIterator<T> (nullptr);
			};

			//! @brief Iteratorからのアクセス権を解放
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