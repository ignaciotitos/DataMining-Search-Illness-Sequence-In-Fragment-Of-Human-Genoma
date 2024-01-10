#include <string>
#include <vector>

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

    bool getLeaf() const{
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

    // This method clear the children of the node that calls the function and copy on the node passed through reference
    // Then connect the the node who calls (father) to the copyNode (newChild).
    void clearChildren(PatriciaNode* copyNode) {
        for (PatriciaNode* child : this->children) {
            PatriciaNode* childCopy = new PatriciaNode(*child);  // Crear una copia profunda del nodo hijo
            copyNode->children.push_back(childCopy);
            delete child;  // Borrar el nodo original ya que hemos realizado una copia profunda
        }
        this->children.clear();
    }

    // This method remove the node that calls the function
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

    string getKey() const{
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