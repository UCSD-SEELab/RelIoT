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
#include "performance-model.h"
#include <Eigen/Dense>

using namespace Eigen;
NS_LOG_COMPONENT_DEFINE ("PerformanceModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PerformanceModel);

TypeId
PerformanceModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PerformanceModel")
    .SetParent<Object> ()
    .AddConstructor<PerformanceModel> ()
    .AddAttribute ("A",
                   "Coeff A.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&PerformanceModel::SetA,
                                       &PerformanceModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Coeff B.",
                   DoubleValue (2.28*pow(10,-4)),
                   MakeDoubleAccessor (&PerformanceModel::SetB,
                                       &PerformanceModel::GetB),
                   MakeDoubleChecker<double> ())
   .AddAttribute ("C",
                   "Coeff C.",
                   DoubleValue (9.38*pow(10,-1)),
                   MakeDoubleAccessor (&PerformanceModel::SetC,
                                       &PerformanceModel::GetC),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("ExecutionTime",
                     "Execution Time of the App.",
                     MakeTraceSourceAccessor (&PerformanceModel::m_exectime),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

PerformanceModel::PerformanceModel ()
{
  NS_LOG_FUNCTION (this);
  m_exectime = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_datasize = 0.0;
}

PerformanceModel::~PerformanceModel ()
{
  NS_LOG_FUNCTION (this);
}

double
PerformanceModel::GetExecTime (void) const
{
  NS_LOG_FUNCTION (this);
  return m_exectime;
}


void
PerformanceModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}
double
PerformanceModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PerformanceModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PerformanceModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PerformanceModel::SetC (double C)
{
  NS_LOG_FUNCTION (this);

  m_C = C ;
}

double
PerformanceModel::GetC (void) const
{
  NS_LOG_FUNCTION (this);
  return m_C;
}

void
PerformanceModel::SetApplication (std::string n0, const DoubleValue &v0)
{
  NS_LOG_FUNCTION (this);
  m_datasize = v0.Get() ;
  if(n0 == "LinearRegression")
  {m_A = 0.0;
   m_B = 5.32*pow(10,-2);
   m_C = 2.40*pow(10,1);
   m_exectime = m_B*m_datasize + m_C;
  }
  if(n0 == "AdaBoost")
  {m_A = 0.0;
   m_B = 2.28*pow(10,-4);
   m_C = (-9.38)*pow(10,-1);
   m_exectime = m_B*m_datasize + m_C;
}

}

void
PerformanceModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3