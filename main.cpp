#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include<chrono>
#include<map>

using namespace std;

class Date{
    public:
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
    char tab[100];
    flux.getline(tab, 100, '/');
    int jour = atoi(tab);

    flux.getline(tab, 100, '/');
    int mois = atoi(tab);

    flux.getline(tab, 100,' ');
    int annee = atoi(tab);

    d = Date(jour, mois, annee);
    flux.ignore();
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

        Date date;
        string action;
        double prix;
    public:
        PrixJournalier(){};
        friend istream& operator>>(istream& flux, PrixJournalier& pj) ;
        friend ostream& operator<<(ostream& flux, const PrixJournalier& pj) ;
        Date& getDate(){return date;};
        string& getAction(){return action;};
        double& getPrix(){return prix;};



};

ostream& operator<<(ostream& flux, const PrixJournalier& pj) {
            flux << pj.date <<"   "<< pj.action << "  \n "<<pj.prix<<endl;
            return flux;
        }

istream& operator>>(istream& flux, PrixJournalier& pj) {
    string ligne;
    if (getline(flux, ligne)) {
        stringstream ss(ligne);
        string cell;
        getline(ss, cell, '/');
        pj.getDate().jour = stoi(cell);
        getline(ss, cell, '/');
        pj.getDate().mois = stoi(cell);
        getline(ss, cell, ';');
        pj.getDate().annee = stoi(cell);
        getline(ss, cell, ';');
        pj.getAction() = cell;
        getline(ss, cell);
        pj.getPrix ()= stod(cell);
    }
    return flux;
}

class PersistancePrixJournaliers {
public:
    static vector<PrixJournalier> lirePrixJournaliersDUnFichier(string chemin) {
        vector<PrixJournalier> historique;
        ifstream f(chemin);
        int nbLignes = 0;
        string entete;
        if (f.is_open()) {
            getline(f, entete);
            while (!f.eof()) {
                PrixJournalier pj;
                f >> pj;
                historique.push_back(pj);
                nbLignes++;
            }
        }
        f.close();
        return historique;
    }
};

class Bourse{
 protected:
    Date dateAujourdhui;
 public:
    Date getDateAujourdhui() { return dateAujourdhui;}
    void setDateAujourdhui(Date date){dateAujourdhui=date;}
    virtual vector<PrixJournalier> getPrixJournaliersDisponiblesParAujourdhui()=0;
    virtual vector<string> getActionsDisponiblesParAujourdhui()=0;
    virtual vector<string> getActionsDisponiblesParDate(Date date)=0;
    virtual vector<string> getActionsDisponiblesParPrix(double prix)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParAction(string action)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParDate(Date date)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParPrix(double prix)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParPrixAujourdhui(double prix)=0;
    virtual ~Bourse(){};

};

class BourseVector: public Bourse
{
 private:
    vector<PrixJournalier> prixj;
 public:



    BourseVector( vector<PrixJournalier> historique) : prixj(historique) {};
    vector<PrixJournalier> getPrixjournaliers(){
        return prixj;
    }
    vector<string> getActionsDisponiblesParPrix(double prix)
        {
        vector<PrixJournalier> prj =getPrixJournaliersParPrix(prix);
        vector<string> actionsParPrix;
        for(auto i : prj)
        {
                actionsParPrix.push_back(i.getAction());
        }
        return actionsParPrix;

}

