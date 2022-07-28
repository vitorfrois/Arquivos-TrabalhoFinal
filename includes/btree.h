/**
 * @file btree.h
 * @brief Functions for managing B-Tree
 */
#ifndef BTREE_H
#define BTREE_H  

#include <stdio.h>
#include <stdlib.h>

#define INF 10e8

#define MAX_KEYS 3
#define MAX_DESC 4

//cada nó possui um tipo
#define ROOT '0'
#define MID '1'
#define LEAF '2'

/**
 * @brief Struct that represents a B-Tree key.
 * Every key has a id, file type and a rrn or byteoffset.
 * A union "value" is used to store different type values on the key. 
 */
typedef struct key{
    int id;
    char fileType;
    union{
        long int byteOffset;
        int rrn;
    } value;
}Key;

/**
 * @brief Struct that represents a B-Tree node.
 * The node type has 3 possible values: ROOT(0), MID(1) and LEAF(2)
 */
struct bNode{
    char nodeType;
    int numKeys;
    Key key[MAX_KEYS+1];
    int desc[MAX_DESC+1];
    int rrn;
};

/**
 * @brief Struct that represents a B-Tree
 */
struct bTree{
    char status;
    struct bNode *root;
    int rootRRN;
    int nextRRN;
    int numNodes;
};


/**
 * @brief Compare two values to make operations in the B-Tree
 * 
 * @param x id 1
 * @param y id 2
 * @return int 
 */
int compareKeys (const void *x, const void *y);

/**
 * @brief Read a node from the indexFile with the specified RRN
 * 
 * @param indexFile B-Tree index file
 * @param nodeRRN Node RRN
 * @param type Type of file. 1 for fixed size and 2 for variable size
 * @return struct bNode* A struct that represents found node
 */
struct bNode *readNode(FILE *indexFile, int nodeRRN, char type);

/**
 * @brief Reads the B-Tree header
 * 
 * @param indexFile B-Tree index file
 * @param type Type of file. 1 for fixed size and 2 for variable size
 * @return struct bTree* A struct that represents B-Tree
 */
struct bTree *read_tree_header(FILE *indexFile, char type);

/**
 * @brief Search for a key in B-Tree with id
 * 
 * @param node B-Tree root
 * @param id ID to be searched
 * @return Key Struct 
 */
Key searchID(FILE *indexFile, struct bNode *node, int id, char type);

/**
 * @brief Insert a new key in B-Tree
 * 
 * @param indexFile B-Tree index file
 * @param node B-Tree root
 * @return struct bNode* 
 */
struct bNode *insertKey(FILE *indexFile, struct bNode *node, Key);

/**
 * @brief Free all allocated memory for B-Tree
 * 
 * @param tree Allocated B-Tree
 */
void destroyTree(struct bTree *tree);

/**
 * @brief Creates a type 1 B-Tree index file based on registries binary file
 * 
 * @param binFile binFile
 * @param indexFile B-Tree index file
 */
void create_btree_t1(FILE *binFile, FILE *indexFile);

/**
 * @brief Creates a type 2 B-Tree index file based on registries binary file
 * 
 * @param binFile binFile
 * @param indexFile B-Tree index file
 */
void create_btree_t2(FILE *binFile, FILE *indexFile);

#endif