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
 * Authors: Vishwesh Rege <vrege2012@gmail.com>
 */
#ifndef RELIABILITY_HELPER_H
#define RELIABILITY_HELPER_H

#include <ns3/node-container.h>
#include <ns3/trace-helper.h>
#include <vector>
#include <ns3/reliability-model.h>
#include <ns3/temperature-model.h>
#include <ns3/performance-model.h>
#include <ns3/power-tracemodel.h>
#include <ns3/power-linearmodel.h>
#include <ns3/power-polynomialmodel.h>

namespace ns3 {

/**
 * \ingroup reliability
 *
 * \brief helps to manage and create Reliability objects
 *
 * This class can help to create IEEE 802.15.4 NetDevice objects
 * and to configure their attributes during creation.  It also contains
 * additional helper functions used by client code.
 */

class ReliabilityHelper
{
public:
  /**
   * \brief Create a LrWpan helper in an empty state.  By default, a
   * SingleModelSpectrumChannel is created, with a 
   * LogDistancePropagationLossModel and a ConstantSpeedPropagationDelayModel.
   *
   * To change the channel type, loss model, or delay model, the Get/Set
   * Channel methods may be used.
   */
  ReliabilityHelper (void);

  virtual ~ReliabilityHelper (void);

  /**
   * \brief Install a LrWpanNetDevice and the associated structures (e.g., channel) in the nodes.
   * \param c a set of nodes
   * \returns A container holding the added net devices.
   */
  void Install (NodeContainer c);

  /**
   * Helper to enable all LrWpan log components with one statement
   */
  void EnableLogComponents (void);

  
  void SetPowerModel(std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue ());

  void SetTrace(const std::vector<float> cpu_power, const std::vector<float> gpu_power);
  void SetSamplingPeriod(const DoubleValue &v0);
  void SetApplication(std::string n0, const DoubleValue &v0);
private:
  Ptr<ReliabilityModel> rm;
  Ptr<TemperatureModel> tm;
  Ptr<PowerTraceModel> ptm;
  Ptr<PowerLinearModel> plm;
  Ptr<PowerPolynomialModel> ppm;
  Ptr<PerformanceModel> perf;
  int powermodelselect;
  std::vector<float> m_cpupwr;
  std::vector<float> m_gpupwr;
  double m_Ts;
  std::string m_appname;
  double m_datasize;

  // Disable implicit constructors
  /**
   * \brief Copy constructor - defined and not implemented.
   */
  ReliabilityHelper (ReliabilityHelper const &);
  /**
   * \brief Copy constructor - defined and not implemented.
   * \returns
   */
  ReliabilityHelper& operator= (ReliabilityHelper const &);


};

}

#endif /* RELIABILITY_HELPER_H */
