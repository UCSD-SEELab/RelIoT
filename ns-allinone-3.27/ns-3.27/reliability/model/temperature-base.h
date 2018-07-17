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

#ifndef TEMPERATURE_BASE_H
#define TEMPERATURE_BASE_H

//#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
//#include "ns3/reliability-model.h"

namespace ns3 {

class TemperatureBase : public Object
{
public:

  static TypeId GetTypeId (void);
  TemperatureBase ();
  virtual ~TemperatureBase ();

  //virtual void RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel);

  /**
   * \brief Sets pointer to EnergySouce installed on node.
   *
   * \param source Pointer to EnergySource installed on node.
   *
   * Implements DeviceEnergyModel::SetEnergySource.
   */
  //virtual void SetEnergySource (Ptr<EnergySource> source);

  // Setter & getters
  virtual double GetA (void) const;
  virtual void SetA (double txCurrentA);
  virtual double GetB (void) const;
  virtual void SetB (double rxCurrentA);

  virtual void UpdateTemperature (double powerCon);

  /**
   * \returns Current temperature.
   */
  virtual double GetTemperature (void) const;

private:
  virtual void DoDispose (void);

private:

  // The Energy Source associated with this model
  //Ptr<EnergySource> m_source;
  //Ptr<ReliabilityModel> m_reliabilityModel;

  // This variable keeps track of the device temperature.
  TracedValue<double> m_temperature;

  Time m_lastUpdateTime;          // time stamp of previous energy update

  double m_A;
  double m_B;

};

} // namespace ns3

#endif /* TEMPERATURE_BASE_H */
