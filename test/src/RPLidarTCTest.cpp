// -*- C++ -*-
/*!
 * @file  RPLidarTCTest.cpp
 * @brief RPLiDAR RTC
 * @date $Date$
 *
 * $Id$
 */

#include "RPLidarTCTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* rplidartc_spec[] =
  {
    "implementation_id", "RPLidarTCTest",
    "type_name",         "RPLidarTCTest",
    "description",       "RPLiDAR RTC",
    "version",           "1.0.0",
    "vendor",            "aist",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.port_name", ""COM1"",
    "conf.default.baudrate", "115200",
    "conf.default.debug", "0",
    "conf.default.encoding", "2",
    "conf.default.geometry_x", "0",
    "conf.default.geometry_y", "0",
    "conf.default.geometry_z", "0",

    // Widget
    "conf.__widget__.port_name", "text",
    "conf.__widget__.baudrate", "text",
    "conf.__widget__.debug", "text",
    "conf.__widget__.encoding", "spin",
    "conf.__widget__.geometry_x", "text",
    "conf.__widget__.geometry_y", "text",
    "conf.__widget__.geometry_z", "text",
    // Constraints
    "conf.__constraints__.encoding", "{twochar:2,threechar:3}",

    "conf.__type__.port_name", "string",
    "conf.__type__.baudrate", "int",
    "conf.__type__.debug", "int",
    "conf.__type__.encoding", "int",
    "conf.__type__.geometry_x", "double",
    "conf.__type__.geometry_y", "double",
    "conf.__type__.geometry_z", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RPLidarTCTest::RPLidarTCTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rangeOut("range", m_range)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RPLidarTCTest::~RPLidarTCTest()
{
}



RTC::ReturnCode_t RPLidarTCTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("range", m_rangeIn);
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("port_name", m_port_name, ""COM1"");
  bindParameter("baudrate", m_baudrate, "115200");
  bindParameter("debug", m_debug, "0");
  bindParameter("encoding", m_encoding, "2");
  bindParameter("geometry_x", m_geometry_x, "0");
  bindParameter("geometry_y", m_geometry_y, "0");
  bindParameter("geometry_z", m_geometry_z, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarTCTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarTCTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarTCTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RPLidarTCTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RPLidarTCTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RPLidarTCTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarTCTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarTCTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RPLidarTCTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarTCTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarTCTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RPLidarTCTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(rplidartc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RPLidarTCTest>,
                             RTC::Delete<RPLidarTCTest>);
  }
  
};


