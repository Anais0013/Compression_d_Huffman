/* Programme Huffman : entête   */
/* Projet programmation C */

#ifndef ARBRES_H_INCLUDED
#define ARBRES_H_INCLUDED
#define CODE_MAX_LEN 32
#define BUF_LEN 512

#ifndef MAX /* Vérification que MAX est inutilisée */
  #define MAX 256
#endif /* MAX */
/* Definition des structures */
 struct arbre
{
    short branche0;
    short branche1;
} ;

 struct arbre_data{
    unsigned long frequence;
    short index_suivant;
};

struct dictionnaire{
    unsigned char taille;
    char code[CODE_MAX_LEN];
};

/* Prototypes des fonctions*/

short huffman_creer_arbre (short);
void huffman_creer_dictionnaire(unsigned char *, short, short);

#endif // ARBRES_H_INCLUDED
