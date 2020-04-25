#include <stdio.h>   
#include <string.h>
#include <fcntl.h>

#define I 4      
#define M 6	
#define PC 100  
#define N 15    
#define E 200	  			    
#define IN 30

#define IP 2   
#define MAC 3
#define MASK 4
#define PASS 5
#define DNS 6
#define NBT 1

int T[IN];

typedef unsigned char AdrMac[M];  
typedef unsigned char AdrIp[I];  
typedef unsigned char Nom[N];    

typedef struct{     
	AdrMac mac; 
	AdrIp ip;   
	AdrIp mask; 
	AdrIp pass; 
	AdrIp dns;  
	Nom nom;    
}Machine;

typedef struct{         
	Machine ca[PC]; 
	int premlib;    
}Parc;

Parc p;  

void Interface(int T[]){
	int i;
	printf("\n");
	for(i=0;i<IN;i++)
		printf("-");
}

int IsOctet(int i){    
	return i>=0 && i<256; 
}

void MacSaisie(AdrMac mac){ 
	int a,b,c,d,e,f,i;                                       
	do{                                                       
		printf("Entrez l'adresse Mac:");                  
		i = scanf("%x-%x-%x-%x-%x-%x",&a,&b,&c,&d,&e,&f);                      
		if (i<6) {                                         
			puts("Adresse non hexadecimal");          
			while (getchar()!='\n');                   
			continue;                                 
		}
		if ((!(IsOctet(a) && IsOctet(b) && IsOctet(c) && IsOctet(d)
		     && IsOctet(e) && IsOctet(f))))
			puts("Adresse Mac Incorrecte");
		else break;
	}while(1);
	mac[0] = (unsigned char)a;
	mac[1] = (unsigned char)b;
	mac[2] = (unsigned char)c;
	mac[3] = (unsigned char)d;
	mac[4] = (unsigned char)e;
	mac[5] = (unsigned char)f;
}

void Saisie(AdrIp ip){
	int a,b,c,d,i;
	do{
		i = scanf("%d.%d.%d.%d",&a,&b,&c,&d); 
		if(i<4){                              
			puts("Adresse incorrecte");
			while (getchar()!='\n');
			continue;
		}
		if ((!(IsOctet(a) && IsOctet(b) && IsOctet(c) && IsOctet(d))))
			puts("Adresse incorrecte");
		else break;
	}while(1);
	ip[0] = (unsigned char)a;
	ip[1] = (unsigned char)b;
	ip[2] = (unsigned char)c;
	ip[3] = (unsigned char)d;
}

// procédure de saisie du nom NetBios

void NomSaisie(Nom nom){
	printf("nom NetBios: ");
	scanf("%s", nom);
}

// procédure qui permet de faire l'inventaire des machines présentes dans le parc

void liste(){                        
    	int i;
    	puts("\n");
    	printf("Il y a %d machine(s) répertorié(es) dans le parc\n", p.premlib);
	printf("--------------------------------\n");
    	for(i=0;i<p.premlib;i++)             
		printf("%s\n", p.ca[i].nom); 
}

// fonction de saisie générale qui retourne une machine m

Machine SaisieMachine(){             
	Machine m;
	NomSaisie(m.nom);
	MacSaisie(m.mac);
	printf("Saisir l'adresse IP:");
	Saisie(m.ip);
	printf("Saisir l'adresse du masque:");
	Saisie(m.mask);
	printf("Saisir l'adresse de la passerelle:");
	Saisie(m.pass);
	printf("Saisir l'adresse DNS:");
	Saisie(m.dns);
	return m;
}

// procédure qui permet de saisir une machine puis de la placer dans le parc.

void Ajout(){
    	p.ca[p.premlib] = SaisieMachine(); 
    	p.premlib++;
}
// procédure qui permet d'effacer une machine 

void MachSupp(){
	int i,j;
	char nom2[20];
	printf("Quelle machine voulez-vous effacer\n");
	printf("nom NetBios:");
	scanf("%s", nom2);
	for(i=0;i<PC;i++)
		if((strcmp(nom2, p.ca[i].nom)) == 0){ 
				for(j=i;j<PC-1;j++)
					p.ca[j]=p.ca[j+1];
				PC-1;
		}
	p.premlib--;
	printf("------------------------------------\n");
	printf("%s a été supprimée avec succès\n", nom2); 
}
												
