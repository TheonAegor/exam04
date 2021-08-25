#include "all.hpp"

void	print_all(PhoneBook *pb, int stop)
{
	int	i = -1;

	cout << "stop = " << stop << "\n";
	while (++i < stop)
	{
		cout << "i = " << i << "\n";
		pb[i].ShowP();
	}
}

bool if_digit(std::string &s)
{
	for (std::string::iterator it = s.begin(); it != s.end(); ++it)
	{
		if (!std::isdigit(*it))
			return (false);
	}
	return (true);
}

void	print_header(void)
{
	int	w = 10;
	
	std::cout.width(w); cout << std::right << "index"; 
	cout << "|";
	std::cout.width(w); cout << std::right << "first name"; 
	cout << "|";
	std::cout.width(w); cout << std::right << "last name"; 
	cout << "|";
	std::cout.width(w); cout << std::right << "nickname"; 
	cout << "|" << endl;

}

string	my_trim(string s, int w)
{
	int	len;

	len = s.length();
	if (len > w - 1)
	{
		s.resize(w - 1);
		s.append(".");
	}
	return (s);
}

int	process_add(PhoneBook *inst)
{
	string	add_cmds[] = {"first name", "last name", "nickname", "phone number","darkest secret"};
	int		num_fields = 5;
	string	response;
	int		i = -1;

	while (++i < num_fields)
	{
		cout << add_cmds[i] << ": ";
		getline(cin,response);
		(*inst).Add(response, add_cmds[i]);
	}
	if (inst->num_of_ppl < inst->limit)
		inst->num_of_ppl++;
	return (0);
}

void	process_search(PhoneBook *pb)
{
	int	w = 10;
	string response;
	int	index;

	print_header();
	if (pb != nullptr)
	{
		for (int i = 0; i < pb[i].num_of_ppl; i++ )
		{
			std::cout.width(w); cout << std::right << i; 
			cout << "|";
			std::cout.width(w); cout << std::right << my_trim(pb[i].Get("first_name"), w); 
			cout << "|";
			std::cout.width(w); cout << std::right << my_trim(pb[i].Get("last_name"), w); 
			cout << "|";
			std::cout.width(w); cout << std::right << my_trim(pb[i].Get("nickname"), w); 
			cout << "|" << endl;
		}
		cout << "Type index: ";
		getline(cin, response);
		if (!if_digit(response))
			cout << "Bad input" << endl;
		else
		{
			index = std::atoi(response.c_str());
			if (index > pb[0].num_of_ppl)
				cout << "Index is out of range" << endl;
			else
				pb[index].ShowP();
		}
	}
}

int	main(void) {
	string 		cmd;
	int			i;
	PhoneBook	pb[8];

	i = 0;
	while (cmd != "EXIT")
	{
		if (i == pb[0].limit)
			i = 0;
		cout << "Enter a command:";
		getline(cin, cmd);
/*
		if (cmd == "SHOW")
			print_all(pb, i);
*/
		if (cmd == "SEARCH")
			process_search(pb);
		if (cmd == "ADD")
		{
			process_add(&(pb[i]));
			i++;
		}
	}
}
