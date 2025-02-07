#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "../externals/zstr.hpp"
#include "ls_tree.hpp"

using namespace std;

int handle_ls_tree(int argc, char *argv[])
{
  if (argc <= 3)
  {
    cerr << "Not enough arguments.";
    return EXIT_FAILURE;
  }

  const string flag = argv[2];
  const set<string> supported_flags = {"--name-only"};

  if (supported_flags.find(flag) == supported_flags.end())
  {
    cerr << "Invalid flag for ls-tree command, expected one of ";
    for (const auto &f : supported_flags)
      cerr << f << " ";
    cerr << "\n";
    return EXIT_FAILURE;
  }

  if (flag == "--name-only")
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

    string object_str{istreambuf_iterator<char>(input), istreambuf_iterator<char>()};
    input.close();

    // check if object is an actual tree
    if (object_str.substr(0, 4) != "tree")
    {
      cerr << "Invalid tree object.";
      return EXIT_FAILURE;
    }

    // find the null byter after the tree <size>
    size_t pos = object_str.find('\0');
    if (pos == string::npos)
    {
      cerr << "Malformed tree object";
      return EXIT_FAILURE;
    }

    // move past the tree <size>\0 header
    pos += 1;

    set<string> file_names;

    while (pos < object_str.size())
    {
      // start from pos to extract mode
      size_t mode_end = object_str.find(' ', pos);
      if (mode_end == string::npos)
      {
        break;
      }

      pos = mode_end + 1;

      // extracting file name (stop at null byte before the sha raw data)
      size_t name_end = object_str.find('\0', pos);
      if (name_end == string::npos)
      {
        break;
      }

      string file_name = object_str.substr(pos, name_end - pos);
      file_names.insert(file_name);

      pos = name_end + 20 + 1;
    }

    for (auto name : file_names)
    {
      cout << name << endl;
    }
  }

  return EXIT_SUCCESS;
}