    vector<string> getActionsDisponiblesParAujourdhui()
    {
        vector<PrixJournalier> prjr=getPrixJournaliersDisponiblesParAujourdhui();
        vector<string> actionsAujourdhui;
        for(auto i : prjr){
                actionsAujourdhui.push_back(i.getAction());
                  }
        return actionsAujourdhui;
    }
    vector<string> getActionsDisponiblesParDate(Date date)
    {
        vector<PrixJournalier> prj =getPrixJournaliersParDate(date);
        vector<string> actions;
        int i=0;

        for (auto i : prj)
        {
                actions.push_back(i.getAction());
            }
        return actions;

}
    vector<PrixJournalier> getPrixJournaliersParDate(Date date)
    {
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()&& date < dateAujourdhui){
         while ( prixj.size()>i)
        {
            if (prixj[i].getDate() == date)
            {
                prix_journaliers.push_back(prixj[i]);
            }
             i++;
        }
        return prix_journaliers;
    }
    }
  vector<PrixJournalier> getPrixJournaliersParAction(string action){
       vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
        while (prixj[i].getDate() < dateAujourdhui && prixj.size()>i)
        {

            if (prixj[i].getAction()==action)
            {
                prix_journaliers.push_back(prixj[i]);
            }
             i++;
        }
        return prix_journaliers;
    }
    }
   vector<PrixJournalier> getPrixJournaliersParPrix(double prix){
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
           while (prixj[i].getDate() < dateAujourdhui && prixj.size()>i)
          {
              if (prixj[i].getPrix() <=prix)
              {
                prix_journaliers.push_back(prixj[i]);
              }
              i++;
          }
        return prix_journaliers;
    }
    vector<PrixJournalier> getPrixJournaliersParPrixAujourdhui(double prix){
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
           while (prixj[i].getDate()= dateAujourdhui && prixj.size()>i)
          {
              if (prixj[i].getPrix() <=prix)
              {
                prix_journaliers.push_back(prixj[i]);
              }
              i++;
          }
        return prix_journaliers;
    }
   }
    vector<PrixJournalier> getPrixJournaliersDisponiblesParAujourdhui(){
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
         while ( prixj.size()>i)
        {
            if (prixj[i].getDate() == dateAujourdhui)
            {
                prix_journaliers.push_back(prixj[i]);
            }
             i++;
        }
        return prix_journaliers;
    }
    }

};

enum TypeTransaction {acheter, vendre, rienfaire};
class Transaction{
    private:
        TypeTransaction type;
        string nomAction;
        int quantite;
     public:
        TypeTransaction getType(){return type;}
        string getNomAction(){return nomAction;}
        int getQuantite(){return quantite;}
        Transaction(TypeTransaction t):type(t){};
        Transaction(TypeTransaction t,string a,int q):type(t),nomAction(a),quantite(q){};
};
class Titre{
    private:
      string action;
      double qt;
    public:
      Titre(string a, float q): action(a), qt(q){};
      string getAction()const {return action;};
      double getQt()const{return qt;};
      void setQt(double quantite){qt=quantite;}
};

class Portefeuille{
   private:

       vector<Titre> titres;
       double solde;
   public:

       Portefeuille(vector<Titre> t,double s):titres(t),solde(s){};
       double getSolde(){return solde;};
       vector<Titre> getTitres(){return titres;};
       void ajouterTitre (Titre titre);
       void retirerTitre(Titre* titre);
       void deposerMontant(double montant);
       void retirerMontant(double montant);
};

void Portefeuille::ajouterTitre(Titre titre) {
  titres.push_back(titre);
}
void Portefeuille::retirerTitre(Titre* titre) {
    for (auto it = titres.begin(); it != titres.end(); ++it) {
        if (it->getAction() == titre->getAction() && it->getQt() == titre->getQt()) {
            titres.erase(it);
            break;
    }
}
}
void Portefeuille::deposerMontant(double montant){

    solde+=montant;
}
void Portefeuille::retirerMontant(double montant){
    solde-=montant;
}
class Trader{
    protected:
        string nom;
        Portefeuille portefeuille;
    public:
        virtual Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille)=0;
        Trader(string n,Portefeuille p):nom(n),portefeuille(p){};
        virtual ~Trader() {};
        string getNom() const;
        Portefeuille& getPortefeuille() { return portefeuille; }

    };


