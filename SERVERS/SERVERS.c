#include "./wrappers.h"


void socket_servers();//prototipo della funzione dedicata alla creazione del socket
void segreteria_studenti(int , struct sockaddr_in );//prototipo della funzione al clients
void docenti(int , struct sockaddr_in );//prototipo della funzione dedicata alla ricezione di appelli
int main()//funzione del main
{
 socket_servers();//chiamata della funzione void_socket_servers()
return 0;
}


void  socket_servers()
{
int ascolto, conn_fd; //qui vengono definiti i descrittori del socket per il serverd
struct sockaddr_in server, serverd;//qui vengono definite le strutture per assegnare una porta e un ip al socket
int i=0;
socklen_t len;//qui si definisce len come un tipo intero di alemno 32 bit
pid_t pid;//qui viene definito il descrittore del processo figlio
int pool[10];
int scelta=0;


ascolto = Socket(AF_INET, SOCK_STREAM, 0);//qui viene creato il socket, al suo interno troviamo 3 parametri(il dominio,il tipo di socket(nel nostro caso SOCK_STREAM è un canale bidirezionale e sequenziale),infine il protocollo che viene inizializzato a 0) 
server.sin_family= AF_INET;//qui con AF_INET definiamo che tipo di protocollo usare(IPV4)
server.sin_addr.s_addr = inet_addr("127.0.0.5");//qui impostiamo l'indirizzo del servers,convertendo tramite htonl un numero a 32 bit dal formato macchina a quello di rete
server.sin_port= htons(8888);//qui impostiamo la porta per le comunicazioni con questo server,htons converte un numero a 16bit dal formato macchina a quello di rete


Bind(ascolto, (struct sockaddr *) &server, sizeof(server));//qui tramite la Bind assegnamo un indirizzo al socket

Listen(ascolto, 10);//qui mettiamo in ascolto il server e definiamo quante connessioni pendenti ci posso essere al massimo
printf("IL SERVERS AVVIATO..\n");


while(1)//Nel momento che entriamo nel while il server si manterrà sempre in ascolto
{
len = sizeof(serverd);
conn_fd = Accept(ascolto, (struct sockaddr *)&serverd, &len);//La funzione accept permette al server di accettare connessioni in entrata 
char *ip = inet_ntoa(serverd.sin_addr);//qui otteniamo l'indirizzo del client che si è connesso
if (Fork() == 0)
{



close(ascolto);//qui si chiude il descrittore della listen 
printf("Ip del del client connesso:%s\n",ip);//qui conosciamo ip del nuovo client connesso al server



FullRead(conn_fd,(char *)&scelta, sizeof(scelta));


if(scelta==1){//tramite questo if verifichiamo  chi si è connesso al servers, nel caso si effettua una chiamata alla funzione docenti
docenti(conn_fd, serverd);
}


else if(scelta==2){//tramite questo if verifichiamo se il client che si è connesso è il clients, nel caso si effettua una chiamata alla funzione segreteria_studenti
segreteria_studenti(conn_fd, serverd);
}




}

else //qui siamo nel processo padre

close (conn_fd);//qui si chiude la connesione al ritorno del figlio

}

}

//la seguente funzione permette al servers di ricevere un appello modificato dal serverd, nel caso ciò avvenga il file inviato dal serverd sostitutisce quello con lo stesso nome presente sul servers
void docenti(int conn_fd, struct sockaddr_in serverd) 
{	
	
	int nread;	
	
				 
	char buffer[1024],nomefile[50];

int fd;
int rval;
printf("IL SERVERD VUOLE INVIARE UN APPELLO...RICEZIONE IN CORSO..\n");
//qui riceviamo dal servers la lunghezza del nome del file e poi la stringa, infine aggiungiamo il carattere terminatore \0 per segnalare la fine del nome

uint32_t nameLength;
read(conn_fd, &nameLength, sizeof nameLength);
nameLength = ntohl(nameLength);
read(conn_fd, nomefile, nameLength);
nomefile[nameLength]='\0';


//qui apriamo l'appello in sola lettura
FILE *fr = fopen(nomefile, "w");
		if(fr == NULL)
			printf("file %s impossibile da aprire.\n", nomefile);
		else
		{

ssize_t bytes = 0;
//qui riceviamo il contenuto del file dal servers e lo carichiamo in un buffer e importiamo tutto in un nuovo file che sostituirà il vecchio file cpon lo stesso nome
while ((bytes = recv(conn_fd, buffer, sizeof(buffer), 0)) > 0)
{
   fwrite(buffer, sizeof(char), bytes, fr);
}
}	
fclose(fr);
 
}

