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
 */
#include "ns3/gnuplot.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/energy-module.h"
#include "ns3/internet-module.h"
#include "ns3/reliability-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/olsr-helper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("Experiment");

/** RPi 0 **/
/***************************************************************************/
std::string rpi0FileNameWithNoExtension1avg = "rpi0_avg_energy_plot";
std::string rpi0FileNameWithNoExtension1all = "rpi0_all_energy_plot";
std::string rpi0FileNameWithNoExtension2avg = "rpi0_avg_power_plot";
std::string rpi0FileNameWithNoExtension2all = "rpi0_all_power_plot";
std::string rpi0FileNameWithNoExtension3avg = "rpi0_avg_temperature_plot";
std::string rpi0FileNameWithNoExtension3all = "rpi0_all_temperature_plot";
std::string rpi0FileNameWithNoExtension4avg = "rpi0_avg_reliability_plot";
std::string rpi0FileNameWithNoExtension4all = "rpi0_all_reliability_plot";

std::string rpi0GraphicsFileName1avg        = rpi0FileNameWithNoExtension1avg + ".png";
std::string rpi0GraphicsFileName1all        = rpi0FileNameWithNoExtension1all + ".png";
std::string rpi0GraphicsFileName2avg        = rpi0FileNameWithNoExtension2avg + ".png";
std::string rpi0GraphicsFileName2all        = rpi0FileNameWithNoExtension2all + ".png";
std::string rpi0GraphicsFileName3avg        = rpi0FileNameWithNoExtension3avg + ".png";
std::string rpi0GraphicsFileName3all        = rpi0FileNameWithNoExtension3all + ".png";
std::string rpi0GraphicsFileName4avg        = rpi0FileNameWithNoExtension4avg + ".png";
std::string rpi0GraphicsFileName4all        = rpi0FileNameWithNoExtension4all + ".png";

std::string rpi0PlotFileName1avg            = rpi0FileNameWithNoExtension1avg + ".plt";
std::string rpi0PlotFileName1all            = rpi0FileNameWithNoExtension1all + ".plt";
std::string rpi0PlotFileName2avg            = rpi0FileNameWithNoExtension2avg + ".plt";
std::string rpi0PlotFileName2all            = rpi0FileNameWithNoExtension2all + ".plt";
std::string rpi0PlotFileName3avg            = rpi0FileNameWithNoExtension3avg + ".plt";
std::string rpi0PlotFileName3all            = rpi0FileNameWithNoExtension3all + ".plt";
std::string rpi0PlotFileName4avg            = rpi0FileNameWithNoExtension4avg + ".plt";
std::string rpi0PlotFileName4all            = rpi0FileNameWithNoExtension4all + ".plt";

std::string rpi0PlotTitle1avg               = "RPi0 Average Energy Consumption vs Time";
std::string rpi0PlotTitle1all               = "RPi0 Energy Consumption vs Time";
std::string rpi0PlotTitle2avg               = "RPi0 Average Power vs Time";
std::string rpi0PlotTitle2all               = "RPi0 Power vs Time";
std::string rpi0PlotTitle3avg               = "RPi0 Average Temperature vs Time";
std::string rpi0PlotTitle3all               = "RPi0 Temperature vs Time";
std::string rpi0PlotTitle4avg               = "RPi0 Average Reliability vs Time";
std::string rpi0PlotTitle4all               = "RPi0 Reliability vs Time";

std::string rpi0Data1Titleavg               = "Average RPi0 Energy";
std::string rpi0Data1Titleall1               = "RPi0 1";
std::string rpi0Data1Titleall2               = "RPi0 2";
std::string rpi0Data1Titleall3               = "RPi0 3";
std::string rpi0Data1Titleall4               = "RPi0 4";
std::string rpi0Data1Titleall5               = "Average";

std::string rpi0Data2Titleavg               = "Average RPi Power";
std::string rpi0Data2Titleall1               = "RPi0 1";
std::string rpi0Data2Titleall2               = "RPi0 2";
std::string rpi0Data2Titleall3               = "RPi0 3";
std::string rpi0Data2Titleall4               = "RPi0 4";
std::string rpi0Data2Titleall5               = "Average";

std::string rpi0Data3Titleavg               = "Average RPi Temperature";
std::string rpi0Data3Titleall1               = "RPi0 1";
std::string rpi0Data3Titleall2               = "RPi0 2";
std::string rpi0Data3Titleall3               = "RPi0 3";
std::string rpi0Data3Titleall4               = "RPi0 4";
std::string rpi0Data3Titleall5               = "Average";

std::string rpi0Data4Titleavg               = "Average RPi Reliability";
std::string rpi0Data4Titleall1               = "RPi0 1";
std::string rpi0Data4Titleall2               = "RPi0 2";
std::string rpi0Data4Titleall3               = "RPi0 3";
std::string rpi0Data4Titleall4               = "RPi0 4";
std::string rpi0Data4Titleall5               = "Average";

// Instantiate the plot and set its title.
Gnuplot rpi0Plot1avg (rpi0GraphicsFileName1avg);
Gnuplot rpi0Plot1all (rpi0GraphicsFileName1all);
Gnuplot rpi0Plot2avg (rpi0GraphicsFileName2avg);
Gnuplot rpi0Plot2all (rpi0GraphicsFileName2all);
Gnuplot rpi0Plot3avg (rpi0GraphicsFileName3avg);
Gnuplot rpi0Plot3all (rpi0GraphicsFileName3all);
Gnuplot rpi0Plot4avg (rpi0GraphicsFileName4avg);
Gnuplot rpi0Plot4all (rpi0GraphicsFileName4all);
// Instantiate the dataset, set its title, and make the points be plotted along with connecting lines.
Gnuplot2dDataset rpi0Dataset1avg;
Gnuplot2dDataset rpi0Dataset1all1;
Gnuplot2dDataset rpi0Dataset1all2;
Gnuplot2dDataset rpi0Dataset1all3;
Gnuplot2dDataset rpi0Dataset1all4;
Gnuplot2dDataset rpi0Dataset1all5;

Gnuplot2dDataset rpi0Dataset2avg;
Gnuplot2dDataset rpi0Dataset2all1;
Gnuplot2dDataset rpi0Dataset2all2;
Gnuplot2dDataset rpi0Dataset2all3;
Gnuplot2dDataset rpi0Dataset2all4;
Gnuplot2dDataset rpi0Dataset2all5;

Gnuplot2dDataset rpi0Dataset3avg;
Gnuplot2dDataset rpi0Dataset3all1;
Gnuplot2dDataset rpi0Dataset3all2;
Gnuplot2dDataset rpi0Dataset3all3;
Gnuplot2dDataset rpi0Dataset3all4;
Gnuplot2dDataset rpi0Dataset3all5;

Gnuplot2dDataset rpi0Dataset4avg;
Gnuplot2dDataset rpi0Dataset4all1;
Gnuplot2dDataset rpi0Dataset4all2;
Gnuplot2dDataset rpi0Dataset4all3;
Gnuplot2dDataset rpi0Dataset4all4;
Gnuplot2dDataset rpi0Dataset4all5;
/***************************************************************************/

/** RPi 3 **/
/***************************************************************************/
std::string rpi3FileNameWithNoExtension1avg = "rpi3_avg_energy_plot";
std::string rpi3FileNameWithNoExtension1all = "rpi3_all_energy_plot";
std::string rpi3FileNameWithNoExtension2avg = "rpi3_avg_power_plot";
std::string rpi3FileNameWithNoExtension2all = "rpi3_all_power_plot";
std::string rpi3FileNameWithNoExtension3avg = "rpi3_avg_temperature_plot";
std::string rpi3FileNameWithNoExtension3all = "rpi3_all_temperature_plot";
std::string rpi3FileNameWithNoExtension4avg = "rpi3_avg_reliability_plot";
std::string rpi3FileNameWithNoExtension4all = "rpi3_all_reliability_plot";

std::string rpi3GraphicsFileName1avg        = rpi3FileNameWithNoExtension1avg + ".png";
std::string rpi3GraphicsFileName1all        = rpi3FileNameWithNoExtension1all + ".png";
std::string rpi3GraphicsFileName2avg        = rpi3FileNameWithNoExtension2avg + ".png";
std::string rpi3GraphicsFileName2all        = rpi3FileNameWithNoExtension2all + ".png";
std::string rpi3GraphicsFileName3avg        = rpi3FileNameWithNoExtension3avg + ".png";
std::string rpi3GraphicsFileName3all        = rpi3FileNameWithNoExtension3all + ".png";
std::string rpi3GraphicsFileName4avg        = rpi3FileNameWithNoExtension4avg + ".png";
std::string rpi3GraphicsFileName4all        = rpi3FileNameWithNoExtension4all + ".png";

std::string rpi3PlotFileName1avg            = rpi3FileNameWithNoExtension1avg + ".plt";
std::string rpi3PlotFileName1all            = rpi3FileNameWithNoExtension1all + ".plt";
std::string rpi3PlotFileName2avg            = rpi3FileNameWithNoExtension2avg + ".plt";
std::string rpi3PlotFileName2all            = rpi3FileNameWithNoExtension2all + ".plt";
std::string rpi3PlotFileName3avg            = rpi3FileNameWithNoExtension3avg + ".plt";
std::string rpi3PlotFileName3all            = rpi3FileNameWithNoExtension3all + ".plt";
std::string rpi3PlotFileName4avg            = rpi3FileNameWithNoExtension4avg + ".plt";
std::string rpi3PlotFileName4all            = rpi3FileNameWithNoExtension4all + ".plt";

