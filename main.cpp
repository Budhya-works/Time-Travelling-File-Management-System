#include <iostream>
#include <string>
#include <vector>
#include "fileSturcture.hpp"
#include "heap.hpp"
using namespace std;

hashMap<string, File *> files(10007);//A big prime number to reduce collisions
vector<File *> allFiles;

void funCreateFile(const string &fileName)
{
    if (files.find(fileName) != nullptr)
    {
        cout << "error: File already exists\n\n";
        return;
    }
    File *newFile = new File(10007, fileName);
    files.insert(fileName, newFile);
    newFile->last_edited = time(nullptr);
    allFiles.push_back(newFile);
    cout << "File with name '" << fileName << "' created successfully\n\n";
}

void funRead(const string &fileName)
{
    File *file = files.find(fileName);
    if (file == nullptr)
    {
        cout << "error: File not found\n\n";
        return;
    }
    cout << file->vTree.currentVersion->content << "\n\n";
}

void funInput(const string &fileName, const string &newContent, bool isUpdate)
{
    File *file = files.find(fileName);
    if (file == nullptr)
    {
        cout << "error: File not found\n\n";
        return;
    }
    if (newContent.empty())
    {
        cout << "error: Content cannot be empty\n\n";
        return;
    }
    TreeNode *current = file->vTree.currentVersion;

    if (current->snapshot_timestamp != -1)
    {
        string finalcontent = isUpdate ? newContent : (current->content + newContent);
        file->vTree.createChild(current, file->total_versions, finalcontent);
        file->vTree.currentVersion = current->children.back();
        file->version_map.insert(file->total_versions, file->vTree.currentVersion);
        file->total_versions++;
        cout << "Created new version: " << file->vTree.currentVersion->version_id << " of " << fileName << "\n\n";
    }
    else
    {
        // As current version is not a snapshot, update its content
        if (isUpdate)
        {
            current->content = newContent;
            cout << "Content updated in current version of " << fileName << "\n\n";
        }
        else
        {
            current->content += newContent;
            cout << "Content appended in current version of " << fileName << "\n\n";
        }
    }
    file->last_edited = time(nullptr);
}

void funSnapshot(const string &fileName, const string &message)
{
    File *file = files.find(fileName);
    if (file == nullptr)
    {
        cout << "error: File not found\n\n";
        return;
    }
    TreeNode *current = file->vTree.currentVersion;
    if (current->snapshot_timestamp != -1)
    {
        cout << "error: Current version is already a snapshot\n\n";
        return;
    }
    current->snapshot_timestamp = time(nullptr);
    current->message = message;
    cout << "Snapshot created for version: " << current->version_id << " of " << fileName << "\n\n";
}

void funRollback(const string &fileName, int versionID)
{
    File *file = files.find(fileName);
    if (file == nullptr)
    {
        cout << "error: File not found\n\n";
        return;
    }
    TreeNode *targetVersion;
    if (versionID == -1)
    {
        TreeNode *par = file->vTree.currentVersion->parent;
        if (par == nullptr)
        {
            cout << "error: The current version has no parent to Rollback to\n\n";
            return;
        }
        cout << "Rolled back to the parent snapshot \n\n";
        return;
    }
    else
    {
        targetVersion = file->version_map.find(versionID);
        if (targetVersion == nullptr || targetVersion->snapshot_timestamp == -1)
        {
            cout << "error: Invalid version ID\n\n";
            return;
        }
    }
    file->vTree.currentVersion = targetVersion;
    cout << "Rolled back to version: " << targetVersion->version_id << " of " << fileName << "\n\n";
}

void funHistory(const string &fileName)
{
    File *file = files.find(fileName);
    if (file == nullptr)
    {
        cout << "error: File not found\n\n";
        return;
    }
    vector<TreeNode *> path = file->vTree.retrieveSnapshots(file->vTree.currentVersion);
    if (path.empty())
    {
        cout << "No snapshots available in " << fileName << "prior to current version\n\n";
        return;
    }
    cout << "Snapshot History of this file:\n\n";
    for (const auto &node : path)
    {
        cout << "Version ID: " << node->version_id << ",\nMessage: " << node->message << ",\nSnapshotted At: " << ctime(&node->snapshot_timestamp);
    }
    cout << "\n";
}

