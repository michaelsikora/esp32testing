/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Thomas Barth, barth-dev.de
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * \version 0.1
 * \brief A basic WebSocket Server Espressif ESP32
 *
 * Within this demo, a very basic WebSocket server is created, which loops back WebSocket messages with a maximum length of 125 bytes.
 * \see http://www.barth-dev.de/websockets-on-the-esp32
 */
#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#include "WebSocket_Task.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_adc_cal.h"

#include <sys/fcntl.h>
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#define AP_SSID "ESP_32"
#define AP_PASSPHARSE "123456789"
#define AP_SSID_HIDDEN 0
#define AP_MAX_CONNECTIONS 4
#define AP_AUTHMODE WIFI_AUTH_WPA2_PSK // the passpharese should be atleast 8 chars long
#define AP_BEACON_INTERVAL 100 // in milli seconds
#define LISTENQ 2
#define MESSAGE "Hello TCP Client!!"

const int CLIENT_CONNECTED_BIT = BIT0;
const int CLIENT_DISCONNECTED_BIT = BIT1;
const int AP_STARTED_BIT = BIT2;
//WebSocket frame receive queue
QueueHandle_t WebSocket_rx_queue;

/*Note: Different ESP32 modules may have different reference voltages varying from
 * 1000mV to 1200mV. Use #define GET_VREF to route v_ref to a GPIO
 */
#define V_REF   1100
#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_6)      //GPIO 34
//#define V_REF_TO_GPIO  //Remove comment on define to route v_ref to GPIO

void task_process_WebSocket( void *pvParameters ){
    (void)pvParameters;

    //frame buffer
    WebSocket_frame_t __RX_frame;
    WebSocket_frame_t __TX_frame;
    float voltage = 261.5;

    //create WebSocket RX Queue
    WebSocket_rx_queue = xQueueCreate(10,sizeof(WebSocket_frame_t));

    while (1){
        //receive next WebSocket frame from queue
        if(xQueueReceive(WebSocket_rx_queue,&__RX_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

        	//write frame information to UART
        	printf("New Websocket frame. Length %d, payload %.*s \r\n", __RX_frame.payload_length, __RX_frame.payload_length, __RX_frame.payload);

        	__TX_frame = __RX_frame;
        	__TX_frame.payload_length = 50;
        	int ret = snprintf(__TX_frame.payload,50 , "The voltage is %f", voltage);

        	//loop back frame
        	WS_write_data(__TX_frame.payload, __TX_frame.payload_length);

        	//free memory
			if (__RX_frame.payload != NULL)
				free(__RX_frame.payload);

        }
    }
}

static void initialise_wifi_in_ap(void)
{
    esp_log_level_set("wifi", ESP_LOG_NONE); // disable wifi driver logging
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );

    // configure the wifi connection and start the interface
	wifi_config_t ap_config = {
        .ap = {
            .ssid = AP_SSID,
            .password = AP_PASSPHARSE,
			.ssid_len = 0,
			.channel = 0,
			.authmode = AP_AUTHMODE,
			.ssid_hidden = AP_SSID_HIDDEN,
			.max_connection = AP_MAX_CONNECTIONS,
			.beacon_interval = AP_BEACON_INTERVAL,
        },
    };
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK( esp_wifi_start() );
    printf("ESP WiFi started in AP mode \n");
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{

    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
        ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
        ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
        wifi_config_t sta_config = {
            .sta = {
                .ssid = "WifiSSD",
                .password = "WifiPass",
                .bssid_set = false
            }
        };
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
        ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    //create WebSocker receive task
    xTaskCreate(&task_process_WebSocket, "ws_process_rx", 2048, NULL, 5, NULL);

    //Create Websocket Server Task
    xTaskCreate(&ws_server, "ws_server", 2048, NULL, 5, NULL);

}
