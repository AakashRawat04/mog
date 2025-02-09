#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"
#include "utils.hpp"

using namespace std;

filesystem::path find_git_root(const filesystem::path &start_path)
{
  filesystem::path current = start_path;
  while (!filesystem::exists(current / ".git"))
  {
    if (current == current.root_path())
    {
      throw runtime_error("Not inside a git repository");
    }
    current = current.parent_path();
  }
  return current;
}

vector<unsigned char> hex_to_raw(const string &hex)
{
  vector<unsigned char> raw;
  for (size_t i = 0; i < hex.length(); i += 2)
  {
    string byteString = hex.substr(i, 2);
    raw.push_back((unsigned char)strtol(byteString.c_str(), nullptr, 16));
  }
  return raw;
}

string get_current_timestamp()
{
  time_t now = time(nullptr);
  struct tm *gmt = gmtime(&now);

  char buffer[32];
  strftime(buffer, sizeof(buffer), "%s %z", gmt);
  return string(buffer);
}

string create_commit_content(const string &tree_sha, const string &parent_sha, const string &commit_message)
{
  string author = "Aakash Rawat <2004rawataakash@gmail.com>";
  string time_stamp = get_current_timestamp();

  stringstream commit;
  commit << "tree " << tree_sha << "\n";
  if (!parent_sha.empty())
  {
    commit << "parent " << parent_sha << "\n";
  }

  commit << "author " << author << " " << time_stamp << "\n";
  commit << "committer " << author << " " << time_stamp << "\n";
  commit << "\n"
         << commit_message << "\n";

  cout << "commit message created: " << commit.str() << endl;

  return commit.str();
}
