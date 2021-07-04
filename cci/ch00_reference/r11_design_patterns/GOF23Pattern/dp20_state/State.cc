/*           
 * Title        :   20. State pattern
 * Filename     :   State.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#include "State.h"
#include "Account.h"


/* State interface  */

Account* IState::GetAccount(void)
{
  return _account;
}

void IState::SetAccount(Account* account)
{
  _account = account;
}

double IState::GetBalance(void)
{
  return _balance;
}

void IState::SetBalance(double balance)
{
  _balance = balance;
}

std::string IState::GetStateName(void)
{
  return _stateName;
}

/* Concrete state */
/* Red indicates that account is overdrawn */
RedState::RedState(IState* state)
{
  this->_balance = state->GetBalance();
  this->_account = state->GetAccount();
  Initialize();
}
void RedState::Deposit(double amount)
{
  _balance += amount;
  StateChangeCheck();
}

void RedState::Withdraw(double amount)
{
  double newAmount = amount + _serviceFee;
  if(_balance - newAmount < _lowerLimit)
    std:: cout << "No funds available for withdrawal!" << std::endl;
  else
    _balance -= newAmount;
}

void RedState::PayInterest()
{
 //No interest is paid
}

void RedState::StateChangeCheck()
{
  if (_balance > _upperLimit)
  {
    _account->SetState(reinterpret_cast<IState*>(new SilverState(this)));
    delete this;
    return;
  }
}

void RedState::Initialize()
{
  _stateName = "Red";
  //Should come from a data source
  _interest = 0.0;
  _lowerLimit = -100.0;
  _upperLimit = 0.0;
  _serviceFee = 15.0;
}
 
/* Silver indicates less interest bearing state */
SilverState::SilverState(IState* state)
{
  this->_balance = state->GetBalance();
  this->_account = state->GetAccount();
  Initialize();
}

SilverState::SilverState(double balance, Account* account)
{
  this->_balance = balance;
  this->_account = account;
  Initialize();
}

void SilverState::Deposit(double amount)
{
  _balance += amount;
  StateChangeCheck();
}

void SilverState::Withdraw(double amount)
{
  _balance -= amount;
  StateChangeCheck();
}

void SilverState::PayInterest()
{
  _balance = _balance * _interest;
  StateChangeCheck();
}

void SilverState::StateChangeCheck()
{
  if (_balance < _lowerLimit)
  {
    _account->SetState(reinterpret_cast<IState*>(new RedState(this)));
    delete this;
    return;
  }
  else if (_balance > _upperLimit)
  {
    _account->SetState(reinterpret_cast<IState*>(new GoldState(this)));
    delete this;
    return;
  }
}

void SilverState::Initialize()
{
  _stateName = "Silver";
  //Should come from a data source
  _interest = 1.0;
  _lowerLimit = 0.0;
  _upperLimit = 1000.0;
} 

/* Gold indicates high interest bearing state */
GoldState::GoldState(IState* state)
{
  this->_balance = state->GetBalance();
  this->_account = state->GetAccount();
  Initialize();
}

void GoldState::Deposit(double amount)
{
  _balance += amount;
  StateChangeCheck();
}

void GoldState::Withdraw(double amount)
{
  _balance -= amount;
  StateChangeCheck();
}

void GoldState::PayInterest()
{
  _balance = _balance * _interest;
  StateChangeCheck();
}

void GoldState::StateChangeCheck()
{
  if (_balance < 0.0)
  {
    _account->SetState(reinterpret_cast<IState*>(new RedState(this)));
    delete this;
    return;
  }
  else if (_balance < _lowerLimit)
  {
    _account->SetState(reinterpret_cast<IState*>(new SilverState(this)));
    delete this;
    return;
  }
  else if (_balance > _upperLimit)
  {
    std::cout << "Your account is too big now. " 
      << "Please consider using Swiss banks" << std::endl;
  }
}

void GoldState::Initialize()
{
  _stateName = "Gold";
  //Should come from a data source
  _interest = 5.0;
  _lowerLimit = 1000.0;
  _upperLimit = 10000000.0;
}
