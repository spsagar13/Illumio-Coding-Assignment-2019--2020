###############################################################
# Author 	: Sagar Surendran
# Date Created	: Nov - 08 - 2019
# Brief		: This is a simple Firewall c++ program filters 
#		  the input vectors based on predefined vectors
###############################################################


#include "firewall.h"

int main()
{
	ifstream test_file;
	string f_name;

	cout << "\nInput the name of the csv file : ";
	cin >> f_name;

	Firewall fw = Firewall(f_name);
	fw.init();

	cout << "\nEnter the value to be tested in csv format or type EXIT to stop eg \" inbound,udp,24,52.12.48.92 \" : ";
	string t_line;
	cin >> t_line;

	while ( 0 != strcmp(t_line.c_str(), "EXIT") )
	{
		list<string> t_values_splitted;
		split_line(t_line, t_values_splitted);

		string t_direction;
		string t_protocol;
		int t_port = 0;
		string t_ip;
		string s_port;

		t_direction = t_values_splitted.front();
		t_values_splitted.pop_front();
		t_protocol = t_values_splitted.front();
		t_values_splitted.pop_front();
		s_port = t_values_splitted.front();

		stringstream ss(s_port);
		ss >> t_port;

		t_values_splitted.pop_front();
		t_ip = t_values_splitted.front();
		t_values_splitted.pop_front();

		if (true == fw.accept_packet(t_direction, t_protocol, t_port, t_ip))
			cout << "\ntrue\n";
		else
			cout << "\nfalse\n";

		cout << "\nEnter the value to be tested in csv format or type EXIT to stop eg \" inbound,udp,24,52.12.48.92 \" : ";
		cin >> t_line;
	}

}

Firewall::Firewall(string file_name)
{
	m_input_file_name = file_name;
}

void Firewall::init()
{
	ifstream input_file;

	input_file.open(m_input_file_name);
	if (input_file.is_open())
	{
		string line;
		while (getline(input_file, line))
		{
			construct_structure(line);
		}
	}
	input_file.close();
}

bool Firewall::accept_packet(string t_direction, string t_protocol, int t_port, string t_ip)
{

	if (0 == strcmp(t_direction.c_str(), "inbound"))
	{
		if (0 == strcmp(t_protocol.c_str(), "tcp"))
		{
			return is_match(itcp, t_port, t_ip);
		}
		else if (0 == strcmp(t_protocol.c_str(), "udp"))
		{
			return is_match(iudp, t_port, t_ip);
		}
		else
		{
			return false;
		}
	}
	else if (0 == strcmp(t_direction.c_str(), "outbound"))
	{
		if (0 == strcmp(t_protocol.c_str(), "tcp"))
		{
			return is_match(otcp, t_port, t_ip);
		}
		else if (0 == strcmp(t_protocol.c_str(), "udp"))
		{
			return is_match(oudp, t_port, t_ip);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool Firewall::is_match(map<long,ip_port> &firewall_map , int &_port, string &_ip)
{
	bool is_found_match = false;

	map<long, ip_port>::iterator it_main = firewall_map.find(_port);

	struct ip _ip_o;
	make_ip(_ip, _ip_o);

	long long Ip = _ip_o.start;

	if ( it_main == firewall_map.end())
	{//not found
		for (map<long, ip_port>::iterator it = firewall_map.begin(); it != firewall_map.end(); ++it)
		{
			if (  (it->second._port.range)&&(( _port > it->second._port.start ) && ( _port <= it->second._port.end)) )
			{
				is_found_match = is_ip_in_range(it->second._ip, Ip);	
			}
		}
			
	}
	else
	{//found
		is_found_match = is_ip_in_range(it_main->second._ip, Ip);
	}
	return is_found_match;
}

bool Firewall::is_ip_in_range(struct ip &_ip, long long &Ip)
{
	bool is_found_match = false;
	if ( _ip.range)
	{
		if (( _ip.range ) && (( Ip >= _ip.start ) && ( Ip <= _ip.end )))
		{
			is_found_match = true;
		}
	}
	else
	{
		if (_ip.start == Ip)
		{
			is_found_match = true;
		}
	}
	return is_found_match;
}

void Firewall::construct_structure(string line)
{
	list<string> values_splitted;
	string _direction;
	string _protocol;
	string _port;
	string _ip;
	split_line(line, values_splitted);

	_direction = values_splitted.front();
	values_splitted.pop_front();
	_protocol = values_splitted.front();
	values_splitted.pop_front();
	_port = values_splitted.front();
	values_splitted.pop_front();
	_ip = values_splitted.front();
	values_splitted.pop_front();

	ip_port ipo;
	make_ip_port(_ip, _port, ipo);

	if (0 == strcmp(_direction.c_str(), "inbound"))
	{
		if (0 == strcmp(_protocol.c_str(), "tcp"))
		{
			itcp.insert(make_pair(ipo._port.start, ipo));
		}
		else if (0 == strcmp(_protocol.c_str(), "udp"))
		{
			iudp.insert(make_pair(ipo._port.start,ipo));
		}

	}
	else if (0 == strcmp(_direction.c_str(), "outbound"))
	{
		if (0 == strcmp(_protocol.c_str(), "tcp"))
		{
			otcp.insert(make_pair(ipo._port.start,ipo));
		}
		else if (0 == strcmp(_protocol.c_str(), "udp"))
		{
			oudp.insert(make_pair(ipo._port.start, ipo));
		}

	}
}

void Firewall::make_ip_port(string ip, string port, struct ip_port &ipo)
{
	struct ip _ip_o;
	struct port _port_o;
	make_ip(ip, _ip_o);
	make_port(port, _port_o);

	ipo._ip = _ip_o;
	ipo._port = _port_o;
}

void Firewall::make_ip(string ip, struct ip& _ip_o)
{
	long long start = 0;
	long long end = 0;
	bool range = false;

	std::size_t found = ip.find("-");

	if (string::npos == found)
	{
		//It is a single ip value
		start = ip_to_int(ip);
	}
	else //it has a range
	{
		range = true;
		string ip1 = ip.substr(0, found);
		string ip2 = ip.substr(found+1);

		start = ip_to_int(ip1);
		end = ip_to_int(ip2);
	}

	_ip_o.start = start;
	_ip_o.end = end;
	_ip_o.range = range;
}

void Firewall::make_port(string port_, struct port& _port_o)
{
	long start = 0;
	long end = 0;
	bool range = false;

	std::size_t found = port_.find("-");

	if (string::npos == found)
	{
		//It is a single port value
		stringstream value(port_);
		value >> start;
	}
	else //it has a range
	{
		range = true;
		string port1 = port_.substr(0, found);
		string port2 = port_.substr(found + 1);

		stringstream value1(port1);
		stringstream value2(port2);

		value1 >> start;
		value2 >> end;

	}
	_port_o.start = start;
	_port_o.end = end;
	_port_o.range = range;
}

long long ip_to_int(string ip)
{
	stringstream ss_temp(ip);
	vector<int> dis_ip;

	for (int i; ss_temp >> i;) {
		dis_ip.push_back(i);
		if (ss_temp.peek() == '.')
			ss_temp.ignore();
	}

	//Converting the IP into a single integer
	long long value = 1000 + dis_ip[0];
	value *= 1000;
	value += dis_ip[1];
	value *= 1000;
	value += dis_ip[2];
	value *= 1000;
	value += dis_ip[3];

	return value;
}

void split_line(string line, list<string>& values_splitted)
{
	stringstream ss(line);
	while (ss.good())
	{
		string sub;
		getline(ss, sub, ',');
		values_splitted.push_back(sub);
	}
}