class TraderAleatoire: public Trader{
     public:
      Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille);
      TraderAleatoire(string n,Portefeuille p):Trader(n, p){};

};
Transaction Trader::choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille){
    vector<PrixJournalier> prixJournaliersDisponiblesDansLaBourseAujourdhui=bourse.getPrixJournaliersDisponiblesParAujourdhui();
    vector<Titre> titresDeTrader=portfeuille.getTitres();
    vector<PrixJournalier> titresDeTraderDisponiblesDansLaBourseAujourdhui;
    vector<PrixJournalier> prixJournaliersDisponiblesaAcheter =getPrixJournaliersParPrixAujourdhui(portefeuille.getSolde());
    for(auto i :prixJournaliersDisponiblesDansLaBourseAujourdhui){
        for (auto j:titresDeTrader){
            if(i.getAction()==j.getAction()){
                titresDeTraderDisponiblesDansLaBourseAujourdhui.push_back(i);
            }
        }
    }
    if (titresDeTrader.empty()){
        if(prixJournaliersDisponiblesaAcheter.empty()){
            Transaction transaction(rienfaire);
            return Transaction;
        }
        else{
           int index = rand() % prixJournaliersDisponiblesaAcheter.size();
           PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
           double prixAction= pj.getPrix();
           string action =pj.getAction();
           double quantite= floor(portefeuille.getSolde()/prixAction);
           Transaction transaction(acheter,action,quantite);
           Titre t(action, quantite);
           trader.getPortefeuille().ajouterTitre(t);
           trader.getPortefeuille().retirerMontant(prixAction*quantite);
           return Transaction;
        }
    }
    else{
            if(portefeuille.getSolde()==0){
                 if(titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()){
                     Transaction transaction(rienfaire);
                     return Transaction;
                 }
                 else{
                    int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                    PrixJournalier prixJournalierAvendre=titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                    for(Titre i: titresDeTrader){
                          if (i.getAction()== prixJournalierAvendre.getAction()){
                             double quantiteAvendre = 1.0 + static_cast<double>(rand()) / RAND_MAX * (i.getQt());
                             break;}
                    }
                    string action =prixJournalierAvendre.getAction();
                    double prixAction = prixJournalierAvendre.getPrix();
                    Titre titreAvendre(action,quantiteAvendre);
                    trader.getPortefeuille().retirerTitre(&titreAvendre);
                    trader.getPortefeuille().deposerMontant(prixAction*quantiteAvendre);
                    Transaction transaction(vendre,action,quantiteAvendre);
                    return Transaction;
                 }
            }
            else{
                TypeTransaction type = static_cast<TypeTransaction>(rand()%3);
                if(type==acheter){
                    if(prixJournaliersDisponiblesaAcheter.empty()){
                        do{
                          TypeTransaction type = static_cast<TypeTransaction>(rand()%3);}while(type=acheter);
                        if (type==vendre){
                            if(titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()){
                                Transaction transaction(rienfaire);
                                return Transaction;}
                            }
                            int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                            PrixJournalier prixJournalierAvendre=titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                            for(Titre i: titresDeTrader){
                                if (i.getAction()== prixJournalierAvendre.getAction()){
                                  double quantiteAvendre = 1.0 + static_cast<double>(rand()) / RAND_MAX * (i.getQt());
                                  break;}
                            }
                            string action =prixJournalierAvendre.getAction();
                            double prixAction = prixJournalierAvendre.getPrix();
                            Titre titreAvendre(action,quantiteAvendre);
                            trader.getPortefeuille().retirerTitre(&titreAvendre);
                            trader.getPortefeuille().deposerMontant(prixAction*quantiteAvendre);
                            Transaction transaction(vendre,action,quantiteAvendre);
                            return Transaction;
                            }
                        else{
                        Transaction transaction(rienfaire);
                        return Transaction;}
                    }
                    else{
                      int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                      PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                      double prixAction= pj.getPrix();
                      string action =pj.getAction();
                      double quantite= floor(portefeuille.getSolde()/prixAction);
                      Transaction transaction(acheter,action,quantite);
                      Titre t(action, quantite);
                      trader.getPortefeuille().ajouterTitre(t);
                      trader.getPortefeuille().retirerMontant(prixAction*quantite);
                      return Transaction;
                      }
                   }
                   else if (type==vendre){
                              if(titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()){
                                   do{
                                       TypeTransaction type = static_cast<TypeTransaction>(rand()%3);}while(type=vendre);
                                   if (type==acheter){
                                       if(prixJournaliersDisponiblesaAcheter.empty()){
                                           Transaction transaction(rienfaire);
                                           return Transaction;
                                       }
                                       int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                                       PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                                       double prixAction= pj.getPrix();
                                       string action =pj.getAction();
                                       double quantite= floor(portefeuille.getSolde()/prixAction);
                                       Transaction transaction(acheter,action,quantite);
                                       Titre t(action, quantite);
                                       trader.getPortefeuille().ajouterTitre(t);
                                       trader.getPortefeuille().retirerMontant(prixAction*quantite);
                                       return Transaction;
                                       }
                                     else{
                                         Transaction transaction(rienfaire);
                                         return Transaction;}
                               }
                               else{
                                  int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                                  PrixJournalier prixJournalierAvendre=titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                                  for(Titre i: titresDeTrader){
                                         if (i.getAction()== prixJournalierAvendre.getAction()){
                                              double quantiteAvendre = 1.0 + static_cast<double>(rand()) / RAND_MAX * (i.getQt());
                                              break;}
                                              }
                                  string action =prixJournalierAvendre.getAction();
                                  double prixAction = prixJournalierAvendre.getPrix();
                                  Titre titreAvendre(action,quantiteAvendre);
                                  trader.getPortefeuille().retirerTitre(&titreAvendre);
                                  trader.getPortefeuille().deposerMontant(prixAction*quantiteAvendre);
                                  Transaction transaction(vendre,action,quantiteAvendre);
                                  return Transaction;
                 }

              }
                else{
                    Transaction transaction(rienfaire);
                    return Transaction;
                }
            }
      }



