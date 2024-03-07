#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<unistd.h>
#include<ncurses.h>
#include<termios.h>
#include"fonction.h"
#define MAXCHARACTERS 25
#define CHARACTEREBYROW 50
#define LETTERSTOVERIFY 3
#define CHARACTERBYLINE 1000
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

void hidePassword(char username[], char password[]){
    int length = 0;
    char character;
    initscr();
    curs_set(0);


        do{
            printw("Nom d'utilisateur : ");
            scanw("%s", username);
            
            if(strlen(username) < 7){
                printw("Le nom  d'utilisateur doit faire au moins 7 caracteres\n");
            }

        }while(strlen(username) < 7);

        do{
            noecho();
            printw("Mot de passe : ");
            refresh();
            flushinp();

            while(1){
                character = getch();
                if(character == '\n'){
                    password[length] = '\0';
                    break;
                } else if((character == KEY_BACKSPACE) && length > 0){
                    int x, y;
                    getyx(stdscr, y, x);
                    move(y, x - 1);
                    delch();
                    length--;
                } else if(length < MAXCHARACTERS){
                    password[length++] = character;
                    printw("*");
                }
                refresh();
            }

            if(strlen(password) < 7){
                printw("\nLe mot de passe doit faire au moins 7 caracteres\n");
                length = 0;
            }

        }while(strlen(password) < 7);
        echo();
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

        

        succes = isauthenticate(user, fileLogin);

        if(!succes){
            echo();
            printw("\nNom d'utilisateur ou mot de passe incorrect\n");
            length = 0;
            system("clear");
            
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
        system("clear");
        choice = adminMenu();
        switch(choice){
            case 1:
                system("clear");
                markPresence();
            break;
        }
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
    
    strcat(classe, ".txt");
    FILE *fileStudent = fopen(classe, "a");
    if(fileStudent == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        return 0;
    }
    else{
        fprintf(fileStudent, "\n%d\t%s\t %s\t%s\t%s\t%s\t%d/%d/%d\t%s\n", student.id, student.matricule, student.lastname, student.firstname, student.email, student.tel, student.birth.day, student.birth.month, student.birth.year, student.classe.name);
        fclose(fileStudent);
    } 
return 1;
}

void listClasse(char classe[]){
    int row = 1000;
    char string[row];
    strcat(classe, ".txt");
    FILE *list = fopen(classe, "r");

    if(list == NULL){
        fprintf(stderr, "Imposible d'acceder au fichier\n");
        exit(EXIT_FAILURE);
    }
    else{
        while(fgets(string, row, list) != NULL){
            printf("%s\n", string);
        }
    } fclose(list);
}

int verifyID(int id, char file[]){
    char line[MAXLENGTH];
    FILE *fileToVerify = fopen(file, "r");
    if (fileToVerify == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } 
    else{
        while (fgets(line, sizeof(line), fileToVerify) != NULL) {
            int tempID;
            if (sscanf(line, "%d", &tempID) == 1) {
                if (id == tempID) {
                    fclose(fileToVerify);
                    return 1;
                }
            }
        }
    } fclose(fileToVerify);
return 0;
}

int isAlreadyPresent(int id, char filename[]) {
    FILE *presenceList = fopen(filename, "r");
    if (presenceList == NULL) {
        perror("Erreur lors de l'ouverture du fichier de présence");
        exit(EXIT_FAILURE);
    }

    char line[MAXLENGTH];
    while (fgets(line, sizeof(line), presenceList) != NULL) {
        int tempID;
        sscanf(line, "%d", &tempID);
        if (tempID == id) {
            fclose(presenceList);
            return 1;
        }
    }

    fclose(presenceList);
    return 0;
}

void markPresence(void){
    int id;
    time_t date;
    struct tm *localTime;
    char formattedDate[MAXLENGTH];
    date = time(NULL);
    localTime = localtime(&date);
    strftime(formattedDate, sizeof(formattedDate), "%d-%m-%Y", localTime);

    char todaysPresence[MAXLENGTH] = "Presence-";
    strcat(todaysPresence, formattedDate);
    strcat(todaysPresence, ".txt");

    FILE *fileODC = fopen("ODC.txt", "r");
    FILE *presenceList = fopen(todaysPresence, "at");
    if(fileODC == NULL || presenceList == NULL){
        fprintf(stderr, "Impossible d'ouvrir les fichiers\n");
        exit(EXIT_FAILURE);
    }

    else {
        char line[CHARACTERBYLINE], row[20];
        fprintf(presenceList, "\t\t===Liste de presence du %s===\n\n", formattedDate);
        int succes;
        char quit;
        do {
            printf("Taper votre id unique ou 'q' pour quitter : ");
            scanf(" %c", &quit);

            if(quit == 'q'){
                    char character;
                    int length = 0;
                    User admin;

                    
                    strcpy(admin.username, "AD-TODI");
                    printf("Donner votre mot de passe : ");
                    while(getchar() != '\n');
                    struct termios old_term, new_term;

                    tcgetattr(STDIN_FILENO, &old_term);
                    new_term = old_term;

                    // Désactivation de l'affichage de la saisie
                    new_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
                    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
                    

                    while (1) {
                        char character = getchar();
                        if (character == '\n') {
                            admin.password[length] = '\0';
                            break;
                        } else {
                            admin.password[length++] = character;
                            printf("*");
                        }
                    }

                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

                    if(isauthenticate(admin, "userLogin")){
                        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
                        break;
                    } else{
                        printf("Mot de passe incorrect\n");
                        usleep(100000);
                        system("clear");
                    }
                    
                
            }else {
                sscanf(&quit, "%d", &id);
                succes = verifyID(id, "ODC.txt");

                if (succes) {
                    rewind(fileODC);
                    char line[MAXLENGTH];
                    while (fgets(line, sizeof(line), fileODC) != NULL) {
                        
                        int tempID;
                        char matricule[MAXLENGTH], lastname[MAXLENGTH], firstname[MAXLENGTH];
                        sscanf(line, "%d %s %s %s", &tempID, matricule, lastname, firstname);

                        if (tempID == id) {
                            if(isAlreadyPresent(tempID, todaysPresence)){
                                printf("Vous vous etes deja enregistre.e\n");
                                break;
                            }
                            else{
                            time_t rawtime;
                            struct tm * timeinfo;
                            char timeString[MAXLENGTH];

                             time(&rawtime);
                             timeinfo = localtime(&rawtime);

                            strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
                            
                            fprintf(presenceList, "%d %s %s %s present a %s\n", tempID, matricule, lastname, firstname, timeString);
                            printf("Presence enregistree pour %s %s a %s\n", lastname, firstname, timeString);

                            usleep(1000000);
                            system("clear");

                            fflush(presenceList);
                            break;
                            }
                        }
                    }
                }
            }
        }while(1);
        fclose(fileODC); fclose(presenceList);
    }
}