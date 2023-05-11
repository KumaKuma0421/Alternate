//! @file	DesignPattern.hpp
//! @brief	種々のデザインパターンの実装クラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#pragma once

#include "framework.h"
#include "pch.h"

namespace alt
{
	namespace skeleton
	{
		//! @class	ISupplier
		//! @brief	ISupplierインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template <class T>
		class ISupplier
		{
		public:
			//! @brief	Tを返す関数 get()
			//! @return	T
			virtual T get () = 0;
		};

		//! @class	IConsumer
		//! @brief	IConsumerインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template <class T>
		class IConsumer
		{
		public:
			//! @brief		Tを引数とする関数 accept()
			//! @param[in]	t テンプレートT
			virtual void accept (T t) = 0;
		};

		//! @class	IPredicate
		//! @brief	IPredicateインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template <class T>
		class IPredicate
		{
		public:
			//! @brief		Tを引数とする関数 test()
			//! @param[in]	t テンプレートT
			//! @return		実行結果
			//! @retval		true:成功
			//! @retval		false:失敗
			virtual bool test (T t) = 0;
		};

		//! @class	IFunction
		//! @brief	IFunctionインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template <class T, class V>
		class IFunction
		{
		public:
			//! @brief		Tを引数とする関数 apply()
			//! @param[in]	t テンプレートT
			//! @return		V テンプレートV
			virtual V apply (T t) = 0;
		};

		//! @class	IObserver
		//! @brief	IObserverインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<typename Data>
		class IObserver
		{
		public:
			//! @brief		Dataを引数とする関数 Update()
			//! @param[in]	data テンプレートData
			virtual void Update (Data data) = 0;
		};

		//! @class	ISubscriber
		//! @brief	ISubscriberインターフェースクラス
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<typename Data>
		class ISubscriber
		{
		public:
			//! @brief Dataを引数とする関数 Notify()
			//! @param data テンプレートData
			virtual void Notify (Data data)
			{
				for (IObserver<Data>* observer : _observers)
				{
					observer->Update (data);
				}
			}

			//! @brief		IObserver<Data>*を引数とする関数 Add()
			//! @param[in]	observer Subscriberに登録するObserverオブジェクト
			virtual void Add (IObserver<Data>* observer)
			{
				_observers.push_back (observer);
			}

			//! @brief		IObserver<Data>*を引数とする関数 Remove()
			//! @param[in]	observer Subscriberから削除するObserverオブジェクト
			virtual void Remove (IObserver<Data>* observer)
			{
				for (auto item : _observers)
				{
					if (item == observer)
					{
						//! @todo 消し方は後で実装する
					}
				}
			}

		private:
			//! @brief Subscriberが管理するObserverの保存用
			std::vector<IObserver<Data>*> _observers;
		};

		//! @class	Strategy
		//! @brief	Strategy
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<class T, class V>
		class Strategy
		{
		public:
			//! @brief コンストラクタ
			Strategy () {};

			//! @brief デストラクタ
			virtual ~Strategy () {};

			//! @brief	テンプレート引数Tに応じて、テンプレートVを返却します。
			//! @param	t テンプレートT
			//! @return	V テンプレートV
			//! @retval	事前にSet()で設定した、Tに紐付くオブジェクト
			virtual V Get (T t)
			{
				return _strategy[t];
			};

			//! @brief	テンプレートTをキーとしたテンプレートVを設定します。
			//! @param	t テンプレートT
			//! @param	v テンプレートV
			virtual void Set (T t, V v)
			{
				_strategy.insert (std::make_pair (t, v));
			};

		private:
			//! @brief Set()で登録されたオブジェクトを管理するマップ
			std::map<T, V> _strategy;
		};

		//! @class	Singleton
		//! @brief	シングルトン テンプレート
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<class T>
		class Singleton
		{
		public:
			//! @brief 唯一となるインスタンスを取得します。
			//! @return T*インスタンス
			static T* Instance ()
			{
				if (_instance == nullptr)
				{
					_instance = new T ();
				}

				return _instance;
			};

