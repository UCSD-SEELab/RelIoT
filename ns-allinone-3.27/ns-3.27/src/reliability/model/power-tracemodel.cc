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
#include "ns3/power-tracemodel.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream> //istringstream
//#include "ns3/energy-source.h"

NS_LOG_COMPONENT_DEFINE ("PowerTraceModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PowerTraceModel);

TypeId
PowerTraceModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PowerTraceModel")
    .SetParent<Object> ()
    .AddConstructor<PowerTraceModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (6.0957e-11),    //1.21914316767465*0.0000001
                   MakeDoubleAccessor (&PowerTraceModel::SetA,
                                       &PowerTraceModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.7775e-05),    //0.0955492791339378
                   MakeDoubleAccessor (&PowerTraceModel::SetB,
                                       &PowerTraceModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("f",
                   "Frequency.",
                   DoubleValue (1000),
                   MakeDoubleAccessor (&PowerTraceModel::SetFrequency,
                                       &PowerTraceModel::GetFrequency),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("CPU Power",
                     "CPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerTraceModel::m_cpupower),
                     "ns3::TracedValue::DoubleCallback") 
    .AddTraceSource ("GPU Power",
                     "GPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerTraceModel::m_gpupower),
                     "ns3::TracedValue::DoubleCallback") 
  ; 
  return tid;
}

PowerTraceModel::PowerTraceModel ()
{
  NS_LOG_FUNCTION (this);
  //m_power = {0.0,0.0};
  m_cpupower = 0.0;
  m_gpupower = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (1.0);
  m_A = 6.0957e-11;
  m_B = 4.7775e-05;
  m_frequency = 1000;
  //m_energyDepletionCallback.Nullify ();
  m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
  vec_cpupwr={1,2,3,4};
  vec_gpupwr={1,2,3,4};

}

PowerTraceModel::~PowerTraceModel ()
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
PowerTraceModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
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
PowerTraceModel::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  std::vector<double> v = {m_cpupower, m_gpupower};
  return v;
}

void
PowerTraceModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
PowerTraceModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PowerTraceModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PowerTraceModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PowerTraceModel::SetFrequency (double freqeuncy)
{
  NS_LOG_FUNCTION (this);
  m_frequency = freqeuncy;
}

double
PowerTraceModel::GetFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_frequency;
}

void
PowerTraceModel::SetPowerTrace ( const std::vector<float> cpu_power, const std::vector<float> gpu_power)
{
  NS_LOG_FUNCTION (this);
  vec_cpupwr = cpu_power;
  vec_gpupwr = gpu_power;
}

void
PowerTraceModel::SetSamplingPeriod(const DoubleValue &v0)
{
//Ptr<DoubleChecker> v = MakeDoubleChecker<double>(0.0,10.0);
m_powerUpdateInterval = Seconds(v0.Get()); 
}

void
PowerTraceModel::UpdatePower ()
{
  NS_LOG_FUNCTION (this << m_cpupower << m_frequency);
  //NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " m_frequency:" << m_frequency);

  if (Simulator::IsFinished ())
    {
      return;
    }

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

//  double supplyVoltage = m_source->GetSupplyVoltage ();

  m_powerUpdateEvent.Cancel ();
  //int FAC = 2;
  //m_power = 0.0000000001*0.0000000001*(m_A*m_frequency*m_frequency*m_frequency + m_B*m_frequency)/(1000*FAC);
  //m_power = m_A*m_frequency*m_frequency*m_frequency + m_B*m_frequency;
  Time updatetime = Simulator::Now ();

  m_cpupower = vec_cpupwr[ (int)ceil(updatetime.GetSeconds()/0.2)];
  m_gpupower = vec_gpupwr[ (int)ceil(updatetime.GetSeconds()/0.2)];
  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  m_temperatureModel->UpdateTemperature (m_cpupower, m_gpupower, updatetime.GetSeconds());

  m_powerUpdateEvent = Simulator::Schedule (m_powerUpdateInterval,&PowerTraceModel::UpdatePower,this);

}

void
PowerTraceModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
  //m_energyDepletionCallback.Nullify ();
}



} // namespace ns3
