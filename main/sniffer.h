#ifndef SNIFFER_H
#define SNIFFER_H

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_wifi_types.h"
#include "esp_log.h"

#include "config.h"


typedef struct {
	unsigned frame_ctrl:16;
	unsigned duration_id:16;
	uint8_t addr1[MAC_ADDR_SIZE]; /* receiver address */
	uint8_t addr2[MAC_ADDR_SIZE]; /* sender address */
	uint8_t addr3[MAC_ADDR_SIZE]; /* filtering address */
	unsigned sequence_ctrl:16;
	uint8_t addr4[MAC_ADDR_SIZE]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
	wifi_ieee80211_mac_hdr_t hdr;
	uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

void init_sniffer(void);
void sniffer(void* buf, wifi_promiscuous_pkt_type_t type);
void print_mac_list(void);
bool mac_in_list(uint8_t arr1[]);
bool check_if_cell(uint8_t arr[]);
void clear_mac_list();
void print_promis_packet(const wifi_promiscuous_pkt_t* ppkt, const wifi_ieee80211_mac_hdr_t* hdr);


#endif