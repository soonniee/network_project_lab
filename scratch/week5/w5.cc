#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("week5");

int main(int argc, char *argv[]){
	
	CommandLine cmd;
	std::string dataRate = "10", delay="10";
	cmd.AddValue("DataRate","DataRate removing Mbps",dataRate);
	cmd.AddValue("Delay","Delay",delay);
	cmd.Parse(argc,argv);
	
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoClientApplication",LOG_PREFIX_TIME);
	LogComponentEnable("UdpEchoClientApplication",LOG_PREFIX_FUNC);

	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication",LOG_PREFIX_TIME);
	LogComponentEnable("UdpEchoServerApplication",LOG_PREFIX_FUNC);
	

	LogComponentEnable("week4",LOG_LEVEL_ALL);
	NS_LOG_INFO("CREATE NODES..");
	NodeContainer nodes;
	nodes.Create(2);
	NodeContainer n0n1 = NodeContainer(nodes.Get(0),nodes.Get(1));
	//NodeContainer n1n2 = NodeContainer(nodes.Get(1),nodes.Get(2));

	PointToPointHelper p2p;

	p2p.SetDeviceAttribute("DataRate",StringValue(dataRate));
	p2p.SetChannelAttribute("Delay",StringValue(delay));
	NetDeviceContainer d0d1 = p2p.Install(n0n1);

	//p2p.SetDeviceAttribute("DataRate",StringValue("0.1Mbps"));
	//p2p.SetChannelAttribute("Delay",StringValue("10ms"));
	//NetDeviceContainer d1d2 = p2p.Install(n1n2);

	p2p.EnablePcapAll("week5");
	InternetStackHelper internet;
	internet.Install(nodes);
		
	Ipv4AddressHelper ipv4;
	ipv4.SetBase("10.1.1.0","255.255.255.0");
	Ipv4InterfaceContainer i0i1 = ipv4.Assign(d0d1);	
	
	//ipv4.SetBase("10.1.2.0","255.255.255.0");
	//Ipv4InterfaceContainer i1i2 = ipv4.Assign(d1d2);
	
	UdpEchoClientHelper echoClient1(i0i1.GetAddress(1),9);
	echoClient1.SetAttribute("MaxPackets",UintegerValue(1500));
	echoClient1.SetAttribute("Interval",TimeValue(Seconds(0.001)));
	echoClient1.SetAttribute("PacketSize",UintegerValue(1050));

	ApplicationContainer clientApps1;
	clientApps1.Add(echoClient1.Install(n0n1.Get(0)));
	clientApps1.Start(Seconds(2.0));
	clientApps1.Stop(Seconds(4.0));

	UdpEchoServerHelper echoServer1(9);
	ApplicationContainer serverApps1(echoServer1.Install(n0n1.Get(1)));
	serverApps1.Start(Seconds(1.0));
	serverApps1.Stop(Seconds(5.0));
	
	/*UdpEchoClientHelper echoClient2(i1i2.GetAddress(1),9);
	echoClient2.SetAttribute("MaxPackets",UintegerValue(1500));
	echoClient2.SetAttribute("Interval",TimeValue(Seconds(0.01)));
	echoClient2.SetAttribute("PacketSize",UintegerValue(1050));

	ApplicationContainer clientApps2;
	clientApps2.Add(echoClient2.Install(n1n2.Get(0)));
	clientApps2.Start(Seconds(2.0));
	clientApps2.Stop(Seconds(4.0));

	UdpEchoServerHelper echoServer2(9);
	ApplicationContainer serverApps2(echoServer2.Install(n1n2.Get(1)));
	serverApps2.Start(Seconds(1.0));
	serverApps2.Stop(Seconds(5.0));*/
	Simulator::Run();
	Simulator::Stop(Seconds(5.0));

	Simulator::Destroy();
	return 0;

	

}
