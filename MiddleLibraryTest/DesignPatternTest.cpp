//! @file	DesignPatternTest.cpp
//! @brief	種々のデザインパターンの実装クラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "DesignPattern.hpp"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace alt;

TString* skeleton::Singleton<TString>::_instance = nullptr;

namespace MiddleLibraryTest
{
	TEST_CLASS (DesignPatternTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("-->>method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("<<--method cleanup.\n");
		}

		TEST_METHOD (SingletonTest)
		{
			TString* pString1 = skeleton::Singleton<TString>::Instance ();
			*pString1 = _T ("SINGLETON_TEST");

			TString* pString2 = skeleton::Singleton<TString>::Instance ();

			Assert::AreEqual (_T ("SINGLETON_TEST"), pString2->Ptr ());
			Assert::IsTrue (pString1 == pString2);
			Assert::AreEqual (pString1->Ctr (), pString2->Ctr ());
		}

		static bool testFunction (size_t count, size_t total)
		{
			TString string (_T ("CommandFunction:"));
			string << static_cast<const int>(count) << _T ("/") << static_cast<const int>(total) << _T ("\n");
			Logger::WriteMessage (string.Ptr ());

			return true;
		}

		TEST_METHOD (CommandTest)
		{
			// ラムダ式ではcommander.Execute()する頃にはすでに
			// 消滅しており、std::weak_ptrなども試してみましたが
			// うまくいかず、今後の課題です。
			typedef bool (*CommandFunction)(size_t, size_t);

			skeleton::Commander<CommandFunction> commander;

			for (int i = 0; i < 10; i++)
			{
				skeleton::Command<CommandFunction> command;
				command.SetFunction (testFunction);
				command.SetWaitTime (1000);
				commander.Add (&command);
			}

			commander.Execute ();
		}
	};
}