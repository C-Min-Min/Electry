/* HTTP File Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "main.h"
#include <sys/param.h>

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"


#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"
#include "esp_http_client.h"



#include "esp_spiffs.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/gpio.h"							// vld
#include "driver/uart.h"

#define EXAMPLE_AP_WIFI_SSID		"WRover"
#define EXAMPLE_AP_WIFI_PASS		""
#define EXAMPLE_MAX_STA_CONN		4
#define EXAMPLE_STA_WIFI_SSID		"Koko"
//#define EXAMPLE_STA_WIFI_SSID		"Bluebird"
//#define EXAMPLE_STA_WIFI_SSID		"Tech_D3881996"
//#define EXAMPLE_STA_WIFI_PASS		"vas1905vld"
//#define EXAMPLE_STA_WIFI_PASS		"FJHPEPJJ"
#define EXAMPLE_STA_WIFI_PASS		"KokoLia13"
#define EXAMPLE_STA_MAXIMUM_RETRY	5

#define MAX_EVENT_TAB 10
#define MAX_MEASUREMENT_TAB 50
#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)


/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


/* This example demonstrates how to create file server
 * using esp_http_server. This file has only startup code.
 * Look in file_server.c for the implementation */

static const char *TAG="example";
static const int RX_BUF_SIZE = 255;
uint8_t index_write_measurement_struct = 0, index_write_event_struct = 0, index_send_measurement_struct = 0, index_send_event_struct = 0, flag_send_measurement = 0, flag_send_event = 0;
#define URL "http://learningmoorree.000webhostapp.com/post-esp-data.php"


typedef struct event_table_struct{
	char consumer_id[5];
	char event_id[5];
	char power[10];
	char start_time[20];
}event_table;

typedef struct measurement_table_struct{
	char consumer_id[5];
	char event_id[5];
	char power[10];
	char start_time[20];
}measurement_table;

event_table event_struct[MAX_EVENT_TAB];
measurement_table measurement_struct[MAX_MEASUREMENT_TAB];

void prepare_buf_for_event_table(event_table *table, char *bufffer);
void prepare_buf_for_measurement_table(measurement_table *table, char *bufffer);
void send_request_to_mysql(char *buf);
//char check_uart_request_table_type(char *tx_buff);


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

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_STA_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		ipaddress = event->ip_info.ip.addr;
		ipnetmask = event->ip_info.netmask.addr;
		ipgw = event->ip_info.gw.addr;
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/* /////// esp_wifi_types.h ///////
 * typedef union {
 *    wifi_ap_config_t  ap;
 *    wifi_sta_config_t sta;
 * } wifi_config_t; */
esp_err_t wifi_init_apsta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

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

    ESP_LOGI(TAG, "wifi_init_apsta finished. SSID:%s password:%s",
             EXAMPLE_AP_WIFI_SSID, EXAMPLE_AP_WIFI_PASS);

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_STA_WIFI_SSID, EXAMPLE_STA_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_STA_WIFI_SSID, EXAMPLE_STA_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
 
	return ESP_OK;
}

/* Declare the function which starts the file server.
 * Implementation of this function is to be found in
 * file_server.c */
esp_err_t start_file_server(const char *base_path);

void init_programs_data (programData_t *programs)
{
	programs->startHour = 0;
	programs->startMinute = 0;
	programs->separator1 = ',';
	programs->endHour = 0;
	programs->endMinute = 0;
	programs->separator2 = ',';
	*((uint8_t*)&programs->days_of_the_week) = 0x00;
	programs->separator3 = ',';
	programs->temperature = 0x2D;
	programs->separator4 = ',';
	programs->crc = programs->startHour + programs->startMinute + programs->endHour + \
					programs->endMinute + *((uint8_t*)&programs->days_of_the_week) + programs->temperature;
	programs->separator5 = '\n';
}