		private:
			//! @brief コンストラクタ(privateで秘匿状態)
			Singleton () {};

			//! @brief 唯一となるインスタンスを管理する変数
			static T* _instance;
		};

		//! @class	Command
		//! @brief	コマンド テンプレート
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<typename CommandFunction>
		class Command
		{
		public:
			Command ()
			{
				_function = nullptr;
				_waitTime = 0;
			};

			void SetFunction (CommandFunction function) { _function = function; };
			virtual bool Execute (size_t count, size_t total) { return _function (count, total); };
			long GetWaitTime () { return _waitTime; };
			void SetWaitTime (long time) { _waitTime = time; };

		private:
			CommandFunction _function;
			long _waitTime;
		};

		//! @class	Commander
		//! @brief	コマンド テンプレート
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		template<typename CommandFunction>
		class Commander
		{
		public:
			void Add (Command<CommandFunction>* command) { _commands.push_back (command); };
			bool Execute ()
			{
				int ret = 0;
				size_t count = 0;
				const size_t total = _commands.size ();

				for (auto command : _commands)
				{
					ret = command->Execute (++count, total);
					if (ret == false) break;

					long waitTime = command->GetWaitTime ();
					Sleep (waitTime);
				}

				return ret;
			};

		private:
			std::vector<Command<CommandFunction>*> _commands;
		};

		//! @class	CompositeLeaf
		//! @brief	コンポジット テンプレート
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		class CompositeLeaf
		{
		public:
			std::wstring GetName () { return _leafName; };

			void SetName (std::wstring& name) { _leafName = name; };

			void Add (std::wstring key, std::wstring value)
			{
				_properties.insert (std::make_pair<std::wstring&, std::wstring&> (key, value));
			}

			std::wstring Get (std::wstring key) { return _properties[key]; };

		private:
			std::wstring _leafName;
			std::map<std::wstring, std::wstring> _properties;
		};

		//! @class CompositeNode
		//! @brief	コンポジット テンプレート
		//! @note	テンプレートクラスなのでDLL_DECLSPECは付けません。
		class CompositeNode
		{
		public:
			std::wstring GetName () { return _nodeName; };

			void SetName (std::wstring name) { _nodeName = name; };

			void Add (std::wstring key, CompositeNode& node)
			{
				_nodes.insert (std::make_pair<std::wstring&, CompositeNode&> (key, node));
			};

			CompositeNode& GetNode (std::wstring key) { return _nodes[key]; };

			std::vector<std::wstring> GetNodeKeys ()
			{
				std::vector<std::wstring> response;

				for (auto itr = _nodes.begin (); itr != _nodes.end (); ++itr)
				{
					response.push_back (itr->first);
				}

				return response;
			};

			std::vector<CompositeNode> GetNodeValues ()
			{
				std::vector<CompositeNode> response;

				for (auto itr = _nodes.begin (); itr != _nodes.end (); ++itr)
				{
					response.push_back (itr->second);
				}

				return response;
			}

			void Add (std::wstring key, CompositeLeaf& leaf)
			{
				_leafs.insert (std::make_pair<std::wstring&, CompositeLeaf&> (key, leaf));
			};

			CompositeLeaf& GetLeaf (std::wstring key) { return _leafs[key]; };

			std::vector<std::wstring> GetLeafKeys ()
			{
				std::vector<std::wstring> response;

				for (auto itr = _leafs.begin (); itr != _leafs.end (); ++itr)
				{
					response.push_back (itr->first);
				}

				return response;
			};

			std::vector<CompositeLeaf> GetLeafValues ()
			{
				std::vector<CompositeLeaf> response;

				for (auto itr = _leafs.begin (); itr != _leafs.end (); ++itr)
				{
					response.push_back (itr->second);
				}

				return response;
			}

		private:
			std::wstring _nodeName;
			std::map<std::wstring, CompositeNode>_nodes;
			std::map<std::wstring, CompositeLeaf> _leafs;
		};
	}
}