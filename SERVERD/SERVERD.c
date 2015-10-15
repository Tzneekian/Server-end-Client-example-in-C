#include "./wrappers.h"

struct client_struct client_database[25];//strutura che conterrà le coppie matricole password dei docenti

void servers(int , struct sockaddr_in );//prototipo della funzione per ricevere appelli modificati dal SERVERS
void docenti(int , struct sockaddr_in );//prototipo della funzione dedicata al CLIENTP(client docenti)
void segreteria_didattica(int , struct sockaddr_in );//prototipo della funzione dedidcata al CLIENTD(client segreteria didattica
void socket_serverd();//prototipo della funzione dedicata alla creazione del socket
int main()//funzione del main
{

socket_serverd();//chiamata della funzione void_socket_serverd()

return 0;
}


void socket_serverd()//funzione dedicata alla creazione del socket
{
int ascolto,conn_fd,invio=0;//qui vengono definiti i descrittori del socket per il serverd
struct sockaddr_in server, client;//qui vengono definite le strutture per assegnare una porta e un ip al socket
int i=0;
socklen_t len;//qui si definisce len come un tipo intero di alemno 32 bit
struct indirrizi{//struttura che conterrà gli indirizzi sia servers sia quelli dei client
struct sockaddr_in INT; 
struct sockaddr_in EST;
};
struct indirrizi rete[10];//qui possiamo definire al max 10 indirizzi di rete
int pool[10];
for (i=0;i<4;i++)
{ pool[i]=0; 
  rete[i].INT.sin_family = AF_INET;
  rete[i].EST.sin_family = AF_INET;
  rete[i].INT.sin_addr.s_addr = inet_addr("0.0.0.0");
}
rete[0].EST.sin_addr.s_addr = inet_addr("127.0.0.2");//indirizzo ip del clientp
rete[1].EST.sin_addr.s_addr = inet_addr("127.0.0.3");//indirizzo ip del clientd



ascolto = Socket(AF_INET, SOCK_STREAM, 0);//qui viene creato il socket, al suo interno troviamo 3 parametri(il dominio,il tipo di socket(nel nostro caso SOCK_STREAM è un canale bidirezionale e sequenziale),infine il protocollo che viene inizializzato a 0) 
server.sin_family= AF_INET;//qui con AF_INET definiamo che tipo di protocollo usare(IPV4)
server.sin_addr.s_addr = htonl(INADDR_ANY);//qui impostiamo INADDR_ANY per accettare comunicazioni da qualsiasi client,convertendo tramite htonl un numero a 32 bit dal formato macchina a quello di rete
server.sin_port= htons(1024);//qui impostiamo la porta per le comunicazioni con questo server,htons converte un numero a 16bit dal formato macchina a quello di rete


Bind(ascolto, (struct sockaddr *) &server, sizeof(server));//qui tramite la Bind assegnamo un indirizzo al socket


Listen(ascolto,10);//qui mettiamo in ascolto il server e definiamo quante connessioni pendenti ci posso essere al massimo

printf("SERVERD AVVIATO...PRONTO PER NUOVE CONNESSIONI\n");
while(1)//Nel momento che entriamo nel while il server si manterrà sempre ina scolto
{
len = sizeof(client);


conn_fd = Accept(ascolto, (struct sockaddr *)&client, &len);//La funzione accept permette al server di accettare connessioni in entrata 

char *ip = inet_ntoa(client.sin_addr);//qui otteniamo l'indirizzo del client che si è connesso

if (Fork() == 0)
{

close(ascolto);//qui si chiude il descrittore della listen 

printf("Ip del del client connesso:%s\n",ip);//qui conosciamo ip del nuovo client connesso al server

FullRead(conn_fd,(char *)&invio, sizeof(invio));

if (client.sin_addr.s_addr==rete[0].EST.sin_addr.s_addr && invio==0){//tramite questo if verifichiamo se il client che si è connesso è il clientp, nel caso si effettua una chiamata alla funzione docenti
		printf("Client professore connesso\n"); 
	docenti(conn_fd, client);
		
 }

else if(client.sin_addr.s_addr==rete[1].EST.sin_addr.s_addr && invio==0){//tramite questo else if verifichiamo se il client che si è connesso è il clientd, nel caso si effettua una chiamata alla funzione segreteria_didattica
printf("Client segreteria didattica\n");
  segreteria_didattica(conn_fd, client);

}

else if(client.sin_addr.s_addr==rete[0].EST.sin_addr.s_addr && invio==1){//tramite questo if verifichiamo se il client che si è connesso è il servers per l'invio di un appello modificato, nel caso si effettua una chiamata alla funzione servers
printf("Servers connesso\n");
servers(conn_fd,client);
}


}

else //qui siamo nel processo padre
close (conn_fd);//qui si chiude la connesione al ritorno del figlio

}

}



