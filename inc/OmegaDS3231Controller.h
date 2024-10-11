/**
 * @file OmegaDS3231MController.h
 * @author Omegaki113r
 * @date Wednesday, 9th October 2024 11:52:08 am
 * @copyright Copyright <<projectCreationYear>> - 2024 0m3g4ki113r, Xtronic
 * */
/*
 * Project: inc
 * File Name: OmegaDS3231MController.h
 * File Created: Wednesday, 9th October 2024 11:52:08 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 11th October 2024 4:07:23 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2024 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#ifndef __OMEGA_DS3231M_CONTROLLER_H__
#define __OMEGA_DS3231M_CONTROLLER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdlib.h>

/*NOTE -
    [ 7     6 5 4       3 2 1 0 ]
      x     10 seconds  seconds
*/
#define ADDRESS_SECONDS 0x00
/*NOTE -
    [ 7     6 5 4       3 2 1 0 ]
      x     10 minutes  minutes
*/
#define ADDRESS_MINUTES 0x01
/*NOTE -
    [ 7     6       5       4           3 2 1 0 ]
      x     12/24   AM/PM   10 hours    hour
*/
#define ADDRESS_HOURS 0x02
/*NOTE -
    [ 7 6 5 4 3     2 1 0 ]
      x x x x x     day
*/
#define ADDRESS_DAY 0x03
/*NOTE -
    [ 7 6   5 4         3 2 1 0 ]
      x x   10 date     date
*/
#define ADDRESS_DATE 0x04
/*NOTE -
    [ 7         6 5     4           3 2 1 0 ]
      century   x x     10 month    month
*/
#define ADDRESS_MONTH 0x05
/*NOTE -
    [ 7 6 5 4   3 2 1 0 ]
      10 year   year
*/
#define ADDRESS_YEAR 0x06
/*NOTE -
    [ 7     6 5 4       3 2 1 0 ]
      A1M1  10 seconds  seconds
*/
#define ADDRESS_ALARM1_SECOND 0x07
/*NOTE -
    [ 7     6 5 4       3 2 1 0 ]
      A1M2  10 minutes  minutes
*/
#define ADDRESS_ALARM1_MINUTE 0x08
/*NOTE -
    [ 7     6       5       4           3 2 1 0 ]
      A1M3  12/24   AM/PM   10 hours    hour
*/
#define ADDRESS_ALARM1_HOUR 0x09
/*NOTE -
    [ 7     6       5 4         3 2 1 0 ]
      A1M4  DY/DT   10 date     day/date
*/
#define ADDRESS_ALARM1_DAY 0x0A
/*NOTE -
    [ 7     6 5 4       3 2 1 0 ]
      A1M2  10 minutes  minutes
*/
#define ADDRESS_ALARM2_MINUTES 0x0B
/*NOTE -
    [ 7     6       5       4           3 2 1 0 ]
      A1M3  12/24   AM/PM   10 hours    hour
*/
#define ADDRESS_ALARM2_HOURS 0x0C
/*NOTE -
    [ 7     6       5 4         3 2 1 0 ]
      A1M4  DY/DT   10 date     day/date
*/
#define ADDRESS_ALARM2_DAYS 0x0D
/*NOTE -
    [ 7         6       5       4 3 2       1       0 ]
      ~EOSC     BBSQW   CONV    x x INTCN   A2IE    A1IE
*/
#define ADDRESS_CONTROL 0x0E
/*NOTE -
    [ 7     6 5 4 3         2       1       0 ]
      OSF   x x x EN32KHZ   BSY     A2F     A1F
*/
#define ADDRESS_STATUS 0x0F
/*NOTE -
    [ 7     6 5 4 3 2 1 0 ]
      SIGN  DATA
*/
#define ADDRESS_AGING_OFFSET 0x10
/*NOTE -
    [ 7     6 5 4 3 2 1 0 ]
      SIGN  DATA
*/
#define ADDRESS_MSB_TEMPERATURE 0x11
/*NOTE -
    [ 7 6   5 4 3 2 1 0 ]
      DATA  x x x x x x
*/
#define ADDRESS_LSB_TEMPERATURE 0x12

  typedef enum
  {
    AM,
    PM
  } TimeFormat_t;
  typedef int8_t i8;
  typedef uint8_t u8;
  typedef uint16_t u16;
  typedef struct
  {
    struct
    {
      i8 hour_24;
      i8 hour_12;
      u8 minute;
      u8 second;
      TimeFormat_t which;

      u8 year;
      u8 month;
      u8 date;
    };
  } Time_t;

  void OmegaDS3231Controller_register_write_callback(void (*cb)(u8 *, size_t, void *));
  void OmegaDS3231Controller_register_read_callback(void (*cb)(u8 *, size_t, u8 *, size_t, void *));
  void OmegaDS3231Controller_set_time_24(u8 hour, u8 minute, u8 second);
  void OmegaDS3231Controller_set_time_12(u8 hour, u8 minute, u8 second, TimeFormat_t which);
  void OmegaDS3231Controller_set_date(u8 year, u8 month, u8 day);
  Time_t OmegaDS3132Controller_get_time();

#ifdef __cplusplus
}
#endif

#endif //__OMEGA_DS3231M_CONTROLLER_H__