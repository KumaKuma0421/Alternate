//! @file	TemplateTest.cpp
//! @brief	�e�X�g�N���X�̃e���v���[�g
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "WindowsLibrary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	// @sa https://docs.microsoft.com/ja-jp/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2022
	TEST_CLASS (TemplateTest)
	{
	public:
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Template")
		//	TEST_CLASS_ATTRIBUTE(L"Date", L"2019.12.21")
			TEST_CLASS_ATTRIBUTE(L"Ignore", L"true")
		END_TEST_CLASS_ATTRIBUTE()

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

		BEGIN_TEST_METHOD_ATTRIBUTE(TemplateSampleTest1)
		//	TEST_OWNER(L"kumakuma0421")
			TEST_DESCRIPTION(L"����͂��ׂẴe�X�g�̃e���v���[�g�ƂȂ�܂��B")
		//	TEST_PRIORITY(9)
		//	TEST_IGNORE()
			TEST_METHOD_ATTRIBUTE(L"Category", L"Template")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD (TemplateSampleTest1)
		{
			Assert::AreEqual<int> (123, 123, _T ("This test is failed.\n"));
			OutputDebugString (_T ("Output TemplateSampleTest.\n"));
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TemplateSampleTest2)
		//	TEST_OWNER(L"kumakuma0421")
			TEST_DESCRIPTION(L"����͂��ׂẴe�X�g�̃e���v���[�g�ƂȂ�܂��B")
		//	TEST_PRIORITY(9)
			TEST_IGNORE()
			TEST_METHOD_ATTRIBUTE(L"Category", L"Template")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD (TemplateSampleTest2)
		{
			Assert::AreEqual<int> (123, 123, _T ("This test is failed.\n"));
			OutputDebugString (_T ("Output TemplateSampleTest.\n"));
		}
	};
}