void docenti(int conn_fd, struct sockaddr_in client)//funzione deidcata al clientp
{

//qui si definiscono tutte le varibili necessarie per la funzione
FILE *file;
ssize_t nread;
int j,not_valid=1;
int scelta=0,scelta2=1;
char risposta_da_client[2000],appello_da_chiudere[50],matricola[20],password[20],appoggio1[50], appoggio2[50],nomefile[250],mat1[50],mat2[50],buffer2[1024],star[]={"####################################################################\n##########################LISTA ESAMI###############################\n\n\n"},star2[]={"####################################################################\n##########################INFO APPELLO##############################\n"};;
// qui si assegna tramite la struttura un indirizzo ip e un porta per metterci in contatto col servers per inviare un appello per l'archiviazione
struct sockaddr_in servers;
servers.sin_family = AF_INET;
servers.sin_port = htons(8888);
servers.sin_addr.s_addr = inet_addr("127.0.0.5");

//qui vengono definite le matricole e le password per l'autenticazione dei docenti
    strcpy(client_database[0].matricola,"010001");
    strcpy(client_database[0].password,"reti");

    strcpy(client_database[1].matricola,"010002");
    strcpy(client_database[1].password,"reti");

    strcpy(client_database[2].matricola,"010003");
    strcpy(client_database[2].password,"reti");

write(conn_fd,"\nSalve..Benvenuto Professore\n ",strlen("Salve..Benvenuto Professore\n"));

 write(conn_fd,"\nPrego inserire Matricola:",strlen("Prego inserire Matricola:\n"));
            recv(conn_fd,matricola,20,0);

write(conn_fd,"Prego inserire Password:",strlen("Prego inserire Password:\n"));
            recv(conn_fd,password,20,0);
for(j=0;j<5;j++)
            {
//qui si verifica se la coppia matricola/password combacia con quella registrata in una struttura
if(!(strcmp(client_database[j].matricola,matricola)) &&
                    !(strcmp(client_database[j].password,password)))
                {
     //nel caso la coppia sia valida si invia un messaggio di autenticazione al client e si procede               
                    printf("Client professore autenticazione: riuscita!\n");
                    write(conn_fd,"Autenticato!\n",strlen("Autenticato!\n"));
                    
                    not_valid=0;
                    break;

                }

             }

            if(not_valid==1)
            {
//nel caso la coppia non sia valida il server termina la comunicazione col clientp
                printf("Auntenticazione fallita....docente %s disconnesso dal server...\n",matricola);
                write(conn_fd,"Errore:Coppia non valida!\n",strlen("Errore:Coppia non valida!\n"));
               
                
exit(0);}



 
 
while (scelta !=5)//qui si entra nel while per scegliere cosa fare
{



printf ("IN ATTESA DEL CLIENTP\n");


FullRead(conn_fd,(char *)&scelta, sizeof(scelta));

printf("IL CLIENTP HA INVIATO: %d \n", scelta);



switch (scelta)
{
case 1:
{
//in questo case permetteremo al clientp di creare un nuovo appello tramite la creazione di un nuovo file momentaneo che chiameremo "temp", successiavamente inseriremo tramite delle fwrite le stringe ricevute dal clietp attraverso le recv(),una volta terminato inserimento delle informazioni nel file questo viene rinominato con un nome scelto dal docente nel clientp
printf("IL CLIENT DOCENTI VUOLE INSERIRE UN NUOVO APPELLO..\n");



          file=fopen("temp","w");	
					
          write(conn_fd,"\nNuovo file creato!",strlen("Nuovo file creato!\n"));
           recv(conn_fd,appoggio1,50,0);
 memset( appoggio1, '\0', sizeof( appoggio1 ) );
fwrite(star2, strlen(star2),sizeof(char),file);
           write(conn_fd,"\nDipartimento:",strlen("Dipartimento \n"));				
          recv(conn_fd,appoggio1,50,0);								
			
	  strcpy(appoggio2,"Dipartimento:");							
	  strcat(appoggio2,appoggio1);								

       
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),file);					
   memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );
                   
          write(conn_fd,"\nCorso di laurea:", strlen("Corso di laurea\n"));
          recv(conn_fd,appoggio1,50,0);
       
          strcpy(appoggio2,"Corso di laurea:");
          strcat(appoggio2,appoggio1);

          
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),file);
          memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );
          
          write(conn_fd,"\nNome esame:", strlen("Nome esame\n"));
          recv(conn_fd,appoggio1,50,0);
         
          strcpy(appoggio2,"Nome esame:");
          strcat(appoggio2,appoggio1);
        
      
	  fwrite(appoggio2, strlen(appoggio2),sizeof(char),file);
          memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );
          
          write(conn_fd,"\nData:", strlen("Data\n"));
          recv(conn_fd,appoggio1,50,0);
       
          strcpy(appoggio2,"Data:");
          strcat(appoggio2,appoggio1);
         
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),file);
          memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );

             fwrite(star, strlen(star),sizeof(char),file);
   

 write(conn_fd,"Inserire nome appello per terminare:\n", strlen("Inserire nome appello per terminare:\n"));
          recv(conn_fd,appoggio1,50,0);

