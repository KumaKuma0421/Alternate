#pragma once

#include "DesignPattern.hpp"
#include "TString.h"

namespace alt
{
	namespace FactoryMethod
	{
		class DLL_DECLSPEC IData
		{
		public:
			IData (int seqNo);
			virtual ~IData ();

			int GetSeqNo () { return _seqNo; };
			SYSTEMTIME& GetCreateTime () { return _createDateTime; };
			void SetCreateTime (SYSTEMTIME& systemTime) { _createDateTime = systemTime; };

		protected:
			IData ();

		private:
			int _seqNo;
			SYSTEMTIME _createDateTime;
		};

		class DLL_DECLSPEC IMessage
		{
		public:
			IMessage ();
			virtual ~IMessage ();

			int GetTo () { return _to; };
			int GetFrom () { return _from; };
			int GetCommand () { return _command; };
			void SetParam (int to, int from, int command);

		private:
			int _to;
			int _from;
			int _command;
		};

		class DLL_DECLSPEC IConfig
		{
		public:
			IConfig ();
			virtual ~IConfig ();

			virtual bool Load (LPCTSTR lpctszConfigPath) = 0;
			virtual bool Reload () = 0;
		};

		class IProduct : public skeleton::ISubscriber<IMessage*>, public skeleton::IObserver<IMessage*>
		{
		public:
			IProduct (int id, TString productName);
			virtual ~IProduct ();

			virtual bool Init (IConfig* config) = 0;
			virtual bool Start () = 0;
			virtual IData* Process (IData* data) = 0;
			virtual bool Stop () = 0;
			virtual bool Exit () = 0;

			virtual void Update (IMessage* message) = 0;

			virtual int GetID () { return _id; };
			virtual TString GetName () { return _productName; };

		protected:
			IProduct ();

			int _id;
			TString _productName;
		};

		class IElement : public skeleton::ISubscriber<IData*>, public skeleton::IObserver<IData*>
		{
		public:
			IElement (TString elementName);
			virtual ~IElement ();

			virtual void SetProduct (IProduct* product) { _product = product; };
			virtual void Setup ();
			virtual TString& GetName () { return _elementName; };
			virtual void Update (IData* data) = 0;

		protected:
			IElement ();

			TString _elementName;
			IProduct* _product = nullptr;
		};

		class ICreator
		{
		public:
			ICreator ();
			virtual ~ICreator ();

			virtual IProduct* CreateProduct (int id, TString target, TString prefix) = 0;
			virtual IElement* CreateElement (TString target, TString fullName) = 0;
		};

		class IProductManager : public skeleton::ISubscriber<IMessage*>, public skeleton::IObserver<IMessage*>
		{
		public:
			IProductManager ();
			virtual ~IProductManager ();

			virtual bool Init (IConfig* config) = 0;
			virtual bool Start () = 0;
			virtual bool Stop () = 0;
			virtual bool Exit () = 0;

			virtual void Update (IMessage* message) = 0;

		};

		class DLL_DECLSPEC IFactoryMethod
		{
		public:
			IFactoryMethod ();
			virtual ~IFactoryMethod ();

			virtual IProductManager* Buildup () = 0;
		};
	}
}