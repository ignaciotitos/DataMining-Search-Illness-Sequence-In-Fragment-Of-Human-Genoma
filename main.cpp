#include "ReadFiles.cpp"
#include "BuildTreesAndSearch.cpp"

using namespace std;

int maxSequenceSize = 0;

int main(int argc, char *argv[]) {
    PatriciaTrie trieMutations = PatriciaTrie();
    PatriciaTrie trieLarge = PatriciaTrie();

    // This is the file that contains the large sequence
    string fileToRead = current_working_directory() + "/Homo_sapiens_CFTR_sequence-2.fa";
    string seq = processFile(fileToRead, maxSequenceSize); 

    cout << "Enter 0 to search the long sequence for mutations or 1 to search the long sequence for mutations: ";
    int choice = 1;
    cin >> choice;

    if (choice == 0) {
        buildMutationsTree(trieMutations, maxSequenceSize);
        searchSequenceMut(trieMutations, seq, maxSequenceSize);
    } else if (choice == 1) {
        vector<string> seqs = getMutationsSeqs(maxSequenceSize);
        insertFragmentsIntoTrie(trieLarge, seq, maxSequenceSize); 
        searchSequenceLarge(trieLarge, seqs);
    }
    return 0;
}