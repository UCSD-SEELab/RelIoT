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
#include "temperature-simple-model.h"


NS_LOG_COMPONENT_DEFINE ("TemperatureSimpleModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TemperatureSimpleModel);

TypeId
TemperatureSimpleModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TemperatureSimpleModel")
    .SetParent<TemperatureModel> ()
    .SetGroupName ("Temperature")
    .AddConstructor<TemperatureSimpleModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (0.14434),
                   MakeDoubleAccessor (&TemperatureSimpleModel::SetA,
                                       &TemperatureSimpleModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (0.98885),
                   MakeDoubleAccessor (&TemperatureSimpleModel::SetB,
                                       &TemperatureSimpleModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Parameter C.",
                   DoubleValue (0.04894698),
                   MakeDoubleAccessor (&TemperatureSimpleModel::SetC,
                                       &TemperatureSimpleModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("D",
                   "Parameter D.",
                   DoubleValue (-3.14462264),
                   MakeDoubleAccessor (&TemperatureSimpleModel::SetD,
                                       &TemperatureSimpleModel::GetD),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("m_Tenv",
                   "Ambient Temperature",
                   DoubleValue (25.0),
                   MakeDoubleAccessor (&TemperatureSimpleModel::SetTenv,
                                       &TemperatureSimpleModel::GetTenv),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Temperature",
                     "Temperature of the device.",
                     MakeTraceSourceAccessor (&TemperatureSimpleModel::m_temperatureCPU),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

TemperatureSimpleModel::TemperatureSimpleModel ()
{
  NS_LOG_FUNCTION (this);
  m_lastUpdateTime = Seconds (0.0);
  //m_reliabilityModel = NULL;      // ReliabilityModel
}

TemperatureSimpleModel::~TemperatureSimpleModel ()
{
  NS_LOG_FUNCTION (this);
}

// void
// TemperatureSimpleModel::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
// {
//   m_reliabilityModel = reliabilityModel;
// }

double
TemperatureSimpleModel::GetTemperature (void) const
{
  NS_LOG_FUNCTION (this);
  return m_temperatureCPU;
}

void
TemperatureSimpleModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
TemperatureSimpleModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
TemperatureSimpleModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;

}

double
TemperatureSimpleModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
TemperatureSimpleModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);
  m_C = C;
}

double
TemperatureSimpleModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}

void
TemperatureSimpleModel::SetD (double D)
{
  NS_LOG_FUNCTION (this);
  m_D = D;
}

double
TemperatureSimpleModel::GetD (void) const
{
  NS_LOG_FUNCTION (this);
  return m_D;
}

void
TemperatureSimpleModel::SetTenv (double Tenv)
{
  NS_LOG_FUNCTION (this);
  m_Tenv = Tenv;
}

double
TemperatureSimpleModel::GetTenv (void) const
{
  NS_LOG_FUNCTION (this);
  return m_Tenv;
}

void
TemperatureSimpleModel::UpdateTemperature (double cpupower)
{
  NS_LOG_FUNCTION (this << m_temperatureCPU);
  NS_LOG_DEBUG ("TemperatureSimpleModel:Updating temperature" << " at time = " << Simulator::Now ());
  m_temperatureCPU =  m_A*m_Tenv + m_B*m_temperatureCPU + m_C*cpupower + m_D;
  //m_reliabilityModel->UpdateReliability (cpupower, m_temperatureCPU);

}

void
TemperatureSimpleModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_source = NULL;
}



} // namespace ns3