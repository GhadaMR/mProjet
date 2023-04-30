#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

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

/*istream& operator>>(istream& flux, PrixJournalier& pj) {
            flux >> pj.date >> pj.action >> pj.prix;
            return flux;
        }*/

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
        return historique;
    }
};

class Bourse{
 protected:
    Date dateAujourdhui;
 public:
    Date getDateAujourdhui() {

        time_t now = time(0);
        tm* localtm = localtime(&now);
        int jour = localtm->tm_mday;
        int mois = localtm->tm_mon + 1;
        int annee = localtm->tm_year + 1900;
        dateAujourdhui.jour = jour;
        dateAujourdhui.mois = mois;
        dateAujourdhui.annee = annee;
        return dateAujourdhui;

    }
    virtual vector<PrixJournalier> getPrixJournaliersDisponiblesParAujourdhui()=0;
    virtual vector<string> getActionsDisponiblesParAujourdhui()=0;
    virtual vector<string> getActionsDisponiblesParDate(Date date)=0;
    virtual vector<string> getActionsDisponiblesParPrix(double prix)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParAction(string action)=0;//hethy matemchich
    virtual vector<PrixJournalier> getPrixJournaliersParDate(Date date)=0;
    virtual vector<PrixJournalier> getPrixJournaliersParPrix(double prix)=0;
    virtual ~Bourse(){};

};

class BourseVector: public Bourse
{
public:
    vector<PrixJournalier>prixj;
 public:



    BourseVector( vector<PrixJournalier> historique) : prixj(historique) {};
    vector<PrixJournalier> getPrixjournaliers(){
        return prixj;
    }
    vector<string> getActionsDisponiblesParPrix(double prix)
        {
        vector<PrixJournalier> prj =getPrixJournaliersParPrix(prix);
        vector<string> actionsParPrix;
        int i=0;

        while ((prj[i].getDate() < dateAujourdhui) && i<prj.size())
        {

            if (prj[i].getPrix()== prix)
            {
                actionsParPrix.push_back(prj[i].getAction());
            }
            i++;
        }
        return actionsParPrix;

}

