#include <iostream>
#include <windows.h>
#include <strsafe.h>

using namespace std;

void failure_exit(const char* lpsz_function)
{
    // Retrieve the system error message for the last-error code

    LPVOID lp_msg_buf;
    LPVOID lp_display_buf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lp_msg_buf,
        0,
        NULL
    );

    // Display the error message

    lp_display_buf = (LPVOID)LocalAlloc(
        LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lp_msg_buf) + lstrlen((LPCTSTR)lpsz_function) + 40) * sizeof(TCHAR)
    );

    StringCchPrintf(
        (LPTSTR)lp_display_buf,
        LocalSize(lp_display_buf) / sizeof(TCHAR),
        TEXT("%s failed with error code %d: %s"),
        lpsz_function, dw, lp_msg_buf
    );

    MessageBox(NULL, (LPCTSTR)lp_display_buf, TEXT("Error"), MB_OK);

    // and exit the process

    LocalFree(lp_msg_buf);
    LocalFree(lp_display_buf);
    ExitProcess(dw);
}

void print_system_info()
{
    SYSTEM_INFO si_sys_info;

    // Copy the hardware information to the SYSTEM_INFO structure. 

    GetSystemInfo(&si_sys_info);

    // Display the contents of the SYSTEM_INFO structure. 

    cout << "Hardware information:" << endl
        << "\tOEM ID: " << si_sys_info.dwOemId << endl
        << "\tNumber of processors: " << si_sys_info.dwNumberOfProcessors << endl
        << "\tProcessor architecture: " << si_sys_info.wProcessorArchitecture << endl
        << "\t\t0 = x86" << endl
        << "\t\t5 = ARM" << endl
        << "\t\t6 = Intel Itanium-based" << endl
        << "\t\t9 = x64 (AMD or Intel)" << endl
        << "\t\t12 = ARM64" << endl
        << "\tProcessor type: " << si_sys_info.dwProcessorType << endl
        << "\tProcessor level: " << si_sys_info.wProcessorLevel << endl
        << "\tProcessor revision: " << si_sys_info.wProcessorRevision << endl
        << "\tPage size: " << si_sys_info.dwPageSize << endl
        << "\tActive processor mask: " << si_sys_info.dwActiveProcessorMask << endl
        << "\tMinimum application address: " << si_sys_info.lpMinimumApplicationAddress << endl
        << "\tMaximum application address: " << si_sys_info.lpMaximumApplicationAddress << endl;
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        while (*++argv)
        {
            if ((*argv)[0] == '-')
            {
                switch ((*argv)[1])
                {
                case 'e':
                    if (!GetProcessId(NULL))
                    {
                        failure_exit("GetProcessId");
                    }

                    break;
                case 's':
                    print_system_info();
                    break;
                default:
                    cout << "Uknown option: " << (*argv)[1] << endl;
                    break;
                }
            }
        }
    }
    else
    {
        cout << "At least one option is required." << endl;
    }

    return 0;
}