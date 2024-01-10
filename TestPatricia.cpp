#include "PatriciaTriesMine.cpp"

// Brief test to see if Patricia works
int mainTest(){
    //Testear insert
    PatriciaTrie trie = PatriciaTrie();

    trie.insertWord("abc");
    trie.insertWord("abaco");
    trie.insertWord("abac");
    trie.insertWord("abcd");
    trie.insertWord("abcdo");
    trie.insertWord("abaci");
    trie.insertWord("abcdi");
    trie.insertWord("abcdio");
    trie.insertWord("caca");
    trie.insertWord("doeooeooooeoeoeoeoeo");
    trie.insertWord("dado");
    trie.insertWord("deal");
    trie.insertWord("ad");
    trie.insertWord("ca");
    trie.insertWord("ca");

    // // Testear search
    // cout << trie.searchWord("abcdi");
    // cout << trie.searchWord("abac");
    // cout << "\n" ;

    trie.deleteWord("abcdo");
    trie.deleteWord("abcdio");
    trie.deleteWord("dado");
    trie.deleteWord("abac");
    trie.deleteWord("ca");
    watchTree(trie.getRoot());

    cout << (trie.searchWord("abcdo") ? "EUREKA"
                                    :  "The word is NOT in the tree");
    cout << "\n";
    cout << (trie.searchWord("abcdi") ? "EUREKA"
                                    :  "The word is NOT in the tree");
    cout << "\n";
    cout << (trie.searchWord("ad") ? "EUREKA"
                                    :  "The word is NOT in the tree");
    cout << "\n";
    cout << (trie.searchWord("abac") ? "EUREKA"
                                    :  "The word is NOT in the tree");
    cout << "\n";
    cout << (trie.searchWord("abaci") ? "EUREKA"
                                    :  "The word is NOT in the tree");
                                    cout << "\n";
    cout << (trie.searchWord("ca") ? "EUREKA"
                                    :  "The word is NOT in the tree");
                                    cout << "\n";
    cout << (trie.searchWord("caca") ? "EUREKA"
                                    :  "The word is NOT in the tree");
    
    return 0;
}