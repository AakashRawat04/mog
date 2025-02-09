#include <filesystem>

using namespace std;
filesystem::path find_git_root(const filesystem::path &start_path);

vector<unsigned char> hex_to_raw(const string &hex);

string create_commit_content(const string &tree_sha, const string &parent_sha, const string &commit_message);