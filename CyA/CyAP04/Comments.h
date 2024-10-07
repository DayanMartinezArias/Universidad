#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <iostream>

class Analyzer;

enum CommentType {
  FunctionHeader, // Comment describing a function
  BasicComment    // Simple inline or block comment
};

class Comment {
 public:
  Comment(const CommentType& type, int start, int end, const std::string& content, bool header) : type_(type), start_(start), end_(end), content_(content), header_(header) {}
  CommentType GetType() const;
  int GetStart() const;
  int GetEnd() const;
  std::string GetContent() const;
  bool IsAHeader() const;
  friend std::ostream& operator<<(std::ostream& os, Comment obj);

 private:
  CommentType type_;  
  int start_;  
  int end_;   
  std::string content_;   
  bool header_;     
};

#endif