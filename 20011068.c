#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_{
	char *color;
	int data;
	struct linked_ *next;
}LinkedList;

typedef struct HASH_TABLE{
	int size;
	LinkedList **start;
}hash_table;

void rotateRight(int *board,int N);
int rcr(int **board,int m, int N,int mode);
int isValid(int **board,int m,int N);
void clearArr(int *arr,int n);
void printMatrix(int **board,int size);
hash_table *createHashTable(int size);
int stringHash(hash_table *hsh,char* strng);
void addString(hash_table *hsh,char* strng);
void giveNumbers(hash_table *hsh,int,char**);
int findHash(hash_table *hsh,char* strng);
void simple_mode(char ***mxS,int n);
void detailed_mode(char ***mxS,int n);

int main(){
	int i,j,m,n,mod;
	char helper[100];
	char **colorList;
	printf("Matrisin genisligini giriniz: (3<N<8)\n N:");
	scanf("%d",&n);
	printf("\n String Matrisini Giriniz:\n");
	fflush(stdin);
	char ***mxS = (char***) malloc(n*sizeof(char**));
	for(i = 0; i<n;i++){
    	mxS[i] = (char**) malloc(n*sizeof(char*));
    	fgets(helper,100,stdin);
    	mxS[i][0]=strdup(strtok(helper," "));
    	for(j=1;j<n;j++){
    		mxS[i][j]=strdup(strtok(NULL," "));
		}
		mxS[i][j-1][strlen(mxS[i][j-1])-1] = '\0';
	}
	printf("Detay mod:1\nnormal mod:0\n:");
	scanf("%d",&mod);
	if(mod == 0){
		simple_mode(mxS,n);
	}else if(mod == 1){
		detailed_mode(mxS,n);
	}
	
}
void simple_mode(char ***mxS,int n){
	int i,j;
	char **colorList;
	hash_table *hashTable = createHashTable(n*n);
	for(j=0;j<n;j++){
		addString(hashTable,mxS[0][j]);
	}
	colorList = (char**) malloc((n+1)*sizeof(char*));
	giveNumbers(hashTable,n,colorList);
	
	int **mx = (int**) malloc(n*sizeof(int*));
	for(i = 0; i<n;i++){
    	mx[i] = (int*) malloc(n*sizeof(int));
    	for(j=0;j<n;j++){
    		mx[i][j] = findHash(hashTable,mxS[i][j]); 
		}
	}
	if(rcr(mx,0,n,0)){
		printf("\nSonuc Bulundu..\n");
		for(i = 0; i<n;i++){
	    	for(j = 0;j<n;j++){
	    		printf("%s ",colorList[mx[i][j]]);
			}
		printf("\n");
		}
	}
	else{
		printf("\n\nSonuc bulunamadi");
	}
	
	
}
void detailed_mode(char ***mxS,int n){
	int i,j;
	char **colorList;
	hash_table *hashTable = createHashTable(n*n);
	for(j=0;j<n;j++){
		addString(hashTable,mxS[0][j]);
	}
	colorList = (char**) malloc((n+1)*sizeof(char*));
	giveNumbers(hashTable,n,colorList);
	
	int **mx = (int**) malloc(n*sizeof(int*));
	for(i = 0; i<n;i++){
    	mx[i] = (int*) malloc(n*sizeof(int));
    	for(j=0;j<n;j++){
    		mx[i][j] = findHash(hashTable,mxS[i][j]); 
		}
	}
	if(rcr(mx,0,n,1)){
		printf("\nSonuc Bulundu..\n");
		for(i = 0; i<n;i++){
	    	for(j = 0;j<n;j++){
	    		printf("%s ",colorList[mx[i][j]]);
			}
		printf("\n");
		}
	}
	else{
		printf("\n\nSonuc bulunamadi");
	}
}

int rcr(int **board,int m, int N,int mode){
	int i,j,tmp,k = 0;
	if(m>=N){
		return 1;
	}
	while(k<N){
		if(!isValid(board,m,N) || !rcr(board,m+1,N,mode)){
			k++;
			rotateRight(board[m],N);
			if(mode==1){
				printf("\n");
				printMatrix(board,N);	
			}
		}else{
			return 1;
		}
	}
	return 0;
}

void rotateRight(int *board,int N){
	char tmp = board[N-1];
	int i;
	for(i=N-1;i>0;i--){
		board[i] = board[i-1];
	}
	board[0] = tmp;
}

int isValid(int **board,int m,int N){
	int i,j,k;
	if(m<=0){
		return 1;
	}
	int *hlpr = (int*) malloc(sizeof(int)*(N+2));
	for(i = 0;i<N;i++){
		clearArr(hlpr,N+1);
		for(j = 0;j<=m;j++){
			if( ++hlpr[board[j][i]] > 1){
				return 0;
			} 
		}
	}
	free(hlpr);
}
void clearArr(int *arr,int n){
	int i;
	for(i = 0;i<n;i++){
		arr[i] = 0;
	}
}
void printMatrix(int **board,int size){
	int i,j;
	for(i = 0;i<size;i++){
		for(j = 0;j<size;j++){
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}
////////////////////////////////////////////////////////hash Functions
hash_table *createHashTable(int size){
	int i;
	hash_table *hsh = (hash_table*) malloc(sizeof(hash_table));
	hsh->size = size;
	hsh->start = (LinkedList**) malloc(size*sizeof(LinkedList*));
	for(i = 0;i<size;i++){
		hsh->start[i] = NULL;
	}
	return hsh;
}

int stringHash(hash_table *hsh,char* strng){
	int sum=0,tmp = 1,i;
	for(i = 0;i<strlen(strng);i++){
		if(strng[i]!='\0' && strng[i]!='\n'){
			sum+= strng[i]*tmp;
			tmp*=2; 
		}
		
	}
	return sum%hsh->size;
}
void addString(hash_table *hsh,char* strng){
	int val = stringHash(hsh,strng);
	LinkedList *tmp = hsh->start[val];
	while(tmp !=NULL && strcmp(tmp->color,strng) != 0){
		tmp = tmp->next;
	}
	if(tmp ==NULL){
		LinkedList *hlpr = (LinkedList*) malloc(sizeof(LinkedList));
		hlpr->color = strdup(strng);
		hlpr->data = -1;
		hlpr->next = hsh->start[val];
		hsh->start[val] = hlpr;
	}
}
void giveNumbers(hash_table *hsh,int n,char **arr){
	int i,j=1;
	LinkedList *tmp;
	for(i = 0;i< hsh->size;i++){
		for(tmp = hsh->start[i];tmp!=NULL;tmp = tmp->next){
			tmp->data = j++;
			printf("\n%s : %d",tmp->color,tmp->data);
			arr[tmp->data] = strdup(tmp->color);
		}
	}
}
int findHash(hash_table *hsh,char* strng){
	int val = stringHash(hsh,strng);
	LinkedList *tmp = hsh->start[val];
	while(tmp !=NULL && strcmp(tmp->color,strng) != 0){
		tmp = tmp->next;
	}
	if(strcmp(tmp->color,strng) == 0){
		return tmp->data;
	}
	printf("Hata findHash deger Bulamadi!!\n");
	return -1;
}
