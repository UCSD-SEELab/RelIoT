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
 */
#ifndef TEMPERATURE_HELPER_H
#define TEMPERATURE_HELPER_H

#include <ns3/node-container.h>
#include <ns3/trace-helper.h>

namespace ns3 {

/**
 *
 * \brief helps to manage and create Temperature objects
 *

 */

class TemperatureHelper
{
public:
  /**
  
   */
  TemperatureHelper (void);

  virtual ~TemperatureHelper (void);

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

private:
  // Disable implicit constructors
  /**
   * \brief Copy constructor - defined and not implemented.
   */
  TemperatureHelper (TemperatureHelper const &);
  /**
   * \brief Copy constructor - defined and not implemented.
   * \returns
   */
  TemperatureHelper& operator= (TemperatureHelper const &);

};

}

#endif /* TEMPERATURE_HELPER_H */
