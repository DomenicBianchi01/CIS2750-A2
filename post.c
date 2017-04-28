/*Domenic Bianchi*/ 
/*CIS 2750 Assignment 2*/ 
/*February 19 , 2017*/ 
/*This program gets user input to create a new post*/ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include <time.h> 
#include "stream.h" 
#include "addauthor.h" 
struct postEntry {
    void(*postEntrysubmitPost)(); 
    void(*postEntrygetTimeAndDatec)(char*); 
    userPost*(*postEntryformatEntrycccc)(char*,char*,char*,char*); 
    void(*postEntryreadInputci)(char*,int); 
}; 
void postEntryreadInputci ( char input[] , int option ) {
    /*Get input for stream name*/ 
    if ( option == 1 ) {
        printf ( "stream: " ) ; 
        fgets ( input , 256 , stdin ) ; 
        input[strlen ( input ) -1] = '\0' ; 
    }
    /*Get input for post text*/ 
    else {
        char input2[512] ; clearArray ( input2 , 512 ) ; 
        printf ( "Enter text: " ) ; 
        while ( fgets ( input2 , 79 , stdin ) != NULL ) {
            /*If there is a new line character at the end of the string , that means fgets has read in all of the input*/ 
            if ( input2[strlen ( input2 ) -1] == '\n' ) {
                strcat ( input , input2 ) ; 
                clearArray ( input2 , 512 ) ; 
                printf ( "- " ) ; 
            }
            else {
                strcat ( input , input2 ) ; 
                strcat ( input , "\n" ) ; 
            }
        }
        printf ( "ctrl-d\n" ) ; 
    }
}
userPost * postEntryformatEntrycccc  ( char name[] , char stream[] , char text[] , char dateAndTime[] ) {
    userPost * current ; 
    /*Malloc memory for the Post node*/ 
    current = malloc ( sizeof ( userPost ) ) ; 
    /*If the malloc fails , do not create the node*/ 
    if ( current == NULL ) {
        free ( current ) ; 
        return NULL ; 
    }
    current->username = name ; 
    current->streamname = stream ; 
    current->date = dateAndTime ; 
    current->text = text ; 
    return current ; 
}
void postEntrygetTimeAndDatec ( char string[] ) {
    time_t currentTime ; 
    struct tm * timeStruct ; 
    /*Get the current data and time from the machine this program is being run on*/ 
    currentTime = time ( NULL ) ; 
    timeStruct = localtime ( &currentTime ) ; 
    /*Format the time string in order to be parsed correctly in the python script*/ 
    strftime ( string , 256 , "%b. %d, %Y %H:%M\n" , timeStruct ) ; 
}
void postEntrysubmitPost ( userPost * post ) {
    /*updateStream is a library function*/ 
    updateStream ( post ) ; 
    free ( post ) ; 
}
void constructorpostEntry (struct postEntry * ptr) {
    ptr->postEntrysubmitPost = postEntrysubmitPost;
    ptr->postEntrygetTimeAndDatec = postEntrygetTimeAndDatec;
    ptr->postEntryreadInputci = postEntryreadInputci;
    ptr->postEntryformatEntrycccc = postEntryformatEntrycccc;
}
 int main ( int argc , char * argv[] ) {
    if ( argc == 1 ) {
        printf ( "%s\n" , "Username not included." ) ; 
        return 1 ; 
    }
    struct postEntry post ; 
    constructorpostEntry(&post); 
    userPost * fullPost ; 
    char streamName[256] ; 
    char username[256] ; 
    char text[512] ; 
    char dateAndTime[256] ; 
    int i = 0 ; 
    clearArray ( streamName , 256 ) ; 
    clearArray ( text , 512 ) ; 
    clearArray ( dateAndTime , 256 ) ; 
    clearArray ( username , 256 ) ; 
    /*Prompt for user input*/ 
    post.postEntryreadInputci ( streamName , 1 ) ; 
    post.postEntryreadInputci ( text , 2 ) ; 
    /*Get the time and date*/ 
    post.postEntrygetTimeAndDatec ( dateAndTime ) ; 
    /*Loop through all arguments after the first one*/ 
    for ( i = 1 ; argv[i] != NULL ; i++ ) {
        /*Add argument to username string*/ 
        strcat ( username , argv[i] ) ; 
        /*Multiple username arguments mean the username is multiple words so seperarte the arguments/words by a space*/ 
        if ( argv[i+1] != NULL ) {
            strcat ( username , " " ) ; 
        }
    }
    /*Format all the inputed information into a struct that can be sent to the stream library*/ 
    fullPost = post.postEntryformatEntrycccc ( username , streamName , text , dateAndTime ) ; 
    /*Send struct to stream library*/ 
    post.postEntrysubmitPost ( fullPost ) ; 
    return 0 ; 
}
