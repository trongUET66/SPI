// Slave as receiver for SPI communitation

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "driver/spi_slave.h"
// #include "esp_log.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

// Pins in use
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 5

int i;

//Main application
void app_main(void)
{
    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,
        .spics_io_num=GPIO_CS,
        .queue_size=3,
        .flags=0,
    };

    //Initialize SPI slave interface
    spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);

    char recvbuf[129]="";
    char sendbuf[128] = {0};
    memset(recvbuf, 0, 33);
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));

    while(1) {
        t.length=128*8;
        t.rx_buffer=recvbuf;
        spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
        printf("Received: %s\n", recvbuf);

        snprintf(sendbuf, sizeof(sendbuf), "Sent by slave - %d", i);
        t.length = sizeof(sendbuf) * 8;
        t.tx_buffer = sendbuf;
        spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
        printf("Transmitted: %s\n", sendbuf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        i++;
    }
}