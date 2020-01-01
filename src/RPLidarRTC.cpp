// -*- C++ -*-
/*!
 * @file  RPLidarRTC.cpp
 * @brief RPLiDAR RTC
 * @date $Date$
 *
 * $Id$
 */

#include "RPLidarRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* rplidarrtc_spec[] =
  {
    "implementation_id", "RPLidarRTC",
    "type_name",         "RPLidarRTC",
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
#if WIN32
    "conf.default.port_name", ""COM1"",
#else
    "conf.default.port_name", "/dev/ttyUSB0",
#endif
    "conf.default.baudrate", "115200",
    "conf.default.debug", "0",
    "conf.default.encoding", "2",
    "conf.default.geometry_x", "0",
    "conf.default.geometry_y", "0",
    "conf.default.geometry_z", "0",
    "conf.default.orientation_r", "0.0",
    "conf.default.orientation_p", "0.0",
    "conf.default.orientation_y", "0.0",
    "conf.default.scale", "1.0",

    // Widget
    "conf.__widget__.port_name", "text",
    "conf.__widget__.baudrate", "text",
    "conf.__widget__.debug", "text",
    "conf.__widget__.encoding", "spin",
    "conf.__widget__.geometry_x", "text",
    "conf.__widget__.geometry_y", "text",
    "conf.__widget__.geometry_z", "text",
    "conf.__widget__.orientation_r", "text",
    "conf.__widget__.orientation_p", "text",
    "conf.__widget__.orientation_y", "text",
    "conf.__widget__.scale", "text",
    // Constraints
    "conf.__constraints__.encoding", "{twochar:2,threechar:3}",

    "conf.__type__.port_name", "string",
    "conf.__type__.baudrate", "int",
    "conf.__type__.debug", "int",
    "conf.__type__.encoding", "int",
    "conf.__type__.geometry_x", "double",
    "conf.__type__.geometry_y", "double",
    "conf.__type__.geometry_z", "double",
    "conf.__type__.orientation_r", "double",
    "conf.__type__.orientation_p", "double",
    "conf.__type__.orientation_y", "double",
    "conf.__type__.scale", "float",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RPLidarRTC::RPLidarRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rangeOut("range", m_range)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RPLidarRTC::~RPLidarRTC()
{
}



RTC::ReturnCode_t RPLidarRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("range", m_rangeOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("port_name", m_port_name, "COM1");
  bindParameter("baudrate", m_baudrate, "115200");
  bindParameter("debug", m_debug, "0");
  bindParameter("encoding", m_encoding, "2");
  bindParameter("geometry_x", m_geometry_x, "0");
  bindParameter("geometry_y", m_geometry_y, "0");
  bindParameter("geometry_z", m_geometry_z, "0");
  bindParameter("orientation_r", m_orientation_r, "0.0");
  bindParameter("orientation_p", m_orientation_p, "0.0");
  bindParameter("orientation_y", m_orientation_y, "0.0");
  bindParameter("scale", m_scale, "1.0");
  // </rtc-template>

  drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
  if (!drv) {
      std::cerr << "insufficent memory, exit." << std::endl;
      return RTC::RTC_ERROR;
  } 
  return RTC::RTC_OK;
}

RTC::ReturnCode_t RPLidarRTC::onFinalize()
{
  RPlidarDriver::DisposeDriver(drv);\
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RPLidarRTC::onActivated(RTC::UniqueId ec_id)
{
  rplidar_response_device_health_t healthinfo;
  rplidar_response_device_info_t devinfo;
  u_result     op_result;

  std::cout << "[RPLidarRTC] Staring in (" << m_port_name <<", baudrate=" << m_baudrate << ")" << std::endl; 

  if (IS_FAIL(drv->connect(m_port_name.c_str(), m_baudrate))) {
    std::cerr << "Error: cannot bind to the specified serial port " <<  m_port_name << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << "[RPLidarRTC] Waiting..." << std::endl; 
  op_result = drv->getDeviceInfo(devinfo);
  if (IS_FAIL(op_result)) {
    if (op_result == RESULT_OPERATION_TIMEOUT) {
      std::cerr << "[RPLidarRTC] Error: operation time out." << std::endl;
    } else {
       std::cerr << "[RPLidarRTC] Error: unexpected error, code: " << std::hex << op_result << std::endl;;
    }
    return RTC::RTC_ERROR;
  }
  if(devinfo.hardware_version > 4){
    m_range.config.minRange = 0.15;
    m_range.config.maxRange = 12;
  }else{
    m_range.config.minRange = 0.15;
    m_range.config.maxRange = 6;
  }
  op_result = drv->getHealth(healthinfo);
  if (!IS_OK(op_result)) {
    std::cerr << "[RPLidarRTC] Error: cannot retrieve the lidar health code: " <<  std::hex << op_result << std::endl;
    return RTC::RTC_ERROR;
  }
  if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
    std::cerr << "[RPLidarRTC] Error: rplidar internal error detected. Please reboot the device to retry." << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << "[RPLidarRTC] Starting RPLiDAR..." << std::endl;
  drv->startMotor();
  if (IS_FAIL(drv->startScan( 0,1 ))) {
    std::cerr << "[RPLidarRTC] Error: cannot start the scan operation." << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << "[RPLidarRTC] offset_x:" << m_geometry_x << std::endl;
  std::cout << "[RPLidarRTC] offset_y:" << m_geometry_y << std::endl;
  std::cout << "[RPLidarRTC] offset_z:" << m_geometry_z << std::endl;

  m_range.geometry.geometry.pose.position.x = m_geometry_x;
  m_range.geometry.geometry.pose.position.y = m_geometry_y;
  m_range.geometry.geometry.pose.position.z = m_geometry_z;

  m_range.geometry.geometry.pose.orientation.r = m_orientation_r;
  m_range.geometry.geometry.pose.orientation.p = m_orientation_p;
  m_range.geometry.geometry.pose.orientation.y = m_orientation_y;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RPLidarRTC::onDeactivated(RTC::UniqueId ec_id)
{
  std::cout << "[RPLidarRTC] Deactivating RTC..."  << std::endl;
  drv->stop();
  drv->stopMotor();
  drv->disconnect();
  std::cout << "[RPLidarRTC] Stoped."  << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RPLidarRTC::onExecute(RTC::UniqueId ec_id)
{
  u_result ans;
  rplidar_response_measurement_node_hq_t nodes[8192];
  size_t   count = _countof(nodes);
  
  //std::cerr << "waiting for data..." << std::endl;
  ans = drv->grabScanDataHq(nodes, count);
  if (IS_OK(ans) || ans == RESULT_OPERATION_TIMEOUT) {
    drv->ascendScanData(nodes, count);
    double minAngle, maxAngle, angularRes;
    minAngle = getAngle(nodes[0]);
    maxAngle = getAngle(nodes[count-1]);
    angularRes = (maxAngle - minAngle)/(count-1);
    if(m_debug){
      std::cout << "[RPLidarRTC] Length: " << count << std::endl;
      std::cout << "[RPLidarRTC] MinAngle: " << minAngle << std::endl;
      std::cout << "[RPLidarRTC] MaxAngle: " << maxAngle << std::endl;
    }

    m_range.config.minAngle = minAngle - 3.14159265358979;
    m_range.config.maxAngle = maxAngle - 3.14159265358979;
    m_range.config.angularRes = angularRes;
    m_range.ranges.length(count);

    for(int i=0; i< (int)count; i++){
      int idx = (count - i -1  + count/2) % count;
      //int idx = (count - i -1 ) % count;
      //printf("%d ", idx);
      m_range.ranges[i] = getDistance(nodes[idx]) * m_scale;
    }
    ///printf("\n");
    m_rangeOut.write();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RPLidarRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RPLidarRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RPLidarRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RPLidarRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(rplidarrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RPLidarRTC>,
                             RTC::Delete<RPLidarRTC>);
  }
  
};


