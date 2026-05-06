#include "hash.h"
#include "AVL/avl.h"

// Constructor - initializing table based on chosen hash table variant
HashTable::HashTable(int size, CollisionHandling variant)
{
    tableSize = size;
    elementCount = 0;
    method = variant;
    switch (method)
    {
    case CHAINING_VECTOR:
        tableVector.resize(size);
        break;
    case CHAINING_LIST:
        tableList.resize(size);
        break;
    case CHAINING_BST:
        tableBST.resize(size);
        break;
    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
        tableProbing.resize(size);
        break;
    }
}

HashTable::~HashTable() {}

int HashTable::hash1(const string &key) const
{
    unsigned long hash = 0;
    for (char c : key)
    {
        hash = (hash * 31 + c);
    }
    return hash % tableSize;
}

int HashTable::hash2(const string &key) const
{
    unsigned long hash = 5381;
    for (char c : key)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return 1 + (hash % (tableSize - 1));
}

// complete this
int HashTable::probe(const string &key, int index, int i) const
{
    int h1 = hash1(key);
    int h2 = hash2(key);
    int c1 = 1337;
    int c2 = 7331;

    switch (method)
    {
    case LINEAR_PROBING:
        return (h1 + i) % tableSize;
    case QUADRATIC_PROBING:
        return (h1 + c1 * i + c2 * i * i) % tableSize;
    case DOUBLE_HASHING:
        return (h1 + i * h2) % tableSize;
    default:
        return index;
    }
}
void HashTable::insert1(const string &key, int value)
{
    resizeIfNeeded();
    int index = hash1(key);
    switch (method)
    {
    case CHAINING_VECTOR:
        for (auto &pair : tableVector[index])
        {
            if (pair.first == key)
            {
                value = pair.second; // Key already exists, update the value
                return;
            }
        }
        tableVector[index].push_back({key, value});
        break;

    case CHAINING_LIST:
        // complete this
        for (auto &pair : tableList[index])
        {
            if (pair.first == key)
            {
                value = pair.second; // Key already exists, update the value
                return;
            }
        }
        tableList[index].push_back({key, value}); // Qst: do you advise using
        break;

    case CHAINING_BST:
        // complete this - use AVL insert method
        tableBST[index].insert(key, value);
        break;

    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
        int slot = findEmptySlot(key);
        if (slot == -1)
        {
            cerr << "Hash table is full, cannot insert: " << key << endl;
            return;
        }
        tableProbing[slot] = {key, value};
        break;
    }
    elementCount++;
}

bool HashTable::search(const string &key, int &value)
{
    int index = hash1(key);
    switch (method)
    {
    case CHAINING_VECTOR:
        for (auto &pair : tableVector[index])
        {
            if (pair.first == key)
            {
                return true;
            }
        }
        return false;

    case CHAINING_LIST:
        // complete this
        for (auto &pair : tableList[index])
        {
            if (pair.first == key)
            {
                return true;
            }
        }
        return false;

    case CHAINING_BST:
        // complete this - use AVL insert method
        return tableBST[index].search(key, value);

    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
        // complete this
        int i = 0;
        int visited = 0;
        while (visited < tableSize)
        {
            int probeIndex = probe(key, index, i);
            if (tableProbing[probeIndex].first.empty())
                return false; // empty slot = key not present
            if (tableProbing[probeIndex].first == key)
            {
                value = tableProbing[probeIndex].second;
                return true;
            }
            i++;
            visited++;
        }
    }
    return false; // full table, not found
}
static const string DELETED = "__DELETED__";

