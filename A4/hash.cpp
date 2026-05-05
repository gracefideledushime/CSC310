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
        // complete this
        index = (h1 + i) % tableSize;
        cout << 'l probing: ' << index;
    case QUADRATIC_PROBING:
        // complete this
        index = (h1 + c1 * i + c2 * i ^ 2) % tableSize;
        cout << 'q probing: ' << index;

    case DOUBLE_HASHING:
        // complete this
        index = (h1 + i * h2) % tableSize;
        cout << 'd hashing: ' << index;
    default:
        return index;
    }
}
void HashTable::insert(const string &key, int value)
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
                pair.second = value; // Key already exists, update the value
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
                pair.second = value; // Key already exists, update the value
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
        // complete this
        if (tableProbing[index].first.empty())
        {
            tableProbing[index] = {key, value};
        }
        else
        {
            int i = 1;
            index = probe(key, index, i);
            tableProbing[index] = {key, value};
        }
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
                pair.second = value;
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
                pair.second = value; // Key already exists, update the value
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
        return false; // full table, not found
    }
}

bool HashTable::remove(const string &key)
{
    // complete this int index = hash1(key);

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
        // Probing tables can't just blank a slot — that breaks the probe chain
        // for any key that was inserted AFTER this one collided here.
        // Solution: use a DELETED tombstone sentinel instead of truly emptying.
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
        table.insert(pair.first, pair.second);
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
    // complete this
}

int HashTable::findEmptySlot(const string &key)
{
    // complete this
}

// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded()
{
    // complete this
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