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
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
//#include "ns3/energy-source.h"
#include "temperature-base.h"

NS_LOG_COMPONENT_DEFINE ("TemperatureBase");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TemperatureBase);

TypeId
TemperatureBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TemperatureBase")
    .SetParent<Object> ()
    .AddConstructor<TemperatureBase> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&TemperatureBase::SetA,
                                       &TemperatureBase::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Thermal Resistance.",
                   DoubleValue (4.5),
                   MakeDoubleAccessor (&TemperatureBase::SetB,
                                       &TemperatureBase::GetB),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Temperature",
                     "Temperature of the device.",
                     MakeTraceSourceAccessor (&TemperatureBase::m_temperature),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

TemperatureBase::TemperatureBase ()
{
  NS_LOG_FUNCTION (this);
  m_temperature = 40.0;
  m_lastUpdateTime = Seconds (0.0);
  //m_reliabilityModel = NULL;      // ReliabilityModel
}

TemperatureBase::~TemperatureBase ()
{
  NS_LOG_FUNCTION (this);
}

/*void
TemperatureBase::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
{
  m_reliabilityModel = reliabilityModel;
  //reliabilityModel->RegisterListener (this);
}*/

/*void
TemperatureBase::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
TemperatureBase::GetTemperature (void) const
{
  NS_LOG_FUNCTION (this);
  return m_temperature;
}

void
TemperatureBase::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
TemperatureBase::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
TemperatureBase::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
TemperatureBase::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
TemperatureBase::UpdateTemperature (double powerCon)
{
  NS_LOG_FUNCTION (this << m_temperature);
}

void
TemperatureBase::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3
