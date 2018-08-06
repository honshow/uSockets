/* This example, or test, is a moron test where the library is being hammered in all the possible ways randomly over time */

#include <libusockets.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void perform_random_operation(struct us_socket *s) {
    switch (rand() % 3) {
        case 0: {
            us_socket_close(s);
        }
        break;
        case 1: {
            us_socket_write(s, "hej!", 4, 0);
        }
        break;
        case 2: {
            us_socket_write(s, "hå", 2, 0);
        }
        break;
    }
}

struct us_socket *client;

struct http_socket {

};

struct http_context {

};

void on_wakeup(struct us_loop *loop) {

}

void on_pre(struct us_loop *loop) {

}

void on_post(struct us_loop *loop) {

}

void on_http_socket_writable(struct us_socket *s) {
    perform_random_operation(s);
}

void on_http_socket_close(struct us_socket *s) {
    perform_random_operation(s);
}

void on_http_socket_end(struct us_socket *s) {
    perform_random_operation(s);
}

void on_http_socket_data(struct us_socket *s, char *data, int length) {
    if (s == client) {
        printf("Client got data: %.*s\n", length, data);
    } else {
        printf("Server got data: %.*s\n", length, data);
    }

    perform_random_operation(s);
}

void on_http_socket_open(struct us_socket *s) {
    perform_random_operation(s);
}

void on_http_socket_timeout(struct us_socket *s) {

}

int main() {
    srand(time(0));

    struct us_loop *loop = us_create_loop(1, on_wakeup, on_pre, on_post, 0);

    struct us_socket_context *http_context = us_create_socket_context(loop, sizeof(struct http_context));

    us_socket_context_on_open(http_context, on_http_socket_open);
    us_socket_context_on_data(http_context, on_http_socket_data);
    us_socket_context_on_writable(http_context, on_http_socket_writable);
    us_socket_context_on_close(http_context, on_http_socket_close);
    us_socket_context_on_timeout(http_context, on_http_socket_timeout);
    us_socket_context_on_end(http_context, on_http_socket_end);

    struct us_listen_socket *listen_socket = us_socket_context_listen(http_context, 0, 3000, 0, sizeof(struct http_socket));

    // establish a couple of connections
    client = us_socket_context_connect(http_context, "localhost", 3000, 0, sizeof(struct http_socket));

    // add us_listen_socket_close(listen_socket); for fallthrough testing

    if (listen_socket) {
        printf("Running hammer test\n");
        us_loop_run(loop);
    } else {
        printf("Failed to listen!\n");
    }
}