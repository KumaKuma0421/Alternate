/*
 https://docs.microsoft.com/ja-jp/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2019#general_asserts
 Microsoft.VisualStudio.TestTools.CppUnitTestFramework API リファレンス
 */
#include "pch.h"
#include "PipelineFactoryMethod.h"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace alt::FactoryMethod;

namespace MiddleLibraryTest
{
	TEST_CLASS (PipelineFactoryMethodTest)
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

		TEST_METHOD (PipelineFactoryMethodTest1)
		{
			PipelineFactoryMethod* factory = new PipelineFactoryMethod ();

			ProductManager* manager = (ProductManager*)factory->Buildup ();

			Config* config = new Config ();
			bool ret = config->Load (_T (".\\Config.ini"));
			Assert::IsTrue (ret, _T ("Configファイルの読み込みに失敗しました。"));

			manager->Init (config);

			for (int i = 0; i < 1; i++)
			{
				manager->Start ();

				Sleep (30000); // 30秒

				manager->Stop ();
			}

			manager->Exit ();

			Sleep (10000); // 10秒

			Logger::WriteMessage ("PipelineFactoryMethodTest1 Stopped.\n");
		}
	};
}