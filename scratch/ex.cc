#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include <iostream>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("week4");

int 
main(int argc, char*argv[])
{
	LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_ALL);
	//LogComponentEnable("UdpEchoClientApplication", LOG_PREFIX_TIME);
	LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_ALL);
	LogComponentEnable("week4", LOG_LEVEL_ALL);
	
	NS_LOG_INFO("Create Nodes");	
	NodeContainer nodes;
	nodes.Create(2);

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("2ms"));

	NetDeviceContainer devices;
	devices = p2p.Install(nodes);

	InternetStackHelper stack;
	stack.Install(nodes);

	Ipv4AddressHelper addr;
	addr.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = addr.Assign(devices);
	
	UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
	echoClient.SetAttribute("MaxPackets", UintegerValue(100));
	echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
	echoClient.SetAttribute("PacketSize", UintegerValue(1024));

	ApplicationContainer clientApps;
	clientApps.Add(echoClient.Install(nodes.Get(0)));
	clientApps.Start(Seconds(1.0));
	clientApps.Stop(Seconds(10.0));

	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApps(echoServer.Install(nodes.Get(1)));
	serverApps.Start(Seconds(0));
	serverApps.Stop(Seconds(11.0));

	Simulator::Run();
	Simulator::Stop(Seconds(11.0));
	Simulator::Destroy();

	return 0;
}
