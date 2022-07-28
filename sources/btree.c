#include "../includes/btree.h"
#include "../includes/utils.h"
#include "../includes/file_t1.h"
#include "../includes/file_t2.h"
#include <stdio.h>

/**
 * @brief função de comparação entre chaves 
 */
int compareKeys (const void *x, const void *y) {
    int xId = ((Key *)x)->id;
    int yId = ((Key *)y)->id;
    return (xId - yId);
}

/**
 * @brief imprime um nó
 * 
 * @param node 
 */
void printNode(struct bNode *node){
    if(node == NULL) return;
    for(int i = 0; i < node->numKeys; i++)
        printf("%d ", node->key[i].id);
    printf("\n");
}

/**
 * @brief lê o header do arquivo de árvore B
 * 
 * @param indexFile 
 * @param type 
 * @return struct bTree* 
 */
struct bTree *read_tree_header(FILE *indexFile, char type){
    struct bTree *tree = malloc(sizeof(struct bTree));
    fread(&tree->status, sizeof(char), 1, indexFile);
    fread(&tree->rootRRN, sizeof(int), 1, indexFile);
    fread(&tree->nextRRN, sizeof(int), 1, indexFile);
    fread(&tree->numNodes, sizeof(int), 1, indexFile);
    tree->root = readNode(indexFile, tree->rootRRN, type);
    return tree;
}


/**
 * @brief cria um novo nó tipo folha e inicializa chaves e descendentes
 * 
 * @return struct bNode* 
 */
struct bNode *newNode(char type){
    struct bNode *root = malloc(sizeof(struct bNode));
    root->nodeType = LEAF;
    root->numKeys = 0;
    //inicia todas as chaves
    for(int i = 0; i <= MAX_KEYS; i++){
        root->key[i].id = -1;
        if(type == '1')
            root->key[i].value.rrn = -1;
        else
            root->key[i].value.byteOffset = -1;
    }
    //inicia todos os descendentes
    for(int i = 0; i <= MAX_DESC; i++){
        root->desc[i] = -1;
    }
    return root;
}

/**
 * @brief inicializa o cabeçalho de uma árvore B
 * 
 * @param indexFile 
 * @param type 
 * @return struct bTree* 
 */
struct bTree *create_tree_header(FILE *indexFile, char type){
    struct bTree *tree = malloc(sizeof(struct bTree));
    tree->status = '0';
    tree->rootRRN = -1;
    tree->nextRRN = 0;
    tree->numNodes = 0;
    tree->root = newNode(type);
    tree->root->nodeType = ROOT;
    return tree;
}

/**
 * @brief lê um nó do arquivo de árvore B
 * 
 * @param indexFile 
 * @param nodeRRN 
 * @param type 
 * @return struct bNode* 
 */
struct bNode *readNode(FILE *indexFile, int nodeRRN, char type){
    //aloca espaço e pesquisa pelo registro no arquivo
    struct bNode *node = newNode(type);
    node->rrn = nodeRRN;
    int headerSize;
    if(type == '1') 
        headerSize = 45;
    else 
        headerSize = 57;
    long int byteOffset = headerSize+(nodeRRN*headerSize);
    fseek(indexFile, byteOffset, SEEK_SET);
    //faz a leitura do tipo do nó e do número de chaves
    fread(&node->nodeType, sizeof(char), 1, indexFile);
    fread(&node->numKeys, sizeof(int), 1, indexFile);
    int i;
    //leitura das chaves
    for(i = 0; i < MAX_KEYS; i++){
        Key k;
        fread(&k.id, sizeof(int), 1, indexFile);
        if(type == '1')
            fread(&k.value.rrn, sizeof(int), 1, indexFile);
        else
            fread(&k.value.byteOffset, sizeof(long int), 1, indexFile);
        node->key[i] = k;
    }
    //pula para a parte dos descendentes no registro

    //leitura dos descendentes
    for(i = 0; i < MAX_DESC; i++){
        fread(&node->desc[i], sizeof(int), 1, indexFile);
    }
    return node;
}

/**
 * @brief escreve o nó no arquivo de árvore-B
 * 
 * @param indexFile arquivo da árvore-B
 * @param node nó à ser escrito
 */