bool HashTable::remove(const string &key)
{
    // complete this
    int index = hash1(key);

    switch (method)
    {
    case CHAINING_VECTOR:
    {
        auto &vec = tableVector[index];
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            if (it->first == key)
            {
                vec.erase(it);
                return true;
            }
        }
        return false;
    }

    case CHAINING_LIST:
    {
        auto &lst = tableList[index];
        for (auto it = lst.begin(); it != lst.end(); ++it)
        {
            if (it->first == key)
            {
                lst.erase(it);
                return true;
            }
        }
        return false;
    }

    case CHAINING_BST:
        return tableBST[index].remove(key); // delegate to AVL remove

    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
    {
        int i = 0;
        int visited = 0;
        while (visited < tableSize)
        {
            int probeIndex = probe(key, index, i);
            if (tableProbing[probeIndex].first.empty())
                return false; // genuine empty = never inserted
            if (tableProbing[probeIndex].first == key)
            {
                tableProbing[probeIndex] = {"__DELETED__", -1}; // tombstone
                return true;
            }
            i++;
            visited++;
        }
        return false;
    }
    }
    return false;
}

// Function to read data from file
vector<pair<string, int>> readDataFromFile(const string &filename)
{
    vector<pair<string, int>> data;
    ifstream file(filename);
    string key;
    int value;
    if (!file)
    {
        cerr << "Error reading file: " << filename << endl;
        return data;
    }
    while (file >> key >> value)
    {
        data.push_back({key, value});
    }

    file.close();
    return data;
}

