/*           
 * Title        :   21. Strategy pattern
 * Filename     :   Strategy.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include <cassert>

/* Strategy */
class Record
{
public:
  virtual void StartRecord() = 0;
  virtual void StoreField(const std::string &name, const std::string &value) 
    = 0;
  virtual void FinishRecord() = 0;
  virtual ~Record() {}
};

struct ContactData
{
  std::string firstName, lastName, phone, email;
};

/* Context */
class ContactRecorder
{
public:
  ContactRecorder(Record *a) : _record(a)
  {
    assert(a != 0);
  }

  void Store(const ContactData &data)
  {
    assert(_record != 0);

    _record->StartRecord();
    _record->StoreField("first name", data.firstName);
    _record->StoreField("last name", data.lastName);
    _record->StoreField("phone", data.phone);
    _record->StoreField("email", data.email);
    _record->FinishRecord();
  }

private:
  Record* _record;
};

/* Concrete strategy */
class StreamRecord : public Record
{
public:
  StreamRecord(std::ostream &s, const std::string &record_name = std::string())
    : _ostream(s), _recordName(record_name)
  {
  }

  void StartRecord()
  {
    _ostream << _recordName << "[ ";
  }

  void StoreField(const std::string &name, const std::string &value)
  {
    _ostream << name << ": " << value << "; ";
  }

  void FinishRecord()
  {
    _ostream << "]" << std::endl;
  }

  void SetRecordName(const std::string &name)
  {
    _recordName = name;
  }

private:
  std::ostream &_ostream;
  std::string _recordName;
};

class MySql {};

class DatabaseRecord : public Record
{
public:
  DatabaseRecord() : _dbConnection(new MySql) {}
  ~DatabaseRecord() { delete _dbConnection; }

  void StartRecord()
  {
    std::cout << "start transaction" << std::endl;
  }

  void StoreField(const std::string &name, const std::string &value)
  {
    std::cout << "insert into table, " << name << ": " << value << std::endl;
  }

  void FinishRecord()
  {
    std::cout << "finish transaction" << std::endl;
  }

private:
    MySql *_dbConnection;
};
