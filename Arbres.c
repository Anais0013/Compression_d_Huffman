/* Programme Huffman � Code source : Arbres.c */
/* Projet programmation C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arbres.h"


/* Variables globales */
 struct arbre_data *arbre_d;
 struct arbre *arbre;
 struct dictionnaire *dico;


/*Cr�e un arbre � partir d'une liste de fr�quencesindex_fr�quence : idnex de la premi�re structure 
 dans la liste *arbre_dRetourne l'index de la racine de l'arbre dans la liste *arbre*/

short huffman_creer_arbre(short index_frequence){
    short i, j, j_save;
    unsigned long somme_frequence;
    short nbre_noeuds=256;
    char struct_inseree=0;

    /* Les structures 0 � 255 correspondent aux caract�res, ce sont des terminaisons => -1 */

    for(j=0; j<256; j++)
        arbre[j].branche0=arbre[j].branche1=-1;

    /* Cr�ation de l'arbre :   La mise en commun les deux fr�quences les plus faibles cr�e un nouveau noeud avec une frequence 
       �gale a la somme des deux fr�quences. Il s'agit ensuite d'ins�rer cette nouvelle structure dans la liste tri�e */

    i=index_frequence;
    while(i!=-1)
    {
        if (arbre_d[i].index_suivant==-1)
        {
        /*printf("Arbre cree\n");*/
        break;
        }

    somme_frequence=arbre_d[i].frequence +arbre_d[arbre_d[i].index_suivant].frequence;
    arbre_d[nbre_noeuds].frequence=somme_frequence;
    arbre[nbre_noeuds].branche0=arbre_d[i].index_suivant;
    arbre[nbre_noeuds].branche1=i;
    /* Insertion du nouveau noeud dans la liste tri�e */
    j_save=-1;
    struct_inseree=0;
    j=i;
    while (j!=-1 && struct_inseree==0)
    {
        if (arbre_d[j].frequence>=somme_frequence)
        {
            if (j_save!=-1)
                arbre_d[j_save].index_suivant=nbre_noeuds;
            arbre_d[nbre_noeuds].index_suivant=j;

            struct_inseree=1;
        }
        j_save=j;
        j=arbre_d[j].index_suivant;
    }
    /* Insertion du nouveau noeud a la fin */
    if (struct_inseree==0)
    {
        arbre_d[j_save].index_suivant=nbre_noeuds;
        arbre_d[nbre_noeuds].index_suivant=-1;
    }

    nbre_noeuds++;
    i=arbre_d[i].index_suivant;
    i=arbre_d[i].index_suivant;
    }
/* On retourne l'index du noeud racine */
return nbre_noeuds-1;
}

/*Proc�dure r�cursive qui cr�e un dictionnaire (correspondance entre la valeur ascii d'un
caract�re et son codage obtenu avec la compression huffman) � partir d'un arbre*code : pointeur
sur une zone m�moire de taille CODE_MAX_LEN recevant temporairement le code, au fur et � mesure
de la progression dans l'arbreindex : position dans l'arbre (index de la structure courante)pos :
nombre de bits deja inscrits dans *code*/

void huffman_creer_dictionnaire(unsigned char *code, short index, short pos)
{
    /* On a atteint une terminaison de l'arbre : c'est un caract�re */
    if ((arbre[index].branche0==-1) && (arbre[index].branche1==-1))
    {
        /* Copie du code dans le dictionnaire */
        memcpy(dico[index].code, code, CODE_MAX_LEN);
        dico[index].taille=(unsigned char)pos;
    }
    /* le noeud poss�de d'autres branches : on continue � les suivre */
   else
    {
    /* On suit la branche ajoutant un bit valant 0 */
    code[pos/8]&=~(0x80>>(pos%8));
    /* Le "(short)" devant "(pos+1)", c'est juste pour empecher VC++
    de chipoter (Warning : integral size mismatch in argument : conversion supplied) */
    huffman_creer_dictionnaire(code, arbre[index].branche0, (short)(pos+1));
    /* On suit la branche ajoutant un bit valant 1 */
    code[pos/8]|=0x80>>(pos%8);
    huffman_creer_dictionnaire(code, arbre[index].branche1, (short)(pos+1));
    }
}

/*Lit les fr�quences de chaque caract�re, inscrites soit dans le fichiercompress�, soit dans un fichier de fr�quences sp�cial*/
short huffman_lire_frequences(FILE *frq){
    unsigned short nbre_ascii;
    unsigned char i;
    short index_frequence=-1, index_precedent=-1;

    /* Lecture de la taille de la table des fr�quences */
    fread(&nbre_ascii, 2, 1, frq);

    /* Lecture de la table des fr�quences */
    while (nbre_ascii>0)
    {
        /* Lecture du caract�re en cours */
        fread(&i, 1, 1, frq);
        /* Lecture de la fr�quence du caract�re en cours */
        fread((char *)&arbre_d[i].frequence, 4, 1, frq);
        /* Chainage de la structure */
        if (index_frequence==-1)
            index_frequence=i;
        else
            arbre_d[index_precedent].index_suivant=i;
            index_precedent=i;
            nbre_ascii--;
    }
    if (index_precedent==-1)
        return -1;

    arbre_d[index_precedent].index_suivant=-1;
    return index_frequence;
}





