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
 * Author: Vishwesh Rege <vrege2012@gmail.com>
 */

#include "reliability-helper.h"
#include <ns3/reliability-model.h>
#include <ns3/temperature-model.h>
#include <ns3/power-model.h>
#include <ns3/log.h>
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ReliabilityHelper");

ReliabilityHelper::ReliabilityHelper (void)
{

}

ReliabilityHelper::~ReliabilityHelper (void)
{

}

void
ReliabilityHelper::EnableLogComponents (void)
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("ReliabilityModel", LOG_LEVEL_ALL);
}

void
ReliabilityHelper::Install (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;

      Ptr<ReliabilityModel> model = CreateObject<ReliabilityModel> ();
      node->AggregateObject (model);


    }
}

} // namespace ns3

