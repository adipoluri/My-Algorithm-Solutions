#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>

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
extern char *trim_angle_brackets(char *name);

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
extern int   split(char *buf, char *parts[]);

extern int   be_verbose;
/**
 * Print a log message to the standard error stream, if be_verbose is 1
 *
 * Parameters: fmt:     A printf-line formating string
 *
 * The __attribute__ in this function allows the compiler to provide
 * useful warnings when compiling the code.
 **/
extern void  dlog(const char *fmt, ...)
		__attribute__ ((format(printf, 1, 2)));


/** Sends a printf-style formatted string to a socket descriptor. The
 *  string can contain format directives (e.g., %d, %s, %u), which
 *  will be translated using the same rules as printf. For example,
 *  you may call it like:
 *
 *  send_formatted(fd, "+OK Server ready\r\n");
 *  send_formatted(fd, "+OK %d messages found\r\n", msg_count);
 *
 *  Parameters: fd: Socket file descriptor.
 *              fmt: String to be sent, including potential
 *                   printf-like format directives.
 *              additional parameters based on string format.
 *
 *  Returns: If the string was successfully sent, returns
 *           the number of bytes sent. Otherwise, returns -1.
 * The __attribute__ in this function allows the compiler to provide
 * useful warnings when compiling the code.
 */
int         send_formatted(int fd, const char *str, ...)
__attribute__ ((format(printf, 2, 3)));

/** Sends a buffer of data, until all data is sent or an error is
 *  received. This function is used to handle cases where send is able
 *  to send only part of the data. If this is the case, this function
 *  will call send again with the remainder of the data, until all
 *  data is sent.
 *
 *  Data is sent using the MSG_NOSIGNAL flag, so that, if the
 *  connection is interrupted, instead of a PIPE signal that crashes
 *  the program, this function will be able to return an error that
 *  can be handled by the caller.
 *
 *  Parameters: fd: Socket file descriptor.
 *              buf: Buffer where data to be sent is stored.
 *              size: Number of bytes to be used in the buffer.
 *
 *  Returns: If the buffer was successfully sent, returns
 *           size. Otherwise, returns -1.
 */
int send_all(int fd, char buf[], size_t size);

/**
 * return val rounded up to be a multiple of chunksize.
 */
int roundup(int val, int chunksize);

#endif