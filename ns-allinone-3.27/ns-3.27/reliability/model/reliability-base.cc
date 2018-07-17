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
#include "ns3/reliability-base.h"

NS_LOG_COMPONENT_DEFINE ("ReliabilityBase");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ReliabilityBase);

TypeId
ReliabilityBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ReliabilityBase")
    .SetParent<Object> ()
    .AddConstructor<ReliabilityBase> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&ReliabilityBase::SetA,
                                       &ReliabilityBase::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.5),
                   MakeDoubleAccessor (&ReliabilityBase::SetB,
                                       &ReliabilityBase::GetB),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Reliability",
                     "Reliability of the device.",
                     MakeTraceSourceAccessor (&ReliabilityBase::m_reliability),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

ReliabilityBase::ReliabilityBase ()
{
  NS_LOG_FUNCTION (this);
  m_reliability = 100.0;
  m_lastUpdateTime = Seconds (0.0);
}

ReliabilityBase::~ReliabilityBase ()
{
  NS_LOG_FUNCTION (this);
}

void
ReliabilityBase::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
ReliabilityBase::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
ReliabilityBase::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
ReliabilityBase::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
ReliabilityBase::SetArea (double area)
{
  NS_LOG_FUNCTION (this);
  m_area = area;
}

double
ReliabilityBase::GetArea (void) const
{
  NS_LOG_FUNCTION (this);
  return m_area;
}


/*void
ReliabilityBase::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
ReliabilityBase::GetReliability (void) const
{
  NS_LOG_FUNCTION (this);
  return m_reliability;
}

void
ReliabilityBase::UpdateReliability (double powerCon, double temperature)
{
  NS_LOG_FUNCTION (this << powerCon);

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  m_reliability = m_A*m_reliability + m_B*powerCon;
//  double supplyVoltage = m_source->GetSupplyVoltage ();

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

}

void
ReliabilityBase::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}


} // namespace ns3
