#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>
#include "zstr.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Flush after every cout / cerr
    cout << unitbuf;
    cerr << unitbuf;

    if (argc < 2)
    {
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    string command = argv[1];

    if (command == "init")
    {
        try
        {
            filesystem::create_directory(".git");
            filesystem::create_directory(".git/objects");
            filesystem::create_directory(".git/refs");

            ofstream headFile(".git/HEAD");
            if (headFile.is_open())
            {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            }
            else
            {
                cerr << "Failed to create .git/HEAD file.\n";
                return EXIT_FAILURE;
            }

            cout << "Initialized git directory\n";
        }
        catch (const filesystem::filesystem_error &e)
        {
            cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    }
    else if (command == "cat-file")
    {
        if (argc <= 3)
        {
            cerr << "Invalid arguments, required `-p <blob_sha>`\n";
            return EXIT_FAILURE;
        }

        const string flag = argv[2];

        // Define supported flags
        const set<string> supported_flags = {"-p"};

        // Check if the provided flag is supported
        if (supported_flags.find(flag) == supported_flags.end())
        {
            cerr << "Invalid flag for cat-file, expected one of ";
            for (const auto &f : supported_flags)
            {
                cerr << f << " ";
            }
            cerr << "\n";
            return EXIT_FAILURE;
        }

        if (flag == "-p")
        {
            const string value = argv[3];
            const string dir_name = value.substr(0, 2);
            const string blob_sha = value.substr(2);

            string path = ".git/objects/" + dir_name + "/" + blob_sha;

            zstr::ifstream input(path, ios::binary);
            if (!input.is_open())
            {
                cerr << "Failed to open object file.\n";
                return EXIT_FAILURE;
            }

            string object_str{istreambuf_iterator<char>(input),
                              istreambuf_iterator<char>()};
            input.close();

            const auto object_content = object_str.substr(object_str.find('\0') + 1);
            cout << object_content << flush;
        }
    }
    else
    {
        cerr << "Unknown command " << command << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
