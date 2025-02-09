
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "../externals/zstr.hpp"
#include "commit_tree.hpp"
#include "filesystem"
#include "../utils/write_blob.hpp"
#include "../utils/write_tree.hpp"
#include "../utils/utils.hpp"
#include "../utils/write_commit.hpp"

using namespace std;

// mog commit-tree <tree_sha> (-p <parent_sha>)[optional] -m <message>
int handle_commit_tree(int argc, char *argv[])
{
  if (argc < 5)
  {
    cerr << "usage: mog commit-tree <tree_sha> (-p <parent_sha>)[optional] -m <message>" << endl;
    return EXIT_FAILURE;
  }

  string tree_sha = argv[2];
  string parent_sha = "";
  string commit_message = "";

  for (int i = 3; i < argc; i++)
  {
    string arg = argv[i];
    if (arg == "-p" && i + 1 < argc)
    {
      parent_sha = argv[++i];
    }
    else if (arg == "-m" && i + 1 < argc)
    {
      commit_message = argv[++i];
    }
  }

  string commit_content = create_commit_content(tree_sha, parent_sha, commit_message);
  string digest = write_commit_object(commit_content);

  cout << digest << endl;
  return EXIT_SUCCESS;
}