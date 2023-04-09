#ifndef _COMMANDS_
#define _COMMANDS_

char **connection(char username[], char password[], int type);

char **enter(char **cookie);

void add_book(char **token, char title[], char author[], char genre[], char publisher[], int page_count);

void get_books(char **token, int type, int id);

void delete_book(char **token, int id);

void logout(char **cookie);

#endif
