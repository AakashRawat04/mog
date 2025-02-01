#include <iostream>
#include "sha1.hpp"
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
    else if (command == "hash-object")
    {

        if (argc <= 3)
        {
            cerr << "Invalid arguments, please provide valid arguments \n";
            return EXIT_FAILURE;
        }

        const string flag = argv[2];

        const set<string> supported_flags = {"-w"};

        if (supported_flags.find(flag) == supported_flags.end())
        {
            cerr << "Invalid flag for hash-object, expected one of ";
            for (const auto &f : supported_flags)
            {
                cerr << f << " ";
            }
            cerr << "\n";
            return EXIT_FAILURE;
        }

        if (flag == "-w")
        {

            // third argument value will be the name of the file to be compressed using zlib
            // and then this has to be added as a object file in the object directory

            // steps
            // check if the file actually exist ( if not exist then throw error )
            // extract the content of the file
            // generate sha1 digest of the content as blob <size?\0<actual_content>
            // then create a sub directory in the object folder with first 2 characters of the sha1 digest
            // the rest will be the name of the file inside the sub folder
            // store the same data ( blob <size?\0<actual_content> ) with zlib compression inside the file

            string file_path = argv[3];
            if (!filesystem::exists(file_path))
            {
                cerr << "file does not exist\n";
                return EXIT_FAILURE;
            }

            ifstream input_file(file_path);
            if (!input_file.is_open())
            {
                cerr << "Failed to load file.\n";
                return EXIT_FAILURE;
            }

            string file_content{istreambuf_iterator<char>(input_file), istreambuf_iterator<char>()};
            input_file.close();

            string final_content = "blob " + to_string(file_content.size()) + '\0' + file_content;

            // generate sha1 hash
            SHA1 checksum;
            checksum.update(final_content);
            string digest = checksum.final();

            string sub_dir_name = digest.substr(0, 2);
            string object_file_name = digest.substr(2);

            // create a sub directory
            filesystem::create_directory(".git/objects/" + sub_dir_name);

            string full_path = ".git/objects/" + sub_dir_name + "/" + object_file_name;

            // Compress final content
            zstr::ofstream output_file(full_path);
            if (!output_file.is_open())
            {
                cerr << "Failed to create object file: " << full_path << "\n";
                return EXIT_FAILURE;
            }

            output_file.write(final_content.c_str(), final_content.size());
            output_file.close();

            cout << digest << endl;
        }
    }
    else
    {
        cerr << "Invalid command.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
