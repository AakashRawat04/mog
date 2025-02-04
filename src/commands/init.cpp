#include <iostream>
#include <filesystem>
#include <fstream>
#include "../externals/strict_fstream.hpp"
#include "init.hpp"

using namespace std;

int handle_init()
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
    return EXIT_SUCCESS;
  }
  catch (const filesystem::filesystem_error &e)
  {
    cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
