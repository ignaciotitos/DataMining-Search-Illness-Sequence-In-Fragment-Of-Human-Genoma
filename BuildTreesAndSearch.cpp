#include "ReadFiles.cpp"
#include "GenerateDotTree.cpp"
#include <string>

using namespace std;

// This part it's used to build the tree with the sequences of the mutations

// Insert the sequences to the trie 
void insertSequenceToTree(PatriciaTrie& trie, const vector<string>& seqs) {
    for (const auto& sequence : seqs){
        if (sequence != "")
            trie.insertWord(sequence);
    }
}

// Transform the sequences from the .fa files into string sequences.
// Then inserto to the tree and create the .dot file.
// It's only generated here because the tree for the large sequences there are a lot of nodes,
// we can't see it and takes so much time to generate.
// This tree you can see and have a knowledge of his form.
void buildMutationsTree(PatriciaTrie& trie, int& maxSequenceSize) {
    string directory = current_working_directory() + "/MUTATIONS";
    vector<string> seqsForMutations = listDir(directory, maxSequenceSize, true); // vector de las secuencias de las mutaciones
    insertSequenceToTree(trie, seqsForMutations); // Inserta las mutaciones en el arbol
    generateDotFile(trie, "trie.dot"); // Genera el archivo .dot para ver el arbol
}

// Search the large sequence into the mutations trie
// It's search the sequence of size of (i, maxSequenceSize).
// maxSequenceSize is the size of the max sequence inside the sequences of the mutations vector.
void searchSequenceMut(PatriciaTrie& trie, string seq, int& maxSequenceSize) {
    for (size_t i = 0; i <= seq.size() - maxSequenceSize; ++i) {
        string fragment = seq.substr(i, maxSequenceSize);
        bool search = trie.searchSequenceInMutations(fragment);

        if (search){
            cout << i << ": It's found" << endl;
            cout << fragment << endl;
        }
    }
}

/* This part it's used to build the tree with the large sequence
   Works when you pass the large sequence, then it fragment the sequence
   and insert into the trie.
*/

// Fragment the sequence and insert into the tie.
void insertFragmentsIntoTrie(PatriciaTrie& trie, const string& sequence, size_t fragmentSize) {
    size_t totalSize = sequence.size();

    for (size_t i = 0; i < totalSize - fragmentSize; i++) {
        string fragment = sequence.substr(i, fragmentSize);
        trie.insertWord(fragment);
    }
    //generateDotFile(trie, "trieLarge.dot");
}

// It takes the vector of the mutations sequences and search it in the tree 
// that have the large sequence inside.
void searchSequenceLarge(PatriciaTrie& trie, vector<string> mutations) {
    for (size_t i = 0; i < mutations.size(); ++i) {
        //string fragment = seq.substr(i, maxSequenceSize);
        bool search = trie.searchMutationsInSequence(mutations[i]);

        if(search){
            cout << i <<": It's found" << endl;
            cout << mutations[i] << endl;
        }
    }
}

// Get the sequences of the mutations and transform to strings.
vector<string> getMutationsSeqs(int& maxSequenceSize) {
    string directory = current_working_directory() + "/MUTATIONS";
    vector<string> seqsForMutations = listDir(directory, maxSequenceSize, true);
    return seqsForMutations;
}