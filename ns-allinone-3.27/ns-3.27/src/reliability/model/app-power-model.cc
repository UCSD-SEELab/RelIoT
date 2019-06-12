/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ns3/log.h"
#include "ns3/traced-value.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/app-power-model.h"
#include <ns3/performance-model.h>
#include <iterator>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream> //istringstream


NS_LOG_COMPONENT_DEFINE ("AppPowerModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AppPowerModel);

TypeId
AppPowerModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AppPowerModel")
    .SetParent<PowerModel> ()
    .SetGroupName ("Power")
    .AddConstructor<AppPowerModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (6.0957e-11),    
                   MakeDoubleAccessor (&AppPowerModel::SetA,
                                       &AppPowerModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.7775e-05),    
                   MakeDoubleAccessor (&AppPowerModel::SetB,
                                       &AppPowerModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Parameter C.",
                   DoubleValue (4.7775e-05),   
                   MakeDoubleAccessor (&AppPowerModel::SetC,
                                       &AppPowerModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("DataSize",
                   "Input data size for the application.",
                   DoubleValue (100),
                   MakeDoubleAccessor (&AppPowerModel::SetDataSize,
                                       &AppPowerModel::GetDataSize),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("AppName",
                   "Application name",
                   StringValue ("AdaBoost"),    // default
                   MakeStringAccessor (&AppPowerModel::SetAppName,
                                       &AppPowerModel::GetAppName),
                   MakeStringChecker())
    .AddAttribute ("IdlePowerW",
                   "Idle Power Consumption of Cpu",
                   DoubleValue (2.8),    // default
                   MakeDoubleAccessor (&AppPowerModel::SetIdlePowerW,
                                       &AppPowerModel::GetIdlePowerW),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("CpuPower",
                     "CPU power consumption of the device.",
                     MakeTraceSourceAccessor (&AppPowerModel::m_cpupower),
                     "ns3::TracedValueCallback::Double")
  ; 
  return tid;
}

AppPowerModel::AppPowerModel ()
{
  NS_LOG_FUNCTION (this);
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (0.045);
  m_temperatureModel = NULL;      // TemperatureModel
  m_performanceModel = NULL;      // PerformanceModel
  m_currentState = 0;
  m_cpupower = 2.8;
}

AppPowerModel::~AppPowerModel ()
{
  NS_LOG_FUNCTION (this);
}


void
AppPowerModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
}

void
AppPowerModel::RegisterPerformanceModel (Ptr<PerformanceModel> performanceModel)
{
  m_performanceModel = performanceModel;
}


double
AppPowerModel::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  return m_cpupower;
}


void
AppPowerModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
AppPowerModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
AppPowerModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
AppPowerModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
AppPowerModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);
  m_C = C;
}

double
AppPowerModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}


void
AppPowerModel::SetIdlePowerW (double idlePowerW)
{
  NS_LOG_FUNCTION (this << idlePowerW);
  m_idlePowerW = idlePowerW;
}

double
AppPowerModel::GetIdlePowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_idlePowerW;
}

void
AppPowerModel::SetDataSize(double dataSize)
{
  NS_LOG_FUNCTION (this << dataSize);
  m_dataSize = dataSize;
}

double
AppPowerModel::GetDataSize(void) const
{
  NS_LOG_FUNCTION (this);
  return m_dataSize;
}

