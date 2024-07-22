#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <iostream>
#include "helpers.hpp"
#include "requests.hpp"
#include "json.hpp"

#define REGISTER "register"
#define LOGIN "login"
#define ENTER_LIBRARY "enter_library"
#define GET_BOOKS "get_books"
#define GET_BOOK "get_book"
#define ADD_BOOK "add_book"
#define DELETE_BOOK "delete_book"
#define LOGOUT "logout"
#define EXIT "exit"

using namespace std;
using json = nlohmann::json;

char server_ip[] = "34.246.184.49";
int server_port = 8080;
//checks if the string str has " "
int check_space(string str) {
    for (char c : str) {
        if (c == ' ') {
            return 1;
        }
    }
    return 0;
}

int check_digit(string str) {
    for (char c : str) {
        if (c < '0' || c > '9') {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    string cookie;
    string token;

    bool logged_in = false;
    bool in_library = false;

    while (1) {
        string command;
        getline(cin, command);

        if (command == REGISTER) {
            string username, password;
            cout << "username=";
            getline(cin, username);

            cout << "password=";
            getline(cin, password);

            if(username == "") {
                cout << "EROARE la username\n";
                continue;
            }

            if (check_space(username) == 1) {
                cout << "EROARE la username\n";
                continue;
            }

            if (password == "") {
                cout << "EROARE la parola\n";
                continue;
            }

            if (check_space(password) == 1) {
                cout << "EROARE la parola\n";
                continue;
            }


            json j = {
                {"username", username},
                {"password", password}
            };

            string s = j.dump();

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_post_request(server_ip, "/api/v1/tema/auth/register", "application/json", s, "");
            
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            close_connection(sockfd);

            char *c = strstr(response, "error");
            if (c != NULL) {
                cout << "EROARE\n";
                continue;
            }

            cout << "Utilizator înregistrat cu SUCCES!\n";

            continue;
        }

        if (command == LOGIN) {
            string username, password;
            cout << "username=";
            getline(cin, username);

            cout << "password=";
            getline(cin, password);

            if (logged_in == true) {
                cout << "EROARE utilizatorul este deja logat\n";
                continue;
            }

            if (username == "") {
                cout << "EROARE la username\n";
                continue;
            }

            if (check_space(username) == 1) {
                cout << "EROARE la username\n";
                continue;
            }

            if (password == "") {
                cout << "EROARE la parola\n";
                continue;
            }

            if (check_space(password) == 1) {
                cout << "EROARE la parola\n";
                continue;
            }

            json j = {
                {"username", username},
                {"password", password}
            };

            string s = j.dump();

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_post_request(server_ip, "/api/v1/tema/auth/login", "application/json", s, "");

            send_to_server(sockfd, message);
            
            response = receive_from_server(sockfd);

            close_connection(sockfd);

            char *c = strstr(response, "Set-Cookie: ");
            if (c != NULL) {
                c += 12;
                char *d = strchr(c, ';');
                if (d != NULL) {
                    cookie = string(c, d - c);
                } else {
                    cout << "EROARE la logare2\n";
                    continue;
                }
            } else {
                cout << "EROARE la logare1\n";
                continue;
            }

            cout << "Utilizatorul a fost logat cu SUCCES\n";
            logged_in = true;

            continue;
        }

        if (command == ENTER_LIBRARY) {
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_get_request(server_ip, "/api/v1/tema/library/access", NULL, cookie, "");

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            
            close_connection(sockfd);

            char *e = strstr(response, "error");
            if (e != NULL) {
                cout << "EROARE la accesarea bibliotecii\n";
                continue;
            }

            char *c = strstr(response, "token");
            if (c != NULL) {
                c += 8;
                char *d = strchr(c, ':\"');
                if (d != NULL) {
                    token = string(c, d - c);
                } else {
                    cout << "EROARE la accesarea bibliotecii\n";
                    continue;
                }
            } else {
                cout << "EROARE la accesarea bibliotecii\n";
                continue;
            }

            in_library = true;

            cout << "SUCCES: Utilizatorul are acces la biblioteca\n";

            continue;
        }
        
        if (command == GET_BOOKS) {
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }
            if (in_library == false) {
                cout << "EROARE utilizatorul nu are acces la biblioteca\n";
                continue;
            }
            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_get_request(server_ip, "/api/v1/tema/library/books", NULL, cookie, token);

            send_to_server(sockfd, message);
            
            response = receive_from_server(sockfd);

            close_connection(sockfd);

            char *c = strstr(response, "[");
            if(c != NULL) {
                cout << c << "\n";
            } else {
                cout << "EROARE la accesarea cartilor\n";
                continue;
            }

            continue;
        }
        
        if (command == GET_BOOK) {
            string id;
            cout << "id=";
            
            getline(cin, id);
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }
            if (in_library == false) {
                cout << "EROARE utilizatorul nu are acces la biblioteca\n";
                continue;
            }
            
            if (check_space(id) == 1) {
                cout << "EROARE la id\n";
                continue;
            }

            string path = "/api/v1/tema/library/books/" + id;

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_get_request(server_ip, path.c_str(), NULL, cookie, token);

            send_to_server(sockfd, message);
            
            response = receive_from_server(sockfd);

            close_connection(sockfd);

            char *c = strstr(response, "{");
            if(c != NULL) {
                cout << c << "\n";
            } else {
                cout << "EROARE la accesarea cartii\n";
                continue;
            }

            continue;
        }
        
        if (command == ADD_BOOK) {

            string title, author, genre, publisher, page_count;

            cout << "title=";
            getline(cin, title);

            cout << "author=";
            getline(cin, author);

            cout << "genre=";
            getline(cin, genre);

            cout << "publisher=";
            getline(cin, publisher);

            cout << "page_count=";
            getline(cin, page_count);
            
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }
            if (in_library == false) {
                cout << "EROARE utilizatorul nu are acces la biblioteca\n";
                continue;
            }

            if (title == "" || author == "" || genre == "" || publisher == "" || page_count == "") {
                cout << "EROARE: Campuri necompletate\n";
                continue;
            }

            if (check_digit(page_count) == 1) {
                cout << "EROARE: Tip de date incorect pentru numarul de pagini\n";
                continue;
            }

            json j = {
                {"title", title},
                {"author", author},
                {"genre", genre},
                {"publisher", publisher},
                {"page_count", page_count}
            };

            string s = j.dump();

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_post_request(server_ip, "/api/v1/tema/library/books", "application/json", s, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            close_connection(sockfd);

            cout << "Cartea a fost adaugata cu SUCCES\n";

            continue;
        }
        
        if (command == DELETE_BOOK) {

            string id;
            cout << "id=";
            getline(cin, id);
            if (check_digit(id) == 1) {
                cout << "EROARE la id\n";
                continue;
            }
            
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }
            if (in_library == false) {
                cout << "EROARE utilizatorul nu are acces la biblioteca\n";
                continue;
            }

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            string path = "/api/v1/tema/library/books/" + id;

            message = compute_delete_request(server_ip, path.c_str(), token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            close_connection(sockfd);

            char *c = strstr(response, "error");
            if (c != NULL) {
                cout << "EROARE la stergerea cartii\n";
                continue;
            }

            cout << "Cartea a fost stearsa cu SUCCES\n";

            continue;
        }

        if (command == LOGOUT) {
            if (logged_in == false) {
                cout << "EROARE utilizatorul nu este logat\n";
                continue;
            }

            sockfd = open_connection(server_ip, server_port, AF_INET, SOCK_STREAM, 0);

            message = compute_get_request(server_ip, "/api/v1/tema/auth/logout", NULL, cookie, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            close_connection(sockfd);

            cout << "SUCCES: Utilizatorul s-a delogat\n";

            logged_in = false;
            in_library = false;

        }

        if (command == EXIT) {
            cout << "Inchidere program\n";
            break;
        }

    }

    return 0;
}
