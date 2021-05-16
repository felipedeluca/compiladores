/*
    Brainflakes - a simple single-file Brainfuck 8-bit memory block implementation.

    Works with 8-bit unsigned int compatible Brainfuck algorithm.
    To compile: gcc -Wall bf.c -o bf -std=c99
    Run: ./bf filename.ext

    Author: Felipe R. de Luca
    Date: May, 2021.

    Disclaimer: 
        This implementation was done as part of my Computer Science compiler classes exercises so bugs
        and unoptimized parts of the code is expected.

    Sources:
        https://esolangs.org/wiki/Brainfuck
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ascending ordered list according to symbols value */
const char language_symbol[] = {'+', ',', '-', '.', '<', '>', '[', ']'};

typedef uint8_t mem_cell_size_t;

#define MEM_CELL_COUNT 30000
#define FILE_BUFF_SIZE 4096 /* Max file length in characters */


struct BFile {
    char buffer[FILE_BUFF_SIZE];
    FILE* file;
} bf_file;


struct Tape {
    char* content;
    int ptr;
    size_t length;
} bf_code_tape;


struct LocalMemory {
    mem_cell_size_t* data;
    size_t ptr;
    size_t size;
} local_memory;


enum Command {
    CmdMoveLeft = 0,
    CmdMoveRight = 1,
    CmdIncrementValue = 2,
    CmdDecrementValue = 3,
    CmdPrintChar = 4,
    CmdInputChar = 5
};


enum ErrorCode {
    ErrMemOverflow = 1,
    ErrTapeNotInitialized = 2,
    ErrMemNotInitialized = 3,
    ErrNoMatchingLoopStart = 4,
    ErrNoMatchingLoopEnd = 5,
    ErrOpenInputFile = 105,
    ErrNoBFCodeFound = 200
};


void print_error(enum ErrorCode err) {
    if (err != 0) {
        printf("\n(!) ERROR: ");
    }
    switch (err) {
        case ErrMemOverflow:
            printf("Memory overflow.\n");
            printf("\t Memory size: %lu\n", local_memory.size);
            printf("\t Memory pointer: %zu\n", local_memory.ptr);
        break;
        case ErrTapeNotInitialized: printf("Tape not initialized.\n");          break;
        case ErrMemNotInitialized:  printf("Memory not initialized.\n");        break;
        case ErrNoMatchingLoopStart:printf("No matching loop start.\n");        break;
        case ErrNoMatchingLoopEnd:  printf("No matching loop end.\n");          break;
        case ErrOpenInputFile:      printf("Error opening input file.\n");      break;
        case ErrNoBFCodeFound:      printf("File content is not BF code.\n");   break;
    }
}


int cmd_loop(void) {
    /* Current position on the tape should be at ']' symbol. */
    int bracket_count = 1;

    /* Loop ended. */
    if (local_memory.data[local_memory.ptr] == 0)
        return 0;

    while (bf_code_tape.ptr >= 0) {
        bf_code_tape.ptr--;

        if (bf_code_tape.content[bf_code_tape.ptr] == ']')
            bracket_count++;

        if (bf_code_tape.content[bf_code_tape.ptr] == '[') {
            bracket_count--;
            if (bracket_count == 0)
                break;
        }
    }

    return 0;
}


int cmd_change_value(enum Command cmd) {
    if (cmd == CmdIncrementValue) {
        local_memory.data[local_memory.ptr]++;
    }
    else if (cmd == CmdDecrementValue) {
        local_memory.data[local_memory.ptr]--;
    }

    return 0;
}


int cmd_move_mem_ptr(enum Command cmd) {
    if (cmd == CmdMoveRight) {
        if (local_memory.ptr > local_memory.size)
            return ErrMemOverflow;
        else
            local_memory.ptr++;
    }
    else if (cmd == CmdMoveLeft) {
        if (local_memory.ptr < 0)
            return ErrMemOverflow;
        else
            local_memory.ptr--;
    }

    return 0;
}


int cmd_input_char(enum Command cmd) {
    if (cmd == CmdInputChar) {
        unsigned char c;
        printf("Enter a character: ");
        c = getchar();
        getchar();
        local_memory.data[local_memory.ptr] = c;
    }
    return 0;
}


int cmd_print(enum Command cmd) {
    if (cmd == CmdPrintChar) {
        mem_cell_size_t v = local_memory.data[local_memory.ptr];
        printf("%c", v);
    }
    return 0;
}


int execute_command(char symbol) {
    int err = 0;

    switch(symbol){
        case '>':
            err = cmd_move_mem_ptr(CmdMoveRight);
        break;
        case '<':
            err = cmd_move_mem_ptr(CmdMoveLeft);
        break;
        case '+':
            err = cmd_change_value(CmdIncrementValue);
        break;
        case '-':
            err = cmd_change_value(CmdDecrementValue);
        break;
        case ']':
            err = cmd_loop();
        break;
        case '.':
            err = cmd_print(CmdPrintChar);
        break;
        case ',':
            err = cmd_input_char(CmdInputChar);
        break;
    }

    if (err <= 0)
        bf_code_tape.ptr++;

    return err;
}


