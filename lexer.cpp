#include "lexer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
using namespace std; 

enum TokenType{
    END = -1,
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,
    LABEL = 101,
    GOTO = 102,
    PRINT = 103,
    INPUT = 104,
    LET = 105,
    IF = 106,
    THEN = 107,
    ENDIF = 108,
    WHILE = 109,
    REPEAT = 110,
    ENDWHILE = 111,
    EQ = 201,  
    PLUS = 202,
    MINUS = 203,
    ASTERISK = 204,
    SLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211,
    LPAREN = 301,
    RPAREN = 302,
    LBRACE = 303,
    RBRACE = 304,
    None = 305, 
};

class Token{
  public: 
    char text;
    string multiCharText;
    TokenType kind;
    int number; 
    Token(char* tokenText, TokenType tokenKind){
      text = *tokenText; 
      kind = tokenKind;
    }

    Token(string tokenText, TokenType tokenKind){
      multiCharText = tokenText; 
      kind = tokenKind;
    }

    Token(){
    }

    static TokenType checkKeyword(string token){
      for(string kind : {"LABEL", "GOTO", "PRINT", "INPUT", "LET", "IF", "THEN", "ENDIF", "WHILE", "REPEAT", "ENDWHILE"}){
        if(token == kind){
          if(kind == "LABEL"){
            return LABEL;
          }
          else if(kind == "GOTO"){
            return GOTO;
          }
          else if(kind == "PRINT"){
            return PRINT;
          }
          else if(kind == "INPUT"){
            return INPUT;
          }
          else if(kind == "LET"){
            return LET;
          }
          else if(kind == "IF"){
            return IF;
          }
          else if(kind == "THEN"){
            return THEN;
          }
          else if(kind == "ENDIF"){
            return ENDIF;
          }
          else if(kind == "WHILE"){
            return WHILE;
          }
          else if(kind == "REPEAT"){
            return REPEAT;
          }
          else if(kind == "ENDWHILE"){
            return ENDWHILE;
          }
        }
      }
      return None; 
    }
};

ifstream source = ifstream("sample.txt");
char currentChar; 
int currentLine = 1;
Token token; 

//process the next character 
void nextChar(){ 
  char c = source.get();
  if(c == ifstream::traits_type::eof()){
    currentChar = '\0';
  }else{
    currentChar = c;
  }
}

//return the lookahead character 
char peek(){
  char peekVal = source.peek();
  if(peekVal == ifstream::traits_type::eof()){
    return '\0';
  }
  return peekVal; 
}


//skip whitespaces in the source file, except new lines
void skipWhiteSpace(){
  while(currentChar == ' ' || currentChar == '\t'){
    nextChar();
  } 
}

//skip comments in the source file
//by typing a '#' you indicate that there is a single line comment in which case that line will be skipped
void skipComment(){
  if (currentChar == '#'){ 
    while(currentChar != '\n'){
      nextChar();
      cout<<"Skipping"<<endl; 
    }
    nextChar(); 
  }
}

//return the next character in the source file
void getToken(){
  skipWhiteSpace();
  skipComment(); 
  if(currentChar == '+'){
    token = Token(&currentChar, PLUS);
  }else if(currentChar == '-'){
    token = Token(&currentChar, MINUS);
  }else if(currentChar == '*'){
    token = Token(&currentChar, ASTERISK);
  }else if(currentChar == '/'){
    token = Token(&currentChar, SLASH);
  }else if(currentChar == '\n'){
    token = Token(&currentChar, NEWLINE);
  }else if(currentChar == '\0'){
    token = Token(&currentChar, END);
  }else if(currentChar == '='){ 
    if(peek() == '='){
      cout<<"found the second equal"; 
      token = Token(&currentChar, EQEQ);
      nextChar(); 
    }else{
      token = Token(&currentChar, EQ);
    }
  }else if (currentChar == '>'){
    if(peek() == '='){
      token = Token(&currentChar, GTEQ);
      nextChar(); 
    }
    else{
      token = Token(&currentChar, GT);
    }
  }else if(currentChar == '<'){
    if(peek() == '='){
      token = Token(&currentChar, LTEQ);
      nextChar();
    }
    else {
      token = Token(&currentChar, LT);
    }
  }else if(currentChar == '!'){
    if(peek() == '='){
      token = Token(&currentChar, NOTEQ);
      nextChar();
    }
    else{
      cout<<"! is not a recognized token"<<endl;
    }
  }
  else if (currentChar == '('){
    token = Token(&currentChar, LPAREN);
  }
  else if(currentChar == ')'){
    token = Token(&currentChar, RPAREN);
  }
  else if(currentChar == '{'){
    token = Token(&currentChar, LBRACE);
  }
  else if(currentChar == '}'){
    token = Token(&currentChar, RBRACE);
  }
  else if(isdigit(currentChar)){
    string number = ""; 
    number = number + currentChar;
    while(isdigit(peek())){
      nextChar();
      number = number + currentChar;
    }
    if(peek() == '.'){
      nextChar();
      if(!isdigit(peek())){
        cout<<"Invalid number"<<endl;
      }
      while(isdigit(peek())){
        nextChar();
        number = number + currentChar; 
      }
    }
    token = Token(number, NUMBER);
  }
  //Handling keywords
  else if(isalpha(currentChar)){
    string tokText = ""; 
    tokText = tokText + currentChar; 
    while(isalpha(peek()) || isdigit(peek())){
      nextChar();
      tokText = tokText + currentChar;
    }
    
    transform(tokText.begin(), tokText.end(), tokText.begin(), ::toupper); 
    
    TokenType keyword = token.checkKeyword(tokText);
    if(keyword == None){
      token = Token(tokText, IDENT);
    }
    else{
      token = Token(tokText, keyword);
    }
  }
  //handling strings
  else if(currentChar == '\"'){
    nextChar(); 
    string str = "";
    while(currentChar != '\"'){
      if(currentChar == '\n' || currentChar == '\r' || currentChar == '\t'){
        cout<<"Illegal character in string"; 
      }else{
        str += currentChar;
        nextChar();
      }
    }
    token = Token(str, STRING);
  }
  else{
    cout<<"Could not tokenize "<<currentChar<<endl; 
  }
  cout<<"token kind: " << token.kind<< " token text: "<< token.text << " Long text: "<< token.multiCharText << endl; 
  nextChar(); 
}