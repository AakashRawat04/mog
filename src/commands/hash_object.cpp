#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"
#include "hash_object.hpp"

using namespace std;

int handle_hash_object(int argc, char *argv[])
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
      cerr << f << " ";
    cerr << "\n";
    return EXIT_FAILURE;
  }

  if (flag == "-w")
  {
    string file_path = argv[3];
    if (!filesystem::exists(file_path))
    {
      cerr << "File does not exist\n";
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

    SHA1 checksum;
    checksum.update(final_content);
    string digest = checksum.final();

    string sub_dir_name = digest.substr(0, 2);
    string object_file_name = digest.substr(2);

    filesystem::create_directory(".git/objects/" + sub_dir_name);
    string full_path = ".git/objects/" + sub_dir_name + "/" + object_file_name;

    string compressed_data;
    if (!compressData(final_content, compressed_data))
    {
      cerr << "Compression failed!\n";
      return EXIT_FAILURE;
    }

    ofstream output_file(full_path, ios::binary);
    output_file.write(compressed_data.data(), compressed_data.size());

    cout << digest << endl;
  }

  return EXIT_SUCCESS;
}
