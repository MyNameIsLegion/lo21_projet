#ifndef DATAGESTION_H
#define DATAGESTION_H

/**
 * \file DataGestion.h
 * \brief Classe DataGestion servant de facade entre l'IHM et le reste du programme
 * \author Nicolas Biehler/Jean-Baptiste Arzounian
 *
 */

#include <QStack>
#include <QStringList>
#include <string>
#include "data.h"
#include "factory.h"
#include "pile.h"
#include "operateur.h"
#include "calculexception.h"

/*! \class DataGestion
   * \brief classe servant de facade entre les donn�es du programme et l'interface
   * Permet de g�rer tout le programme
   *
   */
class DataGestion {
private:
    Pile<QString> * pileAffichage; /*!< la pile d'affichage */
    Pile<Data> * pileStockage; /*!< la pile de calcul */
    Pile<Data> * pileRetablir; /*!< la pile servant � l'annulation/r�tablissement */
    Factory* factoryInstance; /*!< l'instance de la factory */
    Operation::OperateurStrategy* calculStrategy; /*!< un pointeur sur un operateur g�n�ral */
public:
            /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe
         *
         */
    DataGestion();
                /*!
         *  \brief Destructeur
         *
         *  Destructeur de la classe
         *
         */
    ~DataGestion();
           /*!
         *  \brief retourne la pile de stockage
         *
         */
    Pile<Data>& getStockage() const {return *pileStockage;}
               /*!
         *  \brief retourne la pile r�tablir
         *
         */
    Pile<Data>& getRetablir() const {return *pileRetablir;}
               /*!
         *  \brief retourne la pile d'affichage
         *
         */
    Pile<QString>& getAffichage() const {return *pileAffichage;}
               /*!
         *  \brief change la pile de stockage
         *
         * \param p : nouvelle pile
         */
    void setStockage(Pile<Data>& p) {
        delete pileStockage;
        pileStockage = &p;
    }
                   /*!
         *  \brief change la pile r�tablir
         *
         * \param p : nouvelle pile
         */
    void setRetablir(Pile<Data>& p) {
        delete pileRetablir;
        pileRetablir = &p;
    }
    
                   /*!
         *  \brief change la pile d'affichage
         *
         * \param p : nouvelle pile
         */
    void setAffichage(Pile<QString>& p) {
        delete pileAffichage;
        pileAffichage = &p;
    }
                   /*!
         *  \brief retourne l'instance de la factory
         *
         */
    Factory& getFactory() const {return *factoryInstance;}
                   /*!
         *  \brief d�code une entr�e utlisateur
         *
         * Appel la factory, cr�e les Data qu'il faut, remplit les piles et appelle calcul
         *
         * \param expression : la QStrign entr�e par l'utilisateur
         */
    void parse(QString expression);
                   /*!
         *  \brief effectue le calcul
         *
         * Utilise le pattern strategy des op�rations pour calculer et mettre les piles � jour
         */
    void calcul();
  //  DataGestion& clone() const;
                 /*!
         *  \brief annule les x derni�res op�rations sur la pile de stockage
         *
         */
    void annuler();
                     /*!
         *  \brief r�tablit les x derni�res op�rations sur la pile de stockage
         *
         */
    void retablir();
};

#endif // DATAGESTION_H
