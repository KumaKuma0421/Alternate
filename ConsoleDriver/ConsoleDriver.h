//! @file	ConsoleDriver.h
//! @brief	Consoleからシステムを起動します。
//! @author	kumakuma0421@gmail.com
//! @date	2020.01.03

#pragma once

#include "framework.h"

#define CONFIG_FILE	                _T(".\\Config.ini")

#define SECTION_GENERAL			    _T("General")
#define KEYWORD_ACTION			    _T("Action")

#define SECTION_PIPELINE_FACTORY    _T("PipelineFactory")

void Init ();

BOOL DrivePipelineFactoryHandler (DWORD dwType);
void DrivePipelineFactory ();