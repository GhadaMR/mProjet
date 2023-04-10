#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;
class Date{
     int jour;
     int mois;
     int annee;
    public:
     void incrimenterDate(Date& d);
     void afficherDate();
     Date (string d);
};

void Date:: incrimenterDate(Date& d)
{
    if (d.mois &&d.jour &&d.annee){
    int limiteJour = 31;
    if (d.mois == 2) {
        if (d.annee % 4 == 0) {
            limiteJour = 29;
        } else {
            limiteJour = 28;
        }
    } else if (d.mois == 4 || d.mois == 6 || d.mois == 9 || d.mois == 11) {
        limiteJour = 30;
    }
    d.jour++;
    if (d.jour > limiteJour) {
        d.jour = 1;
        d.mois++;
    }
    if (d.mois > 12) {
        d.jour=1;
        d.mois = 1;
        d.annee++;
    }
    }
}
Date::Date(string d){
    istringstream ss(d);
    string j, m, a;
    getline(ss, j, '/');
    getline(ss, m, '/');
    getline(ss, a);
    int jr = stoi(j);
    int ms = stoi(m);
    int an = stoi(a);
    jour=jr;
    mois=ms;
    annee=an;
    int limiteJour = 31;
    if (mois == 2) {
        if (annee % 4 == 0) {
            limiteJour = 29;
        } else {
            limiteJour = 28;
        }
    } else if (mois == 4 || mois == 6 || mois == 9 || mois == 11) {
        limiteJour = 30;
    }
    if (jour> limiteJour){
        jour=00;
        mois=00;
        annee=0000;

    }
}
void Date::afficherDate(){
    cout<<jour<<"/"<<mois<<"/"<<annee<<endl;
}


int main()
{

    Date d("31/7/2020");
    d.afficherDate();
    d.incrimenterDate(d);
    d.afficherDate();

    return 0;
}
