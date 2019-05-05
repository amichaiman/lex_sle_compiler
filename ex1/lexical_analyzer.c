#include "lexical_analyzer.h"

#define TOK_EXTENSION (".tok")
#define LST_EXTENSION (".lst")

void create_open_output_files(FILE** tok_file, FILE** lst_file, char* file_name) {
    char *output_tok_file_name;
    char *output_lst_file_name;

    output_tok_file_name = (char*) malloc((strlen(file_name) + strlen(TOK_EXTENSION) + 1) * sizeof(char));
    output_lst_file_name = (char*) malloc((strlen(file_name) + strlen(LST_EXTENSION) + 1) * sizeof(char));

    if (!output_tok_file_name || !output_lst_file_name) {
        fprintf(stderr, "ERROR: allocation failed");
        exit(1);
    }

    sprintf(output_tok_file_name, "%s%s", file_name, TOK_EXTENSION);
    sprintf(output_lst_file_name, "%s%s", file_name, LST_EXTENSION);

    printf("creating token output file: %s\n", output_tok_file_name);
    printf("creating list output file: %s\n", output_lst_file_name);

    *tok_file = fopen(output_tok_file_name, "w");
    *lst_file = fopen(output_lst_file_name, "w");
}

int valid_file_name(char* filename) {
    char* file_extension_loc = filename + strlen(filename)-3;
    return (file_extension_loc == strstr(filename, "sle")) || (file_extension_loc == strstr(filename, "SLE"));
}

void center_print(FILE* f, char* str, int field_len) {
    int i, num_spaces = field_len-strlen(str);
    for(i=0; i<(num_spaces/2); i++) {
        fputc(' ', f);
    }

    fputs(str, f);

    for(i=0; i<(num_spaces/2)+(num_spaces%2); i++) {
        fputc(' ', f);
    }
}

#define LEXEME_FIELD_LEN (15)
#define TOKEN_FIELD_LEN  (25)
#define ATTR_FIELD_LEN   (25)

void print_line(FILE* f) {
    fprintf(f, "---------------+-------------------------+--------------------------\n");
}

void init_tok_file(FILE* tok_file) {
    print_line(tok_file);
    center_print(tok_file, "LEXEME", LEXEME_FIELD_LEN);
    fputc('|', tok_file);
    center_print(tok_file, "TOKEN", TOKEN_FIELD_LEN);
    fputc('|', tok_file);
    center_print(tok_file, "ATTRIBUTES", ATTR_FIELD_LEN);
    fputc('\n', tok_file);
    print_line(tok_file);
}


char* token_to_string(token_t token) {
    #define MAX_TOKEN_LEN (15)
    static char buf[MAX_TOKEN_LEN];

    switch (token) {
        case START:              strcpy(buf, "START");             break;
        case DO:                 strcpy(buf, "DO");                break;
        case ELSE:               strcpy(buf, "ELSE");              break;
        case IF:                 strcpy(buf, "IF");                break;
        case ENDI:               strcpy(buf, "ENDI");              break;
        case INT:                strcpy(buf, "INT");               break;
        case PUT:                strcpy(buf, "PUT");               break;
        case PROG:               strcpy(buf, "PROG");              break;
        case GET:                strcpy(buf, "GET");               break;
        case REAL:               strcpy(buf, "REAL");              break;
        case THEN:               strcpy(buf, "THEN");              break;
        case VAR:                strcpy(buf, "VAR");               break;
        case LOOP:               strcpy(buf, "LOOP");              break;
        case ENDL:               strcpy(buf, "ENDL");              break;
        case UNTIL:              strcpy(buf, "UNTIL");             break;
        case ENDP:               strcpy(buf, "ENDP");              break;
        case ID:                 strcpy(buf, "ID");                break;
        case NUM:                strcpy(buf, "NUM");               break;
        case RELOP:              strcpy(buf, "RELOP");             break;
        case LOGOP:              strcpy(buf, "LOGOP");             break;
        case ADDOP:              strcpy(buf, "ADDOP");             break;
        case MULOP:              strcpy(buf, "MULOP");             break;
        case DOT:                strcpy(buf, "DOT");               break;
        case SEMICOLON:          strcpy(buf, "SEMICOLON");         break;
        case COLON:              strcpy(buf, "COLON");             break;
        case OPEN_PARENTHESIS:   strcpy(buf, "OPEN_PARENTHESIS");  break;
        case CLOSE_PARENTHESIS:  strcpy(buf, "CLOSE_PARENTHESIS"); break;
        case SPACE:              strcpy(buf, "SPACE");             break;
        case TAB:                strcpy(buf, "TAB");               break;
        case ASSIGNOP:           strcpy(buf, "ASSIGNOP");          break;
        default:                 strcpy(buf, "INVALID_TOKEN");
    }

    return buf;
}

char* get_attributes(token_t token_type, char* token) {
    #define MAX_ATTR_LEN (35)
    static char buf[MAX_ATTR_LEN];

    switch (token_type) {
        case RELOP:
            if (!strcmp(token, ">")) {
                strcpy(buf, "GT");
            } else if (!strcmp(token, "<")) {
                strcpy(buf, "ST");
            } else if (!strcmp(token, "=")) {
                strcpy(buf, "EQ");
            } else {
                strcpy(buf, token);
            }
            break;
        case NUM:
            sprintf(buf, "Type=integer, val=%s", token);
            break;
        case ID:
            sprintf(buf, "ID=%s, Length=%ld", token, strlen(token));
            break;
        default:
            sprintf(buf, " ");
    }

    return buf;
}

void print_token_to_token_file(FILE* tok_file, token_t current_token, char* token) {
    center_print(tok_file, token, LEXEME_FIELD_LEN);
    fputc('|', tok_file);
    center_print(tok_file, token_to_string(current_token), TOKEN_FIELD_LEN);
    fputc('|', tok_file);
    center_print(tok_file, get_attributes(current_token, token), ATTR_FIELD_LEN);
    fputc('\n', tok_file);
}

int is_whitespace_token(token_t token_type) {
    return token_type == SPACE || token_type == ENTER || token_type == TAB;
}

void print_error_message(FILE* lst_file, char* error_token) {
    fprintf(lst_file, "\n---------------- SYNTAX ERROR ---------------\n");
    fprintf(lst_file, "\t\tInvalid token: %s\n", error_token);
    fprintf(lst_file, "---------------------------------------------\n");
}

void handle_token(FILE* tok_file, FILE* lst_file, token_t current_token, char* token) {
    if (current_token == INVALID_TOKEN) {
        print_error_message(lst_file, token);
    } else if (!is_whitespace_token(current_token)){
        print_token_to_token_file(tok_file, current_token, token);
    }
}
