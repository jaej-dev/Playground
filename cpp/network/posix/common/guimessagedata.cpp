#include "guimessagedata.h"
#include <iostream>
#include <iomanip>
#include <ctime>

namespace mgp = master_gui_protocol;
using namespace master_gui_protocol;
using std::cout;
using std::endl;

std::string MessageData::description() const
{
    std::stringstream ss;
    ss << "category: " << category_
       << ", lev: " << level_
       << ", icon: " << icon_
       << ", dur: " << duration_ << endl;
    ss << "msg: [" << message_ << "]" << endl;

    return ss.str();
}
