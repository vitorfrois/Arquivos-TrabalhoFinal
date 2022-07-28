#include "../includes/utils.h"
#include <stdio.h>

int compare_registry(registry_data_st registry, Field *f, int n){
    int isEqual = 1;
    for(int i = 0; i < n; i++){
        if(strcmp(f[i].name, "id") == 0)
            if(atoi(f[i].value) != registry.id)
                isEqual *= 0;

        if(strcmp(f[i].name, "ano") == 0)
            if(atoi(f[i].value) != registry.year)
                isEqual *= 0;

        if(strcmp(f[i].name, "cidade") == 0)
            if(strcmp(f[i].value, registry.city) != 0)
                isEqual *= 0;

        if(strcmp(f[i].name, "marca") == 0)
            if(strcmp(f[i].value, registry.brand) != 0)
                isEqual *= 0;

        if(strcmp(f[i].name, "modelo") == 0)
            if(strcmp(f[i].value, registry.model) != 0)
                isEqual *= 0;

        if(strcmp(f[i].name, "qtt") == 0)
            if(atoi(f[i].value) != registry.amount){
                isEqual *= 0;
			}


        if(strcmp(f[i].name, "sigla") == 0){
            if(f[i].value[0] != registry.initials[0])
                isEqual *= 0;
			if(f[i].value[1] != registry.initials[1])
                isEqual *= 0;
		}
    }
	
    return isEqual;
}

FILE *open_file_rb(const char *filename){
    FILE *file = fopen(filename, "rb");
    if(!file)
        printf("Falha no processamento do arquivo.\n");
	
    return file;
}

FILE *open_file_rplusb(const char *filename){
    FILE *file = fopen(filename, "r+b");
    if(!file){
        printf("Falha no processamento do arquivo.\n");
	}
	
    return file;
}

FILE *open_file_wb(const char *filename){
	FILE *file = fopen(filename, "wb+");
	if(!file){
        printf("Falha no processamento do arquivo.\n");
	}
    return file;
}

void print_registry(registry_data_st registry){
    //brand
    printf("MARCA DO VEICULO: %s\n", registry.brand);
    //model
    if(registry.model[0] != '\0') printf("MODELO DO VEICULO: %s\n", registry.model);
    else printf("MODELO DO VEICULO: NAO PREENCHIDO\n");
    //year
    if(registry.year != -1) printf("ANO DE FABRICACAO: %d\n", registry.year);
    else printf("ANO DE FABRICACAO: NAO PREENCHIDO\n");
    //city
	if(registry.city[0] != '\0') printf("NOME DA CIDADE: %s\n", registry.city);
	else printf("NOME DA CIDADE: NAO PREENCHIDO\n");
    //amount
    if(registry.amount != -1) printf("QUANTIDADE DE VEICULOS: %d\n", registry.amount);
    else printf("QUANTIDADE DE VEICULOS: NAO PREENCHIDO\n");

    //print \n after every register
    printf("\n");
}

void readline(char* string){
    char c = 0;

    do{
        c = (char) getchar();

    } while(c == '\n' || c == '\r');

    int i = 0;

    do{
        string[i] = c;
        i++;
        c = getchar();
    } while(c != '\n' && c != '\r');

    string[i]  = '\0';
}

void binarioNaTela(const char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string_static(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
	R = getchar();
}

char *scan_quote_string(){
	char buffer[50];
	scan_quote_string_static(buffer);
	char *str = (char *)malloc((strlen(buffer)+1) * sizeof(char));
	strcpy(str, buffer);

	return str;
}

int check_status(FILE *file){
	char status = 0;
	fseek(file, 0, SEEK_CUR);
	fread(&status, sizeof(char), 1, file);
    if(status ==  '0'){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
	return 1;
}

void logical_deletion(FILE *file, int offsetSize){
	fseek(file, offsetSize, SEEK_CUR);
	char status = '1';
	fwrite(&status, sizeof(char), 1, file);
}

int search_field_id(Field *f, int n){
	for(int i = 0; i < n; i++)
		if(strcmp(f[i].name, "id") == 0) return i;
		
	return -1;
}

Field *get_field(int x){
	Field *f = malloc(sizeof(Field) * x);
	for(int j = 0; j < x; j++){
		f[j].name = readString();
		f[j].value = scan_quote_string();
	}
	return f;
}

void free_field(Field *f, int x){
	//freeing memory
	for(int j = 0; j < x; j++){
		free(f[j].name);
		free(f[j].value);
	}
	free(f);
}

registry_data_st read_registry_line(){
	//lendo input
	char *id = readString();
	char *year = readString();
	char *amount = readString();
	char *initials = scan_quote_string();
	char *city = scan_quote_string();
	char *brand = scan_quote_string();
	char *model = scan_quote_string();

	//salvando os dados na struct
	registry_data_st data;
	data.id = atoi(id);
	if(strcmp(year, "NULO") == 0)
		data.year = 0;
	else
		data.year = atoi(year);
	if(strcmp(amount, "NULO") == 0)
		data.amount = 0;
	else
		data.amount = atoi(amount);
	strcpy(data.initials, initials);
	strcpy(data.city, city);
	strcpy(data.brand, brand);
	strcpy(data.model, model);

	//liberando memória alocada
	free(id);
	free(year);
	free(amount);
	free(initials);
	free(city);
	free(brand);
	free(model);

	return data;
}