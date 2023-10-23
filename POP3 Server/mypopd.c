#include "netbuffer.h"
#include "mailuser.h"
#include "server.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

typedef enum state {
    Undefined,
    AUTHORIZATION,
    TRANSACTION,
} State;

typedef struct serverstate {
    int fd;
    net_buffer_t nb;
    char recvbuf[MAX_LINE_LENGTH + 1];
    char *words[MAX_LINE_LENGTH];
    int nwords;
    State state;
    struct utsname my_uname;
    char username[MAX_LINE_LENGTH];
    mail_list_t mail_list;
} serverstate;

static void handle_client(void *new_fd);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments. Expected: %s <port>\n", argv[0]);
        return 1;
    }
    run_server(argv[1], handle_client);
    return 0;
}

// syntax_error returns
//   -1 if the server should exit
//    1 otherwise
int syntax_error(serverstate *ss) {
    if (send_formatted(ss->fd, "-ERR %s\r\n", "Syntax error in parameters or arguments") <= 0) return -1;
    return 1;
}

// checkstate returns
//   -1 if the server should exit
//    0 if the server is in the appropriate state
//    1 if the server is not in the appropriate state
int checkstate(serverstate *ss, State s) {
    if (ss->state != s) {
        if (send_formatted(ss->fd, "-ERR %s\r\n", "Bad sequence of commands") <= 0) return -1;
        return 1;
    }
    return 0;
}

// All the functions that implement a single command return
//   -1 if the server should exit
//    0 if the command was successful
//    1 if the command was unsuccessful

int do_quit(serverstate *ss) {
    // Note: This method has been filled in intentionally!
    dlog("Executing quit\n");
    send_formatted(ss->fd, "+OK Service closing transmission channel\r\n");
    ss->state = Undefined;
    return -1;
}

int do_user(serverstate *ss) {
    dlog("Executing user\n");
    if(ss->words[1] == NULL) {
        send_formatted(ss->fd, "-ERR that's weird, never heard of that mailbox name\r\n");
        return 1;
    }
    if(is_valid_user(ss->words[1],NULL)){
        strcpy(ss->username, ss->words[1]);
        dlog("Username is %s\n", ss->username);
        send_formatted(ss->fd, "+OK User is valid, proceed with password\r\n");
        return 0;
    } else {
        send_formatted(ss->fd, "-ERR username does not exist\r\n");
        return 1;
    }
    
}


int do_pass(serverstate *ss) {
    dlog("Executing pass\n");
    if(ss->username == NULL) {
        send_formatted(ss->fd, "-ERR %s\r\n", "Bad sequence of commands");
        return 1;
    } if(ss->words[1]== NULL) {
        send_formatted(ss->fd, "-ERR invalid password\r\n");
        return 1;
    } else {
        dlog("Username is %s, Password is %s\n", ss->username, ss->words[1]);
        if(is_valid_user(ss->username,ss->words[1])){
            send_formatted(ss->fd, "+OK Password is valid, mail loaded\r\n");
            ss->state = TRANSACTION;
            ss->mail_list = load_user_mail(ss->username);
            return 0;
        } else {
            send_formatted(ss->fd, "-ERR invalid password\r\n");
            //ss->username = NULL;
            return 1;
        }
    }
}

int do_stat(serverstate *ss) {
    dlog("Executing stat\n");
    send_formatted(ss->fd, "+OK %x %zd\r\n",mail_list_length(ss->mail_list, 0), mail_list_size(ss->mail_list));
    return 0;
}

int do_list(serverstate *ss) {
    dlog("Executing list\n");

    //Get count of all Mail (including deleted)
    int mailCount = mail_list_length(ss->mail_list, 1);

    //No arg ( LIST )
    if(ss->words[1] == NULL) {
        //Send Okay message
        send_formatted(ss->fd, "+OK %x messages (%zd octects)\r\n",mail_list_length(ss->mail_list, 0), mail_list_size(ss->mail_list));
        
        //Iterate through mail and print size if not deleted
        for(int i = 0; i < mailCount; i++) {
            if(mail_list_retrieve(ss->mail_list,i) != NULL) {
                send_formatted(ss->fd, "%x %zd\r\n", i+1, mail_item_size(mail_list_retrieve(ss->mail_list,i)));
            }
        }
        //Send finishing .
        send_formatted(ss->fd, ".\r\n");

        return 0;
    } 

    //Arg included ( List 1)
    //Set Input to int val of arg
    int input = atoi(ss->words[1]);
    if(input > mailCount) { 
        //send error if arg > mail count
        send_formatted(ss->fd, "-ERR no such message, only %x messages in maildrop\r\n",mail_list_length(ss->mail_list, 0));
    } else {
        //send OK message with sizes if mail exists and is not deleted 
        if(mail_list_retrieve(ss->mail_list,input-1) != NULL) {
            send_formatted(ss->fd, "+OK %x %zd\r\n", input, mail_item_size(mail_list_retrieve(ss->mail_list,input-1)));
        } else {
            //Send error if message has been deleted
            send_formatted(ss->fd, "-ERR no such message\r\n");
        }
    }
    return 0;
}

