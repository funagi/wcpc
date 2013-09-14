// ProcConversion.cpp : ת���������

#include "stdafx.h"

#define SENTENCE_LEN 0x400000 // ÿ����󳤶� 4 MiB // �����汾������Զ��建������

inline void reverse_utf16(wchar_t* ws, const int wsl)
{
    for(int i = 0; i < wsl; ++i) {
        ws[i] = (ws[i] << 8) | (ws[i] >> 8);
    }
}

void cpc_internel(const int scp, const void* src, const int srcl,
                  const int tcp, void* tag, int& tagl,
                  const char dft, bool& islost)
{
    wchar_t* wstmp = new wchar_t[MEMPROTECT(SENTENCE_LEN * 2)]; // ���������������� UTF-16 ����

    int wslen = 0; // wstmp �� UTF-16 ���еĳ���, ���� wchar_t ����, �� WinAPI ����һ��

    switch(scp) { // ����ҳ
    case 1200: // UTF-16LE
        memcpy_s(wstmp, MEMPROTECT(SENTENCE_LEN * 2), src, srcl + 1);
        wslen = (srcl + 1) /2;
        break;
    case 1201: // UTF-16BE
        memcpy_s(wstmp, MEMPROTECT(SENTENCE_LEN * 2), src, srcl);
        wslen = srcl / 2;
        reverse_utf16(wstmp, wslen);
        break;
    default: // UTF-8 || ANSI
        wslen = MultiByteToWideChar(scp, 0, (LPCCH)src, srcl, NULL, 0);
        MultiByteToWideChar(scp, 0, (LPCCH)src, srcl, wstmp, wslen);
    }

    switch(tcp) {
    case 1200: // UTF-16LE
        memcpy_s(tag, MEMPROTECT(SENTENCE_LEN * 2), wstmp, tagl = wslen * 2);
        break;
    case 1201: // UTF-16BE
        reverse_utf16(wstmp, wslen);
        memcpy_s(tag, MEMPROTECT(SENTENCE_LEN * 2), wstmp, tagl = wslen * 2);
        break;
    default: // UTF-8 || ANSI
        int lost = 0;
        int* plost = (tcp == 65001) ? NULL : &lost; // UTF-8 �Ļ��� BUG ...
        const char* pdft = (tcp == 65001) ? NULL : &dft;
        tagl = WideCharToMultiByte(tcp, 0, wstmp, wslen, NULL, NULL, pdft, plost);
        WideCharToMultiByte(tcp, 0, wstmp, wslen, (LPSTR)tag, tagl, pdft, plost);
        if(lost == TRUE) {
            islost = true;
        } else {
            islost = false;
        }
    }

    delete [] wstmp; // ��ջ���
}

inline void test_cp(const int cp)
{
    if(cp != NO_CODEPAGE && cp != 1200 && cp != 1201) {
        if(IsValidCodePage(cp) == FALSE) {
            // û��װ���Ĵ���ҳ�ľ����Լ���취��ww
            // ��˵�˲�Ҫ�þ����ϵͳΪʲô���ǲ���233
            PrintMessage.PrintIt(stderr, IDS_CODEPAGE_INVALID_INT, cp);
            throw EC_CODEPAGE_INVALID;
        }
    }
}

void ProcConversion()
{
        F64ReadLine ImportFile;
        F64PushLine ExportFile;

        ImportFile.open(Args.getImportFile()); // ���������ļ�, ǿ��ִ��
        if(!ImportFile.good()) {
            PrintMessage.PrintIt(stderr, IDS_FILE_ERROR_STR, Args.getImportFile());
            throw EC_FILE_ERROR;
        }
        ImportFile.TestEnc(); // ���������ļ�����
        test_cp(Args.getSourceCodepage()); // ���Ա��������
        test_cp(Args.getTargetCodepage());

        if(Args.getExportFile()) { // ��������ļ������ // ��������ļ���Ӧ�������׳��쳣
            ExportFile.open(Args.getExportFile());
            if(!ExportFile.good()) {
                PrintMessage.PrintIt(stderr, IDS_FILE_ERROR_STR, Args.getExportFile());
                throw EC_FILE_ERROR;
            }
            if(Args.getTargetCodepage() == 65001 && Args.getAddBOM()) { // Ϊ����ļ����� BOM
                ExportFile.pushline((char*)"\xEF\xBB\xBF", 3);
            } else if(Args.getTargetCodepage() == 1200) {
                ExportFile.pushline((char*)"\xFF\xFE", 2);
            } else if(Args.getTargetCodepage() == 1201) {
                ExportFile.pushline((char*)"\xFE\xFF", 2);
            }
        }
        /////////////////////////////////////////////////////////////////////////////
        int source_len = 0; // ��ȡ����(�ֽ�
        int target_len = 0; // ת���󳤶�(�ֽ�
        bool islost = false; // ��ʧ�ַ����
        long long lost_count = 0; // ��ʧ�ַ�������

        void* sentence = new char[MEMPROTECT(SENTENCE_LEN * 2)]; // ÿ��ͨ�û���
        ///---------------------------------------------------------------------------------------///
        for(long long i = 1; ; ++i) { // ��ʼת��

            source_len = ImportFile.readline(sentence, SENTENCE_LEN); // Step 1: ��ȡһ�仰
            if(source_len == -1) { // �е��ַ��������޶�
                PrintMessage.PrintIt(stderr, IDS_LENGTH_ERROR_INT, i);
                PrintMessage.PrintIt(stderr, IDS_NAKED_STR, Args.getImportFile());
                throw EC_LENGTH_ERROR;
            }
            cpc_internel(Args.getSourceCodepage(), sentence, source_len, // Step 2: �õ��е���BOM���ݺ�ת��
                Args.getTargetCodepage(), sentence, target_len,
                Args.getDefaultChar(), islost);

            if(islost) { // Step 3: ������ʧ������������
                ++lost_count;
                if(Args.getAnalyze()) { // ���� -A ѡ��󱨸��ַ���ʧ���

                    PrintMessage.PrintIt(stdout, IDS_ANALYZE_SINGLE_INT, i);

                }
                islost = false;
            }

            if(Args.getExportFile()) { // Step 4: ��ת���ľ���������ļ�
                ExportFile.pushline(sentence, target_len);
            }

            if(ImportFile.eof()) { // Step 5: ת������󱣴��ļ�, �˳�ѭ��
                if(Args.getExportFile()) {
                    ExportFile.flush();
                }
                break;
            }
        }
        ///---------------------------------------------------------------------------------------///
        delete [] sentence; // �������
        ////////////////////////////////////////////////////////////////////////////////
        if(Args.getAnalyze() || Args.getWarning()) { // ���� -A �� -W ʱ���ͳ�ƽ��
            PrintMessage.PrintIt(stdout, IDS_ANALYZE_RESULT_INT, lost_count);
        }
        // �Զ�����
}

