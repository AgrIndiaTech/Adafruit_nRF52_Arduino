/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org) for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "BLEAdafruitService.h"
#include <Adafruit_AHRS.h>

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+


/* All Adafruit Service/Characteristic UUID128 share the same Base UUID:
 *    ADAFxxx-C332-42A8-93BD-25E905756CB8
 *
 * Shared Characteristics
 *  - Measurement Period  0001 | int32_t | Read + Write |
 *    ms between measurements, -1: stop reading, 0: update when changes
 *
 * Quaternion service  0D00
 *  - Quater              0D01 | float[4] | Read + Notify | qw, qx, qy, qz
 *  - Measurement Period  0001
 */

const uint8_t BLEAdafruitQuaternion::UUID128_SERVICE[16] =
{
  0xB8, 0x6c, 0x75, 0x05, 0xE9, 0x25, 0xBD, 0x93,
  0xA8, 0x42, 0x32, 0xC3, 0x00, 0x0D, 0xAF, 0xAD
};

const uint8_t BLEAdafruitQuaternion::UUID128_CHR_DATA[16] =
{
  0xB8, 0x6c, 0x75, 0x05, 0xE9, 0x25, 0xBD, 0x93,
  0xA8, 0x42, 0x32, 0xC3, 0x01, 0x0D, 0xAF, 0xAD
};

// Constructor
BLEAdafruitQuaternion::BLEAdafruitQuaternion(void)
  : BLEAdafruitSensor(UUID128_SERVICE, UUID128_CHR_DATA)
{
  _accel = _gyro = _mag = NULL;
  _filter = NULL;

  // Setup Measurement Characteristic
  _measurement.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  _measurement.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  _measurement.setFixedLen(4*4);
}

err_t BLEAdafruitQuaternion::begin(Adafruit_AHRS_FusionInterface* filter, Adafruit_Sensor* accel, Adafruit_Sensor* gyro, Adafruit_Sensor* mag)
{
  _filter = filter;
  _accel = accel;
  _gyro  = gyro;
  _mag   = mag;

  // Invoke base class begin(), this will add Service, Measurement and Period characteristics
  VERIFY_STATUS( BLEAdafruitSensor::_begin(DEFAULT_PERIOD) );

  return ERROR_NONE;
}

// Invoked by period timer in Base class
// Note invoked in RTOS Timer thread
void BLEAdafruitQuaternion::_measure_handler(void)
{
  // TODO multiple connections
  _measurement.notify("test", 4);

//  // get sensor
//  sensors_event_t accel_evt, gyro_evt, mag_evt;
//
//  _accel->getEvent(&accel_evt);
//  _gyro->getEvent(&gyro_evt);
//  _mag->getEvent(&mag_evt);
//
//  // calibrate
//
//
//  // Convert gyro from Rad/s to Degree/s
//  gyro_evt.gyro.x *= SENSORS_RADS_TO_DPS;
//  gyro_evt.gyro.y *= SENSORS_RADS_TO_DPS;
//  gyro_evt.gyro.z *= SENSORS_RADS_TO_DPS;
//
//  // apply filter, update 10 times before notify
//  filter->update(gyro_evt.gyro.x , gyro_evt.gyro.y, gyro_evt.gyro.z,
//                 accel_evt.acceleration.x, accel_evt.acceleration.y, accel_evt.acceleration.z,
//                 mag_evt.magnetic.x, mag_evt.magnetic.y, mag_evt.magnetic.z);
}
