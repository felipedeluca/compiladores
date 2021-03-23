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


char tokens[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '+', '-', '*', '/', '(', ')', '%', '^',
    ' '
};


char *token_names[] = {
    "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Zero",
    "Plus", "Minus", "Multiplier", "Divider", "Left_parenthesis", "Right_parenthesis", "Remainder", "Power",
    "Blank"
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


int token_exists( char t ) {
    int list_size = sizeof( tokens );
    for ( int i = 0; i < list_size; i++ ) {
        if (t == tokens[i]) {
            return i;
        }
    }
    return -1;
} 


/*
 Complexity O(mn)
 No optimizations at all!
 Blank spaces are discarded.
*/
int parse( char *str ) {
    printf( "\n\n--> Parsing string: %s\n\n", str );

    int invalid_tokens = 0;
    int str_size = strlen( str );

    printf("RESULT: \n\n");
    for ( int i = 0; i < str_size; i++ ) {
        int token_index = token_exists( str[i] );
        if ( token_index <= -1 ) {
            printf( "UNKNOWN( '%c' ), ", str[i] );
            invalid_tokens += 1;
        } else {
            if (str[i] != ' ') {
                printf( "%s( '%c' ), ", token_names[token_index], str[i] );
            }
        }
    }

    printf( "\n\n---------- Parsing completed.\n" );
    return invalid_tokens;
}
