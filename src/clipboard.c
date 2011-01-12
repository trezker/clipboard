#include "clipboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#ifdef FILENAME_SIZE
#error FILENAME_SIZE is already defined... :(
#endif

// 20 for filename, 1 for terminating character.
#define FILENAME_SIZE 21

#ifdef COMMAND_SIZE
#error COMMAND_SIZE is already defined... :(
#endif

// 20 for filename, 25 for command line, 1 for terminating character.
#define COMMAND_SIZE 46

typedef struct
{
    int fd;
    FILE * file;
    char filename[FILENAME_SIZE];
} clipboard_temp_file_t_;

const clipboard_temp_file_t_ NULL_TEMP_FILE_ = {0};

int clipboard_temp_file_equals_(
        const clipboard_temp_file_t_ * const lhs,
        const clipboard_temp_file_t_ * const rhs)
{
    assert(lhs);
    assert(rhs);

    return lhs->fd == rhs->fd &&
            lhs->file == rhs->file &&
            strncmp(lhs->filename, rhs->filename, FILENAME_SIZE - 1) == 0;
}

clipboard_temp_file_t_ clipboard_open_temp_file_(void)
{
    const char * const filename_pattern = "clipboard.tmp.XXXXXX";
    clipboard_temp_file_t_ temp_file = {0};

    memcpy(
            temp_file.filename,
            filename_pattern,
            strlen(filename_pattern) + 1);

    temp_file.fd = mkstemp(temp_file.filename);

    if(!temp_file.fd)
    {
        perror("Failed to open temp file");
        return NULL_TEMP_FILE_;
    }

    temp_file.file = fdopen(temp_file.fd, "r+");

    if(!temp_file.file)
    {
        perror("Failed to open file descriptor");

        if(close(temp_file.fd != 0))
            perror("Failed to close file descriptor");

        return NULL_TEMP_FILE_;
    }

    return temp_file;
}

void clipboard_close_temp_file_(clipboard_temp_file_t_ temp_file)
{
    if(temp_file.file)
        fclose(temp_file.file);

    if(temp_file.fd)
        close(temp_file.fd);
}

int Set_clipboard_text(const char * const text, const int size)
{
    int len;
    int status = 0;

    clipboard_temp_file_t_ temp_file = clipboard_open_temp_file_();

    if(clipboard_temp_file_equals_(&temp_file, &NULL_TEMP_FILE_))
        return 0;

    char * buffer = (char *)malloc(sizeof(char) * size);

    if(!buffer)
    {
        perror("Out of memory");
        goto cleanup;
    }

    // Copy maximum of size bytes to buffer of known size.
    memcpy(buffer, text, size);

    // Ensure buffer is terminated.
    buffer[size] = '\0';

    len = strlen(buffer);

    if(fwrite(buffer, sizeof(char), len, temp_file.file) != len)
        goto cleanup;

    fflush(temp_file.file);

    char command[COMMAND_SIZE];

    snprintf(
            command,
            COMMAND_SIZE,
            "cat %s | xclip -selection c",
            temp_file.filename);

    status = system(command) == 0;

cleanup:
    free(buffer);
    clipboard_close_temp_file_(temp_file);
    if(strlen(temp_file.filename) > 0)
        unlink(temp_file.filename);

    return status;
}

const char * Get_clipboard_text()
{
    char buffer[BUFSIZ];
    char command[COMMAND_SIZE];
    char * text = 0;
    int text_size = BUFSIZ;
    int total_len = 0;
    clipboard_temp_file_t_ temp_file = clipboard_open_temp_file_();

    if(clipboard_temp_file_equals_(&temp_file, &NULL_TEMP_FILE_))
        return (const char *)0;

    snprintf(
            command,
            COMMAND_SIZE,
            "xclip -o -selection c > %s",
            temp_file.filename);

    if(system(command) != 0)
        return (const char *)0;

    text = (char *)malloc(sizeof(char) * text_size);

    if(!text)
    {
        perror("Out of memory");
        goto cleanup;
    }

    text[0] = '\0';

    int len;

    do
    {
        len = fread(buffer, sizeof(char), BUFSIZ - 1, temp_file.file);

        if(len)
        {
            if(total_len > 0)
            {
                text = (char *)realloc(text, sizeof(char) * text_size);

                if(!text)
                {
                    perror("Out of memory");
                    goto cleanup;
                }
            }

            memcpy(text + total_len, buffer, len);
            total_len += len;
            *(text + total_len) = '\0';
        }
        else if(ferror(temp_file.file))
        {
            perror("Failed to read temp file");
            free(text);
            goto cleanup;
        }
    }while(len == BUFSIZ - 1);

cleanup:
    unlink(temp_file.filename);

    return text;
}