std::string rpi3PlotTitle1avg               = "RPi3 Average Energy Consumption vs Time";
std::string rpi3PlotTitle1all               = "RPi3 Energy Consumption vs Time";
std::string rpi3PlotTitle2avg               = "RPi3 Average Power vs Time";
std::string rpi3PlotTitle2all               = "RPi3 Power vs Time";
std::string rpi3PlotTitle3avg               = "RPi3 Average Temperature vs Time";
std::string rpi3PlotTitle3all               = "RPi3 Temperature vs Time";
std::string rpi3PlotTitle4avg               = "RPi3 Average Reliability vs Time";
std::string rpi3PlotTitle4all               = "RPi3 Reliability vs Time";

std::string rpi3Data1Titleavg               = "Average RPi Energy";
std::string rpi3Data1Titleall1               = "RPi3 1";
std::string rpi3Data1Titleall2               = "RPi3 2";
std::string rpi3Data1Titleall5               = "Average";

std::string rpi3Data2Titleavg               = "Average RPi Power";
std::string rpi3Data2Titleall1               = "RPi3 1";
std::string rpi3Data2Titleall2               = "RPi3 2";
std::string rpi3Data2Titleall5               = "Average";

std::string rpi3Data3Titleavg               = "Average RPi Temperature";
std::string rpi3Data3Titleall1               = "RPi3 1";
std::string rpi3Data3Titleall2               = "RPi3 2";
std::string rpi3Data3Titleall5               = "Average";

std::string rpi3Data4Titleavg               = "Average RPi Reliability";
std::string rpi3Data4Titleall1               = "RPi3 1";
std::string rpi3Data4Titleall2               = "RPi3 2";
std::string rpi3Data4Titleall5               = "Average";

// Instantiate the plot and set its title.
Gnuplot rpi3Plot1avg (rpi3GraphicsFileName1avg);
Gnuplot rpi3Plot1all (rpi3GraphicsFileName1all);
Gnuplot rpi3Plot2avg (rpi3GraphicsFileName2avg);
Gnuplot rpi3Plot2all (rpi3GraphicsFileName2all);
Gnuplot rpi3Plot3avg (rpi3GraphicsFileName3avg);
Gnuplot rpi3Plot3all (rpi3GraphicsFileName3all);
Gnuplot rpi3Plot4avg (rpi3GraphicsFileName4avg);
Gnuplot rpi3Plot4all (rpi3GraphicsFileName4all);
// Instantiate the dataset, set its title, and make the points be plotted along with connecting lines.
Gnuplot2dDataset rpi3Dataset1avg;
Gnuplot2dDataset rpi3Dataset1all1;
Gnuplot2dDataset rpi3Dataset1all2;
Gnuplot2dDataset rpi3Dataset1all5;

Gnuplot2dDataset rpi3Dataset2avg;
Gnuplot2dDataset rpi3Dataset2all1;
Gnuplot2dDataset rpi3Dataset2all2;
Gnuplot2dDataset rpi3Dataset2all5;

Gnuplot2dDataset rpi3Dataset3avg;
Gnuplot2dDataset rpi3Dataset3all1;
Gnuplot2dDataset rpi3Dataset3all2;
Gnuplot2dDataset rpi3Dataset3all5;

Gnuplot2dDataset rpi3Dataset4avg;
Gnuplot2dDataset rpi3Dataset4all1;
Gnuplot2dDataset rpi3Dataset4all2;
Gnuplot2dDataset rpi3Dataset4all5;
/***************************************************************************/

/** ESP **/
/***************************************************************************/
std::string espFileNameWithNoExtension1avg = "esp_avg_energy_plot";
std::string espFileNameWithNoExtension1all = "esp_all_energy_plot";
std::string espFileNameWithNoExtension2avg = "esp_avg_power_plot";
std::string espFileNameWithNoExtension2all = "esp_all_power_plot";
std::string espFileNameWithNoExtension3avg = "esp_avg_temperature_plot";
std::string espFileNameWithNoExtension3all = "esp_all_temperature_plot";
std::string espFileNameWithNoExtension4avg = "esp_avg_reliability_plot";
std::string espFileNameWithNoExtension4all = "esp_all_reliability_plot";

std::string espGraphicsFileName1avg        = espFileNameWithNoExtension1avg + ".png";
std::string espGraphicsFileName1all        = espFileNameWithNoExtension1all + ".png";
std::string espGraphicsFileName2avg        = espFileNameWithNoExtension2avg + ".png";
std::string espGraphicsFileName2all        = espFileNameWithNoExtension2all + ".png";
std::string espGraphicsFileName3avg        = espFileNameWithNoExtension3avg + ".png";
std::string espGraphicsFileName3all        = espFileNameWithNoExtension3all + ".png";
std::string espGraphicsFileName4avg        = espFileNameWithNoExtension4avg + ".png";
std::string espGraphicsFileName4all        = espFileNameWithNoExtension4all + ".png";

std::string espPlotFileName1avg            = espFileNameWithNoExtension1avg + ".plt";
std::string espPlotFileName1all            = espFileNameWithNoExtension1all + ".plt";
std::string espPlotFileName2avg            = espFileNameWithNoExtension2avg + ".plt";
std::string espPlotFileName2all            = espFileNameWithNoExtension2all + ".plt";
std::string espPlotFileName3avg            = espFileNameWithNoExtension3avg + ".plt";
std::string espPlotFileName3all            = espFileNameWithNoExtension3all + ".plt";
std::string espPlotFileName4avg            = espFileNameWithNoExtension4avg + ".plt";
std::string espPlotFileName4all            = espFileNameWithNoExtension4all + ".plt";

std::string espPlotTitle1avg               = "ESP Average Energy Consumption vs Time";
std::string espPlotTitle1all               = "ESP Energy Consumption vs Time";
std::string espPlotTitle2avg               = "ESP Average Power vs Time";
std::string espPlotTitle2all               = "ESP Power vs Time";
std::string espPlotTitle3avg               = "ESP Average Temperature vs Time";
std::string espPlotTitle3all               = "ESP Temperature vs Time";
std::string espPlotTitle4avg               = "ESP Average Reliability vs Time";
std::string espPlotTitle4all               = "ESP Reliability vs Time";

std::string espData1Titleavg               = "Average ESP Energy";
std::string espData1Titleall1               = "ESP 1";
std::string espData1Titleall2               = "ESP 2";
std::string espData1Titleall3               = "ESP 3";
std::string espData1Titleall4               = "ESP 4";
std::string espData1Titleall5               = "Average";

std::string espData2Titleavg               = "Average ESP Power";
std::string espData2Titleall1               = "ESP 1";
std::string espData2Titleall2               = "ESP 2";
std::string espData2Titleall3               = "ESP 3";
std::string espData2Titleall4               = "ESP 4";
std::string espData2Titleall5               = "Average";

std::string espData3Titleavg               = "Average ESP Temperature";
std::string espData3Titleall1               = "ESP 1";
std::string espData3Titleall2               = "ESP 2";
std::string espData3Titleall3               = "ESP 3";
std::string espData3Titleall4               = "ESP 4";
std::string espData3Titleall5               = "Average";

std::string espData4Titleavg               = "Average ESP Reliability";
std::string espData4Titleall1               = "ESP 1";
std::string espData4Titleall2               = "ESP 2";
std::string espData4Titleall3               = "ESP 3";
std::string espData4Titleall4               = "ESP 4";
std::string espData4Titleall5               = "Average";

// Instantiate the plot and set its title.
Gnuplot espPlot1avg (espGraphicsFileName1avg);
Gnuplot espPlot1all (espGraphicsFileName1all);
Gnuplot espPlot2avg (espGraphicsFileName2avg);
Gnuplot espPlot2all (espGraphicsFileName2all);
Gnuplot espPlot3avg (espGraphicsFileName3avg);
Gnuplot espPlot3all (espGraphicsFileName3all);
Gnuplot espPlot4avg (espGraphicsFileName4avg);
Gnuplot espPlot4all (espGraphicsFileName4all);
// Instantiate the dataset, set its title, and make the points be plotted along with connecting lines.
Gnuplot2dDataset espDataset1avg;
Gnuplot2dDataset espDataset1all1;
Gnuplot2dDataset espDataset1all2;
Gnuplot2dDataset espDataset1all3;
Gnuplot2dDataset espDataset1all4;
Gnuplot2dDataset espDataset1all5;

Gnuplot2dDataset espDataset2avg;
Gnuplot2dDataset espDataset2all1;
Gnuplot2dDataset espDataset2all2;
Gnuplot2dDataset espDataset2all3;
Gnuplot2dDataset espDataset2all4;
Gnuplot2dDataset espDataset2all5;

Gnuplot2dDataset espDataset3avg;
Gnuplot2dDataset espDataset3all1;
Gnuplot2dDataset espDataset3all2;
Gnuplot2dDataset espDataset3all3;
Gnuplot2dDataset espDataset3all4;
Gnuplot2dDataset espDataset3all5;

Gnuplot2dDataset espDataset4avg;
Gnuplot2dDataset espDataset4all1;
Gnuplot2dDataset espDataset4all2;
Gnuplot2dDataset espDataset4all3;
Gnuplot2dDataset espDataset4all4;
Gnuplot2dDataset espDataset4all5;
/***************************************************************************/


Ptr<PacketSink> sink;                         /* Pointer to the packet sink application */
uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */
double totalPower = 0;
double totalTemperature = 0;
double totalReliability = 0;
double averagePower = 0;
double averageTemperature = 0;
double averageReliability = 0;
double avgcount = 0;
double rpi0cpuenergy1 = 0;
double rpi0cpuenergy2 = 0;
double rpi0cpuenergy3 = 0;
double rpi0cpuenergy4 = 0;
double rpi3cpuenergy1 = 0;
double rpi3cpuenergy2 = 0;
double espcpuenergy1 = 0;
double espcpuenergy2 = 0;
double espcpuenergy3 = 0;
double espcpuenergy4 = 0;
double rpi0wifienergy1 = 0;
double rpi0wifienergy2 = 0;
double rpi0wifienergy3 = 0;
double rpi0wifienergy4 = 0;
double rpi3wifienergy1 = 0;
double rpi3wifienergy2 = 0;
double espwifienergy1 = 0;
double espwifienergy2 = 0;
double espwifienergy3 = 0;
double espwifienergy4 = 0;

