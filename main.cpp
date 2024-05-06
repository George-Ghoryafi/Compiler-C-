#include <iostream>
#include <string>
#include <fstream>
#include "lexer.h"
using namespace std;


int main() {
  while(peek() != '\0'){
    nextChar();
    getToken(); 
  }
  return 0;   
}