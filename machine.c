#include <stdio.h>
#include <stdlib.h>
#include <string.h> //définit fonct manipulation string
#include <ctype.h>  //définit toupper() et tolower()
#include <stdbool.h>  

typedef char* string;

//definition structure machine
typedef struct machine{
	char name[50];
	char MacAddr[6];
	char IpAddr[4];
	char mask[4];
	bool state; //connecté ou !connecté
	string *logiciels_client; //liste logiciel client
	string *logiciels_server;
	//struct machine *next;	
}machine; 

//definition structure parc
typedef struct parc{
	machine* machine;
	struct parc* next;
}parc;

//fonction verifie nombre est un octet
_Bool isOctet(int x){
	if(x>=0 && x<256)
		return 1;
	else
		return 0;
}

//fonction qui récupere addresse mac
void MacSaisi(char mac[6]){
	int a, b, c, d, e, f, i;
	do{	
		i=scanf("%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f);
		if(i<6){
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
			while(getchar()!='\n');
				continue;
		}
		if(!(isOctet(a) && isOctet(b) && isOctet(c) && isOctet(d) &&isOctet(e) && isOctet(f)))
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
		else
			break;
	}while(1);
	mac[0]=(unsigned char)a;
	mac[1]=(unsigned char)b;
	mac[2]=(unsigned char)c;
	mac[3]=(unsigned char)d;
	mac[4]=(unsigned char)e;
	mac[5]=(unsigned char)f;
}

//fonction qui récupère addresse Ip
void IpSaisi(char ip[4]){
	int a, b, c, d, i;
	do{
		i=scanf("%d.%d.%d.%d",&a,&b,&c,&d); //recupere addresse forma a.b.c.d
		if(i<4){ //verifie addresse complete
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
			while(getchar()!='\n');
				continue;
		}
		if(!(isOctet(a) && isOctet(b) && isOctet(c) && isOctet(d))) //verifie element est un octet
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
		else
			break;
	}while(1);
	ip[0]=(unsigned char)a;
	ip[1]=(unsigned char)b;
	ip[2]=(unsigned char)c;
	ip[3]=(unsigned char)d;
}

//fonction ajouter liste logiciel
void addLogiciel(string* list_logiciel){
	char logiciel[15];
	int i,j=0;
	do{
		printf("Taper 1 pour ajouter un logiciel:\t");
		scanf("%d",&i);
		if(i==1){
			printf("Entrer le logiciel:\t");
			scanf("%s", logiciel);
			if(j!=0)	//j!=0
				list_logiciel=realloc(list_logiciel, (j+1)*sizeof(string)); //réallouer espace memoire
			list_logiciel[j]=(string)malloc(strlen(logiciel)+1);
			if(list_logiciel[j]!=NULL) //allocation réussi
				strcpy(list_logiciel[j], logiciel); //copier logiciel dans le tableau	
		}
		j++;
	}while(i==1);
}

//fonction initialiser machine		
machine *initMachine(){
	machine *m= malloc(sizeof(machine));
	if(m!=NULL){
		//récupère nom machine
		char name[50];
		printf("Entrer le nom de la machine:\t");
		scanf("%s",name);
		strcpy(m->name, name);	//copie nom machine

		//récupère liste logiciel client
		puts("---Ajouter Logiciels Clients---");
		m->logiciels_client=(string*) malloc(sizeof(string));//alloue espace pour tableau de string
		if (m->logiciels_client!=NULL)
			addLogiciel(m->logiciels_client);	//on initialise liste de logiciel client

		//ajouter les logiciel serveur
		puts("---Ajouter Logiciels Serveur---");
		m->logiciels_server=(string*) malloc(sizeof(string));//alloue espace pour tableau de string
		if (m->logiciels_server!=NULL)
			addLogiciel(m->logiciels_server);

		//initialise état à false
		m->state=false;
		
		//initialiser address mac
		printf("Entrer l'addresse Mac(format: x:x:x:x:x:x)\t");
		MacSaisi(m->MacAddr);

		//initialiser address Ip
		printf("Entrer une adresse Ip(format: a.b.c.d)\t");
		IpSaisi(m->IpAddr);

		//initialiser mask
		printf("Entrer le masque de réseau(format: a.b.c.d)\t");
		IpSaisi(m->mask);
	    }
	    return m;

}

//ajouter machine au parc
parc *addMachine(parc *list){
	parc *p=malloc(sizeof(parc));	
	if(p!=NULL){
		p->machine=initMachine();//initialiser machine
		p->next=NULL;
	}
	if(list == NULL)	//verifie si parc contient aucun machine
		list=p;
	else{ //sinon
		parc *tmp=list;	//pointeur sur list machine
		while(tmp->next!=NULL)	//verifie prochain machine existe
			tmp=tmp->next;
		tmp->next=p;
		tmp=tmp->next;
	}
	return list;
}

//fonction qui affiche nom machine
void afficheName(machine *m){
	printf("HostName:\t");
	printf("%s",m->name);
	puts("");
}

//fonction affiche liste logiciel client
void afficheLogicielClient(machine *m){
	string* client=m->logiciels_client;
	printf("Liste Logiciels clients installés sur cette machine: ");
	for(int j=0; client[j]!=NULL; j++){
		printf("%s",client[j]);
		if(client[j+1]!=NULL)
			printf("; ");
	}
	puts("");
}

//fonction affiche liste logiciel serveur
void afficheLogicielServer(machine *m){
	string* serveur=m->logiciels_server;
	printf("Liste Logiciels serveurs installés sur cette machine: ");
	for(int j=0; serveur[j]!=NULL; j++){
		printf("%s",serveur[j]);
		if(serveur[j+1]!=NULL)
			printf("; ");
	}
	puts("");
}

//fonction affiche addresse MAC

void afficheMacAddr(machine *m){
	printf("MAC Address:\t");
	unsigned char *mac=m->MacAddr;
	for(int i=0; i<6; i++){
		printf("%x", mac[i]);
		if(i<5)
			printf(":");
	}
	puts("");
}

//fonction affiche addresse IP
void afficheIpAddr(machine *m){
	printf("IP Address:\t");
	unsigned char *ip= m->IpAddr;
	for(int i=0; i<4; i++){
		printf("%d", ip[i]);
		if(i<3)
			printf(".");
	}
	puts("");
}

//fonction affiche Mask réseau
void afficheMask(machine *m){
	printf("Mask:\t");
	unsigned char *mask= m->mask;
	for(int i=0; i<4; i++){
		printf("%d", mask[i]);
		if(i<3)
			printf(".");
	}
	puts("");
}

//fonction affiche état
void afficheState(machine *m){
	printf("State:\t");
	if(m->state)
		printf("Connecté!");
	else
		printf("Non connecté!");
	puts("");
}

//fonction affiche machine
void afficheMachine(machine *m){
	puts("--------------------------------------------------");
	afficheName(m);
	afficheLogicielClient(m);
	afficheLogicielServer(m);
	afficheMacAddr(m);
	afficheIpAddr(m);
	afficheMask(m);
	afficheState(m);
	puts("--------------------------------------------------");
}

//afficher parc
void afficheParc(parc *p){
	puts("Debut de la liste");
	for(parc *tmp=p; tmp!=NULL; tmp=tmp->next)
		afficheMachine(tmp->machine);
	puts("Fin de la liste");
}
	
//fonction recherche machine
machine *searchMachine(string hostname, parc *p){
	machine *m=NULL;
	for (parc *tmp=p; tmp!=NULL; tmp=tmp->next){
		if((strcmp(tmp->machine->name, hostname))==0){//comparer les noms machines 
			m=tmp->machine;
			break;
		}
	}
	return m;
}

//fonction suprimer machine dans le parc
parc *deleteMachine(string hostname, parc *p){
	machine *m =searchMachine(hostname, p);
	if(m==NULL)
		printf("La machine: %s n\'existe pas dans le parc!\n", hostname);
	else{
		parc *tmp= p;
		while(tmp!=NULL){
			if(tmp->machine->name==m->name){
				for(parc *t=tmp; t!=NULL; t=t->next){
					if(t->next!=NULL)
						t->machine=t->next->machine;
				}
			}
			tmp=tmp->next;
		}
		for(parc *tmp=p; tmp !=NULL; tmp=tmp->next){
			if(tmp->next->next==NULL)
				tmp->next=NULL;
		}
		
		printf("La machine: %s a ete supprimer avec SUCCES!\n", hostname);
	}
	return p;
}
				

int main(void){
	parc *list=NULL; //list des machines
	machine *m=NULL;
    	int option, stop=1;
	char hostname[50];
	do{
		puts("***************************************");
		puts("1: Ajouter une machine au parc");
		puts("2: Afficher le parc");
		puts("3: Rechercher une machine");
		puts("4: Supprimer une machine du parc");
		puts("***************************************");
		printf("Que voulez vous faire\t");
		scanf("%d", &option);
		switch(option){
			case 1:
				list=addMachine(list);
				break;
			case 2:
				afficheParc(list);
				break;
			case 3:
				printf("Entrer le nom de la machine que vous voulez rechercher:\t");
				scanf("%s", hostname);
				m =searchMachine(hostname, list);
				if(m!=NULL)
					afficheMachine(m);
				else
					printf("La machine: %s n\'existe pas dans le parc!\n", hostname);
				break;
			case 4:
				printf("Entrer le nom de la machine que vous voulez supprimer:\t");
				scanf("%s", hostname);
				list=deleteMachine(hostname, list);
				break;
			default:
				puts("Option Non disponible!!! Faites un autre choix!");
				continue;
		}
		printf("Voulez-vous continuer? 1:OUI\t 2:NON\t");
		scanf("%d",&stop);
	}while(stop==1);
    	while (list != NULL){
		parc *tmp = list->next;
		free(list);
		list = tmp;
	}
	//printf("%s\n", list->machine->name);
	
	return 0;
}
