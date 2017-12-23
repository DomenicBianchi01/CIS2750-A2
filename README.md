# CIS2750 A2

The second assignment invloved creating a Forum for a terminal. With one C program, you can create Streams (essentially a Board) and write Posts. A second C program allows you to add and remove users from specific Boards. A python script displays allows the user to view a Stream.

The Python script also keeps track of which Posts a user has already read. When a user views a Stream, the script will display unread messages first. Finally, there is an option to sort the posts based on date or user (alphabetically)

************
Compilation
************

To compile this program execute the following command when inside the "A2" directory:

make

This command will compile A1 (converter.c and parseFile.c) and run the program to convert a file named post.cc. Please note that the file must be named post.cc, otherwise the conversion will fail. Next, an object file for the stream library will be created. Then, the post and addauthor programs will be compiled where the library will be linked. Finally, permissions will be updated for view.py

********************
Running the programs
********************

To add a user to a stream:

./addauthor <username>

where <username> can be a single word or multi-word

To remove a user from a stream:

./addauthor -r <username>

where <username> (single word or multi-word) comes after the -r flag.

To add a post to a stream:

./post <username>

where <username> can be a single word or multi-word.

To view streams/posts:

chmod +x ./view.py (for the first time running the program)

./view.py

*****************
Known Limitations
*****************
-For posts that are under 24 lines, if the post cannot fit on the current page, it will displayed on the next. In other words, posts will be displayed in full and not cut between pages.

-For posts that are over 23 lines, the post will always begin on a new page and continue on for however many pages are required.

-For posts that are over 23 lines, when paging to the second or greater page of the post, 3 lines are printed above but are pushed off screen by extra new line characters. As long as the user does not physically scroll the terminal, these extra lines will not be seen.

-It is assumed the terminal size will always be 80x24

-A stream name cannot be longer than 256 characters

-The post text cannot be longer than 512 characters

-When running addauthor and inputing a list of stream names for "list streams:", the number of characters you input cannot exceed 256 characters

-Stream names cannot have a comma in them. Doing so will cause the add and remove user functions in addauthor to fail.

-A stream cannot be named "all"

-If a file in the "messages" directory is deleted while the view program is being run and requires to use the file, the program will crash

-Given a stream or set of streams where some posts have been read but others haven't, the unread posts may appear to be disorganized (in terms that there is room for a post to be included on a page but instead its on another page). This is because read and unread posts are grouped separately in two individually arrays. Then both arrays are indexed (to determine which posts can be displayed on each page) from the start of the array to the end. After both arrays are merged together. As a result, when viewing the page that contains the oldest unread message, if you were to page up, you could possibly see as few as one post on this page since the program indexed this page last (after all other read posts were indexed).

-When initially viewing messages, the oldest unread message will be displayed first. If the 'O' key is pressed, the messages will be sorted by names. If pressed again, the messages will be sorted by date again, but the display will begin at the very beginning/first page; it will not go to the oldest unread post.