rimuovi(appoggio1);
terminatore(appoggio1);
strcat(appoggio1,".txt");

    if((fopen(appoggio1,"r"))!=NULL)
        {
            printf("L'APPELLO CON QUESTO NOME ESISTE GIÀ\n");
         write(conn_fd,"\nappello gia esistente riprovare", strlen("appello gia esistente riprovare\n"));
        }
    else
        {
 write(conn_fd,"\ncreazione appello riuscita", strlen("creazione appello riuscita\n"));
           rename("temp",appoggio1);
        }



fclose(file);

//da questo punto fino alla fine del case 1 effetturemo una connect al servers per l'archiviazione dell'appello, se il valore di ritorno è -1 allora il servers è offline e il case 1 si chiude, altrimenti l'invio del file viene effettuato  
int servers_fd;
servers_fd = Socket(AF_INET,SOCK_STREAM,0);
if((connect(servers_fd,(struct sockaddr *) &servers,sizeof(struct sockaddr)) == -1))
	{
	printf("Il SERVERS é offline..impossibile inviare appello per l'archiviazione\n");
	}
	else {

	FullWrite(servers_fd, &scelta2, sizeof(int));//qui si invia al client un intero(nel nostro caso scelta2 è impostato a 1)in modo tale che il servers sa cosa deve fare
//qui inviamo al servers la lunghezza del nome del file convertendo i numeri tramite htonl in network byte order, si invia prima la lunghezza del nome e poi il nome vero è proprio
uint32_t nameLength = htonl(strlen(appoggio1));
write(servers_fd, &nameLength, sizeof nameLength); 
write(servers_fd, appoggio1, strlen(appoggio1)); 

//qui apriamo il l'appello appena creato in sola lettura
FILE *fd=fopen(appoggio1,"r");
    if (fd < 0) {
      fprintf(stderr, "impossibile aprire il file '%s': %s\n", appoggio1, strerror(errno));
      exit(1);
    }
ssize_t bytes;
//qui si carica il file in un buffer che poi verrà inviato tramite una send al servers
while ((bytes = fread(buffer2,sizeof(char), sizeof(buffer2),fd)) > 0)
{
   send(servers_fd, buffer2, bytes, 0);
}
      fclose(fd);           //chiuduamo il file

close(servers_fd);//chiudiamo la connessione col servers

}