// procédure d'affichage d'une machine

void MacAff(AdrMac mac){
	int i;
	for(i=0;i<M;i++){
		printf("%x", mac[i]);
		if(i<M-1)
			printf("-");
	}
}

// procédure qui permet d'afficher les éléments du type AdrIp

void AdrAff(AdrIp ip){
	int i;
	for(i=0;i<I;i++){
		printf("%d", ip[i]);
		if(i<I-1)
			printf(".");
	}
}

// procédure permetant d'afficher le nom NetBios

void NomAff(Nom nom){    
	printf("%s", nom);
}

// procédure qui a pour fonction d'afficher ou non une machine

void MachAff(Machine m){
	Interface(T);
	puts("");
	printf("Nom NetBios : "); NomAff(m.nom);
	puts("");
	printf("Adresse Mac : "); MacAff(m.mac);
	puts("");
	printf("Adresse IP : "); AdrAff(m.ip);
	puts("");
	printf("Adresse du masque : "); AdrAff(m.mask);
	puts("");
	printf("Adresse de la passerelle : "); AdrAff(m.pass);
	puts("");
	printf("Adresse DNS : "); AdrAff(m.dns);
}				

// procédure qui permet d'afficher le parc

void ParcAff(){
	int i=0;
	while(i<p.premlib){
		MachAff(p.ca[i]);
		puts("");
		i++;
	}
}

// procédure qui permet d'effectuer un ping

void Ping(AdrIp ip){
	char commande[50];
	sprintf(commande,"ping -c 2 %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	switch(system(commande)){
		case 0 : puts("Machine en ligne");break;
		case 2 : puts("Erreur 1");break;
		case 1 : puts("Machine non reliée");break;
	}
}

int IpClasse(AdrIp ip){
	return (ip[0]<128?1:(ip[0]<192?2:3));
}

int Ip2Res(AdrIp ip, AdrIp res){
	int i=0;
	int c = IpClasse(res);
	while(i<c && ip[i] == res[i]) i++;
	return i = c;
}

void ResRecherche(){
	int i;
	AdrIp res;
	printf("Entrez une adresse réseau:");
	Saisie(res);
	printf("%d.%d.%d.%d", res[0], res[1], res[2], res[3]);
	while(i<p.premlib){
		Ip2Res(p.ca[i].ip, res);
		i++;
	}
}

// procédure qui permet de rechercher une machine dans le parc

void Recherche(){       
	int i=0;
 	char nom2[20];
	printf("Quelle machine voulez-vous afficher\n");
	printf("nom NetBios:");
	scanf("%s", nom2);
	while (i<PC && strcmp(nom2, p.ca[i].nom)) i++;
	if(i == PC) puts("Machine introuvable"); 
	else MachAff(p.ca[i]);
    Ping(p.ca[i].ip);
}

// procédure qui permet de sauvegarder le parc dans un fichier.

void sauvegarde(char nomf[]){  
    	int i, fic;
    	fic = open(nomf, O_CREAT | O_TRUNC | O_RDWR, 777);   
    	write(fic, &p.premlib, sizeof(int));           
	write(fic, p.ca, p.premlib * sizeof(Machine)); 
    	close(fic);                                    
}

// procédure qui permet d'importer(de charger) le parc.

void import(char nomf[]){  
    	int i,fic;
    	fic = open(nomf, O_RDONLY);  
    	read(fic, &p.premlib, sizeof(int));
	read(fic, p.ca, p.premlib * sizeof(Machine));
}

// procédure qui permet de comparer deux adresses(ip, mask, pass ou dns).

int IPcmp(AdrIp ip1, AdrIp ip2){  
	int i;
	for(i=0;i<I;i++){
		if(ip1[i] < ip2[i]) return -1; 
		if(ip1[i] > ip2[i]) return 1;  
	}
	return 0;  
}

// Même principe que pour la fonction IPcmp

