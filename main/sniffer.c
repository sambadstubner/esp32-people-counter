#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sniffer.h"

const signed int rssi_thresh = -10000;
const wifi_promiscuous_filter_t filter={
    .filter_mask=WIFI_PROMIS_FILTER_MASK_MGMT
};

const uint8_t mac_cell_filter[4] = {0x2, 0x6, 0xa, 0xe};
static uint16_t mac_addr_list [MAX_NUM_MAC_ADDR][MAC_ADDR_SIZE] = {0};
static uint32_t curr_count = 0;

// Initialize the WiFi with settings to sniff packets
void init_sniffer(void){
  ESP_LOGI(INIT_SNIFFER_TAG, "entering init_wifi");
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

  wifi_country_t wifi_country = {
  .cc="US",
  .schan=MIN_WIFI_CHANNEL,
  .nchan=MAX_WIFI_CHANNEL,
  .policy=WIFI_COUNTRY_POLICY_AUTO
  };

  nvs_flash_init();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_country));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&filter));
  ESP_ERROR_CHECK(esp_wifi_set_channel(MIN_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(&sniffer));

  // init mac list to zero
  clear_mac_list();


  ESP_LOGI(INIT_SNIFFER_TAG, "exiting init_wifi");
}

// Callback function which accepts a promiscuous WiFi packet and it's type.
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type){
  ESP_LOGI(SNIFFER_TAG, "entering sniffer");

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  // Isolate the sender MAC
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;
  uint8_t mac[MAC_ADDR_SIZE];
  // Copy the mac address from the header to an array
  for(uint8_t i = 0; i < MAC_ADDR_SIZE; i++){
    mac[i] = hdr->addr2[i];
  }
  // If the signal possibly a cellphone and is near then consider it
  if((check_if_cell(mac)) && (ppkt->rx_ctrl.rssi > rssi_thresh)){
    // Check to see if it is already in the list
    if(!mac_in_list(mac)){
      // Verify we have not exceeded the size of the array, if not then increment counter
      if(curr_count < MAX_NUM_MAC_ADDR){
        curr_count++;
        // Copy the mac address into the 2D array of all addresses.
        for(uint8_t i = 0; i < MAC_ADDR_SIZE; i++){
          mac_addr_list[curr_count-1][i] = mac[i];
        }
      }
    }
    
  }

  ESP_LOGI(SNIFFER_TAG, "exiting sniffer");
}

// Accepts MAC address and returns true if it is already in the list, otherwise false
bool mac_in_list(uint8_t arr1[]){
  bool same_mac = false;
  for(uint32_t mac = 0; mac < curr_count; mac++){
    bool same_index = true;
    for(uint8_t i = 0; i < MAC_ADDR_SIZE; i++){
      if(arr1[i]!=mac_addr_list[mac][i]){
        same_index = false;
        break;
      }
    }
    if(same_index){
      same_mac = true;
      break;
    }
  }
  return same_mac;
}

// Checks to see if the MAC address is randomized. If it is randomized, it is likely a cellphone
bool check_if_cell(uint8_t arr[]){
  bool cell = false;
  // Check to see if second value is a 2, 6, a or e
  for(uint8_t i = 0; i < 4; i++){
    if(arr[1] == mac_cell_filter[i])
      cell = true;
  }
  return cell;
}

// Reset all values in the mac_list
void clear_mac_list(){
  for(uint32_t mac = 0; mac < curr_count; mac++){
    for(uint8_t i = 0; i < MAC_ADDR_SIZE; i++){
      mac_addr_list[mac][i] = 0;
    }
  }
  curr_count = 0;
}


// Print each mac address in the address list
void print_mac_list(void){
  // Only print if there are devices detected
  if(curr_count > 0)
    printf("\n");
  for(uint32_t i = 0; i < curr_count; i++){
    printf("%d: ADDR=%02x:%02x:%02x:%02x:%02x:%02x\n", 
    i,
    mac_addr_list[i][0],
    mac_addr_list[i][1],
    mac_addr_list[i][2],
    mac_addr_list[i][3],
    mac_addr_list[i][4],
    mac_addr_list[i][5]
    );
  }
  if(curr_count > 0)
    printf("\n");
}

// Print a promiscuous wifi packet's information
void print_promis_packet(const wifi_promiscuous_pkt_t *ppkt, const wifi_ieee80211_mac_hdr_t* hdr){
  	printf("CHAN=%02d, RSSI=%02d,"
		" ADDR1=%02x:%02x:%02x:%02x:%02x:%02x,"
		" ADDR2=%02x:%02x:%02x:%02x:%02x:%02x,"
		" ADDR3=%02x:%02x:%02x:%02x:%02x:%02x\n",
		ppkt->rx_ctrl.channel,
		ppkt->rx_ctrl.rssi,
		/* ADDR1 */
		hdr->addr1[0],hdr->addr1[1],hdr->addr1[2],
		hdr->addr1[3],hdr->addr1[4],hdr->addr1[5],
		/* ADDR2 */
		hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
		hdr->addr2[3],hdr->addr2[4],hdr->addr2[5],
		/* ADDR3 */
		hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
		hdr->addr3[3],hdr->addr3[4],hdr->addr3[5]
	);
}