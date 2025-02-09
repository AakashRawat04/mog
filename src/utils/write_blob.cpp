#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <filesystem>
#include "../externals/sha1.hpp"
#include "../utils/compress.hpp"

using namespace std;

string write_blob(const string &file_path)
{
  ifstream input_file(file_path, ios::binary);
  if (!input_file)
  {
    cerr << "Failed to open the file: " << file_path << endl;
    exit(EXIT_FAILURE);
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
    cerr << "Compression failed!!" << endl;
    exit(EXIT_FAILURE);
  }

  ofstream output_file(full_path, ios::binary);
  output_file.write(compressed_data.data(), compressed_data.size());

  // returns the sha in hex format
  return digest;
}