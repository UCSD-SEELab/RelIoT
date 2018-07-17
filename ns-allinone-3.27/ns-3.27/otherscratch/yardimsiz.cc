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

// Global variables for use in callbacks.
double g_signalDbmAvg;
double g_noiseDbmAvg;
uint32_t g_samples;
uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */
Ptr<PacketSink> sink;  

void
Rx (Ptr<const Packet> packet, const Address& add)
{
  uint8_t *buffer = new uint8_t[packet->GetSize ()];
  packet->CopyData(buffer, packet->GetSize ());
  std::string s = std::string((char*)buffer);
  //std::cout << s << std::endl; 
  
}


void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
Time now = Simulator::Now ();
Vector position = model->GetPosition ();
//Vector vel = model->GetVelocity ();
//std::cout << context << " x = " << position.x << ", y = " << position.y << " , z = " << position.z<<std::endl;
//std::cout << "Velocity:" << vel.x << ", y=" << vel.y
//            << ", z=" << vel.z <<std::endl;
double distance = sqrt(pow((position.x -500.0), 2.0) + pow((position.y -500.0), 2.0) + pow((position.z-0.0), 2.0));
std::cout << now.GetSeconds () << "s: \t" <<"Distance: \t" << distance<<std::endl;
}

void
GetDistance_From (uint32_t nSta, Ptr<NodeContainer> staNodes, Ptr<NodeContainer> apNode)
{
  Time now = Simulator::Now ();
  Ptr<MobilityModel> model1 = (apNode->Get(0))->GetObject<MobilityModel>();
  Ptr<MobilityModel> model2 = (staNodes->Get(0))->GetObject<MobilityModel>();
  double distance = model1->GetDistanceFrom (model2);
  std::cout << now.GetSeconds () << "s: \t" << "Distance:" << distance ;
} 

void    
CalculateThroughput ()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  double cur = (sink->GetTotalRx () - lastTotalRx) * (double) 8 / 1e5;     /* Convert Application RX Packets to MBits. */
  std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
  lastTotalRx = sink->GetTotalRx ();
  Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
}

void MonitorSniffRx (Ptr<const Packet> packet,
                     uint16_t channelFreqMhz,
                     WifiTxVector txVector,
                     MpduInfo aMpdu,
                     SignalNoiseDbm signalNoise)

{
  g_samples++;
  g_signalDbmAvg += ((signalNoise.signal - g_signalDbmAvg) / g_samples);
  g_noiseDbmAvg += ((signalNoise.noise - g_noiseDbmAvg) / g_samples);
}

void PrintSNR()
{
  Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
  //YansErrorRateModel error;
  //double errorrate = error.GetQamBer((g_signalDbmAvg - g_noiseDbmAvg) );
  std::cout << now.GetSeconds () << "s: \t" << std::setw(8) << "Signal Power:" << std::setw (8) << g_signalDbmAvg <<
  std::setw(14) << "Noise Power:" << std::setw (8) << g_noiseDbmAvg <<
  std::setw(8) << "SNR:" << (g_signalDbmAvg - g_noiseDbmAvg) <<
  std::endl;
  //std::cout << "BER:" << errorrate ;
  Simulator::Schedule (MilliSeconds (100), &PrintSNR);
}

NS_LOG_COMPONENT_DEFINE ("Yardimsiz");

/// Trace function for remaining energy at node.
void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy << "J");
}

/// Trace function for total energy consumption at node.
void
TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Total energy consumed by radio = " << totalEnergy << "J");
}


