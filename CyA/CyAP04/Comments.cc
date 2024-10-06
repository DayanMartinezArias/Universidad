#include "Comments.h"

CommentType Comment::GetType() const{
  return type_;
} 

int Comment::GetStart() const{
  return start_;
}

int Comment::GetEnd() const{
  return end_;
}

std::string Comment::GetContent() const{
  return content_;
}

bool Comment::IsAHeader() const{
  return header_;
}

std::ostream& operator<<(std::ostream& os, Comment obj) {
    // Formato de línea de inicio y fin
    if (obj.GetStart() == obj.GetEnd()) {
        os << "[ Line " << obj.GetStart() << "] ";  // Una sola línea
    } else {
        os << "[ Line " << obj.GetStart() << " - " << obj.GetEnd() << "] ";  // Varias líneas
    }
    
    // Contenido o tipo de comentario
    switch (obj.GetType()) {
        case CommentType::Description:
            os << "DESCRIPTION";
            break;
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