void
rpi0Plotter(void)
{
  // Add the dataset to the plot.
  rpi0Plot1avg.AddDataset (rpi0Dataset1avg);
  rpi0Plot1all.AddDataset (rpi0Dataset1all1);
  rpi0Plot1all.AddDataset (rpi0Dataset1all2);
  rpi0Plot1all.AddDataset (rpi0Dataset1all3);
  rpi0Plot1all.AddDataset (rpi0Dataset1all4);
  rpi0Plot1all.AddDataset (rpi0Dataset1all5);

  rpi0Plot2avg.AddDataset (rpi0Dataset2avg);
  rpi0Plot2all.AddDataset (rpi0Dataset2all1);
  rpi0Plot2all.AddDataset (rpi0Dataset2all2);
  rpi0Plot2all.AddDataset (rpi0Dataset2all3);
  rpi0Plot2all.AddDataset (rpi0Dataset2all4);
  rpi0Plot2all.AddDataset (rpi0Dataset2all5);

  rpi0Plot3avg.AddDataset (rpi0Dataset3avg);
  rpi0Plot3all.AddDataset (rpi0Dataset3all1);
  rpi0Plot3all.AddDataset (rpi0Dataset3all2);
  rpi0Plot3all.AddDataset (rpi0Dataset3all3);
  rpi0Plot3all.AddDataset (rpi0Dataset3all4);
  rpi0Plot3all.AddDataset (rpi0Dataset3all5);

  rpi0Plot4avg.AddDataset (rpi0Dataset4avg);
  rpi0Plot4all.AddDataset (rpi0Dataset4all1);
  rpi0Plot4all.AddDataset (rpi0Dataset4all2);
  rpi0Plot4all.AddDataset (rpi0Dataset4all3);
  rpi0Plot4all.AddDataset (rpi0Dataset4all4);
  rpi0Plot4all.AddDataset (rpi0Dataset4all5);

  // Open the plot file.
  std::ofstream rpi0PlotFile1avg (rpi0PlotFileName1avg.c_str());
  std::ofstream rpi0PlotFile1all (rpi0PlotFileName1all.c_str());
  std::ofstream rpi0PlotFile2avg (rpi0PlotFileName2avg.c_str());
  std::ofstream rpi0PlotFile2all (rpi0PlotFileName2all.c_str());
  std::ofstream rpi0PlotFile3avg (rpi0PlotFileName3avg.c_str());
  std::ofstream rpi0PlotFile3all (rpi0PlotFileName3all.c_str());
  std::ofstream rpi0PlotFile4avg (rpi0PlotFileName4avg.c_str());
  std::ofstream rpi0PlotFile4all (rpi0PlotFileName4all.c_str());

  // Write the plot file.
  rpi0Plot1avg.GenerateOutput (rpi0PlotFile1avg);
  rpi0Plot1all.GenerateOutput (rpi0PlotFile1all);
  rpi0Plot2avg.GenerateOutput (rpi0PlotFile2avg);
  rpi0Plot2all.GenerateOutput (rpi0PlotFile2all);
  rpi0Plot3avg.GenerateOutput (rpi0PlotFile3avg);
  rpi0Plot3all.GenerateOutput (rpi0PlotFile3all);
  rpi0Plot4avg.GenerateOutput (rpi0PlotFile4avg);
  rpi0Plot4all.GenerateOutput (rpi0PlotFile4all);

  // Close the plot file.
  rpi0PlotFile1avg.close ();
  rpi0PlotFile1all.close ();
  rpi0PlotFile2avg.close ();
  rpi0PlotFile2all.close ();
  rpi0PlotFile3avg.close ();
  rpi0PlotFile3all.close ();
  rpi0PlotFile4avg.close ();
  rpi0PlotFile4all.close ();
}

void
rpi3Plotter(void)
{
  // Add the dataset to the plot.
  rpi3Plot1avg.AddDataset (rpi3Dataset1avg);
  rpi3Plot1all.AddDataset (rpi3Dataset1all1);
  rpi3Plot1all.AddDataset (rpi3Dataset1all2);
  rpi3Plot1all.AddDataset (rpi3Dataset1all5);

  rpi3Plot2avg.AddDataset (rpi3Dataset2avg);
  rpi3Plot2all.AddDataset (rpi3Dataset2all1);
  rpi3Plot2all.AddDataset (rpi3Dataset2all2);
  rpi3Plot2all.AddDataset (rpi3Dataset2all5);

  rpi3Plot3avg.AddDataset (rpi3Dataset3avg);
  rpi3Plot3all.AddDataset (rpi3Dataset3all1);
  rpi3Plot3all.AddDataset (rpi3Dataset3all2);
  rpi3Plot3all.AddDataset (rpi3Dataset3all5);

  rpi3Plot4avg.AddDataset (rpi3Dataset4avg);
  rpi3Plot4all.AddDataset (rpi3Dataset4all1);
  rpi3Plot4all.AddDataset (rpi3Dataset4all2);
  rpi3Plot4all.AddDataset (rpi3Dataset4all5);

  // Open the plot file.
  std::ofstream rpi3PlotFile1avg (rpi3PlotFileName1avg.c_str());
  std::ofstream rpi3PlotFile1all (rpi3PlotFileName1all.c_str());
  std::ofstream rpi3PlotFile2avg (rpi3PlotFileName2avg.c_str());
  std::ofstream rpi3PlotFile2all (rpi3PlotFileName2all.c_str());
  std::ofstream rpi3PlotFile3avg (rpi3PlotFileName3avg.c_str());
  std::ofstream rpi3PlotFile3all (rpi3PlotFileName3all.c_str());
  std::ofstream rpi3PlotFile4avg (rpi3PlotFileName4avg.c_str());
  std::ofstream rpi3PlotFile4all (rpi3PlotFileName4all.c_str());

  // Write the plot file.
  rpi3Plot1avg.GenerateOutput (rpi3PlotFile1avg);
  rpi3Plot1all.GenerateOutput (rpi3PlotFile1all);
  rpi3Plot2avg.GenerateOutput (rpi3PlotFile2avg);
  rpi3Plot2all.GenerateOutput (rpi3PlotFile2all);
  rpi3Plot3avg.GenerateOutput (rpi3PlotFile3avg);
  rpi3Plot3all.GenerateOutput (rpi3PlotFile3all);
  rpi3Plot4avg.GenerateOutput (rpi3PlotFile4avg);
  rpi3Plot4all.GenerateOutput (rpi3PlotFile4all);

  // Close the plot file.
  rpi3PlotFile1avg.close ();
  rpi3PlotFile1all.close ();
  rpi3PlotFile2avg.close ();
  rpi3PlotFile2all.close ();
  rpi3PlotFile3avg.close ();
  rpi3PlotFile3all.close ();
  rpi3PlotFile4avg.close ();
  rpi3PlotFile4all.close ();
}

void
espPlotter(void)
{
  // Add the dataset to the plot.
  espPlot1avg.AddDataset (espDataset1avg);
  espPlot1all.AddDataset (espDataset1all1);
  espPlot1all.AddDataset (espDataset1all2);
  espPlot1all.AddDataset (espDataset1all3);
  espPlot1all.AddDataset (espDataset1all4);
  espPlot1all.AddDataset (espDataset1all5);

  espPlot2avg.AddDataset (espDataset2avg);
  espPlot2all.AddDataset (espDataset2all1);
  espPlot2all.AddDataset (espDataset2all2);
  espPlot2all.AddDataset (espDataset2all3);
  espPlot2all.AddDataset (espDataset2all4);
  espPlot2all.AddDataset (espDataset2all5);

  espPlot3avg.AddDataset (espDataset3avg);
  espPlot3all.AddDataset (espDataset3all1);
  espPlot3all.AddDataset (espDataset3all2);
  espPlot3all.AddDataset (espDataset3all3);
  espPlot3all.AddDataset (espDataset3all4);
  espPlot3all.AddDataset (espDataset3all5);

  espPlot4avg.AddDataset (espDataset4avg);
  espPlot4all.AddDataset (espDataset4all1);
  espPlot4all.AddDataset (espDataset4all2);
  espPlot4all.AddDataset (espDataset4all3);
  espPlot4all.AddDataset (espDataset4all4);
  espPlot4all.AddDataset (espDataset4all5);

  // Open the plot file.
  std::ofstream espPlotFile1avg (espPlotFileName1avg.c_str());
  std::ofstream espPlotFile1all (espPlotFileName1all.c_str());
  std::ofstream espPlotFile2avg (espPlotFileName2avg.c_str());
  std::ofstream espPlotFile2all (espPlotFileName2all.c_str());
  std::ofstream espPlotFile3avg (espPlotFileName3avg.c_str());
  std::ofstream espPlotFile3all (espPlotFileName3all.c_str());
  std::ofstream espPlotFile4avg (espPlotFileName4avg.c_str());
  std::ofstream espPlotFile4all (espPlotFileName4all.c_str());

  // Write the plot file.
  espPlot1avg.GenerateOutput (espPlotFile1avg);
  espPlot1all.GenerateOutput (espPlotFile1all);
  espPlot2avg.GenerateOutput (espPlotFile2avg);
  espPlot2all.GenerateOutput (espPlotFile2all);
  espPlot3avg.GenerateOutput (espPlotFile3avg);
  espPlot3all.GenerateOutput (espPlotFile3all);
  espPlot4avg.GenerateOutput (espPlotFile4avg);
  espPlot4all.GenerateOutput (espPlotFile4all);

  // Close the plot file.
  espPlotFile1avg.close ();
  espPlotFile1all.close ();
  espPlotFile2avg.close ();
  espPlotFile2all.close ();
  espPlotFile3avg.close ();
  espPlotFile3all.close ();
  espPlotFile4avg.close ();
  espPlotFile4all.close ();
}

