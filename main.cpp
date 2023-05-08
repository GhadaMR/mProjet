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
        if (annee % 4 == 0 && annee % 100 != 0 || annee % 400 == 0) {
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
Date::Date(int j,int m,int a):jour(j), mois(m), annee(a){}


class PrixJournalier {
    Date date;
    string action;
    double prix;
public:
    PrixJournalier(){};
    friend istream& operator>>(istream& flux, PrixJournalier& pj);
    friend ostream& operator<<(ostream& flux, const PrixJournalier& pj);
    Date getDate() const { return date; };
    void setDate(const Date& d) { date = d; }
    string getAction() const { return action; };
    void setAction(const string& a) { action = a; }
    double getPrix() const { return prix; };
    void setPrix(double p) { prix = p; }
};

ostream& operator<<(ostream& flux, const PrixJournalier& pj) {
    flux << pj.date << "   " << pj.action << "  \n" << pj.prix << endl;
    return flux;
}

istream& operator>>(istream& flux, PrixJournalier& pj) {
    string ligne;
    if (getline(flux, ligne)) {
        stringstream ss(ligne);
        string cell;
        getline(ss, cell, '/');
        Date d;
        d.jour = stoi(cell);
        getline(ss, cell, '/');
        d.mois = stoi(cell);
        getline(ss, cell, ';');
        d.annee = stoi(cell);
        pj.setDate(d);
        getline(ss, cell, ';');
        pj.setAction(cell);
        getline(ss, cell);
        pj.setPrix(stod(cell));
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
    Date getDateAujourdhui() const{ return dateAujourdhui;}
    void setDateAujourdhui(Date date){dateAujourdhui=date;}
    virtual vector<PrixJournalier> getPrixJournaliersDisponiblesParAujourdhui()const=0;
    virtual vector<string>getActionsDisponiblesParAujourdhui()const=0;
    virtual vector<string> getActionsDisponiblesParDate(Date date)const=0;
    virtual vector<string> getActionsDisponiblesParPrix(double prix)const=0;
    virtual vector<PrixJournalier> getPrixJournaliersParAction(string action)const=0;
    virtual PrixJournalier getPrixJournalierParActionAujourdhui(string action)const=0;
    virtual vector<PrixJournalier> getPrixJournaliersParDate(Date date)const=0;
    virtual vector<PrixJournalier> getPrixJournaliersParPrix(double prix)const=0;
    virtual vector<PrixJournalier> getPrixJournaliersParPrixAujourdhui(double prix)const=0;
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
    vector<string> getActionsDisponiblesParPrix(double prix) const
        {
        vector<PrixJournalier> prj =getPrixJournaliersParPrix(prix);
        vector<string> actionsParPrix;
        for(auto i : prj)
        {
                actionsParPrix.push_back(i.getAction());
        }
        return actionsParPrix;

}

    vector<string> getActionsDisponiblesParAujourdhui() const
    {
        vector<PrixJournalier> prjr=getPrixJournaliersDisponiblesParAujourdhui();
        vector<string> actionsAujourdhui;
        for(auto i : prjr){
                actionsAujourdhui.push_back(i.getAction());
                  }
        return actionsAujourdhui;
    }
    vector<string> getActionsDisponiblesParDate(Date date) const
    {
        vector<PrixJournalier> prj =getPrixJournaliersParDate(date);
        vector<string> actions;

        for (auto i : prj)
        {
                actions.push_back(i.getAction());
            }
        return actions;

}
    vector<PrixJournalier> getPrixJournaliersParDate(Date date) const
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
        }
        return prix_journaliers;
    }
    vector<PrixJournalier> getPrixJournaliersParAction(string action)const{
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
        }
        return prix_journaliers;
    }
  PrixJournalier getPrixJournalierParActionAujourdhui(string action)const{
        int i=0;
        PrixJournalier pj;
        if(!prixj.empty()){
        while (prixj[i].getDate() == dateAujourdhui && prixj.size()>i)
        {

            if (prixj[i].getAction()==action)
            {
                pj.setAction(prixj[i].getAction());
                pj.setPrix(prixj[i].getPrix());
                pj.setDate(prixj[i].getDate());
            }
             i++;
        }
    }
    return pj;
    }
   vector<PrixJournalier> getPrixJournaliersParPrix(double prix)const{
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
        }
        return prix_journaliers;
   }
    vector<PrixJournalier> getPrixJournaliersParPrixAujourdhui(double prix)const{
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
           while (prixj[i].getDate()== dateAujourdhui && prixj.size()>i)
          {
              if (prixj[i].getPrix() <=prix)
              {
                prix_journaliers.push_back(prixj[i]);
              }
              i++;
          }
        }
        return prix_journaliers;
   }
    vector<PrixJournalier> getPrixJournaliersDisponiblesParAujourdhui()const{
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
    }
          return prix_journaliers;
    }


};

