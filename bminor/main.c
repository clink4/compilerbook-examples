#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int backslash_codes(char *slash, char *str){
	/* determine slash not last character */
	if (slash == str + strlen(str) - 1) {
		return 1;
	}

	/* replace escape codes */
	char next = *(slash + sizeof(char));
	if (next == '0') {
		*slash = '\0';
	}
	else if (next == 'n') {
		*slash = '\n';
	}
	else {
		*slash = next;
	}

	/* back-copy the string */
	strcpy(slash + sizeof(char), slash + 2*sizeof(char));
	return 0;
}


int str_clean(char *new, int max_len) {
	int retval = 0;
	char *yycopy = strdup(yytext);

	/* remove quotes */
	yycopy++;
	yycopy[strlen(yycopy)-1] = '\0';

	/* check for newline without an escape */
	char *newline = strchr(yycopy, '\n');
	if (newline) {
		if (newline == yycopy || *(newline - sizeof(char)) != '\\') {
			yycopy--;
			free(yycopy);
			fprintf(stderr, "scan error: incorrectly formatted newline\n");
			return 1;
		}
	}

	/* replace backslash codes with literals */
	char *slash = strchr(yycopy, '\\');
	int slash_idx = abs((uintptr_t)slash - (uintptr_t)yycopy);
	int prev_idx = -1;
	while (slash && (slash_idx > prev_idx)) {
		if (backslash_codes(slash, yycopy)) {
			fprintf(stderr, "scan error: incorrectly formatted backslash code\n");
			retval = 1;
			break;
		}
		slash = strchr(slash, '\\');
		prev_idx = slash_idx;
		slash_idx = abs((uintptr_t)slash - (uintptr_t)yycopy);
	}

	/* check length */
	if (strlen(yycopy) > max_len) {
		fprintf(stderr, "scan error: input exceeds maximum length\n");
		retval = 1;
	}

	strcpy(new, yycopy);
	yycopy--;
	free(yycopy);
	return retval;
}

char * token_to_string( int token )
{
	char *token_names[50] = { "TOKEN_EOF", "TOKEN_ERROR", "KEYWORD", "ARRAY", "BOOLEAN", "CHAR",
	"ELSE", "FALSE", "FOR", "FUNCTION", "IF", "INTEGER", "PRINT", "RETURN", "STRING", "TRUE",
	"VOID", "WHILE", "IDENT", "INT_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "LPAREN", "RPAREN",
	"LBRACKET", "RBRACKET", "INCR", "DECR", "PLUS", "MINUS", "LESS_THAN", "LESS_THAN_OR_EQ", "GREATER_THAN",
	"GREATER_THAN_OR_EQ", "EQUAL", "NOT_EQUAL", "LOGICAL_AND", "LOGIAL_OR", "ASSIGNMENT", "LOGICAL_NOT",
	"EXPONENT", "MULT", "DIVISION", "MOD", "COLON", "COMMA", "SEMICOLON", "LBRACE", "RBRACE", "UNMATCHED_COMMENT"};
	char *str = strdup(token_names[token]);
	return str;
}


int main(int argc, char * argv[])
{
	/* parse command line arguments */
	if (argc != 3) {
		fprintf(stderr, "Usage: bminor -scan [filename]\n");
		return 1;
	}

	if (strcmp("-scan", argv[1])) {
		fprintf(stderr, "unrecognized argument \"%s\"\n", argv[1]);
		return 1;
	}

	char *file_name = argv[2];
	yyin = fopen(file_name, "r");
	if(!yyin) {
		fprintf(stderr, "could not open file \"%s\"\n", file_name);
		return 1;
	}

	int retval = 0;

	/* main loop to scan for tokens */
	while(1) {
		token_t t = yylex();
		char *token_type = token_to_string(t);

		if (t==TOKEN_EOF) break;

		if (t==TOKEN_ERROR) {
			fprintf(stderr, "scan error: Invalid token %s\n", yytext);
			retval = 1;
			if (token_type) {
				free(token_type);
			}
			break;
		}
		else if (t==TOKEN_STRING_LITERAL){
			char cleaned[BUFSIZ];
			if (!str_clean(cleaned, 255)) {
				printf("%s %s\n", token_type, cleaned);
			}
			else {
				retval = 1;
				if (token_type) {
					free(token_type);
				}
				break;
			}

		}
		else if (t == TOKEN_CHAR_LITERAL) {
			char cleaned[BUFSIZ];
			if (!str_clean(cleaned, 1)) {
				printf("%s %s\n", token_type, cleaned);
			}
			else {
				retval = 1;
				if (token_type) {
					free(token_type);
				}
				break;
			}
		}
		else if (t == TOKEN_IDENT) {
			if (strlen(yytext) > 256) {
				retval = 1;
				fprintf(stderr, "scan error: Identifier exceeds maximum length\n");
				if (token_type) {
					free(token_type);
				}
				break;
			}

			printf("%s %s\n", token_type, yytext);

		}
		else if (t == TOKEN_INT) {
				int64_t num = atoll(yytext);
				printf("%s %" PRId64 "\n", token_type, num);
		}
		else if (t == TOKEN_UNMATCHED_COMMENT) {
			fprintf(stderr, "scan error: Unmatched comment\n");
			retval = 1;
			if (token_type) {
				free(token_type);
			}
			break;
		}
		else {
			printf("%s\n", token_type);

		}

		if (token_type) {
			free(token_type);
		}

	}

	return retval;
}
