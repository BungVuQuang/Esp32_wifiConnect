#ifndef __APP_HTTP_SERVER_H
#define __APP_HTTP_SERVER_H
#include "stdint.h"
typedef void (*http_server_handle_t)(char *data, int len);
void start_webserver(void);
void stop_webserver(void);
void http_get_set_callback(void *cb);
void http_post_set_callback(void *cb);
void http_send_response(char *data, int len);
void http_post_slider1_set_callback(void *cb);
void http_post_rgb_set_callback(void *cb);
#endif