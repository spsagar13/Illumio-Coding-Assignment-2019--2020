##################################################################
# Author	: Sagar Surendran
# Date Created	: Nov - 08 - 2019
# Brief		: Header file for Firewall.cpp
##################################################################


#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <vector>

using namespace std;

struct ip
{
	long long start;
	long long end;
	bool range;
};

struct port
{
	int start;
	int end;
	bool range;
};

struct ip_port
{
	ip _ip;
	port _port;
};


class Firewall
{
public:
	Firewall(string file_name);
	
	void init();
	void construct_structure(string line);
	void make_ip_port(string ip, string port, struct ip_port& ipo);
	void make_ip(string ip, struct ip& _ip_o);
	void make_port(string port_, struct port& _port_o);

	bool accept_packet(string t_direction, string t_protocol, int t_port, string t_ip);

	bool is_match(map<long, ip_port>& firewall_map, int& _port, string& _ip);
	bool is_ip_in_range(struct ip& _ip, long long& Ip);

private:
	string m_input_file_name;

	map<long, ip_port> itcp;
	map<long, ip_port> iudp;
	map<long, ip_port> otcp;
	map<long, ip_port> oudp;
};


//Utility Functions
long long ip_to_int(string ip);
void split_line(string line, list<string>& values_splitted);
