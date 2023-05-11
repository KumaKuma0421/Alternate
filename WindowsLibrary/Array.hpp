// ----------------------------------------------------------------------------
//! @file	Array.hpp
//! @brief	可変配列に関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "HeapMemory.h"

namespace alt
{
	namespace skeleton
	{
		//! @class Array
		//! @brief サイズ可変の配列を具現したクラス
		//! @note構造体、クラス以外は動作不可です。
		template<class T>
		class Array
		{
		public:
			//! @brief コンストラクタ
			Array<T> ()
			{
				_memory = new alt::BasicMemory ();
				_array = NULL;
				_size = 0;
				_reserve = 0;
			};

			//! @brief コンストラクタ
			//! @param memory IMemoryオブジェクト
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

			//! @brief コピーコンストラクタ
			//! @param base コピー元オブジェクト
			Array<T> (const Array<T>& base)
			{
				operator =(base);
			};

			//! @brief デストラクタ
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

			//! @brief 指定したインデックスの値を取得
			//! @param index 取得するインデックス
			//! @return T*
			T* APIENTRY Get (SIZE_T index) const
			{
				if (index >= _size) return (T*)NULL;
				return &(_array[index]);
			};

			//! @brief Array<T>を配列操作で取得
			//! @param index 取得するインデックス
			//! @return T*
			T* APIENTRY operator[](SIZE_T index) const
			{
				return Get (index);
			};

			//! @brief Array<T>へ値を追加
			//! @param item T&
			//! @note 配列の最後列に追加します。
			VOID APIENTRY Add (T& item)
			{
				Extend ();

				CopyMemory (&_array[_size], &item, sizeof (LPVOID)); // ★☆★
				_array[_size] = item;
				_size++;
			};

			//! @brief Array<T>へ値を挿入
			//! @param item T&
			//! @param position 挿入インデックス
			VOID APIENTRY Insert (T& item, SIZE_T position)
			{
				if (position > _size) return;

				Extend ();

				MoveMemory (
					&_array[position + 1],
					&_array[position],
					(_size - position) * sizeof (T));
				ZeroMemory (&_array[position], sizeof (T));
				CopyMemory (&_array[position], &item, sizeof (LPVOID)); // ★☆★
				_array[position] = item;

				_size++;
			};

			//! @brief Array<T>から値を削除
			//! @param position 削除インデックス
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

			//! @brief 削除を繰り返すことで発生するリザーブ領域を解放
			VOID APIENTRY Shurink ()
			{
				if (_reserve == 0) return;

				T* newArray = static_cast<T*>(_memory->Allocate (sizeof (T) * _size));
				CopyMemory (newArray, _array, (sizeof (T) * _size));
				_memory->Free (_array);
				// ちょっと強引な気もする。
				auto heapMemory = dynamic_cast<alt::HeapMemory*>(_memory);
				if (heapMemory != NULL) heapMemory->Compact ();
				_array = newArray;
				_reserve = 0;
			};

			//! @brief 事前に空の領域を取得
			//! @param size 取得サイズ（オブジェクト件数）
			VOID APIENTRY Reserve (SIZE_T size)
			{
				Extend (size);
			};

			//! @brief 領域の開放
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

			//! @brief 同じ型のArray<T>をコピー
			//! @param base コピー元 Array<T>
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
					CopyMemory (&_array[i], &base._array[i], sizeof (LPVOID)); // ★☆★
					_array[i] = base._array[i];
				}
				_reserve = 0;
			};

			//! @brief 同じ型のArray<T>を末尾に追加
			//! @param base 追加元 Array<T>
			VOID APIENTRY operator += (const Array<T>& base)
			{
				Extend (base._size - _reserve);

				for (SIZE_T i = 0; i < base._size; i++)
				{
					CopyMemory (&_array[_size + i], &base._array[i], sizeof (LPVOID)); // ★☆★
					_array[_size + i] = base._array[i];
				}
				_size += base._size;
				_reserve = 0;
			};

			//! @brief サイズを取得
			//! @return SIZE_T
			//! @retval 現在のリザーブ量を除く配列サイズ
			SIZE_T APIENTRY Size () const
			{
				return _size;
			};

			//! @brief for(auto n : array){}を使用できるようにするインターフェース
			//! @return T* 先頭アドレス
			T* APIENTRY begin ()
			{
				return &_array[0];
			};

			//! @brief for(auto n : array){}を使用できるようにするインターフェース
			//! @return T* 末尾アドレス
			T* APIENTRY end ()
			{
				return &_array[_size];
			};

			//! @brief for(auto& n : array){}を使用できるようにするインターフェース
			//! @return T* 先頭アドレス
			const T* APIENTRY begin () const
			{
				return &_array[0];
			};

			//! @brief for(auto& n : array){}を使用できるようにするインターフェース
			//! @return T* 末尾アドレス
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
					// #1 初期状態
					_array = static_cast<T*>(_memory->Allocate (sizeof (T) * size));
				}
				else if (_size > 0 && _reserve == 0)
				{
					// #2 確保済み、リザーブなし
					_array = static_cast<T*>(
						_memory->Reallocate (_array, sizeof (T) * (_size + size)));
				}
				else if (_size == 0 && _reserve > 0)
				{
					// #3 確保なし、リザーブあり
					_reserve--;
				}
				else
				{
					// #4 確保済み、リザーブあり
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