#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<ncurses.h>
#include"fonction.h"
#define MAXCHARACTERS 25
#define CHARACTEREBYROW 50
#define LETTERSTOVERIFY 3
int autoStudentID = 0;

Date inputDate(){
    Date date;
    //printf("Saisissez la date (jj/mm/aaaa) : ");
    scanf("%d %d %d", &date.day, &date.month, &date.year);
return date;
}

int adminMenu(void){
    int choice;

    do{
        printf("===Menu===\n");
        printf("1.Marquer presence\n2.Listes presence\3.Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        if(choice < 1 || choice > 3){
            printf("Choix indisponible\n");
        }

    }while(choice < 1 || choice > 3);

return choice;
}


int studentMenu(void){
    int choice;

    do{
        printf("===Menu===\n");
        printf("1.Marquer ma presence\n2.Nombre de minutes d'absence\n3.Mes messages\n4.Deconnexion\n");
        scanf("%d", &choice);
        printf("Votre choix : ");
        while(getchar() != '\n');

        if(choice < 1 || choice > 4){
            printf("Choix indisponible\n");
        }

    }while(choice < 1 || choice > 4);

return choice;
}


int isauthenticate(User user, char filename[]){
    FILE *fileLogin = fopen(filename, "rb");
    User tempuser;
    char goodUsername[MAXCHARACTERS], goodPassword[MAXCHARACTERS];
    int succes = 0;
    if(fileLogin == NULL){
        fprintf(stderr, "Impossible d'acceder au ficher de connexion\n");
        exit(EXIT_FAILURE);
    } else{
        while(fread(&tempuser, sizeof(User), 1, fileLogin) == 1){
            
            strcpy(goodUsername, tempuser.username);
            strcpy(goodPassword, tempuser.password);

            if(strcmp(user.username, goodUsername) == 0 && strcmp(user.password, goodPassword) == 0){
                succes = 1;
                break;
            }
        }
    } fclose(fileLogin);

return succes;
}



void isconnected(){
    User user;
    char usern[MAXCHARACTERS], pwd[MAXCHARACTERS];
    char character;
    char twoFirstLetter[LETTERSTOVERIFY]; 
    char fileLogin[] = "userLogin";
    int length = 0;
    char answer;
    int succes;

    initscr();
    curs_set(0);

    do{
        printw("===Veuillez vous authentifier===\n");

        do{
            printw("Nom d'utilisateur : ");
            scanw("%s", user.username);
            
            if(strlen(user.username) < 7){
                printw("Le nom  d'utilisateur doit faire au moins 7 caracteres\n");
            }

        }while(strlen(user.username) < 7);

        do{
        noecho();
        printw("Mot de passe : ");
        refresh();
        flushinp();

        while(1){
            character = getch();
            if(character == '\n'){
                user.password[length] = '\0';
                break;
            } else if((character == KEY_BACKSPACE) && length > 0){
                int x, y;
                getyx(stdscr, y, x);
                move(y, x - 1);
                delch();
                length--;
            } else if(length < MAXCHARACTERS){
                user.password[length++] = character;
                printw("*");
            }
            refresh();
        }

        if(strlen(user.password) < 7){
            printw("\nLe mot de passe doit faire au moins 7 caracteres\n");
            length = 0;
        }

        }while(strlen(user.password) < 7);

        // strcpy(user.username, usern);
        // strcpy(user.password, pwd);

        succes = isauthenticate(user, fileLogin);

        if(!succes){
            echo();
            printw("\nNom d'utilisateur ou mot de passe incorrect\n");
            length = 0;

            
            printw("Voullez vous reconnecter (o/n) : ");
            scanw(" %c", &answer);
            answer = tolower(answer);

            if(answer != 'o'){
                printw("\nBye!\n");
                refresh();
                endwin();
                exit(EXIT_SUCCESS);
            }
        } 

        
    }while(!succes || answer == 'o');

    refresh();
    endwin();
    strncpy(twoFirstLetter, user.username, 2);

    int choice;
    if(strcmp(twoFirstLetter, "AD") == 0 && succes == 1){
        choice = adminMenu();
    }

    if(strcmp(twoFirstLetter, "ST") == 0 && succes == 1){
        choice = studentMenu();
    }
    
    
}
void generateMatricule(char letter[], char firstname[], char lastname[], char matricule[]){

    strcpy(matricule, "ST");
    matricule[2] = '-';
    strncat(matricule, firstname, 2);
    strncat(matricule, lastname, 2);
    for(int i = 0; i < strlen(matricule); i++){
        matricule[i] = toupper(matricule[i]);
    }
}

void inputStudent(Student *student){
    student->id = ++autoStudentID;
    printf("Donner le nom de l'apprenant : ");
    scanf("%s", student->lastname);
    printf("Donner le prenom de l'apprenant : ");
    scanf("%s", student->firstname);
    printf("Donner le mail de l'apprenant : ");
    scanf("%s", student->email);
    printf("Donner le telephone de l'apprenant : ");
    scanf("%s", student->tel);
    printf("Donner la date de naissance de l'apprenant : ");
    student->birth = inputDate();
    printf("Donner la classe de l'etudiant : ");
    scanf("%s", student->classe.name);
    generateMatricule("ST", student->firstname, student->lastname, student->matricule);


}
int saveStudent(Student student, char classe[]){
    char filename[MAXLENGTH] = "filStudent.txt";
    //fscanf(filename, "-%s", classe);
    strcpy(filename, "StudentList-");
    // strcat(filename, classe);
    strcat(filename, ".txt");
    FILE *fileStudent = fopen(filename, "w");
    if(fileStudent == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        return 0;
    }
    else{
        fprintf(fileStudent, "ID : %d\tMatricule = %s\tNom : %s\tPrenom : %s\tMail : %s\tTelephone : %s\tDate de naissance : %d/%d/%d\tClasse : %s\n", student.id, student.matricule, student.lastname, student.firstname, student.email, student.tel, student.birth.day, student.birth.month, student.birth.year, student.classe.name);
        fclose(fileStudent);
    } 
return 1;
}