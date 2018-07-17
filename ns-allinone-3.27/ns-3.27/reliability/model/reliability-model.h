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

#ifndef RELIABILITY_MODEL_H
#define RELIABILITY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
#include "ns3/reliability-base.h"

namespace ns3 {

class ReliabilityModel : public ReliabilityBase// : public DeviceEnergyModel, public LrWpanPhyListener
{
public:

  static TypeId GetTypeId (void);
  ReliabilityModel ();
  virtual ~ReliabilityModel ();

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
  virtual double GetArea (void) const;
  virtual void SetArea (double rxCurrentA);

  /**
   * \returns Current state.
   */
  virtual double GetReliability (void) const;

  /**
   * \brief Changes state of the LrWpanRadioEnergyMode.
   *
   * \param newState New state the LrWpan radio is in.
   *
   * Implements DeviceEnergyModel::ChangeState.
   */
  virtual void UpdateReliability (double powerCon, double temperature);

private:
  virtual void DoDispose (void);

private:

  double m_A;
  double m_B;
  double m_area;

  // This variable keeps track of the reliability of this model.
  TracedValue<double> m_reliability;

  // State variables.
  Time m_lastUpdateTime;          // time stamp of previous energy update

};

} // namespace ns3

#endif /* RELIABILITY_MODEL_H */