memset( appoggio1, '\0', sizeof( appoggio1 ) );
}break;

case 2:
{
//nel case 2 chiediamo al clientp che appello aprire per registrare un esame
struct flock lock;//definiamo un tipo di truttura flock per poi determinare il tipo di lock di cui abbiamo bisogno
write(conn_fd,"INSERIRE NOME APPELLO:.....\n", strlen("INSERIRE NOME APPELLO:.....\n"));
recv(conn_fd,risposta_da_client,2000,0); // data
        puts(risposta_da_client);
write(conn_fd,"verifica disponibilità...prego rimanere in attesa", strlen("verifica disponibilità...prego rimanere in attesa"));       


FILE *f = fopen(risposta_da_client, "a");
memset(&lock,0,sizeof(lock));
lock.l_type=F_WRLCK;//settiamo la struttura 

   fcntl(fileno(f),F_SETLKW,&lock);//settiamo il lock sul file appena aperto
printf("File ok!!!\n");
write(conn_fd,"Appello disponibile\n",strlen("Appello disponibile\n"));				
          recv(conn_fd,appoggio1,50,0);								
			
	  strcpy(appoggio2,"#######");							
	  strcat(appoggio2,appoggio1);								

          fwrite(appoggio2, strlen(appoggio2),sizeof(char),f);					
memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );

write(conn_fd,"\nMatricola:", strlen("Matricola:\n"));
          recv(conn_fd,mat1,50,0);
       
          strcpy(mat2,"Matricola:");
          strcat(mat2,mat1);


          
          fwrite(mat2, strlen(mat2),sizeof(char),f);


write(conn_fd,"\nNome:", strlen("Nome:\n"));
          recv(conn_fd,appoggio1,50,0);
       
          strcpy(appoggio2,"Nome:");
strtok(appoggio1, "\n");
 matricole(appoggio1,mat1);
 
   strcat(appoggio2,appoggio1);

          
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),f);
fwrite("\n",1,sizeof(char),f);
memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );

          
          write(conn_fd,"\nCognome:", strlen("Cognome\n"));
          recv(conn_fd,appoggio1,50,0);
       
          strcpy(appoggio2,"Cognome:");
strtok(appoggio1, "\n");
 matricole(appoggio1,mat1);
          strcat(appoggio2,appoggio1);

          
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),f);
fwrite("\n",1,sizeof(char),f);
memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );




write(conn_fd,"\nVoto:", strlen("Voto:\n"));
          recv(conn_fd,appoggio1,50,0);
       
          strcpy(appoggio2,"Voto:");
strtok(appoggio1, "\n");
 matricole(appoggio1,mat1);
          strcat(appoggio2,appoggio1);

          
          fwrite(appoggio2, strlen(appoggio2),sizeof(char),f);
fwrite("\n",1,sizeof(char),f);
memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );

 strcpy(appoggio2,"#######");							
	  strcat(appoggio2,appoggio1);								

          fwrite(appoggio2, strlen(appoggio2),sizeof(char),f);
fwrite("\n",1,sizeof(char),f);					
memset( appoggio1, '\0', sizeof( appoggio1 ) );
                memset( appoggio2, '\0', sizeof( appoggio2 ) );



lock.l_type=F_UNLCK;//settiamo la struttura
fcntl(fileno(f),F_SETLKW,&lock);//eliminiamo il lock sul file aperto

fclose(f);//il file viene chiuso
//da questo punto fino alla fine del case 2 effetturemo una connect al servers per l'archiviazione dell'appello, se il valore di ritorno è -1 allora il servers è offline e il case 1 si chiude, altrimenti l'invio del file viene effettuato  






