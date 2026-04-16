#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <cctype>

// TOKEN DEFINITION AND GROUPING
enum class TokenType {
    // Keywords
    IF, ELSE, WHILE, FOR, DO, BREAK,
    // Literals
    INTEGER, FLOAT, STRING, CHAR,
    // Operators
    EQUALS, EQEQ, PLUS, MINUS, MULT, DIV,
    // Special
    IDENTIFIER, END_OF_FILE, UNKNOWN
};

struct Token {
    TokenType tkType;
    std::string value;
    int line;
    int column;

    Token(TokenType type, const std::string& val, int ln, int col)
        : tkType(type), value(val), line(ln), column(col) {}
};

// For debugging display (converts enum values to strings)
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::DO: return "DO";
        case TokenType::BREAK: return "BREAK";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::EQEQ: return "EQEQ";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULT: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

// KEYWORD CHECK
TokenType keywordCheck(const std::string& word) {
    if (word == "if") return TokenType::IF;
    if (word == "else") return TokenType::ELSE;
    if (word == "while") return TokenType::WHILE;
    if (word == "for") return TokenType::FOR;
    if (word == "do") return TokenType::DO;
    if (word == "break") return TokenType::BREAK;
    return TokenType::IDENTIFIER; // if nothing matches
}

// LEXICAL ANALYZER - Reads character by character
std::vector<Token> tokenize(const std::string& input) { // vector list(dynamic array) of token objects
    std::vector<Token> tokens; //empty vector that will store all the tokens in the input
    int line = 1, col = 1;

    for (size_t i = 0; i < input.size();) {
        char c = input[i];

        // Skip whitespace
        if (isspace(c)) {
            if (c == '\n') { line++; col = 1; }
            else col++;
            i++;
            continue;
        }

        // Identifiers / keywords - Starts when a letter or underscore is found; Keeps reading letters/numbers to form a word; Uses keywordCheck() to decide if it’s a keyword or an identifier; Adds it to the token list.

        if (isalpha(c) || c == '_') {
            size_t start = i;
            while (i < input.size() && (isalnum(input[i]) || input[i] == '_')) i++;
            std::string word = input.substr(start, i - start);
            tokens.push_back(Token(keywordCheck(word), word, line, col));
            col += (i - start);
            continue;
        }

        // Numbers (int or float) - Detects numbers; Checks for a decimal point to classify it as a FLOAT; Pushes it as either an INTEGER or FLOAT
        if (isdigit(c)) {
            size_t start = i;
            bool isFloat = false;
            while (i < input.size() && isdigit(input[i])) i++;
            if (i < input.size() && input[i] == '.') {
                isFloat = true;
                i++;
                while (i < input.size() && isdigit(input[i])) i++;
            }
            std::string num = input.substr(start, i - start);
            tokens.push_back(Token(isFloat ? TokenType::FLOAT : TokenType::INTEGER, num, line, col));
            col += (i - start);
            continue;
        }

        // String literals - Detects text between double quotes; Handles escape characters like \" or \\; Stores it as a STRING token
        if (c == '"') {
            size_t start = i++;
            while (i < input.size() && input[i] != '"') {
                if (input[i] == '\\') i++; // escape char
                i++;
            }
            if (i < input.size()) i++; // closing quote
            std::string str = input.substr(start, i - start);
            tokens.push_back(Token(TokenType::STRING, str, line, col));
            col += (i - start);
            continue;
        }

        // Char literals - Similar to string literals but for 'a' or '\\n'
        if (c == '\'') {
            size_t start = i++;
            while (i < input.size() && input[i] != '\'') {
                if (input[i] == '\\') i++;
                i++;
            }
            if (i < input.size()) i++;
            std::string ch = input.substr(start, i - start);
            tokens.push_back(Token(TokenType::CHAR, ch, line, col));
            col += (i - start);
            continue;
        }

        // Operators
        if (c == '=' && i + 1 < input.size() && input[i+1] == '=') {
            tokens.push_back(Token(TokenType::EQEQ, "==", line, col));
            i += 2; col += 2;
            continue;
        }
        if (c == '=') { tokens.push_back(Token(TokenType::EQUALS, "=", line, col)); i++; col++; continue; }
        if (c == '+') { tokens.push_back(Token(TokenType::PLUS, "+", line, col)); i++; col++; continue; }
        if (c == '-') { tokens.push_back(Token(TokenType::MINUS, "-", line, col)); i++; col++; continue; }
        if (c == '*') { tokens.push_back(Token(TokenType::MULT, "*", line, col)); i++; col++; continue; }
        if (c == '/') { tokens.push_back(Token(TokenType::DIV, "/", line, col)); i++; col++; continue; }

        // Unknown
        tokens.push_back(Token(TokenType::UNKNOWN, std::string(1, c), line, col));
        i++; col++;
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "EOF", line, col));
    return tokens;
}

// MAIN BLOCK - Takes a input and uses the tokenize() function to analyze and prints the tokens.
int main() {
    std::cout << "Enter code: ";
    std::string input;
    std::getline(std::cin, input);  // get one line input from user

    std::vector<Token> tokens = tokenize(input);

    for (auto &tok : tokens) {
        std::cout << tokenTypeToString(tok.tkType)
                  << " (" << tok.value << ") at line "
                  << tok.line << ", col " << tok.column << "\n";
    }
    return 0;
}
