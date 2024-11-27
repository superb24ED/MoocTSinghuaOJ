#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STRING 45
#define MAX_N 60011
#define HASH_MULTI_FACTOR 31

typedef struct Node
{
    char name[MAX_STRING];
    int count;
    //int first_dup;
    struct Node * next;
}Node;

Node * hashTable[MAX_N];
char _s[MAX_STRING];
int hash(const char* _s){
    int len =strlen(_s);
    int hash_value = 0;
    for(int i=0;i<len;i++)
        hash_value = (hash_value*HASH_MULTI_FACTOR + _s[i] )%MAX_N ;
    return hash_value;
}

Node * findInList(Node * list, const char * _s){
    Node * p = list;
    while (p)
    {
        if(0==strcmp(p->name,_s))
            return p;
        p = p->next;
    }
    return NULL;
    
}

Node * addInList(int idx, char * _s){
    Node *p = findInList(hashTable[idx],_s);
    if(p){
        p->count ++;
    }
    else{
        p = (Node*)malloc((sizeof(Node)));
        strcpy(p->name,_s);
        //p->first_dup = -1;
        p->count=1;
        p->next= hashTable[idx];
        hashTable[idx] = p;
    }
    return p;
}

Node * add(char* _s){
    return addInList(hash(_s),_s);
}

void freeNodes(Node *list){
    Node *tmp = list;
    while (list)
    {
        tmp = list->next;
        free(list);
        list = tmp;
    }
}
int main(){
    int n;
    scanf("%d",&n);
    Node * p; 
    for(int i=0;i<n;i++){
        scanf("%s",_s);
        p = add(_s);
        if(p->count == 2){
            printf("%s\n",_s);
        }
    }

    for(int i = 0;i<MAX_N;i++)
        freeNodes(hashTable[i]);
}