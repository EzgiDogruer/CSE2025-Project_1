
//EZGÝ DOÐRUER 150117042
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string.h>
#include <locale.h>
#include <math.h>

#define MAXCHAR 1000

struct node{
	char str [MAXCHAR] ;
    int count;
    int n;
    float idf;
    struct node *next;
};
typedef struct node node;
typedef struct node* nodePtr;
typedef struct node ** nodePtrPtr;

struct nodeFile{
    char fName [MAXCHAR];
    nodePtr terms;//link listi tutcak
    struct nodeFile *next;
};
typedef struct nodeFile nodeFile;
typedef struct nodeFile* nodeFilePtr;
typedef struct nodeFile ** nodeFilePtrPtr;

struct nodePath{
	char term1 [MAXCHAR] ;
	char term2 [MAXCHAR] ;
    struct nodePath *next;
};
typedef struct nodePath nodePath;
typedef struct nodePath* nodePathPtr;
typedef struct nodePath ** nodePathPtrPtr;

void insertPath(nodePathPtrPtr header ,char term1[],char term2[]) {
	nodePathPtr newNode;
	// create node to insert and assign values to its fields
	newNode=(nodePathPtr)malloc(sizeof(nodePath));
	strcpy(newNode->term1,term1);
	strcpy(newNode->term2,term2);
	newNode->next=NULL;

	// if LL empty
	if (*header == NULL)
		*header=newNode;
	// if LL not empty
	else {
		newNode->next= *header;
		*header = newNode;
	}
}

