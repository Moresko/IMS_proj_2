//#######################################################
// Project: 2 SHO
// File: main.cpp
// Date: 22.11.2023
// Author: Martin Mores xmores02
//
// Description: Simulation
//#######################################################
#include <iostream>
#include <string>
#include <simlib.h>
#include <ctime>

using namespace std;

// dlzka platby a vyber kebabu
#define PAYMENT_KEBBAB_CHOSING 1 

// dlzka vytvarania kebabu
#define KEBBAB_MAKKING 4 

// cas prichodu zakaznikov
#define CUSTOMER_COMING 3 

// cakacia doba
#define WAIT_FOR_SERVICE 1 

// Ked vo firme robi jeden zamestnanec
const int numberOfServicesOne = 1;  

// Ked vo firme robia dvaja zamestnanci
const int numberOfServicesTwo = 2;   

// Ked vo firme robia traja zamestnanci
const int numberOfServicesThree = 3;   

//zariadenia - ktore urcuju, kolko ludi pracuje momentalne vo firme
Facility serviceOne[numberOfServicesOne];
Facility serviceTwo[numberOfServicesTwo];
Facility serviceThree[numberOfServicesThree];

//statistiky pre urcenie, kolko ludi pride za den do bistra podla toho kolko ludi robi
Stat customerServedOne;
int customersNumberOne = 0;

Stat customerServedTwo;
int customersNumberTwo = 0;

Stat customerServedThree;
int customersNumberThree = 0;

//trieda ked pracuje jeden clovek
class ServiceOne : public Process
{
public:
    void Behavior()
    {   
        int breakin = 0;
        while(true)
        {      
            //cyklus, ktory konroluje ci je k dispozicii nejaky  zamestnanec a ak je a uz skomcil z obsluhou tak vyjde z cyklu
            for(int i = 0; i < numberOfServicesOne; i++)
            {
                if(!serviceOne[i].Busy())
                {   
                    Seize(serviceOne[i]);
                    Wait(PAYMENT_KEBBAB_CHOSING);
                    breakin = i + 1;
                    Wait(KEBBAB_MAKKING);
                    Release(serviceOne[i]);
                    
                    break;
                }

            }

            //ak uz bol obsluzeny zakaznik tak odchadza zo systemu a pricita sa zakaznik do statistik
            if(breakin > 0 && breakin <= numberOfServicesOne)
            {   
                customersNumberOne++;   
                customerServedOne(customersNumberOne);

                break;
            }
            else
            {   
                //ak este nebol obsluzeny tak caka vo fronte a zasa sa pozrie ci uz moze byt obsluzeny
                Wait(Exponential(WAIT_FOR_SERVICE));
            }
        }
    }
};

//trieda ked pracuju dvaja ludia
class ServiceTwo : public Process
{
public:
    void Behavior()
    {   
        int breakin = 0;
        while(true)
        {      
            //cyklus, ktory konroluje ci je k dispozicii nejaky  zamestnanec a ak je a uz skomcil z obsluhou tak vyjde z cyklu
            for(int i = 0; i < numberOfServicesTwo; i++)
            {
                if(!serviceTwo[i].Busy())
                {   
                    Seize(serviceTwo[i]);
                    Wait(PAYMENT_KEBBAB_CHOSING);
                    breakin = i + 1;
                    Wait(KEBBAB_MAKKING);
                    Release(serviceTwo[i]);
                    
                    break;
                }

            }

            //ak uz bol obsluzeny zakaznik tak odchadza zo systemu a pricita sa zakaznik do statistik
            if(breakin > 0 && breakin <= numberOfServicesTwo)
            {   
                customersNumberTwo++;   
                customerServedTwo(customersNumberTwo);

                break;
            }
            else
            {   
               //ak este nebol obsluzeny tak caka vo fronte a zasa sa pozrie ci uz moze byt obsluzeny
                Wait(Exponential(WAIT_FOR_SERVICE));
            }
        }
    }
};

//trieda ked pracuju traja ludia
class ServiceThree : public Process
{
public:
    void Behavior()
    {   
        int breakin = 0;
        while(true)
        {      
            //cyklus, ktory konroluje ci je k dispozicii nejaky  zamestnanec a ak je a uz skomcil z obsluhou tak vyjde z cyklu
            for(int i = 0; i < numberOfServicesThree; i++)
            {
                if(!serviceThree[i].Busy())
                {   
                    Seize(serviceThree[i]);
                    Wait(PAYMENT_KEBBAB_CHOSING);
                    breakin = i + 1;
                    Wait(KEBBAB_MAKKING);
                    Release(serviceThree[i]);
                    
                    break;
                }

            }

            //ak uz bol obsluzeny zakaznik tak odchadza zo systemu a pricita sa zakaznik do statistik
            if(breakin > 0 && breakin <= numberOfServicesThree)
            {   
                customersNumberThree++;   
                customerServedThree(customersNumberThree);

                break;
            }
            else
            {   
                //ak este nebol obsluzeny tak caka vo fronte a zasa sa pozrie ci uz moze byt obsluzeny
                Wait(Exponential(WAIT_FOR_SERVICE));
            }
        }
    }
};


