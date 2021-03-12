/* HTTP File Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "main.h"
#include <sys/param.h>

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_spiffs.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/gpio.h"							// vld

#define EXAMPLE_AP_WIFI_SSID		"WRover"
#define EXAMPLE_AP_WIFI_PASS		""
#define EXAMPLE_MAX_STA_CONN		4
#define EXAMPLE_STA_WIFI_SSID		"Tech_D3881996"
#define EXAMPLE_STA_WIFI_PASS		"FJHPEPJJ"
#define EXAMPLE_STA_MAXIMUM_RETRY	5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about one event
 * - are we connected to the AP with an IP? */
#define WIFI_CONNECTED_BIT BIT0


/* This example demonstrates how to create file server
 * using esp_http_server. This file has only startup code.
 * Look in file_server.c for the implementation */

static const char *TAG="example";

static int s_retry_num = 0;
programData_t programs[21];
uint32_t ipaddress, ipnetmask, ipgw;

/*

programData_t program;			// program ---> RAM: 0x0001206: 0x02, 0xA1, ..... 

program.days_of_the_week.b_friday = 1

k = program.days_of_the_week.b_friday
k = (uint8_t)program.days_of_the_week

if (program.days_of_the_week.b_friday)
{
}

/////////////////////

programData_t *p_program;

p_program = &programs;   // p_program = 0x0001206

k = p_program->days_of_the_week.b_friday;
k = *((uint8_t*)&p_program->days_of_the_week);
k = p_program->temperature;

*((uint8_t*)&p_program->days_of_the_week) = 0x00;

*/

/* Function to initialize SPIFFS */
static esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,   // This decides the maximum number of files that can be created on the storage
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        break;
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
		ipaddress = &event->event_info.got_ip.ip_info.ip;
		ipnetmask = &event->event_info.got_ip.ip_info.netmask;
		ipgw = &event->event_info.got_ip.ip_info.gw;
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        {
            if (s_retry_num < EXAMPLE_STA_MAXIMUM_RETRY) {
                esp_wifi_connect();
                xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                s_retry_num++;
                ESP_LOGI(TAG,"retry to connect to the AP");
            }
            ESP_LOGI(TAG,"connect to the AP fail\n");
            break;
        }
    default:
        break;
    }
    return ESP_OK;
}

/* /////// esp_wifi_types.h ///////
 * typedef union {
 *    wifi_ap_config_t  ap;
 *    wifi_sta_config_t sta;
 * } wifi_config_t; */
esp_err_t wifi_init_apsta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = EXAMPLE_AP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_AP_WIFI_SSID),
            .password = EXAMPLE_AP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
			//.channel = AP_CHANNEL
        },
    };
    if (strlen(EXAMPLE_AP_WIFI_PASS) == 0) {
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    wifi_config_t sta_config = {
        .sta = {
            .ssid = EXAMPLE_STA_WIFI_SSID,
            .password = EXAMPLE_STA_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_ap finished. SSID:%s password:%s",
             EXAMPLE_AP_WIFI_SSID, EXAMPLE_AP_WIFI_PASS);
    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             EXAMPLE_STA_WIFI_SSID, EXAMPLE_STA_WIFI_PASS);

 	return ESP_OK;
}

/* Declare the function which starts the file server.
 * Implementation of this function is to be found in
 * file_server.c */
esp_err_t start_file_server(const char *base_path);

void init_programs_data (programData_t *programs)
{
	programs->separator1 = ',';
	programs->separator2 = ',';
	programs->separator3 = ',';
	programs->temperature = 0x45;
	programs->separator4 = ',';
	programs->crc = programs->startHour + programs->startMinute + programs->endHour + \
					programs->endMinute + *((uint8_t*)&programs->days_of_the_week) + programs->temperature;
	programs->separator5 = '\n';
}

int make_file_data (void)
{
	const char *filepath = "/spiffs/programs_data.txt";
    FILE *fd = NULL;
    struct stat file_stat;
	
	if (stat(filepath, &file_stat) == -1) {
		gpio_set_level(BLINK_GPIO, 1);					// DEL
		for (int i=0; i<21; i++) {
			init_programs_data(&programs[i]);
		}
		fd = fopen(filepath, "w");
		if (!fd) {
			gpio_set_level(BLINK_GPIO, 1);
			return 1;
		}
		fwrite(&programs[0], sizeof(programData_t), 21, fd);
		fclose(fd);
		gpio_set_level(BLINK_GPIO, 0);					// DEL
	}
	
	fd = fopen(filepath, "r");
	if (!fd) {
		gpio_set_level(BLINK_GPIO, 1);
		return 2;
	}
	if (21 != fread(&programs[0], sizeof(programData_t), 21, fd)) {
		return 3;
	}
	
	for (int i=0; i<21; i++) {
		if (programs[i].crc != programs[i].startHour + programs[i].startMinute + programs[i].endHour + \
									programs[i].endMinute + *((uint8_t*)&programs[i].days_of_the_week) + programs[i].temperature) { 
			init_programs_data(&programs[i]);
		} else {
			//gpio_set_level(BLINK_GPIO, 1);
		}
	}
		
	return 0;
}

void ip_file(void) {
	const char *filepath = "/spiffs/ip_data.txt";
    FILE *fd = NULL;

	gpio_set_level(BLINK_GPIO, 1);
	fd = fopen(filepath, "w");
	fwrite(&programs[0], sizeof(programData_t), 21, fd);
	fwrite(&ipaddress, sizeof(uint32_t), 1, fd);
	fwrite(&ipnetmask, sizeof(uint32_t), 1, fd);
	fwrite(&ipgw, sizeof(uint32_t), 1, fd);
	fclose(fd);
	gpio_set_level(BLINK_GPIO, 0);
}

void app_main(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(BLINK_GPIO, 0);

    ESP_ERROR_CHECK(nvs_flash_init());
//    ESP_ERROR_CHECK(esp_netif_init());
//    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    //ESP_ERROR_CHECK(example_connect());
    ESP_ERROR_CHECK(wifi_init_apsta());

    /* Initialize file storage */
    ESP_ERROR_CHECK(init_spiffs());

    /* Start the file server */
    ESP_ERROR_CHECK(start_file_server("/spiffs"));
	
	uint8_t r = make_file_data ();
	ip_file();

}
