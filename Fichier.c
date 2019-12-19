/* Programme Huffman – Code source : Fichier.c */
/* Projet programmation C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fichier.h"
#include "Arbres.h"
#define BUF_LEN 512


/* Variables globales */
 struct arbre_data *arbre_d;
 struct arbre *arbre;
 struct dictionnaire *dico;
 

/*Calcule la fréquence de chaque caractère dans un fichier et trie la liste de structurespar fréquences croissantes src : pointeur 
 sur le fichier*nbre_octets : pointeur sur une variable recevant la taille du fichier*nbre_ascii : pointeur sur une variable recevant 
 le nombre de caractèresdifférents présents dans le fichier sourceRetourne l'index de la première structure dans la liste triée*/
 
short huffman_calculer_frequences(FILE *src, unsigned long *nbre_octets, unsigned short *nbre_ascii){
	int i, r;
	unsigned char buffer[BUF_LEN];
	short index_frequence=0, index_precedent=-1;
	int continuer=1;
	short c1, c2;

	*nbre_octets=0;
	*nbre_ascii=0;

	memset(arbre_d, 0, 512*sizeof(struct arbre_data));
	/*Lecture dans le fichier */
	while ((r=fread(buffer, 1, BUF_LEN, src))>0)
	{
		*nbre_octets+=r;
		for(i=0; i<r; i++)
		/* incrémentation du compteur du caractère correspondant */
		arbre_d[buffer[i]].frequence++;
	}

	/* Chainage des structures avec une fréquence supérieure à 0 */
	for(i=0; i<256; i++)
		if (arbre_d[i].frequence>0)
	{
		(*nbre_ascii)++;
		if (index_precedent==-1)
			index_frequence=i;
		else
			arbre_d[index_precedent].index_suivant=i;
		index_precedent=i;
	}
	if (index_precedent==-1)
		index_frequence=-1;
		else
		arbre_d[index_precedent].index_suivant=-1;
	/* Tri des structures (bubble sort) */
	while (continuer)
	{
		c1=index_frequence;
		continuer=0;
		index_precedent=-1;
		while(c1!=-1)
		{
			if ((c2=arbre_d[c1].index_suivant)!=-1)
			{
				if (arbre_d[c1].frequence>arbre_d[c2].frequence)
				{
					continuer=1;
					if (index_precedent==-1)
						index_frequence=c2;
					else
						arbre_d[index_precedent].index_suivant=c2;
					arbre_d[c1].index_suivant=arbre_d[c2].index_suivant;
					arbre_d[c2].index_suivant=c1;
				}
				index_precedent=c1;
				c1=c2;
			}
			else
				c1=c2;
			}
		}
		/* On retourne l'index de la première structure */
		return index_frequence;
}

/*Crée une table de fréquences à partir de src, l'inscrit dans dst,puis affiche
un tableau des caractères de la table, avec leur fréquence.*/
char huffman_creer_fichier_frequences(FILE *src, FILE *dst){
    short i, compteur=0;
    unsigned long nbre_octets;
    unsigned short nbre_ascii=0;

    i=huffman_calculer_frequences(src, &nbre_octets, &nbre_ascii);

    /*Ecriture de la taille de la table des fréquences */
    fwrite(&nbre_ascii, 1, 1, dst);

    printf("Creation du fichier de frequences...\n");
    printf("%d caracteres representes sur 256\n", nbre_ascii);

    /*Ecriture de la table des fréquences */
    while(i!=-1)
    {
        nbre_ascii=i;
        fwrite(&nbre_ascii, 1, 1, dst);
        fwrite((char *)&arbre_d[i].frequence, 4, 1, dst);
        if ((nbre_ascii>=32 && nbre_ascii<=126) || (nbre_ascii>=161 && nbre_ascii <=255))
            printf("%-4c %-6ld", nbre_ascii, arbre_d[i].frequence);
        else
            printf("0x%02x %-6ld", nbre_ascii, arbre_d[i].frequence);
        if (((compteur+1)%7)==0)
            printf("\n");
            compteur++;
            i=arbre_d[i].index_suivant;
        }
        printf("\n");
        return 0;
}

