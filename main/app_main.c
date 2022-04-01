/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
// #include <string.h>
 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "esp_system.h"
 #include "esp_wifi.h"
 #include "esp_event.h"
 #include "esp_log.h"
 #include "nvs_flash.h"
 #include "freertos/event_groups.h"
 #include "lwip/err.h"
 #include "lwip/sys.h"
 #include "app_http_server.h"
#include "wifi_connecting.h"
#include "esp_smartconfig.h"

 char ssid[64];
 char pass[32];
 EventGroupHandle_t xCreatedEventGroup;
enum provision_type_t provision_type= PROV_SMART_CONFIG;

int WIFI_RECV_INFO=BIT0;
int WIFI_CONNECTED_BIT=BIT1;
int WIFI_FAIL_BIT =BIT2;
int ESPTOUCH_DONE_BIT=BIT3; 

void wifi_data_callback(char *data, int len);
const char *TAG1 = "wifi softAP";

//hàm callback để func poiter chỏ đến hàm này
void wifi_data_callback(char *data, int len){
    printf("%.*s", len, data);
    char *pt = strtok(data, "/");
    strcpy(ssid,pt);
    pt=strtok(NULL, "/");
    strcpy(pass, pt);
    printf("ssid: %s \n pass: %s\n", ssid, pass);
    xEventGroupSetBits(xCreatedEventGroup, WIFI_RECV_INFO);
}

void app_main(void)
{
    //Initialize NVS
    //khởi tạo thông tin wifi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);   
    xCreatedEventGroup = xEventGroupCreate();
    initialise_wifi();
    wifi_config_t conf;
    esp_wifi_get_config(ESP_IF_WIFI_STA, &conf);
    //check xem đã kết nối wifi trươc đó chưa
     if(conf.sta.ssid[0] != 0){
         provision_type = PROV_NONE;
        ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
        ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &conf));
        ESP_ERROR_CHECK( esp_wifi_start() );
        xEventGroupSetBits(xCreatedEventGroup, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG1, "Đã thiết lập wifi\n");
    }
    else {
        ESP_LOGI(TAG1, "ESP_WIFI_MODE_AP");
        if(provision_type == PROV_ACCESS_POINT){
            wifi_init_softap();
        }
        else if(provision_type == PROV_SMART_CONFIG){
            ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
            ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
            ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
            ESP_ERROR_CHECK( esp_wifi_start() );
    }
    } 
    xEventGroupWaitBits(xCreatedEventGroup, WIFI_CONNECTED_BIT, true, false, portMAX_DELAY);
    ESP_LOGI(TAG1, "READY CONECTED TO CLOUD");
}
