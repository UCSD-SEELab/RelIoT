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

#ifndef POWER_POLYNOMIALMODEL_H
#define POWER_POLYNOMIALMODEL_H

//#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
#include "ns3/reliability-model.h"
#include "ns3/temperature-model.h"
#include "ns3/power-base.h"
#include <vector>
//#include "lr-wpan-phy.h"

namespace ns3 {

class PowerPolynomialModel : public PowerBase// : public DeviceEnergyModel, public LrWpanPhyListener
{
public:

  static TypeId GetTypeId (void);
  PowerPolynomialModel ();

  virtual ~PowerPolynomialModel ();

  /**
   * \param phy Pointer to PHY layer attached to device.
   *
   * Registers the LrWpanRadioEnergyModel as listener to the Phy.
   */
  //virtual void RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel);

  /**
   * \param phy Pointer to PHY layer attached to device.
   *
   * Registers the LrWpanRadioEnergyModel as listener to the Phy.
   */
  virtual void RegisterTemperatureModel (Ptr<TemperatureModel> temperatureModel);

  /**
   * \brief Sets pointer to EnergySouce installed on node.
   *
   * \param source Pointer to EnergySource installed on node.
   *
   * Implements DeviceEnergyModel::SetEnergySource.
   */
  //virtual void SetEnergySource (Ptr<EnergySource> source);

  // Setter & getters for state power consumption.
  
  virtual double GetA (void) const;
  virtual void SetA (double txCurrentA);
  virtual double GetB (void) const;
  virtual void SetB (double rxCurrentA);
  virtual double GetFrequency (void) const;
  virtual void SetFrequency (double frequency);  
  virtual void SetPowerTrace(const std::vector<float> cpu_power, const std::vector<float> gpu_power);

  /**
   * \returns Current state.
   */
  virtual std::vector<double> GetPower (void) const;

  /**
   * \brief Changes state of the LrWpanRadioEnergyMode.
   *
   * \param newState New state the LrWpan radio is in.
   *
   * Implements DeviceEnergyModel::ChangeState.
   */
  virtual void UpdatePower ();

private:
  virtual void DoDispose (void);

private:

  // The Energy Source and PHY associated with this model
  //Ptr<EnergySource> m_source;
  //Ptr<ReliabilityModel> m_reliabilityModel;

  Ptr<TemperatureModel> m_temperatureModel;

  // Member variables for current draw in different radio modes.
  double m_A;
  double m_B;
  double m_frequency;
  std::vector<float> vec_cpupwr;
  std::vector<float> vec_gpupwr;
  // This variable keeps track of the total energy consumed by this model.
  TracedValue<double> m_cpupower;
  TracedValue<double> m_gpupower;

  // State variables.
  EventId m_powerUpdateEvent;            // energy update event
  Time m_lastUpdateTime;          // time stamp of previous energy update
  Time m_powerUpdateInterval;            // energy update interval

};

} // namespace ns3

#endif
