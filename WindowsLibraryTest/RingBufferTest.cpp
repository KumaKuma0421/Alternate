//! @file	RingBufferTest.cpp
//! @brief	リングバッファに関するクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#include "pch.h"
#include "RingBuffer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (RingBufferTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("RingBuffer class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("RingBuffer class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("RingBuffer method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("RingBuffer method cleanup.\n");
		}

		TEST_METHOD (RingBufferTest1)
		{
			const SIZE_T bufferSize = 32;
			const SIZE_T chunkSize = 6;
			BOOL ret;

			alt::RingBuffer ringBuffer;

			ret = ringBuffer.Init (bufferSize);
			Assert::IsTrue (ret, _T("RingBuffer::Init() failed.\n"));

			BYTE byWriteBuf[chunkSize];
			BYTE byReadBuf[chunkSize];

			for (SIZE_T i = 0; i < 24; i++)
			{
				FillMemory (byWriteBuf, sizeof (byWriteBuf), (BYTE)(i + 'A'));

				ret = ringBuffer.Write (byWriteBuf, sizeof (byWriteBuf));
				Assert::IsTrue (ret, _T("RingBuffer::Write() failed.\n"));
				Assert::AreEqual<SIZE_T> (chunkSize, ringBuffer.Remain (), _T("RingBuffer::Remain() invalid size.\n"));

				ZeroMemory (byReadBuf, sizeof (byReadBuf));

				ret = ringBuffer.Read (byReadBuf, sizeof (byReadBuf));
				Assert::IsTrue (ret, _T("RingBuffer::Read() failed.\n"));

				int cmp = memcmp (byWriteBuf, byReadBuf, chunkSize);
				Assert::AreEqual (0, cmp, _T("RingBuffer::Read() not matched write data.\n"));
			}
		}

		TEST_METHOD (RingBufferTest2)
		{
			const SIZE_T bufferSize = 64;
			const SIZE_T chunkSize = 5;
			BOOL ret;

			alt::RingBuffer ringBuffer;

			ret = ringBuffer.Init (bufferSize);
			Assert::IsTrue (ret, _T("RingBuffer::Init() failed.\n"));

			BYTE byWriteBuf[chunkSize];
			BYTE byReadBuf[chunkSize];
			BYTE byCheckBuf[chunkSize];

			for (int i = 0; i < 24; i++)
			{
				for (SIZE_T j = 0; j < 5; j++)
				{
					FillMemory (byWriteBuf, sizeof (byWriteBuf), (BYTE)(i + j + '0'));
					ret = ringBuffer.Write (byWriteBuf, sizeof (byWriteBuf));
					Assert::IsTrue (ret, _T("RingBuffer::Write() failed.\n"));
					Assert::AreEqual<SIZE_T> (chunkSize * (j + 1), ringBuffer.Remain (), _T("RingBuffer::Remain() invalid size.\n"));
				}

				for (SIZE_T j = 0; j < 5; j++)
				{
					ZeroMemory (byReadBuf, sizeof (byReadBuf));
					FillMemory (byCheckBuf, sizeof (byCheckBuf), (BYTE)(i + j + '0'));
					ret = ringBuffer.Read (byReadBuf, sizeof (byReadBuf));
					Assert::IsTrue (ret, _T("RingBuffer::Read() failed.\n"));
					Assert::AreEqual<SIZE_T> (chunkSize * (5 - (j + 1)), ringBuffer.Remain (), _T("RingBuffer::Remain() invalid size.\n"));

					int cmp = memcmp (byCheckBuf, byReadBuf, chunkSize);
					Assert::AreEqual (0, cmp, _T("RingBuffer::Read() not matched write data.\n"));
				}
			}
		}

		TEST_METHOD (RingBufferTest3)
		{
			const SIZE_T bufferSize = 2048;
			const SIZE_T chunkSize = 128;
			BOOL ret;

			alt::RingBuffer ringBuffer;

			ret = ringBuffer.Init (bufferSize);
			Assert::IsTrue (ret, _T("RingBuffer::Init() failed.\n"));

			BYTE byBuf[chunkSize];
			ZeroMemory (byBuf, chunkSize);

			for (SIZE_T i = 1; i <= 16; i++)
			{
				ret = ringBuffer.Write (byBuf, chunkSize), _T ("not over flow\n");
				Assert::IsTrue (ret, _T("RingBuffer::Write() failed.\n"));
				Assert::AreEqual<SIZE_T> (chunkSize * i, ringBuffer.Remain (), _T("RingBuffer::Remain() invalid size.\n"));
			}

			ret = ringBuffer.Write (byBuf, 1);
			Assert::IsFalse (ret, _T ("RingBuffer::Write() over flow\n"));

			for (SIZE_T i = 1; i <= 16; i++)
			{
				ret = ringBuffer.Read (byBuf, chunkSize);
				Assert::IsTrue (ret, _T("RingBuffer::Read() failed.\n"));
				Assert::AreEqual<SIZE_T> (chunkSize * (16 - i), ringBuffer.Remain (), _T("RingBuffer::Remain() invalid size.\n"));
			}

			Assert::IsFalse (ringBuffer.Read (byBuf, 1));
		}
	};
}