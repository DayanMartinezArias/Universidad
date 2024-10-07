/**
 * @file Comments.h
 * @author Martínez Arias, Dayán Alu0101644561
 * @brief Declaration of the comment class
 * @version 0.1
 * @date 2024-10-07
 * 
 */

#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <iostream>

class Analyzer;

enum CommentType {
  FunctionHeader, 
  BasicComment   
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