//generoavnie zakaznikov
class CustomerGenerator : public Event
{
    void Behavior()
    {   
        //trieda, ktora urcuje prichod zakaznikov do predajne kababu, zakaznici prichadzaju s exponencialnym rozdelenym 3min
        (new ServiceOne) -> Activate();
        (new ServiceTwo) -> Activate();
        (new ServiceThree) -> Activate();
        Activate(Time + Exponential(CUSTOMER_COMING));
    }
};

/**
 * @brief funckia na urcenie najvacsieho profitu
 * 
 * @param one profit ked pracuje jeden zamestnanec
 * @param two profit ked pracuju dvaja za,estnanci
 * * @param three profit ked pracuju traja zamestnanci
 */
int biggestPorfit(int one, int two, int three)
{
    int biggest = one;
    if(one < two)
    {
        biggest = two;
    }

    if(biggest < three)
    {
        biggest = three;
    }

    return biggest;
}

/**
 * @brief hlavna funkcia programu
 * 
 * 
 * @return 0 ak sa prelozi a uspesne skonci program
 */

int main()
{   
    

    cout << "---------------------------------------------------------------------" << endl
            << "SIMULATION START" << endl
            << "---------------------------------------------------------------------" << endl 
            << "Running experiment with " << numberOfServicesOne << " and " << numberOfServicesTwo <<  " and " << numberOfServicesThree << " services." << endl <<endl;

            
    //simulacia trva 360 minut  
    Init(0,360);

    (new CustomerGenerator) -> Activate();

    Run();

    //vytlacenie statistik podla jednotlivych situaciach
    customerServedOne.Output();
    customerServedTwo.Output();
    customerServedThree.Output();

    // rozdelenie ludi podla toho ci maju ISIC alebo nemaju ISIC
    int isicOne = customersNumberOne * 0.055 * 90;
    int notIsicOne = customersNumberOne * 0.945 * 100;

    int isicTwo = customersNumberTwo * 0.055 * 90;
    int notIsicTwo = customersNumberTwo * 0.945 * 100;

    int isicThree = customersNumberThree * 0.055 * 90;
    int notIsicThree = customersNumberThree * 0.945 * 100;

    int wholeRewOne = isicOne + notIsicOne;
    int wholeRewTwo = isicTwo + notIsicTwo;
    int wholeRewThree = isicThree + notIsicThree;

    // celkovy profit za jeden den
    int sumOne = wholeRewOne - numberOfServicesOne * 767;
    int sumTwo = wholeRewTwo - numberOfServicesTwo * 767;
    int sumThree = wholeRewThree - numberOfServicesThree * 767;

    printf("Obrat za 1 deň s jednym zamestnancom: %d Kc\n", wholeRewOne);
    printf("Profit za 1 den s jednym zamestnancom: %d Kc\n", sumOne );

    printf("Obrat za 1 deňs s dvomi zamestnancami: %d Kc\n", wholeRewTwo);
    printf("Profit za 1 den s dvomi zamestnancami: %d Kc\n", sumTwo );

    printf("Obrat za 1 deňs s tromi zamestnancami: %d Kc\n", wholeRewThree);
    printf("Profit za 1 den s tromi zamestnancami: %d Kc\n", sumThree);

    //najvacsi profit z jednotlivych situacii
    int biggestRew = biggestPorfit(sumOne, sumTwo, sumThree);
    int bestPos;

    //vypis kolko ludi robili za pultom ked bol najvacsi profit
    if(biggestRew == sumOne)
    {
        bestPos = numberOfServicesOne;
    }
    else if(biggestRew == sumTwo)
    {
        bestPos = numberOfServicesTwo;
    }
    else if(biggestRew == sumThree)
    {
        bestPos = numberOfServicesThree;
    }

    printf("najvacsi profit je: %d keď robi/robia: %d zamestnanec/zamestnanci\n", biggestRew, bestPos);

    cout << "---------------------------------------------------------------------" << endl
            << "SIMULATION END" << endl
            << "---------------------------------------------------------------------" << endl << endl;

    return 0;

}