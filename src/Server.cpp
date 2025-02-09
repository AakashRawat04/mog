#include <iostream>
#include <string>
#include "commands/cat_file.hpp"
#include "commands/hash_object.hpp"
#include "commands/init.hpp"
#include "commands/ls_tree.hpp"
#include "commands/write_tree.hpp"
#include "commands/commit_tree.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << unitbuf;
    cerr << unitbuf;

    if (argc < 2)
    {
        cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    string command = argv[1];

    if (command == "init")
    {
        return handle_init();
    }
    else if (command == "cat-file")
    {
        return handle_cat_file(argc, argv);
    }
    else if (command == "hash-object")
    {
        return handle_hash_object(argc, argv);
    }
    else if (command == "ls-tree")
    {
        return handle_ls_tree(argc, argv);
    }
    else if (command == "write-tree")
    {
        return handle_write_tree(argc, argv);
    }
    else if (command == "commit-tree")
    {
        return handle_commit_tree(argc, argv);
    }
    else
    {
        cerr << "Invalid command.\n";
        return EXIT_FAILURE;
    }
}
