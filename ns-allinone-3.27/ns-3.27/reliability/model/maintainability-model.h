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

#ifndef MAINTAINABILITY_MODEL_H
#define MAINTAINABILITY_MODEL_H

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
#include "ns3/maintainability-base.h"

namespace ns3 {

class MaintainabilityModel : public MaintainabilityBase
{
public:

  static TypeId GetTypeId (void);
  MaintainabilityModel ();
  virtual ~MaintainabilityModel ();

  // Setter & getters.
  virtual double GetPmCost (void) const;
  virtual void SetPmCost (double pmCost);
  virtual double GetCmCost (void) const;
  virtual void SetCmCost (double cmCost);
  virtual double GetRho (void) const;
  virtual void SetRho (double rho);

  /**
   * \returns Current cost.
   */
  virtual double GetCost (void) const;

  /**
   * \brief Update maintenance cost.
   *
   * \param 
   *
   */
  virtual void UpdateCost (double powerCon, double temperature);

  virtual void RegisterMobilityModel (Ptr<MobilityModel> mobilityModel);

private:
  void DoDispose (void);

private:

  Ptr<MobilityModel> m_mobilityModel;

  double m_pmCost;
  double m_cmCost;
  double m_rho;

  // This variable keeps track of the total cost of maintenance of this node.
  TracedValue<double> m_maintenanceCost;

  // State variables.
  Time m_lastUpdateTime;          // time stamp of previous update

};

} // namespace ns3

#endif /* RELIABILITY_MODEL_H */
