/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/command-line.h"
//#include "ns3/simple-device-energy-model.h"
//#include "ns3/energy-source-container.h"
#include "ns3/power-model.h"
#include "ns3/temperature-model.h"
#include "ns3/reliability-model.h"
#include "ns3/reliability-helper.h"
#include "ns3/internet-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("rel3-example");
void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy << "J");
}

void
ReliabilityTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Reliability = " << newValue << std::endl;
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
TemperatureTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Temperature = " << newValue << std::endl;
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
PowerTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Power = " << newValue << std::endl;
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
PrintInfo (NodeContainer nodes)
{
  for(uint32_t i=0;i<nodes.GetN();i++)
  {
    std::cout<<"At time "<< Simulator::Now().GetSeconds()<<", NodeId = "<<nodes.Get(i)->GetId();
    std::cout << " Power = " << nodes.Get(i)->GetObject<PowerModel>()->GetPower();
    std::cout << " Temperature = " << nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature();
    std::cout << " Reliability = " << nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability()<<std::endl;
  }
  if (!Simulator::IsFinished ())
  {
    Simulator::Schedule (Seconds (1),&PrintInfo,nodes);
  }
}


int
main (int argc, char **argv)
{

  //LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);

  int i=0,num_nodes=2;

  CommandLine cmd;
  cmd.Parse (argc, argv);
  


  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (num_nodes);

  // Install mobility
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> nodesPositionAlloc = CreateObject<ListPositionAllocator> ();
  nodesPositionAlloc->Add (Vector (0.0, 0.0, 0.0));
  nodesPositionAlloc->Add (Vector (50.0, 0.0, 0.0));
  mobility.SetPositionAllocator (nodesPositionAlloc);
  mobility.Install (nodes);



  //LRWPAN PART
  NS_LOG_INFO ("Create channels.");
  LrWpanHelper lrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  // lrWpanHelper.EnableLogComponents();
  NetDeviceContainer devContainer = lrWpanHelper.Install(nodes);
  lrWpanHelper.AssociateToPan (devContainer, 10);

  std::cout << "Created " << devContainer.GetN() << " devices" << std::endl;
  std::cout << "There are " << nodes.GetN() << " nodes" << std::endl;

  /* Install IPv4/IPv6 stack */
  NS_LOG_INFO ("Install Internet stack.");
  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.Install (nodes);

  // Install 6LowPan layer
  NS_LOG_INFO ("Install 6LoWPAN.");
  SixLowPanHelper sixlowpan;
  NetDeviceContainer six1 = sixlowpan.Install (devContainer);

  NS_LOG_INFO ("Assign addresses.");
  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer ip6c = ipv6.Assign (six1);

  NS_LOG_INFO ("Create Applications.");

  /* Create a Ping6 application to send ICMPv6 echo request from node zero to
   * all-nodes (ff02::1).
   */
  uint32_t packetSize = 10;
  uint32_t maxPacketCount = 5;
  Time interPacketInterval = Seconds (1.);
  Ping6Helper ping6;

  ping6.SetLocal (ip6c.GetAddress (0, 1));
  ping6.SetRemote (ip6c.GetAddress (1, 1));
  // ping6.SetRemote (Ipv6Address::GetAllNodesMulticast ());

  ping6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  ping6.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping6.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = ping6.Install (nodes.Get (0));
  apps.Start (Seconds (60.0));
  apps.Stop (Seconds (3600.0));


  AsciiTraceHelper ascii;
  lrWpanHelper.EnableAsciiAll (ascii.CreateFileStream ("ping6wsn.tr"));
  lrWpanHelper.EnablePcapAll (std::string ("ping6wsn"), true);


  //Install Reliability
  ReliabilityHelper reliabilityHelper;
  reliabilityHelper.Install(nodes);

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");

  Time now = Simulator::Now ();

  PrintInfo (nodes);

  for(i=0;i<num_nodes;i++)
  Simulator::Schedule (now,&PowerModel::UpdatePower,nodes.Get (i)->GetObject<PowerModel> ());
  now += Seconds (60);

  Simulator::Stop (now);
  Simulator::Run ();
  Simulator::Destroy ();

}

