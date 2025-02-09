#include <filesystem>

using namespace std;
filesystem::path find_git_root(const filesystem::path &start_path);

vector<unsigned char> hex_to_raw(const string &hex);