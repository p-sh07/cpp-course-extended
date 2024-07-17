//
//  05.Производительность и оптимизация
//  Тема 3.Работа с файлами
//  Задача 05.Работа с файловой системой
//
//  Created by Pavel Sh on 28.12.2023.
//
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

fs::path operator""_p(const char* data, std::size_t sz) {
    return fs::path(data, data + sz);
}

// напишите эту функцию
void PrintTree(ostream& dst, const fs::path& p);

int main() {
    error_code err;
    fs::remove_all("test_dir", err);
    fs::create_directories("test_dir"_p / "a"_p, err);
    fs::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(out, "test_dir"_p);
    cout << out.str() << endl;
    assert(out.str() ==
        "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"s
    );
}

struct Node {
    fs::path path_;
    vector<Node> subtree;
};

Node BuildDirTreeRecursive(const fs::path& p) {
    Node new_node{p};
    for(const auto& dir_entry : fs::directory_iterator(p)) {
        if(dir_entry.status().type() == fs::file_type::regular) {
            //this is a file, just add its name. Constructs a new node with empty subtree
            new_node.subtree.push_back({dir_entry.path()});
        } else if(dir_entrdy.status().type() == fs::file_type::directory) {
            new_node.subtree.push_back(BuildDirTreeRecursive(dir_entry.path()));
        }
    }
    return new_node;
}

void SortNodesInverseRecursive(Node& root) {
    //sort current subtree
    std::sort(root.subtree.begin(), root.subtree.end(), [](const Node& a, const Node& b){
        //inverse order
        return a.path_.filename().string() > b.path_.filename().string();
    });
    //if a node of the subtree is a directory, sort recursively
    for(auto& node : root.subtree) {
        if(fs::is_directory(node.path_)) {
            SortNodesInverseRecursive(node);
        }
    }
}

void PrintNodesRecursive(ostream& dst, const Node& root, int indent = 0) {
    dst << string(indent, ' ') + root.path_.filename().string() << '\n';
    indent+=2;
    for(const auto& node : root.subtree) {
        if(!node.subtree.empty()) {
            PrintNodesRecursive(dst, node, indent);
        } else {
            dst << string(indent, ' ') + node.path_.filename().string() << '\n';
        }
    }
}

void PrintTree(ostream& dst, const fs::path& p) {
    Node root = BuildDirTreeRecursive(p);
    SortNodesInverseRecursive(root);
    PrintNodesRecursive(dst, root);
}
