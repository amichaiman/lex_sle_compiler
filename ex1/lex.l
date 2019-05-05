%{
#include "lexical_analyzer.h"
#include <stdlib.h>
int line_num=1;
%}

%x MULTI_LINE_COMMENT

DIGIT  [0-9]
LETTER [a-zA-Z]

%%
"/*"                            { BEGIN(MULTI_LINE_COMMENT); }
<MULTI_LINE_COMMENT>"*/"        { BEGIN(INITIAL); }
<MULTI_LINE_COMMENT>.           { }
<MULTI_LINE_COMMENT><<EOF>>     { fprintf(stderr, "ERROR: EOF detected in multi line comment!"); exit(1); }
"//".*\n                        { }
start                           { ECHO; return START; }
do                              { ECHO; return DO; }
else                            { ECHO; return ELSE; }
if                              { ECHO; return IF; }
endi                            { ECHO; return ENDI; }
int                             { ECHO; return INT; }
put                             { ECHO; return PUT; }
prog                            { ECHO; return PROG; }
get                             { ECHO; return GET; }
real                            { ECHO; return REAL; }
then                            { ECHO; return THEN; }
var                             { ECHO; return VAR; }
loop                            { ECHO; return LOOP; }
endl                            { ECHO; return ENDL; }
until                           { ECHO; return UNTIL; }
endp                            { ECHO; return ENDP; }
{LETTER}({LETTER}|{DIGIT})*     { ECHO; return ID; }
{DIGIT}+                        { ECHO; return NUM; }
">"|"<"|"<>"|"="                { ECHO; return RELOP; }
"&"|"~"                         { ECHO; return LOGOP; }
"+"|"-"                         { ECHO; return ADDOP; }
"*"|"/"                         { ECHO; return MULOP; }
"<="                            { ECHO; return ASSIGNOP; }
"."                             { ECHO; return DOT; }
","                             { ECHO; return COMMA; }
";"                             { ECHO; return SEMICOLON; }
":"                             { ECHO; return COLON; }
"("                             { ECHO; return OPEN_PARENTHESIS; }
")"                             { ECHO; return CLOSE_PARENTHESIS; }
" "                             { ECHO; return SPACE; }
"\t"                            { ECHO; return TAB; }
"\n"                            { ECHO; fprintf(yyout, "%d. ", line_num++); return ENTER; }
.                               { ECHO; return INVALID_TOKEN; }
%%


int main(int argc, char **argv) {
    if (argc < 2 || !valid_file_name(argv[1])) {
        fprintf(stderr, "USAGE: ./<exe> <filename>.<sle/SLE>\n");
        exit(1);
    }

    FILE* tok_file;
    FILE* lst_file;
    token_t current_token;

    create_open_output_files(&tok_file, &lst_file, argv[1]);

    yyin = fopen(argv[1], "r");

    if (!yyin || !tok_file || !lst_file) {
        fprintf(stderr, "error opening file");
        exit(1);
    }

    init_tok_file(tok_file);

    yyout = lst_file;
    fprintf(yyout, "%d. ", line_num++);

    while ((current_token = yylex())) {
        handle_token(tok_file, lst_file, current_token, yytext);
    }
}
int yywrap() { return 1; }
