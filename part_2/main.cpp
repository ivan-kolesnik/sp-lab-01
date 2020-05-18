#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

string unicode_to_ansi(const std::wstring &wstr)
{
    int req_size = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    string buf(req_size, 0);

    WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &buf[0], req_size, NULL, NULL);

    return buf;
}

wstring ansi_to_unicode(const std::string &str)
{
    int req_size = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
    wstring w_buf(req_size, 0);

    MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &w_buf[0], req_size);

    return w_buf;
}

void print_help()
{
    cout << "Usage: part_2.exe <path_to_in_file> <path_to_out_file> <-a|-u>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc == 3 || argc == 4)
    {
        string in_filepath;
        string out_filepath;
        char operation = 'a';

        while (*++argv)
        {
            if ((*argv)[0] == '-')
            {
                operation = (*argv)[1];
            }
            else if (!in_filepath.empty())
            {
                out_filepath = *argv;
            }
            else
            {
                in_filepath = *argv;
            }
        }

        if (!in_filepath.empty() && !out_filepath.empty())
        {
            locale loc(locale(), new codecvt_utf16<wchar_t>);

            switch (operation)
            {
                case 'a':
                {
                    ifstream in_file(in_filepath);
                    basic_ofstream<wchar_t> out_file(out_filepath);

                    out_file.imbue(loc);
                    out_file << (wchar_t)0xfeff;

                    string in_line;
                    while (getline(in_file, in_line))
                    {
                        out_file << ansi_to_unicode(in_line);
                    }

                    cout << "Done! (ansi --> unicode)" << endl;

                    break;
                }
                case 'u':
                {
                    wifstream in_file(in_filepath);
                    ofstream out_file(out_filepath);

                    in_file.imbue(loc);
                    in_file.seekg(2);

                    wstring in_line;
                    while (getline(in_file, in_line))
                    {
                        out_file << unicode_to_ansi(in_line);
                    }

                    cout << "Done! (unicode --> ansi)" << endl;

                    break;
                }
                default:
                {
                    cout << "Invalid operation specified!" << endl;
                    print_help();

                    break;
                }
            }
        }
        else
        {
            cout << "Invalid arguments specified!" << endl;
            print_help();
        }
    }
    else
    {
        cout << "Invalid number of argumets." << endl;
        print_help();
    }

    std::cout << "Press ANY enter button to exit...";
    cin.get();

    return 0;
}