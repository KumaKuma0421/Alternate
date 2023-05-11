#include "pch.h"
#include "IFactoryMethod.h"

using namespace alt;
using namespace alt::FactoryMethod;

IData::IData (int seqNo)
{
	_seqNo = seqNo;
	ZeroMemory (&_createDateTime, sizeof (_createDateTime));
}

IData::IData ()
{
	_seqNo = -1;
	ZeroMemory (&_createDateTime, sizeof (_createDateTime));
}

IData::~IData ()
{

}

IMessage::IMessage ()
{
	_to = -1;
	_from = -1;
	_command = -1;
}

IMessage::~IMessage ()
{

}

void IMessage::SetParam (int to, int from, int command)
{
	_to = to;
	_from = from;
	_command = command;
}

IConfig::IConfig ()
{

}

IConfig::~IConfig ()
{

}

IProduct::IProduct ()
{
	_id = -1;
	_productName = nullptr;
}

IProduct::IProduct (int id, TString productName)
{
	_id = id;
	_productName = productName;
}

IProduct::~IProduct ()
{

}

IElement::IElement ()
{
	_product = nullptr;
}

IElement::IElement (TString elementName)
{
	_elementName = elementName;
}

IElement::~IElement ()
{

}

void IElement::Setup ()
{

}

ICreator::ICreator ()
{

}

ICreator::~ICreator ()
{

}

IProductManager::IProductManager ()
{

}

IProductManager::~IProductManager ()
{

}

IFactoryMethod::IFactoryMethod ()
{

}

IFactoryMethod::~IFactoryMethod ()
{

}