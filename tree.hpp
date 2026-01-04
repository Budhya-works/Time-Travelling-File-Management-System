#include<iostream>
#include<utility>
#include<vector>
#include<ctime>
using namespace std;

struct TreeNode{
    int version_id;
    string content;
    string message; // Empty if not a snapshot
    time_t created_timestamp;
    time_t snapshot_timestamp; // Null if not a snapshot
    TreeNode* parent;
    vector <TreeNode*> children;

    TreeNode(int id, string con = "", string msg = "", TreeNode* pnt = nullptr)
    : version_id(id),
      content(con),
      message(msg),
      created_timestamp(time(nullptr)),
      snapshot_timestamp(-1),           // default time: not a snapshot
      parent(pnt) {}
};

struct versionTree{
    public:
        TreeNode* root;
        TreeNode* currentVersion;

        versionTree(){
            root = new TreeNode(0, "", "INITIAL SNAPSHOT", nullptr);
            root->snapshot_timestamp=time(nullptr);
            currentVersion = root;
        }

        void createChild(TreeNode* parent, int id, string con = ""){
            parent->children.push_back(new TreeNode(id, con, "", parent));
        }
        vector<TreeNode*> retrieveSnapshots(TreeNode* Node){
            vector<TreeNode*> snapshots;
            while(Node != nullptr){
                if(Node->snapshot_timestamp != -1){
                    snapshots.push_back(Node);
                }
                Node = Node->parent;
            }
            return vector<TreeNode*> (snapshots.rbegin(), snapshots.rend());
        }
};