enum TypeTransaction {ACHETER, VENDRE, RIEN_FAIRE};
class Transaction{
    private:
        TypeTransaction type;
        string nomAction;
        int quantite;
     public:
        TypeTransaction getType()const{return type;}
        string getNomAction()const{return nomAction;}
        int getQuantite()const{return quantite;}
        Transaction(TypeTransaction t):type(t){};
        Transaction(TypeTransaction t,string a,int q):type(t),nomAction(a),quantite(q){};
};
class Titre{
    private:
      string action;
      int qt;
    public:
      Titre(string a, float q): action(a), qt(q){};
      string getAction()const {return action;};
      int getQt()const{return qt;};
      void setQt(int quantite){qt=quantite;}
};

class Portefeuille{
   private:

       vector<Titre> titres;
       double solde;
   public:

       Portefeuille(vector<Titre> t, double s):titres(t), solde(s){};
       double getSolde()const{return solde;};
       void setSolde(double s){solde=s;};
       vector<Titre> getTitres()const{return titres;};
       void ajouterTitre (Titre titre){
        bool found = false;
        for (auto &t : titres) {
            if (t.getAction() == titre.getAction()) {
                t.setQt(t.getQt() + titre.getQt());
                found = true;
                break;
            }
        }
        if (!found) {
            titres.push_back(titre);
        }
};
       void retirerTitre(Titre* titre) {
        for (auto it = titres.begin(); it != titres.end(); ++it) {
            if (it->getAction() == titre->getAction() && it->getQt() >= titre->getQt()) {
                if (it->getQt() == titre->getQt()) {
                    titres.erase(it);
                } else {
                    it->setQt(it->getQt() - titre->getQt());
                }
                break;
            }
        }
    };
       void deposerMontant(double montant){solde+=montant;};
       void retirerMontant(double montant){solde-=montant;};
};

class Trader{
    public:
        virtual Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille)=0;
        Trader(){};
        virtual ~Trader() {};
    };
 
class TraderAleatoire: public Trader{
  public:
Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille)

