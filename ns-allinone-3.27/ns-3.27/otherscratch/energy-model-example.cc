/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
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
 * Author: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/energy-module.h"
#include "ns3/internet-module.h"
#include "ns3/power-model.h"
#include "ns3/temperature-model.h"
#include "ns3/reliability-model.h"
#include "ns3/reliability-helper.h"
#include "ns3/gnuplot.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace ns3;

  std::string fileNameWithNoExtension1 = "plot1";
  std::string fileNameWithNoExtension2 = "plot2";
  std::string fileNameWithNoExtension3 = "plot3";
  std::string fileNameWithNoExtension4 = "plot4";
  std::string graphicsFileName1        = fileNameWithNoExtension1 + ".png";
  std::string graphicsFileName2        = fileNameWithNoExtension2 + ".png";
  std::string graphicsFileName3        = fileNameWithNoExtension3 + ".png";
  std::string graphicsFileName4        = fileNameWithNoExtension4 + ".png";
  std::string plotFileName1            = fileNameWithNoExtension1 + ".plt";
  std::string plotFileName2            = fileNameWithNoExtension2 + ".plt";
  std::string plotFileName3            = fileNameWithNoExtension3 + ".plt";
  std::string plotFileName4            = fileNameWithNoExtension4 + ".plt";
  std::string plotTitle1               = "Energy vs Time";
  std::string plotTitle2               = "Power vs Time";
  std::string plotTitle3               = "Temperature vs Time";
  std::string plotTitle4               = "Reliability vs Time";
  std::string data1Title               = "Energy";
  std::string data2Title               = "Power";
  std::string data3Title               = "Temperature";
  std::string data4Title               = "Reliability";

  // Instantiate the plot and set its title.
  Gnuplot plot1 (graphicsFileName1);
  Gnuplot plot2 (graphicsFileName2);
  Gnuplot plot3 (graphicsFileName3);
  Gnuplot plot4 (graphicsFileName4);

  // Instantiate the dataset, set its title, and make the points be
  // plotted along with connecting lines.
  Gnuplot2dDataset dataset1;
  Gnuplot2dDataset dataset2;
  Gnuplot2dDataset dataset3;
  Gnuplot2dDataset dataset4;

NS_LOG_COMPONENT_DEFINE ("EnergyExample");

static inline std::string
PrintReceivedPacket (Address& from)
{
  InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);

  std::ostringstream oss;
  oss << "--\nReceived one packet! Socket: " << iaddr.GetIpv4 ()
      << " port: " << iaddr.GetPort ()
      << " at time = " << Simulator::Now ().GetSeconds ()
      << "\n--";

  return oss.str ();
}

/**
 * \param socket Pointer to socket.
 *
 * Packet receiving sink.
 */


void
ReliabilityTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Reliability = " << newValue << std::endl;
  //dataset2.Add(Simulator::Now ().GetSeconds (), newValue);
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
TemperatureTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Temperature = " << newValue << std::endl;
 // dataset3.Add(Simulator::Now ().GetSeconds (), newValue);
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
PowerTracer (Ptr<OutputStreamWrapper> stream, double oldValue, double newValue)
{
  std::cout << "Power = " << newValue << std::endl;
 // dataset4.Add(Simulator::Now ().GetSeconds (), newValue);
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << oldValue << "\t" << newValue;
}

void
PrintInfo (NodeContainer nodes)
{
  for(uint32_t i=0;i<1;i++)
  {
    std::cout<<"At time "<< Simulator::Now().GetSeconds()<<", NodeId = "<<nodes.Get(i)->GetId();
    std::cout << " Power = " << nodes.Get(i)->GetObject<PowerModel>()->GetPower();
    std::cout << " Temperature = " << nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature();
    std::cout << " Reliability = " << nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability()<<std::endl;

  //dataset1.Add(Simulator::Now ().GetSeconds (), newValue);
  dataset2.Add(Simulator::Now ().GetSeconds (), nodes.Get(i)->GetObject<PowerModel>()->GetPower());
  dataset3.Add(Simulator::Now ().GetSeconds (), nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature());
  dataset4.Add(Simulator::Now ().GetSeconds (), nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability());



  }
  if (!Simulator::IsFinished ())
  {
    Simulator::Schedule (Seconds (1),&PrintInfo,nodes);
  }
}


void
ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          NS_LOG_UNCOND (PrintReceivedPacket (from));
        }
    }
}

/**
 * \param socket Pointer to socket.
 * \param pktSize Packet size.
 * \param n Pointer to node.
 * \param pktCount Number of packets to generate.
 * \param pktInterval Packet sending interval.
 *
 * Traffic generator.
 */
static void
GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, Ptr<Node> n,
                 uint32_t pktCount, Time pktInterval)
{
  if (pktCount > 0)
    {
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, n,
                           pktCount - 1, pktInterval);
    }
  else
    {
      socket->Close ();
    }
}

/// Trace function for remaining energy at node.
void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy << "J");
  dataset1.Add(Simulator::Now ().GetSeconds (), remainingEnergy);
}

/// Trace function for total energy consumption at node.
void
TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Total energy consumed by radio = " << totalEnergy << "J");
}


