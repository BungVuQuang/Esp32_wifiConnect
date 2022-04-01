#include "app_http_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
/* A simple example that demonstrates how to crea.te GET and POST
 * handlers for the web server.
 */

static const char *TAG = "app_http_server";
httpd_handle_t server = NULL;
static http_server_handle_t http_get_handle = NULL;
static http_server_handle_t http_post_handle = NULL;
static http_server_handle_t http_post_slider1_handle = NULL;
static http_server_handle_t http_post_rgb_handle = NULL;

extern const uint8_t web_start[] asm("_binary_web_html_start");
extern const uint8_t web_end[] asm("_binary_web_html_end");

static esp_err_t hello_get_handler(httpd_req_t *req)
{
    char*  buf ="Hello World";
    size_t buf_len= strlen(buf);
    //httpd_resp_set_type(req, "image/jpg");
    httpd_resp_send(req,(const char*) web_start, (web_end - web_start));

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        //ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};
// static httpd_req_t *get_req;
// void http_send_response(char *data, int len){
//     httpd_resp_send(get_req,(const char*) data, len);//gửi lên web
//     //phải đưa ra hàm main
// }
// static esp_err_t dht11_getData_handler(httpd_req_t *req)
// {   
//     get_req=req;
//     http_get_handle("dht11", 5);

//     /* After sending the HTTP response the old HTTP request
//      * headers are lost. Check if HTTP request headers can be read now. */
//     if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
//        // ESP_LOGI(TAG, "Request headers lost");
//     }
//     return ESP_OK;
// }

// static const httpd_uri_t dht11 = {
//     .uri       = "/dht11",
//     .method    = HTTP_GET,
//     .handler   = dht11_getData_handler,
//     /* Let's pass response string in user
//      * context to demonstrate it's usage */
//     .user_ctx  = "dht11"
// };

// static esp_err_t rgb_getData_handler(httpd_req_t *req)
// {   
//     /* Read URL query string length and allocate memory for length + 1,
//      * extra byte for null termination */
//     int buf_len = httpd_req_get_url_query_len(req) + 1;
//     char *buf;
//     if (buf_len > 1) {
//         buf = malloc(buf_len);//cấp phát vùng nhớ động cho con trỏ buf,
//         if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
//             char param[32];
//             /* Get value of expected key from query string */
//             if (httpd_query_key_value(buf, "color", param, sizeof(param)) == ESP_OK) {
//                 ESP_LOGI(TAG, "Found URL query parameter => color=%s", param);
//                  http_post_rgb_handle(param, strlen(param));
//             }
//         }
//         free(buf);
//     }
   
//     httpd_resp_send(req,"OK",2);
//     return ESP_OK;
// }

// static const httpd_uri_t rgb = {
//     .uri       = "/rgb",
//     .method    = HTTP_GET,
//     .handler   = rgb_getData_handler,
//     /* Let's pass response string in user
//      * context to demonstrate it's usage */
//     .user_ctx  = "rgb"
// };


// static esp_err_t sw1_post_handler(httpd_req_t *req)
// {
//     char buf[100];
//     //printf("da vao day \n");
//     int len = httpd_req_recv(req, buf, 100);
//     //printf("%s/n",buf);
//     http_post_handle(buf, len);

//     httpd_resp_send_chunk(req, NULL, 0);
//     return ESP_OK;
// }

// static const httpd_uri_t sw1 = {
//     .uri       = "/sw1",
//     .method    = HTTP_POST,
//     .handler   = sw1_post_handler,
//     /* Let's pass response string in user
//      * context to demonstrate it's usage */
//     .user_ctx  = "sw1"
// };

// static esp_err_t sd1_post_handler(httpd_req_t *req)
// {
//     char buf_sd1[100];
//     //printf("da vao day \n");
//     int len = httpd_req_recv(req, buf_sd1, 100);
//     //printf("%s/n",buf);
//     http_post_slider1_handle(buf_sd1, len);

//     httpd_resp_send_chunk(req, NULL, 0);
//     return ESP_OK;
// }

// static const httpd_uri_t sd1 = {
//     .uri       = "/sd1",
//     .method    = HTTP_POST,
//     .handler   = sd1_post_handler,
//     /* Let's pass response string in user
//      * context to demonstrate it's usage */
//     .user_ctx  = "sd1"
// };

static esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf_sd1[100];
    //printf("da vao day \n");
    int len = httpd_req_recv(req, buf_sd1, 100);
    //printf("%s\n",buf_sd1);
    http_post_handle(buf_sd1, len);

    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t wifi_info = {
    .uri       = "/wifi",
    .method    = HTTP_POST,
    .handler   = wifi_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "wifi_info"
};

void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        // httpd_register_uri_handler(server, &dht11);
        // httpd_register_uri_handler(server, &sw1);
        // httpd_register_uri_handler(server, &sd1);
        // httpd_register_uri_handler(server, &rgb);
        httpd_register_uri_handler(server, &wifi_info);
    }
    else{
        ESP_LOGI(TAG, "Error starting server!");
    }
    
}

void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_get_set_callback(void *cb){
    if(cb){
        http_get_handle = cb;
    }
}

void http_post_set_callback(void *cb){
    if(cb){
        http_post_handle = cb;
    }
}

void http_post_slider1_set_callback(void *cb){
    if(cb){
        http_post_slider1_handle = cb;
    }
}

void http_post_rgb_set_callback(void *cb){
    if(cb){
        http_post_rgb_handle = cb;
    }
}