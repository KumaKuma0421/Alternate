//! @file	UtilityTest.cpp
//! @brief	汎用的に使えるユーティリティクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#include "pch.h"
#include "Utility.h"
#include "File.h"
#include "FileUtility.h"
#include "Array.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (UtilityTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("UtilityTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("UtilityTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("UtilityTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("UtilityTest method cleanup.\n");
		}

		TEST_METHOD (CreateGUIDTest)
		{
			for (int i = 0; i < 10; i++)
			{
				auto guid = alt::Utility::MakeGUID ();
				Logger::WriteMessage (guid.Ctr ());
				Logger::WriteMessage (_T ("\n"));
			}
		}

		TEST_METHOD (GetSystemTimeAndLocalTimeTest)
		{
			alt::TString message;
			auto t1 = alt::Utility::GetSystemTime ();
			auto t2 = alt::Utility::GetLocalTime ();

			message.Format (_T ("SystemTime:%04d/%02d/%02dT%02d:%02d:%02d.%03d %d\n"),
				t1.wYear, t1.wMonth, t1.wDay,
				t1.wHour, t1.wMinute, t1.wSecond,
				t1.wMilliseconds, t1.wDayOfWeek);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T (" LocalTime:%04d/%02d/%02dT%02d:%02d:%02d.%03d %d\n"),
							t2.wYear, t2.wMonth, t2.wDay,
							t2.wHour, t2.wMinute, t2.wSecond,
							t2.wMilliseconds, t2.wDayOfWeek);
			Logger::WriteMessage (message.Ctr ());
		}

		TEST_METHOD (GetFormatTimeTest)
		{
			Logger::WriteMessage (alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ()).Ctr ());
		}

		TEST_METHOD (GetLocalTimeQuadTest)
		{
			auto res = alt::Utility::GetLocalTimeQuad ();
			alt::TString res2;
			res2.Format (_T ("%lld"), res.QuadPart);
			Logger::WriteMessage (res2.Ctr ());
		}

		TEST_METHOD (GetTimeByFileTimeTest1)
		{
			auto t = alt::Utility::GetLocalTimeQuad ();
			FILETIME t2{ 0, 0 };
			t2.dwLowDateTime = t.LowPart;
			t2.dwHighDateTime = t.HighPart;

			auto t3 = alt::Utility::GetTimeByFileTime (t2);
			auto t4 = alt::Utility::GetFormatTime (t3);
			Logger::WriteMessage (t4.Ctr ());
		}

		TEST_METHOD (GetTimeByFileTimeTest2)
		{
			auto t = alt::Utility::GetSystemTime ();
			FILETIME ft;
			::SystemTimeToFileTime (&t, &ft);

			auto t3 = alt::Utility::GetTimeByFileTime (ft);
			auto t4 = alt::Utility::GetFormatTime (t3);
			Logger::WriteMessage (t4.Ctr ());
		}

		TEST_METHOD (GetSystemInfoTest)
		{
			alt::TString message;
			SYSTEM_INFO si;
			alt::Utility::GetSystemInfo (si);

			message.Format (_T ("dwPageSize:%d\n"), si.dwPageSize);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("lpMinimumApplicationAddress:%p\n"), si.lpMinimumApplicationAddress);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("lpMaximumApplicationAddress:%p\n"), si.lpMaximumApplicationAddress);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("dwActiveProcessorMask:%d\n"), si.dwActiveProcessorMask);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("dwNumberOfProcessors:%d\n"), si.dwNumberOfProcessors);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("dwProcessorType:%d\n"), si.dwProcessorType);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("dwAllocationGranularity:%d\n"), si.dwAllocationGranularity);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("wProcessorLevel:%d\n"), si.wProcessorLevel);
			Logger::WriteMessage (message.Ctr ());

			message.Format (_T ("wProcessorRevision:%d\n"), si.wProcessorRevision);
			Logger::WriteMessage (message.Ctr ());
		}

		TEST_METHOD (Base64EncodeAndDecodeTest1)
		{
			BYTE byBinary[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B', 'C', 'D' };
			alt::TString response;

			BOOL ret = alt::Utility::Base64Encode (byBinary, sizeof (byBinary), response);
			Assert::IsTrue (ret);

			Logger::WriteMessage (response.Ctr ());

			DWORD dwBuffer;
			ret = alt::Utility::Base64Decode (response, nullptr, dwBuffer);
			Assert::IsTrue (ret);

			LPBYTE lpbyBuffer = new BYTE[dwBuffer * sizeof (TCHAR)];
			ret = alt::Utility::Base64Decode (response, lpbyBuffer, dwBuffer);
			Assert::IsTrue (ret);
			for (int i = 0; i < sizeof (byBinary); i++)
			{
				Assert::AreEqual<BYTE> (byBinary[i], lpbyBuffer[i]);
			}
			delete[] lpbyBuffer;
		}

		TEST_METHOD (Base64EncodeAndDecodeTest2)
		{
			alt::File binFile;
			BOOL ret;

			ret = binFile.Create (_T ("C:\\Windows\\Notepad.exe"), GENERIC_READ, 0, OPEN_EXISTING);
			Assert::IsTrue (ret);

			DWORD dwSize = (DWORD)binFile.GetSize ();
			LPBYTE lpbyBinary = new BYTE[dwSize];

			DWORD dwReadSize = binFile.Read (lpbyBinary, dwSize);
			Assert::AreEqual<DWORD> (dwReadSize, dwSize);

			alt::TString response;
			ret = alt::Utility::Base64Encode (lpbyBinary, dwSize, response);
			Assert::IsTrue (ret);
			Logger::WriteMessage (response.Ctr ());

			DWORD dwBuffer;
			ret = alt::Utility::Base64Decode (response, nullptr, dwBuffer);
			Assert::IsTrue (ret);

			LPBYTE lpbyBuffer = new BYTE[dwBuffer];
			ret = alt::Utility::Base64Decode (response, lpbyBuffer, dwBuffer);
			Assert::IsTrue (ret);
			for (DWORD i = 0; i < dwSize; i++)
			{
				Assert::AreEqual<BYTE> (lpbyBinary[i], lpbyBuffer[i]);
			}

			delete[] lpbyBuffer;
			delete[] lpbyBinary;
		}

		TEST_METHOD (GetEnvironmentsTest)
		{
			auto ret = alt::Utility::GetEnvironments ();
			for (auto item : ret)
			{
				Logger::WriteMessage (item.Ctr ());
				Logger::WriteMessage (_T ("\n"));
			}
		}

		TEST_METHOD (GetEnvTest)
		{
			auto ret = alt::Utility::GetEnv (_T ("PATH"));
			auto ret2 = ret.Split (alt::TString (_T (";")));
			for (auto item : ret2)
			{
				Logger::WriteMessage (item.Ctr ());
				Logger::WriteMessage (_T ("\n"));
			}

			auto ret3 = alt::Utility::GetEnv (_T ("SolutionDir"));
			Assert::IsTrue (ret3.Len () == 0);
		}

		TEST_METHOD (IniFileIOTest1)
		{
			BOOL ret;
			LPCTSTR lpctszIniFile = _T (".\\UtilityTest.ini");
			LPCTSTR lpctszSection = _T ("Section");
			LPCTSTR lpctszKeyword = _T ("Keyword");
			alt::TString response1 = _T ("日本語 の 値 を 設定");
			alt::TString response2;

			if (alt::FileUtility::IsExist (lpctszIniFile))
				alt::FileUtility::Delete (lpctszIniFile);

			ret = alt::Utility::WriteIniFile (
				lpctszIniFile, lpctszSection, lpctszKeyword, response1);
			Assert::IsTrue (ret, _T ("Utility::WriteIniFile() write error.\n"));

			ret = alt::Utility::ReadIniFile (
				lpctszIniFile, lpctszSection, lpctszKeyword, response2);
			Assert::IsTrue (ret, _T ("Utility::ReadIniFile() read error.\n"));
			Assert::IsTrue (
				response1 == response2,
				_T ("Utility::Read/WriteIniFile() value mismatched.\n"));
		}

		TEST_METHOD (IniFileIOTest2)
		{
			BOOL ret;
			LPCTSTR lpctszIniFile = _T (".\\UtilityTest.ini");
			LPCTSTR lpctszSection = _T ("Section");
			LPCTSTR lpctszKeyword = _T ("Keyword");
			INT iValue1 = 12345;
			INT iValue2 = -1;

			if (alt::FileUtility::IsExist (lpctszIniFile))
				alt::FileUtility::Delete (lpctszIniFile);

			ret = alt::Utility::WriteIniFile (
				lpctszIniFile, lpctszSection, lpctszKeyword, iValue1);
			Assert::IsTrue (ret, _T ("Utility::WriteIniFile() write error.\n"));

			iValue2 = alt::Utility::ReadIniFile (
				lpctszIniFile, lpctszSection, lpctszKeyword, -1);
			Assert::AreNotEqual (-1, ret, _T ("Utility::ReadIniFile() read error.\n"));
			Assert::IsTrue (
				iValue1 == iValue2,
				_T ("Utility::Read/WriteIniFile() value mismatched.\n"));
		}

		TEST_METHOD (IniFileIOTest3)
		{
			BOOL ret;
			LPCTSTR lpctszIniFile = _T (".\\UtilityTest.ini");
			LPCTSTR lpctszSection = _T ("Section");
			alt::TString keyword;

			if (alt::FileUtility::IsExist (lpctszIniFile))
				alt::FileUtility::Delete (lpctszIniFile);

			int values[]{ -12345, -1234, -123, -12, -1, 0, 1, 22, 333, 4444, 55555, 666666 };
			int loop = sizeof (values) / sizeof (int);

			for (int i = 0; i < loop; i++)
			{
				keyword.Format (_T ("keyword_%02d"), i);
				alt::TString response;
				response << values[i];
				ret = alt::Utility::WriteIniFile (lpctszIniFile, lpctszSection, keyword.Ctr (), response);
				Assert::IsTrue (ret, _T ("Utility::WriteIniFile() write error.\n"));
			}

			for (int i = 0; i < loop; i++)
			{
				keyword.Format (_T ("keyword_%02d"), i);
				alt::TString response;
				ret = alt::Utility::ReadIniFile (lpctszIniFile, lpctszSection, keyword.Ctr (), response);
				Assert::IsTrue (ret, _T ("Utility::WriteIniFile() write error.\n"));
				Assert::AreEqual<int> (values[i], response.ParseInt ());
			}
		}

		TEST_METHOD (CreateMD5Test1)
		{
			LPCSTR lpcszString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			BYTE  byMD5[40];
			LPSTR lpszResponse = "2AD372C377013BAA4EE32AB6649D2449";

			Assert::IsTrue (alt::Utility::CreateMD5 ((LPBYTE)lpcszString, lstrlenA (lpcszString), byMD5), _T ("CreateMD5() failed.\n"));
			Assert::IsTrue (_memicmp (lpszResponse, byMD5, 32) == 0);
		}

		TEST_METHOD (CreateMD5Test2)
		{
			LPCSTR lpcszResponse = "BA929713DC6AAEBFEDE06D7C6C96E54C";
			LPCTSTR lpctszFileName = _T (".\\MD5_Check.txt");
			alt::File testFile;
			BYTE  byMD5[40];
			alt::TString message;
			Assert::IsTrue (testFile.Create (lpctszFileName, GENERIC_WRITE, 0, CREATE_ALWAYS), _T ("Test file create failed.\n"));
			for (int i = 0; i < 1024; i++)
			{
				message.Format (_T ("This line is No.%d\n"), i + 1);
				Assert::IsTrue (-1 != testFile.Write (message.Ctr (), message.Byte ()), _T ("Test file write failed.\n"));
			}
			Assert::IsTrue (testFile.Close (), _T ("Test file close failed.\n"));

			Assert::IsTrue (alt::Utility::CreateMD5 (lpctszFileName, byMD5), _T ("CreateMD5() failed.\n"));
			Assert::IsTrue (_memicmp (lpcszResponse, byMD5, 32) == 0, _T ("MD5 response invalid.\n"));

			Assert::IsTrue (alt::FileUtility::Delete (lpctszFileName), _T ("Test file delete failed.\n"));
		}

		TEST_METHOD (QuickSortTest1)
		{
			const INT count = 1000;
			SHORT asRand[count];
			srand (123);

			for (int i = 0; i < count; i++)
			{
				asRand[i] = rand ();
			}

			alt::Utility::QuickSort<SHORT> (asRand, 0, count - 1);

			for (int i = 0; i < count - 1; i++)
			{
				Assert::IsTrue (asRand[i] <= asRand[i + 1]);
			}
		}

		class Container
		{
		public:
			Container () { _wPrice = 0; };

			Container (LPCTSTR lpctszKey, LPCTSTR lpctszValue, WORD wPrice) { Set (lpctszKey, lpctszValue, wPrice); };

			Container (const Container& base) { _key = base._key; _value = base._value; _wPrice = base._wPrice; };

			void Set (LPCTSTR lpctszKey, LPCTSTR lpctszValue, WORD wPrice)
			{
				_key = lpctszKey;
				_value = lpctszValue;
				_wPrice = wPrice;
			};

			void Get (alt::TString& key, alt::TString& value, WORD& wPrice)
			{
				key = _key;
				value = _value;
				wPrice = _wPrice;
			};

			bool operator < (const Container& base) const
			{
				bool ret;

				// if (_wPrice < base._wPrice) ret = true;
				// if (_key < base._key) ret = true;
				if (_value < base._value) ret = true;
				else ret = false;

				return ret;
			};

			VOID operator = (const Container& base)
			{
				_key = base._key;
				_value = base._value;
				_wPrice = base._wPrice;
			};

		private:
			alt::TString _key;
			alt::TString _value;
			WORD _wPrice;
		};

		TEST_METHOD (QuickSortTest2)
		{
			const INT count = 10;
			Container container0 (_T ("Strawberry"), _T ("イチゴ"), 350);
			Container container1 (_T ("Pineapple"), _T ("パイナップル"), 280);
			Container container2 (_T ("Orange"), _T ("オレンジ"), 120);
			Container container3 (_T ("Melon"), _T ("メロン"), 1350);
			Container container4 (_T ("Mango"), _T ("マンゴー"), 720);
			Container container5 (_T ("Lemon"), _T ("レモン"), 130);
			Container container6 (_T ("Kiwifruit"), _T ("キウイフルーツ"),60);
			Container container7 (_T ("Cherry"), _T ("サクランボ"), 20);
			Container container8 (_T ("Banana"), _T ("バナナ"), 70);
			Container container9 (_T ("Apple"), _T ("リンゴ"), 130);

			Container fruits[count] {
				container0, container1, container2, container3,
				container4,container5,container6,container7,container8,
				container9 };

			for (auto item : fruits)
			{
				alt::TString key(40);
				alt::TString value(40);
				WORD wPrice;
				alt::TString message;

				item.Get (key, value, wPrice);
				message.Format (_T ("%s\t%s\t%d\n"), key.Ctr(), value.Ctr(), wPrice);
				Logger::WriteMessage (message.Ctr());
			}
			Logger::WriteMessage ("--------------------------------------------\n");

			alt::Utility::QuickSort<Container> (fruits, 0, count - 1);

			for (auto item : fruits)
			{
				alt::TString key(40);
				alt::TString value(40);
				WORD wPrice;
				alt::TString message;
				
				item.Get (key, value, wPrice);
				message.Format (_T ("%s\t%s\t%d\n"), key.Ctr(), value.Ctr(), wPrice);
				Logger::WriteMessage (message.Ctr());
			}
		}
	};
}