#include "geneBank.h"

// =================================== PUBLIC FUNCTIONS =======================================================
void GENE_BANK::sort(Sample array[], int fileSize)
{
    this->fileSize = fileSize;
    merge_sort(array, fileSize);
}

GENE_BANK::GENE_BANK()
{
    this->fileSize = 0;
    this->entryByte = sizeof(Sample);
}

GENE_BANK::~GENE_BANK()
{
    cout << "Destructing ..." << endl;
}

void GENE_BANK::indexSamples(Sample array[], int indexArray[])
{
    this->p_index(array, indexArray);
}

void GENE_BANK::displayResearcher(int speciesCode, int offset, char *filename)
{
    bool checkSample;

    checkSample = searchSample(speciesCode, offset, filename);

    if (checkSample)
    {
        p_displayResearcher(speciesCode, offset, filename);
    }
    else
    {
        cout << "Sample record doesn't exist! Can't display researcher name." << endl;
    }
}

bool GENE_BANK::searchSample(int speciesCode, int offset, char *filename)
{
    fstream sortedBinary;
    Sample temp;

    sortedBinary.open(filename, ios::in | ios::binary);

    if (!sortedBinary)
    {
        cout << "ERROR: Could not open file: " << filename << endl;
        return false;
    }

    // Seek to the record: (startIndex of species + offset) * record size
    streampos byteOffset = (speciesCode + offset) * sizeof(Sample);
    sortedBinary.seekg(byteOffset, ios::beg);

    if (!sortedBinary)
    {
        sortedBinary.close();
        return false;
    }

    // Attempt to read the record
    sortedBinary.read(reinterpret_cast<char *>(&temp), sizeof(Sample));
    int bytesRead = sortedBinary.gcount();
    sortedBinary.close();

    // Return true only if read succeeded and record is not marked deleted
    if (bytesRead == sizeof(Sample) && temp.speciesCode != -1)
        return true;

    return false;
}

void GENE_BANK::updateResearcher(int speciesCode, int offset, char *newName, char *filename)
{
    bool checkSample;

    checkSample = this->searchSample(speciesCode, offset, filename);

    if (checkSample)
    {
        p_updateResearcher(speciesCode, offset, newName, filename);
    }
    else
    {
        cout << "Sample record to be updated doesn't exist!" << endl;
        return;
    }
}

void GENE_BANK::deleteSample(int speciesCode, int offset, char *filename)
{
    bool checkSample;

    checkSample = this->searchSample(speciesCode, offset, filename);

    if (checkSample)
    {
        p_deleteSample(speciesCode, offset, filename);
    }
    else
    {
        cout << "Sample record to be deletesd doesn't exist!" << endl;
    }
}

void GENE_BANK::printSampleRange(int speciesCode, int startIndex, int endIndex, char *filename)
{
    if (startIndex >= endIndex)
        throw MyException("ERROR: start index is larger than end index!");
    else
        this->p_printRange(speciesCode, startIndex, endIndex, filename);
}

// =================================== PRIVATE FUNCTIONS =======================================================

// your sorting algorithm here

void GENE_BANK::merge_sort(Sample arr[], int size)
{
    if (size <= 1)
        return;

    int mid = size / 2;

    merge_sort(arr, mid);              // Sort left half
    merge_sort(arr + mid, size - mid); // Sort right half
    merge(arr, mid, size);
}

void GENE_BANK::merge(Sample arr[], int mid, int size)
{
    Sample *left = new Sample[mid];
    Sample *right = new Sample[size - mid];

    for (int i = 0; i < mid; i++)
        left[i] = arr[i];
    for (int i = 0; i < size - mid; i++)
        right[i] = arr[mid + i];

    int i = 0, j = 0, k = 0;
    while (i < mid && j < size - mid)
    {
        if (left[i].speciesCode <= right[j].speciesCode)
        {
            arr[k++] = left[i++];
        }
        else
            arr[k++] = right[j++];
    }
    while (i < mid)
        arr[k++] = left[i++];
    while (j < size - mid)
        arr[k++] = right[j++];

    delete[] left;
    delete[] right;
}

