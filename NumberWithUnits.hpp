#include<unordered_map>
#include <iostream>
#include <fstream>


namespace ariel{
    
    class NumberWithUnits{
    private:

            
        
    protected:
        std::string Unit;        
        double Value;
        static bool init;
        static std::unordered_map<std::string,std::unordered_map<std::string,double>> ConvertionMap;
        static void placeholder(const std::string & val1,const std::string &val2, double mult);
        static double UnitConvert(const std::string&,const std::string&,double);

    public:
    
    
    static void PrintMap(){
        
    }
    NumberWithUnits(double,const std::string&);
    NumberWithUnits(const std::string&);
    static void read_units(std::ifstream&);

    const NumberWithUnits& operator-=(const NumberWithUnits&) ;
    const NumberWithUnits& operator+=(const NumberWithUnits&) ;

    const NumberWithUnits& operator ++(int) ;
    NumberWithUnits& operator++();

    const NumberWithUnits& operator --(int) ;
    NumberWithUnits& operator--();

    friend std::istream& operator>>(std::istream&, NumberWithUnits&);
    friend std::ostream& operator<<(std::ostream&, const NumberWithUnits&);

    NumberWithUnits operator*(double) const;
    friend NumberWithUnits operator*(double, const NumberWithUnits&);

    NumberWithUnits operator+(const NumberWithUnits&) const;
    NumberWithUnits& operator+() const;

    NumberWithUnits operator-(const NumberWithUnits&) const;
    NumberWithUnits operator-() const{
        NumberWithUnits a(-Value,Unit);
        return a;
    }

    friend bool operator==(const NumberWithUnits&,const NumberWithUnits&);

    friend bool operator<=(const NumberWithUnits&, const NumberWithUnits&);
    friend bool operator>=(const NumberWithUnits&, const NumberWithUnits&);
    friend bool operator>(const NumberWithUnits&,const NumberWithUnits&);
    friend bool operator<(const NumberWithUnits&,const NumberWithUnits&);

};






}

