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
#include "ns3/power-model.h"
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream> //istringstream

//#include "ns3/energy-source.h"

NS_LOG_COMPONENT_DEFINE ("PowerModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PowerModel);

TypeId
PowerModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PowerModel")
    .SetParent<Object> ()
    .AddConstructor<PowerModel> ()
    .AddAttribute ("A",
                   "Parameter A.",
                   DoubleValue (6.0957e-11),    //1.21914316767465*0.0000001
                   MakeDoubleAccessor (&PowerModel::SetA,
                                       &PowerModel::GetA),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Parameter B.",
                   DoubleValue (4.7775e-05),    //0.0955492791339378
                   MakeDoubleAccessor (&PowerModel::SetB,
                                       &PowerModel::GetB),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("f",
                   "Frequency.",
                   DoubleValue (1000),
                   MakeDoubleAccessor (&PowerModel::SetFrequency,
                                       &PowerModel::GetFrequency),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("Power",
                     "Power of the device.",
                     MakeTraceSourceAccessor (&PowerModel::m_power),
                     "ns3::TracedValue::DoubleCallback")
  ; 
  return tid;
}

PowerModel::PowerModel ()
{
  NS_LOG_FUNCTION (this);
  m_power = 0.0;
  m_lastUpdateTime = Seconds (0.0);
  m_powerUpdateInterval = Seconds (1.0);
  m_A = 6.0957e-11;
  m_B = 4.7775e-05;
  m_frequency = 1000;
  //m_energyDepletionCallback.Nullify ();
  m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel


}

PowerModel::~PowerModel ()
{
  NS_LOG_FUNCTION (this);
}

/*void
PowerModel::RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel)
{
  m_reliabilityModel = reliabilityModel;
  //phy->RegisterListener (this);
}*/

void
PowerModel::RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel)
{
  m_temperatureModel = temperatureModel;
  //phy->RegisterListener (this);
}

/*
void
PowerModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);
  m_source = source;
}
*/

double
PowerModel::GetPower (void) const
{
  NS_LOG_FUNCTION (this);
  return m_power;
}

void
PowerModel::SetA (double A)
{
  NS_LOG_FUNCTION (this);
  m_A = A;
}

double
PowerModel::GetA (void) const
{
  NS_LOG_FUNCTION (this);
  return m_A;
}

void
PowerModel::SetB (double B)
{
  NS_LOG_FUNCTION (this);
  m_B = B;
}

double
PowerModel::GetB (void) const
{
  NS_LOG_FUNCTION (this);
  return m_B;
}

void
PowerModel::SetFrequency (double freqeuncy)
{
  NS_LOG_FUNCTION (this);
  m_frequency = freqeuncy;
}

double
PowerModel::GetFrequency (void) const
{
  NS_LOG_FUNCTION (this);
  return m_frequency;
}

void
PowerModel::UpdatePower ()
{
  NS_LOG_FUNCTION (this << m_power << m_frequency);
  //NS_LOG_FUNCTION ("m_A:" << m_A << " m_B:" << m_B << " m_frequency:" << m_frequency);

  if (Simulator::IsFinished ())
    {
      return;
    }

  Time duration = Simulator::Now () - m_lastUpdateTime;
  NS_ASSERT (duration.GetNanoSeconds () >= 0); // check if duration is valid

//  double supplyVoltage = m_source->GetSupplyVoltage ();

  m_powerUpdateEvent.Cancel ();
  //int FAC = 2;
  //m_power = 0.0000000001*0.0000000001*(m_A*m_frequency*m_frequency*m_frequency + m_B*m_frequency)/(1000*FAC);
  m_power = m_A*m_frequency*m_frequency*m_frequency + m_B*m_frequency;

  // update last update time stamp
  m_lastUpdateTime = Simulator::Now ();

  // notify energy source
  m_temperatureModel->UpdateTemperature (m_power, duration.GetSeconds());

  m_powerUpdateEvent = Simulator::Schedule (m_powerUpdateInterval,&PowerModel::UpdatePower,this);

}

void
PowerModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_temperatureModel = NULL;      // TemperatureModel
  //m_reliabilityModel = NULL;      // ReliabilityModel
  //m_energyDepletionCallback.Nullify ();
}



} // namespace ns3
