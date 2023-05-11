/*
 https://docs.microsoft.com/ja-jp/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2019#general_asserts
 Microsoft.VisualStudio.TestTools.CppUnitTestFramework API リファレンス
 */
#include "pch.h"
#include "WorkerTemplate.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace alt;

namespace alt
{
	class WorkerFunction
	{
	public:
		WorkerFunction ()
		{
			_message = nullptr;
			_dwSize = 0;
			_data = nullptr;
		};

		virtual ~WorkerFunction ()
		{
			delete _message;
			delete _data;
		};

		std::function<bool ()> init = [&]()
		{
			_message = new Message ();
			_message->SetFrom (0);
			_message->SetTo (1);
			_message->SetCommand (-1);

			_data = new Data (64);
			_data->SetByte ((LPBYTE)"THIS IS A SAMPLE STRING.", 24);

			return true;
		};

		std::function<bool ()> start = [&]()
		{
			return true;
		};

		std::function<bool (Message message)> command = [&](Message message)
		{
			return true;
		};

		std::function<Data (Data data)> process = [&](Data data) -> Data
		{
			return data;
		};

		std::function<bool ()> stop = [&]()
		{
			return true;
		};

		std::function<bool ()> exit = [&]()
		{
			return true;
		};

	private:
		Message* _message;
		Data* _data;
		DWORD _dwSize;
	};
}

namespace MiddleLibraryTest
{
	TEST_CLASS (WorkerTemplateTest)
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

		TEST_METHOD (SequencialCall)
		{
			//! @todo コピーコンストラクタがまだ動いている。
			//! インスタンスを引数にしているところは'&'への変更が必要。
			WorkerTemplate workerTemplate;
			WorkerFunction workerFunction;
			Data data (128);
			data.SetByte ((LPBYTE)"1234567890", 10);
			Message message;

			[] (WorkerTemplate& t, WorkerFunction& f)
			{
				bool ret = t.Init (f.init);
				Assert::AreEqual<bool> (true, ret);

			}(workerTemplate, workerFunction);

			[] (WorkerTemplate& t, WorkerFunction& f)
			{
				bool ret = t.Start (f.start);
				Assert::AreEqual<bool> (true, ret);

			}(workerTemplate, workerFunction);

			[] (WorkerTemplate& t, WorkerFunction& f, Message& msg)
			{
				bool ret = t.Command (f.command, msg);
				Assert::AreEqual<bool> (true, ret);

			}(workerTemplate, workerFunction, message);

			[] (WorkerTemplate& t, WorkerFunction& f, Data& d)
			{
				Data ret = t.Process (f.process, d);
				Assert::AreEqual (0, lstrcmpA ("1234567890", (LPSTR)ret.GetByte ()));

			}(workerTemplate, workerFunction, data);

			[] (WorkerTemplate& t, WorkerFunction& f)
			{
				bool ret = t.Stop (f.stop);
				Assert::AreEqual<bool> (true, ret);

			}(workerTemplate, workerFunction);

			[] (WorkerTemplate& t, WorkerFunction& f)
			{
				bool ret = t.Exit (f.exit);
				Assert::AreEqual<bool> (true, ret);

			}(workerTemplate, workerFunction);
		}
	};
}