void
rpi0PrintInfo (NodeContainer nodes)
{
  double count = 0;
  double sum_power = 0;
  double sum_temp = 0;
  double sum_rel = 0;
  double avg_energy = 0;
  double avg_power = 0;
  double avg_temp = 0;
  double avg_rel = 0;
  double current_temp = 0;
  double current_pow = 0;
  double current_rel = 0;
  double current_time = Simulator::Now().GetSeconds();

  for(uint32_t i=0;i<nodes.GetN();i++)
  {
    count += 1;
    current_pow = nodes.Get(i)->GetObject<PowerModel>()->GetPower();
    current_temp = nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature();
    current_rel = nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability();

    std::cout<<"At time "<< current_time <<", NodeId = "<<nodes.Get(i)->GetId();
    std::cout << " CPU Power = " << current_pow;
    std::cout << " Temperature = " << current_temp;
    std::cout << " Reliability = " << current_rel <<std::endl;
    
    if(i==0){
        rpi0Dataset1all1.Add(current_time, rpi0cpuenergy1+rpi0wifienergy1);
        rpi0Dataset2all1.Add(current_time, current_pow);
        rpi0Dataset3all1.Add(current_time, current_temp);
        rpi0Dataset4all1.Add(current_time, current_rel);
    }
    if(i==1){
        rpi0Dataset1all2.Add(current_time, rpi0cpuenergy2+rpi0wifienergy2);
        rpi0Dataset2all2.Add(current_time, current_pow);
        rpi0Dataset3all2.Add(current_time, current_temp);
        rpi0Dataset4all2.Add(current_time, current_rel);
    }
    if(i==2){
        rpi0Dataset1all3.Add(current_time, rpi0cpuenergy3+rpi0wifienergy3);
        rpi0Dataset2all3.Add(current_time, current_pow);
        rpi0Dataset3all3.Add(current_time, current_temp);
        rpi0Dataset4all3.Add(current_time, current_rel);
    }
    if(i==3){
        rpi0Dataset1all4.Add(current_time, rpi0cpuenergy4+rpi0wifienergy4);
        rpi0Dataset2all4.Add(current_time, current_pow);
        rpi0Dataset3all4.Add(current_time, current_temp);
        rpi0Dataset4all4.Add(current_time, current_rel);
    }
    sum_power += current_pow;
    sum_temp += current_temp;
    sum_rel += current_rel;
  }
  avg_energy = (rpi0cpuenergy1 + rpi0cpuenergy2 + rpi0cpuenergy3 + rpi0cpuenergy4 + rpi0wifienergy1 + rpi0wifienergy2 + rpi0wifienergy3 + rpi0wifienergy4)/4.0;
  avg_power = sum_power/count;
  avg_temp = sum_temp/count;
  avg_rel = sum_rel/count;
  totalPower += avg_power;
  totalTemperature += avg_temp;
  totalReliability += avg_rel;
  avgcount += 1;
  rpi0Dataset1avg.Add(current_time, avg_energy);
  rpi0Dataset2avg.Add(current_time, avg_power);
  rpi0Dataset3avg.Add(current_time, avg_temp);
  rpi0Dataset4avg.Add(current_time, avg_rel);

  rpi0Dataset1all5.Add(current_time, avg_energy);
  rpi0Dataset2all5.Add(current_time, avg_power);
  rpi0Dataset3all5.Add(current_time, avg_temp);
  rpi0Dataset4all5.Add(current_time, avg_rel);

  if (!Simulator::IsFinished ())
  {
    Simulator::Schedule (Seconds (0.5),&rpi0PrintInfo,nodes);
  }
}
void
rpi3PrintInfo (NodeContainer nodes)
{
  double count = 0;
  double sum_power = 0;
  double sum_temp = 0;
  double sum_rel = 0;
  double avg_energy = 0;
  double avg_power = 0;
  double avg_temp = 0;
  double avg_rel = 0;
  double current_temp = 0;
  double current_pow = 0;
  double current_rel = 0;
  double current_time = Simulator::Now().GetSeconds();

  for(uint32_t i=0;i<nodes.GetN();i++)
  {
    count += 1;
    current_pow = nodes.Get(i)->GetObject<PowerModel>()->GetPower();
    current_temp = nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature();
    current_rel = nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability();

    std::cout<<"At time "<< current_time <<", NodeId = "<<nodes.Get(i)->GetId();
    std::cout << " CPU Power = " << current_pow;
    std::cout << " Temperature = " << current_temp;
    std::cout << " Reliability = " << current_rel <<std::endl;
    
    if(i==0){
        rpi3Dataset1all1.Add(current_time, rpi3cpuenergy1+rpi3wifienergy1);
        rpi3Dataset2all1.Add(current_time, current_pow);
        rpi3Dataset3all1.Add(current_time, current_temp);
        rpi3Dataset4all1.Add(current_time, current_rel);
    }
    if(i==1){
        rpi3Dataset1all2.Add(current_time, rpi3cpuenergy2+rpi3wifienergy2);
        rpi3Dataset2all2.Add(current_time, current_pow);
        rpi3Dataset3all2.Add(current_time, current_temp);
        rpi3Dataset4all2.Add(current_time, current_rel);
    }
    sum_power += current_pow;
    sum_temp += current_temp;
    sum_rel += current_rel;
  }
  avg_energy = (rpi3cpuenergy1 + rpi3cpuenergy2 + rpi3wifienergy1 + rpi3wifienergy2)/2.0;
  avg_power = sum_power/count;
  avg_temp = sum_temp/count;
  avg_rel = sum_rel/count;
  totalPower += avg_power;
  totalTemperature += avg_temp;
  totalReliability += avg_rel;
  avgcount += 1;
  rpi3Dataset1avg.Add(current_time, avg_energy);
  rpi3Dataset2avg.Add(current_time, avg_power);
  rpi3Dataset3avg.Add(current_time, avg_temp);
  rpi3Dataset4avg.Add(current_time, avg_rel);

  rpi3Dataset1all5.Add(current_time, avg_energy);
  rpi3Dataset2all5.Add(current_time, avg_power);
  rpi3Dataset3all5.Add(current_time, avg_temp);
  rpi3Dataset4all5.Add(current_time, avg_rel);

  if (!Simulator::IsFinished ())
  {
    Simulator::Schedule (Seconds (0.5),&rpi3PrintInfo,nodes);
  }
}


void
espPrintInfo (NodeContainer nodes)
{
  double count = 0;
  double sum_power = 0;
  double sum_temp = 0;
  double sum_rel = 0;
  double avg_energy = 0;
  double avg_power = 0;
  double avg_temp = 0;
  double avg_rel = 0;
  double current_temp = 0;
  double current_pow = 0;
  double current_rel = 0;
  double current_time = Simulator::Now().GetSeconds();

  for(uint32_t i=0;i<nodes.GetN();i++)
  {
    count += 1;
    current_pow = nodes.Get(i)->GetObject<PowerModel>()->GetPower();
    current_temp = nodes.Get(i)->GetObject<TemperatureModel>()->GetTemperature();
    current_rel = nodes.Get(i)->GetObject<ReliabilityModel>()->GetReliability();

    std::cout<<"At time "<< current_time <<", NodeId = "<<nodes.Get(i)->GetId();
    std::cout << " CPU Power = " << current_pow;
    std::cout << " Temperature = " << current_temp;
    std::cout << " Reliability = " << current_rel <<std::endl;
    
    if(i==0){
        espDataset1all1.Add(current_time, espcpuenergy1+espwifienergy1);
        espDataset2all1.Add(current_time, current_pow);
        espDataset3all1.Add(current_time, current_temp);
        espDataset4all1.Add(current_time, current_rel);
    }
    if(i==1){
        espDataset1all2.Add(current_time, espcpuenergy2+espwifienergy2);
        espDataset2all2.Add(current_time, current_pow);
        espDataset3all2.Add(current_time, current_temp);
        espDataset4all2.Add(current_time, current_rel);
    }
    if(i==2){
        espDataset1all3.Add(current_time, espcpuenergy3+espwifienergy3);
        espDataset2all3.Add(current_time, current_pow);
        espDataset3all3.Add(current_time, current_temp);
        espDataset4all3.Add(current_time, current_rel);
    }
    if(i==3){
        espDataset1all4.Add(current_time, espcpuenergy4+espwifienergy4);
        espDataset2all4.Add(current_time, current_pow);
        espDataset3all4.Add(current_time, current_temp);
        espDataset4all4.Add(current_time, current_rel);
    }
    sum_power += current_pow;
    sum_temp += current_temp;
    sum_rel += current_rel;
  }
  avg_energy = (espcpuenergy1 + espcpuenergy2 + espcpuenergy3 + espcpuenergy4 + espwifienergy1 + espwifienergy2 + espwifienergy3 + espwifienergy4)/4.0;
  avg_power = sum_power/count;
  avg_temp = sum_temp/count;
  avg_rel = sum_rel/count;
  totalPower += avg_power;
  totalTemperature += avg_temp;
  totalReliability += avg_rel;
  avgcount += 1;
  espDataset1avg.Add(current_time, avg_energy);
  espDataset2avg.Add(current_time, avg_power);
  espDataset3avg.Add(current_time, avg_temp);
  espDataset4avg.Add(current_time, avg_rel);

  espDataset1all5.Add(current_time, avg_energy);
  espDataset2all5.Add(current_time, avg_power);
  espDataset3all5.Add(current_time, avg_temp);
  espDataset4all5.Add(current_time, avg_rel);

  if (!Simulator::IsFinished ())
  {
    Simulator::Schedule (Seconds (0.5),&espPrintInfo,nodes);
  }
}