int servers_fd;
int fd;
servers_fd = Socket(AF_INET,SOCK_STREAM,0);
if((connect(servers_fd,(struct sockaddr *) &servers,sizeof(struct sockaddr)) == -1))
	{
	printf("Il SERVERS é offline..impossibile inviare appello per l'archiviazione\n");
	}
	else {//il servers è online e ha risposto
//qui si invia al client un intero(nel nostro caso scelta2 è impostato a 1)in modo tale che il servers sa cosa deve fare

	FullWrite(servers_fd, &scelta2, sizeof(int));
//qui inviamo al servers la lunghezza del nome del file convertendo i numeri tramite htonl in network byte order, si invia prima la lunghezza del nome e poi il nome vero è proprio
uint32_t nameLength = htonl(strlen(risposta_da_client));
write(servers_fd, &nameLength, sizeof nameLength); 
write(servers_fd, risposta_da_client, strlen(risposta_da_client)); 

//qui apriamo il l'appello appena creato in sola lettura
FILE *fd = fopen(risposta_da_client, "r");
    if (fd < 0) {
      fprintf(stderr, "impossibile aprire '%s': %s\n", risposta_da_client, strerror(errno));
      exit(1);
    }

ssize_t bytes;
//qui si carica il file in un buffer che poi verrà inviato tramite una send al servers
while ((bytes = fread(buffer2,sizeof(char), sizeof(buffer2),fd)) > 0)
{
   send(servers_fd, buffer2, bytes, 0);
}
	
		printf("Trasmissione completata con successo\n\n");

	fclose(fd); 
close(servers_fd);
	}

memset( risposta_da_client, '\0', sizeof( risposta_da_client) );


}break;

case 3:
{

//case 3 permettiamo al client di visualizzare tutti gli appelli contenuti nella cartella del serverd,il serverd mostra soltanto i file con estensione .txt(ovvero gli appelli)
printf("LISTA DEGLI APPELLI:...\n");
DIR *dp;           
  int fscelta,nread3;       
   struct dirent *ep;   
 char buffer3[300],buffert[1024]; 
FILE *l=fopen("temp2","w"); //temp2 è il file che conterra la lista degli appelli  
            dp = opendir ("./");//la opendir punta alla cartella che contiene il file .c del serverd
            if (dp != NULL){
                while (ep = readdir(dp)){
const size_t len = strlen(ep->d_name);
             if (strcmp(ep->d_name, ".") == 0
               || strcmp(ep->d_name, "..")==0 || strcmp(ep->d_name+len - 4, ".txt") != 0 )
               continue;
//carichiamo tutti i nomi in un buffer
               strcpy(buffer3,ep->d_name);
strcat(buffer3,"\n");
//qui con fwrite scriviamo sul file i nomi degli appelli presenti nella cartella del serverd
fwrite(buffer3, strlen(buffer3),sizeof(char),l);
	memset( buffer3, 0, sizeof( buffer3 ) );
            
                }

                (void) closedir(dp);fclose(l);
            }else
                perror ("Impossibile aprire la directory");
//ora con un buffer inviamo il contenuto del file temp2 al clientp che visualizza i nomi degli appelli
fscelta=open("temp2",O_RDONLY);
                if(fscelta<0)
         {
               fprintf(stderr,"Errore nella open(): %s\\ \n",strerror(errno));
               break;}   

while ((nread3=read(fscelta, buffert,sizeof(buffert))) !=0)
{
        write(conn_fd, buffert, nread3);
        
}
close(fscelta);
memset( buffert, 0, sizeof( buffert ) );

}break;

