#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"
#include "write_tree.hpp"
#include "write_blob.hpp"
#include "../utils/utils.hpp"

using namespace std;

string write_tree(const filesystem::path &direcory)
{
  string tree_entries;

  for (const auto &entry : filesystem::directory_iterator(direcory))
  {

    // ignore .git folder
    string relative_path = filesystem::relative(entry.path(), direcory).string();
    if (relative_path.find("./git/") == 0 || relative_path == ".git")
    {
      continue;
    }

    string name = entry.path().filename();
    string mode;
    string objec_hash;

    if (filesystem::is_regular_file(entry))
    {
      mode = "100644";
      objec_hash = write_blob(entry.path());
    }
    else if (filesystem::is_directory(entry.path()))
    {
      mode = "040000";
      objec_hash = write_tree(entry.path());
    }
    else if ((filesystem::status(entry).permissions() & filesystem::perms::owner_exec) != filesystem::perms::none)
    {
      mode = "100755";
      objec_hash = write_blob(entry.path());
    }
    else
    {
      continue;
    }

    // in tree objects we store the hash in binary format and not as hexadecimal format.
    // construct tree entry: "<mode> <filename>\0<raw_hash>"
    string entry_data = mode + " " + name + '\0' + string(reinterpret_cast<const char *>(hex_to_raw(objec_hash).data()), 20);
    tree_entries += entry_data;
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