class TraderAleatoire: public Trader{
  public:
Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille) {
    vector<PrixJournalier> prixJournaliersDisponiblesDansLaBourseAujourdhui = bourse.getPrixJournaliersDisponiblesParAujourdhui();
    vector<Titre> titresDeTrader = portefeuille.getTitres();
    vector<PrixJournalier> titresDeTraderDisponiblesDansLaBourseAujourdhui;
    vector<PrixJournalier> prixJournaliersDisponiblesaAcheter = bourse.getPrixJournaliersParPrixAujourdhui(portefeuille.getSolde());

    for (auto i : prixJournaliersDisponiblesDansLaBourseAujourdhui) {
        for (auto j : titresDeTrader) {
            if (i.getAction() == j.getAction()) {
                titresDeTraderDisponiblesDansLaBourseAujourdhui.push_back(i);
            }
        }
    }

    if (titresDeTrader.empty()) {
        if (prixJournaliersDisponiblesaAcheter.empty()) {
            Transaction transaction(RIEN_FAIRE);
            return transaction;
        }
        else {
            int index = rand() % prixJournaliersDisponiblesaAcheter.size();
            PrixJournalier pj = prixJournaliersDisponiblesaAcheter[index];
            double prixAction = pj.getPrix();
            string action = pj.getAction();
            int quantite = floor(portefeuille.getSolde() / prixAction);
            Transaction transaction(ACHETER, action, quantite);
            return transaction;
        }
    }
    else {
        if (portefeuille.getSolde() == 0) {
            if (titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()) {
                Transaction transaction(RIEN_FAIRE);
                return transaction;
            }
            else {
                int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                PrixJournalier prixJournalierAvendre = titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                int quantiteAvendre = 0;
                for (Titre i : titresDeTrader) {
                    if (i.getAction() == prixJournalierAvendre.getAction()) {
                        quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                        break;
                    }
                }
                string action = prixJournalierAvendre.getAction();
                double prixAction = prixJournalierAvendre.getPrix();
                Transaction transaction(VENDRE, action, quantiteAvendre);
                return transaction;
            }
        }
        else {
            TypeTransaction type = static_cast<TypeTransaction>(rand() % 3);
            if (type == ACHETER) {
                if (prixJournaliersDisponiblesaAcheter.empty()) {
                    do {
                        type = static_cast<TypeTransaction>(rand() % 3);
                    } while (type == ACHETER);

                    if (type == VENDRE) {
                        if (titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()) {
                            Transaction transaction(RIEN_FAIRE);
                            return transaction;
                        }
                        else {
                            int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                            PrixJournalier prixJournalierAvendre = titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                            int quantiteAvendre=0;
                            for(Titre i: titresDeTrader){
                                if (i.getAction()== prixJournalierAvendre.getAction()){
                                  int quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                                  break;}
                            }
                            string action =prixJournalierAvendre.getAction();
                            double prixAction = prixJournalierAvendre.getPrix();
                            Transaction transaction(VENDRE,action,quantiteAvendre);
                            return transaction;
                            }
                    }
                    else{
                        Transaction transaction(RIEN_FAIRE);
                        return transaction;}
                        }
                    else{
                      int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                      PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                      double prixAction= pj.getPrix();
                      string action =pj.getAction();
                      int quantite= floor(portefeuille.getSolde()/prixAction);
                      Transaction transaction(ACHETER,action,quantite);
                      return transaction;
                      }
                }
                   else if (type==VENDRE){
                              if(titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()){
                                   do{
                                       type = static_cast<TypeTransaction>(rand()%3);}while(type==VENDRE);
                                   if (type==ACHETER){
                                       if(prixJournaliersDisponiblesaAcheter.empty()){
                                           Transaction transaction(RIEN_FAIRE);
                                           return RIEN_FAIRE;
                                       }
                                       else{
                                       int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                                       PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                                       double prixAction= pj.getPrix();
                                       string action =pj.getAction();
                                       int quantite= floor(portefeuille.getSolde()/prixAction);
                                       Transaction transaction(ACHETER,action,quantite);
                                       return transaction;
                                       }
                                   }
                                     else{
                                         Transaction transaction(RIEN_FAIRE);
                                         return transaction;}
                              }
                               else{
                                  int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                                  PrixJournalier prixJournalierAvendre=titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                                  int quantiteAvendre;
                                  for(Titre i: titresDeTrader){
                                         if (i.getAction()== prixJournalierAvendre.getAction()){
                                              quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                                              break;}
                                              }
                                  string action =prixJournalierAvendre.getAction();
                                  double prixAction = prixJournalierAvendre.getPrix();
                                  Transaction transaction(VENDRE,action,quantiteAvendre);
                                  return transaction;
                 }

              }
                else{
                    Transaction transaction(RIEN_FAIRE);
                    return transaction;
                }
            }
    }
      };
      TraderAleatoire():Trader(){};

};

