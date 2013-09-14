// wcpc.cpp : �������̨Ӧ�ó������ڵ㡣
// ȫ���� wchar_t �汾��, �Ǹ� _setmode �������ʺ��� _TCHAR

#include "stdafx.h"

PrintMessage_t PrintMessage; // ת�����ڴ�ӡ��Ϣ

int wmain(int argc, wchar_t* argv[])
{
    SendMessageW( // ���ô���ͼ��, Just for entertainment
        GetConsoleWindow(), 
        WM_SETICON, 
        ICON_SMALL, 
        (LPARAM)LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON1))
        );

    try {
        ProcessArgs(argc, argv); // ���������в���

        ProcConversion(); // ִ��ת��

        return 0;
    } catch(const int& e) {
        return e; // ������Դ�ļ�����ͼ���
    } catch(std::bad_alloc&) { // �򵥲�׽�ڴ��������
        PrintMessage.PrintIt(stderr, IDS_MEMORY_ALLOC_ERROR_STR, Args.getImportFile());
        throw EC_MEMORY_ALLOC_ERROR;
    } catch(...) {
        return EC_UNKNOWN_ERROR;
    }
}

