#include "Account.hpp"
#include "iostream"

int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

int	Account::getNbAccounts()
{
	return (_nbAccounts);
}

int	Account::getTotalAmount()
{
	return (_totalAmount);
}

int	Account::getNbDeposits()
{
	return (_totalNbDeposits);
}

int	Account::getNbWithdrawals()
{
	return (_totalNbWithdrawals);
}


Account::Account(int initial_deposit)
{
	_amount = initial_deposit;
	_totalAmount += _amount;
	_accountIndex = _nbAccounts;
	_nbAccounts++;
	std::cout << "index:" << _accountIndex << ";amount:" \
	<< _amount << ";created" << std::endl;
}

Account::~Account()
{
	std::cout << "index:" << _accountIndex << ";amount:" \
	<< _amount << ";closed" << std::endl;
}

void Account::makeDeposit(int deposit)
{
	_totalNbDeposits++;
	this->_nbDeposits += 1;
	this->_amount += deposit;
	_totalAmount += deposit;
	std::cout << "index:" << this->_accountIndex << ";p_amount:" \
	<< this->_amount - deposit << ";deposit:" << deposit << ";amount:" \
	<< this->_amount << ";nb_deposits:" << this->_nbDeposits << std::endl;
}

bool	Account::makeWithdrawal( int withdrawl)
{
	bool accepted = false;

	if (this->_amount >= withdrawl)
		accepted = true;
	
	if (accepted)
	{
		_totalNbWithdrawals++;
		this->_nbWithdrawals += 1;
		this->_amount -= withdrawl;
		_totalAmount -= withdrawl;
		std::cout << "index:" << this->_accountIndex << ";p_amount:" \
		<< this->_amount + withdrawl << ";withdrawal:" << withdrawl \
		<< ";amount:" << this->_amount \
		<< ";nb_withdrawals:" << this->_nbWithdrawals << std::endl;
	}
	else
	{
		std::cout << "index:" << this->_accountIndex << ";p_amount:" \
		<< this->_amount << ";withdrawal:refused" << std::endl;
	}
	return (accepted);
}

int	Account::checkAmount() const
{
	std::cout << _amount << std::endl;
	return (_amount);
}

void	Account::displayStatus() const
{
	std::cout << "index:" << this->_accountIndex \
	<< ";amount:" << this->_amount << ";deposits:" \
	<< this->_nbDeposits << ";withdrawals:" \
	<< this->_nbWithdrawals << std::endl;
}

void Account::displayAccountsInfos()
{
	std::cout << "accounts:" << _nbAccounts << ";total:" \
	<< _totalAmount << ";deposits:" << _totalNbDeposits \
	<< ";withdrawals:" << _totalNbWithdrawals << std::endl;
}