class Simulation {
private:
    Date dateDebut;
    Date dateFin;
    Date dateCourante;
    double budget;

public:
    static void executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde){
        vector<Titre> Titres;
        Portefeuille portefeuille(solde);
        bourse.setDateAujourdhui(dateDebut);
        int nbrTxParJour=0;
        while (bourse.getDateAujourdhui() < dateFin){
            do{
                Transaction transaction = trader.choisirTransaction(bourse, portefeuille);
                nbrTxParJour++;
                if (transaction.getType() == RIEN_FAIRE){
                    bourse.getDateAujourdhui().incrementerDate();
                } else if (transaction.getType() == ACHETER){
                    double prixAction = bourse.getPrixJournalierParActionAujourdhui(transaction.getNomAction()).getPrix();
                    int quantite = transaction.getQuantite();
                    double montant = prixAction * quantite;
                    Titre titre(transaction.getNomAction(), quantite);
                    portefeuille.retirerMontant(montant);
                    portefeuille.ajouterTitre(titre);
                } else if (transaction.getType() == VENDRE){
                    double prixAction = bourse.getPrixJournalierParActionAujourdhui(transaction.getNomAction()).getPrix();
                    int quantite = transaction.getQuantite();
                    Titre titre(transaction.getNomAction(), quantite);
                    portefeuille.retirerTitre(&titre);
                    portefeuille.deposerMontant(prixAction*quantite);
                }
            } while(nbrTxParJour < 100);
            nbrTxParJour = 0;
            bourse.getDateAujourdhui().incrementerDate();
        }
        for (Titre& titre : portefeuille.getTitres()){
            for (int i = bourse.getPrixJournaliersParAction(titre.getAction()).size() - 1; i >= 0; i--){
                if (bourse.getPrixJournaliersParAction(titre.getAction())[i].getDate() < dateFin){
                    double prixAction = bourse.getPrixJournaliersParAction(titre.getAction())[i].getPrix();
                    int quantite = titre.getQt();
                    Titre titreCopie(titre.getAction(), quantite);
                    portefeuille.retirerTitre(&titreCopie);
                    portefeuille.deposerMontant(prixAction * quantite);
                    break;
                }
            }
        }
    }
};

class Test{
public:
    Test(){};
    bool testDate(Date d){
    bool DateValide = true;
    if (d.annee < 0 || d.mois < 1 || d.mois > 12 || d.jour < 1) {
        DateValide = false;
    } else {
        int limiteJour = 31;
        if (d.mois == 2) {
            if (d.annee % 4 == 0 && d.annee % 100 != 0 || d.annee % 400 == 0) {
                limiteJour = 29;
            } else {
                limiteJour = 28;
            }
        } else if (d.mois == 4 || d.mois == 6 || d.mois == 9 || d.mois == 11) {
            limiteJour = 30;
        }
        DateValide = (d.jour >= 1 && d.jour <= limiteJour);
    }
    if(DateValide==false){
        return DateValide;
    }
    return DateValide ;
};
    bool testPrixJournaliers(PrixJournalier PJ){
    bool PJValide = true;
    if (testDate(PJ.getDate())){PJValide = false;}
    if (PJ.getPrix()<=0){PJValide = false;}
    return PJValide ;
    }
};


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
    BourseVector* v= new BourseVector(historique);

    /*Titre t1("act", 123);
    Titre t2("at", 12);
    Titre t3("ac", 13);
    vector<Titre> titres;https://chat.openai.com/chat
    titres.push_back(t1);
    titres.push_back(t1);
    titres.push_back(t1);*/
    Portefeuille p( 133);
    TraderAleatoire* trader= new TraderAleatoire();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    stats["TEMPS_GET_ACTIONS_DISPO_AUJ_µs"]+=duration.count();
    Simulation::executer(*v,*trader, d1, d2, 134411);
    for(auto it:stats){   cout<<it.first<<"\t"<<it.second<<endl; }
    return 0;
}
