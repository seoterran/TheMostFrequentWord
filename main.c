
#include "main.h"

int tests_run;


void findMax(word *lists [BUCKETSIZE][BUCKETSIZE_COL],int *n, int max_count[BUCKETSIZE][BUCKETSIZE_COL], char ** result, int total_max){
    //printf("TOTAL: %d WORDS\n",*n);

    int k=0;
    word* temp;

    for(int i=0;i<BUCKETSIZE; i++){
        for(int j=0;j<BUCKETSIZE_COL; j++){
            temp=lists[i][j];

            while(temp){
                if(temp->count<total_max)
                    break;

                if(temp->count==total_max) {
                    result[k] = malloc(strlen(temp->s));
                    strcpy(result[k], temp->s);
                    //  printf("findMax\t%s\t%s\t%d\n",result[j], temp->s, temp->count);
                    k++;
                }
                temp=temp->next;
            }
         }
    }
    return;
}

word * new_word(char* s){
    int size=strlen(s);

    word* temp = malloc(sizeof(word));
    temp->count=1;
    strcpy ( temp->s, s);

   // printf("new_word %s(%d)\n",temp->s,size);
    temp->next=NULL;
    return temp;
}

static char * test_new_word(){
    char * str="new_word";
    word * temp=new_word(str);

    mu_assert("error, temp->s != new_word", strcmp(temp->s,str)==0);
    mu_assert("error, temp->count != 1", temp->count==1);
    mu_assert("error, temp->next != NULL", temp->next==NULL);

    str="new_word";
    temp=new_word(str);
    mu_assert("error, temp->s != new_word", strcmp(temp->s,str)==0);
    mu_assert("error, temp->count != 1", temp->count==1);
    mu_assert("error, temp->next != NULL", temp->next==NULL);

    str="new";
    temp=new_word(str);
    mu_assert("error, temp->s != new", strcmp(temp->s,str)==0);
    mu_assert("error, temp->count != 1", temp->count==1);
    mu_assert("error, temp->next != NULL", temp->next==NULL);

    return 0;
}

void insertWord ( int *n, char *s, word * lists [BUCKETSIZE][BUCKETSIZE_COL], int max_count[BUCKETSIZE][BUCKETSIZE_COL],int * total_max) {
	int	bucket=strlen(s)%BUCKETSIZE;
    int test=abs((int)*s);
    int bucket_col=test%BUCKETSIZE_COL;
    //printf("insertWord: %d/%s\n",bucket,s);

    word * temp=lists[bucket][bucket_col];
    word * pre;

    (*n)++;

    if (strlen (s) >= MAXWORD) {
        fprintf (stderr, "word too long!\n");
        exit (1);
    }
    if (*n >= MAXWORDS) {
        fprintf (stderr, "too many words!\n");
        exit (1);
    }

    if(temp==NULL) {
        lists[bucket][bucket_col]=new_word(s);
        max_count[bucket][bucket_col]=1;
    } else {

        pre=NULL;
        while (temp) {
            if (strcmp(s, temp->s) == 0) {
                if (temp->count == max_count[bucket][bucket_col]) {
                    temp->count=++max_count[bucket][bucket_col];
                    if(*total_max<temp->count)
                        *total_max= temp->count;
                    if(pre){
                        pre->next=temp->next;
                        temp->next=lists[bucket][bucket_col];
                        lists[bucket][bucket_col]=temp;
                    }
                }else{
                    temp->count++;
                    if (temp->count == max_count[bucket][bucket_col]) {
                        if(pre){
                            pre->next=temp->next;
                            temp->next=lists[bucket][bucket_col];
                            lists[bucket][bucket_col]=temp;
                        }
                    }
                }
                pre=temp;
                return;
            } else {
                pre=temp;
                temp = temp->next;
               // printf("%s\n",s);
            }
        }
        pre->next=new_word(s);
    }
}

void release(word *lists[BUCKETSIZE][BUCKETSIZE_COL] ) {
    for (int i = 0; i < BUCKETSIZE; i++) {
        for (int j = 0; j < BUCKETSIZE_COL; j++) {
            word *temp = lists[i][j];

            while (temp) {
                word *temp2 = temp;
                temp = temp->next;
                free(temp2);
            }
        }
    }
}

