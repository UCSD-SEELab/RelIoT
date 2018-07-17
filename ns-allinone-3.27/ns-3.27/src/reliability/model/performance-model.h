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

#ifndef PERFORMANCE_MODEL_H
#define PERFORMANCE_MODEL_H

//#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
#include <Eigen/Dense>
//#include "ns3/reliability-model.h"
using namespace Eigen;
namespace ns3 {

class PerformanceModel : public Object
{
public:

  static TypeId GetTypeId (void);
  PerformanceModel () ;
  virtual ~PerformanceModel ();

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
  virtual void SetA (double coeff1);
  virtual double GetB (void) const;
  virtual void SetB (double coeff2);
  virtual double GetC (void) const;
  virtual void SetC (double coeff3);


  virtual void SetApplication (std::string n0, const DoubleValue &v0);

  /**
   * \returns Current temperature.
   */
  virtual double GetExecTime (void) const;

private:
  virtual void DoDispose (void);

private:


  Time m_lastUpdateTime;          // time stamp of previous energy update
  double m_A;
  double m_B;
  double m_C;
  double m_datasize;
  TracedValue<double> m_exectime;

};

} // namespace ns3

#endif /* TEMPERATURE_BASE_H */