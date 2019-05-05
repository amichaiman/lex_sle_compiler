#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    START = 1,
    DO,
    ELSE,
    IF,
    ENDI,
    INT,
    PUT,
    PROG,
    GET,
    REAL,
    THEN,
    VAR,
    LOOP,
    ENDL,
    UNTIL,
    ENDP,
    ID,
    NUM,
    RELOP,
    LOGOP,
    ADDOP,
    MULOP,
    ASSIGNOP,
    DOT,
    COMMA,
    SEMICOLON,
    COLON,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    SPACE,
    TAB,
    ENTER,
    INVALID_TOKEN,
} token_t;

void create_open_output_files(FILE** tok_file, FILE** lst_file, char* file_name);
int valid_file_name(char* filename);
void init_tok_file(FILE* tok_file);
void handle_token(FILE* tok_file, FILE* lst_file, token_t current_token, char* token);
