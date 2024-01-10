#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "PatriciaTriesMine.cpp"

using namespace std;

/* This file it's used to generate a .dot file to see the tree graphically
   You can download the VSCode Extension tintinweb.graphviz-interactive-preview
   to see the tree in the .dot file generated 
*/
//////////////////////////////////////////////////////////////////////////

string getLabel(const PatriciaNode* node) {
    return (node->getKey().size() > 10) ? (node->getKey().substr(0, 10) + "..") : node->getKey();
}

string getColor(const PatriciaNode* node) {
    return (node->getLeaf()) ? "red" : "black";
}

// Genearte de nodes in the .dot syntax
void generateDotNodes(const PatriciaNode* node, std::ofstream& dotFile, unordered_map<const PatriciaNode*, std::string>& nodeIds) {
    if (!node) {
        return;
    }

    // Generar nodo actual
    std::string nodeId = std::to_string(nodeIds.size()); // Obtener el identificador único del nodo
    nodeIds[node] = nodeId; // Asignar el identificador al nodo en el mapa
    dotFile << "  \"" << nodeId << "\" [label=\"" << getLabel(node) << "\", color=\"" << getColor(node) << "\"];\n";

    // Llamada recursiva para los hijos
    for (const auto& child : node->children) {
        generateDotNodes(child, dotFile, nodeIds);
    }
}

// Generate the connections of the nodes in the .dot syntax
void generateDotConnections(const PatriciaNode* node, std::ofstream& dotFile, const unordered_map<const PatriciaNode*, std::string>& nodeIds) {
    if (!node) {
        return;
    }

    // Generar conexiones con los hijos
    for (const auto& child : node->children) {
        dotFile << "  \"" << nodeIds.at(node) << "\" -> \"" << nodeIds.at(child) << "\";\n";
    }

    // Llamada recursiva para los hijos
    for (const auto& child : node->children) {
        generateDotConnections(child, dotFile, nodeIds);
    }
}

// Generate the .dot file
void generateDotFile(const PatriciaTrie& trie, const std::string& filename) {
    std::ofstream dotFile(filename, std::ios::trunc);

    if (!dotFile.is_open()) {
        std::cerr << "Error al abrir/crear el archivo DOT: " << filename << std::endl;
        return;
    }

    // Encabezado del archivo DOT
    dotFile << "digraph G {\n";

    unordered_map<const PatriciaNode*, string> nodeIdMap;
    // Recorrer el trie y generar nodos y conexiones
    generateDotNodes(trie.getRoot(), dotFile, nodeIdMap);

    generateDotConnections(trie.getRoot(), dotFile, nodeIdMap);

    // Cierre del archivo DOT
    dotFile << "}\n";

    dotFile.close();
}

void visualizeTrie(const std::string& dotFilePath, const std::string& outputImagePath) {
    // Comando para generar la imagen desde el archivo DOT usando Graphviz
    std::string command = "C:\\Program Files\\Graphviz\\bin\\dot.exe -Tpng " + dotFilePath + " -o " + outputImagePath;

    // Ejecutar el comando
    int result = system(command.c_str());

    // Verificar si la ejecución fue exitosa
    if (result == 0) {
        std::cout << "El grafo ha sido visualizado con éxito." << std::endl;
    } else {
        std::cerr << "Error al visualizar el grafo." << std::endl;
    }
}