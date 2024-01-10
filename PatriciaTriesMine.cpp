#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "PatriciaNode.cpp"

#define INT_MAX 2147483647

using namespace std;

#pragma once
class PatriciaTrie {
    private:
        PatriciaNode* root;
    
    public:
        // Constructor w/out parameters
        PatriciaTrie() : root(nullptr) {}

        void insertWord(const string& word) {

            if (searchWord(word)) {
                //cout << "The word is already in the tree: " << endl;
                return;
            }

            // if the tree is empty
            if (root == nullptr) {
                root = new PatriciaNode(false, "", nullptr); // create the root
                PatriciaNode* p = new PatriciaNode(true, word, root); //create the first word of the tree 
                root->setNewChildren(p); // put the node as a child of the root
                return;
            }
            else { // if not, the hunt begins
                PatriciaNode* currentNode = root;

                // Let's have a count on where we are in the word. 
                // To see later if we finish the word or still have characters.
                int posInWord = 0; 
                
                // This method search the child with better relations to the word.
                // It returns the max correlation in characters of thw word with the key of the better child
                // Return the better child too
                pair<int, int> best = searchForMaxCorrelationChildren(word, posInWord, currentNode);
                int maxCorrelation = best.first;
                int childNode = best.second;

                while (posInWord < word.size()) {
                    
                    // If there's no child found, the above function return best.first as -INT_MAX
                    // This means that currentNode doesn't have a child with any same characters with the word  
                    if (maxCorrelation == -INT_MAX) {
                        // Create a new node with the key of the word from the position we are till final.
                        // Initialize as leaf and with the currentNode as father
                        PatriciaNode *pn = new PatriciaNode(true, word.substr(posInWord), currentNode);
                        currentNode->setNewChildren(pn);
                        break;
                    }
                    else {
                        // If have a child with at least 1 correlation, we move forward to this child and 
                        // sum the position in the word to keep searching
                        posInWord += maxCorrelation;
                        currentNode = currentNode->children[childNode];

                        // If the currentNode has no children and the correlation get all the key of the node,
                        // this means that we get the final word in this path, so we have two options.
                        if (currentNode->hasChildrenEmpty() && maxCorrelation == currentNode->keySize()) {
                            // If leaf is the node to be inserted, 
                            // the new node is added below to save that the current node is a terminal node.
                            if (currentNode->getLeaf()) {
                                PatriciaNode* pn = new PatriciaNode(true, word.substr(posInWord), currentNode);
                                currentNode->setNewChildren(pn);
                            }else {
                                // If not, we append what we have left of the word in the key of the currentNode.
                                currentNode->key.append(word.substr(posInWord));
                                currentNode->setLeaf(true);
                            }
                            break;
                        }
                        // If the currentNode has children and the correlation is with all the key means tow things
                        else if (!currentNode->hasChildrenEmpty() && maxCorrelation == currentNode->keySize()) {
                            // Firts, we reach the final character of the word and the currentNode is not leaf
                            // so we are in the middle of the tree and insert put the currentNode as leaf 
                            // because now is a word.
                            if (posInWord == word.size() && !currentNode->getLeaf()) {
                                currentNode->setLeaf(true);
                                break;
                            }
                            // If not, continue searching
                        }
                        // If the node has no children and the correlation is less than the key
                        // we have two options
                        else if (currentNode->hasChildrenEmpty() && maxCorrelation < currentNode->keySize()) {
                            
                            // We create a node that's gonna have the key of the currentNode that's going from
                            // the position in maxCorrelation to the final.
                            // The part that is not the same as the part of the word(posInWord)
                            PatriciaNode *pn1 = new PatriciaNode(true, currentNode->key.substr(maxCorrelation), currentNode);

                            // That means that we have no word longer, so we reach the final
                            if (posInWord == word.size()) {
                                // Set the current node with the key that is same with the word
                                // and we set as leaf as now is a word.
                                // Then push back the new children (pn1)
                                currentNode->setKey(currentNode->key.substr(0, maxCorrelation));
                                currentNode->setLeaf(true);
                                currentNode->setNewChildren(pn1);
                            }else {
                                // If not, we have to create a new node to split the firts node as 
                                // explained before, and another node to have the key of the rest 
                                // of the word.
                                PatriciaNode *pn2 = new PatriciaNode(true, word.substr(posInWord), currentNode);

                                // We push back the two new nodes to the currentNode(father) and set it with
                                // leaf to false because now this node is not a word.
                                currentNode->setNewChildren(pn1);
                                currentNode->setNewChildren(pn2);
                                currentNode->setLeaf(false);
                                currentNode->setKey(currentNode->key.substr(0, maxCorrelation));
                            }
                            break;
                        }
                        // If the currentNode has children and the correlation is less than the key
                        else if (!currentNode->hasChildrenEmpty() && maxCorrelation < currentNode->keySize()) {
                            // Kept on with the characters that are not the same with the part of the word
                            // that are you searching and the key of the current node
                            string prefix = currentNode->key.substr(maxCorrelation);

                            // Put the key of the current node to the characters that are the same.
                            currentNode->setKey(currentNode->key.substr(0, maxCorrelation));

                            // Now create a new node who's going to be the part that is not the same with the word.
                            // if the current is leaf, the new node that you're splitting to should be either.
                            PatriciaNode* pn1 = new PatriciaNode(currentNode->getLeaf(), prefix, currentNode);

                            // Clear all the children that has current node and assign to the new node (pn1)
                            currentNode->clearChildren(pn1);

                            // Now put pn1 as a children
                            currentNode->setNewChildren(pn1);
                            
                            // Update the father to the child nodes that pn1 is his new father
                            pn1->setNewFather();
                            
                            // If we are in the final position of the word, we set that the node is a leaf
                            if (posInWord == word.size()) {
                                currentNode->setLeaf(true);
                            // If not, we create a new node with the extra characters in the word and 
                            // put as a child of the current node.
                            }else {
                                PatriciaNode* pn2 = new PatriciaNode(true, word.substr(posInWord), currentNode);
                                currentNode->setNewChildren(pn2);
                            }
                            break;

                            /*
                            The else if above means that if you have reached a node where the correlation between the word and the key are less than the key, 
                            but the current node has children, 
                            make sure to create a new node that has all the children of the current node and see if the new node is a leaf or not. 

                            It would not be convenient to paste the extra key that has no correlation with the children, 
                            because if you have many children, the same letter will start for all of them and in the end the search would be more tedious, 
                            better to have a node in the middle that contains the prefix of all of them, the search is faster.
                            */
                        }
                        // If the while does not break, it continues to search the children of the current node.
                        best = searchForMaxCorrelationChildren(word, posInWord, currentNode);
                        maxCorrelation = best.first;
                        childNode = best.second;
                    }
                }
            }
        }
        
