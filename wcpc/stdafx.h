// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#ifndef _DEBUG
#include <afxwin.h> // Ϊ�˾�̬����Ű����� Orz
#endif
#include <Windows.h>

#define STATIC_GETOPT
#include "getopt.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "GlobalDef.h"
#include "resource.h"

#include "PrintMessage.h"
#include "ProcArgs.h"
#include "F64PushLine.h"
#include "F64ReadLine.h"

#include "Declare.h"
