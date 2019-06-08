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
#include "ns3/util-power-model.h"
#include <ns3/performance-model.h>
#include <iterator>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream> //istringstream


NS_LOG_COMPONENT_DEFINE ("UtilPowerModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UtilPowerModel);

TypeId
UtilPowerModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UtilPowerModel")
    .SetParent<PowerModel> ()
    .SetGroupName ("Power")
    .AddConstructor<UtilPowerModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.58183313424e-06),  
                   MakeDoubleAccessor (&UtilPowerModel::SetA,
                                       &UtilPowerModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (0.0188874750319),  
                   MakeDoubleAccessor (&UtilPowerModel::SetB,
                                       &UtilPowerModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Parameter C.",
                   DoubleValue (1.79172123846),   
                   MakeDoubleAccessor (&UtilPowerModel::SetC,
                                       &UtilPowerModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Frequency",
                   "Frequency(kHz).",
                   DoubleValue (2.4*1000000),
                   MakeDoubleAccessor (&UtilPowerModel::SetFrequency,
                                       &UtilPowerModel::GetFrequency),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Utilization",
                   "CPU Utilization",
                   DoubleValue (0.5),    // default
                   MakeDoubleAccessor (&UtilPowerModel::SetUtilization,
                                       &UtilPowerModel::GetUtilization),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("IdlePowerW",
                   "Idle Power Consumption of Cpu",
                   DoubleValue (2.8),    // default
                   MakeDoubleAccessor (&UtilPowerModel::SetIdlePowerW,
                                       &UtilPowerModel::GetIdlePowerW),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("CpuPower",
                     "CPU power consumption of the device.",
                     MakeTraceSourceAccessor (&UtilPowerModel::m_cpupower),
                     "ns3::TracedValueCallback::Double")
  ; 
  return tid;
}

UtilPowerModel::UtilPowerModel ()
{
  NS_LOG_FUNCTION (this);
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (0.045);
  m_temperatureModel = NULL;      // TemperatureModel
  m_performanceModel = NULL;      // PerformanceModel
  m_currentState = 0;
}

UtilPowerModel::~UtilPowerModel ()
{
  NS_LOG_FUNCTION (this);
}


void
UtilPowerModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
}

void
UtilPowerModel::RegisterPerformanceModel (Ptr<PerformanceModel> performanceModel)
{
  m_performanceModel = performanceModel;
}

double
UtilPowerModel::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  return m_cpupower;
}


void
UtilPowerModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
UtilPowerModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
UtilPowerModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
UtilPowerModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
UtilPowerModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);
  m_C = C;
}

double
UtilPowerModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}


void
UtilPowerModel::SetIdlePowerW (double idlePowerW)
{
  NS_LOG_FUNCTION (this << idlePowerW);
  m_idlePowerW = idlePowerW;
}

double
UtilPowerModel::GetIdlePowerW (void) const
{
  NS_LOG_FUNCTION (this);
  return m_idlePowerW;
}

void
UtilPowerModel::SetFrequency (double frequency)
{
  NS_LOG_FUNCTION (this);
  m_freq = frequency;
}

double
UtilPowerModel::GetFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_freq;
}

void
UtilPowerModel::SetUtilization (double cpu_util)
{
  NS_LOG_FUNCTION (this);
  m_util = cpu_util;
}

double
UtilPowerModel::GetUtilization (void) const
{
  NS_LOG_FUNCTION (this);
  return m_util;
}

void
UtilPowerModel::SetApplication(std::string appname, const DoubleValue &v0)
{

  if(appname == "LinearRegression")
  {m_A = 0.0;
    m_B = 1.83*pow(10,-1);
    m_C = (9.72)*pow(10,1);
  }
  if(appname == "AdaBoost")
  {m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  if(appname == "MedianFilter")
  {m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  if(appname == "NeuralNetwork")
  {m_A = 0.0;
    m_B = 8.13*pow(10,-4);
    m_C = (1.94)*pow(10,1);
  }
  m_performanceModel->SetApplication(appname,v0.Get());
}

void
UtilPowerModel::SetState (int state)
{
  NS_LOG_FUNCTION (this << state);
  m_currentState = state;
}

int
UtilPowerModel::GetState (void) const
{
  NS_LOG_FUNCTION (this);
  return m_currentState;
}

double
UtilPowerModel::GetEnergy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_energy;
}

void
UtilPowerModel::RunApp()
{
  Time now = Simulator::Now ();
  m_powerUpdateEvent = Simulator::Schedule (now,&UtilPowerModel::UpdatePower,this);
  m_currentState = 1;
  m_exectime = m_performanceModel->GetExecTime();
  Simulator::Schedule (Seconds(m_exectime),&UtilPowerModel::TerminateApp,this);
  NS_LOG_DEBUG ("UtilPowerModel:Application scheduled successfully!" << " at time = " << Simulator::Now ());
  NS_LOG_DEBUG ("UtilPowerModel:Application will be terminated in " << m_exectime << " seconds ");
}

void
UtilPowerModel::TerminateApp()
{
 m_powerUpdateEvent.Cancel ();
 m_cpupower = m_idlePowerW;
 m_currentState = 0;
   NS_LOG_DEBUG ("UtilPowerModel:Application terminated successfully!" << " at time = " << Simulator::Now ());
}


void
UtilPowerModel::UpdatePower ()
{
  NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " m_C:" << m_C << "m_frequency:" << m_freq << "m_util:" << m_util);
  NS_LOG_DEBUG ("UtilPowerModel:Updating power" << " at time = " << Simulator::Now ());
  if (Simulator::IsFinished ())
    {
      return;
    }
  m_powerUpdateEvent.Cancel ();

  m_cpupower = m_A*m_freq + m_B*m_util + m_C;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  m_temperatureModel->UpdateTemperature (m_cpupower);
  m_powerUpdateEvent = Simulator::Schedule (m_powerUpdateInterval,&UtilPowerModel::UpdatePower,this);
}

void
UtilPowerModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_temperatureModel = NULL;      // TemperatureModel
  m_performanceModel = NULL;      // PerformanceModel

}



} // namespace ns3
