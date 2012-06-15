#include "operateur.h"
#include <iostream>

using namespace std;

Complexe& Operation::Plus::plusComplexe(const Complexe& a, const Complexe& b){
    Operation::Plus *additionner;
    Pile<Data> *mapile = new Pile<Data>();
    mapile->addPile(&a.getReel());
    mapile->addPile(&b.getReel());
    Data& preel = additionner->calcul(mapile);
    DataReelle& reel = dynamic_cast<DataReelle&>(preel);
    mapile->addPile(&a.getImaginaire());
    mapile->addPile(&b.getImaginaire());
    Data& pimaginaire = additionner->calcul(mapile);
    DataReelle& imaginaire = dynamic_cast<DataReelle&>(pimaginaire);
    Complexe *resultat = new Complexe(reel,imaginaire);
    Complexe &ref = *resultat;
    return ref;
}

Rationnel& Operation::Plus::plusRationnel(const Rationnel& r1, const Rationnel& r2){
    Pile<Data> *mapile = new Pile<Data>();
    Operation::Mult *multiplier;
    Operation::Plus *additionner;
    if(r1.getDenominateur().getValeur() == r2.getDenominateur().getValeur()){
        mapile->addPile(&r1.getNumerateur());
        mapile->addPile(&r2.getNumerateur());
        Data& numerateur=additionner->calcul(mapile);
        QString typeop1=typeid(numerateur).name();
            Entier& e1 = dynamic_cast<Entier&>(numerateur);
            Rationnel *Resultat = new Rationnel(e1.getValeur(),r1.getDenominateur().getValeur());
            Rationnel& ref=*Resultat;
            return ref;
    }
    else if (r1.getDenominateur().getValeur() != r2.getDenominateur().getValeur()) {
        mapile->addPile(&r1.getNumerateur());
        mapile->addPile(&r2.getDenominateur());
        Data& tmp1 = multiplier->calcul(mapile);
        Entier& e1 = dynamic_cast<Entier&>(tmp1);
        mapile->addPile(&r1.getDenominateur());
        mapile->addPile(&r2.getDenominateur());
        Data& tmp2 = multiplier->calcul(mapile);
        Entier& denom = dynamic_cast<Entier&>(tmp2);
        mapile->addPile(&r2.getNumerateur());
        mapile->addPile(&r1.getDenominateur());
        Data& tmp3 = multiplier->calcul(mapile);
        Entier& e2 = dynamic_cast<Entier&>(tmp3);
        mapile->addPile(&e1);
        mapile->addPile(&e2);
        Data& numerateur=additionner->calcul(mapile);
        Entier& e = dynamic_cast<Entier&>(numerateur);
        Rationnel *resultat = new Rationnel(e.getValeur(),denom.getValeur());
        resultat->simplifier();
        Rationnel& ref=*resultat;
        return ref;
    }
    else
        throw CalculException("Erreur");
}

