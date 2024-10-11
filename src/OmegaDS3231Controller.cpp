/**
 * @file OmegaDS3231MController.cpp
 * @author Omegaki113r
 * @date Wednesday, 9th October 2024 11:54:43 am
 * @copyright Copyright <<projectCreationYear>> - 2024 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: OmegaDS3231MController.cpp
 * File Created: Wednesday, 9th October 2024 11:54:43 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 11th October 2024 4:08:11 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include "stdio.h"

#include "OmegaDS3231Controller.h"

#define CURRENT_BASE_YEAR 2000
#define internal static

internal u8 bcdToDec(u8 val)
{
    return ((val / 16 * 10) + (val % 16));
}

internal u8 decToBcd(u8 val)
{
    return ((val / 10 * 16) + (val % 10));
}

typedef void (*write_cb)(u8 *, size_t, void *);
typedef void (*read_cb)(u8 *, size_t, u8 *, size_t, void *);

internal write_cb write_callback;
internal read_cb read_callback;

void OmegaDS3231Controller_register_write_callback(void (*cb)(u8 *buffer, size_t size, void *user_data))
{
    if (cb != NULL)
        write_callback = cb;
}
void OmegaDS3231Controller_register_read_callback(void (*cb)(u8 *buffer, size_t size, u8 *buffer1, size_t size1, void *user_data))
{
    if (cb != NULL)
        read_callback = cb;
}

void OmegaDS3231Controller_set_time_24(u8 hour, u8 minute, u8 second)
{
    u8 write_buffer[2] = {0};

    write_buffer[0] = ADDRESS_HOURS;
    write_buffer[1] = decToBcd(hour);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_MINUTES;
    write_buffer[1] = decToBcd(minute);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_SECONDS;
    write_buffer[1] = decToBcd(second);
    write_callback(write_buffer, 2, NULL);
}

void OmegaDS3231Controller_set_time_12(u8 hour, u8 minute, u8 second, TimeFormat_t which)
{
    u8 write_buffer[2] = {0};

    write_buffer[0] = ADDRESS_HOURS;
    write_buffer[1] = 1 << 6 | which << 5 | decToBcd(hour);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_MINUTES;
    write_buffer[1] = decToBcd(minute);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_SECONDS;
    write_buffer[1] = decToBcd(second);
    write_callback(write_buffer, 2, NULL);
}

void OmegaDS3231Controller_set_date(u8 year, u8 month, u8 day)
{
    u8 write_buffer[2] = {0};

    write_buffer[0] = ADDRESS_YEAR;
    write_buffer[1] = decToBcd(year);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_MONTH;
    write_buffer[1] = decToBcd(month);
    write_callback(write_buffer, 2, NULL);

    write_buffer[0] = ADDRESS_DATE;
    write_buffer[1] = decToBcd(day);
    write_callback(write_buffer, 2, NULL);
}

Time_t OmegaDS3132Controller_get_time()
{
    Time_t current_time = {0};
    u8 transceive_buffer[2] = {0};

    transceive_buffer[0] = ADDRESS_HOURS;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    if (transceive_buffer[1] >> 6 & 0x01)
    {
        if (transceive_buffer[1] >> 5 & 0x01)
        {
            current_time.which = PM;
        }
        else
        {
            current_time.which = AM;
        }
        current_time.hour_12 = bcdToDec(transceive_buffer[1] & 0b00011111);
        current_time.hour_24 = -1;
    }
    else
    {
        current_time.hour_24 = bcdToDec(transceive_buffer[1] & 0b00011111);
        current_time.hour_12 = -1;
    }

    transceive_buffer[0] = ADDRESS_MINUTES;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    current_time.minute = bcdToDec(transceive_buffer[1]);

    transceive_buffer[0] = ADDRESS_SECONDS;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    current_time.second = bcdToDec(transceive_buffer[1]);

    transceive_buffer[0] = ADDRESS_YEAR;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    current_time.year = bcdToDec(transceive_buffer[1]);

    transceive_buffer[0] = ADDRESS_MONTH;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    current_time.month = bcdToDec(transceive_buffer[1]);

    transceive_buffer[0] = ADDRESS_DATE;
    read_callback(&transceive_buffer[0], 1, &transceive_buffer[1], 1, NULL);
    current_time.date = bcdToDec(transceive_buffer[1]);

    return current_time;
}