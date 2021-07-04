/*
 * deque.cc: 
 */

#include <iostream>
#include <deque>
#include <cstring>

namespace test {
typedef struct _packet {
  int index;
  int size;
  char raw[128];
} packet;  
}  //namespace test

int main(void)
{
  test::packet pkt1;
  pkt1.index = 1;
  std::strcpy(pkt1.raw, "aaa");
  pkt1.size = sizeof(pkt1.raw);

  test::packet pkt2;
  pkt2.index = 2;
  std::strcpy(pkt2.raw, "bbb");
  pkt2.size = sizeof(pkt2.raw);

  test::packet pkt3;
  pkt3.index = 3;
  std::strcpy(pkt3.raw, "ccc");
  pkt3.size = sizeof(pkt3.raw);

  std::deque<test::packet> receive_packets;

  // 1st push 
  receive_packets.push_back(pkt2); 
  receive_packets.push_back(pkt3);
  receive_packets.push_front(pkt1);

  // 1st print
  for(std::deque<test::packet>::iterator it = receive_packets.begin(); it != receive_packets.end(); ++it) {
    std::cout << "origin packets: " << "index(" << it->index << ") ";
    std::cout << "size(" << it->size << ") ";
    std::cout << "raw(" << it->raw << ")\n";
  }

  // insert recive_packets_2nd into recive_packets
  std::deque<test::packet> receive_packets_2nd;
  
  test::packet pkt4;
  pkt4.index = 4;
  std::strcpy(pkt4.raw, "ddd");
  pkt4.size = sizeof(pkt4.raw);

  test::packet pkt5;
  pkt5.index = 5;
  std::strcpy(pkt5.raw, "eee");
  pkt5.size = sizeof(pkt5.raw);

  // 2nd push 
  receive_packets_2nd.push_front(pkt5);
  receive_packets_2nd.push_front(pkt4);
  
  receive_packets.insert(receive_packets.end(), receive_packets_2nd.begin(), receive_packets_2nd.end());
  
  // 2nd print
  for(std::deque<test::packet>::iterator it = receive_packets.begin(); it != receive_packets.end(); ++it) {
    std::cout << "merged packets: " << "index(" << it->index << ") ";
    std::cout << "size(" << it->size << ") ";
    std::cout << "raw(" << it->raw << ")\n";
  }

}
