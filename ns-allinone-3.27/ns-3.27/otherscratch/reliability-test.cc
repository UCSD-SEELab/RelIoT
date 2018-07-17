                /* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
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
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/energy-module.h"
#include <ns3/reliability-model.h>
#include <ns3/temperature-model.h>
#include <ns3/power-model.h>
#include <ns3/reliability-helper.h>


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <sys/stat.h>
//#include "ns3/netanim-module.h"
#include <iomanip>

using namespace std;
using namespace ns3;

Ptr<PacketSink> sink; 

int main (int argc, char *argv[])
{

  uint32_t nSta = 1;
  uint32_t nAp = 1;
  string DataMode = "OfdmRate6Mbps" ; 
  double simulationTime = 10;
  CommandLine cmd;
  cmd.AddValue ("nSta", "Number of wifi STA devices", nSta);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);


  cmd.Parse (argc,argv);


  NodeContainer apNode;
  apNode.Create(nAp);
  NodeContainer staNodes;
  staNodes.Create(nSta);

  YansWifiChannelHelper channel ; 

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetErrorRateModel ("ns3::YansErrorRateModel");
  phy.SetChannel (channel.Create ());

  WifiHelper wifi ;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue("OfdmRate6Mbps"),
                                    "ControlMode", StringValue("OfdmRate6Mbps"));



  WifiMacHelper mac;


  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, staNodes);


  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, apNode);

  // Mobility
  MobilityHelper mobility;


  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (500.0, 500.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");


  mobility.Install (staNodes);


  MobilityHelper mobilityAp;

  mobilityAp.SetPositionAllocator (positionAlloc);
  mobilityAp.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityAp.Install(apNode);


  // Internet stack
  InternetStackHelper stack;
  stack.Install (apNode);
  stack.Install (staNodes);

  Ipv4AddressHelper address;


  address.SetBase ("192.168.0.0", "255.255.0.0");
  Ipv4InterfaceContainer staNodeInterface;
  Ipv4InterfaceContainer apNodeInterface;

  staNodeInterface = address.Assign (staDevices);
  apNodeInterface = address.Assign (apDevices);

  ReliabilityHelper relihelp;
  relihelp.Install(staNodes);
  

  // Setting applications
  ApplicationContainer sourceApplications, sinkApplications;
  uint32_t portNumber = 9;
  for (uint8_t index = 0; index < nSta; ++index)
    {

          auto ipv4 = apNode.Get (0)->GetObject<Ipv4> ();
          const auto address = ipv4->GetAddress (1, 0).GetLocal ();
          InetSocketAddress sinkSocket (address, portNumber++);
          OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
          onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
          onOffHelper.SetAttribute ("DataRate", DataRateValue (6000000 / nSta));
          onOffHelper.SetAttribute ("PacketSize", UintegerValue(1472)); //bytes
          sourceApplications.Add (onOffHelper.Install (staNodes.Get (index)));
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
          sinkApplications.Add (packetSinkHelper.Install (apNode.Get (0)));
          sink = StaticCast<PacketSink> (sinkApplications.Get (index));
    }


  sinkApplications.Start (Seconds (0.0));
  sinkApplications.Stop (Seconds (simulationTime + 1));
  sourceApplications.Start (Seconds (1.0));
  sourceApplications.Stop (Seconds (simulationTime + 1));


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (simulationTime + 1));
 // AnimationInterface anim("animationwifi.xml");
  Simulator::Run ();
  Simulator::Destroy (); }
