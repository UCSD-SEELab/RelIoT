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
#include <Eigen/Dense>

using namespace Eigen;

NS_LOG_COMPONENT_DEFINE ("TemperatureModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TemperatureModel);

TypeId
TemperatureModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TemperatureModel")
    .SetParent<Object> ()
    .AddConstructor<TemperatureModel> ()
    /*.AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&TemperatureModel::SetA,
                                       &TemperatureModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (300),
                   MakeDoubleAccessor (&TemperatureModel::SetB,
                                       &TemperatureModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Parameter C.",
                   DoubleValue (300),
                   MakeDoubleAccessor (&TemperatureModel::SetC,
                                       &TemperatureModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Temperature",
                     "Temperature of the device.",
                     MakeTraceSourceAccessor (&TemperatureModel::m_temperatureCPU),
                     "ns3::TracedValue::DoubleCallback")*/
  ; 
  return tid;
}

TemperatureModel::TemperatureModel ()
{
  NS_LOG_FUNCTION (this);
  m_temperatureCPU = 25.0;
  m_temperatureGPU = 25.0;
 // m_Tenv(0) = 25.0;
  //m_Tenv(1) = 25.0;
  m_lastUpdateTime = Seconds (0.0);
 /* m_A(2,2);
  m_B(2,2);
  m_C(2,2);
  m_Tenv(2);*/
  m_A.row(0) << 1.0052, 0.0232;
  m_A.row(1) << -0.0841, 0.09269;
  m_B.row(0) << 0.00058403, -0.0064;
  m_B.row(1) << -0.0010, 0.0636;
  m_C.row(0) << 690.9977, 25.6622;
  m_C.row(1) << 636.6312, 100.3856;
  m_Tenv << 25.0,25.0;
  vec_temp << 48.75, 45.0;
  vec_statevar << 0.0910,0.0099; 

 /* m_A << 1.0052, 0.0232,
         -0.0841, 0.09269;
  m_B << 0.00058403, -0.0064,
         -0.0010, 0.0636;
  m_C << 690.9977, 25,6622,
         636.6312, 100.3856;*/


 
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
  return m_temperatureCPU;
}

void
TemperatureModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A.row(0) << 1.0052, 0.0232;
  m_A.row(1) << -0.0841, 0.09269;
}

Matrix2d
TemperatureModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
TemperatureModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B.row(0) << 0.00058403, -0.0064;
  m_B.row(1) << -0.0010, 0.0636;

}

Matrix2d
TemperatureModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
TemperatureModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);
  /*m_C =
{
    {690.9977,25.6622},
    {636.6312,100.3856}
};*/

  m_C.row(0) << 690.9977, 25.6622;
  m_C.row(1) << 636.6312, 100.3856;
}

Matrix2d
TemperatureModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}

void
TemperatureModel::UpdateTemperature (double cpupower, double gpupower,  double duration)
{
  NS_LOG_FUNCTION (this << m_temperatureCPU);
  vec_power << cpupower, gpupower; 
 //NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " powerCon:" << powerCon);
 

  //while (duration > 0){
    //m_temperature = m_A*m_temperature + m_B*powerCon + m_C*m_Tenv;
    //duration = duration - 1;
  //}
//  double supplyVoltage = m_source->GetSupplyVoltage ();
   vec_temp = m_C*vec_statevar;
   vec_statevar = m_A*vec_statevar + m_B*vec_power ;

   m_temperatureCPU = vec_temp(0);
   m_temperatureGPU = vec_temp(1);
   m_reliabilityModel->UpdateReliability (cpupower, m_temperatureCPU);

}

void
TemperatureModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_source = NULL;
}



} // namespace ns3