void writeNode(FILE *indexFile, struct bNode *node, char type){
    long int byteOffset;
    if(type == '1')
        byteOffset = 45+(node->rrn*45);
    else
        byteOffset = 57+(node->rrn*57);
    fseek(indexFile, byteOffset, SEEK_SET);

    fwrite(&node->nodeType, sizeof(char), 1, indexFile);
    fwrite(&node->numKeys, sizeof(int), 1, indexFile);
    for(int i = 0; i < MAX_KEYS; i++){
        if(type == '1'){
            if(node->key[i].value.rrn != -1){
                fwrite(&node->key[i].id, sizeof(int), 1, indexFile);
                fwrite(&node->key[i].value.rrn, sizeof(int), 1, indexFile);
            }
            else{
                int nullValue = -1;
                fwrite(&nullValue, sizeof(int), 1, indexFile);
                fwrite(&nullValue, sizeof(int), 1, indexFile);
            }
        }
        else{
            if(node->key[i].value.byteOffset != -1){
                fwrite(&node->key[i].id, sizeof(int), 1, indexFile);
                fwrite(&node->key[i].value.byteOffset, sizeof(long int), 1, indexFile);
            }
            else{
                long int nullValue = -1;
                fwrite(&nullValue, sizeof(int), 1, indexFile);
                fwrite(&nullValue, sizeof(long int), 1, indexFile);
            }
        }
    }

    for(int i = 0; i < MAX_DESC; i++)
        fwrite(&node->desc[i], sizeof(int), 1, indexFile);
}

/**
 * @brief escreve o header do arquivo de árvore B
 * 
 * @param indexFile 
 * @param tree 
 * @param type 
 */
void write_header_btree(FILE *indexFile, struct bTree *tree, char type){
    char status = '0';
    fwrite(&status, sizeof(char), 1, indexFile);
    fwrite(&tree->rootRRN, sizeof(int), 1, indexFile);
    fwrite(&tree->nextRRN, sizeof(int), 1, indexFile);
    fwrite(&tree->numNodes, sizeof(int), 1, indexFile);
    char garbage = '$';
    int garbageSize = 32;
    if(type == '2') garbageSize = 44;
    for(int i = 0; i < garbageSize; i++)
        fwrite(&garbage, sizeof(char), 1, indexFile);
}

/**
 * @brief Copia as chaves e descendentes de src para dst
 * 
 * @param dst destino
 * @param src origem
 * @param dstStart a partir de qual chave deve-se copiar
 * @param srcStart a partir de qual chave deve-se copiar
 * @param nKeys número de chaves que devem ser copiadas 
 * @return struct bNode* dst com os dados copiados
 */
struct bNode *copyKeys(struct bNode *dst, struct bNode *src, int dstStart, int srcStart, int nKeys){
    dst->desc[dstStart] = src->desc[srcStart];
    for(int i = 0; i < nKeys; i++){
        dst->desc[dstStart+i+1] = src->desc[srcStart+i+1];
        dst->key[dstStart+i] = src->key[srcStart+i];
    }
    return dst;
}

/**
 * @brief realiza o merge de backNode em upNode na posição i
 * 
 * @param upNode 
 * @param backNode 
 * @param i 
 * @return struct bNode* 
 */
struct bNode *mergeNode(struct bNode *upNode, struct bNode *backNode, int i){
    if(upNode->key[i].id != INF){
        //aloca espaço para inserir o novo nó
        for(int j = upNode->numKeys; j > i; j--){
            upNode->desc[j] = upNode->desc[j-1];
            upNode->key[j] = upNode->key[j-1];
        }
    }
    upNode = copyKeys(upNode, backNode, i, 0, 1);
    upNode->numKeys++;
    upNode->nodeType = backNode->nodeType;
    return upNode;
}

/**
 * @brief cria um novo RRN para um novo nó da árvore B e atualiza os campos no header
 * 
 * @param indexFile 
 * @return int 
 */
int newRRN(FILE *indexFile){
    //pulando status e nó raiz
    fseek(indexFile, 5, SEEK_SET);

    //lendo número de nós e próx RRN
    int nextRRN, numNodes;
    fread(&nextRRN, sizeof(int), 1, indexFile);
    fread(&numNodes, sizeof(int), 1, indexFile);

    //incrementando ambos
    nextRRN++;
    numNodes++;

    //escrevendo de volta os valores atualizados
    fseek(indexFile, 5, SEEK_SET);
    fwrite(&nextRRN, sizeof(int), 1, indexFile);
    fwrite(&numNodes, sizeof(int), 1, indexFile);
    //retornando novo RRN
    return nextRRN-1;
}

/**
 * @brief Insere uma chave na arvore b
 * 
 * @param node raiz da árvore
 * @param new novo id a ser inserido
 * @return struct bNode* 
 */