int make_file_data (char *buf, uint8_t mode)
{
	if (mode == 0) {
		const char *filepath = "/spiffs/program_list.hex";
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
		if (fread(&programs[0], sizeof(programData_t), 21, fd) != 21) {
			gpio_set_level(BLINK_GPIO, 1);
			return 3;
		}
	
		for (int i=0; i<21; i++) {
			if (programs[i].crc != programs[i].startHour + programs[i].startMinute + programs[i].endHour + \
										programs[i].endMinute + *((uint8_t*)&programs[i].days_of_the_week) + programs[i].temperature) { 
				init_programs_data(&programs[i]);
			}
		}
		fclose(fd);
	} else if (mode == 1){
		char *r;
		uint8_t n;
		uint8_t m;
	
/*		if (stat(filepath, &file_stat) == -1) {
			for (int i=0; i<21; i++) {
				if (programs[i].crc != programs[i].startHour + programs[i].startMinute + programs[i].endHour + \
											programs[i].endMinute + *((uint8_t*)&programs[i].days_of_the_week) + programs[i].temperature) { 
					init_programs_data(&programs[i]);
				}
			}
		}*/
		r = strstr(buf, "program");
		if (r != NULL) {
			if(*(r+9) == '&') {
				n = (*(r+8) - 0x30);
			} else {
				n = ((*(r+8) - 0x30) * 10) + (*(r+9) - 0x30);
			}
			n--;
			r = strstr(buf, "start_time");
			if (r != NULL) {
				m = ((*(r+11) - 0x30) * 10) + (*(r+12) - 0x30);
				programs[n].startHour = m;
				m = ((*(r+16) - 0x30) * 10) + (*(r+17) - 0x30);
				programs[n].startMinute = m;
			}
			r = strstr(buf, "end_time");
			if (r != NULL) {
				m = ((*(r+9) - 0x30) * 10) + (*(r+10) - 0x30);
				programs[n].endHour = m;
				m = ((*(r+14) - 0x30) * 10) + (*(r+15) - 0x30);
				programs[n].endMinute = m;
			}

			r = strstr(buf, "mon");
			if (r != NULL) {
				*((uint8_t*)&programs[n].days_of_the_week) = 0;
				if (*(r+4) == 'y') programs[n].days_of_the_week.b_monday = 1;
				if (*(r+10) == 'y') programs[n].days_of_the_week.b_tuesday = 1;
				if (*(r+16) == 'y') programs[n].days_of_the_week.b_wednesday = 1;
				if (*(r+22) == 'y') programs[n].days_of_the_week.b_thursday = 1;
				if (*(r+28) == 'y') programs[n].days_of_the_week.b_friday = 1;
				if (*(r+34) == 'y') programs[n].days_of_the_week.b_saturday = 1;
				if (*(r+40) == 'y') programs[n].days_of_the_week.b_sunday = 1;
				if (*(r+46) == 'y') programs[n].days_of_the_week.b_hollidays = 1;
			}
			
			r = strstr(buf, "temperature");
			if (r != NULL) {
				m = ((*(r+12) - 0x30) * 10) + (*(r+13) - 0x30);
				programs[n].temperature = m;
			}
			programs[n].crc = programs[n].startHour + programs[n].startMinute + programs[n].endHour + \
											programs[n].endMinute + *((uint8_t*)&programs[n].days_of_the_week) + programs[n].temperature;
		}

	}
		
	return Data_OK;
}

void ip_file(void) {
	const char *filepath = "/spiffs/ip_data.hex";
    FILE *fd = NULL;
	

	fd = fopen(filepath, "w");
	fwrite(&ipaddress, sizeof(uint32_t), 1, fd);
	fwrite(&ipnetmask, sizeof(uint32_t), 1, fd);
	fwrite(&ipgw, sizeof(uint32_t), 1, fd);
	fclose(fd);
}

char* convert_uint8_charchar(char *buf, uint8_t *number){
	uint8_t ones = 0;
    uint8_t tens= 0;
    buf[2] = 0;
  
    if(*number >= 10){
        tens = (int)(*number / 10);
        ones = *number - (tens * 10);
        buf[0] = tens + 0x30;
    }else{
		buf[0] = 0x30;
		ones = *number;
	}

	buf[1] = ones + 0x30;
	return buf;
}

char* convert_hex_charchar(char *buf, uint8_t *number){
	uint8_t k = *number;
    buf[2] = 0;

	k = (k >> 4) & 0x0f;
	if (k < 0x0a) {
		buf[0] = k + 0x30;
	} else {
		buf[0] = k + 0x37;
	}
	
	k = *number & 0x0f;
	if (k < 0x0a) {
		buf[1] = k + 0x30;
	} else {
		buf[1] = k + 0x37;
	}

	return buf;
}



/*esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
			ESP_LOGI(TAG, "------------------------------------------");
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
			if(strstr(evt->data, "New record created successfully") != NULL){
				ESP_LOGI(TAG, "We have a new record on MySql Database!");
			}
			if (!esp_http_client_is_chunked_response(evt->client)) {
                ESP_LOGI(TAG, "Data length: %d", evt->data_len);
				ESP_LOGI(TAG, "\n%s", (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}*/

