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
 */

//
// This is an illustration of how one could use virtualization techniques to
// allow running applications on virtual machines talking over simulated
// networks.
//
// The actual steps required to configure the virtual machines can be rather
// involved, so we don't go into that here.  Please have a look at one of
// our HOWTOs on the nsnam wiki for more details about how to get the
// system confgured.  For an example, have a look at "HOWTO Use Linux
// Containers to set up virtual networks" which uses this code as an
// example.
//
// The configuration you are after is explained in great detail in the
// HOWTO, but looks like the following:
//
//  +----------+
//  | virtual  |
//  |  Linux   |
//  |   Host   |
//  |          |
//  |   eth0   |
//  +----------+
//       |
//  +----------+
//  |  Linux   |
//  |  Bridge  |
//  +----------+
//       |
//  +----------+
//  | "tap0"   |
//  +----------+
//       |           n0            n1            n2            n3
//       |       +--------+    +--------+    +--------+    +--------+
//       +-------|  tap   |    |        |    |        |    |        |
//               | bridge |    |        |    |        |    |        |
//               +--------+    +--------+    +--------+    +--------+
//               |  wifi  |    |  wifi  |    |  wifi  |    |  wifi  |
//               +--------+    +--------+    +--------+    +--------+
//                   |             |             |             |
//                 ((*))         ((*))         ((*))         ((*))
//
//                       Wifi LAN
//
//                        ((*))
//                          |
//                     +--------+
//                     |  wifi  |
//                     +--------+
//                     | access |
//                     |  point |
//                     +--------+
//
 #include <iostream>
 #include <fstream>

 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/wifi-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/ipv4-global-routing-helper.h"
 #include "ns3/tap-bridge-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/internet-apps-module.h"
 #include "ns3/mobility-module.h"
 using namespace ns3;

 #define packetSize 30
 #define ROUNDS 10
 #define CNTS 5
 Time st_time, delay_time; // global variables to track delay
 double delayTime, execTime, commTime; // global variable to track delay in seconds
 Ptr<Socket> source, dest; // source node in ns-3 and remote node of Pi
 Time interPacketInterval; // interval between end of receiving and next transmitting
 uint32_t datasize[CNTS] = {10000, 20000, 30000, 40000, 50000};
 uint8_t dSize_idx = 0, cnt = 0;

 NS_LOG_COMPONENT_DEFINE ("HILTapWifiExample");


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
 GenerateTraffic (Ptr<Socket> socket, Time pktInterval)
 {
   char dSize[packetSize];
   sprintf(dSize, "%d", datasize[dSize_idx]);

   //
   // This part for tranmitting all data
   // char msg[packetSize] = "data,0,";
   // strcat(msg, dSize);

   //
   // This part for local computation
   char msg[packetSize] = "data,";
   strcat(msg, dSize);
   strcat(msg, ",10");

   uint8_t size = strlen(msg);
   Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (msg), size);
   st_time = Simulator::Now();
   socket->Send (pkt);
   // Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktInterval);
 }

 static inline std::string
 PrintReceivedPacket (Address& from)
 {
   InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);
   std::ostringstream oss;
   oss << "--\nReceived one packet! From socket: " << iaddr.GetIpv4 ()
       << " port: " << iaddr.GetPort ()
       << " at time = " << Simulator::Now ().GetSeconds ()
       << "\n--";

   return oss.str ();
 }

 static inline void
 LogtoFile ()
 {
   std::ofstream log_file("log.txt", std::ios_base::app | std::ios_base::out);
   log_file << datasize[dSize_idx] << "," << execTime << ","
            << delayTime << "," << commTime << "\n";
   std::cout << "Data Size: " << datasize[dSize_idx] << " "
             << "Exec. Time: " << execTime << " "
             << "Total Time: " << delayTime << " "
             << "Comm. Time: " << commTime << std::endl;
 }

 /**
  * \param socket Pointer to socket.
  *
  * Packet receiving sink.
  */
 void
 ReceivePacket (Ptr<Socket> socket)
 {
   Ptr<Packet> packet;
   Address from;
   while ((packet = socket->RecvFrom (from)))
   {
     if (packet->GetSize () > 0)
     {
       // process the header of the message
       char buf[packetSize];
       packet->CopyData( (uint8_t *) buf, packetSize);
       char * pch = strchr (buf, ',');
       * pch = '\0';
       * (pch + 14) = '\0';

       // check the content of the message
       if (strcmp (buf, "data") == 0) {
         // calculate the total delay in ns-3
         delay_time = Simulator::Now() - st_time;
         delayTime = delay_time.ToDouble (Time::S);
         // extract the execution delay on Pi
         execTime = strtod (pch + 1, NULL);
         commTime = delayTime - execTime;

         LogtoFile();
         NS_LOG_INFO (PrintReceivedPacket (from));

         // update index
         cnt++;
         if (cnt >= ROUNDS) {
           dSize_idx++;
           cnt = 0;
         }
         if (dSize_idx >= CNTS)
           Simulator::Stop(); // terminate
         else
           Simulator::Schedule (interPacketInterval, &GenerateTraffic, source,
             interPacketInterval);
       }
     }
   }
 }


 int
 main (int argc, char *argv[])
 {
   // Wireless STA nodes do not support SendFrom()
   // Can only work in UseLocal mode
   std::string mode = "UseLocal";
   std::string tapName = "tap0";
   double interval = 1;          // seconds
   double startTime = 0.0;       // seconds
   interPacketInterval = Seconds (interval); // convert to Time object

   CommandLine cmd;
   cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
   cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
   cmd.Parse (argc, argv);

   GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
   GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

   // explicitly create four nodes
   NodeContainer nodes;
   nodes.Create (4);

   //
   // We're going to use 802.11 A so set up a wifi helper to reflect that.
   //
   WifiHelper wifi;
   wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
   wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate54Mbps"));

   //
   // No reason for pesky access points, so we'll use an ad-hoc network.
   //
   WifiMacHelper wifiMac;
   wifiMac.SetType ("ns3::AdhocWifiMac");

   //
   // Configure the physcial layer.
   //
   YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
   YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
   wifiPhy.SetChannel (wifiChannel.Create ());
   wifiPhy.Set ("ChannelNumber", UintegerValue (5));

   //
   // Install the wireless devices onto our ghost nodes.
   //
   NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

   //
   // We need location information since we are talking about wifi, so add a
   // constant position to the ghost nodes.
   //
   MobilityHelper mobility;
   Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
   positionAlloc->Add (Vector (0.0, 0.0, 0.0));
   positionAlloc->Add (Vector (10.0, 0.0, 0.0));
   positionAlloc->Add (Vector (20.0, 0.0, 0.0));
   positionAlloc->Add (Vector (0.0, 20.0, 0.0));
   mobility.SetPositionAllocator (positionAlloc);
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (nodes);

   // add an ip stack to all nodes
   InternetStackHelper stack;
   stack.Install (nodes);

   // assign ip addresses
   Ipv4AddressHelper ipv4;
   ipv4.SetBase ("10.1.1.0", "255.255.255.0");
   Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

   // add a UDP receiver to node 3 (10.1.1.4)
   // Run udp-client.py on Pi to get the receiving log
   uint8_t revNode = 3;
   char revIP[] = "10.1.1.4";
   uint16_t revPort = 4000;
   TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
   dest = Socket::CreateSocket (nodes.Get (revNode), tid);
   InetSocketAddress local = InetSocketAddress (Ipv4Address (revIP), revPort);
   dest->Bind (local);
   dest->SetRecvCallback (MakeCallback (&ReceivePacket));

   // add a UDP sender on node 3 (10.1.1.4) toward piNode (10.1.1.5)
   // Run udp-server.py on Pi to see the sent message
   uint8_t sendNode = 3;
   char piAddr[] = "10.1.1.5";
   uint16_t piPort = 4000;
   source = Socket::CreateSocket (nodes.Get (sendNode), tid);
   InetSocketAddress remote = InetSocketAddress (Ipv4Address (piAddr), piPort);
   // Note: in our test, broadcast on this channel will not send to the external Pi
   // InetSocketAddress remote = InetSocketAddress (Ipv4Address::GetBroadcast (), piPort);
   source->SetAllowBroadcast (true);
   source->Connect (remote);

   // enable routing tables
   // csma.EnablePcapAll ("tap-csma", false);
   // Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

   // set tap bridge to connect to external pi
   TapBridgeHelper tapBridge;
   tapBridge.SetAttribute ("Mode", StringValue (mode));
   tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
   tapBridge.Install (nodes.Get (0), devices.Get (0));

   Simulator::Schedule (Seconds (startTime), &GenerateTraffic, source, interPacketInterval);
   // Simulator::Stop (Seconds (300.));
   Simulator::Run ();
   Simulator::Destroy ();
 }