struct bNode *insertKey(FILE *indexFile, struct bNode *node, Key new){
    struct bNode *backNode;
    if((node->nodeType == ROOT && node->desc[0] == -1) || (node->nodeType == LEAF)){
        //se couber mais elementos no nó nós adicionamos
        if(node->numKeys < MAX_KEYS){
            node->key[node->numKeys] = new;
            node->numKeys++;
            //ordenando
            qsort(node->key, node->numKeys, sizeof(Key), compareKeys);
            //inserindo no arquivo de dados
            writeNode(indexFile, node, new.fileType);
            return NULL;
        }
        //se não faremos um split
        //inicializa backNode para o split
        backNode = newNode(new.fileType);
        backNode->key[0] = new;
        backNode->numKeys++;
    }else{
        //se o nó não for uma folha, achamos o descendente que devemos acessar
        
        int i = 0;
        while(i < node->numKeys && new.id > node->key[i].id)
            i++; 

        //achando o descendente, descemos a árvore para achar um nó folha
        struct bNode *descNode = readNode(indexFile, node->desc[i], new.fileType);
        // printf("nó a esquerda dentro do else");
        // printNode(readNode(indexFile, node->desc[0], new.fileType));
        backNode = insertKey(indexFile, descNode, new);
        free(descNode);

        //se o retorno for nulo, a operação de inserção no nó folha foi bem sucedida
        if(!backNode)
            return NULL;
        
        //caso o retorno seja diferente de nulo, houve split e recebemos o nó backNode
        //se houver espaço no nó atual, realiza um merge de node e backNode
        else if(node->numKeys < MAX_KEYS){
            //se o espaço estiver livre para inserção
            if(node->nodeType == ROOT)
                backNode->nodeType = ROOT;
            mergeNode(node, backNode, i);
            writeNode(indexFile, node, new.fileType);
            free(backNode);
            //free(backNode);
            return NULL;
        }
    }

    //se a função chegar até aqui, é sinal que deve-se splitar
    //cria o nó superior
    struct bNode *upNode = newNode(new.fileType);
    upNode->numKeys = 3;
    for(int i = 0; i < MAX_KEYS; i++)
        upNode->key[i] = node->key[i];

    for(int i = 0; i < MAX_DESC; i++)
        upNode->desc[i] = node->desc[i];
        
    int i = 0;
    while(i < upNode->numKeys && new.id > upNode->key[i].id)
        i++;
        
    mergeNode(upNode, backNode, i);
    //  O split pode ser entendido como
    //  <-- ^ ->
    //  0 1 2 3
    // | | | | |
        
    //cria o nó a esquerda
    struct bNode *leftNode = newNode(new.fileType);
    leftNode->numKeys = 2;
    leftNode = copyKeys(leftNode, upNode, 0, 0, leftNode->numKeys);
    leftNode->rrn = node->rrn;

    //cria o nó a direita
    struct bNode *rightNode = newNode(new.fileType);
    rightNode->numKeys = 1;
    rightNode = copyKeys(rightNode, upNode, 0, 3, rightNode->numKeys);
    rightNode->rrn = newRRN(indexFile);
    
    //se o nó atual for intermediario, atualiza o tipo dos nós filhos
    if(upNode->nodeType == MID){
        leftNode->nodeType = MID;
        rightNode->nodeType = MID;
    }

    //atualiza o upNode
    upNode->key[0] = upNode->key[2];
    upNode->desc[0] = leftNode->rrn;
    upNode->desc[1] = rightNode->rrn;
    for(int i = 1; i <= MAX_KEYS; i++){
        upNode->key[i].id = -1;
        if(new.fileType == '1')
            upNode->key[i].value.rrn = -1;
        else
            upNode->key[i].value.byteOffset = -1;
    }
    for(int i = 2; i <= MAX_DESC; i++){
        upNode->desc[i] = -1;
    }

    //seta o número de chaves para 1
    upNode->numKeys = 1;
    //vemos se estamos na raiz
    if(node->nodeType == ROOT){
        //se estivermos, atualizamos o tipo
        upNode->nodeType = ROOT;
        //mudamos o rrn do nó da direita
        upNode->rrn = rightNode->rrn;
        rightNode->rrn = newRRN(indexFile);
        upNode->desc[1] = rightNode->rrn;
        //e atualizamos o cabeçalho
        //pulando o status
        fseek(indexFile, 1, SEEK_SET);
        fwrite(&upNode->rrn, sizeof(int), 1, indexFile);

        //por último, escremos a nova raiz no arquivo
        writeNode(indexFile, upNode, new.fileType);

    }
    else
        upNode->nodeType = MID;

    //escreve ambos os nós nos locais corretos
    writeNode(indexFile, leftNode, new.fileType);
    writeNode(indexFile, rightNode, new.fileType);

    //liberando memória
    free(leftNode);
    free(rightNode);
    free(backNode);
            
    return upNode;
}

/**
 * @brief procura por um ID na árvore B
 * 
 * @param indexFile 
 * @param node 
 * @param id 
 * @param type 
 * @return Key 
 */
