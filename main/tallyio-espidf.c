// Standard Libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "string.h"
// ESP-IDF Libraries
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi_types.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
// 3rd Party Libaries
#include "wifi_manager.h"
// Project Headers
#include "config.h"
#include "sniffer.h"
#include "wifi_scanner.h"


#define ENABLE_WIFI_CONNECTION
#define ENABLE_AWS_CONNECTION
// #define ENABLE_WIFI_SCAN

void set_verbosity(void);
void wifi_init();

int aws_iot_demo_main( int argc, char ** argv );

int init_mqtt();
void mqtt_loop();


TaskHandle_t handle_wifi_scan = NULL;

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";

void wifi_connected_callback(void *pvParameter){
	ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;

	/* transform IP to human readable string */
	char str_ip[16];
	esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

	ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);

  #ifdef ENABLE_AWS_CONNECTION
  // aws_iot_demo_main(0,NULL);
  init_mqtt();
  mqtt_loop();
  #endif
}


void app_main(void){
    /* start the wifi manager */
    #ifdef ENABLE_WIFI_CONNECTION
    wifi_manager_start();
    #endif

    #ifdef ENABLE_WIFI_SCAN
    /* register a callback as an example to how you can integrate your code with the wifi manager */
    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &wifi_connected_callback);
    #endif
}


// Set which log messages will be output to serial
void set_verbosity(void){
  #ifndef LOG_APP_MAIN
    esp_log_level_set(APP_MAIN_TAG, ESP_LOG_NONE);
  #endif
  #ifndef LOG_SNIFFER_INIT
    esp_log_level_set(INIT_SNIFFER_TAG, ESP_LOG_NONE);
  #endif
  #ifndef LOG_SNIFFER
    esp_log_level_set(SNIFFER_TAG, ESP_LOG_NONE);
  #endif
  #ifndef LOG_WIFI_SCAN
    esp_log_level_set(WIFI_SCAN_TAG, ESP_LOG_NONE);
  #endif
  #ifndef LOG_MQTT
    esp_log_level_set(MQTT_TAG, ESP_LOG_NONE);
  #endif
}