//funzione dedicata ai servizi della segreteria studenti
void segreteria_studenti(int conn_fd, struct sockaddr_in client)
{

int sockfd,proffd,j,not_valid=1,fmenu,scelta=0,mscelta=0;
char buffer[1024],file[1024],data[50], buffer2[1024];
ssize_t nread,nread2,nread3;
char Buffer[4095];//buffer per caricare un appelo di dimensioni x
    char *filetemp = "temp.txt";//file che andrà a sostituire il file dell'appello con le modifiche apportate
    char trova[30];//buffer dedicato alla ricerca della stringa da modificare
    char sostituisci[30];//buffer dedicato alla sostituzione della stringa
    int line = 0,line2=0,invio=1;

// qui si assegna tramite la struttura un indirizzo ip e un porta per metterci in contatto col serverd per inviare un appello modificato
struct sockaddr_in serverd;
serverd.sin_family = AF_INET;
serverd.sin_port = htons(1024);
serverd.sin_addr.s_addr = inet_addr("127.0.0.1");



while (scelta !=5)//qui si entra nel while per scegliere cosa fare
{



printf ("Aspetto il codice del menu principale.\n");


FullRead(conn_fd,(char *)&scelta, sizeof(scelta));

printf("Messaggio Arrivato: %d \n", scelta);

switch(scelta)
{
case 1:
{
//case 1 permettiamo al client di visualizzare tutti gli appelli contenuti nella cartella del servers,il servers mostra soltanto i file con estensione .txt(ovvero gli appelli)
printf("LISTA DEGLI APPELLI:...\n");
DIR *dp;           
  int rv,rv1,stop_received,fscelta;       
   struct dirent *ep;   
 char buffer3[300],buffert[1024]; 
FILE *l=fopen("temp2","w");   //temp2 è il file che conterra la lista degli appelli 
            dp = opendir ("./");//la opendir punta alla cartella che contiene il file .c del servers
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
                perror ("impossibile aprire la directory\n");
//ora con un buffer inviamo il contenuto del file temp2 al clients che visualizza i nomi degli appelli
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


case 2:
{
//nel cse due permettiamo al clients di visualizzare il contenuto di un appello trmite un buffer
char nomefile[50];
write(conn_fd,"\nPREGO INSERIRE IL NOME DELL'APPELLO:",strlen("PREGO INSERIRE IL NOME DELL'APPELLO:\n"));			
          recv(conn_fd,nomefile,50,0);			

puts(nomefile);

int fsceltafile,nread2;
char buffer2[1024];

fsceltafile=open(nomefile,O_RDONLY);
                if(fsceltafile<0)
         {
               fprintf(stderr,"Errore nella open(): %s\\ \n",strerror(errno));
               break;}   

while ((nread2=read(fsceltafile, buffer2,sizeof(buffer2))) !=0)
{
        write(conn_fd, buffer2, nread2);
}
close(fsceltafile);
memset(buffer2, 0, sizeof(buffer2));
memset(nomefile, 0, sizeof(nomefile));
}break;



case 3:
{
//nel case 3 il clients puo modificare con semplicità nome/cognome/voto di un esame ricondando nel momento di sotituire una stringa di immettere al finaco del nome/cognome/voto anche le ultime tre cifre della matricola esempio stringa da sostituire=mario.320, nuova stringa carlo
char buffer2[1024];
char nomefile2[50];
//varibili dedicate a caricare le infomazioni sull'ultima modifica del file
char data[80];
time_t rawtime;
struct tm *timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);

char *ip = inet_ntoa(client.sin_addr);//qui conosciamo l'ip del cliuent che sta effettuando la modifica
write(conn_fd,"\nPREGO INSERIRE IL NOME DELL'APPELLO:",strlen("PREGO INSERIRE IL NOME DELL'APPELLO:\n"));				
          recv(conn_fd,nomefile2,50,0);			
puts(nomefile2);
write(conn_fd,"\nPREGO INSERIRE LA PAROLA DA SOSTITUIRE:",strlen("PREGO INSERIRE LA PAROLA DA SOSTITUIRE:\n"));
   recv(conn_fd,trova,30,0);	
write(conn_fd,"\nPREGO INSERIRE LA NUOVA PAROLA:",strlen("PREGO INSERIRE LA NUOVA PAROLA:\n"));
   recv(conn_fd,sostituisci,30,0);	
//per sostituire un nome/cognome/voto useremo duel file Input=l'appello da mkodifcare,output=l'appello modificato
    FILE *Input,*Output;

    Input = fopen(nomefile2, "r");
    Output = fopen(filetemp, "w");
 //il seguente codice effettua la modifica cercando l'occorrenza all'interno del file, per evitare che la sostituzione avvenga su tutte le stringe dello stesso tipo ES.(matricola:0124000322nome:mario,matricola:0123000323nome:mario), il serverd precedentemente attribuisce a ogni nome/cognome/voto le ultime tre cifre della matricola dello stundente in modod tale da creare una chiave univoca per identificare la stringa da trovare nel file

    if(NULL == Input)
    {
        printf("\nimpossibile aprire il file");
       break;
    }
 
    // per tutte le linee del file
    while(NULL != fgets(Buffer, 4095, Input))
    {
        char *Stop = NULL;
        char *Start = Buffer;
      //nel seguente while viene effettuata la ricerca e la sotituzione nel nuovo file terminando la nuova stringa con le ultime 3 cifre della matricola dello studente
        while(1)
        {
          
            
            Stop = strstr(Start, trova);
            if(NULL == Stop)
            {
                fwrite(Start, 1, strlen(Start), Output);
                break;
            }
        
            

            fwrite(Start, 1, Stop - Start, Output);
                       char cifre[4]; 
cifre[3]=trova[strlen(trova)-1]; 
cifre[2]=trova[strlen(trova)-2];
cifre[1]=trova[strlen(trova)-3]; 
cifre[0] = '.';
strcat(sostituisci,cifre);
  memset(cifre, 0, sizeof(cifre)); 
            fwrite(sostituisci, 1, strlen(sostituisci), Output);
            Start = Stop + strlen(trova);
    
     }
line++;



}
//nel codice seguente viene impresso sul file alla prima riga l'ultima modifica effettuata all'appello con data ora e ip del client che l'ha modificato
strftime(data,80,"ultima modifica in data %d-%m-%Y ora %H:%M da ",timeinfo);
strcat(data,ip);
puts(data);
rewind(Output);
fwrite(data, strlen(data),sizeof(char),Output);
memset(sostituisci, 0, sizeof(sostituisci)); 
memset(trova, 0, sizeof(trova));    
memset(data, 0, sizeof(data));
    fclose(Input);
    fclose(Output);
    remove(nomefile2);
    rename(filetemp, nomefile2);

//nel seguente codice viene effettuata la connect al serverd e l'invio dell'appello modificato

int serverd_fd;
serverd_fd = Socket(AF_INET,SOCK_STREAM,0);
if((connect(serverd_fd,(struct sockaddr *) &serverd,sizeof(struct sockaddr)) == -1))
	{
	printf("Il SERVERD é offline..impossibile inviare appello per l'archiviazione\n");
	}
	else {//il servers è online e ha risposto
	FullWrite(serverd_fd, &invio, sizeof(int));
//inviamo al serverd la lunghezza del nome del file e la stringa de inviare
uint32_t nomel = htonl(strlen(nomefile2));
write(serverd_fd, &nomel, sizeof nomel); 
write(serverd_fd, nomefile2, strlen(nomefile2)); 

//si apre l'appello modificato in lettura
FILE *fd = fopen(nomefile2,"r");
    if (fd < 0) {
      fprintf(stderr, "impossibile aprire '%s': %s\n", nomefile2, strerror(errno));
      exit(1);
    }

ssize_t bytes;
//si invia l'appello modficato al serverd
while ((bytes = fread(buffer2,sizeof(char), sizeof(buffer2),fd)) > 0)
{
   send(serverd_fd, buffer2, bytes, 0);
}
	
		printf("Trasmissione completata con successo\n\n");

	fclose(fd); 
close(serverd_fd);
	}

memset( nomefile2, '\0', sizeof( nomefile2 ) );





}break;

case 4:
{
//nel case 4 il clients puo modificare qualunque stringa del file 
char nomefile2[50];
char buffer2[1024];
char data[80];
//varibili dedicate a caricare le infomazioni sull'ultima modifica del file
time_t rawtime;
struct tm *timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
char *ip = inet_ntoa(client.sin_addr);//ip del client che effettua la modifica

write(conn_fd,"\nPREGO INSERIRE IL NOME DELL'APPELLO:",strlen("PREGO INSERIRE IL NOME DELL'APPELLO:\n"));				
          recv(conn_fd,nomefile2,50,0);			
puts(nomefile2);
write(conn_fd,"\nPREGO INSERIRE RECORD DA MODIFICARE:",strlen("PREGO INSERIRE RECORD DA MODIFICARE::\n"));
   recv(conn_fd,trova,30,0);	
write(conn_fd,"\nPREGO INSERIRE IL NUOVO RECORD:",strlen("PREGO INSERIRE IL NUOVO RECORD:\n"));
   recv(conn_fd,sostituisci,30,0);	
//per sostituire un nome/cognome/voto useremo duel file Input=l'appello da mkodifcare,output=l'appello modificato
    FILE *Input,*Output;

    Input = fopen(nomefile2, "r");
    Output = fopen(filetemp, "w");
  //il seguente codice effettua la modifica cercando l'occorrenza all'interno del file

    if(NULL == Input)
    {
        printf("\nimpossibile aprire il file");
       break;
    }
 
 
    // per ogni linea
    while(NULL != fgets(Buffer, 4095, Input))
    {
        char *Stop = NULL;
        char *Start = Buffer;
      //nel seguente while viene effettuata la ricerca e la sotituzione nel nuovo file 
        while(1)
        {
          
       
            Stop = strstr(Start, trova);
            if(NULL == Stop)
            {
                fwrite(Start, 1, strlen(Start), Output);
                break;
            }

            fwrite(Start, 1, Stop - Start, Output);

            fwrite(sostituisci, 1, strlen(sostituisci), Output);

            Start = Stop + strlen(trova);
    
     }
line++;



}
//nel codice seguente viene impresso sul file alla prima riga l'ultima modifica effettuata all'appello con data ora e ip del client che l'ha modificato
strftime(data,80,"ultima modifica in data %d-%m-%Y ora %H:%M da ",timeinfo);
strcat(data,ip);
puts(data);
rewind(Output);
fwrite(data, strlen(data),sizeof(char),Output);

memset(sostituisci, 0, sizeof(sostituisci)); 
memset(trova, 0, sizeof(trova));    
memset(data, 0, sizeof(data));
    fclose(Input);
    fclose(Output);
    remove(nomefile2);
    rename(filetemp, nomefile2);
 //nel seguente codice viene effettuata la connect al serverd e l'invio dell'appello modificato

int serverd_fd;
int fd;
serverd_fd = Socket(AF_INET,SOCK_STREAM,0);
if((connect(serverd_fd,(struct sockaddr *) &serverd,sizeof(struct sockaddr)) == -1))
	{
	printf("Il SERVERD é offline..impossibile inviare appello per l'archiviazione\n");
	}
	else {

	FullWrite(serverd_fd, &invio, sizeof(int));
//inviamo al serverd la lunghezza del nome del file e la stringa de inviare
uint32_t nomel = htonl(strlen(nomefile2));
write(serverd_fd, &nomel, sizeof nomel); 
write(serverd_fd, nomefile2, strlen(nomefile2)); 

//si apre l'appello modificato in lettura
FILE *fd = fopen(nomefile2, "r");
    if (fd < 0) {
      fprintf(stderr, "impossibile aprire '%s': %s\n", nomefile2, strerror(errno));
      exit(1);
    }

ssize_t bytes;
//si invia l'appello modficato al serverd
while ((bytes = fread(buffer2,sizeof(char), sizeof(buffer2),fd)) > 0)
{
   send(serverd_fd, buffer2, bytes, 0);
}
	
	
		printf("Trasmissione completata con successo\n\n");

	fclose(fd); 
close(serverd_fd);
	}

memset( nomefile2, '\0', sizeof( nomefile2 ) );


}break;

case 0:
{
// nel case 0 terminaimo la connessione col servers
printf("IL CLIENT SEGRETERIA STUDENTI HA CHIUSO LA CONNESSIONE..\n");
close(conn_fd);//Chiudo la connessione
exit(0);


}break;




}
}
}


