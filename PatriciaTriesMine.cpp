#include <iostream>
#include <string>
#include <vector>
#include <limits>

#define INT_MAX 2147483647

using namespace std;

struct PatriciaNode {
    bool isLeaf;
    string key;
    vector<PatriciaNode*> children;
    PatriciaNode* father;

    PatriciaNode(bool leaf, const string& k, PatriciaNode* f) : isLeaf(leaf), key(k), father(f) {}

    bool hasChildrenEmpty() {
        return this->children.empty();
    }

    bool getLeaf() {
        return this->isLeaf;
    }

    void setLeaf(bool leafValue) {
        this->isLeaf = leafValue;
    }

    void setNewFather() {
        for (int i = 0; i < children.size(); ++i) {
            children[i]->father = this;
        }
    }

    void clearChildren(PatriciaNode* copyNode) {
        for (PatriciaNode* child : this->children) {
            PatriciaNode* childCopy = new PatriciaNode(*child);  // Crear una copia profunda del nodo hijo
            copyNode->children.push_back(childCopy);
            delete child;  // Borrar el nodo original ya que hemos realizado una copia profunda
        }
        this->children.clear();
    }

    bool removeMe() {
        if (father == nullptr) {
            // Si no hay padre, no se puede eliminar
            return false;
        }

        // Buscar en el padre cual es el nodo a borrar para eliminarlo de su lista
        for (auto it = father->children.begin(); it != father->children.end(); ++it) {
            if (*it == this) {
                father->children.erase(it);
                delete this;
                return true;
            }
        }
        // No se encontró el nodo en la lista del padre
        return false;
    }

    string getKey() {
        return key;
    }

    void setKey(string key) {
        this->key = key;
    }

    void setNewChildren(PatriciaNode* child) {
        children.push_back(child);
    }

    int keySize() {
        return key.size();
    }
};

class PatriciaTrie {
    private:
        PatriciaNode* root;
    
    public:
        // Constructor w/out parameters
        PatriciaTrie() : root(nullptr) {}

        void InsertWord(const string& word) {

            if (searchWord(word)) {
                cout << "The word is already in the tree" << word << endl;
            }

            // empty tree
            if (root == nullptr) {
                root = new PatriciaNode(false, "", nullptr);
                PatriciaNode* p = new PatriciaNode(true, word, root);
                root->setNewChildren(p);
                return;
            }
            else {
                PatriciaNode* currentNode = root;
                int posInWord = 0;
                
                pair<int, int> best = searchForMaxCorrelationChildren(word, posInWord, currentNode);
                int maxCorrelation = best.first;
                int childNode = best.second;

                while (posInWord < word.size()) {

                    if (maxCorrelation == -INT_MAX) {
                        PatriciaNode *pn = new PatriciaNode(true, word.substr(posInWord), currentNode);
                        currentNode->setNewChildren(pn);
                        break;
                    }
                    else {
                        posInWord += maxCorrelation;
                        currentNode = currentNode->children[childNode];   
                        
                        if (currentNode->hasChildrenEmpty() && maxCorrelation == currentNode->keySize()) {
                            // Si es hoja el nodo a insertar, el nuevo nodo se añade debajo para guardar que el nodo actual es un nodo terminal
                            if (currentNode->getLeaf()) {
                                PatriciaNode* pn = new PatriciaNode(true, word.substr(posInWord), currentNode);
                                currentNode->setNewChildren(pn);
                            }else {
                                currentNode->key.append(word.substr(posInWord));
                            }
                            break;
                        }
                        else if (!currentNode->hasChildrenEmpty() && maxCorrelation == currentNode->keySize()) {
                            if (posInWord == word.size() && !currentNode->getLeaf()) {
                                currentNode->setLeaf(true);
                                break;
                            }
                            //continue;
                        }
                        else if (currentNode->hasChildrenEmpty() && maxCorrelation < currentNode->keySize()) {

                            PatriciaNode *pn1 = new PatriciaNode(true, currentNode->key.substr(maxCorrelation), currentNode);

                            if (posInWord == word.size()) {
                                currentNode->setKey(currentNode->key.substr(0, maxCorrelation));
                                currentNode->setLeaf(true);
                                currentNode->setNewChildren(pn1);
                            }else {
                                PatriciaNode *pn2 = new PatriciaNode(true, word.substr(posInWord), currentNode);

                                currentNode->setNewChildren(pn1);
                                currentNode->setNewChildren(pn2);
                                currentNode->setLeaf(false);
                                currentNode->setKey(currentNode->key.substr(0, maxCorrelation));
                            }
                            break;
                        }
                        else if (!currentNode->hasChildrenEmpty() && maxCorrelation < currentNode->keySize()) {
                            string prefix = currentNode->key.substr(maxCorrelation);

                            currentNode->setKey(currentNode->key.substr(0, maxCorrelation));
                            PatriciaNode* pn1 = new PatriciaNode(currentNode->getLeaf(), prefix, currentNode);

                            // Borra los hijos de currentNode y los mete en pn1
                            currentNode->clearChildren(pn1);

                            // Su unico hijo ahora va a ser pn1
                            currentNode->setNewChildren(pn1);
                            
                            // Actualiza el padre a los nodos hijos
                            pn1->setNewFather();
                            
                            if (posInWord == word.size()) {
                                currentNode->setLeaf(true);
                            }else {
                                PatriciaNode* pn2 = new PatriciaNode(true, word.substr(posInWord), currentNode);
                                currentNode->setNewChildren(pn2);
                            }
                            break;
                        }
                        // Search the kids of the currentNode
                        best = searchForMaxCorrelationChildren(word, posInWord, currentNode);
                        maxCorrelation = best.first;
                        childNode = best.second;
                    }
                }
            }
        }

