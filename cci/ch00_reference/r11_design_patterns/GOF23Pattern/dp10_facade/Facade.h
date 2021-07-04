/*           
 * Title        :   10. Fasade
 * Filename     :   Facade.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>

 
/* Clinet */
class Customer
{
public:
  Customer(const std::string& name) : _name(name)
  {
    std::cout << "Customer::Customer: added " << _name << std::endl;
  }
  const std::string& GetName(void)
  {
    return _name;
  }

private:
  Customer(); //not allowed
  std::string _name;
};
 
/* Subsystem ClassA */
class Bank
{
public:
  bool HasSufficientSavings(Customer* c, int amount)
  {
    std::cout << "Bank::HasSufiicientSavings: Check bank for " << c->GetName() 
      << std::endl;
    return true;
  }
};
 
/* Subsystem ClassB */
class Credit
{
public:
  bool HasGoodCredit(Customer* c, int amount)
  {
    std:: cout << "Credit::HadGoodCredit: Check credit for " << c->GetName() 
      << std::endl;
    return true;
  }
};
 
/* Subsystem ClassN */
class Loan
{
public:
  bool HasGoodCredit(Customer* c, int amount)
  {
    std:: cout << "Load::HadGoodCredit: Check loans for " << c->GetName()
      << std::endl;
    return true;
  }
};
 
/* Facade class */
class Mortgage
{
public:
  bool IsEligible(Customer* c, int amount)
  {
    bool eligible = true;
    eligible = _bank.HasSufficientSavings(c, amount);
 
    if(eligible)
      eligible = _loan.HasGoodCredit(c, amount);
 
    if(eligible)
      eligible = _credit.HasGoodCredit(c, amount);

    std::cout << "Mortgage::IsEligible: " << c->GetName() 
      << " applies for a loan for $" << amount << std::endl;
 
    return eligible;
  }
 
private:
  Bank _bank;
  Loan _loan;
  Credit _credit;
};
