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

/* Context */
// defined here as its used for forward declaration
Account::Account(std::string owner):_owner(owner)
{
  // default
  _state = reinterpret_cast<IState*>(new SilverState(0.0, this));
  std::cout << "Account::Account: " << _owner << std::endl;
}

Account::~Account()
{
  delete _state;
  std::cout << "Account::~Account: " << _owner << std::endl;
}

double Account::GetBalance(void)
{
  return _state->GetBalance();
}

void Account::Deposit(double amount)
{
  _state->Deposit(amount);
  std::cout << "Deposited $" << amount << std::endl;
  std::cout << "Balance   $" << GetBalance() << std::endl;
  std::cout << "Status     " << _state->GetStateName() << std::endl;
  std::cout << "\n";
}

void Account::Withdraw(double amount)
{
  _state->Withdraw(amount);
  std::cout << "Withdrew  $" << amount << std::endl;
  std::cout << "Balance   $" << GetBalance() << std::endl;
  std::cout << "Status     " << _state->GetStateName() << std::endl;
  std::cout << "\n";
}

void Account::PayInterest()
{
  _state->PayInterest();
  std::cout << "Interest Paid --------" << std::endl;
  std::cout << "Balance   $" << GetBalance() << std::endl;
  std::cout << "Status     " << _state->GetStateName() << std::endl;
  std::cout << "\n";
}

void Account::SetState(IState* state)
{
  _state = state;
}

IState* Account::GetState(void)
{
  return _state;
}
