/*
A VERY NAIVE PARSER V. 0.1.0

Author:
    Felipe R. de Luca

Description:
    Detects whether a string contains valid mathematical expression symbols or not.

Compile & run:
    gcc naive-parser.c -o naive-parser -std=c99

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parse( char *str );


char token_number_type[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};

char token_operator_type[] = {
    '+', '-', '*', '/', '%', '^'
};

char token_punctuation_type[] = {
    '(', ')', ' '
};

enum token_type {
    Unknown = 0, Number, Operator, Punctuation
};

char *token_type_name[] = {
    "Unknown", "Number", "Operator", "Punctuation"
};


int main( int argc, char *argv[] ) {
    char *str1 = "x +  1*2+(5^6a)/89 - 4 // 0";
    int invalid_tokens = parse( str1 );

    if (invalid_tokens > 0) {
        printf("\n The string contains %d invalid token(s)\n\n", invalid_tokens);
    } else {
        printf( "\n The string is valid!\n\n" );
    }

    return EXIT_SUCCESS;
}


/*
Input: a token
Output: token type.
*/
int token_exists( char t ) {
    int list_size = sizeof( token_number_type );
    for ( int i = 0; i < list_size; i++ ) {
        if (t == token_number_type[i]) {
            return Number;
        }
    }

    list_size = sizeof( token_operator_type );
    for ( int i = 0; i < list_size; i++ ) {
        if (t == token_operator_type[i]) {
            return Operator;
        }
    }

    list_size = sizeof( token_punctuation_type );
    for ( int i = 0; i < list_size; i++ ) {
        if (t == token_punctuation_type[i]) {
            return Punctuation;
        }
    }

    return Unknown;
} 


/*
Input: a string
Output: number of invalid tokens found.

Blank spaces are discarded.
*/
int parse( char *str ) {
    printf( "\n\n--> Parsing string: %s\n\n", str );

    int invalid_tokens = 0;
    int str_size = strlen( str );

    printf("RESULT: \n\n");
    for ( int i = 0; i < str_size; i++ ) {
        int token_type = token_exists( str[i] );
        if ( token_type == Unknown )
            invalid_tokens += 1;
        if (str[i] != ' ')
            printf( "%s( '%c' ), ", token_type_name[token_type], str[i] );
    }

    printf( "\n\n---------- Parsing completed.\n" );
    return invalid_tokens;
}
