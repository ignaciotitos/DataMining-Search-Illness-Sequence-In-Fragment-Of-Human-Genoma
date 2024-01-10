#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <cstdlib> // Para el uso de system()
#include "PatriciaTriesMine.cpp"

using namespace std;

#pragma once
// Method to read a file (FASTA / .fa) and return as a sequence.
// The maxSequenceSize is to have the max size of all the sequence to later 
// have an idea of the fragment size you can divide the large sequence 
string processFile(string filepath, int& maxSequenceSize, bool notSetMaxSequence = false) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filepath << std::endl;
        return "";
    }

    string line = "";
    string sequence;

    while (getline(file, line)) {
        // Ignorar líneas de encabezado
        if (line[0] == '>') {
            continue;
        }

        // Concatenar la línea a la secuencia
        sequence += line;
    }

    if (maxSequenceSize < sequence.size() && notSetMaxSequence) {
        maxSequenceSize = sequence.size();
    }
    
    file.close();

    return sequence;
}

// Method to search on the directory and get all the .fa files and then read them all.
// Return a string vector with all the sequences read
vector<string> listDir(string dir, int& maxSequenceFile, bool canChangeMaxSequence) {
    DIR* directory;
    struct dirent* element;
    string elem;
    vector<string> directions;

    if (directory = opendir(dir.c_str())) {
        while (element = readdir(directory)) {
            elem = element->d_name;
            if (elem == "." || elem == "..") {
                continue;
            }
            string seq = processFile(dir+"/"+elem, maxSequenceFile, canChangeMaxSequence);
            directions.push_back(seq);
        }
    }

    closedir(directory);
    return directions;
}

// See in wich directory are you to search the files.
// Only works for windows I guess
string current_working_directory() {
    char* cwd = getcwd(0, 0);  // Para sistemas Unix (Mac y Linux)
    std::string working_directory(cwd);
    std::free(cwd);
    return working_directory;
}