// Benchmark function for custom HashTable
void HashTable::benchmarkHashTable(HashTable &table, const vector<pair<string, int>> &data, int numSearch, int numDelete)
{
    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto &pair : data)
    {
        table.insert1(pair.first, pair.second);
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i)
    {
        table.search(data[rand() % data.size()].first, value);
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i)
    {
        table.remove(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}

void HashTable::displayStats()
{
    int totalElements = 0;
    int maxChainLength = 0;
    int emptyBuckets = 0;

    switch (method)
    {
    case CHAINING_VECTOR:
        for (const auto &bucket : tableVector)
        {
            if (bucket.empty())
                emptyBuckets++;
            maxChainLength = max(maxChainLength, (int)bucket.size());
            totalElements += bucket.size();
        }
        break;
    case CHAINING_LIST:
        for (const auto &bucket : tableList)
        {
            if (bucket.empty())
                emptyBuckets++;
            maxChainLength = max(maxChainLength, (int)bucket.size());
            totalElements += bucket.size();
        }
        break;
    case CHAINING_BST:
        for (const auto &bucket : tableBST)
        {
            vector<pair<string, int>> elements = bucket.inOrderTraversal();
            if (elements.empty())
            {
                emptyBuckets++;
            }
            else
            {
                maxChainLength = max(maxChainLength, (int)elements.size());
                totalElements += elements.size();
            }
        }
        break;
    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
        for (const auto &entry : tableProbing)
        {
            if (!entry.first.empty())
                totalElements++;
            else
                emptyBuckets++;
        }
        break;
    }

    cout << "Total elements: " << totalElements << endl;
    cout << "Load factor: " << (double)totalElements / tableSize << endl;
    cout << "Empty buckets: " << emptyBuckets << endl;
    if (method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST)
    {
        cout << "Max chain length: " << maxChainLength << endl;
    }
}

void HashTable::rehash()
{
    // Step 1 — collect all live entries
    vector<pair<string, int>> live;
    for (int i = 0; i < tableSize; i++)
    {
        if (!tableProbing[i].first.empty() &&
            tableProbing[i].first != DELETED)
        {
            live.push_back(tableProbing[i]);
        }
    }

    // Step 2 — wipe the table
    for (int i = 0; i < tableSize; i++)
        tableProbing[i] = {"", 0};

    // Step 3 - re-insert
    for (auto &pair : live)
    {
        int slot = findEmptySlot(pair.first);
        if (slot != -1)
            tableProbing[slot] = pair;
    }
}

int HashTable::findEmptySlot(const string &key)
{
    // complete this
    int index = hash1(key);
    int firstDel = -1; // remember first tombstone we passed
    int i = 0;
    int visited = 0;

    while (visited < tableSize)
    {
        int probeIndex = probe(key, index, i);

        if (tableProbing[probeIndex].first == key)
            return probeIndex; // key already exists → update in place

        if (tableProbing[probeIndex].first == DELETED && firstDel == -1)
            firstDel = probeIndex; // note first tombstone, keep probing
                                   // (key might exist further down chain)

        if (tableProbing[probeIndex].first.empty())
            return (firstDel != -1) ? firstDel : probeIndex;
        i++;
        visited++;
    }

    // Table is full of live entries and tombstones
    return (firstDel != -1) ? firstDel : -1; // -1 means no room at all
}

// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded()
{
    switch (method)
    {
    // ─── Chaining variants ────
    case CHAINING_VECTOR:
    {
        double loadFactor = (double)elementCount / tableSize;
        if (loadFactor <= 0.7)
            return;

        int newSize = tableSize * 2;
        vector<vector<pair<string, int>>> newTable(newSize);

        for (auto &bucket : tableVector)
            for (auto &kv : bucket)
            {
                // recompute hash against new size inline
                unsigned long h = 0;
                for (char c : kv.first)
                    h = h * 31 + c;
                newTable[h % newSize].push_back(kv);
            }

        tableVector = move(newTable);
        tableSize = newSize;
        break;
    }
    case CHAINING_LIST:
    {
        double loadFactor = (double)elementCount / tableSize;
        if (loadFactor <= 0.7)
            return;

        int newSize = tableSize * 2;
        vector<list<pair<string, int>>> newTable(newSize);

        for (auto &bucket : tableList)
            for (auto &kv : bucket)
            {
                unsigned long h = 0;
                for (char c : kv.first)
                    h = h * 31 + c;
                newTable[h % newSize].push_back(kv);
            }

        tableList = move(newTable);
        tableSize = newSize;
        break;
    }
    case CHAINING_BST:
    {
        double loadFactor = (double)elementCount / tableSize;
        if (loadFactor <= 0.7)
            return;

        // collect all entries
        vector<pair<string, int>> live;
        for (auto &tree : tableBST)
        {
            auto elems = tree.inOrderTraversal();
            live.insert(live.end(), elems.begin(), elems.end());
        }

        int newSize = tableSize * 2;
        tableBST.assign(newSize, AVLTree{}); // fresh trees
        tableSize = newSize;

        for (auto &kv : live)
        {
            unsigned long h = 0;
            for (char c : kv.first)
                h = h * 31 + c;
            tableBST[h % tableSize].insert(kv.first, kv.second);
        }
        break;
    }

    // ─── Open-addressing variants ───
    case LINEAR_PROBING:
    case QUADRATIC_PROBING:
    case DOUBLE_HASHING:
    {
        // Count only truly occupied slots (not tombstones)
        int occupied = 0;
        for (int i = 0; i < tableSize; i++)
            if (!tableProbing[i].first.empty() &&
                tableProbing[i].first != DELETED)
                occupied++;

        double loadFactor = (double)occupied / tableSize;
        if (loadFactor <= 0.7)
            return;

        // Save live entries, grow table, re-insert
        vector<pair<string, int>> live;
        live.reserve(occupied);
        for (int i = 0; i < tableSize; i++)
            if (!tableProbing[i].first.empty() &&
                tableProbing[i].first != DELETED)
                live.push_back(tableProbing[i]);

        tableSize *= 2;
        tableProbing.assign(tableSize, {"", 0});

        for (auto &kv : live)
        {
            int slot = findEmptySlot(kv.first);
            if (slot != -1)
                tableProbing[slot] = kv;
        }
        break;
    }
    }
}

// Benchmark function for built-in hash table in C++
void benchmarkStdUnorderedMap(const vector<pair<string, int>> &data, int numSearch, int numDelete)
{
    unordered_map<string, int> stdMap;

    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto &pair : data)
    {
        stdMap[pair.first] = pair.second;
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i)
    {
        auto it = stdMap.find(data[rand() % data.size()].first);
        if (it != stdMap.end())
        {
            value = it->second;
        }
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i)
    {
        stdMap.erase(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "std::unordered_map Benchmark Results:" << endl;
    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}