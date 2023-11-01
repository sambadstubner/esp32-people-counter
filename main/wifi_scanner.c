#include "wifi_scanner.h"
#include "sniffer.h"


// Iterate through each of the WiFi channels, increment every 1 second
void wifi_scan(void *arg){
  while(true){
    static uint16_t curr_channel = MIN_WIFI_CHANNEL;

    if(curr_channel > MAX_WIFI_CHANNEL){
      curr_channel = MIN_WIFI_CHANNEL;
    }
    esp_wifi_set_channel(curr_channel, WIFI_SECOND_CHAN_NONE);
    ESP_LOGI(WIFI_SCAN_TAG, "Current channel: %d\n", curr_channel);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    print_mac_list();
    curr_channel++;
  }
}