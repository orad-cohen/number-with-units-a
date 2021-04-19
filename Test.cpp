#include "doctest.h"
#include "NumberWithUnits.hpp"

using namespace ariel;

#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <math.h>
using namespace std;


double generateRand(double min, double max){
    srand(time(NULL));
    double result = (double)rand();
    result = pow(result,10);
    double modifier = (max-min+1);
    return fmod(result,modifier)+min;


}


TEST_CASE("Check Pluses"){
    ifstream units("units.txt");
    NumberWithUnits::read_units(units);
    //Check the distance units;    
    NumberWithUnits aDis{12,"km"};
    NumberWithUnits bDis{3200,"m"};
    NumberWithUnits cDis{25,"cm"}; 
    NumberWithUnits eDis= aDis+bDis;
    CHECK_EQ((aDis+bDis),NumberWithUnits("15.2[km]"));
    CHECK_EQ(eDis,NumberWithUnits("15.2[km]"));
    CHECK_EQ((bDis+cDis),NumberWithUnits("3200.25[m]"));
    CHECK_EQ((cDis+aDis),NumberWithUnits("1200025[cm]"));
    CHECK_EQ((aDis+cDis),NumberWithUnits("12.00025[km]"));
    CHECK_EQ((cDis+cDis),NumberWithUnits("50[cm]"));
    CHECK_EQ((cDis+bDis),NumberWithUnits("320025[cm]"));
    aDis++;
    bDis++;
    cDis++;    
    CHECK_EQ(aDis,NumberWithUnits("13[km]"));
    CHECK_EQ(bDis,NumberWithUnits("3201[m]"));
    CHECK_EQ(cDis,NumberWithUnits("26[cm]"));
    aDis+=bDis;
    bDis+=cDis;
    cDis+=aDis;
    CHECK_EQ(aDis,NumberWithUnits("16.201[km]"));
    CHECK_EQ(bDis,NumberWithUnits("3201.26[m]"));
    CHECK_EQ(cDis,NumberWithUnits("1620126[cm]"));
    //Check the time units;
    NumberWithUnits aTime{36,"hour"};
    NumberWithUnits bTime{420,"min"};
    NumberWithUnits cTime{4620,"sec"};

    CHECK_EQ((aTime+bTime),NumberWithUnits("43[hour]"));
    CHECK_EQ((bTime+cTime),NumberWithUnits("497[min]"));
    CHECK_EQ((cTime+aTime),NumberWithUnits("134220[sec]"));
    ++aTime;
    ++bTime;
    ++cTime;
    CHECK_EQ(aTime,NumberWithUnits("37[hour]"));
    CHECK_EQ(bTime,NumberWithUnits("421[min]"));
    CHECK_EQ(cTime,NumberWithUnits("4621[sec]"));
    aTime+=bTime;
    bTime+=cTime;
    cTime+=aTime;

    //Check the Weight units;
    NumberWithUnits aWeight{216,"kg"};
    NumberWithUnits bWeight{902,"g"}; 
    NumberWithUnits cWeight{16,"ton"};

    CHECK_EQ((aWeight+bWeight),NumberWithUnits("216.90200[kg]"));
    CHECK_EQ((bWeight+cWeight),NumberWithUnits("16000902[g]"));
    CHECK_EQ((cWeight+aWeight),NumberWithUnits("16.216[ton]"));
    
    CHECK_EQ(aWeight*2,NumberWithUnits("432[kg]"));
    CHECK_EQ(3*bWeight,NumberWithUnits("2706[g]"));
    CHECK_EQ(2*cWeight*3,NumberWithUnits("96[ton]"));
    aWeight-=bWeight;
    bWeight-=cWeight;
    cWeight-=aWeight;
    CHECK_EQ(aWeight,NumberWithUnits("215.098[kg]"));
    CHECK_EQ(bWeight,NumberWithUnits("-15999098[g]"));
    CHECK_EQ(cWeight,NumberWithUnits("15.784902[ton]"));
    //Check currency Units
    NumberWithUnits NIS {333,"ILS"};
    NumberWithUnits Dolars {15,"USD"};
    CHECK_EQ(Dolars+NIS,NumberWithUnits("115[USD]"));
    CHECK_EQ(NIS+Dolars,NumberWithUnits("382.95[ILS]"));
    NIS--;
    --Dolars;
    CHECK_EQ(NIS,NumberWithUnits("332[ILS]"));
    CHECK_EQ(Dolars,NumberWithUnits("14[USD]"));
    NIS+=Dolars;
    Dolars+=NIS;
    CHECK_EQ(NIS,NumberWithUnits("378.62[ILS]"));
    CHECK_EQ(Dolars,NumberWithUnits("127.699699699699[USD]"));//string percision issue?


}
TEST_CASE("Check Chaining"){
    NumberWithUnits aDis{12,"km"};
    NumberWithUnits bDis{3200,"m"};
    NumberWithUnits cDis{25,"cm"};
    NumberWithUnits DisSum = aDis+bDis+cDis;
 
    CHECK_EQ(DisSum,NumberWithUnits{15.20025,"km"});
    CHECK_EQ(6*bDis-aDis-cDis,NumberWithUnits{7199.75,"m"});

    NumberWithUnits aTime{36,"hour"};
    NumberWithUnits bTime{420,"min"};
    NumberWithUnits cTime{4620,"sec"};

    CHECK_EQ(aTime-(-bTime)*2,NumberWithUnits{50,"hour"});
    CHECK_EQ(cTime+bTime-aTime*0.5*0.5*0.33,NumberWithUnits{19128,"sec"});


}

TEST_CASE("Check Throws"){
    NumberWithUnits aDis{12,"km"};
    NumberWithUnits bDis{22,"m"};
    NumberWithUnits aTime{36,"hour"};

    NumberWithUnits aWeight{216,"kg"};
    
    NumberWithUnits NIS {2,"ILS"};
   
    CHECK_THROWS(aDis+aTime);
    CHECK_THROWS(aTime+aWeight);
    CHECK_THROWS(aWeight+NIS);
    CHECK_THROWS(aDis+bDis+aTime);

}

TEST_CASE("Check Booleans"){
    NumberWithUnits aWeight{200,"kg"};
    NumberWithUnits bWeight{902,"g"}; 
    NumberWithUnits cWeight{16,"ton"};
    CHECK_FALSE(aWeight>cWeight);
    CHECK(bWeight<=aWeight);
    CHECK(cWeight==(aWeight*80));
    CHECK(cWeight<NumberWithUnits{17,"ton"});
    CHECK(cWeight+aWeight>=bWeight);

    
}
TEST_CASE("Test input/output operators"){
    istringstream TestA{"1200[USD] "};
    istringstream TestB{" 32 [km] "};
    istringstream TestC{" 55[min]"};
    istringstream TestD{"66[ton] "};
    NumberWithUnits A(TestA.str());
    CHECK_EQ(A,NumberWithUnits{1200,"USD"});
    TestB>>A;
    CHECK_EQ(A,NumberWithUnits{32,"km"});
    TestC>>A;
    CHECK_EQ(A,NumberWithUnits{55,"min"});
    TestD>>A;
    CHECK_EQ(A,NumberWithUnits{66,"ton"});
    stringstream C;
    C<<A;
    CHECK_EQ(A,C.str());

}
