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

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/command-line.h"
//#include "ns3/simple-device-energy-model.h"
//#include "ns3/energy-source-container.h"
#include "ns3/power-model.h"
#include "ns3/temperature-model.h"
#include "ns3/reliability-model.h"
#include "ns3/reliability-helper.h"

using namespace ns3;

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

  int i=0,num_nodes=3;

  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  ReliabilityHelper reliabilityHelper;
  //reliabilityHelper.EnableLogComponents (level);
  //LogComponentEnable ("PowerModel", LOG_LEVEL_ALL);
  //LogComponentEnable ("TemperatureModel", LOG_LEVEL_ALL);

  NodeContainer nodes;
  //Ptr<Node> nodeOne = CreateObject<Node> ();
  //Ptr<Node> nodeTwo = CreateObject<Node> ();
  nodes.Create (num_nodes);
  //nodes.Add (node);

  reliabilityHelper.Install(nodes);
/*
  Ptr<PowerModel> pm = CreateObject<PowerModel> ();
  Ptr<TemperatureModel> tm = CreateObject<TemperatureModel> ();
  Ptr<ReliabilityModel> rm = CreateObject<ReliabilityModel> ();

  pm->RegisterTemperatureModel (tm);
  pm->RegisterReliabilityModel (rm);
  tm->RegisterReliabilityModel (rm);
*/
  //esCont->Add (es);
  //es->SetNode (node);
  //sem->SetNode (node);
  //sem->SetEnergySource (es);
  //es->AppendDeviceEnergyModel (sem);

  //node->AggregateObject (esCont);

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("sixth.cwnd");

  Time now = Simulator::Now ();
  //nodes.Get (0)->GetObject<ReliabilityModel> ()->TraceConnectWithoutContext ("Reliability", MakeBoundCallback (&ReliabilityTracer, stream));
  //nodes.Get (0)->GetObject<TemperatureModel> ()->TraceConnectWithoutContext ("Temperature", MakeBoundCallback (&TemperatureTracer, stream));
  //nodes.Get (0)->GetObject<PowerModel> ()->TraceConnectWithoutContext ("Power", MakeBoundCallback (&PowerTracer, stream));

  PrintInfo (nodes);

  for(i=0;i<num_nodes;i++)
  Simulator::Schedule (now,&PowerModel::UpdatePower,nodes.Get (i)->GetObject<PowerModel> ());
  //Simulator::Schedule (now,&PowerModel::UpdatePower,nodes.Get (1)->GetObject<PowerModel> ());
  //Simulator::Schedule (now,&TemperatureModel::UpdateTemperature,node->GetObject<TemperatureModel> (),100);
  now += Seconds (64);

  Simulator::Stop (now);
  Simulator::Run ();
  Simulator::Destroy ();

}

