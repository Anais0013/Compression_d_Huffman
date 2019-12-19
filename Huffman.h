/* Programme Huffman : entête   */
/* Projet programmation C */

#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED
#define CODE_MAX_LEN 32
#define BUF_LEN 512

/* Prototypes des fonctions*/

char huffman_compression(FILE *, FILE *, FILE *);
char huffman_decompression(FILE *, FILE *, FILE *);
#endif // HUFFMAN_H_INCLUDED