void funRecent(int n)
{
    int totalFiles = files.sizeofmap();
    if (totalFiles < n)
    {
        cout << "error: Only " << totalFiles << " files exists\n\n";
        return;
    }
    cout << "Most recently edited " << n << " files:\n";
    maxHeap recent;
    for (int i = 0; i < totalFiles; i++)
    {
        File *file = allFiles[i];

        recent.push(file->name, file->last_edited);
    }
    for (int i = 0; i < n; i++)
    {
        heapNode top = recent.top();
        time_t t = (time_t)(top.val);
        cout << top.fileName << " (Last Edited: " << ctime(&t) << ")\n";
        recent.pop();
    }
    cout << "\n";
}

void funBiggest(int n)
{
    int totalFiles = files.sizeofmap();
    if (totalFiles < n)
    {
        cout << "error: Only " << totalFiles << " files exists\n\n";
        return;
    }
    cout << "Top " << n << " files with most number of versions:\n";
    maxHeap biggest;
    for (int i = 0; i < totalFiles; i++)
    {
        File *file = allFiles[i];
        biggest.push(file->name, file->total_versions);
    }
    for (int i = 0; i < n; i++)
    {
        heapNode top = biggest.top();
        cout << top.fileName << " (Total Versions: " << top.val << ")\n";
        biggest.pop();
    }
    cout << "\n";
}

int main()
{
    string command;
    while (true)
    {
        cin >> command;
        if (command == "CREATE")
        {
            string fileName;
            cin >> fileName;
            funCreateFile(fileName);
        }
        else if (command == "READ")
        {
            string fileName;
            cin >> fileName;
            funRead(fileName);
        }
        else if (command == "INSERT")
        {
            string fileName, newContent;
            cin >> fileName;
            cin.ignore(); // To ignore the newline character after fileName
            getline(cin, newContent);
            if (newContent.empty())
            {
                cout << "error: Invalid command, usage: INSERT <filename> <content>\n\n";
                continue;
            }
            funInput(fileName, newContent, false);
        }
        else if (command == "UPDATE")
        {
            string fileName, newContent;
            cin >> fileName;
            cin.ignore();
            getline(cin, newContent);
            if (newContent.empty())
            {
                cout << "error: Invalid command, usage: UPDATE <filename> <content>\n\n";
                continue;
            }
            funInput(fileName, newContent, true);
        }
        else if (command == "SNAPSHOT")
        {
            string fileName, message;
            cin >> fileName;
            cin.ignore();
            getline(cin, message);
            funSnapshot(fileName, message);
        }
        else if (command == "ROLLBACK")
        {
            string line;
            getline(cin, line);
            // To remove leading and trailing spaces
            while (line.front() == ' ')
                line.erase(line.begin());
            while (line.back() == ' ')
                line.pop_back();
            string fileName;
            int versionID = -1;
            // Find the first space separating fileName and versionID
            long long pos = -1;
            for (long long i = 0; i < line.size(); i++)
            {
                if (line[i] == ' ')
                {
                    pos = i;
                    break;
                }
            }
            if (pos == -1)
            {
                fileName = line; // No versionID provided
            }
            else
            {
                fileName = line.substr(0, pos);
                for (long long i = pos + 1; i < line.size(); i++)
                {
                    if (line[i] != ' ')
                    {
                        if (line.substr(i).empty())
                            versionID = -1;
                        else
                            versionID = stoi(line.substr(i));
                        break;
                    }
                }
            }

            funRollback(fileName, versionID);
        }

        else if (command == "HISTORY")
        {
            string fileName;
            cin >> fileName;
            funHistory(fileName);
        }
        else if (command == "RECENT_FILES")
        {
            int num;
            cin >> num;
            if (num <= 0)
            {
                cout << "error: Invalid command, usage: RECENT_FILES <positive integer>\n\n";
                continue;
            }
            funRecent(num);
        }
        else if (command == "BIGGEST_TREES")
        {
            int num;
            cin >> num;
            if (num <= 0)
            {
                cout << "error: Invalid command, usage: BIGGEST_TREES <positive integer>\n\n";
                continue;
            }
            funBiggest(num);
        }
        else
        {
            cout << "error: Invalid command\n\n"; // For any unrecognized command
        }
    }
}