#ifndef FONCTION_H
#define FONCTION_H
#define MAXLENGTH 30


typedef struct{
    char username[MAXLENGTH], password[MAXLENGTH];
}User;


// int adminMenu(void);

int isauthenticate(User, char*);
void isconnected(void);

#endif