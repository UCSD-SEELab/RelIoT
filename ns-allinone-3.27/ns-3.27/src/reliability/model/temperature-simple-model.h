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

#ifndef TEMPERATURE_SIMPLE_MODEL_H
#define TEMPERATURE_SIMPLE_MODEL_H


#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"
//#include "ns3/reliability-model.h"
#include "ns3/temperature-model.h"

namespace ns3 {

class TemperatureSimpleModel : public TemperatureModel// 
{
public:

  static TypeId GetTypeId (void);
  TemperatureSimpleModel ();
  virtual ~TemperatureSimpleModel ();

  /**
   * \param  Pointer to reliability object attached to the device.
   *
   * Registers the Reliability Model to Temperature Model.
   */
  //virtual void RegisterReliabilityModel (Ptr<ReliabilityModel> reliabilityModel);


  // Setter & getters.
  virtual double GetA (void) const;
  virtual void SetA (double A);
  virtual double GetB (void) const;
  virtual void SetB (double B);
  virtual double GetC (void) const;
  virtual void SetC (double C);
  virtual double GetD (void) const;
  virtual void SetD (double D);
  virtual double GetTenv (void) const;
  virtual void SetTenv (double Tenv);

  /**
   * \brief Updates the temperature.
   *
   */
  virtual void UpdateTemperature (double cpupower);

  /**
   * \returns Current state.
   */
  virtual double GetTemperature (void) const;

private:
  virtual void DoDispose (void);

private:

  //Ptr<ReliabilityModel> m_reliabilityModel;
  TracedValue<double> m_temperatureCPU;
  Time m_lastUpdateTime;          // time stamp of previous temperature update
  double m_Tenv;
  double m_A;
  double m_B;
  double m_C;
  double m_D;
};

} // namespace ns3

#endif /* TEMPERATURE_SIMPLE_MODEL_H */
