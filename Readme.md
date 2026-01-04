
# Time-Travelling File System

## **Overview**
The **Time-Travelling File System** is a simplified, **in-memory version control system** implemented entirely in **C++**, inspired by tools like Git. It is designed to explore advanced data structures and algorithmic concepts by building a fully functional mini version control system without relying too much on external libraries.

### **Key Features**
- **File Versioning:**
  - Create files and track their versions over time.
  - Append or update file contents with branching version history.
- **Snapshots:**
  - Mark important versions with descriptive messages.
  - Rollback files to previous snapshots or parent versions.
- **Analytics:**
  - List the most **recently edited files**.
  - Identify files with the **largest version trees**.
- **Custom Data Structures:**
  - Hash Map for file storage.
  - Tree for managing version histories.
  - Heap for analytics and sorting.
- **Robust Error Handling:**
  - Gracefully handles invalid inputs and illegal operations.

---

## **Ambition**
This project was created as part of the **COL106 - Data Structures and Algorithms** course to:
- Implement **fundamental data structures** from scratch.
- Understand **real-world systems like Git** through simplified modeling.
- Gain experience with **efficient time and memory management**.
- Handle **complex command parsing** and **error detection**.

---

## **Compilation and Setup**

### **Requirements**
- **C++17/g++7.0** or later
- A terminal or command line interface (Linux, macOS, or Windows with WSL)

### **Instructions for setup**
1. **Download:**
   - Download all the files except **"Readme.md"** and **"LICENSE"** and put them in the same folder.
2. **Grant execution permission:**
   - by using the command
   ```sh
   >chmod +x compile.sh
3. **Compile the code:**
   - Run the command **`./compile.sh`** (without the quotes) to compile the program.

4. **Run the executable:**
   - Execute the program using **`./start`** (without the quotes).

5. **Interact with the system using commands**:
   ```sh
   CREATE project.txt
   SNAPSHOT project.txt "Initial Release"
   INSERT project.txt "Hello World"
   HISTORY project.txt
   ROLLBACK project.txt
   ```

---

## **System Architecture**

The system is organized into **three core modules**, each implemented from scratch.

### **1. Hash Map (`hashmap.hpp`)**
Stores **all files** in the system by their filenames as **keys**.

| Feature            | Details                                                                 |
|--------------------|-------------------------------------------------------------------------|
| **Implementation** | `vector<vector<pair<K,V>>>` (Separate Chaining)                        |
| **Usage**          | Fast storage and retrieval of files.                                   |
| **Average Complexity** | `insert`, `find`, `erase` → **O(1)**                              |
| **Worst Case Complexity** | **O(n)** (when all keys collide)                                |
| **Memory Usage**   | `O(n + m)` where `n = number of files` and `m = number of collisions` |

#### Example:
```cpp
hashmap.insert("report.txt", fileObject);
hashmap.find("report.txt");
```

---

### **2. Version Tree (`tree.hpp`)**
Each file maintains its **own tree** where:
- **Nodes = File versions**
- **Edges = Changes between versions**

| Feature             | Details                                                     |
|---------------------|-------------------------------------------------------------|
| **Snapshot Nodes**  | Frozen versions that cannot be edited directly.             |
| **Branching**       | When a snapshot is taken and further edits occur, branches form. |
| **Root Node**       | Represents the very first version of the file.              |
| **Purpose**         | Efficiently manage version history and rollbacks.           |

#### Example Tree Structure:
```
Version 0 (root)
   |
   +-- Version 1 (snapshot: "Initial Draft")
   |       |
   |       +-- Version 2 (edited after snapshot)
   |
   +-- Version 3 (alternate branch)
```

| Operation     | Time Complexity | Description |
|---------------|----------------|-------------|
| Create root node | **O(1)** | File creation |
| Insert new version | **O(1)** | Add child node |
| Snapshot | **O(1)** | Mark current node as frozen |
| Rollback | **O(1)** | Move pointer to parent/snapshot |
| Traverse history | **O(h)** | `h` = height of version tree |

Memory per node:
- `content`: dynamic string → **O(k)** where `k` is content size  
- `snapshotMessage`: **O(m)**  
- pointers (`parent` + `children`): **O(1)**

---

### **3. Max Heap (`heap.hpp`)**
Used for **system-wide analytics**, such as:
- Most recently edited files.
- Files with the largest number of versions.

| Feature | Details |
|----------|---------|
| **Key Structure** | Pair of `(priorityValue, filename)` |
| **Tie-breaking** | Alphabetical order when priority is equal |

| Operation   | Complexity |
|-------------|------------|
| `push`      | **O(log n)** |
| `pop`       | **O(log n)** |
| `top`       | **O(1)** |
| Build Heap  | **O(n)** |

---

## **Commands**

### 1. `CREATE`
Create a new empty file.

| Parameter | Description |
|------------|-------------|
| `<filename>` | Name of the file to be created (must be unique). |

**Usage:**
```
CREATE report.txt
```

**System Response:**
```
File with name 'report.txt' created successfully
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| File already exists | `CREATE report.txt` (file exists) | `error: File already exists` |


---

### 2. `READ`
Display the **current content** of a file.

**Usage:**
```
READ report.txt
```

**Response:**
```
Current content of 'report.txt': Hello World
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| File does not exist | `READ notes.txt` | `error: File not found` |

---

### 3. `INSERT`
Append content to the current version or **create a new version if the current version is a snapshot**.

**Usage:**
```
INSERT report.txt "Added conclusion section"
```

**Response:**
```
Content appended in current version of report.txt
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| File missing | `INSERT draft.txt "hello"` | `error: File not found` |
| Empty content | `INSERT report.txt` | `error: Content cannot be empty` |