        bool searchWord(const string& word) {

            // See if the tree is empty
            if (root == nullptr) {
                cout << "The tree is empty";
                return false;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;
            int wordSearchLen = 0; 

            while (posInWord <= word.size()) {
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) { //maxCorrelation == -INT_MAX    
                    return false;
                }
                /* Mira si la correlacion en el nodo actual mas el computo global de los caracteres de la palabra es igual que la palabra 
                   Ademas de que si el hijo es hoja y si la correlacion en ese nodo es completa
                */
                else if ((posInWord + bestValues.first) == word.size() && 
                          currentNode->children[bestValues.second]->getLeaf() &&
                          bestValues.first == currentNode->children[bestValues.second]->keySize()) {
                    return true;
                }
                else {
                    currentNode = currentNode->children[bestValues.second]; // The children where the maxCorrelation is max
                    posInWord += bestValues.first;
                }
            }
        }
        
        // Devolver palabra pero que me devuelvan el nodo a eliminar
        PatriciaNode* searchWordReturningNode(const string& word) {

            // See if the tree is empty
            if (root == nullptr) {
                cout << "The tree is empty";
                return nullptr;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;

            while (posInWord <= word.size()) {
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) { //maxCorrelation == -INT_MAX    
                    return nullptr;
                }
                else if ((posInWord + bestValues.first) == word.size() && 
                          currentNode->children[bestValues.second]->getLeaf() && 
                          bestValues.first == currentNode->children[bestValues.second]->keySize()) {
                    return currentNode->children[bestValues.second];
                }
                else {
                    currentNode = currentNode->children[bestValues.second]; // The children where the maxCorrelation is max
                    posInWord += bestValues.first;
                }
            }
        }

