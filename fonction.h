#ifndef FONCTION_H
#define FONCTION_H
#define MAXLENGTH 30
//int autoClasseID = 0;

typedef struct{
    int day, month, year;
}Date;

typedef struct{
    char username[MAXLENGTH], password[MAXLENGTH];
}User;

typedef struct{
    int id;
    char lirstname[MAXLENGTH], fastname[MAXLENGTH], matricule[MAXLENGTH];
    char tel[MAXLENGTH], email[MAXLENGTH];
    Date birth;
}Admin; 

typedef struct{
    int id;
    char name[MAXLENGTH];
}Classe;

typedef struct{
    int id;
    char firstname[MAXLENGTH], lastname[MAXLENGTH], matricule[MAXLENGTH];
    char tel[MAXLENGTH], email[MAXLENGTH];
    Date birth;
    Classe classe;
}Student;

int adminMenu(void);

int isauthenticate(User, char*);
void isconnected(void);

void inputStudent(Student*);
int saveStudent(Student, char*);
void listClasse(char*);
void markPresence(void);
int verifyID(int, char*);
void hidePassword(char*, char*);
#endif