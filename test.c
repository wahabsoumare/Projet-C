#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"fonction.h"

int main() {
    Student student;
    Date date;

    Message message;
    message = writeMessageToAll();
    int succes = sendMessageToAll(message, "ADMIN");
    if(succes){
        printf("Message envoyer avec succes\n");
    } else{
        printf("Messagve non envoyer\n");
    }
    readMessage("messageToAll.txt");
    
return 0; 
}
