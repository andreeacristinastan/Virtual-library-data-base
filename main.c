#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "find.h"
#include "commands.h"

/**
 * @brief - Functia main se ocupa cu parsarea datelor de la stdin si verifica
 *          faptul ca utilizatorii sunt logati, inregistrati sau ca au acces
 *          in biblioteca.
 */
int main()
{
    char instruction[100], username_reg[100], password_reg[100], username_log[100], password_log[100];
    char title[100], author[100], genre[100], page_count[100], publisher[100];

    int logged = 0, entered = 0;

    char **cookie = calloc(1, sizeof(char *));
    cookie[0] = calloc(LINELEN, sizeof(char));

    char **token = calloc(1, sizeof(char *));
    token[0] = calloc(LINELEN, sizeof(char));

    for (int l = 0;; l++)
    {
        fgets(instruction, 1000, stdin);

        instruction[strlen(instruction) - 1] = '\0';

        if (strcmp(instruction, "login") == 0)
        {
            printf("username=");
            scanf("%s", username_log);
            printf("password=");
            scanf("%s", password_log);

            if (logged != 1)
            {
                cookie = connection(username_log, password_log, 1);
                if (cookie != NULL)
                {
                    logged = 1;
                    printf("You are logged in\n");
                }
            }
            else
            {
                printf("You are already logged in\n");
            }
        }
        else if (strcmp(instruction, "register") == 0)
        {
            printf("username=");
            scanf("%s", username_reg);
            printf("password=");
            scanf("%s", password_reg);

            if (logged == 0)
            {
                cookie = connection(username_reg, password_reg, 0);

                if (cookie != NULL)
                {
                    printf("You are register\n");
                }
            }
            else if (logged == 1)
            {
                printf("You are already logged in\n");
            }
        }
        else if (strcmp(instruction, "get_book") == 0)
        {
            char id[100];
            printf("id=");
            scanf("%s", id);

            if (entered == 1)
            {
                if (find_is_int(id) == 0)
                {
                    int id_value = atoi(id);
                    get_books(token, 1, id_value);
                }
                else
                {
                    printf("Wrong format of informations\n");
                }
            }
            else
            {
                printf("You don't have access to the library\n");
            }
        }
        else if (strcmp(instruction, "enter_library") == 0)
        {
            if (logged == 1)
            {
                token = enter(cookie);
                entered = 1;
                printf("You entered the library\n");
            }
            else
            {
                printf("You are not logged in\n");
            }
        }
        else if (strcmp(instruction, "get_books") == 0)
        {
            if (entered == 1)
            {
                get_books(token, 0, -1);
            }
            else
            {
                printf("You don't have access to the library\n");
            }
        }
        else if (strcmp(instruction, "add_book") == 0)
        {
            printf("title=");
            scanf("%s", title);
            printf("author=");
            scanf("%s", author);
            printf("publisher=");
            scanf("%s", publisher);
            printf("genre=");
            scanf("%s", genre);
            printf("page_count=");
            scanf("%s", page_count);

            if (entered == 1)
            {
                if (find_is_int(page_count) == 0)
                {
                    int number_of_page = atoi(page_count);
                    add_book(token, title, author, genre, publisher, number_of_page);
                }
                else
                {
                    printf("Wrong format of informations\n");
                }
            }
            else
            {
                printf("You don't have acces to the library\n");
            }
        }
        else if (strcmp(instruction, "delete_book") == 0)
        {
            char id[100];
            printf("id=");
            scanf("%s", id);

            if (entered == 1)
            {
                if (find_is_int(id) == 0)
                {
                    int id_value = atoi(id);
                    delete_book(token, id_value);
                }
                else
                {
                    printf("Wrong format of informations\n");
                }
            }
            else
            {
                printf("You don't have acces to the library\n");
            }
        }
        else if (strcmp(instruction, "logout") == 0)
        {
            if (logged == 1)
            {
                logged = 0;
                entered = 0;

                logout(cookie);

                free(cookie);

                free(token);
            }
            else
            {
                printf("You are not logged in\n");
            }
        }
        else if (strcmp(instruction, "exit") == 0)
        {
            if (logged == 1)
            {
                free(cookie);
            }

            if (entered == 1)
            {
                free(token);
            }

            return 0;
        }
    }
    return 0;
}