void
PrintAverages(void)
{
  
  averagePower = totalPower/avgcount;
  averageTemperature = totalTemperature/avgcount;
  averageReliability = totalReliability/avgcount;
  std::cout << "Average power = " << averagePower << std::endl;
  std::cout << "Average temperature = " << averageTemperature << std::endl;
  std::cout << "Average reliability = " << averageReliability << std::endl;
}

static inline std::string
PrintID (Ptr<Node> n)
{
    uint32_t id = n->GetId();

    std::ostringstream oss;
    oss << "ID: " << id << "  ";
    
    return oss.str ();
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

/** Total CPU Energy RPi0 **/
/***************************************************************************/
void rpi0TotalCpuEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0cpuenergy1 = totalEnergy;
}
void rpi0TotalCpuEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0cpuenergy2 = totalEnergy;
}
void rpi0TotalCpuEnergy3 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0cpuenergy3 = totalEnergy;
}
void rpi0TotalCpuEnergy4 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0cpuenergy4 = totalEnergy;
}
/***************************************************************************/
/** Total WiFi Energy RPi0 **/
/***************************************************************************/
void rpi0TotalWifiEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0wifienergy1 = totalEnergy;
}
void rpi0TotalWifiEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0wifienergy2 = totalEnergy;
}
void rpi0TotalWifiEnergy3 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0wifienergy3 = totalEnergy;
}
void rpi0TotalWifiEnergy4 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi0wifienergy4 = totalEnergy;
}
/***************************************************************************/
/** Total CPU Energy RPi3 **/
/***************************************************************************/
void rpi3TotalCpuEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total CPU energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi3cpuenergy1 = totalEnergy;
}
void rpi3TotalCpuEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total CPU energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi3cpuenergy2 = totalEnergy;
}
/***************************************************************************/
/***************************************************************************/
/** Total WiFi Energy RPi3 **/
/***************************************************************************/
void rpi3TotalWifiEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total WiFi energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi3wifienergy1 = totalEnergy;
}
void rpi3TotalWifiEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total WiFi energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    rpi3wifienergy2 = totalEnergy;
}
/***************************************************************************/
/** Total CPU Energy ESP **/
/***************************************************************************/
void espTotalCpuEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espcpuenergy1 = totalEnergy;
}
void espTotalCpuEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espcpuenergy2 = totalEnergy;
}
void espTotalCpuEnergy3 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espcpuenergy3 = totalEnergy;
}
void espTotalCpuEnergy4 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espcpuenergy4 = totalEnergy;
}
/***************************************************************************/
/** Total WiFi Energy ESP **/
/***************************************************************************/
void espTotalWifiEnergy1 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espwifienergy1 = totalEnergy;
}
void espTotalWifiEnergy2 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espwifienergy2 = totalEnergy;
}
void espTotalWifiEnergy3 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espwifienergy3 = totalEnergy;
}
void espTotalWifiEnergy4 (std::string context, double oldValue, double totalEnergy)
{
    //NS_LOG_UNCOND ("%INFO TimeStamp: "<<Simulator::Now ().GetSeconds ()<<" segs Total energy consumed Node: "<<context<<" "<<totalEnergy<< " Joules");
    //dataset1all1.Add(Simulator::Now ().GetSeconds (), totalEnergy);
    espwifienergy4 = totalEnergy;
}
/***************************************************************************/

