//! @file	PicoJsonTest.cpp
//! @brief	PicoJsonのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.07

#include "pch.h"
#include "OSSLibrary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OSSLibraryTest
{
	TEST_CLASS (PicoJsonTest)
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

		void ShowValue (std::string key, std::string value)
		{
			std::string response = key + ":" + value + "\n";
			Logger::WriteMessage (response.c_str ());
		}

		TEST_METHOD (PicoJsonTest1)
		{
			std::ifstream ifs;
			ifs.open ("test.json"); // CAUTION:ビルドパッチで実行環境にコピーしています。
			if (ifs.fail ())
			{
				std::cerr << "fail to read test.json" << std::endl;
				Assert::Fail (_T ("test.jsonのオープンに失敗しました。"));
			}

			const std::string json (
				(std::istreambuf_iterator<char> (ifs)),
				std::istreambuf_iterator<char> ());
			ifs.close ();

			picojson::value val;
			const std::string error = picojson::parse (val, json);
			if (!error.empty ())
			{
				std::cerr << error << std::endl;
				Assert::Fail (_T ("picojson::parse()に失敗しました。"));
			}

			// @sa https://qiita.com/usagi/items/da3568d8fa61e4aafede
			/*
			   picojson::valueの中身の可能性
			   double           JSONの数値
			   std::string      JSONの文字列
			   picojson::array  JSONの配列
			   picojson::object JSONのオブジェクト(=辞書)
			   picojson::null   JSONのnull
			 */

			try
			{
				picojson::object Root = val.get<picojson::object> ();

				int Numeric = (int)Root["Numeric"].get<double> ();
				ShowValue ("Numeric", std::to_string (Numeric));

				auto String = Root["String"].get<std::string> ();
				ShowValue ("String", String);

				bool Boolean = Root["Boolean"].get<bool> ();
				ShowValue ("Boolean", std::to_string (Boolean));

				ShowValue ("Object", ">>");
				picojson::object Object = Root["Object"].get<picojson::object> ();

				double _Numeric = Object["numeric"].get<double> ();
				ShowValue (">numeric", std::to_string (_Numeric));

				auto _String = Object["string"].get<std::string> ();
				ShowValue (">string", _String);

				bool _Boolean = Object["boolean"].get<bool> ();
				ShowValue (">boolean", std::to_string (_Boolean));

				auto _Array1 = Object["Array1"].get<picojson::array> ();
				ShowValue (">Array1", ">>");
				for (auto item : _Array1)
				{
					ShowValue (">>Array1Value", std::to_string (item.get<double> ()));
				}

				auto _Array2 = Object["Array2"].get<picojson::array> ();
				ShowValue (">Array2", ">>");
				for (auto item : _Array2)
				{
					ShowValue (">>Array2Value", item.get<std::string> ());
				}

				auto Array1 = Root["Array1"].get<picojson::array> ();
				ShowValue ("Array1Value", ">>");
				for (auto item : Array1)
				{
					ShowValue (">Array1Value", std::to_string (item.get<double> ()));
				}

				auto Array2 = Root["Array2"].get<picojson::array> ();
				ShowValue ("Array2", ">>");
				for (auto item : Array2)
				{
					ShowValue (">Array2Value", item.get<std::string> ());

				}
			}
			catch (...)
			{
				Logger::WriteMessage ("catched anything.\n");
				Assert::Fail (_T ("JSON分析中に例外が発生しました。"));
			}
		}

		TEST_METHOD (FlowSettingsTest1)
		{
			std::ifstream ifs;
			ifs.open ("FlowSettings.json"); // CAUTION:ビルドパッチで実行環境にコピーしています。
			if (ifs.fail ())
			{
				std::cerr << "fail to read FlowSettings.json" << std::endl;
				Assert::Fail (_T ("FlowSettings.jsonのオープンに失敗しました。"));
			}

			const std::string json (
				(std::istreambuf_iterator<char> (ifs)),
				std::istreambuf_iterator<char> ());
			ifs.close ();

			picojson::value val;
			const std::string error = picojson::parse (val, json);
			if (!error.empty ())
			{
				std::cerr << error << std::endl;
				Assert::Fail (_T ("picojson::parse()に失敗しました。"));
			}

			try
			{
				picojson::object Root = val.get<picojson::object> ();

				for (const auto& item : Root)
				{
					ShowValue ("Key:", item.first);

					picojson::object Element = Root[item.first].get<picojson::object> ();
					auto id = Element["id"].get<std::string> ();
					ShowValue (" id", id);
					auto name = Element["name"].get<std::string> ();
					ShowValue (" name", name);
					auto object = Element["object"].get<std::string> ();
					ShowValue (" object", object);
					auto product = Element["product"].get<std::string> ();
					ShowValue (" product", product);

					if (Element["next"].is<picojson::array> ())
					{
						auto nextElements = Element["next"].get<picojson::array> ();
						for (const auto& next : nextElements)
						{
							ShowValue (" >next", next.get<std::string> ());
						}
					}
					else
					{
						auto next = Element["next"].get<std::string> ();
						ShowValue (" next", next);
					}

				}
			}
			catch (...)
			{
				std::cout << "catched anything." << std::endl;
				Assert::Fail (_T ("JSON分析中に例外が発生しました。"));
			}
		}
	};
}