int run(void) {
    printf("\nExecuting program...\n\n");
    if (bf_code_tape.content == NULL)
        return ErrTapeNotInitialized;

    if (local_memory.data == NULL)
        return ErrMemNotInitialized;

    mem_cell_size_t instructions_count = 0;
    while(bf_code_tape.ptr >= 0 && bf_code_tape.ptr < bf_code_tape.length) {
        int err = execute_command(bf_code_tape.content[bf_code_tape.ptr]);
        if (err > 0) {
            print_error(err);
            break;
        }
        instructions_count++;
    }

    printf("\n-------------------------------------------------");
    printf("\nExecution finished. Total instructions executed: %d", instructions_count);

    return 0;
}


int binary_search(int start, int end, int v) {
     if (start == end) {
        if (language_symbol[end] == v)
            return 1;
        return 0;
    }

    int result = 0;
    int mid = (start + end) / 2;
    if (language_symbol[mid] > v)
        result = binary_search(start, mid, v);
    else if (language_symbol[mid] < v)
        result = binary_search(mid, end, v);
    else
        return 1;
    
    return result;
}


int is_valid_symbol(char s) {
    return binary_search(0, sizeof(language_symbol)-1, s);
}


size_t count_valid_symbol(const char* input_text) {
    size_t count, text_size;
    count = 0;
    text_size = strlen(input_text);

    for (int i = 0; i < text_size; i++) {
        if (is_valid_symbol(input_text[i]))
            count++;
    }

    return count;
}


/*
Input: BF source code.
Output: BF source code without invalid symbols.
*/
char* lexer(const char* source_code) {
    if (strlen(source_code) <= 0)
        return NULL;
    
    size_t size = strlen(source_code);
    size_t valid_symbol_count = count_valid_symbol(source_code);
    printf("Lexer results: text size = %zu \t valid symbols = %zu", size, valid_symbol_count);

    char* bf_code = (char*) malloc(valid_symbol_count * sizeof(char));
    
    size_t j = 0;
    for (int i = 0; i < size; i++) {
        if (is_valid_symbol(source_code[i])) {
            bf_code[j] = source_code[i];
            j++;
        }
    }

    return bf_code;
}


/*
Input: memory size in number of cells.
Output: error code.
*/
int init(const char* input_text, size_t mem_size) {
    printf("-> Initializing Brainflakes...\n");
    bf_code_tape.ptr = 0;
    local_memory.data = (mem_cell_size_t*) malloc(sizeof(mem_cell_size_t) * mem_size);

    if (local_memory.data == NULL)
        return ErrMemNotInitialized;

    local_memory.size = mem_size;
    local_memory.ptr = 0;
    memset(local_memory.data, 0, mem_size);

    bf_code_tape.content = lexer(input_text);
    if (bf_code_tape.content == NULL) {
        return ErrNoBFCodeFound;
    }

    bf_code_tape.length = strlen(bf_code_tape.content);

    printf("\n\nMemory cell size: %lu bits", sizeof(mem_cell_size_t) * 8);
    printf("\nMemory size: %lu bytes\n", sizeof(mem_cell_size_t) * MEM_CELL_COUNT);

    return 0;
}


int unload(void) {
    int err_count = 0;

    if (local_memory.data != NULL) {
        free(local_memory.data);
        local_memory.data = NULL;
        local_memory.size = 0;
        local_memory.ptr = 0;
    } else
        err_count += 1;

    if (bf_code_tape.content != NULL) {
        free(bf_code_tape.content);
        bf_code_tape.content = NULL;
        bf_code_tape.length = 0;
        bf_code_tape.ptr = 0;
    }
    else
        err_count += 1;

    return err_count;
}

/*
read_file_contents:
    Store file contents in bf_file.buffer.

Input: filename
Output: 0 if success and ErrOpenInputFile otherwise. 
*/
int read_file_contents(const char* filename) {
    printf("-> Reading file: %s\n", filename);
    bf_file.file = fopen(filename, "r");
    
    if (bf_file.file == NULL)
        return ErrOpenInputFile;

    size_t delta = 0;
    while (fgets(bf_file.buffer+delta, FILE_BUFF_SIZE, bf_file.file));
    fclose(bf_file.file);
    return 0;
}


int main(int argc, char* argv[] ) {
    int err = 0;

    if (argc < 2) {
        printf("\n\nInstructions:\n");
        printf("Brainflakes is compatible with unsigned 8-bit memory cell size.\n");
        printf("\n   To run Brainflakes type: ./bf <filename.ext>\n\n");
        return EXIT_FAILURE;
    }

    err = read_file_contents(argv[1]);
    if (err > 0) {
        print_error(err);
        return EXIT_FAILURE;
    }

    err = init(bf_file.buffer, MEM_CELL_COUNT);
    if (err > 0) {
        print_error(err);
        return EXIT_FAILURE;
    }

    run();

    printf("\n\n");

    if (unload() > 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
