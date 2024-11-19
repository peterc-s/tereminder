#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "debug.h"

#define MAX_FILE_SIZE 1024 * 1024 * 128 // 128MB

char* read_file(char* file_name) {
    // open source file
    trace("Opening file `%s`\n", file_name);
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        error("%s (%s).\n", strerror(errno), file_name);
        exit(errno);
    };

    // get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // check file is not too large
    if (file_size > MAX_FILE_SIZE) {
        error("File `%s` too large.\n", file_name);
        trace("Closing file `%s`.\n", file_name);
        fclose(file);
        exit(EXIT_FAILURE);
    };

    // allocate memory for the file array
    trace("Attempting to malloc %d bytes for contents of %s\n", file_size + 1, file_name);
    char *file_buffer = (char *)malloc(file_size + 1);
    if (!file_buffer) {
        error("%s (%s).\n", strerror(errno), file_name);
        trace("Closing file `%s`.\n", file_name);
        fclose(file);
        exit(errno);
    };

    // read file contents
    size_t bytes_read = fread(file_buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        error("Partial file read of file `%s` occurred.\n", file_name);
        trace("Closing file `%s`.\n", file_name);
        fclose(file);
        trace("Freeing file buffer.\n");
        free(file_buffer);
        exit(EXIT_FAILURE);
    };

    // close file
    trace("Closing file `%s`.\n", file_name);
    fclose(file);

    // null terminate
    file_buffer[file_size] = '\0';

    return file_buffer;
}