int onlyExistNode(nodePtr header, char str[]) {
   	nodePtr temp = header;
	while (temp!=NULL) {
	   	if (strcmp(temp->str,str)==0) {
	   		return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int existNode(nodePtrPtr header, char str[],int cnt) {
   	nodePtr temp = *header;
	while (temp!=NULL) {
	   	if (strcmp(temp->str,str)==0) {
	   		temp->count = temp->count + cnt;
	   		return 1;
		}
		temp = temp->next;
	}
	return 0;
}


void insert(nodePtrPtr header, char str[],int cnt) {
	nodePtr newNode;
	// create node to insert and assign values to its fields
	newNode=(nodePtr)malloc(sizeof(node));
	strcpy(newNode->str,str);
	newNode->next=NULL;
	newNode->count=cnt;
	newNode->n = 0;
	// if LL emptys
	if (*header == NULL)
		*header=newNode;
	// if LL not empty
	else {
		newNode->next= *header;
		*header = newNode;
	}
}

void insertFile(nodeFilePtrPtr header,char* dirName,char* fileName ){
    FILE* fp;
	int cnt;
	char *fName=(char *)malloc(sizeof(dirName)+sizeof(fileName));
	fName[0]='\0';
	strcat(fName,dirName);
	strcat(fName,fileName);
	fp = fopen(fName, "r");
    if (fp == NULL){
        printf("Could not open file %s\n",fName);
        return;
    }
    // printf("--- %s\n",fName);

    nodeFilePtr newNode;
	// create node to insert and assign values to its fields
	newNode=(nodeFilePtr)malloc(sizeof(nodeFile));
	strcpy(newNode->fName,fName);
	char str[MAXCHAR];
	nodePtr hdr;
	hdr = NULL;
	while (fscanf(fp, "%s",str) == 1) {
		cnt = existNode(&hdr,str,1);
		if (cnt==0) {
        	insert(&hdr,str,1);
		}
	}

    newNode->terms=hdr;
	newNode->next=NULL;

	// if LL empty
	if (*header == NULL)
		*header=newNode;
	// if LL not empty
	else {
		newNode->next= *header;
		*header = newNode;
	}

    fclose(fp);
}

void insertFirstPath(nodePathPtrPtr header, nodeFilePtr nodeF) {
    nodePtr temp = nodeF->terms;
    nodePtr temp2 = temp->next;
    while(temp->next != NULL) {
        temp2 = temp->next;
        while(temp2!=NULL) {
            insertPath(header,temp->str,temp2->str);
            temp2=temp2->next;
        }
        temp=temp->next;
    }
}

void sortNodeListB(nodePtrPtr header) {
    int cnt;
    int nDoc;
    float id;
	char str[MAXCHAR];
    nodePtr temp1;
    nodePtr temp2;

    for(temp1=*header;temp1!=NULL;temp1=temp1->next) {
		for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next) {
            if(temp2->count > temp1->count) {
                cnt = temp1->count;
                temp1->count = temp2->count;
                temp2->count = cnt;
                nDoc = temp1->n;
                temp1->n = temp2->n;
                temp2->n = nDoc;
                id = temp1->idf;
                temp1->idf = temp2->idf;
                temp2->idf = id;
                strcpy(str,temp1->str);
                strcpy(temp1->str,temp2->str);
                strcpy(temp2->str,str);
            }
        }
    }
}


void sortNodeListC(nodePtrPtr header) {
    int cnt;
    int nDoc;
    float id;
	char str[MAXCHAR];
    nodePtr temp1;
    nodePtr temp2;

    for(temp1=*header;temp1!=NULL;temp1=temp1->next) {
        for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next) {
            if(temp2->idf > temp1->idf) {
                cnt = temp1->count;
                temp1->count = temp2->count;
                temp2->count = cnt;
                nDoc = temp1->n;
                temp1->n = temp2->n;
                temp2->n = nDoc;
                id = temp1->idf;
                temp1->idf = temp2->idf;
                temp2->idf = id;
                strcpy(str,temp1->str);
                strcpy(temp1->str,temp2->str);
                strcpy(temp2->str,str);
            }
        }
    }
}

void printList(nodePtr header) {
    nodePtr temp;
    temp=header;
    while(temp!=NULL) {
        printf("%s %d %d %f\n",temp->str,temp->count,temp->n, temp->idf);
        temp =temp->next;
    }
}

int docCount(nodeFilePtr header) { // N
	nodeFilePtr tempFile=header;
	int cnt=0;
	while(tempFile!=NULL) {
		cnt++;
		tempFile=tempFile->next;
	}
	return cnt;
}

void frequencyList(nodeFilePtr header) {
	nodePtr hdr;
	hdr = NULL;
	nodePtr tempNode;
	nodePtr tempHdr;
	nodeFilePtr tempFile=header;
	int cnt;
	while(tempFile!=NULL) {
		tempNode=tempFile->terms;
		while(tempNode!=NULL) {
			cnt = existNode(&hdr,tempNode->str,tempNode->count);
			if(cnt==0) {
				insert(&hdr,tempNode->str,tempNode->count);
			}
			tempNode=tempNode->next;
		}
		tempFile=tempFile->next;
	}

	tempHdr=hdr;
	int dC = docCount(header);
	while(tempHdr!=NULL) {
		tempFile = header;
		tempHdr->n = 0;
		while(tempFile!=NULL) {
			tempNode=tempFile->terms;
			cnt = onlyExistNode(tempNode,tempHdr->str);
			if(cnt==1) {
				tempHdr->n = tempHdr->n + 1;
			}
			tempFile=tempFile->next;
		}
		tempHdr->idf = tempHdr->count * (log(dC/(float)tempHdr->n));
		tempHdr=tempHdr->next;
	}

	// printList(hdr);
	// printf("\n");

	sortNodeListB(&hdr);
	// printList(hdr);
	// printf("\n");

    nodePtr temp;
    temp=hdr;
    int i;
    for(i=0;(i<5) && (temp!=NULL);i++) {
        printf("%s;%d\n",temp->str,temp->count);
        temp =temp->next;
    }
	printf("\n");
	sortNodeListC(&hdr);
	// printList(hdr);
	// printf("\n");

    temp=hdr;
    for(i=0;(i<5) && (temp!=NULL);i++) {
        printf("%s;%f\n",temp->str,temp->idf);
        temp =temp->next;
    }
	printf("\n");
}

void printFiles(nodeFilePtr header) {
    nodeFilePtr temp;
    temp=header;
    while(temp!=NULL) {
        printf("%s\n",temp->fName);
        printList(temp->terms);
        temp =temp->next;
    }
}

void printPathList(nodePathPtr header) {
    nodePathPtr temp;
    temp=header;
    while(temp!=NULL) {
        printf("{%s,%s}, ",temp->term1,temp->term2);
        temp =temp->next;
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
	setlocale(LC_ALL,"Turkish");
	nodeFilePtr hdr=NULL;
	DIR* dir;
	char* dirName="."; //"""econ/";
    struct dirent *inFile;
	dir = opendir(dirName);
	if(dir==NULL){
        printf("Error! Unable to read directory");
        return 1;
    }
    while( (inFile=readdir(dir)) != NULL) {
        if (!strcmp (inFile->d_name, "."))
            continue;
        if (!strcmp (inFile->d_name, ".."))
            continue;

        struct stat st;
		stat(inFile->d_name,&st);
        if (S_ISDIR(st.st_mode)) {
        	char *dirName2 = (char *)malloc(sizeof(inFile->d_name)+sizeof(char));
    		dirName2[0]='\0';
			strcat(dirName2,inFile->d_name);
			strcat(dirName2,"\\");
        	//printf("%s\n",dirName2);

			DIR* dir2;
    		struct dirent *inFile2;
			dir2 = opendir(dirName2);
			if(dir2==NULL){
        		printf("Error! Unable to read directory");
        		return 1;
    		}
    		while( (inFile2=readdir(dir2)) != NULL) {
        		if (!strcmp (inFile2->d_name, "."))
            		continue;
        		if (!strcmp (inFile2->d_name, ".."))
            		continue;
        		//printf("%s %s\n",dirName2,inFile2->d_name);
        		insertFile( &hdr,dirName2,inFile2->d_name);
			}
			closedir(dir2);
		}
	}
	closedir(dir);

    //printFiles(hdr);
    //printf("\n");
    nodePathPtr hdrFirst=NULL;
    nodePathPtr hdrSecond=NULL;
    nodePathPtr hdrThird=NULL;
    nodeFilePtr temp = hdr;
    while(temp!=NULL) {
        insertFirstPath(&hdrFirst,temp);
        temp = temp->next;
    }
    printf("First-order term co-occurence:");
    printPathList(hdrFirst);
    printf("\n");
	frequencyList(hdr);

	return 0;
}

