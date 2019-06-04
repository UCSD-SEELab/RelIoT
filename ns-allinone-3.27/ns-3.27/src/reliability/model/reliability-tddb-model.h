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

#ifndef RELIABILITY_TDDB_MODEL_H
#define RELIABILITY_TDDB_MODEL_H


#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
#include "ns3/reliability-model.h"

namespace ns3 {

class ReliabilityTDDBModel : public ReliabilityModel
{
public:

  static TypeId GetTypeId (void);
  ReliabilityTDDBModel ();
  virtual ~ReliabilityTDDBModel ();

  // Setter & getters for state power consumption.
  virtual double GetA (void) const;
  virtual void SetA (double A);
  virtual double GetB (void) const;
  virtual void SetB (double B);
  virtual double GetArea (void) const;
  virtual void SetArea (double area);

  /**
   * \returns Current reliability
   */
  virtual double GetReliability (void) const;

  /**
   * \brief 
   *
   * \param cpupower, temperature.
   *
   * Updates reliability.
   */
  virtual void UpdateReliability (double cpupower, double temperature);

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

#endif /* RELIABILITY_TDDB_MODEL_H */
