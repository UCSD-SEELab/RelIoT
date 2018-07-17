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
#include "temperature-model.h"

NS_LOG_COMPONENT_DEFINE ("TemperatureModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TemperatureModel);

TypeId
TemperatureModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TemperatureModel")
    .SetParent<Object> ()
    .AddConstructor<TemperatureModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&TemperatureModel::SetA,
                                       &TemperatureModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Thermal Resistance.",
                   DoubleValue (300),
                   MakeDoubleAccessor (&TemperatureModel::SetB,
                                       &TemperatureModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Temperature",
                     "Temperature of the device.",
                     MakeTraceSourceAccessor (&TemperatureModel::m_temperature),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

TemperatureModel::TemperatureModel ()
{
  NS_LOG_FUNCTION (this);
  m_temperature = 40.0;
  m_lastUpdateTime = Seconds (0.0);
  m_reliabilityModel = NULL;      // ReliabilityModel
}

TemperatureModel::~TemperatureModel ()
{
  NS_LOG_FUNCTION (this);
}

void
TemperatureModel::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
{
  m_reliabilityModel = reliabilityModel;
  //reliabilityModel->RegisterListener (this);
}

/*void
TemperatureModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}*/

double
TemperatureModel::GetTemperature (void) const
{
  NS_LOG_FUNCTION (this);
  return m_temperature;
}

void
TemperatureModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
TemperatureModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
TemperatureModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
TemperatureModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
TemperatureModel::UpdateTemperature (double powerCon, double duration)
{
  NS_LOG_FUNCTION (this << m_temperature);
  //NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " powerCon:" << powerCon);


  while (duration > 0){
    m_temperature = m_A*m_temperature + m_B*powerCon;
    duration = duration - 1;
  }
//  double supplyVoltage = m_source->GetSupplyVoltage ();

  m_reliabilityModel->UpdateReliability (powerCon, m_temperature);

}

void
TemperatureModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_source = NULL;
}



} // namespace ns3
