#ifndef FONCTION_H
#define FONCTION_H
#define MAXLENGTH 30
#define MESSAGECONTENT 5000
//int autoClasseID = 0;
// enum classename{Dev_web, Dev_data, Ref_dig};
typedef struct{
    int day, month, year;
}Date;

typedef struct{
    char username[MAXLENGTH], password[MAXLENGTH];
    int id;
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

typedef struct{
    char sender[MAXLENGTH], receiver[MAXLENGTH];
    char content[MESSAGECONTENT];
}Message;

Date inputDate(char*);
int isLeapyear(Date);
int isvalidDayinMonth(Date);
int isvalideDate(Date);

int adminMenu(void);

int isauthenticate(User, char*);
void isconnected(void);

int verifyClasse(Classe);
void inputStudent(Student*);
int saveStudent(Student, char*);
void listClasse(char*);
void markPresence(void);
void markPresenceforStudent(int);
int verifyID(int, char*);
void hidePassword(char*, char*);
void homeMenu();
void manageAdmin();
void manageStudent(int);
int getStudentID(char*, char*);
Classe getStudentClasse(int, char*);
Student getStudentMatricule(int id, char*);
void writeToFile(FILE*, FILE*);
void generateAllPresenceList(char*);
Message writeMessage(void);
Message writeMessageToAll(void);
Message writeMessageToClasse(void);
int sendMessageByStudent(Message, char*);
int sendMessageToAll(Message, char*);
int sendMessageByClasse(Message, char*);
void readMessage(char fileMessage[]);
int countMessageNumber(char*);
#endif