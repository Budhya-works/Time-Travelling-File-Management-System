#include<iostream>
#include<vector>
#include<string>
#include<ctime>
using namespace std;

struct heapNode {
    string fileName;
    long long val;
};

bool compare(heapNode &a, heapNode &b){
    if(a.val != b.val) return a.val > b.val;
    return a.fileName < b.fileName;
}

class maxHeap{
    int size;
    vector<heapNode> H;
    void heapifyUp(int i){
        while(i > 0){
            int parent = (i - 1) / 2;
            if(compare(H[i], H[parent])){
                swap(H[i], H[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int i){
        int n = H.size();
        while(true){
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;

            if(left < n && compare(H[left], H[largest])){
                largest = left;
            }
            if(right < n && compare(H[right], H[largest])){
                largest = right;
            }
            if(largest != i){
                swap(H[i], H[largest]);
                i = largest;
            } else {
                break;
            }
        }
    }
    public:
        maxHeap(){
            size = 0;
        }
        void push(const string &fileName, long long val){
            H.push_back({fileName, val});
            heapifyUp(H.size() - 1);
            size++;
        }

        heapNode top(){
            return H[0];
        }

        void pop(){
            H[0] = H.back();
            H.pop_back();
            if(!H.empty()){
                heapifyDown(0);
            }
            size--;
        }

        bool isEmpty() {
            return H.empty();
        }
};