#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("BgpLiteExample");

int main(int argc, char *argv[])
{
  Time::SetResolution(Time::NS);
  CommandLine cmd;
  cmd.Parse(argc, argv);

  LogComponentEnable("BgpLiteExample", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NS_LOG_INFO("Starting BGP-lite style simulation with 3 routers...");

  // Create 3 routers (AS1, AS2, AS3)
  NodeContainer routers;
  routers.Create(3);

  // Assign constant positions (silences NetAnim warnings)
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(routers);

  // Install Internet stack
  InternetStackHelper internet;
  internet.Install(routers);

  // Standard AS1-AS2, AS2-AS3 links
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer d12 = p2p.Install(routers.Get(0), routers.Get(1));
  NetDeviceContainer d23 = p2p.Install(routers.Get(1), routers.Get(2));

  // Direct AS1-AS3 faster link
  PointToPointHelper p2pNewLink;
  p2pNewLink.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
  p2pNewLink.SetChannelAttribute("Delay", StringValue("1ms"));
  NetDeviceContainer d13 = p2pNewLink.Install(routers.Get(0), routers.Get(2));

  // Assign IP addresses
  Ipv4AddressHelper ipv4;

  ipv4.SetBase("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i12 = ipv4.Assign(d12);

  ipv4.SetBase("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i23 = ipv4.Assign(d23);

  ipv4.SetBase("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i13 = ipv4.Assign(d13);

  // Let global routing decide best path
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  // UDP Echo server on AS3
  uint16_t port = 9;
  UdpEchoServerHelper server(port);
  ApplicationContainer serverApps = server.Install(routers.Get(2));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));

  // UDP Echo client on AS1 targeting AS3’s IP on AS1-AS3 link
  UdpEchoClientHelper client(i13.GetAddress(1), port);
  client.SetAttribute("MaxPackets", UintegerValue(5));
  client.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  client.SetAttribute("PacketSize", UintegerValue(1024));

  ApplicationContainer clientApps = client.Install(routers.Get(0));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10.0));

  // Flow Monitor
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  // NetAnim visualization
  AnimationInterface anim("bgp-lite-final-animation.xml");

  Simulator::Stop(Seconds(11.0));
  Simulator::Run();

  // Save flow statistics
  monitor->SerializeToXmlFile("bgp-lite-final-results.xml", true, true);

  Simulator::Destroy();
  return 0;
}