int main (int argc, char *argv[])
{

  uint32_t nSta = 1;
  uint32_t nAp = 1;
  string DataMode = "OfdmRate6Mbps" ; 
  double simulationTime = 10;
  bool enablePcap = false;
  std::ostringstream msg; msg << "Hello World!" << '\0';
  uint16_t packetSize = msg.str().length()+1;
  std::cout<<"PacketSize:"<<packetSize;
  CommandLine cmd;
  cmd.AddValue ("nSta", "Number of wifi STA devices", nSta);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);

  cmd.Parse (argc,argv);

  NodeContainer apNode;
  apNode.Create(nAp);

  NodeContainer staNodes;
  staNodes.Create(nSta);

  Ptr<Node> p = apNode.Get (0);
 

  YansWifiChannelHelper channel ; 
  channel.AddPropagationLoss ("ns3::FriisPropagationLossModel",
                                     "Frequency", DoubleValue (900.000e6));
  channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");


  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetErrorRateModel ("ns3::YansErrorRateModel");
  phy.SetChannel (channel.Create ());
  phy.Set ("TxPowerStart", DoubleValue (30)); // dBm (4 mW)
  phy.Set ("TxPowerEnd", DoubleValue (30));
  phy.Set ("RxGain", DoubleValue (30));
  phy.Set ("RxNoiseFigure", DoubleValue(57));

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


  mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
  "X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"),
  "Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"),
  "Z", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"));

  //mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel",
  "Bounds", BoxValue (Box (0, 1000, 0, 1000, 0, 1000)),
  "TimeStep", TimeValue (Seconds (0.1)),
  "Alpha", DoubleValue (0.85),
  "MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=1|Max=100]"),
  "MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=6.283185307]"),
  "MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
  "NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
  "NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
  "NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.02|Bound=0.04]"));
  mobility.Install (staNodes);


  MobilityHelper mobilityAp;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (500.0, 500.0, 0.0));
  mobilityAp.SetPositionAllocator (positionAlloc);
  mobilityAp.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityAp.Install(apNode);


/** Energy Model **/
  /***************************************************************************/
  /* energy source */
  BasicEnergySourceHelper basicSourceHelper;
  // configure energy source
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (50.1));
  // install source
  EnergySourceContainer sources = basicSourceHelper.Install (staNodes);
  /* device energy model */
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.174));
  // install device model
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (staDevices, sources);
  /***************************************************************************/

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


  /** connect trace sources **/
  /***************************************************************************/
  // all sources are connected to node 1
  // energy source
  Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (0));
  basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeCallback (&RemainingEnergy));
  // device energy model
  Ptr<DeviceEnergyModel> basicRadioModelPtr =
  basicSourcePtr->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  //NS_ASSERT (basicRadioModelPtr != NULL);
  basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
  /***************************************************************************/


  sinkApplications.Start (Seconds (0.0));
  sinkApplications.Stop (Seconds (simulationTime + 1));
  sourceApplications.Start (Seconds (1.0));
  sourceApplications.Stop (Seconds (simulationTime + 1));
  Simulator::Schedule (Seconds (1.1), &CalculateThroughput);
  Simulator::Schedule (Seconds (1.1), &PrintSNR);
  //Simulator::Schedule (Seconds (1.1), &GetDistance_From,nSta,staPointer,apPointer);

      Config::ConnectWithoutContext ("/NodeList/0/DeviceList/*/Phy/MonitorSnifferRx", MakeCallback (&MonitorSniffRx));
      Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&CourseChange));
     // Config::ConnectWithoutContext ("/NodeList/1/DeviceList/*/Phy/MonitorSnifferTx", MakeCallback (&MonitorSniffTx));
      Config::ConnectWithoutContext("/NodeList/0/ApplicationList/*/$ns3::PacketSink/Rx", MakeCallback(&Rx));

      if (enablePcap)
        {
          std::stringstream ss;
          ss << "yardimsiz-";
          phy.EnablePcap ("AccessPoint", apDevices);
          phy.EnablePcap ("Station", staDevices);
        }
      g_signalDbmAvg = 0;
      g_noiseDbmAvg = 0;
      g_samples = 0;


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (simulationTime + 1));
 // AnimationInterface anim("animationwifi.xml");
  Simulator::Run ();
  Simulator::Destroy ();


  double throughput = 0;
  for (unsigned index = 0; index < sinkApplications.GetN (); ++index)
    {
      uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();
      throughput += ((totalPacketsThrough * 8) / (simulationTime * 1000000.0)); //Mbit/s
      //uint64_t totalBytesSent = DynamicCast<OnOffApplication>(sourceApplications.Get(index)->

    }
  if (throughput > 0)
    {
      std::cout << "Aggregated throughput: " << throughput << " Mbit/s" << std::endl;
      std::cout << std::setw(8) << "Signal Power:" << std::setw (8) << g_signalDbmAvg <<
      std::setw(14) << "Noise Power:" << std::setw (8) << g_noiseDbmAvg <<
      std::setw(8) << "SNR:" << (g_signalDbmAvg - g_noiseDbmAvg) <<
      std::endl;
    }
  else
    {
      NS_LOG_ERROR ("Obtained throughput is 0!");
      exit (1);
    }
  return 0;
}