Key searchID(FILE *indexFile, struct bNode *node, int id, char type){
    // printNode(node);
    if(node->nodeType == LEAF){
        for(int i = 0; i < node->numKeys; i++)
            if(node->key[i].id == id)
                return node->key[i];
    }

    int i = 0;
    while(i < node->numKeys && id > node->key[i].id)
        i++;

    if(id == node->key[i].id)
        return node->key[i];

    struct bNode *desc = readNode(indexFile, node->desc[i], type);
    Key foundKey = searchID(indexFile, desc, id, type);
    free(desc);
    return foundKey;
}

/**
 * @brief libera memória de uma struct bTree
 * 
 * @param tree 
 */
void destroyTree(struct bTree *tree){
    free(tree->root);
    free(tree);
}

/**
 * @brief insere o primeiro nó no arquivo da árvore B
 * 
 * @param indexFile 
 * @param tree 
 * @param newKey 
 * @param type 
 */
void insert_first_node(FILE *indexFile, struct bTree *tree, Key newKey, char type){
    tree->root->key[0] = newKey;
    tree->root->rrn = 0;
    tree->root->numKeys++; // = 1
    writeNode(indexFile, tree->root, type);
}   

/**
 * @brief cria um arquivo de árvore B do tipo 1 para um certo arquivo de dados
 * 
 * @param dataFile 
 * @param indexFile 
 */
void create_btree_t1(FILE *dataFile, FILE *indexFile){
    //inicializando btree
    char type = '1';
    struct bTree *tree = create_tree_header(indexFile, type);
    // pulando header
    fseek(dataFile, 182, SEEK_SET);

    tree->root->rrn = 0;
    char removed;
    int currRRN = 0;
    int firstNode = 1;
    int notEOF = fread(&removed, sizeof(char), 1, dataFile);
    while(notEOF){
        if (removed == '0'){
            // pegando ID do arquivo de dados
            registry_data_st registry = read_registry_t1(dataFile);
            // alocando memória
            Key data;
            data.id = registry.id;
            data.value.rrn = currRRN;
            data.fileType = type;
            if(firstNode){
                //atualizando header para a primeira inserção
                tree->rootRRN = 0;
                tree->nextRRN = 1;
                tree->numNodes = 1;
                write_header_btree(indexFile, tree, type);
                insert_first_node(indexFile, tree, data, type);
            }else{
                //se não for o primeiro nó, fazemos uma inserção normal
                struct bNode *node = insertKey(indexFile, tree->root, data);
                if(node){
                    free(tree->root);
                    tree->root = node;
                }
            }
            firstNode = 0;
            fseek(indexFile, 0, SEEK_SET);

            // pulando lixo
            do{
                notEOF = fread(&removed, sizeof(char), 1, dataFile);
            } while (removed == '$' && notEOF);
        }
        else
        {
            // pulando o registro caso ele esteja marcado como removido
            fseek(dataFile, 96, SEEK_CUR);
            notEOF = fread(&removed, sizeof(char), 1, dataFile);
        }
        currRRN++;
    }
    destroyTree(tree);
}

/**
 * @brief cria um arquivo de árvore B do tipo 2 para um certo arquivo de dados
 * 
 * @param dataFile 
 * @param indexFile 
 */
void create_btree_t2(FILE *dataFile, FILE *indexFile){
    //inicializando struct
    char type = '2';
    struct bTree *tree = create_tree_header(indexFile, type);
    int totalOffset = 190; //header size
    // pulando header
    fseek(dataFile, totalOffset, SEEK_SET);

    char removed;
    int registrySize;
    int firstNode = 1;
    int notEOF = fread(&removed, sizeof(char), 1, dataFile);
    while(notEOF){
        //lendo tamanho do registro
        fread(&registrySize, sizeof(int), 1, dataFile);
        //adicionando o inicio do registro no tamanho total
        registrySize += 5;
        if(removed == '0'){
            //pegando ID do arquivo de dados
            registry_data_st registry = read_registry_t2(dataFile);
            //alocando memória
            Key data;
            data.id = registry.id;
            data.value.byteOffset = totalOffset;
            data.fileType = type;
            if(firstNode){
                //atualizando header para a primeira inserção
                tree->rootRRN = 0;
                tree->nextRRN = 1;
                tree->numNodes = 1;
                write_header_btree(indexFile, tree, type);
                insert_first_node(indexFile, tree, data, type);
            }else{
                //se não for o primeiro nó, fazemos uma inserção normal
                struct bNode *node = insertKey(indexFile, tree->root, data);
                if(node){
                    free(tree->root);
                    tree->root = node;
                }
            }
            firstNode = 0;
        }else{
            fseek(dataFile, registrySize-5, SEEK_CUR);
        }
        
        totalOffset += registrySize;
        notEOF = fread(&removed, sizeof(char), 1, dataFile);
        while(removed == '$' && notEOF)
            notEOF = fread(&removed, sizeof(char), 1, dataFile);
    }
    destroyTree(tree);
}