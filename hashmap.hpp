#include<iostream>
#include<vector>
#include<utility>
using namespace std;
template<typename KT, typename VT>
class hashMap{
    vector<vector<pair<KT, VT>>> M;
    int cap, size;
// when int is the key
int computeHash(const int &key) const {
    return key % cap;
}

// when string is the key
int computeHash(const string &key) const {
    unsigned long hash = 5381UL;
    for (unsigned char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return (int)(hash % cap);
}

    public:
        hashMap(int capacity){
            cap = capacity;
            size = 0;
            M.resize(cap);
        }
        void insert(const KT &key,const VT &val){
            int index = computeHash(key);

            for(auto &x : M[index]){
                if(x.first==key){
                    x.second = val;
                    return;
                }
            }

            M[index].push_back(make_pair(key, val));
            size++;
        }
        bool erase(const KT &key){
            int index = computeHash(key);
            for(auto it = M[index].begin(); it!=M[index].end(); ++it){
                if(it->first==key){
                    M[index].erase(it);
                    size--;
                    return true;
                }
            }
            return false;
        }
        VT find(const KT &key){
            int index = computeHash(key);
            for(auto &x: M[index]){
                if(x.first==key){
                    return x.second;
                }
            }
            return nullptr;
        }
        int sizeofmap(){
            return size;
        }
};