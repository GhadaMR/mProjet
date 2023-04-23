#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include <ctime>
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
    char tab[100];
    flux.getline(tab, 100, '/');
    int jour = atoi(tab);

    flux.getline(tab, 100, '/');
    int mois = atoi(tab);

    flux.getline(tab, 100, ';');
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
    private:
        Date date;
        string action;
        double prix;
    public:
        PrixJournalier(){};
        friend istream& operator>>(istream& flux, PrixJournalier& pj) ;
        friend ostream& operator<<(ostream& flux, const PrixJournalier& pj) ;
        Date getDate(){return date;};
        string getAction(){return action;};
        double getPrix(){return prix;};
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
        vector<PrixJournalier> prjr=getPrixJournaliersParDate(date);
        vector<string> actions;
        int i=0;
        while ((prjr[i].getDate() < dateAujoudhui) && prjr.size()<i)
        {
            i++;
            if (prjr[i].getDate() == date)
            {
                actions.push_back(prjr[i].getAction());
            }
        }
        return actions;
    }

    vector<PrixJournalier> getPrixJournaliersParDate(Date date)
    {
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        while (prixj[i].getDate() < dateAujoudhui && prixj.size()<i)
        {
            i++;
            if (prixj[i].getDate() == date)
            {
                prix_journaliers.push_back(prixj[i]);
            }
        }
        return prix_journaliers;
    }
};
class Simulation {
     private:
         Date dateDebut;
         Date dateFin;
         Date dateCourante;
         double budget;
     static:
         void executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde);
};
void Simulation::executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde){
    int nbrTxParJour=0;
    trader.acheter(bourse,dateCourante);
    nbrTxParJour++;
    while (dateCourante<dateFin){
        string choix=trader.choisirTransaction(bourse,portefeuille);
        if(strcmp(choix,"acheter")){
             trader.acheter(bourse,dateCourante);
             nbrTxParJour++;}
        if(strcmp(choix,"vendre")){
             trader.vendre(bourse,dateCourante);
             nbrTxParJour++;}
        else
             dateCourante.incrementerDate();
        if (nbrTxParJour>=100){
             nbrTxParJour=0;
             dateCourante.incrementerDate();}
    }
    for(string action :trader.getportfeuille().getActions())
         
}
enum TypeTransaction {"acheter", "vendre", "rienfaire"};
class Transaction{
    private;
        TypeTransaction type;
        string nomAction;
        int quantite;
     public:
       
};
class Titre{
    private:
      string action;
      double qt;
    public:
      Titre(string a, float q): action(a), qt(q){};
      string getAction(){return action;};
      double getQt(){return qt;};
       
};

class Portefeuille{
   private:
       double solde;
       vector<Titre> titres;
   public:
       double getSolde(){return solde;};
       vector<Titre> getTitres(){return titres;};
       void ajouterTitre(Titre titre);
       void retirerTitre(Titre titre);
       void deposerMontant(double montant);
       void retirerMontant(double montant);
};

class Trader{
    private:
        string nom;
        Portefeuille portefeuille;
    public:
        virtual Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille)=0;
        Trader(string nom, double budget, double limiteAchat, double limiteVente);
        virtual ~Trader() {};
        string getNom() const;
        vector<string> getPortefeuille() const{return portefeuille;};
        virtual void acheter(Bourse& bourse, Date date)=0;
        virtual void vendre(string action, int quantite, double prix)=0;
        
    };


class TraderAleatoire: public Trader{
     public:
      Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille);
      void acheter(Bourse& bourse, Date date);
      void vendre(Bourse& bourse, Date date);
};
Transaction Trader::choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille){
      TypeTransaction type = static_cast<TypeTransaction>(rand()%3);
      return type;
}
 void acheter(Bourse& bourse, Date date){
    vector<PrixJournalier> PrixJournaliersDisponibles = bourse.getPrixJournaliersParDate(date);
    do{
       int index = rand() % PrixJournaliersDisponibles.size();
       string action= PrixJournaliersDisponibles[index];
       double prixAction= action.getPrix();
    }while(prixAction>portefeuille.getSolde());
    double quantite= floor(portefeuille.getSolde()/prixAction);
    Titre t(action, quantite);
    portefeuille.ajouterTitre(t);
    portefeuille.retirerMontant(prixAction*quantite);
 }
void vendre(Bourse& bourse, Date date){
       vector<Titre> titresAvendre = portefeuille.titres;
       int index = rand() % titresAvendre.size();
       Titre titre= titresAvendre[index];
       vector<PrixJournalier> PrixJournaliersDisponibles = bourse.getPrixJournaliersParDate(date);
       for (PrixJournalier i = PrixJournaliersDisponibles.rbegin(); i != PrixJournaliersDisponibles.rend(); ++i) {
            if (strcmp(PrixJournaliersDisponibles[*i].getAction(),titre.getAction())){
                 double prixAction=PrixJournaliersDisponibles[*i].getPrix();
                 break;}
       }
       int quantiteAvendre= 1+rand()%titre.getQt();
       portefeuille.retirerTitre(titre);
       portefeuille.deposerMontant(prixAction*quantiteAvendre);
}
    
int main()
{
    srand (time(NULL));
    Date d1;
    cin>> d1;
    cout<<d1;
    Date d2;
    cin>>d2;
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
    vector<PrixJournalier> historique = PersistancePrixJournaliers::lirePrixJournaliersDUnFichier("C:\\Users\\hp\\Desktop\\enit\\S2\\MP\\prices_simple.txt");
    BourseVector  bourse(historique);
    vector<PrixJournalier> PrixJournaliersParDate= bourse.getPrixJournaliersParDate(d1);
    for (int i; i<PrixJournaliersParDate.size();i++)
              cout<<PrixJournaliersParDate[i];
    vector<string> actionsDisponibles= bourse.getActionsDisponiblesParDate(d1);
    for (int i; i<actionsDisponibles.size();i++)
                cout<<actionsDisponibles[i];
    return 1;
}