---

### 4. `UPDATE`
Replace the **entire content** of the current version.

**Usage:**
```
UPDATE report.txt "Rewritten introduction"
```

**Response:**
```
Content updated for current version of report.txt
```

---

### 5. `SNAPSHOT`
Mark the **current version as frozen** with a message.

**Usage:**
```
SNAPSHOT report.txt "Milestone 1"
```

**Response:**
```
Snapshot created for version: 3 of report.txt
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| Snapshot on already snapshotted version | `SNAPSHOT report.txt` (on snapshot node) | `error: Current version is already a snapshot` |

---

### 6. `ROLLBACK`
Revert the file to a **previous snapshot** or to its **parent version** if no ID provided.

**Usage:**
```
ROLLBACK report.txt 2
```

or
```
ROLLBACK report.txt
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| Invalid ID | `ROLLBACK report.txt 999` | `error: Invalid version ID` |
| At root version | `ROLLBACK report.txt` (root version) | `error: The current version has no parent to Rollback to` |

---

### 7. `HISTORY`
List **all snapshots** of a file with details.

**Usage:**
```
HISTORY report.txt
```

**Response:**
```
Snapshot History of this file:
Version ID: 1
Message: Initial Draft
Snapshotted At: Wed Sep 10 19:45:05 2025
```

---

### 8. `RECENT_FILES`
Show the **most recently edited `n` files**.

**Usage:**
```
RECENT_FILES 3
```

**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| More than available files | `RECENT_FILES 6` (only 3 exist) | `error: Only 3 files exist` |
| Negative or zero count | `RECENT_FILES -5` | `error: Invalid command, usage: RECENT_FILES <positive integer>` |

---

### 9. `BIGGEST_TREES`
List files with the **largest version trees**.

**Usage:**
```
BIGGEST_TREES 2
```
## Few Notes on Commands:

> **Note:**  
> 1. I tried hard to detect unfinished command calls like `CREATE<whitespace>` to display an error,  
>    but ended up not adding it to the finalized version as it was getting complicated.  
>
> 2. While running any command if you hit enter without writting the filename it will move to next line you can still write the filename of the desired file.
> 3. The `ROLLBACK` command can run **with or without the `[version ID]` parameter**:  
>    - **With `[version ID]`** → Rolls back the file to the specified version ID (if it exists).  
>    - **Without `[version ID]`** → Attempts to rollback to the **parent version** of the current version (if available).  
> 4. If you try to Snapshot a version without giving a message, the terminal will move to the next line and will try again to take input of the message, if you hit enter again, then it snapshot the version with a empty string message.
> 5. While successfully running any command it will give you a succession message in the terminal, even though that dosen't usually happen with the terminal commands. Also each such of these messages or error message are separated by two new lines so that if there a lot of commands it is easy to read and debug.
> 6. In my implementation of the `RECENT_FILES` and `BIGGEST_TREES` analytics it creates a new max heap each time the commands are called and pops the desired amount of files and always destroys itself after the function call.
> 7. I'm using `<ctime>` library for time management due to which the time precision in seconds. So, if two files are updated within a single second, both will have the same last_edited time. In which case my code tiebreaks according their **file Names** aplhabetically.


**Errors:**
| Condition | Example | Response |
|------------|---------|----------|
| More than available files | `BIGGEST_TREES 9` (only 3 exist) | `error: Only 3 files exist` |
| Negative or zero count | `BIGGEST_TREES -3` | `error: Invalid command, usage: BIGGEST_TREES <positive integer>` |

---

---

## **Error Handling Summary**

The system guarantees **safe execution** by:
-
- Preserving spaces in file contents and messages.
- Validating all numeric inputs.
- Rejecting invalid characters in IDs and counts.
- Displaying **descriptive error messages**.

---

## **Time & Memory Complexity Summary**

| Operation              | Time Complexity | Memory Complexity |
|------------------------|-----------------|------------------|
| CREATE / READ / UPDATE / INSERT | **O(1)** avg | **O(k)** (k = content size) |
| SNAPSHOT               | **O(1)** | **O(m)** (m = snapshot message size) |
| ROLLBACK               | **O(1)** | - |
| HISTORY (traverse tree)| **O(h)** | **O(h)** |
| RECENT_FILES           | **O(n log n)** | **O(n)** |
| BIGGEST_TREES          | **O(n log n)** | **O(n)** |

---

## **Example Session**
```
> CREATE project.txt
File with name 'project.txt' created successfully

> SNAPSHOT project.txt "Initial root"
Snapshot created for version: 0 of project.txt

>INSERT project.txt "Hello mom"
Created new version: 1 of project.txt

> SNAPSHOT project.txt "First"
Snapshot created for version: 1 of project.txt

> INSERT project.txt "My nameless Hero"
Created new version: 2 of project.txt

> HISTORY project.txt
Snapshot History of this file:
Version ID: 1
Message: First
Snapshotted At: Wed Sep 10 19:45:05 2025

> RECENT_FILES 1
Most recently edited 1 files:
project.txt (Last Edited: Wed Sep 10 19:47:10 2025)
```

---

## **Conclusion**
The **Time-Travelling File System** is:
- A **self-contained educational tool** for learning:
  - Version control concepts
  - Data structure implementation
  - Efficient time and memory management
- A showcase of:
  - Custom-built **hash maps**, **trees**, and **heaps**
  - Robust command parsing and error handling
  - Clean and scalable design

---

<sub>Made by Budhyadeb Roy | COL106 Pre-minor Assignment</sub>