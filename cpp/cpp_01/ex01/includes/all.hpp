#ifndef ALL_HPP
#define ALL_HPP

#include <iostream>
#include <unistd.h>
#include <iomanip> 

using namespace std;

class PhoneBook
{
	private:
		string		first_name;
		string		last_name;
		string		nickname;
		string		phone;
		string		darkest_secret;

	public:
		PhoneBook();

		string		Get(string field);
		void		Add(string smth, string field);
		void		ShowP();
		static int	num_of_ppl;
		const int	limit = 8;

};
int PhoneBook::num_of_ppl = 0;

void	PhoneBook::ShowP()
{
	string	add_cmds[] = {"first name", "last name", "nickname", "phone number","darkest secret"};
	int		i;

	i = -1;
	cout << add_cmds[++i] << ": ";
	cout << this->first_name << "\n";
	cout << add_cmds[++i] << ": ";
	cout << this->last_name << "\n";
	cout << add_cmds[++i] << ": ";
	cout << this->nickname << "\n";
	cout << add_cmds[++i] << ": ";
	cout << this->phone << "\n";
	cout << add_cmds[++i] << ": ";
	cout << this->darkest_secret << "\n";
}

string	PhoneBook::Get(string field)
{
	if (field == "first_name")
		return(this->first_name);
	if (field == "last_name")
		return(this->last_name);
	if (field == "nickname")
		return(this->nickname);
	if (field == "phone")
		return(this->phone);
	if (field == "darkest_secret")
		return(this->darkest_secret);
	else
		return ("");
}

void	PhoneBook::Add(string smth, string field)
{
	if (field == "first name")
		this->first_name.assign(smth);
	else if (field == "last name")
		this->last_name.assign(smth);
	else if (field == "nickname")
		this->nickname.assign(smth);
	else if (field == "phone number")
		this->phone.assign(smth);
	else if (field == "darkest secret")
		this->darkest_secret.assign(smth);
}

PhoneBook::PhoneBook()
{
	first_name = "";
	last_name = "";
	nickname = "";
	phone = "";
	darkest_secret = "";
}

bool if_digit(const std::string &s);
#endif