        void deleteWord(const string& word) {
            /* 2 casos:
                1. No tiene hijos:
                    - Se sube al padre y se borra el nodo de su lista de hijos
                    - Se borra el nodo
                    - IF el padre tiene solo un hijo, juntarlo y crear un nuevo nodo
                      sólo si el padre no es un nodo hoja.
                        - SI pasa lo anterior, al nodo padre-hijo, hacerlo hoja. 
                        - LLevar todos los hijos del nodo hijo anterior al nuevo nodo padre-hijo   
                    - ELSE, se deja todo como está.

                2. Tiene hijos:
                    - Poner el nodo->isLeaf a falso.
                    - Intentar subirla con el padre, sino, dejarla como esta quitando solo el isLeaf a false
                    - Si solo tiene un hijo, subir al hijo con el.
                    - delete nodo.
            */

            if (!searchWord(word)) {
                cout << "The word doesn't exist in the tree";
                return;
            }

            // busca la palabra
            PatriciaNode* nodeToDelete = searchWordReturningNode(word);

            // No tiene hijos
            if (nodeToDelete->hasChildrenEmpty()) {
                PatriciaNode* child;
                
                if ((nodeToDelete->father->children.size() - 1) == 1 && !nodeToDelete->father->getLeaf()) {
                    // Cogemos el hijo que no es el nodo que vamos a borrar
                    for (PatriciaNode* c : nodeToDelete->father->children) {
                        if (c != nodeToDelete) {
                            child = c;
                            break;
                        }
                    }
                    
                    PatriciaNode* father = nodeToDelete->father;
                    father->key += child->key;
                    child->clearChildren(father);
                    father->setNewFather();
                    father->setLeaf(child->getLeaf());
                    child->removeMe();
                }

                nodeToDelete->setLeaf(false);
                nodeToDelete->removeMe();

            // Tiene hijos
            }else {
                if (nodeToDelete->children.size() == 1) {
                    nodeToDelete->key += nodeToDelete->children[0]->key;
                    nodeToDelete->children[0]->removeMe();
                    nodeToDelete->children.clear();
                } else {
                    nodeToDelete->setLeaf(false);
                }
            }
        }

        pair<int, int> searchForMaxCorrelationChildren(const string& word, int posInWord, PatriciaNode* currentNode) {

            // Search the kids of the currentNode
            int maxCorrelation = -INT_MAX;
            int childNode = 0;

            for (int j = 0; j < currentNode->children.size(); ++j) {
                int tmpPos = posInWord;
                int posInChildren = 0;

                while (posInChildren < currentNode->children[j]->keySize() && 
                       tmpPos < word.size() &&
                       currentNode->children[j]->key[posInChildren] == word[tmpPos]) {

                    ++posInChildren;
                    ++tmpPos; 
                }

                if (posInChildren > maxCorrelation && posInChildren != 0) {
                    maxCorrelation = posInChildren;
                    childNode = j; 
                }
            }

            return make_pair(maxCorrelation, childNode);
        }

        // Destructor
        ~PatriciaTrie() {
        
        }

        PatriciaNode* getNode() {            
            return root;
        }
};

void watchTree(PatriciaNode* root, int level = 0) {
    if (root == nullptr) {
        return;
    }

    // Imprimir el nodo actual de manera más visual
    for (int i = 0; i < level - 1; ++i) {
        cout << "|   ";
    }

    if (level > 0) {
        cout << "|-- " ;
    }
    cout << root->key << endl;

    // Imprimir nodos hijos
    for (size_t i = 0; i < root->children.size(); ++i) {
        watchTree(root->children[i], level + 1);
    }
}
int main() {
    PatriciaTrie trie = PatriciaTrie();

    //Testear insert
    trie.InsertWord("abc");
    trie.InsertWord("abaco");
    trie.InsertWord("abac");
    trie.InsertWord("abcd");
    trie.InsertWord("abcdo");
    trie.InsertWord("abaci");
    trie.InsertWord("abcdi");
    trie.InsertWord("abcdio");
    trie.InsertWord("caca");
    trie.InsertWord("dedo");
    trie.InsertWord("dado");
    trie.InsertWord("deal");
    trie.InsertWord("ad");
    trie.InsertWord("ca");
    trie.InsertWord("ca");

    // // Testear search
    // cout << trie.searchWord("abcdi");
    // cout << trie.searchWord("abac");
    // cout << "\n" ;

    trie.deleteWord("abcdo");
    trie.deleteWord("abcdio");
    trie.deleteWord("dado");
    trie.deleteWord("abac");
    trie.deleteWord("ca");
    watchTree(trie.getNode());

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
    
}