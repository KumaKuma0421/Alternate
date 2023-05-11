// ----------------------------------------------------------------------------
//! @file	PerformanceDataHelper.cpp
//! @brief	�p�t�H�[�}���X���j�^�[�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.15

#include "pch.h"
#include "PerformanceDataHelper.h"

using namespace alt;

// PerformanceDataItem ------------------------------------------------

PerformanceDataItem::PerformanceDataItem (HQUERY hQuery, TString key)
{
	_hQuery = hQuery;
	_counterPath = key;
	_hCounter = INVALID_HANDLE_VALUE;
}

PerformanceDataItem::~PerformanceDataItem ()
{

}

bool PerformanceDataItem::AddCounter ()
{
	PDH_STATUS stat = ::PdhAddCounter (
		_hQuery, _counterPath.Ctr (), 0, &_hCounter);
	return ERROR_SUCCESS == stat ? true : false;
}

bool PerformanceDataItem::GetValue (
	DWORD dwFormat, PDH_FMT_COUNTERVALUE& value)
{
	PDH_STATUS stat = ::PdhGetFormattedCounterValue (
		_hCounter, dwFormat, NULL, &value);
	return ERROR_SUCCESS == stat ? true : false;
}

TString& PerformanceDataItem::GetKey ()
{
	return _counterPath;
}

/* The query keys are...
	  Processor
	   _T("\\Processor(_Total)\\% Processor Time")          % Processor Time �́A�v���Z�b�T���A�C�h���ȊO�̃X���b�h�����s���邽�߂Ɏg�p�����o�ߎ��Ԃ̊������p�[�Z���g�ŕ\�����܂��B
	   _T("\\Processor(_Total)\\% User Time")               ���[�U�[ ���[�h�Ōo�߂�����A�C�h���̃v���Z�b�T���Ԃ̊������p�[�Z���g�ŕ\�����܂��B
	   _T("\\Processor(_Total)\\% Privileged Time")         �v���Z�X�̃X���b�h���������[�h�ŃR�[�h���s�ɔ�₵���o�ߎ��Ԃ̊������p�[�Z���g�ŕ\�����܂��B
	   _T("\\Processor(_Total)\\Interrupts/sec")            �n�[�h�E�F�A�̊��荞�݂��A�v���Z�b�T����M�A�����������ϗ��𖈕b�̉񐔂ŕ\�������̂ł��B
	  Memory
		_T("\\Memory\\Available MBytes")                    �v���Z�X�ւ̊��蓖�Ă܂��̓V�X�e���̎g�p�ɂ����ɗ��p�\�ȕ����������̃T�C�Y�����K�o�C�g���ŕ\�����܂��B
		_T("\\Memory\\Pages/sec")                           �n�[�h �y�[�W �t�H�[���g���������邽�߂Ƀf�B�X�N����ǂݎ��ꂽ�A�܂��̓f�B�X�N�֏������܂ꂽ�y�[�W�̐��ł��B
		_T("\\Memory\\Committed Bytes")                      �R�~�b�g���ꂽ���z�������̃T�C�Y���o�C�g���ŕ\�����܂��B
		_T("\\Memory\\% Committed Bytes In Use")             Memory\\Committed Bytes �� Memory\\Commit Limit �ɑ΂��銄���ł��B
	  Paging File
		_T("\\Paging File(_Total)\\% Usage")                �g�p���̃y�[�W �t�@�C�� �C���X�^���X�̍��v���p�[�Z���g�ŕ\�����܂��B
	  PhysicalDisk
		_T("\\PhysicalDisk(_Total)\\% Disk Time")           �I�������f�B�X�N �h���C�u���ǂݎ��܂��͏������ݗv�����������Ă��ăr�W�[��Ԃɂ������o�ߎ��Ԃ̊������p�[�Z���g�ŕ\�����܂��B
		_T("\\PhysicalDisk(_Total)\\Current Disk Queue Length")  �p�t�H�[�}���X �f�[�^�̎��W���Ƀf�B�X�N�Ɏc���Ă���v���̐��ł��B
		_T("\\PhysicalDisk(_Total)\\Disk Writes/sec")            �f�B�X�N��̏���������̑��x�ł��B
	  System
		_T("\\System\\Processor Queue Length")              �v���Z�b�T �L���[�ɂ���X���b�h�̐��ł��B
		_T("\\System\\Context Switches/sec")                �C�ӂ̃X���b�h����ق��̃X���b�h�ɐ؂�ւ�邷�ׂẴv���Z�b�T���܂߂����ł��B
	  Network
		_T("\\Network Interface(*)\\Bytes Total/sec")       �e�l�b�g���[�N �A�_�v�^�[��ő���M�����o�C�g�̗��ł��B

 */

// PerformanceDataHelper ----------------------------------------------

PerformanceDataHelper::PerformanceDataHelper ()
{
	_hQuery = INVALID_HANDLE_VALUE;
}

PerformanceDataHelper::~PerformanceDataHelper ()
{
	if (_hQuery != INVALID_HANDLE_VALUE) Close ();
	_hQuery = INVALID_HANDLE_VALUE;
}

bool PerformanceDataHelper::Open ()
{
	PDH_STATUS stat = ::PdhOpenQuery (NULL, 0, &_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}

bool PerformanceDataHelper::Close ()
{
	PDH_STATUS stat = ::PdhCloseQuery (_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}

PerformanceDataItem& PerformanceDataHelper::CreateItem (TString key)
{
	PerformanceDataItem* item = new PerformanceDataItem (_hQuery, key);
	item->AddCounter ();
	return *(item);
}

bool PerformanceDataHelper::Query ()
{
	PDH_STATUS stat = ::PdhCollectQueryData (_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}