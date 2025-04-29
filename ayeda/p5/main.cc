// main.cc
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "bst.h"
#include "avl_tree.h"

using namespace std;

enum TreeType { TREE_ABB, TREE_AVL };
enum InitMode { MANUAL, RANDOM, FILE_INPUT };

struct Config {
    TreeType treeType = TREE_ABB;
    InitMode initMode = MANUAL;
    size_t randomSize = 0;
    string filename;
    bool trace = false;
};

void printHelp() {
    cout << "Uso: programa [opciones]\n"
         << "Opciones:\n"
         << "  -ab <abb|avl>      Tipo de árbol (ABB por defecto)\n"
         << "  -init <i> [s][f]   Inicialización:\n"
         << "     i=manual        Árbol vacío\n"
         << "     i=random [s]    Generar s NIFs aleatorios\n"
         << "     i=file [s][f]   Leer s NIFs del archivo f\n"
         << "  -trace [y|n]       Mostrar traza (n por defecto)\n"
         << "  -h                 Mostrar esta ayuda\n";
}

Config parseArgs(int argc, char* argv[]) {
    Config config;
    
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-ab" && i + 1 < argc) {
            string type = argv[++i];
            if (type == "avl") config.treeType = TREE_AVL;
            else if (type != "abb") {
                cerr << "Tipo de árbol no válido: " << type << endl;
                exit(1);
            }
        }
        else if (arg == "-init" && i + 1 < argc) {
            string mode = argv[++i];
            if (mode == "manual") {
                config.initMode = MANUAL;
            }
            else if (mode == "random") {
                config.initMode = RANDOM;
                if (i + 1 < argc && isdigit(argv[i+1][0])) {
                    config.randomSize = stoul(argv[++i]);
                } else {
                    cerr << "Se requiere tamaño para inicialización random" << endl;
                    exit(1);
                }
            }
            else if (mode == "file") {
                config.initMode = FILE_INPUT;
                if (i + 1 < argc) {
                    config.filename = argv[++i];
                    if (i + 1 < argc && isdigit(argv[i+1][0])) {
                        config.randomSize = stoul(argv[++i]);
                    }
                } else {
                    cerr << "Se requiere nombre de archivo para inicialización file" << endl;
                    exit(1);
                }
            }
        }
        else if (arg == "-trace" && i + 1 < argc) {
            string trace = argv[++i];
            config.trace = (trace == "y");
        }
        else if (arg == "-h") {
            printHelp();
            exit(0);
        }
    }
    
    return config;
}

vector<long> generateRandomNIFs(size_t count) {
    vector<long> nifs;
    srand(time(0)); // Inicializar semilla para números aleatorios
    for (size_t i = 0; i < count; ++i) {
        nifs.push_back(10000000 + rand() % 90000000);
    }
    return nifs;
}

vector<long> readNIFsFromFile(const string& filename, size_t max_count = 0) {
    vector<long> nifs;
    ifstream file(filename);
    if (!file) {
        throw runtime_error("No se pudo abrir el archivo: " + filename);
    }

    string line;
    while ((max_count == 0 || nifs.size() < max_count) && getline(file, line)) {
        try {
            long num = stol(line);
            if (num >= 10000000 && num <= 99999999) {
                nifs.push_back(num);
            } else {
                cerr << "NIF inválido ignorado (debe tener 8 dígitos): " << line << endl;
            }
        } catch (const invalid_argument& e) {
            cerr << "NIF inválido ignorado: " << line << " (" << e.what() << ")" << endl;
        }
    }

    return nifs;
}

template <typename Tree>
void mainMenu(Tree& tree, bool trace) {
    int option;
    do {
        cout << "\nMenú:\n"
             << "[0] Salir\n"
             << "[1] Insertar clave\n"
             << "[2] Buscar clave\n"
             << "[3] Mostrar árbol inorden\n"
             << "Opción: ";
        cin >> option;

        switch (option) {
            case 0:
                cout << "Saliendo...\n";
                break;
            case 1: {
                long nif;
                cout << "Introduce NIF a insertar (8 dígitos): ";
                cin >> nif;
                try {
                    bool inserted = tree.Insert(nif);
                    cout << (inserted ? "Insertado correctamente" : "NIF ya existente") << endl;
                    if (trace) {
                        cout << "\nÁrbol actual:\n" << tree << endl;
                    }
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 2: {
                long nif;
                cout << "Introduce NIF a buscar (8 dígitos): ";
                cin >> nif;
                try {
                    bool found = tree.Search(nif);
                    cout << (found ? "NIF encontrado" : "NIF no encontrado") << endl;
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 3:
                cout << "Recorrido inorden: ";
                tree.InOrder();
                break;
            default:
                cout << "Opción no válida\n";
        }
    } while (option != 0);
}

int main(int argc, char* argv[]) {
    Config config = parseArgs(argc, argv);

    try {
        vector<long> initialNIFs;
        
        switch (config.initMode) {
            case RANDOM:
                initialNIFs = generateRandomNIFs(config.randomSize);
                break;
            case FILE_INPUT:
                initialNIFs = readNIFsFromFile(config.filename, config.randomSize);
                break;
            case MANUAL:
            default:
                break;
        }

        if (config.treeType == TREE_AVL) {
            AVL<long> tree;
            for (const auto& nif : initialNIFs) {
                try {
                    tree.Insert(nif);
                } catch (const invalid_argument& e) {
                    cerr << "Error insertando NIF: " << e.what() << endl;
                }
            }
            if (config.trace && !initialNIFs.empty()) {
                cout << "Árbol inicial (AVL):\n" << tree << endl;
            }
            mainMenu(tree, config.trace);
        } else {
            BST<long> tree;
            for (const auto& nif : initialNIFs) {
                try {
                    tree.Insert(nif);
                } catch (const invalid_argument& e) {
                    cerr << "Error insertando NIF: " << e.what() << endl;
                }
            }
            if (config.trace && !initialNIFs.empty()) {
                cout << "Árbol inicial (ABB):\n" << tree << endl;
            }
            mainMenu(tree, config.trace);
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}