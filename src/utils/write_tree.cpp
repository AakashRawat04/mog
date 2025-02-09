#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <filesystem>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"
#include "write_tree.hpp"
#include "write_blob.hpp"
#include "../utils/utils.hpp"

using namespace std;

string write_tree(const filesystem::path &directory)
{
  // Use map to automatically sort entries by name
  map<string, pair<string, string>> tree_entries_map;

  for (const auto &entry : filesystem::directory_iterator(directory))
  {
    string relative_path = filesystem::relative(entry.path(), directory).string();
    if (relative_path.find(".git") == 0)
    {
      continue;
    }

    string name = entry.path().filename().string();
    string mode;
    string object_hash;

    if (filesystem::is_regular_file(entry))
    {
      if ((filesystem::status(entry).permissions() & filesystem::perms::owner_exec) != filesystem::perms::none)
      {
        mode = "100755";
      }
      else
      {
        mode = "100644";
      }
      object_hash = write_blob(entry.path());
    }
    else if (filesystem::is_directory(entry))
    {
      mode = "40000"; // Note: removed leading 0
      object_hash = write_tree(entry.path());
    }
    else
    {
      continue;
    }

    if (!object_hash.empty())
    {
      tree_entries_map[name] = make_pair(mode, object_hash);
    }
  }

  string tree_entries;
  for (const auto &entry : tree_entries_map)
  {
    string name = entry.first;
    string mode = entry.second.first;
    string hash = entry.second.second;

    tree_entries += mode + " " + name + '\0' + string(reinterpret_cast<const char *>(hex_to_raw(hash).data()), 20);
  }

  if (tree_entries.empty())
  {
    return "";
  }

  string final_content = "tree " + to_string(tree_entries.size()) + '\0' + tree_entries;

  SHA1 checksum;
  checksum.update(final_content);
  string digest = checksum.final();

  string sub_dir_name = digest.substr(0, 2);
  string object_file_name = digest.substr(2);

  filesystem::create_directory(".git/objects/" + sub_dir_name);
  string tree_object_path = ".git/objects/" + sub_dir_name + "/" + object_file_name;

  string compressed_data;
  if (!compressData(final_content, compressed_data))
  {
    cerr << "Compression Failed" << endl;
    exit(EXIT_FAILURE);
  }

  ofstream output_file(tree_object_path, ios::binary);
  output_file.write(compressed_data.data(), compressed_data.size());

  return digest;
}