        // Search returning if it's found the word or not
        bool searchWord(const string& word) {

            // See if the tree is empty
            if (root == nullptr) {
                return false;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;
            int wordSearchLen = 0; 

            // Have more or less the same logic as the insert
            while (posInWord <= word.size()) {
                // Search the best child
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) {   
                    return false;
                }
                /* See if the correlation at the current node plus the global computation of the characters in the word is the same as the word. 
                   Also if the child is a leaf and if the correlation at that node is complete.
                */
                else if ((posInWord + bestValues.first) == word.size() && 
                          currentNode->children[bestValues.second]->getLeaf() &&
                          bestValues.first == currentNode->children[bestValues.second]->keySize()) {
                    return true;
                }
                else {
                    currentNode = currentNode->children[bestValues.second]; // Keep searching
                    posInWord += bestValues.first;
                }
            }
        }

        // Find the word for the sequences, as the sequence will be longer than the ones inside the tree.
        bool searchMutationsInSequence(const string& word) {
            if (root == nullptr) {
                //cout << "The tree is empty" << endl;
                return false;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;

            // Same logic
            while (posInWord < word.size()) {
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) { 
                    return false;
                } else {
                    currentNode = currentNode->children[bestValues.second];
                    posInWord += bestValues.first;
                }

                if (posInWord == word.size() && currentNode->getLeaf()) {
                    return true;
                }
            }

            return false;
        }