esp_http_client_config_t config = {
	.url = URL
};

void prepare_buf_for_event_table(event_table *table, char *bufffer){
	strcpy(bufffer, "api_key=tPmAT5Ab3j7F9&command=event_table&consumer_id=");
	strcat(bufffer, table->consumer_id);
	strcat(bufffer, "&event_id=");
	strcat(bufffer, table->event_id);
}

void prepare_buf_for_measurement_table(measurement_table *table, char *bufffer){
	strcpy(bufffer, "api_key=tPmAT5Ab3j7F9&command=measurement_table&consumer_id=");
	strcat(bufffer, table->consumer_id);
	strcat(bufffer, "&event_id=");
	strcat(bufffer, table->event_id);
	strcat(bufffer, "&start_time=");
	strcat(bufffer, table->start_time);
	strcat(bufffer, "&power=");
	strcat(bufffer, table->power);
	ESP_LOGI(TAG, "Buffer: %s   datetime: %s", bufffer, table->start_time);
}

void send_request_to_mysql(char *buf){
	esp_http_client_handle_t client = esp_http_client_init(&config);
	esp_http_client_set_method(client, HTTP_METHOD_POST);
	esp_http_client_set_post_field(client, buf, strlen(buf));

	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGI(TAG, "Status = %d, content_length = %d", 
		    esp_http_client_get_status_code(client), 
		    esp_http_client_get_content_length(client));
	}
	esp_http_client_cleanup(client);	
}


int sendData(const char* logName, const char* data){
    const int txBytes = uart_write_bytes(UART_NUM_1, data, strlen(data));
    ESP_LOGI(TAG, "Wrote %d bytes; MESSAGE = \'%s\'", txBytes, data);
    return txBytes;
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK"; 
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
	
    while(1){
        char data[RX_BUF_SIZE], *data_p = data;
        int rxBytes = uart_read_bytes(UART_NUM_1, (uint8_t*)data_p, RX_BUF_SIZE, 1 / portTICK_RATE_MS);
        if(rxBytes > 0){
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s' send flag'%d' write flag'%d'", rxBytes, data, index_send_measurement_struct, index_write_measurement_struct);
			
			/**//**/
			char uart_request_elements[20], header_info[9][20];
			for(int i=0;i<9;i++)  strcpy(header_info[i], "");
	
			strcpy(uart_request_elements, strtok(data_p, ";"));
			strcpy(header_info[0], uart_request_elements);
			for(int i = 1; i < 9; i++){
				ESP_LOGI(TAG, "Data content:  %s", header_info[i-1]);
				strcpy(uart_request_elements, strtok(NULL, ";"));
				strcpy(header_info[i], uart_request_elements);				
			}
			
			if(!strcmp(header_info[1], "0")){
				if(!(strcmp(header_info[4], "0"))){
					if((index_write_measurement_struct != index_send_measurement_struct) || index_write_measurement_struct == 0){
						strcat(measurement_struct[index_write_measurement_struct].consumer_id, header_info[2]);
						strcat(measurement_struct[index_write_measurement_struct].event_id, header_info[3]);
						strcat(measurement_struct[index_write_measurement_struct].start_time, header_info[5]);
						strcat(measurement_struct[index_write_measurement_struct].power, header_info[6]);
						
						index_write_measurement_struct++;
						if(index_write_measurement_struct == 50)  index_write_measurement_struct = 0;
						if(index_write_measurement_struct == index_send_measurement_struct){
							if(!index_write_measurement_struct)  index_write_measurement_struct = 49;
							else  index_write_measurement_struct--;
						}
						flag_send_measurement++;
						sendData(TAG, "DATA LOADED!~");
					}else  ESP_LOGI(TAG, "MEASUREMENTS ERROR CONNECTION!~%d  %d", index_write_measurement_struct, index_send_measurement_struct);
				}else if(!(strcmp(header_info[4], "1"))){
					if((index_write_event_struct != index_send_event_struct) || !index_write_event_struct){
						strcat(event_struct[index_write_event_struct].consumer_id, header_info[2]);
						strcat(event_struct[index_write_event_struct].event_id, header_info[3]);
						strcat(event_struct[index_write_event_struct].start_time, header_info[5]);
						strcat(event_struct[index_write_event_struct].power, header_info[6]);

						if(++index_write_event_struct == 10)  index_write_event_struct = 0;
						if(index_write_event_struct == index_send_event_struct){
							if(!index_write_event_struct)  index_write_event_struct = 49;
							else  index_write_event_struct--;
						}
						flag_send_event++;
						sendData(TAG, "DATA LOADED!~");
					}else  sendData(TAG, "EVENT ERROR NOT ENOUGH SPACE!~");
				}else  sendData(TAG, header_info[4]);
			}else if(!(strcmp(header_info[1], "1"))){
				char transmit_indexes[15];
				sprintf(transmit_indexes, "%d%d%d%d!~", index_write_measurement_struct, index_write_event_struct, index_send_measurement_struct, index_send_event_struct);
				sendData(TAG, (char*)transmit_indexes);
			}else  sendData(TAG, "WHY WHY WHY ERROR CONNECTION!~");
			rxBytes = 0;
			/**//**/
			strcpy(data, "");
			for(int i = 0; i < 8; i++)  strcpy(header_info[i], "");
			//ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
		}
	}
}