class Simulation {
     private:
         Date dateDebut;
         Date dateFin;
         Date dateCourante;
         double budget;
     public:
      static void executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde);
};
void Simulation::executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde){
    bourse.setDateAujourdhui(dateDebut);
    int nbrTxParJour=0;
    while (bourse.getDateAujourdhui()<dateFin){
        do{
            Transaction transaction=trader.choisirTransaction(bourse,trader.getPortefeuille());
            nbrTxParJour++;
            if(transaction.getType()==rienfaire){
                     bourse.setDateAujourdhui(dateAujourdhui.incrementerDate());}
        }while(nbrTxParJour < 100);
        bourse.setDateAujourdhui(dateAujourdhui.incrementerDate());
    }
    vector<Titre> titresDeTrader=trader.getPortefeuille().getTitres()
    for(Titre titre : titresDeTrader){
             vector<PrixJournalier> prixJournaliersDisponibles = bourse.getPrixJournaliersDisponiblesParDate(bourse.getDateAujourdhui());
             for (int i = prixJournaliersDisponibles.size() - 1; i >= 0; i--) {
                if (prixJournaliersDisponibles[i].getAction() == titre.getAction()) {
                       double prixAction = prixJournaliersDisponibles[i].getPrix();
                       double quantite =titre.getQt();
                       trader.getPortefeuille().retirerTitre(&titre);
                       trader.getPortefeuille().deposerMontant(prixAction*quantite);}
        }
    }

}



int main()

{
    srand (time(NULL));
    map<string, long> stats;
    stats["MON_COMPTEUR"]++;
    auto start = chrono::high_resolution_clock::now();
    Date d1;
    cin>> d1;
    cout<<d1;
    Date d2;
    cin>>d2;
    d1.incrementerDate();
    cout << d1 << endl;
    vector<PrixJournalier> historique=PersistancePrixJournaliers::lirePrixJournaliersDUnFichier("C:\\Users\\hp\\Desktop\\enit\\S2\\MP\\prices_simple.csv");
   /* for(int i = 0; i < historique.size(); i++) {
        cout << historique[i] << endl;
    }*/  // afficher le vecteur historique
    BourseVector v(historique);

    Titre t1("act", 123);
    Titre t2("at", 12);
    Titre t3("ac", 13);
    vector<Titre> titres;
    titres.push_back(t1);
    titres.push_back(t1);
    titres.push_back(t1);
    Portefeuille p(titres, 133);
    TraderAleatoire trader("jack",p);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    stats["TEMPS_GET_ACTIONS_DISPO_AUJ_µs"]+=duration.count();
    Simulation::executer(v,trader, d1, d2, 134411);
    for(auto it:stats){   cout<<it.first<<"\t"<<it.second<<endl; }
    return 0;
}
