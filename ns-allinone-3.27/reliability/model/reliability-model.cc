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
#include "ns3/reliability-model.h"

NS_LOG_COMPONENT_DEFINE ("ReliabilityModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ReliabilityModel);

TypeId
ReliabilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ReliabilityModel")
    .SetParent<Object> ()
    .AddConstructor<ReliabilityModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReliabilityModel::SetA,
                                       &ReliabilityModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&ReliabilityModel::SetB,
                                       &ReliabilityModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("area",
                   "Area.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReliabilityModel::SetArea,
                                       &ReliabilityModel::GetArea),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Reliability",
                     "Reliability of the device.",
                     MakeTraceSourceAccessor (&ReliabilityModel::m_reliability),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

ReliabilityModel::ReliabilityModel ()
{
  NS_LOG_FUNCTION (this);
  m_reliability = 1.0;
  m_A = 1.0;
  m_B = 0.5;
  m_area = 1.0;
  m_lastUpdateTime = Seconds (0.0);
}

ReliabilityModel::~ReliabilityModel ()
{
  NS_LOG_FUNCTION (this);
}

void
ReliabilityModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
ReliabilityModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
ReliabilityModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
ReliabilityModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
ReliabilityModel::SetArea (double area)
{
  NS_LOG_FUNCTION (this);
  m_area = area;
}

double
ReliabilityModel::GetArea (void) const
{
  NS_LOG_FUNCTION (this);
  return m_area;
}

/*void
ReliabilityModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
ReliabilityModel::GetReliability (void) const
{
  NS_LOG_FUNCTION (this);
  return m_reliability;
}

void
ReliabilityModel::UpdateReliability (double powerCon, double temperature)
{
  NS_LOG_FUNCTION (this << m_reliability << Simulator::Now ().GetSeconds ());


  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  float offset = 0;
  float multtemp = 0.001;
  float multvolt = 0.1;
  float volt = 1;

  float time_max_years = 5;
  float shape_mod_parameter = offset + multtemp*temperature + multvolt*volt;
  float time_conversion = time_max_years/1000;
  float time_current_scaled = Simulator::Now ().GetSeconds () * time_conversion;
  float time_next_scaled = (Simulator::Now ().GetSeconds () + 1) * time_conversion;
  float damage_current = std::exp(-m_area * time_current_scaled * shape_mod_parameter) - std::exp(-m_area * time_next_scaled * shape_mod_parameter);
  m_reliability = m_reliability - damage_current;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

}

void
ReliabilityModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_source = NULL;
}


} // namespace ns3
