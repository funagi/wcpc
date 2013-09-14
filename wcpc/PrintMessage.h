// PrintMessage.h : ��Դ�ļ��Ĵ�ӡװ��

#pragma once
class PrintMessage_t
{
public:

    PrintMessage_t(void)
    {
        _setmode(_fileno(stdin),  _O_U16TEXT); // ����׼ I/O �ĳ� Unicode ģʽ
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
    }

    ~PrintMessage_t(void)
    {
    }

    template<typename PrEx>
    // ���� wchar_t* �� int
    // �Ժ���ܻ�ĳɲ����������ʽ
    void PrintIt(FILE* fp, int messageID, const PrEx extra)
    {
        const int bufferLen = 512;
        wchar_t msgBuffer[bufferLen];
        LoadStringW(GetModuleHandleW(NULL), messageID, msgBuffer, bufferLen);
        fwprintf_s(fp, msgBuffer, extra);
    }
};

