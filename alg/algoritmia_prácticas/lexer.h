#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>
#include <regex>

enum class Type {
  END,
  NUMBER,
  STRING,
  PRINT,
  ID,
  ASSIGN,
  OP,
  UNKNOWN
};

struct token {
  Type token_type;
  std::string value;
};

void debug(const token& token) {
  std::cout << "Type: " << static_cast<int>(token.token_type) << "; value: " << token.value << "\n";
}

std::vector<token> tokenize(const std::string& code) {
  std::vector<token> tokens;

  std::regex token_regex(R"((\d+)|(\".*?\")|(screen)|([A-Za-z_]\w*)|(=)|(\-|\*|\+|\/)|(;))");
  auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
  auto tokens_end = std::sregex_iterator();
  for (auto it = tokens_begin; it != tokens_end; ++it) {
        std::smatch match = *it;
    
        if (match[7].matched) {
          tokens.push_back({Type::END, match.str()});
        } else if (match[1].matched) {
            tokens.push_back({Type::NUMBER, match.str()});
        } else if (match[2].matched) {
            tokens.push_back({Type::STRING, match.str()});
        } else if (match[3].matched) {
            tokens.push_back({Type::PRINT, match.str()});
        } else if (match[4].matched) {
            tokens.push_back({Type::ID, match.str()});
        } else if (match[5].matched) {
            tokens.push_back({Type::ASSIGN, match.str()});
        } else if (match[6].matched) {
            tokens.push_back({Type::OP, match.str()});
        } else {
            tokens.push_back({Type::UNKNOWN, match.str()});
        }
    }

    return tokens;
}

#endif
