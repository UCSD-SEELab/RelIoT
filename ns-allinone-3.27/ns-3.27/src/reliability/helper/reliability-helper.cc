/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Vishwesh Rege
 *
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

#include "reliability-helper.h"
#include <ns3/reliability-model.h>
#include <ns3/temperature-model.h>
#include <ns3/performance-model.h>
#include <ns3/power-tracemodel.h>
#include <ns3/power-linearmodel.h>
#include <ns3/power-polynomialmodel.h>
#include <ns3/rapidcsv.h>
#include <ns3/log.h>
#include "ns3/names.h"
#include <vector>
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ReliabilityHelper");

ReliabilityHelper::ReliabilityHelper (void)
{
      perf = CreateObject<PerformanceModel> ();
      rm = CreateObject<ReliabilityModel> ();
      tm = CreateObject<TemperatureModel> ();
      ptm = CreateObject<PowerTraceModel> ();
      plm = CreateObject<PowerLinearModel> ();
      ppm = CreateObject<PowerPolynomialModel> ();
      powermodelselect = 2;
      m_Ts = 1.0;
      m_appname = "LinearRegression";
      m_datasize = 1000;
}

ReliabilityHelper::~ReliabilityHelper (void)
{

}

void
ReliabilityHelper::EnableLogComponents (void)
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("ReliabilityModel", LOG_LEVEL_ALL);
  LogComponentEnable ("TemperatureModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PowerTraceModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PowerLinearModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PowerPolynomialModel", LOG_LEVEL_ALL);
}

 
void
ReliabilityHelper::SetPowerModel(std::string n0, const AttributeValue &v0)
{
 //std::cout<<n0<<std::endl;
  if(n0=="PowerTraceModel")
    powermodelselect = 1;
  if(n0=="PowerLinearModel")
    powermodelselect = 2;
  if(n0=="PowerPolynomialModel")
    powermodelselect = 3;
}
void
ReliabilityHelper::SetTrace(const std::vector<float> cpu_power, const std::vector<float> gpu_power)
{

m_cpupwr = cpu_power;
m_gpupwr = gpu_power;

}
void 
ReliabilityHelper::SetSamplingPeriod(const DoubleValue &v0)
{
//Ptr<DoubleChecker> v = MakeDoubleChecker<double>(0.0,10.0);
m_Ts = v0.Get();
}

void
ReliabilityHelper::SetApplication(std::string n0, const DoubleValue &v0)
{
  m_appname = n0;
  m_datasize = v0.Get();
}
void
ReliabilityHelper::Install (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      node->AggregateObject (rm);
      node->AggregateObject (tm);

      if(powermodelselect ==1)
      {
      node->AggregateObject (ptm);
      ptm->SetPowerTrace(m_cpupwr,m_gpupwr);
      ptm->SetSamplingPeriod(DoubleValue(m_Ts));
      ptm->RegisterTemperatureModel (tm);
      }
      if(powermodelselect ==2)
      {
      node->AggregateObject (plm);
      plm->RegisterPerformanceModel(perf);
      plm->SetApplication(m_appname,m_datasize);
      plm->RegisterTemperatureModel (tm);
      }
      if(powermodelselect ==3)
      {
      node->AggregateObject (plm);
      plm->RegisterTemperatureModel (tm);
      }
      tm->RegisterReliabilityModel (rm);
    }
}

} // namespace ns3

