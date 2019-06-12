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
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "performance-simple-model.h"


NS_LOG_COMPONENT_DEFINE ("PerformanceSimpleModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PerformanceSimpleModel);

TypeId
PerformanceSimpleModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PerformanceSimpleModel")
    .SetParent<PerformanceModel> ()
    .SetGroupName("Performance")
    .AddConstructor<PerformanceSimpleModel> ()
    .AddAttribute ("A",
                   "Coeff A.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&PerformanceSimpleModel::SetA,
                                       &PerformanceSimpleModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Coeff B.",
                   DoubleValue (2.28*pow(10,-4)),
                   MakeDoubleAccessor (&PerformanceSimpleModel::SetB,
                                       &PerformanceSimpleModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("C",
                   "Coeff C.",
                   DoubleValue (9.38*pow(10,-1)),
                   MakeDoubleAccessor (&PerformanceSimpleModel::SetC,
                                       &PerformanceSimpleModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("datasize",
                   "Data Size",
                   DoubleValue (100),
                   MakeDoubleAccessor (&PerformanceSimpleModel::SetDataSize,
                                       &PerformanceSimpleModel::GetDataSize),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("ExecutionTime",
                     "Execution Time of the App.",
                     MakeTraceSourceAccessor (&PerformanceSimpleModel::m_exectime),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

PerformanceSimpleModel::PerformanceSimpleModel ()
{
  NS_LOG_FUNCTION (this);
  m_lastUpdateTime = Seconds (0.0);
}

PerformanceSimpleModel::~PerformanceSimpleModel ()
{
  NS_LOG_FUNCTION (this);
}

double
PerformanceSimpleModel::GetExecTime (void) const
{
  NS_LOG_FUNCTION (this);
  return m_exectime;
}


void
PerformanceSimpleModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}
double
PerformanceSimpleModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PerformanceSimpleModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PerformanceSimpleModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PerformanceSimpleModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);

  m_C = C ;
}

double
PerformanceSimpleModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}

void
PerformanceSimpleModel::SetDataSize (const DoubleValue &v0)
{
  NS_LOG_FUNCTION (this);

  m_datasize = v0.Get();
}

double
PerformanceSimpleModel::GetDataSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_datasize;
}

void
PerformanceSimpleModel::SetPacketSize (const DoubleValue &v1)
{
  NS_LOG_FUNCTION (this);
  m_packetSize = v1.Get() ;
}

double
PerformanceSimpleModel::GetPacketSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_packetSize;
}

void
PerformanceSimpleModel::SetApplication (std::string m_appName, const DoubleValue &v0)
{
  NS_LOG_FUNCTION (this);
  m_datasize = v0.Get() ;
//   if(n0 == "LinearRegression")
//   {m_A = 0.0;
//    m_B = 5.32*pow(10,-2);
//    m_C = 2.40*pow(10,1);
//    m_exectime = (m_B*m_datasize)/1000 + m_C;
//   }
//   if(n0 == "AdaBoost")
//   {m_A = 0.0;
//    m_B = 2.28*pow(10,-4);
//    m_C = (-9.38)*pow(10,-1);
//    m_exectime = (m_B*m_datasize)/1000 + m_C;
// }
//   if(n0 == "MedianFilter")
//   {m_A = 0.0;
//    m_B = 2.28*pow(10,-4);
//    m_C = (-9.38)*pow(10,-1);
//    m_exectime = (m_B*m_datasize)/1000 + m_C;
// }

//   if(n0 == "NeuralNetwork")
//   {m_A = 0.0;
//    m_B = 2.28*pow(10,-4);
//    m_C = (-9.38)*pow(10,-1);
//    m_exectime = (m_B*m_datasize)/1000 + m_C;
// }

  if(m_deviceType == "RaspberryPi")
  {
    if(m_appName == "LinearRegression")
    {
      m_A = 0.0;
      m_B = 5.32*pow(10,-2);
      m_C = 2.40*pow(10,1);
      m_exectime = (m_B*m_datasize)/1000 + m_C;
    }
    else if(m_appName == "AdaBoost")
    {
      m_A = 0.0;
      m_B = 2.28*pow(10,-4);
      m_C = (-9.38)*pow(10,-1);
      m_exectime = (m_B*m_datasize)/1000 + m_C;
    }
    else if(m_appName == "NeuralNetwork")
    {
      m_A = 0.0;
      m_B = 2.28*pow(10,-4);
      m_C = (-9.38)*pow(10,-1);
      m_exectime = (m_B*m_datasize)/1000 + m_C;
    }
    else
    {
      NS_FATAL_ERROR ("PerformanceSimpleModel:Undefined application for this device: " << m_appName);
    }
  }
  else if(m_deviceType == "Arduino")
  {
    if(m_appName == "MedianFilter")
    {
      m_A = 0.0;
      m_B = 2.28*pow(10,-4);
      m_C = (-9.38)*pow(10,-1);
      m_exectime = (m_B*m_datasize)/1000 + m_C;
    }
    else
    {
      NS_FATAL_ERROR ("PerformanceSimpleModel:Undefined application for this device: " << m_appName);
    }
  }
  else
  {
    NS_FATAL_ERROR ("PerformanceSimpleModel:Undefined device type: " << m_deviceType);
  }
  
}

void
PerformanceSimpleModel::SetDeviceType (std::string devicetype)
{
  NS_LOG_FUNCTION (this);
  m_deviceType = devicetype;
}

void
PerformanceSimpleModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3