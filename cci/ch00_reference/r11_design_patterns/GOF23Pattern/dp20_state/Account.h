/*           
 * Title        :   20. State pattern
 * Filename     :   Account.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

// Forward Declaration
class IState;

/* Context  */
// defined here as its used for forward declaration
class Account
{
public:
  Account(std::string owner);
  ~Account();
  double GetBalance(void);
  void Deposit(double amount);
  void Withdraw(double amount);
  void PayInterest();
  void SetState(IState* state);
  IState* GetState(void);

private:
  IState* _state;
  std::string _owner;
  Account();
};
