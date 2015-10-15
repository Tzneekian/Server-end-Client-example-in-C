#include "./wrappers.h"




void scelta (int sock); //prototipo della funzione scelta
int main(int argc , char *argv[])
{


    int sock,scelta2=2;

    struct sockaddr_in server,localaddr;//qui vengono definite le strutture per assegnare una porta e un ip al socket
    
      
    //Crea il socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("impossibile creare il socket\n");
    }
    puts("Socket creato\n");
      
//qui definiamo porta e indirizzo ip del clientp
localaddr.sin_family = AF_INET;
 localaddr.sin_addr.s_addr = inet_addr("127.0.0.4");
 localaddr.sin_port = htons(0); 
 bind(sock, (struct sockaddr *)&localaddr, sizeof(localaddr));//qui tramite la Bind assegnamo un indirizzo al socket
//qui definiamo l'indirzzo ip e la porta per connettersi al servers
    server.sin_addr.s_addr = inet_addr("127.0.0.5");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
  

  Connect(sock, (struct sockaddr *) &server, sizeof(server));//La funzione accept permette al server di accettare connessioni in entrata 
FullWrite(sock, &scelta2, sizeof(int));//inviamo un intero al serverd per accertare cosa vogliamo fare

scelta(sock);

return 0;
}

void scelta (int sock)
{
// qui definiamo tutte le variabili necessarie al funzionamento del client
char message[50] , message2[1024],server_reply[50],buffer[1024],home[400],buffer2[1024];
 int scelta=0,nread,nread2,n,sceltafile=0,mscelta=0,linea=0;  

// il while ci permette di entrare in un ciclo infinito e uscirne soltanto quando il client digita 0


while (scelta !=5)
{

  

printf("\n\t\t\t MENU SEGRETERIA STUDENTI\t\t\t\n");
printf("\nCosa vuoi fare? \n");	//Menu' dei servizi che il client può richiedere
            printf("1--> Appelli\n");
            printf("2--> Visualizza esami appello\n");
            printf("3--> Modifica Esame(nome/cognome/voto)\n");
            printf("4--> Modifica totale(modifica matricole e interi record)\n");
            printf("0--> Uscita\n");


printf ("\nDigita la tua scelta: ");
scanf ("%d", &scelta);


FullWrite(sock, &scelta, sizeof(int));

switch (scelta)
{
case 1:
{
// nel case 1 vengono visualizzati gli appelli
system("clear");
printf("\n\n\n##############LISTA APPELLI##################\n\n\n");
recv(sock,buffer2,1024,0); 
        puts(buffer2);
memset(buffer2, 0, sizeof(buffer2));
printf("\n\n\n##################FINE#######################\n\n\n");

}break;


case 2:
{
// nel case 2 vengono visualizzati gli esami presenti in uno specifico appello
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
printf("\n\n\n##############APPELLO##################\n\n\n");
recv(sock,buffer2,1024,0); 
        puts(buffer2);
       memset(buffer2, 0, sizeof(buffer2)); 
printf("\n\n\n############FINE#######################\n\n\n");

}break;

case 3:
{
// nel case 3 si ha la possibilità di modificare nome/cognome/voto di un esame
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));

}break;

case 4:
{
// nel case 4 si ha possibilità di modificare tutte le altre informazioni relative ad un esame
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
recv(sock,server_reply,50,0);
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));
recv(sock,server_reply,50,0); 
        puts(server_reply);
       memset(server_reply, 0, sizeof(server_reply)); 
scanf("%s",message);
     send(sock,message,strlen(message),0);
        memset(message, 0, sizeof(message));

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

