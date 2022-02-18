#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"

#define TAG "main"

typedef struct {
    uint8_t event;
    char* data;
    uint32_t size;
} main_event_t;  

TimerHandle_t statusTimer;
TimerHandle_t pidTimer;
xQueueHandle main_queue;

void main_loop(void* data)
{
    while (1) 
    {
        main_event_t evt;
        xQueueReceive(main_queue, &evt, portMAX_DELAY);
    }
}

void app_main(void)
{

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    nvs_flash_init();

    // Example on how to load raw partition data to ram: https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_mmap/main/main.c
    //part=  esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "data");


    main_queue = xQueueCreate(10, sizeof(main_event_t));
    esp_event_loop_create_default();
    wifi_init();
    xTaskCreate(main_loop,"main loop", 4096,0,10,0);
}


