/*
 * Title      :   xcfg_test.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <xcfg/xcfg.h>
#include <arch_config.h>

int main(int argc, char *argv[]) {
  std::cout << "The xcfg test" << "\n";
  std::cout << "Copyright (C) 2021 Jaewoong Jang" << "\n";

  xcfg_t cfg;
  cfg.init();

  // tesla.xml
  cfg.set_model_name("modelx");
  std::cout << "model name: " << cfg.get_model_name() << "\n";
  cfg.set_model_number(777);
  std::cout << "model number: " << cfg.get_model_number() << "\n";
  std::cout << "model uuid: " << cfg.get_model_uuid() << "\n";
  std::cout << "project name: " << cfg.get_project_name() << "\n";
  std::cout << "platform name: " << cfg.get_platform_name() << "\n";
  std::cout << "battery validation: " << cfg.get_battery_validation() << "\n";
  std::cout << "system clock: " << cfg.get_system_clock_us() << "\n";
  std::cout << "algorithm period: " << cfg.get_algorithm_period_us() << "\n";

  // battery.xml
  cfg.set_autopilot_uuid();
  std::cout << "battery uuid left: " << cfg.get_autopilot_uuid() << "\n";
  cfg.set_autopilot_type(777);
  std::cout << "battery type left: " << cfg.get_autopilot_type() << "\n";
  uint64_t now = cfg.get_current_timestamp_ms();
  cfg.set_autopilot_timestamp_ms(now);
  std::cout << "battery timestamp ms left: " << cfg.get_autopilot_timestamp_ms() << "\n";
  auto tp_s = std::chrono::system_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto tp_f = std::chrono::system_clock::now();
  uint64_t dt =
      std::chrono::duration_cast<std::chrono::milliseconds>(tp_f - tp_s)
          .count();
  cfg.set_autopilot_lifespan_ms(dt);
  std::cout << "battery lifespan ms left: " << cfg.get_autopilot_lifespan_ms() << "\n";

  // power.xml
  cfg.set_power_off_reason(5);
  std::cout << "power off reason: " << cfg.get_power_off_reason() << "\n";
  cfg.set_total_run_cycle();
  std::cout << "total run cycle: " << cfg.get_total_run_cycle() << "\n";
  tp_s = std::chrono::system_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  tp_f = std::chrono::system_clock::now();
  dt =
      std::chrono::duration_cast<std::chrono::milliseconds>(tp_f - tp_s)
          .count();
  cfg.set_last_run_time_ms(dt);
  std::cout << "last run time: " << cfg.get_last_run_time_ms() << "\n";
  std::cout << "total run time: " << cfg.get_total_run_time_ms() << "\n";
  cfg.set_last_error(7);
  std::cout << "last error: " << cfg.get_last_error() << "\n";
  cfg.set_recovery_flag(123);
  std::cout << "recovery flag: " << cfg.get_recovery_flag() << "\n";

  return 0;
}