// on part du principe que l'operateur a ete depile
Data& Operation::Plus::calcul(Pile<Data> *p) {
        Data & op1 = *p->pop();
        Data & op2 = *p->pop();
        QString typeop1=typeid(op1).name();
        QString typeop2=typeid(op2).name();
        if(typeop1.contains("Entier")){
            Entier& e1=dynamic_cast<Entier&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Entier* resultat = new Entier(e1.getValeur()+e2.getValeur());
                Entier& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel& re1=e1.toReel();
                Reel* resultat = new Reel(re1.getValeur()+re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel& ra1=e1.toRationnel();
                Rationnel& temp=plusRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(e1.toEntier(),Entier(0));
                Complexe& temp=plusComplexe(*c1,c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
                return ref;
            }
            else {
                throw CalculException("Erreur Addition Entier");
            }
       }
        else if(typeop1.contains("Reel")){
            Reel& re1=dynamic_cast<Reel&>(op1);
            if(typeop2.contains("Entier")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel* resultat = new Reel(re1.getValeur()+re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel&  ra1=re1.toRationnel();
                Rationnel& temp=plusRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(re1.toReel(),Reel(0));
                Complexe& temp=plusComplexe(*c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Reel");
            }
        }
        else if(typeop1.contains("Rationnel")){
            Rationnel& ra1=dynamic_cast<Rationnel&>(op1);
            if(typeop2.contains("Entier")) {
               Pile<Data> *pile = new Pile<Data>();
               pile->addPile(&op1);
               pile->addPile(&op2);
               return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel& temp=plusRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(ra1.toRationnel(),Rationnel(0,1));
                Complexe& temp=plusComplexe(*c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }
        else if(typeop1.contains("Complexe")){
            Complexe& c1=dynamic_cast<Complexe&>(op1);
            if(typeop2.contains("Entier")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Rationnel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe& temp=plusComplexe(c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }
        else
            throw CalculException("Addition Impossible!!!");
}

Complexe& Operation::Moins::moinsComplexe(const Complexe& a, const Complexe& b){
    Operation::Plus *soustraire;
    Pile<Data> *mapile = new Pile<Data>();
    mapile->addPile(&a.getReel());
    mapile->addPile(&b.getReel());
    Data& preel = soustraire->calcul(mapile);
    DataReelle& reel = dynamic_cast<DataReelle&>(preel);
    mapile->addPile(&a.getImaginaire());
    mapile->addPile(&b.getImaginaire());
    Data& pimaginaire = soustraire->calcul(mapile);
    DataReelle& imaginaire = dynamic_cast<DataReelle&>(pimaginaire);
    Complexe *resultat = new Complexe(reel,imaginaire);
    Complexe &ref = *resultat;
    return ref;
}

Rationnel& Operation::Moins::moinsRationnel(const Rationnel& r1, const Rationnel& r2){
    Pile<Data> *mapile = new Pile<Data>();
    Operation::Mult *multiplier;
    Operation::Moins *soustraire;
    if(r1.getDenominateur().getValeur() == r2.getDenominateur().getValeur()){
        mapile->addPile(&r1.getNumerateur());
        mapile->addPile(&r2.getNumerateur());
        Data& numerateur=soustraire->calcul(mapile);
        Entier& e1 = dynamic_cast<Entier&>(numerateur);
        Rationnel *Resultat = new Rationnel(e1.getValeur(),r1.getDenominateur().getValeur());
        Rationnel& ref=*Resultat;
        return ref;
    }
    else if (r1.getDenominateur().getValeur() != r2.getDenominateur().getValeur()) {
        mapile->addPile(&r1.getNumerateur());
        mapile->addPile(&r2.getDenominateur());
        Data& tmp1 = multiplier->calcul(mapile);
        Entier& e1 = dynamic_cast<Entier&>(tmp1);
        mapile->addPile(&r1.getDenominateur());
        mapile->addPile(&r2.getDenominateur());
        Data& tmp2 = multiplier->calcul(mapile);
        Entier& denom = dynamic_cast<Entier&>(tmp2);
        mapile->addPile(&r2.getNumerateur());
        mapile->addPile(&r1.getDenominateur());
        Data& tmp3 = multiplier->calcul(mapile);
        Entier& e2 = dynamic_cast<Entier&>(tmp3);
        mapile->addPile(&e1);
        mapile->addPile(&e2);
        Data& numerateur=soustraire->calcul(mapile);
        Entier& e = dynamic_cast<Entier&>(numerateur);
        Rationnel *resultat = new Rationnel(e.getValeur(),denom.getValeur());
        resultat->simplifier();
        Rationnel& ref=*resultat;
        return ref;
    }
    else
        throw CalculException("Erreur");
}

Data& Operation::Moins::calcul(Pile<Data> *p) {
        Data & op1 = *p->pop();
        Data & op2 = *p->pop();
        QString typeop1=typeid(op1).name();
        QString typeop2=typeid(op2).name();
        if(typeop1.contains("Entier")){
            Entier& e1=dynamic_cast<Entier&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Entier* resultat = new Entier(e1.getValeur()-e2.getValeur());
                Entier& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel& re1=e1.toReel();
                Reel* resultat = new Reel(re1.getValeur()-re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel& ra1=e1.toRationnel();
                Rationnel& temp=moinsRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(e1.toEntier(),Entier(0));
                Complexe& temp=moinsComplexe(*c1,c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
                return ref;
            }
            else {
                throw CalculException("Erreur Addition Entier");
            }
       }
        else if(typeop1.contains("Reel")){
            Reel& re1=dynamic_cast<Reel&>(op1);
            if(typeop2.contains("Entier")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel* resultat = new Reel(re1.getValeur()-re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel&  ra1=re1.toRationnel();
                Rationnel& temp=moinsRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(re1.toReel(),Reel(0));
                Complexe& temp=moinsComplexe(*c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Reel");
            }
        }
        else if(typeop1.contains("Rationnel")){
            Rationnel& ra1=dynamic_cast<Rationnel&>(op1);
            if(typeop2.contains("Entier")) {
               Pile<Data> *pile = new Pile<Data>();
               pile->addPile(&op1);
               pile->addPile(&op2);
               return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel& temp=moinsRationnel(ra1,ra2);
                return temp;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(ra1.toRationnel(),Rationnel(0,1));
                Complexe& temp=moinsComplexe(*c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }
        else if(typeop1.contains("Complexe")){
            Complexe& c1=dynamic_cast<Complexe&>(op1);
            if(typeop2.contains("Entier")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Reel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Rationnel")) {
                Pile<Data> *pile = new Pile<Data>();
                pile->addPile(&op1);
                pile->addPile(&op2);
                return calcul(pile);
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe& temp=moinsComplexe(c1,c2);
                return temp;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }
        else
            throw CalculException("Addition Impossible!!!");
}


Data& Operation::Mult::calcul(Pile<Data> *p) {
        Data & op1 = *p->pop();
        Data & op2 = *p->pop();
        QString typeop1=typeid(op1).name();
        QString typeop2=typeid(op2).name();
        if(typeop1.contains("Entier")){
            Entier& e1=dynamic_cast<Entier&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Entier* resultat = new Entier(e1.getValeur()*e2.getValeur());
                Entier& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel& re1=e1.toReel();
                Reel* resultat = new Reel(re1.getValeur()*re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
   /*         else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel  ra1=e1.toRationnel();
                Rationnel temp=essai->plusRationnel(ra1,ra2);
                Rationnel* resultat = new Rationnel(temp);
                Rationnel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(e1.toEntier(),Entier(0));
                Complexe temp=essai->plusComplexe(c1,&c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
                return ref;
            }
            else {
                throw CalculException("Erreur Addition Entier");
            } */
       }
        else if(typeop1.contains("Reel")){
            Reel& re1=dynamic_cast<Reel&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Reel& re2=e2.toReel();
                Reel* resultat = new Reel(re1.getValeur()*re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Reel* resultat = new Reel(re1.getValeur()*re2.getValeur());
                Reel& ref=*resultat;
                return ref;
            }
       /*     else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel  ra1=re1.toRationnel();
                Rationnel temp=essai->plusRationnel(ra1,ra2);
                Rationnel* resultat = new Rationnel(temp);
                Rationnel& ref=*resultat;
                return ref;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(re1.toReel(),Reel(0));
                Complexe temp=essai->plusComplexe(c1,&c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
                return ref;
            }
            else {
                throw CalculException("Erreur Addition Reel");
            }
        }
        else if(typeop1.contains("Rationnel")){
            Rationnel& ra1=dynamic_cast<Rationnel&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Rationnel& ra2 = e2.toRationnel();
                Rationnel temp=essai->plusRationnel(ra1,ra2);
                Rationnel* resultat = new Rationnel(temp);
                Rationnel& ref=*resultat;
                return ref;
            }
 /*           else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Rationnel ra2=re2.toRationnel();
                Rationnel temp=essai->plusRationnel(ra1.getNumerateur(),ra2.getNumerateur(),ra1.getDenominateur(),ra2.getDenominateur());
                Rationnel* resultat = new Rationnel(temp);
                Rationnel& ref=*resultat;
             //   return resultat;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Rationnel temp=essai->plusRationnel(ra1.getNumerateur(),ra2.getNumerateur(),ra1.getDenominateur(),ra2.getDenominateur());
                Rationnel* resultat = new Rationnel(temp);
                Rationnel& ref=*resultat;
            //    return resultat;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe* c1=new Complexe(ra1.toRationnel(),Rationnel(0,1));
                Complexe temp=essai->plusComplexe(c1,&c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
           //     return resultat;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }
        else if(typeop1.contains("Complexe")){
            Complexe& c1=dynamic_cast<Complexe&>(op1);
            if(typeop2.contains("Entier")) {
                Entier& e2=dynamic_cast<Entier&>(op2);
                Complexe* c2= new Complexe(e2.toEntier(),Entier(0));
                Complexe temp=essai->plusComplexe(&c1,c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
              //  return resultat;
            }
            else if(typeop2.contains("Reel")) {
                Reel& re2=dynamic_cast<Reel&>(op2);
                Complexe* c2= new Complexe(re2.toReel(),Reel(0));
                Complexe temp=essai->plusComplexe(&c1,c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
             //   return resultat;
            }
            else if(typeop2.contains("Rationnel")) {
                Rationnel& ra2=dynamic_cast<Rationnel&>(op2);
                Complexe* c2= new Complexe(ra2.toRationnel(),Rationnel(0,1));
                Complexe temp=essai->plusComplexe(&c1,c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
            //    return resultat;
            }
            else if(typeop2.contains("Complexe")) {
                Complexe& c2=dynamic_cast<Complexe&>(op2);
                Complexe temp=essai->plusComplexe(&c1,&c2);
                Complexe* resultat = new Complexe(temp.getReel(),temp.getImaginaire());
                Complexe& ref=*resultat;
           //     return resultat;
            }
            else {
                throw CalculException("Erreur Addition Rationnel");
            }
        }*/
        else
            throw CalculException("Multiplication Impossible!!!");
    }
}