case 4:
{
//nel case 4 permettiamo al clientp ovvero al docente di chiudere un appello 
printf("IL CLIENT DOCENTI VUOLE CHIUDERE UN APPELLO..\n");
//apriamo la directory che contiene gli appelli 
printf("LISTA DEGLI APPELLI:...\n");
DIR *dp;                 
   struct dirent *ep;   
 char buffer[300];    
            dp = opendir ("./");
            if (dp != NULL){
                while (ep = readdir(dp)){
                    const size_t len = strlen(ep->d_name);
if (strcmp(ep->d_name, ".") == 0
               || strcmp(ep->d_name, "..")==0 || strcmp(ep->d_name+len - 4, ".txt") != 0 )
               continue;
              
               strcpy(buffer,ep->d_name);
               strcat(buffer,"\n");
               send(conn_fd, buffer,strlen(buffer), 0);   
            
                }

                (void) closedir(dp);
            }else
                perror ("Impossibile aprire la directory");

  //chiediamo che appello chiudere       

write(conn_fd,"\nINSERIRE NOME APPELLO:.....\n", strlen("INSERIRE NOME APPELLO:.....\n"));
recv(conn_fd,appello_da_chiudere,50,0); // data
        puts(appello_da_chiudere);
char cat[50];
strcpy(cat,appello_da_chiudere);
strcat(cat,"(Chiuso)");
rename(appello_da_chiudere,cat);
RDO(cat);//la chiamata alla funzione RDO permette al clientp ovvero mil docente di chiudere un appello impostando il file .txt in modalità readonly(verificabile tramite il case 2)
//da questo momento in poi il clientp non potrà piu vedere ne vedere ne registrare esami nell'appello

}break;




case 0:
{
//nel case 0 il clientp decide di terminare la connesione col serverd 
printf("IL CLIENT DOCENTI HA CHIUSO LA CONNESSIONE..\n");
close(conn_fd);//Chiudo la connessione
exit(0);

}break;


}
}

}


void segreteria_didattica(int conn_fd, struct sockaddr_in client)
{


int scelta=0;

while (scelta !=5)//qui si entra nel file per decidre che fare
{

printf ("IN ATTESA DEL CLIENTD\n");


FullRead(conn_fd,(char *)&scelta, sizeof(scelta));

printf("IL CLIENTD HA INVIATO: %d \n", scelta);

switch(scelta)
{
case 1:
{
//nel case 1 permettiamo al clientd ovvero la segreteria didattica di visualizzare tutti gli appelli aperti
printf("LISTA DEGLI APPELLI:...\n");
DIR *dp;           
  int fscelta,nread3;       
   struct dirent *ep;   
 char buffer3[300],buffert[1024]; 
FILE *l=fopen("temp2","w");   
            dp = opendir ("./");
            if (dp != NULL){
                while (ep = readdir(dp)){
const size_t len = strlen(ep->d_name);
const size_t len2 = strlen(ep->d_name);
             if (strcmp(ep->d_name, ".") == 0
               || strcmp(ep->d_name, "..")==0  || strcmp(ep->d_name+len - 4, ".txt") != 0 )
               continue;
//carichiamo tutti i nomi in un buffer
               strcpy(buffer3,ep->d_name);
strcat(buffer3,"\n");
//tramite la fwtrite scriviamo i nomi sul file temp2
fwrite(buffer3, strlen(buffer3),sizeof(char),l);
	memset( buffer3, 0, sizeof( buffer3 ) );
            
                }

                (void) closedir(dp);fclose(l);
            }else
                perror ("Impossibile aprire la directory");

fscelta=open("temp2",O_RDONLY);
                if(fscelta<0)
         {
               fprintf(stderr,"Errore nella open(): %s\\ \n",strerror(errno));
               break;}   
//ora con un buffer inviamo il contenuto del file temp2 al clientp che visualizza i nomi degli appelli
while ((nread3=read(fscelta, buffert,sizeof(buffert))) !=0)
{
        write(conn_fd, buffert, nread3);
        
}
close(fscelta);
memset( buffert, 0, sizeof( buffert ) );


}break;


case 2:
{
//nel case 2 permettiamo al clientd ovvero la segreteria didattica di visualizzare tutti gli appelli chiusi
printf("LISTA DEGLI APPELLI:...\n");
DIR *dp;           
  int fscelta,nread3;       
   struct dirent *ep;   
 char buffer3[300],buffert[1024]; 
FILE *l=fopen("temp2","w");   
            dp = opendir ("./");
            if (dp != NULL){
                while (ep = readdir(dp)){
const size_t len = strlen(ep->d_name);
             if (strcmp(ep->d_name, ".") == 0
               || strcmp(ep->d_name, "..")==0 || strcmp(ep->d_name+len - 4, "uso)") != 0 )
               continue;
//carichiamo tutti i nomi in un buffer
               strcpy(buffer3,ep->d_name);
strcat(buffer3,"\n");
//tramite la fwtrite scriviamo i nomi sul file temp2
fwrite(buffer3, strlen(buffer3),sizeof(char),l);
	memset( buffer3, 0, sizeof( buffer3 ) );
            
                }

                (void) closedir(dp);fclose(l);
            }else
                perror ("Impossibile aprire la directory");

fscelta=open("temp2",O_RDONLY);
                if(fscelta<0)
         {
               fprintf(stderr,"Errore nella open(): %s\\ \n",strerror(errno));
               break;}   
//ora con un buffer inviamo il contenuto del file temp2 al clientp che visualizza i nomi degli appelli
while ((nread3=read(fscelta, buffert,sizeof(buffert))) !=0)
{
        write(conn_fd, buffert, nread3);
        
}
close(fscelta);
memset( buffert, 0, sizeof( buffert ) );


}break;




case 3:
{
// nel case 2 permettiamo al clientd di visualizzare l'appello e gli esami contenuti in esso
char nomefile[50];
write(conn_fd,"\nPREGO INSERIRE IL NOME:....",strlen("PREGO INSERIRE IL NOME:....\n"));				
          recv(conn_fd,nomefile,50,0);			

puts(nomefile);


int fsceltafile,nread2;
char buffer2[1024];
//qui apriamo il file .txt dell'appello selezionato
fsceltafile=open(nomefile,O_RDONLY);
                if(fsceltafile<0)
         {
               fprintf(stderr,"Errore nella open(): %s\\ \n",strerror(errno));
               break;}   
//ora con un buffer inviamo il contenuto del file dell'appello al clientd che visualizza le informazioni dell'appello e gli esami registrati
while ((nread2=read(fsceltafile, buffer2,sizeof(buffer2))) !=0)
{
        write(conn_fd, buffer2, nread2);
}
close(fsceltafile);
memset(buffer2, 0, sizeof(buffer2));
memset(nomefile, 0, sizeof(nomefile));


}break;


case 0:
{
//nel case 0 il clientd decide di terminare la connessione col serverd
printf("IL CLIENT SEGRETERIA DIDATTICA HA CHIUSO LA CONNESSIONE..\n");
close(conn_fd);//Chiudo la connessione
exit(0);


}break;



}


}
}