static char *test_insertWord(){
    int n=0;
    int max_count[BUCKETSIZE][BUCKETSIZE_COL]={0};
    char str[MAXWORD];
    int total_max=0;
    int bucket_col;
    word * lists[BUCKETSIZE][BUCKETSIZE_COL]={NULL};

    strcpy(str,"cat");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max);//lists[3][9]: cat
    mu_assert("test_insertWord_error, lists[3]->s != cat", strcmp(lists[3][bucket_col]->s,str)==0);
    mu_assert("test_insertWord_error, lists[3]->count != 1", lists[3][bucket_col]->count==1);

    strcpy(str,"neck");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max);//lists[4][0]: neck
    mu_assert("test_insertWord_error, lists[4]->s != neck", strcmp(lists[4][bucket_col]->s,str)==0);
    mu_assert("test_insertWord_error, lists[4]->count != 1", lists[4][bucket_col]->count==1);

    strcpy(str,"aind");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max);//lists[4][7]: aind
    mu_assert("test_insertWord_error, lists[4]->next->s != kind", strcmp(lists[4][bucket_col]->s,str)==0);
    mu_assert("test_insertWord_error, lists[4]->next->count != 1", lists[4][bucket_col]->count==1);

    strcpy(str,"nice");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max); //lists[4][0]: neck->nice
    mu_assert("error, lists[4]->next->next->s != nice", strcmp(lists[4][bucket_col]->next->s,str)==0);
    mu_assert("error, lists[4]->next->next->count != 1", lists[4][bucket_col]->next->count==1);

    strcpy(str,"aind");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max);//lists[4][7] : aind(2)
    mu_assert("test_insertWord_error, lists[4]->s != kind", strcmp(lists[4][bucket_col]->s,str)==0);
    mu_assert("test_insertWord_error, lists[4]->count != 2", lists[4][bucket_col]->count==2);


    strcpy(str,"nice");
    bucket_col=abs((int)*str)%BUCKETSIZE_COL;
    insertWord(&n,str,lists,max_count, &total_max);//lists[4][0]: nice(2)->neck
    mu_assert("test_insertWord_error, lists[4]->s != nice", strcmp(lists[4][bucket_col]->s,str)==0);
    mu_assert("test_insertWord_error, lists[4]->count != 2", lists[4][bucket_col]->count==2);


    release(lists);
    return 0;
}

static char * test_findMax(){
    int n=0;
    int total_max=0;
    int max_count[BUCKETSIZE][BUCKETSIZE_COL]={0};
    char str[MAXWORD];
    char * result[MAXWORD]={NULL};
    word * lists[BUCKETSIZE][BUCKETSIZE_COL]={NULL};

    insertWord(&n,"next",lists,max_count, &total_max);
    insertWord(&n,"cat",lists,max_count, &total_max);
    insertWord(&n,"kind",lists,max_count, &total_max);
    insertWord(&n,"cat",lists,max_count, &total_max);
    insertWord(&n,"kind",lists,max_count, &total_max);

    findMax(lists, &n,max_count,result, total_max);
    mu_assert("test_findMax_error, result[0] != cat", strcmp(result[0],"cat")==0);
    mu_assert("test_findMax_error, result[1] != kind", strcmp(result[1],"kind")==0);
    mu_assert("test_findMax_error, result[2] != NULL", result[2]==NULL);

    release(lists);
    return 0;
}

void toLower(char * str){
    int size=strlen(str);
    for(int i = 0; i<size; i++)
        str[i] = tolower(str[i]);

}
char * allTests(){
    mu_run_test(test_new_word);
    mu_run_test(test_insertWord);
    mu_run_test(test_findMax);
    return 0;
}

void printResult(char ** result){
    printf("\nThe most used word(s) is/are: ");
    for(int i=0;i<MAXWORD;i++){
        if(result[i])
            printf("%s ",result[i]);
    }
    printf("\n\n");
}

int unitTest(){
    char *result = allTests();
    if (result != 0) {
        printf("%s\n", result);
    }else {
        printf("ALL TESTS PASSED\n");
    }
   // printf("Tests run: %d\n", tests_run);

    return result != 0;
}

char * loadFile() {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen("input.txt", "rb");
    if (!fp) perror("input.txt"), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

    fclose(fp);
    return buffer;
}
/* main program */
int main () {

    int total_max=0;
    word * lists[BUCKETSIZE][BUCKETSIZE_COL]={NULL};
    char * result[MAXWORD]={NULL};
    char * str;
    int n = 0;
    int max_count[BUCKETSIZE][BUCKETSIZE_COL]={0};

    const char s[7] = " .!-?,";
    char *token;

    unitTest();
    str=loadFile();

    /* get the first token */
    token = strtok(str, s);
    /* walk through other tokens */
    while( token != NULL ) {
        toLower(token);
        //printf( " %s ", token );
        insertWord ( &n, token, lists, max_count, &total_max);
        token = strtok(NULL, s);
    }

    findMax(lists, &n, max_count, result, total_max);
    printResult(result);
    release(lists);
    free(str);
}