int MACcmp(AdrMac mac1, AdrMac mac2){ 
	int i;
	for(i=0;i<M;i++){
		if(mac1[i] < mac2[i]) return -1;
		if(mac1[i] > mac2[i]) return 1;
	}
	return 0;
}

// Fonction de comparaison sur un champ determiné.

int MachineCmp(Machine m1, Machine m2, int champ){  
	switch(champ){                  
		case MAC : return MACcmp(m1.mac, m2.mac);
		case IP  : return IPcmp(m1.ip, m2.ip);
		case PASS : return IPcmp(m1.pass, m2.pass);
		case MASK : return IPcmp(m1.mask, m2.mask);
		case DNS : return IPcmp(m1.dns, m2.dns);
		case NBT : return strcmp(m1.nom, m2.nom);
        	default : fputs("Erreur de champ",stderr); exit(10);   
	}                                                              
}

// procédure de tri

void Tri(int champ){ 
	int i,j;
	Machine tmp;  
	for(i=p.premlib-1;i>0;i--) 
		for(j=0;j<i;j++)
			if(MachineCmp(p.ca[j], p.ca[j+1], champ) > 0){  
				tmp = p.ca[j];                                  
				p.ca[j] = p.ca[j+1];                            
				p.ca[j+1] = tmp;                                 
			}                                                        
} 
									       
void MenuTri(){   
	int champ;
	puts("");
	puts("------------- TRI ------------");
	puts(" 1) Nom NetBios");
	puts(" 2) Adresse Ip");
	puts(" 3) Adresse Mac");
	puts(" 4) Masque de sous reseau");
	puts(" 5) Adresse de la passerelle");
        puts(" 6) Adresse DNS");
        puts(" 7) Quitter");
        puts("------------------------------");
        printf("%s","Sur quel champ voulez vous effectuer le tri ?");
	scanf("%d",&champ);
        switch(champ){
        	case 1:Tri(1);printf("Le parc a été trié sur les noms NetBios");break;
                case 2:Tri(2);printf("Le parc a été trié sur les adresses IP");break;
                case 3:Tri(3);printf("Le parc a été trié sur les adresses Mac");break;
                case 4:Tri(4);printf("Le parc a été trié sur les adresses des Masques");break;
                case 5:Tri(5);printf("Le parc a été trié sur les adresses des passerelles");break;
                case 6:Tri(6);printf("Le parc a été trié sur les adresses DNS");break;
                case 7:puts("\n"); break;
        }
}

// Procédure du menu général

int MenuGeneral(){
	int choix;
	char nomf[E];
	do{
	puts("\n");
	puts("------------------------- MENU -----------------------");
	puts(" 1) Ajout d'une machine dans le parc");
	puts(" 2) Rechercher une machine // effectuer un ping");
	puts(" 3) Supprimer une machine du parc");
	puts(" 4) Liste de toutes les machines du parc");
	puts(" 5) Afficher le parc");
	puts(" 6) Sauvegarder le parc");
	puts(" 7) Charger le parc");
	puts(" 8) Trier le parc sur un champ");
	puts(" 9) Laver l'écran");
	puts("10) Quitter");
	puts("------------------------------------------------------");
	printf("%s","Quel est votre choix: ");
	scanf("%d",&choix);
	switch(choix){
		case 1 : Ajout(); break;
		case 2 : Recherche(); break;
		case 3 : MachSupp(); break;
		case 4 : liste(); break;	 
		case 5 : system("clear");
			 ParcAff(); break;
		case 6 : printf("Nom du fichier d'export : ");
			 scanf("%s",nomf);
			 sauvegarde(nomf);
			 printf("Le parc a été sauvegardé dans le fichier %s",nomf); break;
		case 7 : printf("Nom du fichier d'import : ");
			 scanf("%s",nomf);
			 import(nomf);
			 printf("%s a été chargé"); break;
		case 8 : MenuTri(); break;
		case 9 : system("clear"); break;
		case 10: puts("-----------");
			 printf("Bye Bye\n");
			 puts("-----------");
	}
	}while(choix!=10);    
}

// procédure main
	
int main(){	
	p.premlib=0;    
	MenuGeneral();  
}

	
