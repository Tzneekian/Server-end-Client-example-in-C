#include <stdlib.h> //libreria standard
#include <sys/socket.h>// libreria per la gestione socket
#include<unistd.h>//libreria standard Unix
#include<stdio.h>//libreria standard input/output 
#include<arpa/inet.h>//libreria per la conversione degli indirizzi ip
#include<string.h>//libreria per la gestione di stringhe
#include<syslog.h>//libreria che contiene le funzioni di sistema syslog
#include<errno.h>//libreria che contiene i codici degli errori di sistema
#include<fcntl.h>//libreria per le opzioni di controllo di un file
#include<netinet/in.h>//libreria che contiene le strutture necessarie per gli indirizzi di dominio di internet
#include <sys/stat.h>//libreria che ci permette di ottenere le informazioni di un file
#include <dirent.h> //libreria per accesso alle directory
#include <time.h> //libreria per ottenere data/ora dal sistema
#include<sys/types.h>//libreria types defined




//strutura per l'autenticazione docente
struct client_struct{
    char ip[20];
    char matricola[20];
    char password[20];
 
};

//funzione che estrare le ultime tre cifre da una matricola di MAX 10 cifre per assegnare a nome/cognome/voto una chiave univoca
char *matricole(char *nome,char *matricola)
 { 
char cifre[4]; 
cifre[3]=matricola[9]; 
cifre[2]=matricola[8]; 
cifre[1]=matricola[7]; 
cifre[0] = '.'; 
strcat(nome,cifre); 
return nome;
 }


//procedura che permette di chiudere un appello tramite  la funzione chmod che permette di cambiare i permessi di un file
void RDO(char *filename)
 {
    int stat;
 
    stat = chmod(filename, S_IREAD);
    if (stat)
       printf("impossibile impostare %s in read-only \n", filename);
    else
       printf(" %s ora è in read-only\n", filename);
 }

//procedura che elimina  il carattere whitespace dal nome di un file e lo sostituisce con underscore
void *rimuovi(char *array)
{
    int i , lenght ;
    lenght = strlen(array);
    for(i = 0 ; i < lenght ; i++)
     {
        if(array[i] == ' ')
            array[i] = '_' ;
     }
}

//funzione che elimina il carattere terminatore da una stringa ottenuta da una fgets
char *terminatore(char *s) {
  int i = strlen(s)-1;
  if ((i > 0) && (s[i] == '\n'))
    s[i] = '\0';
  return s;
}




int Socket(int family,int type, int protocol)
{
   int n;
  if ((n = socket(family,type,protocol)) < 0)
{
   perror("Socket Error");
    exit (1);
}
   return n;
}



int Connect (int sockfd, struct sockaddr *servaddr, socklen_t addrlen)
{

  int n; 

if ((n = connect (sockfd, servaddr, addrlen)) < 0)
{
    perror("Connect Error");
    exit(1);
}
    return n;
}



int Bind (int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int n;
if ((n=bind (sockfd, addr, addrlen)) < 0)
   {
    perror("Bind Error");
    exit(1);
     }

return n;

}

int Listen(int sockfd, int lunghezza_coda)
{
    int n;
if ((n = listen (sockfd, lunghezza_coda)) < 0)
   {
     perror("Listen Error");
     exit(1);
    }
    return n;
}


int Accept(int sockfd, struct sockaddr *clientaddr, socklen_t *addr_dim)
{
int n ;
   if ((n = accept(sockfd, clientaddr, addr_dim)) < 0)
   {

    perror("Accept Error");

    exit(1);

    }
   
return n;

}


    pid_t Fork()
{
     pid_t pid;
      if ((pid = fork()) < 0)
{
      perror("Fork Error");
      exit(-1);
 }
      return pid;
}






ssize_t FullWrite(int fd, const void *buf, size_t count)
{
   size_t nleft;

    ssize_t nwritten;
      nleft = count;

    while (nleft > 0)
{
     if ((nwritten = write(fd, buf, nleft)) < 0){

     if (errno == EINTR){

       continue;

        }else{
       return(nwritten);
}
    }  
     nleft -= nwritten;
    buf +=nwritten;

}
    return (nleft);
}

ssize_t FullRead (int fd, void *buf, size_t count)
{
size_t nleft;
ssize_t nread;
nleft = count;
while (nleft > 0)
{
if ((nread = read (fd, buf, nleft)) > 0){

if (errno == EINTR){

continue;

}else{
return (nread);


}


}else if (nread == 0){

break;
}
nleft -= nread; 
buf += nread;


}
return (nleft);

}
