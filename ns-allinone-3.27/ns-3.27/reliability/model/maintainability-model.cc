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

NS_LOG_COMPONENT_DEFINE ("MaintainabilityModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MaintainabilityModel);

TypeId
MaintainabilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MaintainabilityModel")
    .SetParent<Object> ()
    .AddConstructor<MaintainabilityModel> ()
    .AddAttribute ("PM Cost",
                   "PM Cost.",
                   DoubleValue (100),
                   MakeDoubleAccessor (&MaintainabilityModel::SetCmCost,
                                       &MaintainabilityModel::GetCmCost),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CM Cost",
                   "CM Cost.",
                   DoubleValue (200),
                   MakeDoubleAccessor (&MaintainabilityModel::SetPmCost,
                                       &MaintainabilityModel::GetPmCost),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Maintenance Cost",
                     "Maintenance cost of the device.",
                     MakeTraceSourceAccessor (&MaintainabilityModel::m_maintenanceCost),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

MaintainabilityModel::MaintainabilityModel ()
{
  NS_LOG_FUNCTION (this);
  m_maintenanceCost = 0.0;
  m_lastUpdateTime = Seconds (0.0);
}

MaintainabilityModel::~MaintainabilityModel ()
{
  NS_LOG_FUNCTION (this);
}

void
MaintainabilityModel::RegisterMobilityModel (Ptr<MobilityModel> mobilityModel)
{
  m_mobilityModel = mobilityModel;
  //phy->RegisterListener (this);
}

void
MaintainabilityModel::SetPmCost (double pmCost)
{
  NS_LOG_FUNCTION (this);
  m_pmCost = pmCost;
}

double
MaintainabilityModel::GetPmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_pmCost;
}

void
MaintainabilityModel::SetCmCost (double cmCost)
{
  NS_LOG_FUNCTION (this);
  m_cmCost = cmCost;
}

double
MaintainabilityModel::GetCmCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_cmCost;
}

void
MaintainabilityModel::SetRho (double area)
{
  NS_LOG_FUNCTION (this);
  m_rho = rho;
}

double
MaintainabilityModel::GetRho (void) const
{
  NS_LOG_FUNCTION (this);
  return m_rho;
}


/*void
MaintainabilityModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
MaintainabilityModel::GetCost (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maintenanceCost;
}

void
MaintainabilityModel::UpdateCost (double powerCon, double temperature)
{
  NS_LOG_FUNCTION (this << powerCon);

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

}

void
MaintainabilityModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}


} // namespace ns3
