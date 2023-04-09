#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <ctype.h>
#include "find.h"
#include "helpers.h"

/**
 * @brief - functie care parseaza raspunsul primit de la server
 *          si gaseste eroarea trimisa de acesta
 *
 * @param response - raspunsul server-ului
 * @return char** - eroarea parsata
 */
char **find_error(char *response)
{
    char error[100];
    error[0] = '\0';

    int cnt = 0, start_point = 0;

    for (int i = strlen(response) - 1; i >= 0; i--)
    {
        if (response[i] == '\n')
        {
            start_point = i + 1;
            break;
        }
    }

    for (int i = start_point; i < strlen(response); i++)
    {
        error[cnt] = response[i];
        cnt++;
    }
    error[cnt] = '\0';
    printf("%s\n", error);

    return NULL;
}

/**
 * @brief - functie care parseaza raspunsul primit de la server
 *          si gaseste cookie-ul din raspuns.
 *
 * @param response - raspunsul serverului
 * @return char** - cookie-ul gasit din raspuns
 */
char **find_cookie(char *response)
{
    char **cookie = calloc(1, sizeof(char *));
    cookie[0] = calloc(LINELEN, sizeof(char));

    cookie[0] = strstr(response, "connect.sid");
    cookie[0] = strtok(cookie[0], "\n");
    cookie[0] = strtok(cookie[0], ";");

    return cookie;
}

/**
 * @brief - functie care parseaza raspunsul primit de la server
 *          si gaseste token-ul din cadrul acestuia
 *
 * @param response - raspunsul primit de la server
 * @return char** - token-ul gasit din raspuns
 */
char **find_token(char *response)
{
    char **token = calloc(1, sizeof(char *));
    token[0] = calloc(LINELEN, sizeof(char));

    token[0] = strstr(response, ":\"");
    token[0] = strtok(token[0], "\n");
    token[0] = strtok(token[0], ":}\"");

    return token;
}

/**
 * @brief - functie care primeste un pointer char si
 *          verifica daca acesta este un numar sau nu.
 *
 * @param check - cuvantul ce trebuie verificat
 * @return int - 0 - daca este un numar 1 - daca nu este
 */
int find_is_int(char *check)
{
    int cnt = 0;
    for (int i = 0; check[i] != '\0'; i++)
    {
        if (isdigit(check[i]) == 0)
        {
            cnt = 1;
            break;
        }
    }
    return cnt;
}