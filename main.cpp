#include <iostream>
#include <string>
#include<vector>
#include<fstream>
using namespace std;

class Date{
     int jour;
     int mois;
     int annee;
    public:
     void incrementerDate();
     Date (int j,int m,int a);
     Date(){};
     friend ostream& operator<<(ostream& flux, const Date& d);
     friend istream& operator>>(istream& flux, Date& d);
     bool operator==(const Date& d)const;
     bool operator<(const Date& d)const;
};
ostream& operator<<( ostream& flux, const Date& d){
    flux<<d.jour<<"/"<<d.mois<<"/"<<d.annee<<endl;
    return flux;
}
istream& operator>>(istream& flux, Date& d){
        flux>>d.jour>>d.mois>>d.annee;
    return flux;
}
bool Date::operator==(const Date& d)const{
   return (jour==d.jour &&mois==d.mois && annee==d.annee);
   }
bool Date::operator< (const Date& d)const{
   return (d.annee<annee || (d.annee==annee && d.mois<mois) || (d.annee==annee && d.mois==mois && d.jour<jour));
}
void Date:: incrementerDate()
{
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
    jour++;
    if (jour > limiteJour) {
        jour = 1;
        mois++;
    }
    if (mois > 12) {
        jour=1;
        mois = 1;
        annee++;
    }
}
Date::Date(int j,int m,int a):jour(j), mois(m),annee(a){}



class PrixJournalier {
    private:
        Date date;
        string action;
        double prix;
    public:
        PrixJournalier(){};
        friend istream& operator>>(istream& flux, PrixJournalier& pj) ;
        friend ostream& operator<<(ostream& flux, const PrixJournalier& pj) ;
        friend class BourseVector;
};
istream& operator>>(istream& flux, PrixJournalier& pj) {
            flux >> pj.date >> pj.action >> pj.prix;
            return flux;
        }
ostream& operator<<(ostream& flux, const PrixJournalier& pj) {
            flux << pj.date <<"   "<< pj.action << "  \n "<<pj.prix<<endl;
            return flux;
        }


class PersistancePrixJournaliers
{
    public:
        static vector<PrixJournalier> lirePrixJournaliersDUnFichier(string chemin){
            vector<PrixJournalier> historique;
            ifstream f(chemin);
            int nbLignes= 0;
            string entete;
            if(f.is_open()){
                f>>entete;
                while(!f.eof()){
                    PrixJournalier pj;
                    f>>(pj);
                    historique.push_back(pj);
                    nbLignes++;
                }
            }
            return historique;
        }
};

class Bourse{
 protected:
    Date dateAujoudhui;
 public:
    virtual vector<string> getActionsDisponiblesParDate(Date date)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParDate(Date date)=0;
};

class BourseVector: public Bourse
{
 private:
    vector<PrixJournalier> prixj;
 public:
    BourseVector(vector<PrixJournalier> pj) : prixj(pj) {}

    vector<string> getActionsDisponiblesParDate(Date date)
    {
        vector<string> actions;
        for (int i; i<prixj.size();i++)
        {
            if (prixj[i].date == date)
            {
                actions.push_back(prixj[i].action);
            }
        }
        return actions;
    }

    vector<PrixJournalier> getPrixJournaliersParDate(Date date)
    {
        vector<PrixJournalier> prix_journaliers;
        for (int i; i<prixj.size();i++)
        {
            if (prixj[i].date == date)
            {
                prix_journaliers.push_back(prixj[i]);
            }
        }
        return prix_journaliers;
    }
};


int main()
{
    Date d1(31,7,2014);
    Date d2(8,8,2014);
    d1.incrementerDate();
    cout << d1 << endl;
    Date d3;
    cin>>d3;
    cout << d3 << endl;
    PrixJournalier pj1;
    cin>>pj1;
    cout<< pj1;
    PrixJournalier pj2;
    cin>>pj2;
    cout<< pj2;
    vector<PrixJournalier> historique = PersistancePrixJournaliers::lirePrixJournaliersDUnFichier("C:/Users/hp/Desktop/enit/S2/MP/prices_simple.csv");
    BourseVector  bourse(historique);
    vector<PrixJournalier> PrixJournaliersParDate= bourse.getPrixJournaliersParDate(d1);
    for (int i; i<PrixJournaliersParDate.size();i++)
              cout<<PrixJournaliersParDate[i];
    vector<string> actionsDisponibles= bourse.getActionsDisponiblesParDate(d2);
    for (int i; i<actionsDisponibles.size();i++)
                cout<<actionsDisponibles[i];
    return 1;
}
