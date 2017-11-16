#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace std;

class Table {
	public:
    struct sockaddr_in destination;    //destination --IP address
    struct sockaddr_in mask;           //32 bits string --not an IP address
    struct sockaddr_in gateway;        //next hop --next router IP
    string interface;      //exit --port in which the gateway
                           //is executing
} table;

void convert_input(string str, string sv[])
{
  string::size_type next;

  char strVec0[20], strVec1[20], strVec2[20], strVec3[20];

  next = str.find("/");

  str.copy(strVec0, next);
  str.erase(str.begin(), str.begin()+next+1);
  next = str.find("/");

  str.copy(strVec1, next, 0);
  strVec1[next] = '\0';
  str.erase(str.begin(), str.begin()+next+1);
  next = str.find("/");

  str.copy(strVec2, next);
  strVec2[next] = '\0';
  str.erase(0, next+1);

  str.copy(strVec3, 4); // eh 4 msm?
  strVec3[4] = '\0';

  //string sv[4];
  sv[0].assign(strVec0);
  sv[1].assign(strVec1);
  sv[2].assign(strVec2);
  sv[3].assign(strVec3);
  //cout << sv[0] << endl;

  //return sv;
}

string convert_mask(string mask)
{
  int mask_slash, pot=8, ap=0;

  //mask.erase(mask.begin());
  mask_slash = atoi(mask.c_str());
  mask = "";
  while(mask_slash >= 8)
  {
    mask.append("255.");
    mask_slash -= 8;
  }
  if(mask_slash != 0)
  {
    while(mask_slash != 0){
      pot        -= 1;
      mask_slash -= 1;
      ap += int(pow(2,pot));
    }
    
    mask.append(to_string(ap));
    mask.append(".");
  }
  while(count(mask.begin(),mask.end(), '.') <= 3){
    mask.append("0.");

  }
  mask.pop_back();

  return mask;
}

int main(int argc, char* argv[])
{
  string destination, mask, gateway, interface;
  string input[4];

  convert_input(argv[1], input);

  destination = input[0];
	inet_aton(destination.c_str(), &table.destination.sin_addr);
  mask = input[1];
  if(mask.find('.') == string::npos)
  {
    mask = convert_mask(mask);
  }
  gateway = input[2];
  interface = input[3];
	return 0;
}
