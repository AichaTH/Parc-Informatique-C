#include <stdio.h>
#include <stdlib.h>
#include <string.h> //définit fonct manipulation string
#include <ctype.h>  //définit toupper() et tolower()
#include <stdbool.h>  

typedef char* string;
typedef struct machine{
    char name[50];
    char address[4];
    bool state; //connecté ou !connecté
    string *log_client; //liste logiciel client
    string *log_server;
    struct machine *next;	
}
machine; //structure macchine
machine *ajouter_machine(machine *list){
    machine *m= malloc(sizeof(machine));
    if(m!=NULL){
	//récupère nom machine
        char name[50];
        printf("Entrer le nom de la machine:\t");
        scanf("%s",name);
        strcpy(m->name, name);	//copie nom machine
	//récupère liste logiciel client
        string *client=(string*) malloc(sizeof(string));//alloue espace pour tableau de string
        if (client!=NULL){
            int i,j=0;
            char c[100];
            do{
                printf("Taper 1 pour ajouter un logiciel client à la machine ou un autre chiffre sinon: ");
                scanf("%d", &i);
                if(i==1){	//si on veut ajouter un logiciel client
                    if(j==0){	//si on c'est le premier logiciel
                        
                        printf("Entrer le logiciel client:\t");
                        scanf("%s",c);
                        client[j]=(string)malloc(strlen(c)+1);//on alloue un espace de la taille du saisie
                        if(client[j]!=NULL){
                            strcpy(client[j], c);	//on le copie dans le tableau de string
                        }
                        j++;	//incremente
                    }	
                    else{	//c'est pas le premier logiciel client
                        client=realloc(client,(j+1)*sizeof(string));	//on réalloue de l'espace our stocker un autre string
                        printf("Entrer le logiciel client:\t");
                        scanf("%s",c);
                        client[j]=(string)malloc(strlen(c)+1);
                        if(client[j]!=NULL){
                            strcpy(client[j], c);
                        }
                        j++;
                    }
                }
            }while(i==1);	//si on veut plus ajouter de logiciel client on quitte la boucle
            m->log_client=client;	//on initialise liste de logiciel client
        }
	//même procédé avec les logiciel client
        string *serveur=(string*) malloc(sizeof(string));
        if (serveur!=NULL){
            int i,j=0;
            char c[50];
            do{
                printf("Taper 1 pour ajouter un logiciel serveur à la machine ou un autre chiffre sinon: ");
                scanf("%d", &i);
                if(i==1){
                    if(j==0){
                        printf("Entrer le logiciel serveur:\t");
                        scanf("%s",c);
                        serveur[j]=(string)malloc(strlen(c)+1);
                        if(serveur[j]!=NULL){
                            strcpy(serveur[j],c);
                        }
                        j++;
                    }
                    else{
                        serveur=realloc(serveur,(j+1)*sizeof(string));
                        printf("Entrer le logiciel serveur:\t");
                        scanf("%s",c);
                        serveur[j]=(string)malloc(strlen(c)+1);
                        if(serveur[j]!=NULL){
                            strcpy(serveur[j],c);
                        }
                        j++;
                    }
                }
            }while(i==1);
            m->log_server=serveur;
        }
	//initialise état à false
        m->state=false;
        //m->address="abcd";
        m->next=NULL;
        list=m;
    }
    return list;
}
int main(void){
    machine *list=NULL; //list des machines
    list=ajouter_machine(list);
	
    free(list);
    return 0;
}
