//! @file	MiddleLibraryTest.cpp
//! @brief	MiddleLibraryプロジェクトのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21
//! @sa     https://docs.microsoft.com/ja-jp/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2019#general_asserts
//! 		Microsoft.VisualStudio.TestTools.CppUnitTestFramework API リファレンス

#include "pch.h"
#include "MiddleLibrary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace alt;

namespace MiddleLibraryTest
{
	TEST_MODULE_INITIALIZE (ModuleInitialize)
	{
		Logger::WriteMessage ("module initialize.\n");
	}

	TEST_MODULE_CLEANUP (ModuleCleanup)
	{
		Logger::WriteMessage ("module cleanup.\n");
	}

	TEST_CLASS (MiddleLibraryTest)
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
			Logger::WriteMessage ("method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("method cleanup.\n");
		}

		TEST_METHOD (dllmainTest)
		{
			HMODULE hModule = NULL;
			DWORD  dwReasonForCall = 0;
			LPVOID lpvReserved = NULL;
			BOOL ret;

			ret = DllMain (hModule, dwReasonForCall, lpvReserved);
		}

		TEST_METHOD (ProfileTest1)
		{
			alt::MiddleLibrary target;
			TCHAR tszPath[MAX_PATH];

			lstrcpy (tszPath, _T ("C:\\Users\\User01\\source\\repos\\"));
			lstrcat (tszPath, _T ("Alternate\\MiddleLibraryTest\\"));
			lstrcat (tszPath, _T ("TestProfile.ini"));

			BOOL isExist = target.SetProfile (tszPath);
			Assert::IsTrue (isExist);

			alt::TString testString1 = target.GetProfileStr (_T ("Common"), _T ("TestString1"));
			Assert::AreEqual (_T ("ABCDE"), testString1.Ctr (), _T ("[Common]TestString1"));

			alt::TString testString2 = target.GetProfileStr (_T ("Common"), _T ("TestString2"));
			Assert::AreEqual (_T ("123 567"), testString2.Ctr (), _T ("[Common]TestString2"));

			UINT ret1 = target.GetProfileValue (_T ("Common"), _T ("TestValue1"));
			Assert::AreEqual ((UINT)123, ret1, _T ("[Common]TestValue1"));

			UINT ret2 = target.GetProfileValue (_T ("Common"), _T ("TestValue2"));
			Assert::AreEqual ((UINT)-456, ret2, _T ("[Common]TestValue2"));
		}

		TEST_METHOD (ProfileTest2)
		{
			alt::MiddleLibrary target;
			TCHAR tszPath[MAX_PATH];

			lstrcpy (tszPath, _T ("C:\\Users\\User01\\source\\repos\\"));
			lstrcat (tszPath, _T ("Alternate\\MiddleLibraryTest\\"));
			lstrcat (tszPath, _T ("TestProfile.ini"));

			BOOL isExist = target.SetProfile (tszPath);
			Assert::IsTrue (isExist);

			alt::TString testString1 = target.GetProfileStr (_T ("Section1"), _T ("TestString1"));
			Assert::AreEqual (_T ("abcde"), testString1.Ctr (), _T ("[Common]TestString1"));

			alt::TString testString2 = target.GetProfileStr (_T ("Section1"), _T ("TestString2"));
			Assert::AreEqual (_T ("987 543"), testString2.Ctr (), _T ("[Common]TestString2"));

			UINT ret1 = target.GetProfileValue (_T ("Section1"), _T ("TestValue1"));
			Assert::AreEqual ((UINT)456, ret1, _T ("[Common]TestValue1"));

			UINT ret2 = target.GetProfileValue (_T ("Section1"), _T ("TestValue2"));
			Assert::AreEqual ((UINT)-789, ret2, _T ("[Common]TestValue2"));
		}
	};
}