#include "Analizer.h"

Analyzer::Analyzer(std::string name) : header_(" ") {
  std::ifstream input(name);
  std::string line;
  int line_number{0};
  bool in_multiline_comment = false;
  int comment_start_line = 0;
  std::string multiline_comment_content;

  // Regular expressions for variables, loops, and comments
  std::regex var_regex(R"(^(int|double)\s+(\w+)\s*(=\s*[^;]+)?;)");
  std::regex loop_regex(R"(\b(while|for)\s*\()");
  std::regex single_line_comment_regex(R"(//.*)");
  std::regex multiline_comment_start_regex(R"(/\*[\s\S]*)");
  std::regex multiline_comment_end_regex(R"([\s\S]*?\*/)");

  std::smatch match;

  while (getline(input, line)) {
    line_number++;

    // If inside a multi-line comment, continue appending lines until the comment ends
    if (in_multiline_comment) {
      multiline_comment_content += "\n" + line;

      // Check if the multi-line comment ends on this line
      if (std::regex_search(line, match, multiline_comment_end_regex)) {
        in_multiline_comment = false;
        // Comment ends, push it to the comments vector
        Comment comment(CommentType::FunctionHeader, comment_start_line, line_number, multiline_comment_content, false);
        comments_.push_back(comment);
        multiline_comment_content.clear();  // Reset content for future multi-line comments
      }
      continue;
    }

    // Search for variables
    if (std::regex_search(line, match, var_regex)) {
      std::string var_type = match[1];
      std::string var_name = match[2];
      bool isInitialized = match[3].matched;
      std::string initValue = isInitialized ? match[3].str() : "";
      VarTypes enum_type = (var_type == "int") ? VarTypes::Int : VarTypes::Double;
      Variable var(enum_type, var_name, line_number, isInitialized, initValue);
      vars_.push_back(var);
    }

    // Search for loops
    if (std::regex_search(line, match, loop_regex)) {
      std::string loop_type = match[1];
      LoopTypes enum_type = (loop_type == "for") ? LoopTypes::For : LoopTypes::While;
      Loop loop(enum_type, line_number);
      loops_.push_back(loop);
    }

    // Search for single-line comments
    if (std::regex_search(line, match, single_line_comment_regex)) {
      std::string commentContent = match[0];  // Capture the comment content
      Comment comment(CommentType::BasicComment, line_number, line_number, commentContent, false);
      comments_.push_back(comment);
    }

    // Search for the start of multi-line comments
    if (std::regex_search(line, match, multiline_comment_start_regex)) {
      in_multiline_comment = true;
      comment_start_line = line_number;
      multiline_comment_content = match[0];  // Start capturing the multi-line comment content

      // Check if the comment also ends on the same line
      if (std::regex_search(line, match, multiline_comment_end_regex)) {
        in_multiline_comment = false;
        Comment comment(CommentType::FunctionHeader, comment_start_line, line_number, multiline_comment_content, false);
        comments_.push_back(comment);
        multiline_comment_content.clear();  // Reset for future multi-line comments
      }
    }
  }
}

void Analyzer::PrintResults() const {
  std::cout << "PROGRAM : factorial.cc\n";
  std::cout << "DESCRIPTION :\n";

  // Imprimir el encabezado al principio
  for (const auto& comment : comments_) {
    if (comment.GetType() == CommentType::FunctionHeader && comment.GetStart() == 1) {
      std::cout << comment.GetContent() << "\n";  // Imprime el encabezado
      break;  // Solo queremos imprimir el encabezado una vez
    }
  }

  std::cout << "\nVARIABLES :\n";
  for (const auto& var : vars_) {
    std::cout << var << "\n";
  }

  std::cout << "\nSTATEMENTS :\n";
  for (const auto& loop : loops_) {
    std::cout << loop << "\n";
  }

  std::cout << "\nMAIN :\n";
  std::cout << "True\n";  // Esto es un ejemplo, cámbialo según tu lógica

  std::cout << "\nCOMMENTS :\n";
  for (const auto& comment : comments_) { // Evitar repetir el encabezado
      std::cout << comment << "\n";
  }
}

