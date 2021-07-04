# Title   :   xfsm CMakeLists.txt
# Author  :   Jaewoong Jang
# Date    :   Jun 20th 2021

cmake_minimum_required (VERSION 3.6.3)

project (xfsm)

set (ENABLED_DEBUG_XFSM_DISPATCHER false)
set (ENABLED_DEBUG_XFSM_PRECISION_TIMER false)
set (ENABLED_DEBUG_XFSM_QUEUE false)
set (ENABLED_DEBUG_XFSM_RECEIVER false)
set (ENABLED_DEBUG_XFSM_SENDER false)
set (ENABLED_DEBUG_XFSM_SINGLETONE false)
set (ENABLED_DEBUG_XFSM_STATE_MACHINE false)
set (ENABLED_DEBUG_XFSM_TEMPLATE_DISPATCHER false)
set (ENABLED_DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER false)
set (ENABLED_DEBUG_XFSM_TIMED_DISPATCHER false)
set (ENABLED_DEBUG_XFSM_TIMER_FSM false)


configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/xfsm.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/xfsm.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/logger_adaptor.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/logger_adaptor.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/base_message.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/base_message.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/singleton.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/singleton.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/timer_fsm.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/timer_fsm.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/base_timer.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/base_timer.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/precision_timer.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/precision_timer.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/template_dispatcher.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/template_dispatcher.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/template_timed_dispatcher.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/template_timed_dispatcher.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/dispatcher.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/dispatcher.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/timed_dispatcher.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/timed_dispatcher.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/queue.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/queue.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/receiver.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/receiver.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/sender.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/sender.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/base_state_machine.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/base_state_machine.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/state_machine.hpp.in"
  "${TESLA_HEADER_PATH}/${PROJECT_NAME}/state_machine.h"
  )

#install (
#  TARGETS
#  xfsm
#  DESTINATION
#  "${TESLA_LIB_PATH}"

add_subdirectory (test)