    vector<string> getActionsDisponiblesParAujourdhui()
    {
        vector<PrixJournalier> prjr=getPrixJournaliersDisponiblesParAujourdhui();
        vector<string> actionsAujourdhui;
        int i=0;
         while (i < prjr.size() && prjr[i].getDate() < dateAujourdhui  ){
             if (prjr[i].getDate() == getDateAujourdhui()){
                actionsAujourdhui.push_back(prjr[i].getAction());
                  }
             i++;
             }
        return actionsAujourdhui;
    }
    vector<string> getActionsDisponiblesParDate(Date date)
    {
        vector<PrixJournalier> prj =getPrixJournaliersParDate(date);
        vector<string> actions;
        int i=0;

        while ((prj[i].getDate() < dateAujourdhui) && i<prj.size())
        {

            if (prj[i].getDate() == date)
            {
                actions.push_back(prj[i].getAction());
            }
            i++;
        }
        return actions;

}
    vector<PrixJournalier> getPrixJournaliersParDate(Date date)
    {
        vector<PrixJournalier> prix_journaliers;
        int i=0;
         if(!prixj.empty()){
        while ( prixj.size()>i&& prixj[i].getDate() < dateAujourdhui)
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

            if (prixj[i].getPrix() ==prix)
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
        while (prixj[i].getDate() < dateAujourdhui && prixj.size()>i)
        {

            if (prixj[i].getDate() ==getDateAujourdhui())
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
        Transaction(TypeTransaction t,string a,int q):type(t),nomAction(a),quantite(q){};
};
class Titre{
    public:
      string action;
      double qt;
    public:
      Titre(string a, float q): action(a), qt(q){};
      string getAction()const {return action;};
      double getQt()const{return qt;};
     // friend ostream& operator<<(ostream& flux,const Titre& t);
};

class Portefeuille{
   private:

       vector<Titre> titres;
       double solde;
   public:

       Portefeuille(vector<Titre> t,double s):titres(t),solde(s){};
       double getSolde(){return solde;};
       vector<Titre> getTitres(){return titres;};
       void ajouterTitre (  Titre titre);//hethy matemchich
       void retirerTitre(Titre titre);//hethy matemchich
       void deposerMontant(double montant);
       void retirerMontant(double montant);
};

void Portefeuille::ajouterTitre(Titre titre){
    /*for ( auto t : titres) {
        if (t.getAction() == titre.getAction())

            t.getQt+= titre.getQt();
        else*/
            titres.push_back(titre);

}
void Portefeuille::retirerTitre(Titre titre) {
    for (auto it = titres.begin(); it != titres.end(); ++it) {
        if (it->getAction() == titre.getAction()&& it->getQt()==titre.getQt()) {
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
        virtual TypeTransaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille)=0;
        Trader(string n,Portefeuille p):nom(n),portefeuille(p){};
        virtual ~Trader() {};
        string getNom() const;
        Portefeuille& getPortefeuille() { return portefeuille; }

    };


class TraderAleatoire: public Trader{
     public:
      TypeTransaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille);
      TraderAleatoire(string n,Portefeuille p):Trader(n, p){};

};
TypeTransaction Trader::choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille){
      TypeTransaction type = static_cast<TypeTransaction>(rand()%3);
      return type;
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
    int nbrTxParJour=0;
    Date dateCourante=dateDebut ;
    while (dateCourante<dateFin){
        nbrTxParJour=0;
        do{
            TypeTransaction choix=trader.choisirTransaction(bourse,trader.getPortefeuille());
            if(choix==acheter){
                vector<PrixJournalier> PrixJournaliersDisponibles = bourse.getPrixJournaliersParDate(dateCourante);
                double prixAction;
                string action;
                do{
                    int index = rand() % PrixJournaliersDisponibles.size();
                    PrixJournalier pj= PrixJournaliersDisponibles[index];
                    prixAction= pj.getPrix();
                    action =pj.getAction();
                }while(prixAction>trader.getPortefeuille().getSolde());
                double quantite= floor(trader.getPortefeuille().getSolde()/prixAction);
                Titre t(action, quantite);
                trader.getPortefeuille().ajouterTitre(t);
                trader.getPortefeuille().retirerMontant(prixAction*quantite);
                nbrTxParJour++;
            }
            else if(choix==vendre){
                vector<Titre> titresAvendre = trader.getPortefeuille().getTitres();
                for(Titre titre : titresAvendre){
                    vector<PrixJournalier> PrixJournaliersDisponibles = bourse.getPrixJournaliersParDate(dateCourante);
                    for(PrixJournalier pj: PrixJournaliersDisponibles){
                        if(titre.getAction() == pj.getAction()){
                            double quantiteAvendre = 1.0 + static_cast<double>(rand()) / RAND_MAX * (titre.getQt());
                            double prixAction = pj.getPrix();
                            trader.getPortefeuille().retirerTitre(titre);
                            trader.getPortefeuille().deposerMontant(prixAction*quantiteAvendre);
                            nbrTxParJour++;
                            break;
                        }
                    }
                }
            }
            else{
                dateCourante.incrementerDate();
            }
            dateCourante.incrementerDate();
        }while(nbrTxParJour < 100);

    }
    for(Titre titre : trader.getPortefeuille().getTitres()){
            //effacer tout
        }
}



int main()

{
    /*srand (time(NULL));
    Date d1;
    cin>> d1;
    cout<<d1;
    Date d2;
    cin>>d2;
    d1.incrementerDate();
    cout << d1 << endl;*/
    vector<PrixJournalier> historique=PersistancePrixJournaliers::lirePrixJournaliersDUnFichier("C:\\Users\\hp\\Desktop\\enit\\S2\\MP\\prices_simple.csv");
   /* for(int i = 0; i < historique.size(); i++) {
        cout << historique[i] << endl;
    }*/  // afficher le vecteur historique
    BourseVector v(historique);
    Titre(string a, float q);
    Portefeuille(vector<Titre> t,double s);
    TraderAleatoire("jack",)


 Simulation::executer(v, Trader& trader, d1, d2, 134411);
   for (auto i :p.getTitres()){

          cout<<i.getAction()<<"-" <<i.getQt()<<endl;
          cout<<p.getTitres().size();
          }

   cout<<p.getSolde()<<endl;
   int nb=0;
   vector<Titre> titres = p.getTitres();

   for (int i = 0; i < titres.size(); i++) {
    cout << titres.at(i)<< endl;
}


   cout<<nb++<<endl;


   Trader T("maram", p);


   Simulation
   Simulation(Bourse& bourse, Trader& trader, d1, d2, double solde)*/

    return 0;
}


