#include <iostream>
#include <filesystem>
#include <fstream>
#include <set>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"
#include "hash_object.hpp"
#include "../utils/write_blob.hpp"

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

    string digest = write_blob(file_path);

    cout << digest << endl;
  }

  return EXIT_SUCCESS;
}
