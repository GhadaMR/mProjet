#include <bits/stdc++.h>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
class Date{

     int jour;
     int mois;
     int annee;

    public:
     vector<int>v30={1,3,5,7,8,10,12};
     vector<int> v31={4,6,9,11};
     string incrimenterDate();
     string afficherDate ();
     void setJour(int j);
     void setMois(int m);
     void setAnnee(int a);
     Date (string d);
     int getJour();
     int getMois();
     int getAnnee();

};
int Date::getJour()
{
    return jour;
}
int Date::getMois()
{
    return mois;
}
int Date::getAnnee()
{
    return annee;
}

void Date::setMois(int m)
{
 mois=m;
}
void Date::setJour(int j)
{
 jour=j;
}
void Date::setAnnee(int a)
{
 annee=a;
}
string Date::incrimenterDate() {
    Date d = *this; // make a copy of the current object
    if (mois != 12) {
        if (jour == 30) {
            if (find(v30.begin(),v30.end(),mois) != v30.end()) {
                d.setMois(mois+1);
                d.setJour(1);
            }
        }
        else if (find(v31.begin(), v31.end(), mois) != v31.end()) {
            d.setJour(jour+1);
        }
        else {
            d.setJour(jour+1);
        }
    }
    else if (mois==2){
            if(jour==28){
                if (annee % 4 == 0 && annee % 100 != 0 || annee % 400 == 0)
                        d.setJour(29);
                else{
                    d.setJour(1);
                    d.setMois(3);
                }
            }
          else
            d.setJour(jour+1);
    }
   else{
     if (jour==30){
            d.setMois(1);
            d.setJour(1);
            d.setAnnee(annee+1);
            }
    else
        d.setJour(jour+1);
   }

    string d1 = to_string(d.getJour()) + "/" + to_string(d.getMois()) + "/" + to_string(d.getAnnee());

    return d1;
}


Date::Date(string d){
    istringstream ss(d);
    string j, m, a;
    getline(ss, j, '/');
    getline(ss, m, '/');
    getline(ss, a);
    int jr = std::stoi(j);
    int ms = std::stoi(m);
    int an = std::stoi(a);
    jour=jr;
    mois=ms;
    annee=an;
}



int main()
{
    Date d("30/12/2022");
    cout<<d.incrimenterDate() <<endl;


    return 0;
}