// GlobalDef.h : ����һЩ�궨���ö��ʲô��

#pragma once

#define EC_INCORRECT_ARGUMENTS   0x01
#define EC_CODEPAGE_INVALID      0x02
#define EC_FILE_ERROR          (-0x01)
#define EC_MEMORY_ALLOC_ERROR  (-0x02)
#define EC_LOSTCHAR_FORCE_EXIT   0x03
#define EC_LENGTH_ERROR          0x04
#define EC_INTERNEL_ERROR        0x0F
#define EC_UNKNOWN_ERROR       (-0x03)

#define MEMPROTECT(x) (x + 1024) // �����ڴ�ר��
