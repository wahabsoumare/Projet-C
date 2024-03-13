#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"fonction.h"

int main() {
    Student student;
    Classe classe;
    Message message;
    User user;
    // // message = writeMessage();
    // // char sender[] = "ADMIN";
    // // int sucefuly = sendMessageByStudent(message, sender);
    
    // // if(sucefuly){
    // //     printf("Message envoyer avec succes\n");
    // // } else{
    // //     printf("Message non envoyer\n");
    // // }
    // classe = getStudentClasse(4, "userLogin.txt");
    // printf("%s\n", classe.name);
    strcpy(user.username, "ST-KAHA");
    strcpy(user.password, "passerST123");
    int succes = isauthenticate(user, "userLog.txt");
    printf("%d\n", succes);
return 0; 
}