//la seguente funzione permette al serverd di ricevere un appello modificato dal clients nel servers, nel caso ciò avvenga il file inviato dal servers sostitutisce quello con lo stesso nome presente sul 
void servers(int conn_fd, struct sockaddr_in client)
{



	int nread;	
	
				 
	char buffer[1024],nomefile[50];
printf("IL SERVERS VUOLE INVIARE UN APPELLO...RICEZIONE IN CORSO..\n");
//qui riceviamo dal servers la lugnhezza del nome del file e poi la stringa, infine aggiungiamo il carattere terminatore \0 per segnalare la fine del nome

uint32_t nameLength;
read(conn_fd, &nameLength, sizeof nameLength);
nameLength = ntohl(nameLength);
read(conn_fd, nomefile, nameLength);
nomefile[nameLength]='\0';
//qui apriamo l'appello in sola lettura
FILE *fd = fopen(nomefile, "w");
		if (fd < 0) 
		{
			fprintf(stderr, "errore open(): %s\n", strerror(errno));
			exit(errno);
		}
ssize_t bytes = 0;
//qui riceviamo il contenuto del file dal servers e lo carichiamo in un buffer e importiamo tutto in un nuovo file che sostituirà il vecchio file cpon lo stesso nome
while ((bytes = recv(conn_fd, buffer, sizeof(buffer), 0)) > 0)
{
   fwrite(buffer, sizeof(char), bytes, fd);
}

fclose(fd);


}



