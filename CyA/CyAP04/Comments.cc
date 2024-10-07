/**
 * @file Comments.cc
 * @author Martínez Arias, Dayán Alu0101644561
 * @brief Implementation of the comment class, wich defines a comment
 * @version 0.1
 * @date 2024-10-07
 * 
 */

#include "Comments.h"

/**
 * @brief Returns the type of comment
 * 
 * @return CommentType 
 */
CommentType Comment::GetType() const{
  return type_;
} 

/**
 * @brief Returns the start line 
 * 
 * @return int 
 */
int Comment::GetStart() const{
  return start_;
}

/**
 * @brief Returns the end lines
 * 
 * @return int 
 */
int Comment::GetEnd() const{
  return end_;
}

/**
 * @brief Returns the content
 * 
 * @return std::string 
 */
std::string Comment::GetContent() const{
  return content_;
}

/**
 * @brief Returns  whether the comment is a header or not
 * 
 * @return true 
 * @return false 
 */
bool Comment::IsAHeader() const{
  return header_;
}

/**
 * @brief Prints the comment
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, Comment obj) {
  if (obj.GetStart() == obj.GetEnd()) {
    os << "[ Line " << obj.GetStart() << "] ";  
  } else {
    os << "[ Line " << obj.GetStart() << " - " << obj.GetEnd() << "] ";  
  }
    
  switch (obj.GetType()) {
    case CommentType::FunctionHeader:
      if(obj.GetStart() == 1) {
        os << "DESCRIPTION";
      } else {
        os << "FUNCTION HEADER: " << obj.GetContent();
      }
      break;
    case CommentType::BasicComment:
      os << obj.GetContent();
      break;
    }
  return os;
}
