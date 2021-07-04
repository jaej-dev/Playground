/*           
 * Title        :   20. IState pattern
 * Filename     :   IState.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include "Account.h"

/* IState interface  */
class IState
{
public:
  Account* GetAccount(void);
  void SetAccount(Account* account);
  double GetBalance(void);
  void SetBalance(double balance);
  std::string GetStateName(void);
  virtual void Deposit(double amount) = 0;
  virtual void Withdraw(double amount) = 0;
  virtual void PayInterest(void) = 0;
  virtual ~IState() {}

protected:
  Account* _account;
  double _balance;
  double _interest;
  double _lowerLimit;
  double _upperLimit;
  std::string _stateName;
};

/* Concrete state */
/* Red indicates that account is overdrawn */
class RedState : IState
{
public:
  RedState(IState* state);
  ~RedState() {}
  void Deposit(double amount);
  void Withdraw(double amount);
  void PayInterest();
  void StateChangeCheck();
 
private:
  RedState(); //Not allowed
  void Initialize();
  double _serviceFee;
};

/* Silver indicates less interest bearing state */
class SilverState : IState
{
public:
  SilverState(IState* state);
  SilverState(double balance, Account* account);
  ~SilverState() {}
  void Deposit(double amount);
  void Withdraw(double amount);
  void PayInterest();
  void StateChangeCheck();
 
private:
  SilverState(); //Not allowed
  void Initialize();
};

/* Gold indicates high interest bearing state */
class GoldState : IState
{
public:
  GoldState(IState* state);
  ~GoldState() {}
  void Deposit(double amount);
  void Withdraw(double amount);
  void PayInterest();
  void StateChangeCheck();
 
private:
  GoldState(); //Not allowed
  void Initialize();
};

