//
//  raspi_servo_driver.cpp
//  splashkit
//
// Created by Aditya Parmar on 14/05/2025.
// Copyright © 2024 XQuestCode. All rights reserved.
//

#include "raspi_servo_driver.h"
#include "raspi_gpio.h" // for raspi_init()

#include <algorithm> // for std::clamp

namespace splashkit_lib
{
  struct _servo_data
  {
    pointer_identifier id;
    std::string name;
    gpio_pin pin;
    double min_angle, max_angle;
  };

  static std::map<std::string, servo_device> _servo_devices;
  // Pulse widths for the min and max values of servo range
  static const unsigned MIN_PW = 500;
  static const unsigned MAX_PW = 2500;

  bool has_servo_device(const std::string &name)
  {
    return _servo_devices.count(name) > 0;
  }

  servo_device servo_named(const std::string &name)
  {
    auto it = _servo_devices.find(name);
    return (it != _servo_devices.end()) ? it->second : nullptr;
  }

  servo_device open_servo(const std::string &name, gpio_pin control_pin, double min_angle = 0, double max_angle = 180)
  {
#ifdef RASPBERRY_PI
    if (has_servo_device(name))
      return servo_named(name);

    // ensure pigpio is initialized
    raspi_init();

    auto dev = new _servo_data();
    dev->id = SERVO_DRIVER_PTR; // defined SERVO_DRIVER_PTR in backend_types.h
    dev->name = name;
    dev->pin = control_pin;
    dev->min_angle = min_angle;
    dev->max_angle = max_angle;

    // configure as output
    raspi_set_mode(control_pin, GPIO_OUTPUT);
    // configure the pin for 50 Hz PWM, range = 20000 units ? 1 unit = 1 µs
    raspi_set_pwm_frequency(control_pin, 50);
    raspi_set_pwm_range(control_pin, 20000);
    // stop any pulses initially
    raspi_set_pwm_dutycycle(control_pin, 0);

    _servo_devices[name] = dev;
    return dev;
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
    return nullptr;
#endif
  }

  void set_servo_value(servo_device dev, double value)
  {
#ifdef RASPBERRY_PI
    if (!dev || dev->id != SERVO_DRIVER_PTR)
      return;

    // input value is 0..1
    // scale to servo pulse width range
    unsigned pw = static_cast<unsigned>(value * (MAX_PW - MIN_PW) + MIN_PW);
    raspi_set_servo_pulsewidth(dev->pin, pw);
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }

  void set_servo_angle(servo_device dev, double angle)
  {
#ifdef RASPBERRY_PI
    if (!dev || dev->id != SERVO_DRIVER_PTR)
      return;

    double min = dev->min_angle;
    double max = dev->max_angle;
    
    // clamp input to servo range
    angle = std::clamp(angle, min, max);
    // scale angle to pulse width range
    unsigned pw = static_cast<unsigned>((angle - min / max - min) * (MAX_PW - MIN_PW) + MIN_PW);
    raspi_set_servo_pulsewidth(dev->pin, pw);
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }

  void stop_servo(servo_device dev)
  {
#ifdef RASPBERRY_PI
    if (!dev || dev->id != SERVO_DRIVER_PTR)
      return;
    // 0 µs stops pulses
    raspi_set_pwm_dutycycle(dev->pin, 0);
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }

  void close_servo(servo_device dev)
  {
#ifdef RASPBERRY_PI
    if (!dev || dev->id != SERVO_DRIVER_PTR)
      return;
    // make sure pulses are off
    raspi_set_pwm_dutycycle(dev->pin, 0);
    _servo_devices.erase(dev->name);
    delete dev;
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }

  void close_servo(const std::string &name)
  {
#ifdef RASPBERRY_PI
    auto dev = servo_named(name);
    if (dev)
      close_servo(dev);
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }

  void close_all_servos()
  {
#ifdef RASPBERRY_PI
    for (auto &kv : _servo_devices)
    {
      auto dev = kv.second;
      raspi_set_pwm_dutycycle(dev->pin, 0);
      delete dev;
    }
    _servo_devices.clear();
#else
    LOG(ERROR) << "Servo driver not supported on this platform";
#endif
  }
}
