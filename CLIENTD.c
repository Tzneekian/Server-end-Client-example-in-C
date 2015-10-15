#include "./wrappers.h"


void scelta (int sock);//prototipo della funzione scelta
int main(int argc , char *argv[])
{


    int sock,invio=0;

    struct sockaddr_in server,localaddr;//qui vengono definite le strutture per assegnare una porta e un ip al socket
    
      
    //Crea il socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("impossibile creare il socket");
    }
    puts("Socket creato");
      
//qui definiamo porta e indirizzo ip del clientd
localaddr.sin_family = AF_INET;
 localaddr.sin_addr.s_addr = inet_addr("127.0.0.3");
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
char message[1024] , message2[1024],server_reply[2000],buffer[1024],home[400],buffer2[300];
 int scelta=0,nread,nread2,n,sceltafile=0;  




// il while ci permette di entrare in un ciclo infinito e uscirne soltanto quando il client digita 0


while (scelta !=5)
{

 

printf("\n\t\t\t MENU SEGRETERIA DIDATTICA\t\t\t\n");
printf("\nCosa vuoi fare? \n");	//Menu' dei servizi che il client può richiedere
            printf("1--> Visualizza Appelli aperti\n");
            printf("2--> Visualizza Appelli chiusi\n");
            printf("3--> Visualizza esami appello\n");
            printf("0--> Uscita\n");



printf ("\nDigita la tua scelta: ");
scanf ("%d", &scelta);


FullWrite(sock, &scelta, sizeof(int)); // inviamo l'intero al serverD per decidere cosa fare

switch (scelta)
{
case 1:
{
// nel case 1 permettiamo al client di visualizzare tutti gli appelli non chiusi
system("clear");//Pulizia dello schermo
printf("\n\n\n##############LISTA APPELLI##################\n\n\n");
recv(sock,buffer2,1024,0); 
        puts(buffer2);
memset(buffer2, 0, sizeof(buffer2));
printf("\n\n\n##################FINE#######################\n\n\n");
}break;


case 2:
{
// nel case 2 permettiamo al client di visualizzare tutti gli appelli chiusi
system("clear");//Pulizia dello schermo
printf("\n\n\n##############LISTA APPELLI##################\n\n\n");
recv(sock,buffer2,1024,0); 
        puts(buffer2);
memset(buffer2, 0, sizeof(buffer2));
printf("\n\n\n##################FINE#######################\n\n\n");
}break;


case 3:
{
// nel case 3 permettiamo di visualizzare al clientD il contenuto di un appello: esami e informazioni dell'appello
recv(sock,server_reply,1000,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
printf("\n\n\n##############ESAME##################\n\n\n");
recv(sock,buffer2,1024,0); // nome file
        puts(buffer2);
       memset(buffer2, 0, sizeof(buffer2)); 
printf("\n\n\n############FINE ESAME###############\n\n\n");

}break;



case 0:
{
// case 0 permette di uscire dal while e di terminare la connessione
printf("CONNESSIONE COL SERVERD TERMINATA..\n");
close(sock);//Chiudo la connessione
exit(0);

}break;

     }
 } 

}

