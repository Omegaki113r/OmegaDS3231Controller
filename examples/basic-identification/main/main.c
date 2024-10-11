#include <stdio.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/gpio.h>
#include <driver/i2c_master.h>

#include "OmegaDS3231Controller.h"

#define internal static
#define Delay(x) vTaskDelay(pdMS_TO_TICKS(x))
#define Delay_ms(x) Delay(x)

#define DS3231M_SCL_SPEED_KHZ 100
#define DS3231M_SDA GPIO_NUM_17
#define DS3231M_SCL GPIO_NUM_18

internal i2c_master_bus_handle_t s_i2c_master_handle;
internal i2c_master_dev_handle_t s_touch_device_handle;

extern const uint8_t developer_txt_start[] asm("_binary_developer_txt_start");
extern const uint8_t developer_txt_end[] asm("_binary_developer_txt_end");

uint8_t bcdToDec(uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}

uint8_t decToBcd(uint8_t val)
{
    // Convert normal decimal numbers to binary coded decimal
    return ((val / 10 * 16) + (val % 10));
}

void hellord(uint8_t *buffer, size_t buffer_size, void *user_data)
{
    i2c_master_transmit(s_touch_device_handle, buffer, buffer_size, portMAX_DELAY);
}
void hellowr(uint8_t *buffer, size_t buffer_size, uint8_t *buffer1, size_t buffer_size1, void *user_data)
{
    i2c_master_transmit_receive(s_touch_device_handle, buffer, buffer_size, buffer1, buffer_size1, portMAX_DELAY);
}

void app_main(void)
{
    printf("%s\n", developer_txt_start);

    OmegaDS3231Controller_register_read_callback(hellowr);
    OmegaDS3231Controller_register_write_callback(hellord);

    { /*SECTION - Configure I2C */
        i2c_master_bus_config_t i2c_master_bus_config = {
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .i2c_port = I2C_NUM_0,
            .scl_io_num = DS3231M_SCL,
            .sda_io_num = DS3231M_SDA,
            // .flags.enable_internal_pullup = 1,
        };
        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_bus_config, &s_i2c_master_handle));

        { /* SECTION - Scanning I2C bus*/
            printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
            printf("00:         ");
            for (uint8_t i = 3; i < 0x78; i++)
            {
                esp_err_t res = i2c_master_probe(s_i2c_master_handle, i, pdMS_TO_TICKS(500));
                if (i % 16 == 0)
                    printf("\n%.2x:", i);
                if (res == 0)
                {
                    printf(" %.2x", i);
                }
                else
                {
                    printf(" --");
                }
            }
            printf("\n");
        }

        i2c_device_config_t i2c_touch_device_config = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = 0x68,
            .scl_speed_hz = 400 * 1000,
        };
        ESP_ERROR_CHECK(i2c_master_bus_add_device(s_i2c_master_handle, &i2c_touch_device_config, &s_touch_device_handle));

        OmegaDS3231Controller_set_date(24, 10, 11);
        OmegaDS3231Controller_set_time_12(2, 30, 0, PM);
    }

    for (;;)
    {
        Time_t time = OmegaDS3132Controller_get_time();
        printf("%.4d/%.2d/%.2d\t%.2d:%.2d:%.2d %s\n",
               2000 + time.year,
               time.month,
               time.date,
               time.hour_12,
               time.minute,
               time.second,
               time.which == PM ? "PM" : "AM");
        Delay_ms(1000);
    }
}