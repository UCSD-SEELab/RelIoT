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
#include "ns3/maintainability-base.h"

NS_LOG_COMPONENT_DEFINE ("MaintainabilityBase");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MaintainabilityBase);

TypeId
MaintainabilityBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MaintainabilityBase")
    .SetParent<Object> ()
    .AddConstructor<MaintainabilityBase> ()
    .AddAttribute ("PM Cost",
                   "PM Cost.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&MaintainabilityBase::SetCmCost,
                                       &MaintainabilityBase::GetCmCost),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CM Cost",
                   "CM Cost.",
                   DoubleValue (4.5),
                   MakeDoubleAccessor (&MaintainabilityBase::SetPmCost,
                                       &MaintainabilityBase::GetPmCost),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Maintenance Cost",
                     "Maintenance cost of the device.",
                     MakeTraceSourceAccessor (&MaintainabilityBase::m_maintenanceCost),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

MaintainabilityBase::MaintainabilityBase ()
{
  NS_LOG_FUNCTION (this);
  m_maintenanceCost = 0.0;
  m_lastUpdateTime = Seconds (0.0);
}

MaintainabilityBase::~MaintainabilityBase ()
{
  NS_LOG_FUNCTION (this);
}

void
MaintainabilityBase::SetPmCost (double pmCost)
{
  NS_LOG_FUNCTION (this);
  m_pmCost = pmCost;
}

double
MaintainabilityBase::GetPmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_pmCost;
}

void
MaintainabilityBase::SetCmCost (double cmCost)
{
  NS_LOG_FUNCTION (this);
  m_cmCost = cmCost;
}

double
MaintainabilityBase::GetCmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_cmCost;
}

void
MaintainabilityBase::SetRho (double area)
{
  NS_LOG_FUNCTION (this);
  m_rho = rho;
}

double
MaintainabilityBase::GetRho (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rho;
}


/*void
MaintainabilityBase::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
MaintainabilityBase::GetCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maintenanceCost;
}

void
MaintainabilityBase::UpdateCost (double powerCon, double temperature)
{
  NS_LOG_FUNCTION (this << powerCon);

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

}

void
MaintainabilityBase::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}


} // namespace ns3
