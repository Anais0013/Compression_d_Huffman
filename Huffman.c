/*  Mini Projet: Compression par Huffman
*   Contributeurs:
*       OUEDRAOGO Narelba Abdoul Razac Christian
*       KHADIJA Lahlou
*       El HARRADE Mejdy
*	GIANNUZZI Anaïs
*	Inspiré par : http://projet.eu.org/pedago/sin/term/5-huffman.pdf?fbclid=IwAR01vb8SeW2LP2_74V0DIZLMIys88aHcsY_mLF6f-6noP6ylKOHzNXG8Qmk
*
*   Fichié: Huffman.c
*
*   Création: 03/12/2019
*
*   Description:
*        Il s'agit du programme principal de l'algorithme de compression Huffman.
*        La syntaxe est la suivante :
*   huffman c/d/f source destination
*   - c pour compresser le fichier source dans destination.
*   - d pour le décompresser
*   - f pour le frequence 
*   - fichier source
*   - fichier destination
*
*   Dernière modification: 19/12/2019
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"
#include "Arbres.c"
#include "Fichier.c"



/* Variables globales */
 struct arbre_data *arbre_d;
 struct arbre *arbre;
 struct dictionnaire *dico;

/*Compresse le fichier src.Le résulat se trouve dans le fichier dstSi frq est différent de NULL, il est utilsé pour 
 lire la table des fréquencesnécessaire à la construction de l'arbre*/

char huffman_compression(FILE *src, FILE *dst, FILE *frq){
    int i, r, octet_r, bit_r, bit_count, bit_w;
    unsigned long nbre_octets;
    short index_frequence;
    short racine_arbre;
    unsigned short nbre_ascii=0;
    unsigned char code[CODE_MAX_LEN];
    unsigned char buffer[BUF_LEN];

    printf("Compression en cours...\n");

    /* création ou lecture de la table des fréquences */

    if (frq==NULL)
    {
        index_frequence=huffman_calculer_frequences(src, &nbre_octets, &nbre_ascii);
    }
    else
    {
        printf("Utilisation d'un fichier de frequences pour la creation du dictionaire\n");
        fseek(src, 0, SEEK_END);
        nbre_octets=ftell(src);
        index_frequence=huffman_lire_frequences(frq);
    }

    /*Ecriture de la taille en octets du fichier original */
    fwrite((char *)&nbre_octets, 4, 1, dst);
    /*Ecriture de la taille de la table des fréquences */
    fwrite(&nbre_ascii, 2, 1, dst);
    /*Ecriture de la table des fréquences */
    if (frq==NULL)
    {
        i=index_frequence;
        while(i!=-1)
        {
            nbre_ascii=i;
            fwrite(&nbre_ascii, 1, 1, dst);
            fwrite((char *)&arbre_d[i].frequence, 4, 1, dst);
            i=arbre_d[i].index_suivant;
        }
    }

    /* Construction de l'arbre à partir de la table des fréquences */
    racine_arbre=huffman_creer_arbre(index_frequence);
    /* Allocation de mémoire pour le dictionnaire */
    if ((dico=(struct dictionnaire *)malloc(256*sizeof(struct dictionnaire)))==NULL)
    {
        /*free(arbre_d);
        free(arbre);*/
        perror("malloc");
        return -1;
    }

    /* RAZ du champs taille du dico. Si on utilise une table de fréquences
	prédéfinie pour la compression, et qu'un caractère à compresser n'est pas
	présent dans la table, alors il ne sera pas traité */
    for(i=0; i<256; i++)
        dico[i].taille=0;
    /* Création du dictionnaire à partir de l'arbre */
    huffman_creer_dictionnaire(code, racine_arbre, 0);

    /* Compression du fichier source et écriture dans le fichier cible */
    fseek(src, 0, SEEK_SET);
    code[0]=0;
    bit_w=0x80;
    /* Lecture de BUF_LEN octets dans le fichier source */
    while ((r=fread(buffer, 1, BUF_LEN, src))>0)
    {
    /* Traitement octet par octet */
    for(i=0; i<r; i++)
    {
        /* Ecriture du code correspondant au caractère dans le dictionnaire */
        octet_r=0;
        bit_r=0x80;
        /* Ecriture bit par bit */
        for(bit_count=0; bit_count<dico[buffer[i]].taille; bit_count++)
        {
            if (dico[buffer[i]].code[octet_r] & bit_r)
            code[0]|=bit_w;
            /*else
            code[0]&=~(bit_w); */
            bit_r>>=1;
            if (bit_r==0)
            {
                octet_r++;
                bit_r=0x80;
            }
            bit_w>>=1;
            if (bit_w==0)
            {
                /*printf("%3x", code[0]);*/
                    fputc(code[0], dst);
                    code[0]=0;
                    bit_w=0x80;
            }
        }
    }
}
if(bit_w!=0x80)
    fputc(code[0], dst);

    free(dico);

    printf("Compactage termine. Taux de compression : %.2f\n",
(float)nbre_octets/ftell(dst));

    return 0;
}

