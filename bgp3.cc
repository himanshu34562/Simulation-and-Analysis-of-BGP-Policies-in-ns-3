#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BgpLocalPrefExample");

int main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  CommandLine cmd;
  cmd.Parse (argc, argv);

  LogComponentEnable ("BgpLocalPrefExample", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Starting BGP Local-Pref style simulation...");

  // Create nodes (AS1, AS2, AS3)
  NodeContainer routers;
  routers.Create(3);

  // Install Internet stack
  InternetStackHelper internet;
  internet.Install(routers);

  // Create links
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("2ms"));

  NetDeviceContainer d12 = p2p.Install(routers.Get(0), routers.Get(1));
  NetDeviceContainer d23 = p2p.Install(routers.Get(1), routers.Get(2));

  PointToPointHelper p2pFast;
  p2pFast.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
  p2pFast.SetChannelAttribute("Delay", StringValue("1ms"));
  NetDeviceContainer d13 = p2pFast.Install(routers.Get(0), routers.Get(2));

  // Assign IPs
  Ipv4AddressHelper ipv4;
  ipv4.SetBase("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i12 = ipv4.Assign(d12);

  ipv4.SetBase("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i23 = ipv4.Assign(d23);

  ipv4.SetBase("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i13 = ipv4.Assign(d13);

  // Enforce policy: AS1 prefers AS2 → AS3, not direct AS1–AS3
  Ipv4StaticRoutingHelper staticRoutingHelper;
  Ptr<Ipv4> ipv4As1 = routers.Get(0)->GetObject<Ipv4>();
  Ptr<Ipv4StaticRouting> staticRouting = staticRoutingHelper.GetStaticRouting(ipv4As1);

  staticRouting->AddNetworkRouteTo(
      Ipv4Address("10.0.2.0"), Ipv4Mask("255.255.255.0"),
      Ipv4Address(i12.GetAddress(1)), // Next hop = AS2
      1); // Outgoing iface toward AS2

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  // UDP server on AS3
  uint16_t port = 9;
  UdpEchoServerHelper server(port);
  ApplicationContainer apps = server.Install(routers.Get(2));
  apps.Start(Seconds(1.0));
  apps.Stop(Seconds(10.0));

  // UDP client on AS1 → AS3’s address
  UdpEchoClientHelper client(i23.GetAddress(1), port);
  client.SetAttribute("MaxPackets", UintegerValue(5));
  client.SetAttribute("Interval", TimeValue(Seconds(1.0)));
  client.SetAttribute("PacketSize", UintegerValue(1024));
  apps = client.Install(routers.Get(0));
  apps.Start(Seconds(2.0));
  apps.Stop(Seconds(10.0));

  // Flow monitor
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  // Fix mobility warnings
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(routers);

  // Run simulation + NetAnim
  Simulator::Stop(Seconds(11.0));
  AnimationInterface anim("bgp-local-pref-animation.xml");
  Simulator::Run();

  monitor->SerializeToXmlFile("bgp-local-pref-results.xml", true, true);

  Simulator::Destroy();
  return 0;
}
