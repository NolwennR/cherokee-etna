#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"


/* Test du parsing */

Test(parser_uri, parse_uri_base_folder) {
    char * input = "/ HTTP/1.1\r\nContent-Type: application/json";
    char * result = "/ ";

    request_t *request = malloc(sizeof(request_t));
    parse_uri(request, &input);
    int compare = strcmp(request->url, result);
    free(request);

    cr_assert(compare);
}

Test(parser_uri, parse_uri_folder) {
    char * input = "folder/ HTTP/1.1\r\nContent-Type: application/json";
    char * result = "/folder ";

    request_t *request = malloc(sizeof(request_t));
    parse_uri(request, &input);
    int compare = strcmp(request->url, result);
    free(request);

    cr_assert(compare);
}

Test(parser_uri, parse_uri_file) {
    char * input = "/folder/file.html HTTP/1.1\r\nContent-Type: application/json";
    char * result = "/folder/file.html ";

    request_t *request = malloc(sizeof(request_t));
    parse_uri(request, &input);
    int compare = strcmp(request->url, result);
    free(request);

    cr_assert(compare);
}

Test(parser_uri, parse_uri_with_argument) {
    char * input = "/argument/56?GBG=5 HTTP/1.1\r\nContent-Type: application/json";
    char * result = "/argument/56?GBG=5 ";

    request_t *request = malloc(sizeof(request_t));
    parse_uri(request, &input);
    int compare = strcmp(request->url, result);
    free(request);

    cr_assert(compare);
}

Test(parse_method, parse_get_method) {
    char *input = "GET /lololol/56?GBG=5 HTTP/1.1\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    parse_method(request, &input);

    cr_assert(request->method == GET);
}

Test(parse_method, parse_post_method) {
    char *input = "POST /lnlsjvlkjg HTTP/1.1\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    parse_method(request, &input);

    cr_assert(request->method == POST);
}

Test(parse_method, parse_put_method) {
    char *input = "PUT /losfdsfdfsf HTTP/1.1\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    parse_method(request, &input);

    cr_assert(request->method == PUT);
}

Test(parse_method, parse_head_method) {
    char *input = "HEAD /losfdsfdfsf HTTP/1.1\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    parse_method(request, &input);

    cr_assert(request->method == HEAD);
}

Test(parse_method, parse_delete_method) {
    char *input = "DELETE /losfdsfdfsf HTTP/1.1\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    parse_method(request, &input);

    cr_assert(request->method == DELETE);
}

Test(check_http_version, check_http_version_valid) {
    char *input = "HTTP/1.1\r\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    request->method = GET;
    parse_method(request, &input);
    
    cr_assert(request->method != UNSUPORTED, "the result is %d, expected %d", request->method, UNSUPORTED);
}

Test(check_http_version, check_http_version_unvalid) {
    char *input = "HTTP/1.2\r\nContent-Type: application/json";

    request_t *request = malloc(sizeof(request_t));
    request->method = GET;
    parse_method(request, &input);

    cr_assert(request->method == UNSUPORTED, "the result is %d, expected %d", request->method, UNSUPORTED);
}

Test(parse, parse_request) {
    char *url_result = "/lololol/56?GBG=5 ";
    char *input = "GET /lololol/56?GBG=5 HTTP/1.1\r\n\
        Content-Type: application/json\r\n\
        User-Agent: PostmanRuntime/7.24.0\r\n\
        Accept: */*\r\n\
        Cache-Control: no-cache\r\n\
        Postman-Token: 19b37665-1ff6-4faf-9f75-d90eb6267b4e\r\n\
        Host: localhost:8070\r\n\
        Accept-Encoding: gzip, deflate, br\r\n\
        Connection: keep-alive\r\n\
        Content-Length: 49\r\n\
        {\"password\": \"password\",\"username\": \"jonny\"}";

    request_t *request = malloc(sizeof(request_t));
    request->method = GET;
    parse_header(request, input);
    int compare = strcmp(request->url, url_result);

    cr_expect(request->method == GET, "the result is %d, expected %d", request->method, GET);
    cr_expect(compare, "the result is %s, expected %s", request->url, url_result);
}




/* Test du formatage */



