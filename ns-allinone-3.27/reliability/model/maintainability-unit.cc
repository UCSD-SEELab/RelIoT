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
#include "ns3/maintainability-model.h"
#include "ns3/constant-position-mobility-model.h"

NS_LOG_COMPONENT_DEFINE ("MaintainabilityUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MaintainabilityUnit);

TypeId
MaintainabilityUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MaintainabilityUnit")
    .SetParent<Object> ()
    .AddConstructor<MaintainabilityUnit> ()
    .AddAttribute ("PM Cost",
                   "PM Cost.",
                   DoubleValue (100),
                   MakeDoubleAccessor (&MaintainabilityUnit::SetCmCost,
                                       &MaintainabilityUnit::GetCmCost),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CM Cost",
                   "CM Cost.",
                   DoubleValue (200),
                   MakeDoubleAccessor (&MaintainabilityUnit::SetPmCost,
                                       &MaintainabilityUnit::GetPmCost),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Maintenance Cost",
                     "Maintenance cost of the device.",
                     MakeTraceSourceAccessor (&MaintainabilityUnit::m_maintenanceCost),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

MaintainabilityUnit::MaintainabilityUnit ()
{
  NS_LOG_FUNCTION (this);
  m_maintenanceCost = 0.0;
  m_lastUpdateTime = Seconds (0.0);
}

MaintainabilityUnit::~MaintainabilityUnit ()
{
  NS_LOG_FUNCTION (this);
}

void
MaintainabilityUnit::RegisterMobilityModel (Ptr<MobilityModel> mobilityModel)
{
  m_mobilityModel = mobilityModel;
  //phy->RegisterListener (this);
}

void
MaintainabilityUnit::AddDependentUnit (Ptr<MaintainabilityBase> maintenanceUnit)
{
  NS_LOG_FUNCTION (this);

}

void
MaintainabilityUnit::AddRedundantUnit (Ptr<MaintainabilityBase> maintenanceUnit)
{
  NS_LOG_FUNCTION (this);

}

void
MaintainabilityUnit::SetPmCost (double pmCost)
{
  NS_LOG_FUNCTION (this);
  m_pmCost = pmCost;
}

double
MaintainabilityUnit::GetPmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_pmCost;
}

void
MaintainabilityUnit::SetCmCost (double cmCost)
{
  NS_LOG_FUNCTION (this);
  m_cmCost = cmCost;
}

double
MaintainabilityUnit::GetCmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_cmCost;
}

void
MaintainabilityUnit::SetRho (double area)
{
  NS_LOG_FUNCTION (this);
  m_rho = rho;
}

double
MaintainabilityUnit::GetRho (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rho;
}


/*void
MaintainabilityUnit::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
MaintainabilityUnit::GetCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maintenanceCost;
}

void
MaintainabilityUnit::UpdateCost (double powerCon, double temperature)
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
MaintainabilityUnit::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}


} // namespace ns3