void
Plotter(void)
{
  // Add the dataset to the plot.
  plot1.AddDataset (dataset1);
  plot2.AddDataset (dataset2);
  plot3.AddDataset (dataset3);
  plot4.AddDataset (dataset4);

  // Open the plot file.
  std::ofstream plotFile1 (plotFileName1.c_str());
  std::ofstream plotFile2 (plotFileName2.c_str());
  std::ofstream plotFile3 (plotFileName3.c_str());
  std::ofstream plotFile4 (plotFileName4.c_str());

  // Write the plot file.
  plot1.GenerateOutput (plotFile1);
  plot2.GenerateOutput (plotFile2);
  plot3.GenerateOutput (plotFile3);
  plot4.GenerateOutput (plotFile4);

  // Close the plot file.
  plotFile1.close ();
  plotFile2.close ();
  plotFile3.close ();
  plotFile4.close ();
}
int
main (int argc, char *argv[])
{
  /*
  LogComponentEnable ("EnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("BasicEnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("DeviceEnergyModel", LOG_LEVEL_DEBUG);
  LogComponentEnable ("WifiRadioEnergyModel", LOG_LEVEL_DEBUG);
   */


  plot1.SetTitle (plotTitle1);
  plot2.SetTitle (plotTitle2);
  plot3.SetTitle (plotTitle3);
  plot4.SetTitle (plotTitle4);

  // Make the graphics file, which the plot file will create when it
  // is used with Gnuplot, be a PNG file.
  plot1.SetTerminal ("png");
  plot2.SetTerminal ("png");
  plot3.SetTerminal ("png");
  plot4.SetTerminal ("png");

  // Set the labels for each axis.
  plot1.SetLegend ("time(s)", "Energy(J)");
  plot2.SetLegend ("time(s)", "Power(W)");
  plot3.SetLegend ("time(s)", "Temperature(C)");
  plot4.SetLegend ("time(s)", "Reliability");


  dataset1.SetTitle (data1Title);
  dataset1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  dataset2.SetTitle (data2Title);
  dataset2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  dataset3.SetTitle (data3Title);
  dataset3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  dataset4.SetTitle (data4Title);
  dataset4.SetStyle (Gnuplot2dDataset::LINES_POINTS);



  std::string phyMode ("DsssRate1Mbps");
  double Prss = -80;            // dBm
  uint32_t PpacketSize = 200;   // bytes
  bool verbose = false;

  // simulation parameters
  uint32_t numPackets = 10000;  // number of packets to send
  double interval = 1;          // seconds
  double startTime = 0.0;       // seconds
  double distanceToRx = 100.0;  // meters
  /*
   * This is a magic number used to set the transmit power, based on other
   * configuration.
   */
  double offset = 81;

  CommandLine cmd;
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("Prss", "Intended primary RSS (dBm)", Prss);
  cmd.AddValue ("PpacketSize", "size of application packet sent", PpacketSize);
  cmd.AddValue ("numPackets", "Total number of packets to send", numPackets);
  cmd.AddValue ("startTime", "Simulation start time", startTime);
  cmd.AddValue ("distanceToRx", "X-Axis distance between nodes", distanceToRx);
  cmd.AddValue ("verbose", "Turn on all device log components", verbose);
  cmd.Parse (argc, argv);

  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  NodeContainer c;
  c.Create (2);     // create 2 nodes
  NodeContainer networkNodes;
  networkNodes.Add (c.Get (0));
  networkNodes.Add (c.Get (1));

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();
    }
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  /** Wifi PHY **/
  /***************************************************************************/
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (-10));
  wifiPhy.Set ("TxGain", DoubleValue (offset + Prss));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0));
  /***************************************************************************/

  /** wifi channel **/
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  // create wifi channel
  Ptr<YansWifiChannel> wifiChannelPtr = wifiChannel.Create ();
  wifiPhy.SetChannel (wifiChannelPtr);

  /** MAC layer **/
  // Add a MAC and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode",
                                StringValue (phyMode), "ControlMode",
                                StringValue (phyMode));
  // Set it to ad-hoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");

  /** install PHY + MAC **/
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, networkNodes);

  /** mobility **/
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (2 * distanceToRx, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  /** Energy Model **/
  /***************************************************************************/
  /* energy source */
  BasicEnergySourceHelper basicSourceHelper;
  // configure energy source
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (100));
  // install source
  EnergySourceContainer sources = basicSourceHelper.Install (c);
  /* device energy model */
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0174));
  // install device model
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (devices, sources);
  /***************************************************************************/
  int say=0;


  //Install Reliability
  ReliabilityHelper reliabilityHelper;
  reliabilityHelper.Install(c);

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");

  /** Internet stack **/
  InternetStackHelper internet;
  internet.Install (networkNodes);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (networkNodes.Get (1), tid);  // node 1, receiver
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket (networkNodes.Get (0), tid);    // node 0, sender
  InetSocketAddress remote = InetSocketAddress (Ipv4Address::GetBroadcast (), 80);
  source->SetAllowBroadcast (true);
  source->Connect (remote);

  /** connect trace sources **/
  /***************************************************************************/
  // all sources are connected to node 1
  // energy source
  Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (1));
  basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeCallback (&RemainingEnergy));
  // device energy model
  Ptr<DeviceEnergyModel> basicRadioModelPtr =
    basicSourcePtr->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  NS_ASSERT (basicRadioModelPtr != NULL);
 // basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));
  /***************************************************************************/


  Time now = Simulator::Now ();

  PrintInfo (c);

  for(say=0;say<1;say++)
  Simulator::Schedule (now,&PowerModel::UpdatePower,c.Get (say)->GetObject<PowerModel> ());


  /** simulation setup **/
  // start traffic
  Simulator::Schedule (Seconds (startTime), &GenerateTraffic, source, PpacketSize,
                       networkNodes.Get (0), numPackets, interPacketInterval);
  Simulator::Schedule (Seconds(61), &Plotter);
  now += Seconds (60);
  
  Simulator::Stop (now +Seconds(2));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