void app_main(void){
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
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    //ESP_ERROR_CHECK(example_connect());
    ESP_ERROR_CHECK(wifi_init_apsta());

    /* Initialize file storage */
    ESP_ERROR_CHECK(init_spiffs());

    /* Start the file server */
//    ESP_ERROR_CHECK(start_file_server("/spiffs"));
//	make_file_data(NULL, 0);
//	ip_file();


    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	ESP_LOGI(TAG, "Trying to read UART...");
    xTaskCreate(rx_task, "uart_rx_task", 2600, NULL, 1|portPRIVILEGE_BIT, NULL);

	for(int i = 0; i < 50; i++){
		strcpy(measurement_struct[i].consumer_id, "");
		strcpy(measurement_struct[i].event_id, "");
		strcpy(measurement_struct[i].start_time, "");
		strcpy(measurement_struct[i].power, "");
		if(i < 10){
    		strcpy(event_struct[i].consumer_id, "");
			strcpy(event_struct[i].event_id, "");
			strcpy(event_struct[i].start_time, "");
			strcpy(event_struct[i].power, "");
		}
	}


	///Configuations///
	///Send requests///
  while(1){
	char buf[200];
	if(index_send_event_struct != index_write_event_struct && flag_send_event){
//		ESP_LOGI(TAG, "\nEVENT: %s %s %s %s", event_struct[index_send_event_struct].consumer_id, event_struct[index_send_event_struct].event_id, event_struct[index_send_event_struct].start_time, event_struct[index_send_event_struct].power);
		prepare_buf_for_event_table(&event_struct[index_send_event_struct], (char*)&buf);		
		send_request_to_mysql((char*)&buf);
		strcpy(event_struct[index_send_event_struct].consumer_id, "");
		strcpy(event_struct[index_send_event_struct].event_id, "");
		strcpy(event_struct[index_send_event_struct].start_time, "");
		strcpy(event_struct[index_send_event_struct].power, "");

        if(index_send_event_struct+1 == index_write_event_struct);
		else if(index_send_event_struct+1 == 9)  index_send_event_struct = 0;
		else  ++index_send_event_struct;
		flag_send_event--;
	}

	if(index_send_measurement_struct != index_write_measurement_struct && !flag_send_event && flag_send_measurement){
//		ESP_LOGI(TAG, "\nMEASUREMENT: %s %s %s %s", measurement_struct[index_send_measurement_struct].consumer_id, measurement_struct[index_send_measurement_struct].event_id, measurement_struct[index_send_measurement_struct].start_time, measurement_struct[index_send_measurement_struct].power);
		prepare_buf_for_measurement_table(&measurement_struct[index_send_measurement_struct], (char*)&buf);
		send_request_to_mysql((char*)&buf);
		strcpy(measurement_struct[index_send_measurement_struct].consumer_id, "");
		strcpy(measurement_struct[index_send_measurement_struct].event_id, "");
		strcpy(measurement_struct[index_send_measurement_struct].start_time, "");
		strcpy(measurement_struct[index_send_measurement_struct].power, "");

        if(index_send_measurement_struct+1 == index_write_measurement_struct);
		else if(index_send_measurement_struct+1 == 49)  index_send_measurement_struct = 0;
		else  ++index_send_measurement_struct;
		flag_send_measurement--;
	}
	strcpy(buf, "");
	/*	prepare_buf_for_event_table(&event_struct[current_event_tables_index], buf);
		send_request_to_mysql(buf);

		prepare_buf_for_measurement_table(&measurement_struct[current_measurement_tables_index], buf);
		send_request_to_mysql(buf);*/
  }
}