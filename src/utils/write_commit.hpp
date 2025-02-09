#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

#include "../externals/sha1.hpp"
#include "./compress.hpp"

using namespace std;

// creates commit object file in .git/objects and returns commit sha
string write_commit_object(const string &commit_content)
{
  string header = "commit " + to_string(commit_content.size()) + '\0';
  string final_content = header + commit_content;

  SHA1 checksum;
  checksum.update(final_content);
  string digest = checksum.final();

  string sub_dir_name = digest.substr(0, 2);
  string commit_object_file_name = digest.substr(2);

  filesystem::create_directory(".git/objects/" + sub_dir_name);
  string commit_object_path = ".git/objects/" + sub_dir_name + "/" + commit_object_file_name;

  string compressed_data;
  if (!compressData(final_content, compressed_data))
  {
    cerr << "Compression Failed" << endl;
    exit(EXIT_FAILURE);
  }

  ofstream output_file(commit_object_path, ios::binary);
  output_file.write(compressed_data.data(), compressed_data.size());

  return digest;
}