/*Decompresse le fichier srcLe résultat se trouve dans dst*/
char huffman_decompression(FILE *src, FILE *dst, FILE *frq)
{
    int i, j, r;
    unsigned long nbre_octets;
    unsigned char nbre_ascii, bit_r;
    short index_frequence;
    short racine_arbre;
    unsigned char buffer[BUF_LEN];

    /* Lecture de la taille du fichier original */
    fread((char *)&nbre_octets, 4, 1, src);
    printf("Taille du fichier original : %ld octets\n", nbre_octets);
    printf("Decompression en cours...\n");

    /* Lecture de la table des fréquences */
    if (frq==NULL)
    {
        index_frequence=huffman_lire_frequences(src);
    }
    else
    {
        fread(&nbre_ascii, 1, 1, src);
        index_frequence=huffman_lire_frequences(frq);
    }

if (index_frequence==-1)
{
    printf("Erreur de lecture de la table des frequences\n");
    return -1;
}

/* Construction de l'arbre à partir de la table des fréquences */
racine_arbre=huffman_creer_arbre(index_frequence);

/* Decompression du fichier source et écriture du résultat dans le fichier cible */
j=racine_arbre;
/* Lecture de BUF_LEN octets dans le fichier source */
while ((r=fread(buffer, 1, BUF_LEN, src))>0)
{
    /*Traitement octet par octet */
    for(i=0; i<r && nbre_octets>0; i++)
        {
        /* Traitement bit par bit */
        for(bit_r=0x80; bit_r!=0 && nbre_octets>0; bit_r>>=1)
        {
            if (buffer[i]&bit_r)
                j=arbre[j].branche1;
            else
                j=arbre[j].branche0;
            if ((arbre[j].branche0==-1) || (arbre[j].branche1==-1))
            {
                /*printf("%c", j);*/
                fputc((char)j, dst);
                nbre_octets--;
                j=racine_arbre;
            }
        }
    }
}

printf("Decompression terminee\n");
return 0;
}

/*Fonction principaleOuvre les fichiers en lecture ou écriture
et alloueun espace mémoire suffisantpour les structures*/
int main(int argc, char *argv[])
{
    FILE *src, *dst, *frq;

    /* Syntaxe incorrecte */
    if (argc<4){
        printf("%s c/d/f source destination [frequence]\n", argv[0]);
        return -1;
    }

    /* Ouverture du fichier source en lecture */
    if ((src=fopen(argv[2], "rb"))==NULL)
    {
        perror("fopen");
        return -1;
    }

    /* Ouverture du fichier cible en écriture */
    if ((dst=fopen(argv[3], "wb"))==NULL)
    {
        perror("fopen");
        return -1;
    }

   /* Ouverture d'un éventuel fichier de liste de fréquences, en lecture */
    if (argc>4)
    {
        if ((frq=fopen(argv[4], "rb"))==NULL)
        {
            perror("fopen");
            return -1;
        }
    }
    else
        frq=NULL;

    /* Allocation mémoire pour les données diverses necessaires à la construction de l'arbre */
    if ((arbre_d=(struct arbre_data *)malloc(512*sizeof(struct arbre_data)))==NULL)
    {
        perror("malloc");
        return -1;
    }

    /* Allocation d'une zone mémoire pour l'arbre */
    if ((arbre=(struct arbre *)malloc(512*sizeof(struct arbre)))==NULL)
    {
        free(arbre_d);
        perror("malloc");
        return -1;
    }

    /* Compression ou décompression ? */
    if (*argv[1]=='c')
        huffman_compression(src, dst, frq);
    else if (*argv[1]=='d')
        huffman_decompression(src, dst, frq);
    else if (*argv[1]=='f')
        huffman_creer_fichier_frequences(src, dst);

    /* Libération de la mémoire */
    free(arbre_d);
    free(arbre);

    /* Fermeture des fichiers */
    fclose(src);
    fclose(dst);
    if (frq!=NULL)
        fclose(frq);
    return 0;
}

