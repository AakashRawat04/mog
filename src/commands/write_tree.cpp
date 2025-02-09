#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "../externals/zstr.hpp"
#include "write_tree.hpp"
#include "filesystem"
#include "../utils/write_blob.hpp"
#include "../utils/write_tree.hpp"
#include "../utils/utils.hpp"

using namespace std;
using recursive_directory_iterator = filesystem::recursive_directory_iterator;

int handle_write_tree(int argc, char *argv[])
{
  if (argc < 2)
  {
    cerr << "usage: mog write-tree" << endl;
    return EXIT_FAILURE;
  }

  try
  {
    filesystem::path repo_root = find_git_root(filesystem::current_path());
    string tree_hash = write_tree(repo_root);
    cout << tree_hash << endl;
  }
  catch (const exception &e)
  {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}