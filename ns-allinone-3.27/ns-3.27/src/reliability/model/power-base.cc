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
#include <vector>
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/power-base.h"
//#include "ns3/energy-source.h"

NS_LOG_COMPONENT_DEFINE ("PowerBase");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PowerBase);

TypeId
PowerBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PowerBase")
    .SetParent<Object> ()
    .AddConstructor<PowerBase> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&PowerBase::SetA,
                                       &PowerBase::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.5),
                   MakeDoubleAccessor (&PowerBase::SetB,
                                       &PowerBase::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("f",
                   "Frequency.",
                   DoubleValue (2.4*1000000000),
                   MakeDoubleAccessor (&PowerBase::SetFrequency,
                                       &PowerBase::GetFrequency),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("CPU Power",
                     "CPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerBase::m_cpupower),
                     "ns3::TracedValue::DoubleCallback") 
    .AddTraceSource ("GPU Power",
                     "GPU Power of the device.",
                     MakeTraceSourceAccessor (&PowerBase::m_gpupower),
                     "ns3::TracedValue::DoubleCallback") 
  ; 
  return tid;
}

PowerBase::PowerBase ()
{
  NS_LOG_FUNCTION (this);
  //m_power = 40.0;
  m_cpupower = 0.0;
  m_gpupower = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (0.2);
  m_A = 1.0;
  m_B = 1.0;
  m_frequency = 2*1000000000;
  //m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
  vec_cpupwr = {1,2,3,4};
  vec_gpupwr = {1,2,3,4};
}

PowerBase::~PowerBase ()
{
  NS_LOG_FUNCTION (this);
}

/*
void
PowerBase::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
{
  m_reliabilityModel = reliabilityModel;
  //phy->RegisterListener (this);
}

void
PowerBase::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
  //phy->RegisterListener (this);
}
*/

/*
void
PowerBase::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}
*/

std::vector<double>
PowerBase::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  std::vector<double> v = {m_cpupower, m_gpupower};
  return v;
}


void
PowerBase::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
PowerBase::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PowerBase::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PowerBase::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PowerBase::SetFrequency (double freqeuncy)
{
  NS_LOG_FUNCTION (this);
  m_frequency = freqeuncy;
}

double
PowerBase::GetFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_frequency;
}

void
PowerBase::SetPowerTrace(const std::vector<float> cpu_power, const std::vector<float> gpu_power)
{
  NS_LOG_FUNCTION(this);
  vec_cpupwr = cpu_power;
  vec_gpupwr = gpu_power;
}
void
PowerBase::UpdatePower ()
{
  NS_LOG_FUNCTION (this << m_cpupower);
}

void
PowerBase::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
}



} // namespace ns3