int main (int argc, char *argv[])
{
  /** Plot RPi0 **/
  /***************************************************************************/
  rpi0Plot1avg.SetTitle (rpi0PlotTitle1avg);
  rpi0Plot1all.SetTitle (rpi0PlotTitle1all);
  rpi0Plot2avg.SetTitle (rpi0PlotTitle2avg);
  rpi0Plot2all.SetTitle (rpi0PlotTitle2all);
  rpi0Plot3avg.SetTitle (rpi0PlotTitle3avg);
  rpi0Plot3all.SetTitle (rpi0PlotTitle3all);
  rpi0Plot4avg.SetTitle (rpi0PlotTitle4avg);
  rpi0Plot4all.SetTitle (rpi0PlotTitle4all);
  // Make the graphics file, which the plot file will create when it is used with Gnuplot, be a PNG file.
  rpi0Plot1avg.SetTerminal ("png");
  rpi0Plot1all.SetTerminal ("png");
  rpi0Plot2avg.SetTerminal ("png");
  rpi0Plot2all.SetTerminal ("png");
  rpi0Plot3avg.SetTerminal ("png");
  rpi0Plot3all.SetTerminal ("png");
  rpi0Plot4avg.SetTerminal ("png");
  rpi0Plot4all.SetTerminal ("png");
  // Set the labels for each axis.
  rpi0Plot1avg.SetLegend ("time(s)", "Average Energy Consumption(J)");
  rpi0Plot1all.SetLegend ("time(s)", "Energy Consumption(J)");
  rpi0Plot2avg.SetLegend ("time(s)", "Average Power(W)");
  rpi0Plot2all.SetLegend ("time(s)", "Power(W)");
  rpi0Plot3avg.SetLegend ("time(s)", "Average Temperature(°C)");
  rpi0Plot3all.SetLegend ("time(s)", "Temperature(°C)");
  rpi0Plot4avg.SetLegend ("time(s)", "Average Reliability");
  rpi0Plot4all.SetLegend ("time(s)", "Reliability");
  rpi0Dataset1avg.SetTitle (rpi0Data1Titleavg);
  rpi0Dataset1avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset1all1.SetTitle (rpi0Data1Titleall1);
  rpi0Dataset1all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset1all2.SetTitle (rpi0Data1Titleall2);
  rpi0Dataset1all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset1all3.SetTitle (rpi0Data1Titleall3);
  rpi0Dataset1all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset1all4.SetTitle (rpi0Data1Titleall4);
  rpi0Dataset1all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset1all5.SetTitle (rpi0Data1Titleall5);
  rpi0Dataset1all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2avg.SetTitle (rpi0Data2Titleavg);
  rpi0Dataset2avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2all1.SetTitle (rpi0Data2Titleall1);
  rpi0Dataset2all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2all2.SetTitle (rpi0Data2Titleall2);
  rpi0Dataset2all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2all3.SetTitle (rpi0Data2Titleall3);
  rpi0Dataset2all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2all4.SetTitle (rpi0Data2Titleall4);
  rpi0Dataset2all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset2all5.SetTitle (rpi0Data2Titleall5);
  rpi0Dataset2all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3avg.SetTitle (rpi0Data3Titleavg);
  rpi0Dataset3avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3all1.SetTitle (rpi0Data3Titleall1);
  rpi0Dataset3all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3all2.SetTitle (rpi0Data3Titleall2);
  rpi0Dataset3all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3all3.SetTitle (rpi0Data3Titleall3);
  rpi0Dataset3all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3all4.SetTitle (rpi0Data3Titleall4);
  rpi0Dataset3all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset3all5.SetTitle (rpi0Data3Titleall5);
  rpi0Dataset3all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4avg.SetTitle (rpi0Data4Titleavg);
  rpi0Dataset4avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4all1.SetTitle (rpi0Data4Titleall1);
  rpi0Dataset4all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4all2.SetTitle (rpi0Data4Titleall2);
  rpi0Dataset4all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4all3.SetTitle (rpi0Data4Titleall3);
  rpi0Dataset4all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4all4.SetTitle (rpi0Data4Titleall4);
  rpi0Dataset4all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi0Dataset4all5.SetTitle (rpi0Data4Titleall5);
  rpi0Dataset4all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  /***************************************************************************/

  /** Plot RPi3 **/
  /***************************************************************************/
  rpi3Plot1avg.SetTitle (rpi3PlotTitle1avg);
  rpi3Plot1all.SetTitle (rpi3PlotTitle1all);
  rpi3Plot2avg.SetTitle (rpi3PlotTitle2avg);
  rpi3Plot2all.SetTitle (rpi3PlotTitle2all);
  rpi3Plot3avg.SetTitle (rpi3PlotTitle3avg);
  rpi3Plot3all.SetTitle (rpi3PlotTitle3all);
  rpi3Plot4avg.SetTitle (rpi3PlotTitle4avg);
  rpi3Plot4all.SetTitle (rpi3PlotTitle4all);
  // Make the graphics file, which the plot file will create when it is used with Gnuplot, be a PNG file.
  rpi3Plot1avg.SetTerminal ("png");
  rpi3Plot1all.SetTerminal ("png");
  rpi3Plot2avg.SetTerminal ("png");
  rpi3Plot2all.SetTerminal ("png");
  rpi3Plot3avg.SetTerminal ("png");
  rpi3Plot3all.SetTerminal ("png");
  rpi3Plot4avg.SetTerminal ("png");
  rpi3Plot4all.SetTerminal ("png");
  // Set the labels for each axis.
  rpi3Plot1avg.SetLegend ("time(s)", "Average Energy Consumption(J)");
  rpi3Plot1all.SetLegend ("time(s)", "Energy Consumption(J)");
  rpi3Plot2avg.SetLegend ("time(s)", "Average Power(W)");
  rpi3Plot2all.SetLegend ("time(s)", "Power(W)");
  rpi3Plot3avg.SetLegend ("time(s)", "Average Temperature(°C)");
  rpi3Plot3all.SetLegend ("time(s)", "Temperature(°C)");
  rpi3Plot4avg.SetLegend ("time(s)", "Average Reliability");
  rpi3Plot4all.SetLegend ("time(s)", "Reliability");
  rpi3Dataset1avg.SetTitle (rpi3Data1Titleavg);
  rpi3Dataset1avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset1all1.SetTitle (rpi3Data1Titleall1);
  rpi3Dataset1all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset1all2.SetTitle (rpi3Data1Titleall2);
  rpi3Dataset1all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset1all5.SetTitle (rpi3Data1Titleall5);
  rpi3Dataset1all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset2avg.SetTitle (rpi3Data2Titleavg);
  rpi3Dataset2avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset2all1.SetTitle (rpi3Data2Titleall1);
  rpi3Dataset2all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset2all2.SetTitle (rpi3Data2Titleall2);
  rpi3Dataset2all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset2all5.SetTitle (rpi3Data2Titleall5);
  rpi3Dataset2all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset3avg.SetTitle (rpi3Data3Titleavg);
  rpi3Dataset3avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset3all1.SetTitle (rpi3Data3Titleall1);
  rpi3Dataset3all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset3all2.SetTitle (rpi3Data3Titleall2);
  rpi3Dataset3all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset3all5.SetTitle (rpi3Data3Titleall5);
  rpi3Dataset3all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset4avg.SetTitle (rpi3Data4Titleavg);
  rpi3Dataset4avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset4all1.SetTitle (rpi3Data4Titleall1);
  rpi3Dataset4all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset4all2.SetTitle (rpi3Data4Titleall2);
  rpi3Dataset4all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  rpi3Dataset4all5.SetTitle (rpi3Data4Titleall5);
  rpi3Dataset4all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  /***************************************************************************/
  /** Plot ESP **/
  /***************************************************************************/
  espPlot1avg.SetTitle (espPlotTitle1avg);
  espPlot1all.SetTitle (espPlotTitle1all);
  espPlot2avg.SetTitle (espPlotTitle2avg);
  espPlot2all.SetTitle (espPlotTitle2all);
  espPlot3avg.SetTitle (espPlotTitle3avg);
  espPlot3all.SetTitle (espPlotTitle3all);
  espPlot4avg.SetTitle (espPlotTitle4avg);
  espPlot4all.SetTitle (espPlotTitle4all);
  // Make the graphics file, which the plot file will create when it is used with Gnuplot, be a PNG file.
  espPlot1avg.SetTerminal ("png");
  espPlot1all.SetTerminal ("png");
  espPlot2avg.SetTerminal ("png");
  espPlot2all.SetTerminal ("png");
  espPlot3avg.SetTerminal ("png");
  espPlot3all.SetTerminal ("png");
  espPlot4avg.SetTerminal ("png");
  espPlot4all.SetTerminal ("png");
  // Set the labels for each axis.
  espPlot1avg.SetLegend ("time(s)", "Average Energy Consumption(J)");
  espPlot1all.SetLegend ("time(s)", "Energy Consumption(J)");
  espPlot2avg.SetLegend ("time(s)", "Average Power(W)");
  espPlot2all.SetLegend ("time(s)", "Power(W)");
  espPlot3avg.SetLegend ("time(s)", "Average Temperature(°C)");
  espPlot3all.SetLegend ("time(s)", "Temperature(°C)");
  espPlot4avg.SetLegend ("time(s)", "Average Reliability");
  espPlot4all.SetLegend ("time(s)", "Reliability");
  espDataset1avg.SetTitle (espData1Titleavg);
  espDataset1avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset1all1.SetTitle (espData1Titleall1);
  espDataset1all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset1all2.SetTitle (espData1Titleall2);
  espDataset1all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset1all3.SetTitle (espData1Titleall3);
  espDataset1all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset1all4.SetTitle (espData1Titleall4);
  espDataset1all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset1all5.SetTitle (espData1Titleall5);
  espDataset1all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2avg.SetTitle (espData2Titleavg);
  espDataset2avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2all1.SetTitle (espData2Titleall1);
  espDataset2all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2all2.SetTitle (espData2Titleall2);
  espDataset2all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2all3.SetTitle (espData2Titleall3);
  espDataset2all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2all4.SetTitle (espData2Titleall4);
  espDataset2all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset2all5.SetTitle (espData2Titleall5);
  espDataset2all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3avg.SetTitle (espData3Titleavg);
  espDataset3avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3all1.SetTitle (espData3Titleall1);
  espDataset3all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3all2.SetTitle (espData3Titleall2);
  espDataset3all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3all3.SetTitle (espData3Titleall3);
  espDataset3all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3all4.SetTitle (espData3Titleall4);
  espDataset3all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset3all5.SetTitle (espData3Titleall5);
  espDataset3all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4avg.SetTitle (espData4Titleavg);
  espDataset4avg.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4all1.SetTitle (espData4Titleall1);
  espDataset4all1.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4all2.SetTitle (espData4Titleall2);
  espDataset4all2.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4all3.SetTitle (espData4Titleall3);
  espDataset4all3.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4all4.SetTitle (espData4Titleall4);
  espDataset4all4.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  espDataset4all5.SetTitle (espData4Titleall5);
  espDataset4all5.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  /***************************************************************************/


  std::string phyMode ("DsssRate1Mbps");
  //double rss = -80;  // -dBm , received signal strength
  bool verbose = false;
  double simulationTime = 365.0;                        /* Simulation time in seconds. */
  double startTime = 1.0;
  bool flow_monitor = true;
  //LogComponentEnable ("OnOffApplication", LOG_LEVEL_ALL);

/** Generating traffic parameters **/
  uint32_t packetSize = 1000; // bytes
  std::string dataRate = "150Kbps";                  /* Application layer datarate. */
  std::string dataRate3 = "250Kbps";
  uint32_t dataSize = 10000;   // bytes, for applications
  uint32_t infDataSize = 1000000000;   // bytes, for applications




  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                    StringValue (phyMode));

  /** Wifi Nodes **/
  /***************************************************************************/
  NodeContainer espNodes;
  NodeContainer rpi0Nodes;
  NodeContainer rpi3Nodes;
  espNodes.Create (4);
  rpi0Nodes.Create (4);
  rpi3Nodes.Create (2);
  Ptr<Node> node1 = rpi3Nodes.Get (0);
  Ptr<Node> node2 = rpi3Nodes.Get (1);
  Ptr<Node> node3 = rpi0Nodes.Get (0);
  Ptr<Node> node4 = rpi0Nodes.Get (1);
  Ptr<Node> node5 = rpi0Nodes.Get (2);
  Ptr<Node> node6 = rpi0Nodes.Get (3);
  Ptr<Node> esp1 = espNodes.Get (0);
  Ptr<Node> esp2 = espNodes.Get (1);
  Ptr<Node> esp3 = espNodes.Get (2);
  Ptr<Node> esp4 = espNodes.Get (3);
  // Scenario 1 
  NodeContainer rpi01 = NodeContainer (node3);
  NodeContainer rpi0other = NodeContainer(node4);
  rpi0other.Add(node5);
  rpi0other.Add(node6);
  NodeContainer rpi31 = NodeContainer (node1);
  NodeContainer rpi32 = NodeContainer (node2);


  /***************************************************************************/


  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  /** Wifi PHY **/
  /***************************************************************************/
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (0.0));
  wifiPhy.Set ("TxGain", DoubleValue (0.0));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0));
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  //wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  /***************************************************************************/

  /** Wifi Channel **/
  /***************************************************************************/
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  //wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  // The following has an absolute cutoff at distance > 250
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(70)); 
  // The below FixedRssLossModel will cause the rss to be fixed regardless of the distance between the two stations, and the transmit power
  //wifiChannel.AddPropagationLoss ("ns3::FixedRssLossModel","Rss",DoubleValue (rss));
  // Create wifi channel
  Ptr<YansWifiChannel> wifiChannelPtr = wifiChannel.Create ();
  wifiPhy.SetChannel (wifiChannelPtr);
  /***************************************************************************/



  /** MAC Layer **/
  /***************************************************************************/
  WifiMacHelper wifiMac;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  //wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // Use AARF rate control
  wifiMac.SetType ("ns3::AdhocWifiMac");
  /***************************************************************************/

  /** Install Devices **/
  NetDeviceContainer espDevices = wifi.Install (wifiPhy, wifiMac, espNodes);
  NetDeviceContainer rpi0Devices = wifi.Install (wifiPhy, wifiMac, rpi0Nodes);
  NetDeviceContainer rpi3Devices = wifi.Install (wifiPhy, wifiMac, rpi3Nodes);
  /***************************************************************************/



  /** Mobility - ESP **/
  /***************************************************************************/
  MobilityHelper espMobility;
  Ptr<ListPositionAllocator> espPositionAlloc = CreateObject<ListPositionAllocator> ();
  espPositionAlloc->Add (Vector (59.1, 52.72, 0.0));
  espPositionAlloc->Add (Vector (60.0, 56.96, 0.0));
  espPositionAlloc->Add (Vector (68.5, 68.5, 0.0));
  espPositionAlloc->Add (Vector (68.7, 53.5, 0.0));
  espMobility.SetPositionAllocator (espPositionAlloc);
  espMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  espMobility.Install (espNodes);
  /***************************************************************************/


  /** Mobility - RPI0 **/
  /***************************************************************************/
  MobilityHelper rpi0Mobility;
  Ptr<ListPositionAllocator> rpi0PositionAlloc = CreateObject<ListPositionAllocator> ();
  rpi0PositionAlloc->Add (Vector (35.75, 14.11, 0.0));
  rpi0PositionAlloc->Add (Vector (59.83, 12.5, 0.0));
  rpi0PositionAlloc->Add (Vector (66.0, 30.3, 0.0));
  rpi0PositionAlloc->Add (Vector (56.0, 37.57, 0.0));
  rpi0Mobility.SetPositionAllocator (rpi0PositionAlloc);
  rpi0Mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  rpi0Mobility.Install (rpi0Nodes);
  /***************************************************************************/


    /** Mobility - RPI3 **/
  /***************************************************************************/
  MobilityHelper rpi3Mobility;
  Ptr<ListPositionAllocator> rpi3PositionAlloc = CreateObject<ListPositionAllocator> ();
  rpi3PositionAlloc->Add (Vector (0.0, 0.0, 0.0));
  rpi3PositionAlloc->Add (Vector (63.0, 53.0, 0.0));
  rpi3Mobility.SetPositionAllocator (rpi3PositionAlloc);
  rpi3Mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  rpi3Mobility.Install (rpi3Nodes);
  /***************************************************************************/


  /** Energy-Reliability - ESP **/
  /***************************************************************************/
  /* Energy Source */
  BasicEnergySourceHelper espBasicSourceHelper;
  espBasicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (100000)); //~Unlimited initial energy
  EnergySourceContainer espSources = espBasicSourceHelper.Install (espNodes);
  /* Reliability Stack */
  ReliabilityHelper espReliabilityHelper;
  espReliabilityHelper.SetDeviceType("Arduino");
  espReliabilityHelper.SetPowerModel("ns3::AppPowerModel");
  espReliabilityHelper.SetPerformanceModel("ns3::PerformanceSimpleModel");
  espReliabilityHelper.SetTemperatureModel("ns3::TemperatureSimpleModel");
  espReliabilityHelper.SetReliabilityModel("ns3::ReliabilityTDDBModel");
  espReliabilityHelper.SetApplication("MedianFilter",dataSize,packetSize);
  espReliabilityHelper.Install(espNodes);
  /* CPU Energy Model */
  CpuEnergyModelHelper espCpuEnergyHelper;
  espCpuEnergyHelper.Install(espDevices, espSources);
  /* Device energy model */
  WifiRadioEnergyModelHelper espRadioEnergyHelper;
  espRadioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.150)); // Needs to be configured based on hardware
  espRadioEnergyHelper.Set ("IdleCurrentA", DoubleValue (0.001)); // Needs to be configured based on hardware
  espRadioEnergyHelper.Set ("RxCurrentA", DoubleValue (0.053)); // Needs to be configured based on hardware
  DeviceEnergyModelContainer espDeviceModels = espRadioEnergyHelper.Install (espDevices, espSources);

  // Total energy consumption trace
  Ptr<BasicEnergySource> espbasicSourcePtr1 = DynamicCast<BasicEnergySource> (espSources.Get (0));
  Ptr<BasicEnergySource> espbasicSourcePtr2 = DynamicCast<BasicEnergySource> (espSources.Get (1));
  Ptr<BasicEnergySource> espbasicSourcePtr3 = DynamicCast<BasicEnergySource> (espSources.Get (2));
  Ptr<BasicEnergySource> espbasicSourcePtr4 = DynamicCast<BasicEnergySource> (espSources.Get (3));
  Ptr<DeviceEnergyModel> espbasicCpuModelPtr1 = espbasicSourcePtr1->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicCpuModelPtr2 = espbasicSourcePtr2->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicCpuModelPtr3 = espbasicSourcePtr3->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicCpuModelPtr4 = espbasicSourcePtr4->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  espbasicCpuModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (0)), MakeCallback (&espTotalCpuEnergy1));
  espbasicCpuModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (1)), MakeCallback (&espTotalCpuEnergy2));
  espbasicCpuModelPtr3->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (2)), MakeCallback (&espTotalCpuEnergy3));
  espbasicCpuModelPtr4->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (3)), MakeCallback (&espTotalCpuEnergy4));
  // Total energy consumption trace
  Ptr<DeviceEnergyModel> espbasicRadioModelPtr1 = espbasicSourcePtr1->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicRadioModelPtr2 = espbasicSourcePtr2->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicRadioModelPtr3 = espbasicSourcePtr3->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> espbasicRadioModelPtr4 = espbasicSourcePtr4->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  espbasicRadioModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (0)), MakeCallback (&espTotalWifiEnergy1));
  espbasicRadioModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (1)), MakeCallback (&espTotalWifiEnergy2));
  espbasicRadioModelPtr3->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (2)), MakeCallback (&espTotalWifiEnergy3));
  espbasicRadioModelPtr4->TraceConnect ("TotalEnergyConsumption",PrintID(espNodes.Get (3)), MakeCallback (&espTotalWifiEnergy4));

  /***************************************************************************/

  /** Energy-Reliability - RPI0 **/
  /***************************************************************************/
  /* Energy Source */
  BasicEnergySourceHelper rpi0BasicSourceHelper;
  rpi0BasicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (100000)); //~Unlimited initial energy
  EnergySourceContainer rpi0Sources = rpi0BasicSourceHelper.Install (rpi0Nodes);


  /* Reliability Stack */
  ReliabilityHelper rpi01ReliabilityHelper;
  rpi01ReliabilityHelper.SetDeviceType("RaspberryPi0");
  rpi01ReliabilityHelper.SetPowerModel("ns3::AppPowerModel");
  rpi01ReliabilityHelper.SetPerformanceModel("ns3::PerformanceSimpleModel");
  rpi01ReliabilityHelper.SetTemperatureModel("ns3::TemperatureSimpleModel");
  rpi01ReliabilityHelper.SetReliabilityModel("ns3::ReliabilityTDDBModel");
  rpi01ReliabilityHelper.SetApplication("LinearRegression",infDataSize,packetSize);
  rpi01ReliabilityHelper.Install(rpi01);
  /* CPU Energy Model */
  CpuEnergyModelHelper rpi01CpuEnergyHelper;
  rpi01CpuEnergyHelper.Install(rpi0Devices.Get(0), rpi0Sources.Get(0));

  rpi01.Get(0)->GetObject<TemperatureModel>()->SetTenv(37);

  /* Reliability Stack */
  ReliabilityHelper rpi0otherReliabilityHelper;
  rpi0otherReliabilityHelper.SetDeviceType("RaspberryPi0");
  rpi0otherReliabilityHelper.SetPowerModel("ns3::AppPowerModel");
  rpi0otherReliabilityHelper.SetPerformanceModel("ns3::PerformanceSimpleModel");
  rpi0otherReliabilityHelper.SetTemperatureModel("ns3::TemperatureSimpleModel");
  rpi0otherReliabilityHelper.SetReliabilityModel("ns3::ReliabilityTDDBModel");
  rpi0otherReliabilityHelper.SetApplication("LinearRegression",dataSize,packetSize);
  rpi0otherReliabilityHelper.Install(rpi0other);
  /* CPU Energy Model */
  CpuEnergyModelHelper rpi0other1CpuEnergyHelper;
  rpi0other1CpuEnergyHelper.Install(rpi0Devices.Get(1), rpi0Sources.Get(1));
    /* CPU Energy Model */
  CpuEnergyModelHelper rpi0other2CpuEnergyHelper;
  rpi0other2CpuEnergyHelper.Install(rpi0Devices.Get(2), rpi0Sources.Get(2));
    /* CPU Energy Model */
  CpuEnergyModelHelper rpi0other3CpuEnergyHelper;
  rpi0other3CpuEnergyHelper.Install(rpi0Devices.Get(3), rpi0Sources.Get(3));
  

  /* Device energy model */
  WifiRadioEnergyModelHelper rpi0RadioEnergyHelper;
  rpi0RadioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.300)); // Needs to be configured based on hardware
  rpi0RadioEnergyHelper.Set ("IdleCurrentA", DoubleValue (0.00000058)); // Needs to be configured based on hardware
  rpi0RadioEnergyHelper.Set ("RxCurrentA", DoubleValue (0.040)); // Needs to be configured based on hardware
  DeviceEnergyModelContainer rpi0DeviceModels = rpi0RadioEnergyHelper.Install (rpi0Devices, rpi0Sources);

  // Total energy consumption trace
  Ptr<BasicEnergySource> rpi0basicSourcePtr1 = DynamicCast<BasicEnergySource> (rpi0Sources.Get (0));
  Ptr<BasicEnergySource> rpi0basicSourcePtr2 = DynamicCast<BasicEnergySource> (rpi0Sources.Get (1));
  Ptr<BasicEnergySource> rpi0basicSourcePtr3 = DynamicCast<BasicEnergySource> (rpi0Sources.Get (2));
  Ptr<BasicEnergySource> rpi0basicSourcePtr4 = DynamicCast<BasicEnergySource> (rpi0Sources.Get (3));
  Ptr<DeviceEnergyModel> rpi0basicCpuModelPtr1 = rpi0basicSourcePtr1->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicCpuModelPtr2 = rpi0basicSourcePtr2->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicCpuModelPtr3 = rpi0basicSourcePtr3->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicCpuModelPtr4 = rpi0basicSourcePtr4->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  rpi0basicCpuModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (0)), MakeCallback (&rpi0TotalCpuEnergy1));
  rpi0basicCpuModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (1)), MakeCallback (&rpi0TotalCpuEnergy2));
  rpi0basicCpuModelPtr3->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (2)), MakeCallback (&rpi0TotalCpuEnergy3));
  rpi0basicCpuModelPtr4->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (3)), MakeCallback (&rpi0TotalCpuEnergy4));
  // Total energy consumption trace
  Ptr<DeviceEnergyModel> rpi0basicRadioModelPtr1 = rpi0basicSourcePtr1->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicRadioModelPtr2 = rpi0basicSourcePtr2->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicRadioModelPtr3 = rpi0basicSourcePtr3->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi0basicRadioModelPtr4 = rpi0basicSourcePtr4->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  rpi0basicRadioModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (0)), MakeCallback (&rpi0TotalWifiEnergy1));
  rpi0basicRadioModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (1)), MakeCallback (&rpi0TotalWifiEnergy2));
  rpi0basicRadioModelPtr3->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (2)), MakeCallback (&rpi0TotalWifiEnergy3));
  rpi0basicRadioModelPtr4->TraceConnect ("TotalEnergyConsumption",PrintID(rpi0Nodes.Get (3)), MakeCallback (&rpi0TotalWifiEnergy4));
  /***************************************************************************/

  /** Energy-Reliability - RPI3 **/
  /***************************************************************************/
  /* Energy Source */
  BasicEnergySourceHelper rpi3BasicSourceHelper;
  rpi3BasicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (100000)); //~Unlimited initial energy
  EnergySourceContainer rpi3Sources = rpi3BasicSourceHelper.Install (rpi3Nodes);




  /* Reliability Stack */
  ReliabilityHelper rpi31ReliabilityHelper;
  rpi31ReliabilityHelper.SetDeviceType("RaspberryPi");
  rpi31ReliabilityHelper.SetPowerModel("ns3::AppPowerModel");
  rpi31ReliabilityHelper.SetPerformanceModel("ns3::PerformanceSimpleModel");
  rpi31ReliabilityHelper.SetTemperatureModel("ns3::TemperatureSimpleModel");
  rpi31ReliabilityHelper.SetReliabilityModel("ns3::ReliabilityTDDBModel");
  rpi31ReliabilityHelper.SetApplication("LinearRegression",infDataSize,packetSize);
  rpi31ReliabilityHelper.Install(rpi31);
  /* CPU Energy Model */
  CpuEnergyModelHelper rpi31CpuEnergyHelper;
  rpi31CpuEnergyHelper.Install(rpi3Devices.Get(0), rpi3Sources.Get(0));  

  rpi31.Get(0)->GetObject<TemperatureModel>()->SetTenv(37);

  /* Reliability Stack */
  ReliabilityHelper rpi32ReliabilityHelper;
  rpi32ReliabilityHelper.SetDeviceType("RaspberryPi");
  rpi32ReliabilityHelper.SetPowerModel("ns3::AppPowerModel");
  rpi32ReliabilityHelper.SetPerformanceModel("ns3::PerformanceSimpleModel");
  rpi32ReliabilityHelper.SetTemperatureModel("ns3::TemperatureSimpleModel");
  rpi32ReliabilityHelper.SetReliabilityModel("ns3::ReliabilityTDDBModel");
  rpi32ReliabilityHelper.SetApplication("LinearRegression",infDataSize,packetSize);
  rpi32ReliabilityHelper.Install(rpi32);
  /* CPU Energy Model */
  CpuEnergyModelHelper rpi32CpuEnergyHelper;
  rpi32CpuEnergyHelper.Install(rpi3Devices.Get(1), rpi3Sources.Get(1));

  /* Device energy model */
  WifiRadioEnergyModelHelper rpi3RadioEnergyHelper;
  rpi3RadioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0350)); // Needs to be configured based on hardware
  rpi3RadioEnergyHelper.Set ("IdleCurrentA", DoubleValue (0.00002)); // Needs to be configured based on hardware
  rpi3RadioEnergyHelper.Set ("RxCurrentA", DoubleValue (0.055)); // Needs to be configured based on hardware
  DeviceEnergyModelContainer rpi3DeviceModels = rpi3RadioEnergyHelper.Install (rpi3Devices, rpi3Sources);

    // Total energy consumption trace
  Ptr<BasicEnergySource> rpi3basicSourcePtr1 = DynamicCast<BasicEnergySource> (rpi3Sources.Get (0));
  Ptr<BasicEnergySource> rpi3basicSourcePtr2 = DynamicCast<BasicEnergySource> (rpi3Sources.Get (1));
  Ptr<DeviceEnergyModel> rpi3basicCpuModelPtr1 = rpi3basicSourcePtr1->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi3basicCpuModelPtr2 = rpi3basicSourcePtr2->FindDeviceEnergyModels ("ns3::CpuEnergyModel").Get (0);
  rpi3basicCpuModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(rpi3Nodes.Get (0)), MakeCallback (&rpi3TotalCpuEnergy1));
  rpi3basicCpuModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(rpi3Nodes.Get (1)), MakeCallback (&rpi3TotalCpuEnergy2));
    // Total energy consumption trace
  Ptr<DeviceEnergyModel> rpi3basicRadioModelPtr1 = rpi3basicSourcePtr1->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  Ptr<DeviceEnergyModel> rpi3basicRadioModelPtr2 = rpi3basicSourcePtr2->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  rpi3basicRadioModelPtr1->TraceConnect ("TotalEnergyConsumption",PrintID(rpi3Nodes.Get (0)), MakeCallback (&rpi3TotalWifiEnergy1));
  rpi3basicRadioModelPtr2->TraceConnect ("TotalEnergyConsumption",PrintID(rpi3Nodes.Get (1)), MakeCallback (&rpi3TotalWifiEnergy2));

  /***************************************************************************/
  // Enable OLSR
  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 0);
  list.Add (olsr, 10);
  /** Internet Stack **/
  /***************************************************************************/
  InternetStackHelper internet;
  internet.Install (espNodes);
  internet.Install (rpi0Nodes);
  internet.Install (rpi3Nodes);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4rpi3;
  ipv4rpi3.SetBase ("172.27.0.0", "255.255.0.0", "0.0.0.1"); // Note that second RPi3's ip adress is 172.27.0.2 in this setup
  Ipv4InterfaceContainer i1 = ipv4rpi3.Assign (rpi3Devices);
  Ipv4AddressHelper ipv4rpi0;
  ipv4rpi0.SetBase ("172.27.0.0", "255.255.0.0", "0.0.0.3");
  Ipv4InterfaceContainer i2 = ipv4rpi0.Assign (rpi0Devices);
  Ipv4AddressHelper ipv4esp;
  ipv4esp.SetBase ("172.27.0.0", "255.255.0.0", "0.0.0.7");
  Ipv4InterfaceContainer i3 = ipv4esp.Assign (espDevices);
  /***************************************************************************/

  /** Socket-Source/Sink **/
  /***************************************************************************/
  // Setting applications
  ApplicationContainer sourceApplications, sinkApplications;
  uint32_t portNumber = 9;
  for (uint8_t index = 0; index < rpi0other.GetN (); ++index)
    {
          auto ipv4 = rpi0other.Get (index)->GetObject<Ipv4> ();
          const auto address = ipv4->GetAddress (1, 0).GetLocal ();
          InetSocketAddress sinkSocket (address, portNumber++);
          OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
          onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.12]"));
          onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.88]"));
          onOffHelper.SetAttribute ("DataRate", DataRateValue (DataRate (dataRate)));
          onOffHelper.SetAttribute ("PacketSize", UintegerValue (packetSize)); //bytes
          sourceApplications.Add (onOffHelper.Install (rpi32.Get (0)));
          PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
          sinkApplications.Add (packetSinkHelper.Install (rpi0other.Get (index)));
    }

  sink = StaticCast<PacketSink> (sinkApplications.Get (0));
  sinkApplications.Start (Seconds (0.0));
