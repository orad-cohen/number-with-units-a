#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace ariel;
const double percision = 10000.0;
    


std::unordered_map<std::string,std::unordered_map<std::string,double>> NumberWithUnits::ConvertionMap;
double RoundVal(double d){
  d=round(d*percision)/percision;
  return d;
}
NumberWithUnits::NumberWithUnits(const string &param){
    string temp;
    istringstream toStream(param);
    double d = 0;
    toStream>>d;
    Value =RoundVal(d);
    toStream>>temp;
    Unit = temp.substr(1,temp.size()-2);
    

}
NumberWithUnits::NumberWithUnits(double val, const string &unit){
    Value=RoundVal(val);
    Unit=unit;  
}




void NumberWithUnits::placeholder(const string &val1, const string &val2, double mult){
    ConvertionMap[val1][val2]=mult;
    
    for(auto const& Key:ConvertionMap[val1]){
      if(Key.first==val2){
        continue;
      }
      if(ConvertionMap[Key.first].find(val2)!=ConvertionMap[Key.first].end()){
        continue;
        
      }
      
      double temp = mult/Key.second;
      ConvertionMap[Key.first][val2] = temp;
      ConvertionMap[val2][Key.first] = 1/temp;
      }  
    
}

void NumberWithUnits::read_units(ifstream &file){
    
    string Line;
    string UnitA;
    string UnitB;
 
    double tempValue = 0;
  while (getline(file, Line)){
    UnitA = Line.substr(0,Line.find('='));
    UnitB = Line.substr(Line.find('=')+1);
    UnitA.erase(remove(UnitA.begin(), UnitA.end(), ' '),UnitA.end()); 
    UnitB.erase(remove(UnitB.begin(), UnitB.end(), ' '),UnitB.end());
    istringstream Astream(UnitA);
    istringstream Bstream(UnitB);
    Astream >> tempValue;
    Astream >> UnitA;
    Bstream >> tempValue;
    Bstream >> UnitB;
    placeholder(UnitA,UnitB,tempValue);
    placeholder(UnitB,UnitA,1/tempValue);
  }
  /*for(auto const& Key:ConvertionMap){
      for(auto const& KeyTwo:Key.second){
        cout<<Key.first << " : " << KeyTwo.first << " : " << KeyTwo.second << endl;
      }
  }*/



}

double NumberWithUnits::UnitConvert(const string &from,const string &to, double value){
  if(from==to){
    return value;
  }
  if(NumberWithUnits::ConvertionMap[from].find(to)==NumberWithUnits::ConvertionMap[from].end()){
    
    throw invalid_argument("Units are not convertable.");
  }
  //cout << from << " : " << to << " : " << ConvertionMap[from][to]<<endl;
  return RoundVal(ConvertionMap[from][to]*value);

}
const NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& b){
  Value -= UnitConvert(b.Unit,Unit,b.Value);
  return *this;  
}
const NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& b){
  Value+=UnitConvert(b.Unit,Unit,b.Value);
  return *this;
}

const NumberWithUnits& NumberWithUnits::operator++(int flag){
  Value++;
  return *this;
}
NumberWithUnits& NumberWithUnits::operator++(){
  Value++;
  return *this;
}
const NumberWithUnits& NumberWithUnits::operator--(int flag) {
  Value--;
  return *this;
}
NumberWithUnits& NumberWithUnits::operator--(){
  Value--;
  return *this;
}
NumberWithUnits NumberWithUnits::operator*(double d) const{
  return NumberWithUnits(Value*d,Unit);
}
NumberWithUnits ariel::operator*(double d,const NumberWithUnits& b){
  return NumberWithUnits(b.Value*d,b.Unit);
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& b) const{
  return NumberWithUnits(Value+UnitConvert(b.Unit,Unit,b.Value),Unit);
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& b) const{
  return NumberWithUnits(Value-UnitConvert(b.Unit,Unit,b.Value),Unit);
}

bool ariel::operator==(const NumberWithUnits& a, const NumberWithUnits& b){
  double tempVal = b.UnitConvert(b.Unit,a.Unit,b.Value);
  return a.Value==tempVal;

}

bool ariel::operator<=(const NumberWithUnits&a , const NumberWithUnits& b){
  double tempVal = a.UnitConvert(b.Unit,a.Unit,b.Value);
  return a.Value<=tempVal;

}
bool ariel::operator>=(const NumberWithUnits&a , const NumberWithUnits& b){
  double tempVal = a.UnitConvert(b.Unit,a.Unit,b.Value);
  return a.Value>=tempVal;

}
bool ariel::operator>(const NumberWithUnits& a ,const NumberWithUnits& b){
  double tempVal = a.UnitConvert(b.Unit,a.Unit,b.Value);
  return a.Value>tempVal;
}
bool ariel::operator<(const NumberWithUnits& a,const NumberWithUnits& b){
  double tempVal = a.UnitConvert(a.Unit,b.Unit,b.Value);
  return a.Value<tempVal;
}
std::istream& ariel::operator>>(istream& is, NumberWithUnits &a){
    string temp;
    std::string s(std::istreambuf_iterator<char>(is), {});

    s.erase(remove(s.begin(), s.end(), ' '),s.end());
 
    temp = s.substr(0,s.find('['));
    a.Value = stod(temp);
    a.Unit= s.substr(s.find('[')+1,s.size()-s.find('[')-2);
    

    return is;

  }
  std::ostream& ariel::operator<<(std::ostream& os ,const NumberWithUnits &a){

    return os << a.Value <<"["<<a.Unit<<"]";
  }