void GENE_BANK::p_index(Sample array[], int indexArray[])
{
    // Initialize all to -1 (not yet found)
    for (int i = 0; i < 5; i++)
        indexArray[i] = -1;

    // Find the first record index for each species in the sorted array
    for (int i = 0; i < fileSize; i++)
    {
        int code = array[i].speciesCode;
        if (code >= 0 && code <= 4 && indexArray[code] == -1)
            indexArray[code] = i; // Store starting index of this species
    }
}

void GENE_BANK::p_displayResearcher(int speciesCode, int offset, char *filename)
{
    fstream sortedBin;
    Sample temp;

    sortedBin.open(filename, ios::in | ios::binary);
    streampos byteOffset = (speciesCode + offset) * sizeof(Sample);
    sortedBin.seekg(byteOffset, ios::beg);

    if (!sortedBin)
    {
        sortedBin.close();
        return;
    }

    // Attempt to read the record
    sortedBin.read(reinterpret_cast<char *>(&temp), sizeof(Sample));

    sortedBin.close();

    cout << temp.researcher << endl;
}

void GENE_BANK::p_updateResearcher(int speciesCode, int offset, char *newName, char *filename)
{
    fstream sortedBin(filename, ios::in | ios::out | ios::binary);
    Sample temp;

    sortedBin.seekg(0, ios::beg);

    if (!sortedBin)
    {
        sortedBin.close();
        return;
    }

    streampos byteOffset = (speciesCode + offset) * sizeof(Sample);
    // Attempt to read the record
    sortedBin.seekg(byteOffset, ios::beg);
    sortedBin.read(reinterpret_cast<char *>(&temp), sizeof(Sample));

    cout << "Previous researcher name " << temp.researcher << endl;

    strncpy(temp.researcher, newName, MAX_RESEARCHER_NAME - 1);
    temp.researcher[MAX_RESEARCHER_NAME - 1] = '\0';

    sortedBin.seekp(byteOffset, ios::beg);
    sortedBin.write(reinterpret_cast<char *>(&temp), sizeof(Sample));

    sortedBin.close();

    cout << "Updated researcher name to " << temp.researcher << endl;
}

void GENE_BANK::p_deleteSample(int speciesCode, int offset, char *filename)
{
    fstream sortedBin(filename, ios::in | ios::out | ios::binary);
    Sample temp;

    if (!sortedBin)
    {
        sortedBin.close();
        return;
    }

    streampos byteOffset = (speciesCode + offset) * sizeof(Sample);
    sortedBin.seekg(byteOffset, ios::beg);
    sortedBin.read(reinterpret_cast<char *>(&temp), sizeof(Sample));

    if (sortedBin.gcount() != sizeof(Sample))
    {
        cout << "ERROR: Could not read record." << endl;
        sortedBin.close();
        return;
    }

    temp.speciesCode = -1;

    // Write the tombstoned record back in place
    sortedBin.seekp(byteOffset, ios::beg);
    sortedBin.write(reinterpret_cast<char *>(&temp), sizeof(Sample));

    sortedBin.close();

    cout << "Record deleted at offset " << offset << endl;
}

void GENE_BANK::p_printRange(int speciesCode, int startIndex, int endIndex, char *filename)
{
    fstream sortedBin(filename, ios::in | ios::out | ios::binary);
    Sample temp;

    if (!sortedBin)
    {
        cout << "ERROR: Could not open file." << endl;
        return;
    }

    streampos byteOffset = (speciesCode + startIndex) * sizeof(Sample);
    sortedBin.seekg(byteOffset, ios::beg);

    // Read and print one record at a time across the range
    for (int i = startIndex; i <= endIndex; i++)
    {
        sortedBin.read(reinterpret_cast<char *>(&temp), sizeof(Sample));

        if (sortedBin.gcount() != sizeof(Sample))
        {
            cout << "ERROR: Could not read record at offset " << i << endl;
            break;
        }

        if (temp.speciesCode == -1) // skip deleted records
            continue;

        cout << "SampleID: " << temp.sampleID << " | "
             << "SpeciesCode: " << temp.speciesCode << " | "
             << "Purity: " << temp.purityScore << " | "
             << "Researcher: " << temp.researcher << endl;
    }
}
