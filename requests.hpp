#ifndef _REQUESTS_
#define _REQUESTS_

#include <iostream>
#include <string>

using namespace std;

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(const char *host, const char *url, const char *query_params, string cookie, string token);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(const char *host, const char *url, const char* content_type, string s, string token);

// computes and returns a DELETE request string (cookies can be NULL if not needed)
char *compute_delete_request(const char *host, const char *url, string token);

#endif
