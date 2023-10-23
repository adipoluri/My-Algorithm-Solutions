#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/** Remove any leading and trailing < > brackets around name
 *
 * Parameters: name:  The name from which to remove any brackets
 *
 * Returns: Either the orginal name if it is not surrounded by 
 * brackets, or the name without the brackets.
 *
 * Examples:
 *     trim_angle_brackets("norm@cs.ubc.ca") returns "norm@cs.ubc.ca"
 *     trim_angle_brackets("<norm@cs.ubc.ca>") returns "norm@cs.ubc.ca"
 *     trim_angle_brackets("<norm@cs.ubc.ca") returns "<norm@cs.ubc.ca"
 **/
char *trim_angle_brackets(char *name) {
    if (name[0] == '<' && name[strlen(name) - 1] == '>') {
	name[strlen(name) - 1] = '\0';
	return &name[1];
    }
    return name;
}

/**
 *  Split a line into individual parts separated by white space
 *
 *  Parameters: line:   The line of text to split
 *                      The characters in the line will be modified by the call.
 *              parts:  An array of char * pointers that will receive the
 *                      pointers to the individual parts of the input line. 
 *                      This array must be long enough to hold all of the
 *                      parts of the line.
 **/
int split(char *buf, char *parts[]) {
    static char *spaces = " \t\r\n";
    int i = 1;
    parts[0] = strtok(buf, spaces);
    do {
        parts[i] = strtok(NULL, spaces);
    } while (parts[i++] != NULL);
    return i - 1;
}

int be_verbose = 1;

/**
 * Print a log message to the standard error stream, if be_verbose is 1
 *
 * Parameters: fmt:     A printf-line formating string
 *
 **/
void dlog(const char *fmt, ...) {
    va_list args;
    if (be_verbose) {
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}

int send_formatted(int fd, const char *fmt, ...) {
    int bufsize = 128;
    char *buf = malloc(bufsize);
    va_list args;
    int strsize;

    // Start with string length, increase later if needed
    if (bufsize < strlen(fmt) + 1) {
        bufsize = roundup(strlen(fmt) + 1, 128);
        buf = realloc(buf, bufsize);
    }

    while (1) {

        va_start(args, fmt);
        strsize = vsnprintf(buf, bufsize, fmt, args);
        va_end(args);

        if (strsize < 0)
            return -1;

        // If buffer was enough to fit entire string, send it
        if (strsize <= bufsize)
            break;

        // Try again with more space
        bufsize = roundup(strsize, 128);
        buf = realloc(buf, bufsize);
    }

    int sent_size = send_all(fd, buf, strsize);
    free(buf);
    return sent_size;
}

int send_all(int fd, char buf[], size_t size) {

    size_t rem = size;
    while (rem > 0) {
        int rv = send(fd, buf, rem, MSG_NOSIGNAL);
        // If there was an error, interrupt sending and returns an error
        if (rv <= 0)
            return rv;
        buf += rv;
        rem -= rv;
    }
    return size;
}

int roundup(int val, int chunksize) {
    return ((val + chunksize - 1) / chunksize) * chunksize;
}
