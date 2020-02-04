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

 // Network topology
 //
 // Packets sent to the device "thetap" on the Linux host will be sent to the
 // tap bridge on node zero and then emitted onto the ns-3 simulated CSMA
 // network.  ARP will be used on the CSMA network to resolve MAC addresses.
 // Packets destined for the CSMA device on node zero will be sent to the
 // device "thetap" on the linux Host.
 //
 //  +----------+
 //  | external |
 //  |  Linux   |
 //  |   Host   |
 //  |          |
 //  | "thetap" |
 //  +----------+
 //       |           n0            n1            n2            n3
 //       |       +--------+    +--------+    +--------+    +--------+
 //       +-------|  tap   |    |        |    |        |    |        |
 //               | bridge |    |        |    |        |    |        |
 //               +--------+    +--------+    +--------+    +--------+
 //               |  CSMA  |    |  CSMA  |    |  CSMA  |    |  CSMA  |
 //               +--------+    +--------+    +--------+    +--------+
 //                   |             |             |             |
 //                   |             |             |             |
 //                   |             |             |             |
 //                   ===========================================
 //                                 CSMA LAN 10.1.1
 //
 // The CSMA device on node zero is:  10.1.1.1
 // The CSMA device on node one is:   10.1.1.2
 // The CSMA device on node two is:   10.1.1.3
 // The CSMA device on node three is: 10.1.1.4
 //
 // Some simple things to do:
 //
 // 1) Ping one of the simulated nodes
 //
 //    ./waf --run tap-csma&
 //    ping 10.1.1.2
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

 using namespace ns3;

#define packetSize 20

 NS_LOG_COMPONENT_DEFINE ("HILTapCsmaExample");

 static inline std::string
 PrintReceivedPacket (Address& from, Ptr<Packet> packet)
 {
   InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);
   uint8_t buf[packetSize];
   packet->CopyData(buf, packetSize);

   std::ostringstream oss;
   oss << "--\nReceived one packet! From socket: " << iaddr.GetIpv4 ()
       << " port: " << iaddr.GetPort ()
       << " at time = " << Simulator::Now ().GetSeconds ()
       << "\n" << buf
       << "\n--";

   return oss.str ();
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
           NS_LOG_UNCOND (PrintReceivedPacket (from, packet));
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
 GenerateTraffic (Ptr<Socket> socket, Time pktInterval)
 {
   Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello"), 5);
   socket->Send (pkt);
   Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktInterval);
 }

 int
 main (int argc, char *argv[])
 {
   std::string mode = "UseBridge";
   std::string tapName = "tap0";
   double interval = 1;          // seconds
   double startTime = 0.0;       // seconds
   // Convert to time object
   Time interPacketInterval = Seconds (interval);

   CommandLine cmd;
   cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
   cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
   cmd.Parse (argc, argv);

   GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
   GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

   // explicitly create four nodes
   NodeContainer nodes;
   nodes.Create (4);

   // create csma channel on physical layer
   CsmaHelper csma;
   csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
   csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

   NetDeviceContainer devices = csma.Install (nodes);

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
   Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get (revNode), tid);
   InetSocketAddress local = InetSocketAddress (Ipv4Address (revIP), revPort);
   recvSink->Bind (local);
   recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

   // add a UDP sender on node 2 (10.1.1.3) toward piNode (10.1.1.5)
   // Run udp-server.py on Pi to see the sent message
   uint8_t sendNode = 2;
   char piAddr[] = "10.1.1.5";
   uint16_t piPort = 4000;
   Ptr<Socket> source = Socket::CreateSocket (nodes.Get (sendNode), tid);
   InetSocketAddress remote = InetSocketAddress (Ipv4Address (piAddr), piPort);
   // Note: in our test, broadcast on this channel will not send to the external Pi
   // InetSocketAddress remote = InetSocketAddress (Ipv4Address::GetBroadcast (), piPort);
   source->SetAllowBroadcast (true);
   source->Connect (remote);

   // enable routing tables
   csma.EnablePcapAll ("tap-csma", false);
   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

   // set tap bridge to connect to external pi
   TapBridgeHelper tapBridge;
   tapBridge.SetAttribute ("Mode", StringValue (mode));
   tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
   tapBridge.Install (nodes.Get (0), devices.Get (0));

   Simulator::Schedule (Seconds (startTime), &GenerateTraffic, source, interPacketInterval);
   Simulator::Stop (Seconds (30.));
   Simulator::Run ();
   Simulator::Destroy ();
 }