int do_retr(serverstate *ss) {
    dlog("Executing retr\n");
    
    if(ss->words[1] == NULL) {
        //Send Error msg if no arg included
        send_formatted(ss->fd, "-ERR no such message\r\n");
        return 1;
    } 

    //Arg included ( RETR 1)
    //Set Input to int val of arg
    int input = atoi(ss->words[1]);
    if(mail_list_retrieve(ss->mail_list,input-1) != NULL) {
        FILE * file = mail_item_contents(mail_list_retrieve(ss->mail_list,input-1));
        char line[MAX_LINE_LENGTH];

        if (file == NULL) {
            send_formatted(ss->fd, "-ERR mail file invalid\r\n");
            return 1;
        }
        send_formatted(ss->fd, "+OK Message follows\r\n");

        // Read and print each line
        while (fgets(line, sizeof(line), file) != NULL) {
            send_formatted(ss->fd, "%s", line);
        }
        
        send_formatted(ss->fd, ".\r\n");
        fclose(file);
    } else {
        //Send error if message has been deleted
        send_formatted(ss->fd, "-ERR no such message\r\n");
        return 1;
    }
    return 0;
}

int do_rset(serverstate *ss) {
    dlog("Executing rset\n");
    int n = mail_list_undelete(ss->mail_list);
    send_formatted(ss->fd, "+OK %x messages restored\r\n",n);
    return 0;
}

int do_noop(serverstate *ss) {
    dlog("Executing noop\n");
    send_formatted(ss->fd, "+OK (noop)\r\n");
    return 0;
}

int do_dele(serverstate *ss) {
    dlog("Executing dele\n");
    
    //No arg send error
    if(ss->words[1] == NULL) { //no arg 
        send_formatted(ss->fd, "-ERR no such message\r\n");
        return 0;   
    } 

    int mailCount = mail_list_length(ss->mail_list, 1);
    int input = atoi(ss->words[1]);
    if(input > mailCount) { // input larger than total mail count
        send_formatted(ss->fd, "-ERR no such message, only %x messages in maildrop\r\n",mail_list_length(ss->mail_list, 0));
    } else { 
        //error if message was lready deleted, otherwise OK
        if(mail_list_retrieve(ss->mail_list,input-1) != NULL) {
            mail_item_delete(mail_list_retrieve(ss->mail_list, input-1));
            send_formatted(ss->fd, "+OK message %x deleted\r\n", input);
        } else {
            send_formatted(ss->fd, "-ERR message %x already deleted\r\n", input);
        }
    }
    return 0;
}

void handle_client(void *new_fd) {
    int fd = *(int *)(new_fd);

    size_t len;
    serverstate mstate, *ss = &mstate;

    ss->fd = fd;
    ss->nb = nb_create(fd, MAX_LINE_LENGTH);
    ss->state = AUTHORIZATION;
    uname(&ss->my_uname);

    // TODO: Initialize additional fields in `serverstate`, if any
    if (send_formatted(fd, "+OK POP3 Server on %s ready\r\n", ss->my_uname.nodename) <= 0) return;

    while ((len = nb_read_line(ss->nb, ss->recvbuf)) >= 0) {
        if (ss->recvbuf[len - 1] != '\n') {
            // command line is too long, stop immediately
            send_formatted(fd, "-ERR Syntax error, command unrecognized\r\n");
            break;
        }
        if (strlen(ss->recvbuf) < len) {
            // received null byte somewhere in the string, stop immediately.
            send_formatted(fd, "-ERR Syntax error, command unrecognized\r\n");
            break;
        }
        // Remove CR, LF and other space characters from end of buffer
        while (isspace(ss->recvbuf[len - 1])) ss->recvbuf[--len] = 0;

        dlog("%x: Command is %s\n", fd, ss->recvbuf);
        if (strlen(ss->recvbuf) == 0) {
            send_formatted(fd, "-ERR Syntax error, blank command unrecognized\r\n");
            continue;
        }
        
        // Split the command into its component "words"
        ss->nwords = split(ss->recvbuf, ss->words);
        char *command = ss->words[0];
        /* TODO: Handle the different values of `command` and dispatch it to the correct implementation
         *  TOP, UIDL, APOP commands do not need to be implemented and therefore may return an error response */

        if (strcmp(command, "USER") == 0) 
        {
            if(checkstate(ss, AUTHORIZATION) != 0) {
                continue;
            }

            do_user(ss);
        } 
        else if (strcmp(command, "PASS") == 0) 
        {
            if(checkstate(ss, AUTHORIZATION) != 0) {
                continue;
            }            
            
            do_pass(ss);
        }
        else if (strcmp(command, "STAT") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }
            do_stat(ss);
        }
        else if (strcmp(command, "LIST") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }

            do_list(ss);
        } 
        else if (strcmp(command, "RETR") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }

            do_retr(ss);
        } 
        else if (strcmp(command, "DELE") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }

            do_dele(ss);
        } 
        else if (strcmp(command, "RSET") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }

            do_rset(ss);
        } 
        else if (strcmp(command, "NOOP") == 0) 
        {
            if(checkstate(ss, TRANSACTION) != 0) {
                continue;
            }
            
            do_noop(ss);
        }
        else if (strcmp(command, "QUIT") == 0) 
        {
            //Call do_quit and break while loop
            if(do_quit(ss) == -1) {
                break;
            }
        }     
        else /* default: */
        {
            send_formatted(fd, "-ERR Syntax error, command unrecognized\r\n");
            continue;
        }
    }
    // TODO: Clean up fields in `serverstate`, if required
    mail_list_destroy(ss->mail_list);
    nb_destroy(ss->nb);
    close(fd);
    free(new_fd);
}
