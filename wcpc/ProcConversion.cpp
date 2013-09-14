// ProcConversion.cpp : ת���������

#include "stdafx.h"

#define SENTENCE_LEN 0x3200000 // ÿ����󳤶�

void cpc_internel(int scp, void* src, const int srcl,
                  int tcp, void* tag, int& tagl,
                  const char dft, bool& islost)
{
    tagl = srcl;
    islost = true;
}

inline void test_cp(const int cp)
{
    if(cp != NO_CODEPAGE && cp != 1200 && cp != 1201) {
        if(IsValidCodePage(cp) == FALSE) {
            PrintMessage.PrintIt(stderr, IDS_CODEPAGE_INVALID_INT, cp);
            throw EC_CODEPAGE_INVALID;
        }
    }
}

void ProcConversion()
{
    try {
        F64ReadLine ImportFile(Args.getImportFile()); // ���������ļ�, ǿ��ִ��
        if(!ImportFile.good()) {
            PrintMessage.PrintIt(stderr, IDS_FILE_ERROR_STR, Args.getImportFile());
            throw EC_FILE_ERROR;
        }
//        ImportFile.TestEnc(); // ���������ļ�����, ��ֻ��˵�� SetFilePointerEx ֮�����������������
        test_cp(Args.getSourceCodepage()); // ���Ա��������
        test_cp(Args.getTargetCodepage());

        F64PushLine ExportFile(Args.getExportFile()); // �������޶�, �ȴ���
        if(Args.getExportFile()) { // ȷʵ��������ļ��������
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

        void* sentence = new char[MEMPROTECT(SENTENCE_LEN * 3)]; // ÿ��ͨ�û���

        for(long long i = 1; ; ++i) {

            source_len = ImportFile.readline(sentence, SENTENCE_LEN);
            if(source_len == -1) { // �е��ַ��������޶�
                PrintMessage.PrintIt(stderr, IDS_LENGTH_ERROR_INT, i);
                PrintMessage.PrintIt(stderr, IDS_NAKED_STR, Args.getImportFile());
                throw EC_LENGTH_ERROR;
            }
            cpc_internel(Args.getSourceCodepage(), sentence, source_len, // �õ��е���BOM���ݺ�ת��
                Args.getTargetCodepage(), sentence, target_len,
                Args.getDefaultChar(), islost);

            if(Args.getAnalyze()) { // ���� -A ѡ��󱨸��ַ���ʧ���
                if(islost) {
                    PrintMessage.PrintIt(stdout, IDS_ANALYZE_SINGLE_INT, i);
                    ++lost_count;
                    islost = false;
                }
            }

            if(Args.getExportFile()) { // ���� -o ѡ���������ļ�
                ExportFile.pushline(sentence, target_len);
            }

            if(ImportFile.eof()) { // �����ļ��������˳�ת��ѭ��
                if(Args.getExportFile()) {
                    ExportFile.flush();
                }
                break;
            }
        }

        delete [] sentence; // �������
        ////////////////////////////////////////////////////////////////////////////////
        if(Args.getAnalyze()) { // ���� -A ʱ����������ս��
            PrintMessage.PrintIt(stdout, IDS_ANALYZE_RESULT_INT, lost_count);
        }

        if(Args.getForceExit() && lost_count != 0) { // ���� -E ��, ����δ��������ݲ�ɾ������ļ�
            if(Args.getExportFile()) {
                ExportFile.close(false);
                DeleteFileW(Args.getExportFile());
            }
            PrintMessage.PrintIt(stderr, IDS_LOSTCHAR_FORCE_EXIT_STR, Args.getImportFile());
            throw EC_LOSTCHAR_FORCE_EXIT;
        }

    } catch(std::bad_alloc&) { // �򵥲�׽�ڴ��������
        PrintMessage.PrintIt(stderr, IDS_MEMORY_ALLOC_ERROR_STR, Args.getImportFile());
        throw EC_MEMORY_ALLOC_ERROR;
    }
}

