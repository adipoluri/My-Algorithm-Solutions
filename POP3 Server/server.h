/* server.h
 * Handles the creation of a server socket and data sending.
 * Author  : Jonatan Schroeder
 * Modified: Nov 6, 2021
 *
 * Modified by: Norm Hutchinson
 * Modified: Mar 5, 2022
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>

void        run_server(const char *port, void  (*handler)(void *));

#endif
