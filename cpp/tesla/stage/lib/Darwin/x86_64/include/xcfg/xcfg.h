/*
 * Title      :   xcfg.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _XCFG_H_
#define _XCFG_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <thread>
#include <cmath>
#include <cerrno>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 
#include <boost/lexical_cast.hpp>

//#include "core/vehicle_component.h"

class xcfg_t {
 public:
  enum error_reason {
    error_none = 0,
    error_init_fault,
    error_no_file,
    error_no_dir,
    error_empty_contents,
    error_loop,
    error_access_denied,
    error_read_fault,
    error_write_fault,
    error_parsing_fault,
    error_undefiend
  };

  static const char* error_to_string(xcfg_t::error_reason const i) {
    static const char* const s[] = {"error_none",           
                                    "error_init_fault",
                                    "error_no_file",        
                                    "error_no_dir",
                                    "error_empty_contents", 
                                    "error_loop",
                                    "error_access_denied",  
                                    "error_read_fault",
                                    "error_write_fault",
                                    "error_parsing_fault",  
                                    "error_undefined"};
    return s[i];
  }

  xcfg_t() : error_(xcfg_t::error_none) {
    //bbbf_ = "/root/tesla/config/tesla.xml";
    //autopilotf_ = "/root/tesla/config/modelx.xml";
    //pwrf_ = "/root/tesla/config/power.xml";
    bbbf_ =  "config/tesla.xml";
    autopilotf_ = "config/modelx.xml";
    pwrf_ =  "config/power.xml";
    init_done_ = false;
    // tesla.xml
    model_name_ = "el301";
    model_number_ = 0;
    model_uuid_ = "";
    project_name_ = "";
    platform_name_ = "";
    battery_validation_ = true;
    battery_usage_threshold_ms_ = 60000;
    autopilot_lifespan_limit_ms_ = 36000000;
    battery_usage_limit_ = 10;
    system_clock_us_ = 100;
    algorithm_period_us_ = 5000;
    // modelx.xml
    autopilot_uuid_ = "";
    autopilot_type_ = 0; 
    autopilot_timestamp_ms_ = 0;
    autopilot_lifespan_ms_ = 0;
    // power.xml
    power_off_reason_ = 0;
    total_run_cycle_ = 0;
    total_run_time_ms_ = 0;
    last_run_time_ms_ = 0;
    last_error_ = 0;
    last_error_string_ = "";
    elmo_falut_count_ = 0;
    omega6_falut_count_ = 0;
    recovery_flag_ = 0;
  }

  ~xcfg_t() { ; }

  static bool exists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
  }

  bool init() {
    if (exists(bbbf_) == false) {
      std::cerr << "xcfg_t::init: file not found " << bbbf_ << "\n";
      error_ = error_no_file;
      return false;
    }
    if (exists(autopilotf_) == false) {
      std::cerr << "xcfg_t::init: file not found " << autopilotf_ << "\n";
      error_ = error_no_file;
      return false;
    }
    if (exists(pwrf_) == false) {
      std::cerr << "xcfg_t::init: file not found " << pwrf_ << "\n";
      error_ = error_no_file;
      return false;
    }

    try {
      boost::property_tree::read_xml(
          bbbf_, bbbp_, boost::property_tree::xml_parser::trim_whitespace);
      boost::property_tree::read_xml(
          autopilotf_, autopilotp_, boost::property_tree::xml_parser::trim_whitespace);
      boost::property_tree::read_xml(
          pwrf_, pwrp_, boost::property_tree::xml_parser::trim_whitespace);
    } catch (std::exception& e) {
      std::cerr << "xcfg_t init error: " << e.what() << "\n";
      error_ = error_init_fault;
      return false;
    }

    init_done_ = true;
    return true;
  }
  
  bool init_status() {
    return init_done_;
  }

  void deinit() {
    if ((error_ == xcfg_t::error_init_fault) ||
        (error_ == xcfg_t::error_no_file)) {
      ;
    } else {
      ;
    }
  }

  error_reason get_error() const { return error_; }
  std::string get_error_string() const {
    return error_to_string(error_);
  }

  // tesla.xml
  std::string get_model_name() {
    try {
      model_name_ = bbbp_.get<std::string>(
          "tesla.product_info.<xmlattr>.model_name");
    } catch (std::exception& e) {
      std::cout << "get_model_name: read error: " << e.what() << "\n";
      return "";
    }
    return model_name_;
  }

  bool set_model_name(std::string const value) {
    try {
      bbbp_.put<std::string>("tesla.product_info.<xmlattr>.model_name",
                              value);
      model_name_ = value;
    } catch (std::exception& e) {
      std::cout << "write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_tesla();
  }

  uint32_t get_model_number() {
    try {
      model_number_ =
          bbbp_.get<uint32_t>("tesla.product_info.<xmlattr>.model_number");
    } catch (std::exception& e) {
      std::cout << "get_model_number: read error: " << e.what() << "\n";
      model_number_ = 0;
    }
    return model_number_;
  }

  bool set_model_number(uint32_t const value) {
    try {
      bbbp_.put<uint32_t>("tesla.product_info.<xmlattr>.model_number",
                           value);
      model_number_ = value;
    } catch (std::exception& e) {
      std::cout << "set_model_number: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_tesla();
  }

  std::string get_model_uuid() {
    try {
      model_uuid_ = bbbp_.get<std::string>(
          "tesla.product_info.<xmlattr>.model_uuid");
      if (model_uuid_.empty()) {
        if (!this->set_model_uuid()) {
          std::cout << "get_model_uuid: set_model_uuid failure"
                    << "\n";
          return "";
        }
      }
    } catch (std::exception& e) {
      std::cout << "get_model_uuid: read or write error: " << e.what() << "\n";
      return "";
    }
    return model_uuid_;
  }

  std::string get_project_name() {
    try {
      project_name_ = bbbp_.get<std::string>(
          "tesla.software_info.<xmlattr>.project_name");
    } catch (std::exception& e) {
      std::cout << "get_project_name: read error: " << e.what() << "\n";
      return "";
    }
    return project_name_;
  }

  std::string get_platform_name() {
    try {
      platform_name_ = bbbp_.get<std::string>(
          "tesla.software_info.<xmlattr>.platform_name");
    } catch (std::exception& e) {
      std::cout << "get_platform_name: read error: " << e.what() << "\n";
      return "";
    }
    return platform_name_;
  }

  bool get_battery_validation() {
    try {
      battery_validation_ = bbbp_.get<bool>(
          "tesla.vehicle_config.<xmlattr>.battery_validation");
    } catch (std::exception& e) {
      std::cout << "get_battery_validation: read error: " << e.what() << "\n";
      return "";
    }
    return battery_validation_;
  }

  uint32_t get_battery_usage_threshold_ms() {
    try {
      battery_usage_threshold_ms_ = bbbp_.get<uint32_t>(
          "tesla.vehicle_config.<xmlattr>.battery_usage_threshold_ms");
    } catch (std::exception& e) {
      std::cout << "get_battery_usage_threshold_ms: read error: " << e.what() << "\n";
    }
    return battery_usage_threshold_ms_;
  }

  uint32_t get_battery_usage_limit() {
    try {
      battery_usage_limit_ = bbbp_.get<uint32_t>(
          "tesla.vehicle_config.<xmlattr>.battery_usage_limit");
    } catch (std::exception& e) {
      std::cout << "get_battery_usage_limit: read error: " << e.what() << "\n";
    }
    return battery_usage_limit_;
  }

  uint32_t get_autopilot_lifespan_threshold_ms() {
    try {
      autopilot_lifespan_threshold_ms_ = bbbp_.get<uint32_t>(
          "tesla.vehicle_config.<xmlattr>.autopilot_lifespan_threshold_ms");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_lifespan_threshold_ms: read error: " << e.what() << "\n";
    }
    return autopilot_lifespan_threshold_ms_;
  }

  uint32_t get_autopilot_lifespan_limit_ms() {
    try {
      autopilot_lifespan_limit_ms_ = bbbp_.get<uint32_t>(
          "tesla.vehicle_config.<xmlattr>.autopilot_lifespan_limit_ms");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_lifespan_limit_ms: read error: " << e.what() << "\n";
    }
    return autopilot_lifespan_limit_ms_;
  }

  uint32_t get_system_clock_us() {
    try {
      system_clock_us_ = bbbp_.get<uint32_t>(
          "tesla.realtime_config.<xmlattr>.system_clock_us");
    } catch (std::exception& e) {
      std::cout << "get_sysetm_clock_us: read error: " << e.what() << "\n";
      system_clock_us_ = 100;
    }
    return system_clock_us_;
  }

  uint32_t get_algorithm_period_us() {
    try {
      algorithm_period_us_ = bbbp_.get<uint32_t>(
          "tesla.realtime_config.<xmlattr>.algorithm_period_us");
    } catch (std::exception& e) {
      std::cout << "get_algorithm_period_us: read error: " << e.what() << "\n";
      algorithm_period_us_ = 5000;
    }
    return algorithm_period_us_;
  }

  // modelx.xml
  std::string get_autopilot_uuid() {
    try {
      autopilot_uuid_ =
          autopilotp_.get<std::string>("tesla.autopilot.<xmlattr>.autopilot_uuid");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_uuid: read error: " << e.what() << "\n";
      return "";
    }
    return autopilot_uuid_;
  }

  bool set_autopilot_uuid() {
    try {
      autopilot_uuid_ = this->gen_uuid_24byte();
      autopilotp_.put<std::string>("tesla.autopilot.<xmlattr>.autopilot_uuid",
                              autopilot_uuid_);
    } catch (std::exception& e) {
      std::cout << "set_autopilot_uuid: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_autopilot();
  }
  
  uint32_t get_autopilot_type() {
    try {
      autopilot_type_ =
          autopilotp_.get<uint32_t>("tesla.autopilot.<xmlattr>.autopilot_type");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_type: read error: " << e.what() << "\n";
      autopilot_type_ = 0;
    }
    return autopilot_type_;
  }

  bool set_autopilot_type(uint32_t const value) {
    try {
      autopilotp_.put<uint32_t>("tesla.autopilot.<xmlattr>.autopilot_type", value);
      autopilot_type_ = value;
    } catch (std::exception& e) {
      std::cout << "set_model_number: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_autopilot();
  }

  uint64_t get_current_timestamp_ms() {
    uint64_t ret = 0;
    auto now = std::chrono::system_clock::now();
    return this->tp_to_uint64(now);
  }

  uint64_t get_autopilot_timestamp_ms() {
    try {
      autopilot_timestamp_ms_ =
          autopilotp_.get<uint64_t>("tesla.autopilot.<xmlattr>.autopilot_timestamp_ms");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_timestamp_ms: read error: " << e.what() << "\n";
    }
    return autopilot_timestamp_ms_;
  }

  bool set_autopilot_timestamp_ms(uint64_t value) {
    try {
      autopilot_timestamp_ms_ = value;
      autopilotp_.put<uint64_t>("tesla.autopilot.<xmlattr>.autopilot_timestamp_ms",
                           autopilot_timestamp_ms_);
    } catch (std::exception& e) {
      std::cout << "set_autopilot_timestamp_ms: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_autopilot();
  }

  uint32_t get_autopilot_lifespan_ms() {
    try {
      autopilot_lifespan_ms_ =
          autopilotp_.get<uint32_t>("tesla.autopilot.<xmlattr>.autopilot_lifespan_ms");
    } catch (std::exception& e) {
      std::cout << "get_autopilot_lifespan_ms: read error: " << e.what() << "\n";
    }
    return autopilot_lifespan_ms_;
  }

  bool set_autopilot_lifespan_ms(uint32_t tm) {
    try {
      autopilot_lifespan_ms_ = this->get_autopilot_lifespan_ms() + tm;
      autopilotp_.put<uint32_t>("tesla.autopilot.<xmlattr>.autopilot_lifespan_ms",
                           autopilot_lifespan_ms_);
    } catch (std::exception& e) {
      std::cout << "set_autopilot_lifespan_ms: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_autopilot();
  }

  uint32_t get_battery_usage() {
    try {
      battery_usage_ =
          autopilotp_.get<uint32_t>("tesla.autopilot.<xmlattr>.battery_usage");
    } catch (std::exception& e) {
      std::cout << "get_battery_usage: read error: " << e.what() << "\n";
    }
    return battery_usage_;
  }

  bool set_battery_usage(uint32_t value) {
    try {
      autopilotp_.put<uint32_t>("tesla.autopilot.<xmlattr>.battery_usage", value);
      battery_usage_ = value;
    } catch (std::exception& e) {
      std::cout << "set_battery_usage: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_autopilot();
  }

  // power.xml
  uint32_t get_power_off_reason() {
    try {
      power_off_reason_ = pwrp_.get<uint32_t>(
          "tesla.power_status.<xmlattr>.power_off_reason");
    } catch (std::exception& e) {
      std::cout << "get_power_off_reason: read error: " << e.what() << "\n";
    }
    return power_off_reason_;
  }

  bool set_power_off_reason(uint32_t value) {
    try {
      pwrp_.put<uint32_t>("tesla.power_status.<xmlattr>.power_off_reason", value);
      power_off_reason_ = value;
    } catch (std::exception& e) {
      std::cout << "set_power_off_reason: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  std::string get_power_off_reason_string() {
    try {
      power_off_reason_string_ = pwrp_.get<std::string>(
          "tesla.power_status.<xmlattr>.power_off_reason_string");
    } catch (std::exception& e) {
      std::cout << "get_power_off_reason_string: read error: " << e.what() << "\n";
    }
    return power_off_reason_string_;
  }

  bool set_power_off_reason_string(std::string value) {
    try {
      pwrp_.put<std::string>(
          "tesla.power_status.<xmlattr>.power_off_reason_string", value);
      power_off_reason_string_ = value;
    } catch (std::exception& e) {
      std::cout << "set_power_off_reason_string: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  uint32_t get_total_run_cycle() {
    try {
      total_run_cycle_ = pwrp_.get<uint32_t>(
          "tesla.run_status.<xmlattr>.total_run_cycle");
    } catch (std::exception& e) {
      std::cout << "get_total_run_time_ms: read error: " << e.what() << "\n";
    }
    return total_run_cycle_;
  }

  bool set_total_run_cycle() {
    try {
      uint32_t value = this->get_total_run_cycle() + 1;
      pwrp_.put<uint32_t>("tesla.run_status.<xmlattr>.total_run_cycle",
                          value);
      total_run_cycle_ = value;
    } catch (std::exception& e) {
      std::cout << "set_total_run_cycle: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  uint32_t get_total_run_time_ms() {
    try {
      total_run_time_ms_ = pwrp_.get<uint32_t>(
          "tesla.run_status.<xmlattr>.total_run_time_ms");
    } catch (std::exception& e) {
      std::cout << "get_total_run_time_ms: read error: " << e.what() << "\n";
    }
    return total_run_time_ms_;
  }

  uint32_t get_last_run_time_ms() {
    try {
      last_run_time_ms_ = pwrp_.get<uint32_t>(
          "tesla.run_status.<xmlattr>.last_run_time_ms");
    } catch (std::exception& e) {
      std::cout << "get_last_run_time_ms: read error: " << e.what() << "\n";
    }
    return last_run_time_ms_;
  }

  bool set_last_run_time_ms(uint32_t value) {
    try {
      pwrp_.put<uint32_t>("tesla.run_status.<xmlattr>.last_run_time_ms",
                          value);
      last_run_time_ms_ = value;
    } catch (std::exception& e) {
      std::cout << "set_last_run_time_ms: write error: " << e.what() << "\n";
      return false;
    }
    return this->set_total_run_time_ms(last_run_time_ms_);
  }

  uint32_t get_last_error() {
    try {
      last_error_ = pwrp_.get<uint32_t>(
          "tesla.error_status.<xmlattr>.last_error");
    } catch (std::exception& e) {
      std::cout << "get_last_error: read error: " << e.what() << "\n";
    }
    return last_error_;
  }

  bool set_last_error(uint32_t value) {
    try {
      pwrp_.put<uint32_t>("tesla.error_status.<xmlattr>.last_error",
                          value);
      last_error_ = value;
    } catch (std::exception& e) {
      std::cout << "set_last_error: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  std::string get_last_error_string() {
    try {
      last_error_string_ = pwrp_.get<std::string>(
          "tesla.error_status.<xmlattr>.last_error_string");
    } catch (std::exception& e) {
      std::cout << "get_last_error_string: read error: " << e.what() << "\n";
    }
    return last_error_string_;
  }

  bool set_last_error_string(std::string value) {
    try {
      pwrp_.put<std::string>(
          "tesla.error_status.<xmlattr>.last_error_string", value);
      last_error_string_ = value;
    } catch (std::exception& e) {
      std::cout << "set_last_error_string: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  uint32_t get_recovery_flag() {
    try {
      recovery_flag_ = pwrp_.get<uint32_t>(
          "tesla.error_status.<xmlattr>.recovery_flag");
    } catch (std::exception& e) {
      std::cout << "get_recovery_flag: read error: " << e.what() << "\n";
    }
    return recovery_flag_;
  }

  bool set_recovery_flag(uint32_t value) {
    try {
      pwrp_.put<uint32_t>("tesla.error_status.<xmlattr>.recovery_flag",
                          value);
      recovery_flag_ = value;
    } catch (std::exception& e) {
      std::cout << "set_recovery_flag: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

 private:
  bool save_xml_tesla() {
    std::unique_lock<std::mutex> lk(mutex_);
    try {
      boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
      boost::property_tree::write_xml(bbbf_, bbbp_, std::locale(), settings);
    } catch (std::exception& e) {
      std::cout << "save error: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  bool save_xml_autopilot() {
    std::unique_lock<std::mutex> lk(mutex_);
    try {
      boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
      boost::property_tree::write_xml(autopilotf_, autopilotp_, std::locale(), settings);
    } catch (std::exception& e) {
      std::cout << "save error: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  bool save_xml_power() {
    std::unique_lock<std::mutex> lk(mutex_);
    try {
      boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
      boost::property_tree::write_xml(pwrf_, pwrp_, std::locale(), settings);
    } catch (std::exception& e) {
      std::cout << "save error: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  bool set_total_run_time_ms(uint32_t tm) {
    try {
      total_run_time_ms_ = this->get_total_run_time_ms() + tm;
      pwrp_.put<uint32_t>("tesla.run_status.<xmlattr>.total_run_time_ms",
                          total_run_time_ms_);
    } catch (std::exception& e) {
      std::cout << "set_total_run_time_ms: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_power();
  }

  uint64_t tp_to_uint64(std::chrono::system_clock::time_point tp) {
    uint64_t ret = 0;
    auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(tp);
    auto ep = ms.time_since_epoch();
    ret = ep.count();
    return ret;
  }

  std::string gen_uuid() const {
    std::string uuid = "";
    try {
      boost::uuids::uuid u = boost::uuids::random_generator()();
      uuid = boost::lexical_cast<std::string>(u);
      if (uuid.empty()) {
        std::cout << "gen_uuid: uuid generator failure" << "\n";
        return "";
      }
    } catch (std::exception& e) {
      std::cout << "gen_uuid: " << e.what() << "\n";
      return "";
    }
    return uuid;
  }
  
  bool set_model_uuid() {
    try {
      model_uuid_ = this->gen_uuid();
      bbbp_.put<std::string>("tesla.product_info.<xmlattr>.model_uuid",
                              model_uuid_);
    } catch (std::exception& e) {
      std::cout << "gen_model_uuid: write error: " << e.what() << "\n";
      return false;
    }
    return this->save_xml_tesla();
  }

  std::string gen_uuid_24byte() const {
    char temp[24 + 1] = {};
    try {
      boost::uuids::uuid u = boost::uuids::random_generator()();
      std::string uuid = boost::lexical_cast<std::string>(u);
      if (uuid.empty()) {
        std::cout << "gen_uuid: uuid generator failure" << "\n";
        return "";
      }
      const char* uuid_cstr = boost::uuids::to_string(u).c_str();
      for (int i = 0, j = 0; i < 36; i++) {
        if ((i != 8) && (i != 13) && (i != 18) && (i != 23) && (i < 28)) {
          temp[j] = uuid_cstr[i];
          j++;
        }
      }
    } catch (std::exception& e) {
      std::cout << "gen_uuid: " << e.what() << "\n";
      return "";
    }

    std::string uuid(temp);

    return uuid;
  }

 private:
  error_reason error_;
  bool init_done_;
  std::mutex mutex_;
  boost::property_tree::ptree bbbp_;
  std::string bbbf_;
  boost::property_tree::ptree autopilotp_;
  std::string autopilotf_;
  boost::property_tree::ptree pwrp_;
  std::string pwrf_;
  // tesla.xml
  std::string model_name_;
  uint32_t model_number_;
  std::string model_uuid_;
  std::string project_name_;
  std::string platform_name_;
  std::string watchdog_name_;
  bool battery_validation_;
  uint32_t battery_usage_threshold_ms_;
  uint32_t battery_usage_limit_;
  uint32_t autopilot_lifespan_threshold_ms_;
  uint32_t autopilot_lifespan_limit_ms_;
  uint32_t system_clock_us_;
  uint32_t algorithm_period_us_;
  // modelx.xml
  std::string autopilot_uuid_;
  uint32_t autopilot_type_; 
  uint64_t autopilot_timestamp_ms_;
  uint32_t autopilot_lifespan_ms_;
  uint32_t battery_usage_;
  // power.xml
  uint32_t power_off_reason_;
  std::string power_off_reason_string_;
  uint32_t total_run_cycle_;
  uint32_t total_run_time_ms_;
  uint32_t last_run_time_ms_;
  uint32_t last_error_;
  std::string last_error_string_;
  uint32_t elmo_falut_count_;
  uint32_t omega6_falut_count_;
  uint32_t recovery_flag_;
};

#endif  // !_XCFG_H_