//  sinkApplications.Stop (Seconds (10.0));
  sourceApplications.Start (Seconds (startTime));
//  sourceApplications.Stop (Seconds (10.0));
//  Simulator::Schedule (Seconds (1.1), &CalculateThroughput);
  /***************************************************************************/
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  /** Flow Monitor **/
  /***************************************************************************/
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  if (flow_monitor)
    {
      flowMonitor = flowHelper.InstallAll ();
    }

  rpi0PrintInfo (rpi0Nodes);
  rpi3PrintInfo (rpi3Nodes);
  espPrintInfo (espNodes);

  Simulator::Stop (Seconds (simulationTime+1));
  Simulator::Schedule (Seconds(simulationTime-0.1), &rpi0Plotter);
  Simulator::Schedule (Seconds(simulationTime-0.1), &rpi3Plotter);
  Simulator::Schedule (Seconds(simulationTime-0.1), &espPlotter);
  Simulator::Schedule (Seconds(simulationTime-0.1), &PrintAverages);
  Simulator::Run ();

  if (flow_monitor)
    {
      flowMonitor->SerializeToXmlFile ("flowmonitor.xml", true, true);
    }

  // Print per flow statistics
  flowMonitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = flowMonitor->GetFlowStats ();
  Histogram delayHistogram;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      
      if ((t.sourceAddress == Ipv4Address("172.27.0.2") && t.destinationAddress == Ipv4Address("172.27.0.4"))
         || (t.sourceAddress == Ipv4Address("172.27.0.2") && t.destinationAddress == Ipv4Address("172.27.0.5"))
         || (t.sourceAddress == Ipv4Address("172.27.0.2") && t.destinationAddress == Ipv4Address("172.27.0.6")))
        {
          std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
          std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
          std::cout << "  DelaySum: " << i->second.delaySum.GetSeconds() << "\n";
          std::cout << "  Mean Delay: " << (i->second.delaySum.GetSeconds())/(i->second.rxPackets) << "\n";
          std::cout << "  Number of Hops: " << i->second.timesForwarded << "\n";        
          delayHistogram = i->second.delayHistogram;          
          std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds()-i->second.timeFirstTxPacket.GetSeconds()) / 1024  << " Kbps\n";
        }
    }
  /***************************************************************************/

  Simulator::Destroy ();
  /** Aggregated Throughput **/
  /***************************************************************************/
  double throughput = 0;
  for (unsigned index = 0; index < sinkApplications.GetN (); ++index)
    {
      uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();
      throughput += ((totalPacketsThrough * 8) / ((simulationTime-startTime) * 1000000.0)); //Mbit/s
    }
  if (throughput > 0)
    {
      std::cout << "Aggregated throughput: " << throughput << " Mbit/s" << std::endl;
    }
  else
    {
      NS_LOG_ERROR ("Obtained throughput is 0!");
      exit (1);
    }
  /***************************************************************************/

  return 0;
}

