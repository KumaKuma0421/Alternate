//! @file	framework.h
//! @brief	���̃v���W�F�N�g�̃|�[�e�B���O�Ɋւ�����
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.07

#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����قƂ�ǎg�p����Ă��Ȃ����������O����

#ifdef OSSLIBRARY_EXPORTS
#pragma message("DLL EXPORTING...")
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllimport)
#endif
#pragma comment(lib, "OSSLibrary")
#pragma message("OSSLibrary is now importing...")
#endif

#include <windows.h>
#include <tchar.h>