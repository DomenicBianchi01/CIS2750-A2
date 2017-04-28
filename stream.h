#ifndef __DBIANCHI_STREAM__
#define __DBIANCHI_STREAM__

struct userPost {

    char * username;
    char * streamname;
    char * date;
    char * text;
};

typedef struct userPost userPost;

/**
 * updateStream
 * Adds post to stream and updates all relevant files
 * IN: userPost * st
 * OUT: None
 * POST: Post is added to the stream
 * ERROR: If the struct pointer (parameter) did not malloc correctly, and error may occur
 **/
void updateStream(struct userPost *st);

/**
 * addUser
 * Adds a user to one or more streams (gives permission to post)
 * IN: char *username, char *list
 * OUT: None
 * POST: User is given perssion to post to the specified streams
 * ERROR: If the user does not enter the list of streams correctly, an error with strtok may occur
 **/
void addUser(char *username, char *list);

/**
 * removeUser
 * Removes a user to one or more streams (gives permission to post)
 * IN: char *username, char *list
 * OUT: None
 * POST: User is revoked perssion to post to the specified streams
 * ERROR: If the user does not enter the list of streams correctly, an error with strtok may occur
 **/
void removeUser(char *username, char *list);

#endif