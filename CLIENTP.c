#include "./wrappers.h"


void scelta (int sock);//prototipo della funzione scelta
int main(int argc , char *argv[])
{


    int sock,invio=0;

    struct sockaddr_in server,localaddr;//qui vengono definite le strutture per assegnare una porta e un ip al socket
    
      
    //Crea socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("impossibile creare il socket\n");
    }
    puts("Socket creato\n");
      
//qui definiamo porta e indirizzo ip del clientp
localaddr.sin_family = AF_INET;
 localaddr.sin_addr.s_addr = inet_addr("127.0.0.2");
 localaddr.sin_port = htons(0); 
 bind(sock, (struct sockaddr *)&localaddr, sizeof(localaddr));//qui tramite la Bind assegnamo un indirizzo al socket
//qui definiamo l'indirzzo ip e la porta per connettersi al serverd
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(1024);
  

  Connect(sock, (struct sockaddr *) &server, sizeof(server));//La funzione accept permette al server di accettare connessioni in entrata 
FullWrite(sock, &invio, sizeof(int));//inviamo un intero al serverd per accertare cosa vogliamo fare


scelta(sock);

return 0;
}

void scelta (int sock)
{
// qui definiamo tutte le variabili necessarie al funzionamento del client
char message[1024] , message2[1024],server_reply[2000],buffer[1024],home[400],buffer2[1024];
 int scelta=0,nread,n,sceltafile=0,valid=0;  

// 	qui procediamo con l'autenticazione del docente che viene gestita dal serverd
printf("\n\n\n##############AUTENTICAZIONE DOCENTE##################\n\n\n");
  recv(sock,server_reply,2000,0); //rec username
        puts(server_reply);
        scanf("%s",message);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));
 
recv(sock,server_reply,2000,0); //recv password
        puts(server_reply);
        scanf("%s",message);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));
 

 recv(sock,server_reply,2000,0); //auth
        puts(server_reply);
 if(strcmp(server_reply,"Errore:Coppia non valida!\n")==0)
 exit(0);

 memset(&server_reply[0], 0, sizeof(server_reply));     
         
      


// il while ci permette di entrare in un ciclo infinito e uscirne soltanto quando il client digita 0

while (scelta !=5)
{

 
printf("\n\t\t\t MENU DOCENTI\t\t\t\n");
printf("\nCosa vuoi fare? \n");	//Menu' dei servizi che il client può richiedere
            printf("1--> Crea appello\n");
            printf("2--> Registra Esame\n");
            printf("3--> Visualizza appelli\n");
            printf("4--> Chiudi Appello\n");
            printf("0--> Uscita\n");




printf ("\nDigita la tua scelta: ");
scanf ("%d", &scelta);


FullWrite(sock, &scelta, sizeof(int));

switch (scelta)
{
case 1:
{
// nel case 1 diamo la possibilità al docente di creare un appello con le relative informazioni
printf("\n\n\n##############CREAZIONE APPELLO##################\n\n\n");
recv(sock,server_reply,2000,0); // nome file
        puts(server_reply);
 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));
 

    recv(sock,server_reply,2000,0); 
        puts(server_reply);
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));  
     
        recv(sock,server_reply,2000,0); 
        puts(server_reply);
   
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply)); 
        
        recv(sock,server_reply,2000,0); 
        puts(server_reply);
 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply)); 
       
        recv(sock,server_reply,2000,0); 
        puts(server_reply);
   
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));  

   recv(sock,server_reply,2000,0); 
        puts(server_reply);
   
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));

recv(sock,server_reply,2000,0); //con questa recv verifichiamo se l'appello viene creato o meno
        puts(server_reply);
        memset(&server_reply[0], 0, sizeof(server_reply));
printf("\n\n\n##############FINE CREAZIONE APPELLO##################\n\n\n");

}break;


case 2:
{

// nel case 2 il docente può registrare un esame all'interno di uno specifico appello
system("clear");
printf("\n\n\n####################REGISTRAZIONE ESAME##################\n\n\n");

recv(sock,server_reply,2000,0); 
        puts(server_reply);
scanf("%s",message);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply)); 
 
 recv(sock,server_reply,2000,0); 
        puts(server_reply); 
                memset(&server_reply[0], 0, sizeof(server_reply));


 recv(sock,server_reply,2000,0); 
        puts(server_reply); 
                memset(&server_reply[0], 0, sizeof(server_reply)); 


fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));


 recv(sock,server_reply,2000,0); 
        puts(server_reply); 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));


      recv(sock,server_reply,2000,0);
        puts(server_reply); 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));

     recv(sock,server_reply,2000,0); 
        puts(server_reply); 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));

 recv(sock,server_reply,2000,0); 
        puts(server_reply); 
fgets(message,1000,stdin);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));



printf("\n\n\n##############FINE REGISTRAZIONE ESAME##################\n\n\n");





}break;

case 3:
{
// nel case 3 il docente visualizza gli appelli disponibili
system("clear");
printf("\n\n\n##############LISTA APPELLI##################\n\n\n");
recv(sock,buffer2,1024,0); 
        puts(buffer2);
memset(buffer2, 0, sizeof(buffer2));
printf("\n\n\n###################FINE######################\n\n\n");


}break;

case 4:
{
// nel case 4 il docente può chiudere un appello
system("clear");
printf("\n\n\n#################CHIUDI APPELLO######################\n\n\n");
recv(sock,server_reply,2000,0); 
        puts(server_reply);
scanf("%s",message);
        send(sock,message,strlen(message),0);
        memset(&server_reply[0], 0, sizeof(server_reply));
printf("\n\n\n###############FINE CHIUDI APPELLO####################\n\n\n");

}break;


case 0:
{
// uscita dal while e chiusura della connessione
printf("CONNESSIONE COL SERVERD TERMINATA..\n");
close(sock);//Chiudo la connessione
exit(0);

}break;


     }
 } 

}

