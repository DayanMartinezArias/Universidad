#include <fstream>

#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    std::string file_name = argv[1];
    std::ifstream file(file_name);
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    // Tokenize the input code
    std::vector<token> tokens = tokenize(content);

    Parser parser(tokens);
    parser.parse();
    return 0;
}
