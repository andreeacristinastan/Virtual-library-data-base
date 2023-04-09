#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "find.h"

#define LOGIN "/api/v1/tema/auth/login"
#define REGISTER "/api/v1/tema/auth/register"
#define ENTER_LIBRARY "/api/v1/tema/library/access"
#define ADD_BOOK "/api/v1/tema/library/books"
#define GET_BOOKS "/api/v1/tema/library/books"
#define LOGOUT "/api/v1/tema/auth/logout"

#define PAYLOAD "application/json"

/**
 * @brief - Functie care realizeaaza comenzile register si login
 *          in functie de parametrul type.
 *
 * @param username - numele de utilizator ce se vrea a fi folosit
 * @param password - parola ce se vrea a fi folosita
 * @param type - tipul comenzii 0-register 1-login
 * @return char** - returneaza cookie-ul primit de la server daca este cazul
 */
char **connection(char username[], char password[], int type)
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    char **body = calloc(1, sizeof(char *));
    body[0] = calloc(LINELEN, sizeof(char));

    char **cookie = calloc(1, sizeof(char *));
    cookie[0] = calloc(LINELEN, sizeof(char));

    cookie[0] = "this is not a cookie";

    JSON_Value *json_val = json_value_init_object();
    JSON_Object *json_obj = json_value_get_object(json_val);

    json_object_set_string(json_obj, "username", username);
    json_object_set_string(json_obj, "password", password);
    char *json_prettier = json_serialize_to_string_pretty(json_val);

    strcpy(body[0], json_prettier);

    if (type == 1)
    {
        message = compute_post_request("34.241.4.235", LOGIN, PAYLOAD, body, 1, NULL, 1, NULL, 1);
    }
    else
    {
        message = compute_post_request("34.241.4.235", REGISTER, PAYLOAD, body, 1, NULL, 1, NULL, 1);
    }

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strncmp(response, "HTTP/1.1 4", 10) == 0)
    {
        return find_error(response);
    }
    else if (strncmp(response, "HTTP/1.1 2", 10) == 0 && type == 1)
    {
        cookie = find_cookie(response);
    }

    free(body[0]);
    free(body);

    close(sockfd);
    free(message);
    free(response);
    return cookie;
}

/**
 * @brief - Functie care permite utilizatorului logat sa aiba acces la librarie
 *
 * @param cookie - cookie-ul returnat dupa logare ce ajuta la obtinerea permisiunii
 * @return char** - returneaza token-ul(daca este cazul) cu ajutorul caruia se vor
 *                  accesa, sterge sau adauga carti in librarie
 */
char **enter(char **cookie)
{
    char *message;
    char *response;
    int sockfd;

    char **token = calloc(1, sizeof(char *));
    token[0] = calloc(LINELEN, sizeof(char));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.241.4.235", ENTER_LIBRARY, NULL, cookie, 1, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    token = find_token(response);

    close(sockfd);
    free(message);
    free(response);

    return token;
}

/**
 * @brief - Functia adauga o noua carte in libraria utilizatorului curent
 *
 * @param token - token-ul ce ajuta la introducerea cartii
 */
void add_book(char **token, char title[], char author[], char genre[], char publisher[], int page_count)
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    char **body = calloc(1, sizeof(char *));
    body[0] = calloc(LINELEN, sizeof(char));

    JSON_Value *json_val = json_value_init_object();
    JSON_Object *json_obj = json_value_get_object(json_val);

    json_object_set_string(json_obj, "title", title);
    json_object_set_string(json_obj, "author", author);
    json_object_set_string(json_obj, "genre", genre);
    json_object_set_string(json_obj, "publisher", publisher);
    json_object_set_number(json_obj, "page_count", page_count);

    char *json_prettier = json_serialize_to_string_pretty(json_val);

    strcpy(body[0], json_prettier);
    message = compute_post_request("34.241.4.235", ADD_BOOK, PAYLOAD, body, 1, NULL, 1, token, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strncmp(response, "HTTP/1.1 4", 10) == 0)
    {
        find_error(response);
    }
    else if (strncmp(response, "HTTP/1.1 2", 10) == 0)
    {
        printf("Your book was added\n");
    }

    free(body[0]);
    free(body);

    close(sockfd);
    free(message);
    free(response);
}

/**
 * @brief - functie ce cauta o carte dupa id-ul trimis ca parametru
 *          sau toate cartile daca parametrul type este -1
 *
 * @param token - token-ul ce ajuta la gasirea cartii
 * @param type - 0 - afiseaza toate cartile -1 - afiseaza cartea cu id-ul trimis
 *                ca parametru
 * @param id - id-ul cartii ce se vrea gasita.
 */
void get_books(char **token, int type, int id)
{
    char *message;
    char *response;
    int sockfd;

    char **books = calloc(1, sizeof(char *));
    books[0] = calloc(LINELEN, sizeof(char));

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
    if (type == 0)
    {
        message = compute_get_request("34.241.4.235", GET_BOOKS, NULL, NULL, 1, token, 1);
    }
    else
    {
        char buffer[100];
        sprintf(buffer, "/api/v1/tema/library/books/%d", id);
        message = compute_get_request("34.241.4.235", buffer, NULL, NULL, 1, token, 1);
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strncmp(response, "HTTP/1.1 4", 10) == 0)
    {
        find_error(response);
    }
    else if (strncmp(response, "HTTP/1.1 2", 10) == 0)
    {
        books[0] = strstr(response, "[");
        printf("%s\n", books[0]);
    }

    free(books);

    close(sockfd);
    free(message);
    free(response);
}

/**
 * @brief - functia sterge o carte din libraria utilizatorului curent
 *
 * @param token - token-ul ce ajuta la gasirea cartii
 * @param id - id-ul cartii ce trebuie stearsa
 */
void delete_book(char **token, int id)
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    char buffer[100];
    sprintf(buffer, "/api/v1/tema/library/books/%d", id);

    message = compute_delete_request("34.241.4.235", buffer, NULL, token, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strncmp(response, "HTTP/1.1 4", 10) == 0)
    {
        find_error(response);
    }
    else if (strncmp(response, "HTTP/1.1 2", 10) == 0)
    {
        printf("The book was deleted\n");
    }
}

/**
 * @brief - functie care realizeaza delogarea unui utilizator
 *
 * @param cookie - cookie-ul returnat dupa logare ce ajuta la delogare
 */
void logout(char **cookie)
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request("34.241.4.235", LOGOUT, NULL, cookie, 1, NULL, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strncmp(response, "HTTP/1.1 4", 10) == 0)
    {
        find_error(response);
    }
    else if (strncmp(response, "HTTP/1.1 2", 10) == 0)
    {
        printf("You are logged out\n");
    }
}