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
 * Author: Vishwesh Rege <vrege2012@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/traced-value.h"
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/power-linearmodel.h"
#include <ns3/performance-model.h>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream> //istringstream
//#include "ns3/energy-source.h"

NS_LOG_COMPONENT_DEFINE ("PowerLinearModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PowerLinearModel);

TypeId
PowerLinearModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PowerLinearModel")
    .SetParent<Object> ()
    .AddConstructor<PowerLinearModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (6.0957e-11),    //1.21914316767465*0.0000001
                   MakeDoubleAccessor (&PowerLinearModel::SetA,
                                       &PowerLinearModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.7775e-05),    //0.0955492791339378
                   MakeDoubleAccessor (&PowerLinearModel::SetB,
                                       &PowerLinearModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Parameter C.",
                   DoubleValue (4.7775e-05),    //0.0955492791339378
                   MakeDoubleAccessor (&PowerLinearModel::SetC,
                                       &PowerLinearModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("f",
                   "Frequency.",
                   DoubleValue (1000),
                   MakeDoubleAccessor (&PowerLinearModel::SetFrequency,
                                       &PowerLinearModel::GetFrequency),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("CPU Power",
                     "CPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerLinearModel::m_cpupower),
                     "ns3::TracedValue::DoubleCallback") 
    .AddTraceSource ("GPU Power",
                     "GPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerLinearModel::m_gpupower),
                     "ns3::TracedValue::DoubleCallback") 
  ; 
  return tid;
}

PowerLinearModel::PowerLinearModel ()
{
  NS_LOG_FUNCTION (this);
  //m_power = {0.0,0.0};
  m_cpupower = 0.0;
  m_energy = 0.0;
  m_exectime = 0.0;
  m_gpupower = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (0.2);
  m_A = 6.0957e-11;
  m_B = 4.7775e-05;
  m_C = 5.2e-5;
  m_frequency = 1000;
  m_datasize = 0;
  m_appname = "LinearRegression";
  //m_energyDepletionCallback.Nullify ();
  m_temperatureModel = NULL;      // TemperatureModel
  m_performanceModel = NULL;
  //m_reliabilityModel = NULL;      // ReliabilityModel
  vec_cpupwr={1,2,3,4};
  vec_gpupwr={1,2,3,4};

}

PowerLinearModel::~PowerLinearModel ()
{
  NS_LOG_FUNCTION (this);
}

/*void
PowerModel::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
{
  m_reliabilityModel = reliabilityModel;
  //phy->RegisterListener (this);
}*/

void
PowerLinearModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
  //phy->RegisterListener (this);
}

void
PowerLinearModel::RegisterPerformanceModel (Ptr<PerformanceModel> performanceModel)
{
  m_performanceModel = performanceModel;
  //phy->RegisterListener (this);
}
/*
void
PowerModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}
*/
std::vector<double>
PowerLinearModel::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  std::vector<double> v = {m_cpupower, m_gpupower};
  return v;
}

void
PowerLinearModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
PowerLinearModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PowerLinearModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PowerLinearModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PowerLinearModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);
  m_C = C;
}

double
PowerLinearModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}

void
PowerLinearModel::SetFrequency (double freqeuncy)
{
  NS_LOG_FUNCTION (this);
  m_frequency = freqeuncy;
}

double
PowerLinearModel::GetFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_frequency;
}

void
PowerLinearModel::SetPowerTrace ( const std::vector<float> cpu_power, const std::vector<float> gpu_power)
{
  NS_LOG_FUNCTION (this);
  vec_cpupwr = cpu_power;
  vec_gpupwr = gpu_power;
}

void
PowerLinearModel::SetApplication(std::string n0, const DoubleValue &v0)
{
  m_appname = n0;
  m_datasize = v0.Get();
  m_performanceModel->SetApplication(m_appname,m_datasize);
}
void
PowerLinearModel::UpdatePower ()
{
  NS_LOG_FUNCTION (this << m_cpupower << m_frequency);
  //NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " m_frequency:" << m_frequency);
  m_exectime = m_performanceModel->GetExecTime();
  if (Simulator::IsFinished ())
    {
      return;
    }

  if(m_appname == "LinearRegression")
  {m_A = 0.0;
   m_B = 1.83*pow(10,-1);
   m_C = (9.72)*pow(10,1);
   m_energy = m_B*m_datasize + m_C;
  }
  if(m_appname == "AdaBoost")
  {m_A = 0.0;
   m_B = 8.13*pow(10,-4);
   m_C = (1.94)*pow(10,1);
   m_energy = m_B*m_datasize + m_C;
}
  m_cpupower = m_energy/m_exectime;
  Time updatetime = Simulator::Now ();

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  m_temperatureModel->UpdateTemperature (m_cpupower, m_gpupower, updatetime.GetSeconds());

  m_powerUpdateEvent = Simulator::Schedule (m_powerUpdateInterval,&PowerLinearModel::UpdatePower,this);

}

void
PowerLinearModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
  //m_energyDepletionCallback.Nullify ();
}



} // namespace ns3
