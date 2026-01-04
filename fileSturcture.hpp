#include<iostream>
#include<string>
#include<vector>
#include "hashmap.hpp"
#include "tree.hpp"
using namespace std;

struct File {
    string name;
    versionTree vTree;
    hashMap<int, TreeNode*> version_map;
    int total_versions;
    time_t last_edited;
    File(int capacity , string fileName) : version_map(capacity), name(fileName) {
        total_versions = 1;
        last_edited = time(nullptr);
        version_map.insert(0, vTree.root);
    }
};