void
AppPowerModel::SetAppName(const std::string &appname)
{
  NS_LOG_FUNCTION (this << appname);
  m_appName = appname;

  if(m_appName == "LinearRegression")
  {
    m_A = 0.0;
    m_B = 1.83*pow(10,-1);
    m_C = (9.72)*pow(10,1);
  }
  else if(m_appName == "AdaBoost")
  {
    m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  else if(m_appName == "MedianFilter")
  {
    m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  else if(m_appName == "NeuralNetwork")
  {
    m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  else
  {
    NS_FATAL_ERROR ("AppPowerModel:Undefined application: " << m_appName);
  }

}

std::string
AppPowerModel::GetAppName(void) const
{
  NS_LOG_FUNCTION (this);
  return m_appName;
}

void
AppPowerModel::SetApplication(std::string appname, const DoubleValue &v0)
{
  m_appName = appname;
  m_dataSize = v0.Get();
  if(m_deviceType == "RaspberryPi")
  {
    if(m_appName == "LinearRegression")
    {
      m_A = 0.0;
      m_B = 1.83*pow(10,-1);
      m_C = (9.72)*pow(10,1);
    }
    else if(m_appName == "AdaBoost")
    {
      m_A = 0.0;
      m_B = 8.13*pow(10,-4);
      m_C = (1.94)*pow(10,1);
    }
    else if(m_appName == "NeuralNetwork")
    {
      m_A = 0.0;
      m_B = 8.13*pow(10,-4);
      m_C = (1.94)*pow(10,1);
    }
    else
    {
      NS_FATAL_ERROR ("AppPowerModel:Undefined application for this device: " << m_appName);
    }
  }
  else if(m_deviceType == "Arduino")
  {
    if(m_appName == "MedianFilter")
    {
      m_A = 0.0;
      m_B = 8.13*pow(10,-4);
      m_C = (1.94)*pow(10,1);
    }
    else
    {
      NS_FATAL_ERROR ("AppPowerModel:Undefined application for this device: " << m_appName);
    }
  }
  else
  {
    NS_FATAL_ERROR ("AppPowerModel:Undefined device type: " << m_deviceType);
  }

  m_performanceModel->SetApplication(m_appName,m_dataSize);
}

void
AppPowerModel::SetDeviceType(std::string devicetype)
{
  m_deviceType = devicetype;

  m_performanceModel->SetDeviceType(m_deviceType);
}

void
AppPowerModel::SetState (int state)
{
  NS_LOG_FUNCTION (this << state);
  m_currentState = state;
}

int
AppPowerModel::GetState (void) const
{
  NS_LOG_FUNCTION (this);
  return m_currentState;
}

double
AppPowerModel::GetEnergy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_energy;
}

void
AppPowerModel::RunApp()
{
  Time now = Simulator::Now ();
  m_performanceModel->SetApplication(m_appName,m_dataSize);
  m_currentState = 1;
  m_powerUpdateEvent = Simulator::Schedule (Seconds(0.0),&AppPowerModel::UpdatePower,this);
  m_exectime = m_performanceModel->GetExecTime();
  Simulator::Schedule (Seconds(m_exectime),&AppPowerModel::TerminateApp,this);
  HandleAppRunEvent();
  NS_LOG_DEBUG ("AppPowerModel:Application scheduled successfully!" << " at time = " << Simulator::Now ());
  NS_LOG_DEBUG ("AppPowerModel:Application will be terminated in " << m_exectime << " seconds ");
}

void
AppPowerModel::TerminateApp()
{
 //m_powerUpdateEvent.Cancel ();
 m_cpupower = m_idlePowerW;
 m_currentState = 0;
 HandleAppTerminateEvent();
   NS_LOG_DEBUG ("AppPowerModel:Application terminated successfully!" << " at time = " << Simulator::Now ());
}


void
AppPowerModel::UpdatePower ()
{
  NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " m_C:" << m_C);
  //NS_LOG_DEBUG ("AppPowerModel:Updating power" << " at time = " << Simulator::Now ());
  if (Simulator::IsFinished ())
    {
      return;
    }
  //m_powerUpdateEvent.Cancel ();
  if(m_currentState == 1){
    m_energy = m_B*m_dataSize/1000 + m_C;
    m_cpupower = m_energy/m_exectime;
  }
  else{
     m_cpupower = m_idlePowerW;
  }
  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  m_temperatureModel->UpdateTemperature (m_cpupower);
  m_powerUpdateEvent = Simulator::Schedule (m_powerUpdateInterval,&AppPowerModel::UpdatePower,this);
}

void
AppPowerModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_temperatureModel = NULL;      // TemperatureModel
  m_performanceModel = NULL;      // PerformanceModel

}

void
AppPowerModel::HandleAppRunEvent (void)
{
  NS_LOG_DEBUG ("AppPowerModel: HandleAppRunEvent");
  NS_LOG_FUNCTION (this);
  NotifyAppRun (); // notify DeviceEnergyModel objects
}

void
AppPowerModel::HandleAppTerminateEvent (void)
{
  NS_LOG_DEBUG ("AppPowerModel: HandleAppTerminateEvent");
  NS_LOG_FUNCTION (this);
  NotifyAppTerminate (); // notify DeviceEnergyModel objects
}


} // namespace ns3
