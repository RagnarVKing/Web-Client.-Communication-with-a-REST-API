#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"

char *compute_get_request(const char *host, const char *url, const char *query_params, string cookie, string token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
        compute_message(message, line);
    } else {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "GET %s HTTP/1.1", url);
        compute_message(message, line);
    }

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookie != "") {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "Cookie: %s", cookie.c_str());
        compute_message(message, line);
    }
    if (token != "") {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "Authorization: Bearer %s", token.c_str());
        compute_message(message, line);
    }

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "connection: keep-alive");
    compute_message(message, line);

    compute_message(message, "");
    free(line);
    return message;
}

char *compute_post_request(const char *host, const char *url, const char* content_type, string s, string token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "Content-Length: %ld", s.size());
    compute_message(message, line);

    if (token != "") {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "Authorization: Bearer %s", token.c_str());
        compute_message(message, line);
    }

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "connection: keep-alive");
    compute_message(message, line);

    compute_message(message, "");


    memset(line, 0, LINELEN);

    strcat(message, s.c_str());

    free(line);
    return message;
}

char *compute_delete_request(const char *host, const char *url, string token)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (token != "") {
        memset(line, 0, sizeof(char) * LINELEN);
        sprintf(line, "Authorization: Bearer %s", token.c_str());
        compute_message(message, line);
    }

    memset(line, 0, sizeof(char) * LINELEN);
    sprintf(line, "connection: keep-alive");
    compute_message(message, line);

    compute_message(message, "");
    free(line);
    return message;
}