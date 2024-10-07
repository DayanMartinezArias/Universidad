/**
 * @file Analizer.h
 * @author Martínez Arias, Dayánn alu0101644561
 * @brief Implementation of the Analizer class, wich takes a vector of strings and converts it into a specific format
 * @version 0.1
 * @date 2024-10-07
 */

#include "Analizer.h"

/**
 * @brief Construct a new Analyzer:: Analyzer object
 * 
 * @param vector_of_strings 
 * @param name 
 */
Analyzer::Analyzer(std::vector<std::string> vector_of_strings, std::string name) : name_(name), all_content_(vector_of_strings) {}

/**
 * @brief Detects the main function
 * 
 * @param line 
 * @return true 
 * @return false 
 */
bool Analyzer::DetectMain(const std::string& line) {
  std::regex main_exists_regex(R"(^\s*int\s+main\s*\(\s*\))");
  return std::regex_search(line, main_exists_regex);
}

/**
 * @brief detects loop matches
 * 
 * @param line 
 * @param match 
 * @param loop_regex 
 * @param line_number 
 */
void Analyzer::LoopSearch(const std::string line, std::smatch match, std::regex loop_regex, int line_number) {
  // Search for loops
  if (std::regex_search(line, match, loop_regex)) {
    std::string loop_type = match[1];
    LoopTypes enum_type = (loop_type == "for") ? LoopTypes::For : LoopTypes::While;
    Loop loop(enum_type, line_number);
    loops_.push_back(loop);
  }
}

/**
 * @brief Detects variable matches
 * 
 * @param line 
 * @param match 
 * @param var_regex 
 * @param line_number 
 */
void Analyzer::VarSearch(const std::string line, std::smatch match, std::regex var_regex, int line_number) {
  // Search for variables
  if (std::regex_search(line, match, var_regex)) {
    std::string var_type = match[1];
    std::string var_name = match[2];
    bool is_initialized = match[3].matched;
    std::string init_value = is_initialized ? match[3].str() : "";
    VarTypes enum_type = (var_type == "int") ? VarTypes::Int : VarTypes::Double;
    Variable var(enum_type, var_name, line_number, is_initialized, init_value);
    vars_.push_back(var);
  }
}

/**
 * @brief Detects single ccomment matches
 * 
 * @param line 
 * @param match 
 * @param single_line_comment_regex 
 * @param line_number 
 */
void Analyzer::SingleCommentSearch(const std::string line, std::smatch match, std::regex single_line_comment_regex, int line_number) {
  // Search for single-line comments
  if (std::regex_search(line, match, single_line_comment_regex)) {
    std::string commentContent = match[0];  // Capture the comment content
    Comment comment(CommentType::BasicComment, line_number, line_number, commentContent, false);
    comments_.push_back(comment);
  }
}

/**
 * @brief Main method for analising the input
 * 
 */
void Analyzer::Analyze() {
  int line_number{0};
  bool in_multiline_comment = false;
  int comment_start_line = 0;
  std::string multiline_comment_content;
  is_main_ = false;

  std::regex var_regex(R"(^\s*(int|double)\s+(\w+)\s*(=\s*[^;]+)?;)");
  std::regex loop_regex(R"(^\s*(while|for)\s*\()");
  std::regex single_line_comment_regex(R"(//.*)");
  std::regex multiline_comment_start_regex(R"(/\*[\s\S]*)");
  std::regex multiline_comment_end_regex(R"([\s\S]*?\*/)");

  std::smatch match;

  for (std::string& line : all_content_) {
    line_number++;

    // If inside a multiline comment, continue adding more lines until the comment ends
    if (in_multiline_comment) {
      // Comments are analyzed inside this method because the logic for handeling comments its more dense
      multiline_comment_content += "\n" + line;

      // Check if the multiline comment ends on this line
      if (std::regex_search(line, match, multiline_comment_end_regex)) {
        in_multiline_comment = false;
        // Comment ends, push it to the comments vector
        Comment comment(CommentType::FunctionHeader, comment_start_line, line_number, multiline_comment_content, false);
        comments_.push_back(comment);
        multiline_comment_content.clear(); 
      }
      continue;
    }

    if(DetectMain(line)) {
      is_main_ = true;
    }

    LoopSearch(line, match, loop_regex, line_number);
    VarSearch(line, match, var_regex, line_number);
    SingleCommentSearch(line, match, single_line_comment_regex, line_number);

    // Search for the start of multiline comments
    if (std::regex_search(line, match, multiline_comment_start_regex)) {
      in_multiline_comment = true;
      comment_start_line = line_number;
      multiline_comment_content = match[0]; 

      // Check if the comment also ends on the same line
      if (std::regex_search(line, match, multiline_comment_end_regex)) {
        in_multiline_comment = false;
        Comment comment(CommentType::FunctionHeader, comment_start_line, line_number, multiline_comment_content, false);
        comments_.push_back(comment);
        multiline_comment_content.clear();  
      }
    }
  }
}

/**
 * @brief Counts the amount of whiles
 * 
 * @return int 
 */
int Analyzer::WhileAmount() const {
  int counter{0};
  for (Loop loop : loops_) {
    if (loop.GetType() == LoopTypes::While) {
      counter++;
    }
  }
  return counter;
}

/**
 * @brief Counts the amount of fors
 * 
 * @return int 
 */
int Analyzer::ForAmount() const {
  int counter{0};
  for (Loop loop : loops_) {
    if (loop.GetType() == LoopTypes::For) {
      counter++;
    }
  }
  return counter;
}

/**
 * @brief Counts the amount of doubles
 * 
 * @return int 
 */
int Analyzer::DoubleAmount() const {
  int counter{0};
  for (Variable vars : vars_) {
    if (vars.GetType() == VarTypes::Double) {
      counter++;
    }
  }
  return counter;
}

/**
 * @brief Counts the amount of ints
 * 
 * @return int 
 */
int Analyzer::IntAmount() const {
  int counter{0};
  for (Variable vars : vars_) {
    if (vars.GetType() == VarTypes::Int) {
      counter++;
    }
  }
  return counter;
}

/**
 * @brief Returns the name of the file
 * 
 * @return std::string 
 */
std::string Analyzer::GetName() const {
  return name_;
}

/**
 * @brief Prints the code information
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Analyzer& obj) {
  os << "PROGRAM : " << obj.GetName() << "\n";
  os << "DESCRIPTION :\n";

 
  for (const auto& comment : obj.comments_) {
    if (comment.GetType() == CommentType::FunctionHeader && comment.GetStart() == 1) {
      os << comment.GetContent() << "\n"; 
      break;
    }
  }

  os << "\nVARIABLES :\n";
  for (const auto& var : obj.vars_) {
    os << var << "\n";
  }

  os << "\nSTATEMENTS :\n";
  for (const auto& loop : obj.loops_) {
    os << loop << "\n";
  }

  os << "\nMAIN :\n";
  if (obj.is_main_) {
    os << "True\n";
  } else {
    os << "False\n";
  }

  os << "\nCOMMENTS :\n";
  for (const auto& comment : obj.comments_) { 
    os << comment << "\n";
  }

  os << "\nNUMbER OF VARIABLES:\n";
  os << "int = " << obj.IntAmount() << "\n";
  os << "double = " << obj.IntAmount() << "\n";

  os << "\nNUMbER OF LOOPS:\n";
  os << "for = " << obj.ForAmount() << "\n";
  os << "while = " << obj.WhileAmount() << "\n";
  return os;
}

