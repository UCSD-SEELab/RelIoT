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
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/traced-value.h"
#include "reliability-tddb-model.h"

NS_LOG_COMPONENT_DEFINE ("ReliabilityTDDBModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ReliabilityTDDBModel);

TypeId
ReliabilityTDDBModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ReliabilityTDDBModel")
    .SetParent<ReliabilityModel> ()
    .SetGroupName("Reliability")
    .AddConstructor<ReliabilityTDDBModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReliabilityTDDBModel::SetA,
                                       &ReliabilityTDDBModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&ReliabilityTDDBModel::SetB,
                                       &ReliabilityTDDBModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("area",
                   "Area.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReliabilityTDDBModel::SetArea,
                                       &ReliabilityTDDBModel::GetArea),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Reliability",
                     "Reliability of the device.",
                     MakeTraceSourceAccessor (&ReliabilityTDDBModel::m_reliability),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

ReliabilityTDDBModel::ReliabilityTDDBModel ()
{
  NS_LOG_FUNCTION (this);
  m_lastUpdateTime = Seconds (0.0);
  m_reliabilityUpdateInterval = Seconds(600);
}

ReliabilityTDDBModel::~ReliabilityTDDBModel ()
{
  NS_LOG_FUNCTION (this);
}


void
ReliabilityTDDBModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
}

void
ReliabilityTDDBModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
ReliabilityTDDBModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
ReliabilityTDDBModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
ReliabilityTDDBModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
ReliabilityTDDBModel::SetArea (double area)
{
  NS_LOG_FUNCTION (this);
  m_area = area;
}

double
ReliabilityTDDBModel::GetArea (void) const
{
  NS_LOG_FUNCTION (this);
  return m_area;
}

double
ReliabilityTDDBModel::GetReliability (void) const
{
  NS_LOG_FUNCTION (this);
  return m_reliability;
}

void
ReliabilityTDDBModel::UpdateReliability ()
{
  NS_LOG_FUNCTION (this << m_reliability << Simulator::Now ().GetSeconds ());
  double temperature = m_temperatureModel->GetAvgTemperature();

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  double offset = 0;
  double multtemp = 0.001;
  double multvolt = 0.1;
  double volt = 1;

  double time_max_years = 5;
  double shape_mod_parameter = offset + multtemp*temperature + multvolt*volt;
  double time_conversion = time_max_years/1000;
  double time_current_scaled = Simulator::Now ().GetSeconds () * time_conversion;
  double time_next_scaled = (Simulator::Now ().GetSeconds () + 1) * time_conversion;
  double damage_current = std::exp(-m_area * time_current_scaled * shape_mod_parameter) - std::exp(-m_area * time_next_scaled * shape_mod_parameter);
  m_reliability = m_reliability - 0.1*damage_current;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();
  m_reliabilityUpdateEvent = Simulator::Schedule (m_reliabilityUpdateInterval,&ReliabilityModel::UpdateReliability,this);

}

void
ReliabilityTDDBModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_source = NULL;
}


} // namespace ns3