        bool searchSequenceInMutations(const string& word) {
            if (root == nullptr) {
                //cout << "The tree is empty" << endl;
                return false;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;

            // Same logic
            while (posInWord < word.size()) {
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) { 
                    return false;
                } else {
                    currentNode = currentNode->children[bestValues.second];
                    posInWord += bestValues.first;
                }

                // Checks if the word is fully or partially matched in the current node.
                // This works for words that are longer that the sequence that you're searching
                // checks if the current node is leaf and does not has more children. 
                // This allow you to assume that the word is contained inside the sequence.
                // Also checks if the correlation is all in the current node key 
                if ((currentNode->getLeaf() && currentNode->hasChildrenEmpty()) &&
                    (bestValues.first == currentNode->keySize())) {
                        return true;
                }
            }

            return false;
        }

        
        // Return the node if the word was found
        PatriciaNode* searchWordReturningNode(const string& word) {

            // See if the tree is empty
            if (root == nullptr) {
                //cout << "The tree is empty";
                return nullptr;
            }

            PatriciaNode* currentNode = root;
            int posInWord = 0;

            while (posInWord <= word.size()) {
                pair<int, int> bestValues = searchForMaxCorrelationChildren(word, posInWord, currentNode);

                if (bestValues.first == -INT_MAX) {   
                    return nullptr;
                }
                else if ((posInWord + bestValues.first) == word.size() && 
                          currentNode->children[bestValues.second]->getLeaf() && 
                          bestValues.first == currentNode->children[bestValues.second]->keySize()) {
                    return currentNode->children[bestValues.second];
                }
                else {
                    currentNode = currentNode->children[bestValues.second]; 
                    posInWord += bestValues.first;
                }
            }
        }

        // Delete word
        /* 2 cases:
                1. No children:
                    - Go to the father and delete from its child list.
                    - Delete the node
                    - IF the parent has only one child, join them together and create a new node
                      only if the parent is not a leaf node.
                        - IF the above happens, to the parent-child node, make it a leaf. 
                        - Take all the children of the previous child node to the new parent-child node.   
                    - ELSE, leave everything as is.

                2. Has children:
                    - Set node->isLeaf to false.
                    - Try to upload it with the parent, otherwise leave it as it is by setting isLeaf to false.
                    - If it has only one child, upload the child with it.
                    - delete node.
            */
        void deleteWord(const string& word) {
            if (!searchWord(word)) {
                cout << "The word doesn't exist in the tree";
                return;
            }

            // Search the node to delete
            PatriciaNode* nodeToDelete = searchWordReturningNode(word);

            // If does not has children
            if (nodeToDelete->hasChildrenEmpty()) {
                PatriciaNode* child;

                // If the father only has one children and the father is not a leaf 
                if ((nodeToDelete->father->children.size() - 1) == 1 && !nodeToDelete->father->getLeaf()) {
                    // Take the child node of the father that we are not going to delete
                    for (PatriciaNode* c : nodeToDelete->father->children) {
                        if (c != nodeToDelete) {
                            child = c;
                            break;
                        }
                    }
                    
                    PatriciaNode* father = nodeToDelete->father;
                    // Sum the key of the child to the father, so we put them together
                    father->key += child->key;
                    // We assign the childs if have to the new father-child node
                    child->clearChildren(father);
                    // Set the new father of all the children
                    father->setNewFather();
                    // Set the new node according to whether the child was a leaf or not
                    father->setLeaf(child->getLeaf());
                    // Remove the children node, now is useless
                    child->removeMe();
                }

                // Remove the node that we were going to delete
                nodeToDelete->setLeaf(false);
                nodeToDelete->removeMe();

            // If has children
            }else {
                // If the node we are going to delete only has one child,
                // append the child node with the node to delete, so we only have to
                // delete the child node and put the node that we were deleting according to the leaf of the child.
                if (nodeToDelete->children.size() == 1) {
                    nodeToDelete->key += nodeToDelete->children[0]->key;
                    // If the child of the node to delete does not has children, only have to delete the child.
                    if (nodeToDelete->children[0]->hasChildrenEmpty()) {
                        nodeToDelete->children[0]->removeMe();
                        nodeToDelete->children.clear();
                    // If not, append the childrens of the children to the new parent-child node
                    // and then remove the child
                    }else {
                        nodeToDelete->children[0]->clearChildren(nodeToDelete);
                        nodeToDelete->setNewFather();
                        nodeToDelete->setLeaf(nodeToDelete->children[0]->getLeaf());
                        nodeToDelete->children[0]->removeMe();
                    }
                // if none of above, put the leaf of the node to false
                } else {
                    nodeToDelete->setLeaf(false);
                }
            }
        }

        // Search of in the children vector and take the child with max correlation
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

        PatriciaNode* getRoot() const{            
            return root;
        }
};

// See the tree through the terminal
void watchTree(PatriciaNode* root, int level = 0) {
    if (root == nullptr) {
        return;
    }

    for (int i = 0; i < level - 1; ++i) {
        cout << "|   ";
    }

    if (level > 0) {
        cout << "|-- " ;
    }
    cout << root->key << endl;

    for (size_t i = 0; i < root->children.size(); ++i) {
        watchTree(root->children[i], level + 1);
    }
}