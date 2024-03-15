#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<unistd.h>
#include<ncurses.h>
#include<termios.h>
#include<dirent.h>
#include"fonction.h"
#define MAXCHARACTERS 25
#define CHARACTEREBYROW 50
#define LETTERSTOVERIFY 3
#define CHARACTERBYLINE 1000
int msgCount;
int autoStudentID = 0; 

Date inputDate(char msg[]){
    Date date;
    printf("%s sous format (jj/mm/aaaa) : ", msg);
    scanf("%d %d %d", &date.day, &date.month, &date.year);
return date;
}

int isLeapyear(Date date){
    return ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0));
}

int isvalidDayinMonth(Date date){
    switch (date.month){
        case 4: case 6: case 9: case 11:
            return (date.day > 30) ? 0 : 1;
            
        break;

        case 2:
            if(!(isLeapyear(date))){
                return (date.day > 28) ? 0 : 1;
            } else{
                return (date.day > 29) ? 0 : 1;
            }
        break;

        default:
            return (date.day > 31) ? 0 : 1;
        break;
    }
}
int isvalideDate(Date date){
    if(date.month < 1 || date.month > 12){
        return 0;
    }
    if(!isvalidDayinMonth(date)){
        return 0;
    }

return 1;
}

int adminMenu(void){
    int choice;

    do{
        printf("===Menu===\n");
        printf("1.Marquer presence\n2.Generer des fichiers\n3.Envoyer des messages\n4.Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choice);
        while(getchar() != '\n');

        if(choice < 1 || choice > 3){
            printf("Choix indisponible\n");
            system("clear");
        }

    }while(choice < 1 || choice > 4);

return choice;
}

void manageAdmin(){
    home:
    int choice = adminMenu();
    switch(choice){
        case 1:
            markPresence();
        break;

        case 2:
            printf("Toto tape pathe\n");
        break;

        case 3:
            system("clear");
            generateAllPresenceList("Presence");
            printf("L'ensemble des presences on ete enregistre dans Liste_de_tous_les_presence.txt\n");
            usleep(600000);
            system("clear");
            goto home;
        break;

        case 4:
            Message message;
            message = writeMessage();
            int succefuly = sendMessage(message, "ADMIN");
            if(succefuly){
                printf("Message envoyer avec succes\n");
                usleep(500000);
                system("clear");
                goto home;
            } else{
                printf("Message non envoye\n");
                usleep(500000);
                goto home;
            }
        break;

        case 5:
            printf("Toto tape pathe\n");
        break;
    }
}




int studentMenu(void){
    int choice;
    msgCount = 0;

    do{
        msgCount = countMessageNumber("messageToAll.txt") / 5;
        printf("===Menu===\n");
        printf("1.Marquer ma presence\n2.Nombre de minutes d'absence\n3.Mes messages(%d)\n4.Deconnexion\n", msgCount);
        printf("Votre choix : ");
        // while(getchar() != '\n');
        scanf("%d", &choice);
        while(getchar() != '\n');
        

        if(choice < 1 || choice > 4){
            printf("Choix indisponible\n");
        }

    }while(choice < 1 || choice > 4);

return choice;
}

void manageStudent(int id){
    home:
    int choice = studentMenu();
    switch(choice){
        case 1:
            markPresenceforStudent(id);
            system("clear");
            goto home;
        break;

        case 2:
            printf("Toto tape pathe\n");
        break;

        case 3:
        system("clear");
            readMessage("messageToAll.txt");
            int quit;
            do{
                printf("\nTaper 1 pour quitter : ");
                scanf("%d", &quit);
                if(quit != 1){
                    printf("Commende indisponible\n");
                }
            }while(quit != 1);

            if(quit == 1){
                system("clear");
                msgCount = 0;
                goto home;
            }
            
        break;

        case 4:
            printf("Toto tape pathe\n");
        break;
    }
}

int isauthenticate(User user, char filename[]){
    FILE *fileLogin = fopen(filename, "r");
    User tempuser;
    char goodUsername[MAXCHARACTERS], goodPassword[MAXCHARACTERS];
    int succes = 0;
    if(fileLogin == NULL){
        fprintf(stderr, "Impossible d'acceder au ficher de connexion\n");
        exit(EXIT_FAILURE);
    } else{
        while(!feof(fileLogin)){
            fscanf(fileLogin,"%s %s %d", tempuser.username, tempuser.password, &tempuser.id);
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

int getStudentID(char username[], char filename[]) {
    FILE *fileToRead = fopen(filename, "r");
    char line[CHARACTERBYLINE];
    int tempID;
    char tempUsername[MAXLENGTH];

    if (fileToRead == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else {
        while (fgets(line, CHARACTERBYLINE, fileToRead) != NULL) {
            sscanf(line, "%s  %*s %d", tempUsername, &tempID);
            if (strcmp(username, tempUsername) == 0) {
                fclose(fileToRead);
                return tempID;
            }
        }
    }
    fclose(fileToRead);
return 0;
}



void isconnected(){
    User user;
    char usern[MAXCHARACTERS], pwd[MAXCHARACTERS];
    char character;
    char twoFirstLetter[LETTERSTOVERIFY]; 
    char fileLogin[] = "userLogin.txt";
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

            if(answer != 'n'){
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

    if(strcmp(twoFirstLetter, "AD") == 0 && succes == 1){
        system("clear");
        manageAdmin();
    }

    if(strcmp(twoFirstLetter, "ST") == 0 && succes == 1){
        int id = getStudentID(user.username, fileLogin);
        system("clear");
        manageStudent(id);
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
    student->birth = inputDate("Donner la date ");
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

    char todaysPresence[MAXLENGTH] = "Presence";
    strcat(todaysPresence, formattedDate);
    strcat(todaysPresence, ".txt");

    FILE *fileODC = fopen("ODC.txt", "r");
    FILE *presenceList = fopen(todaysPresence, "a");
    if(fileODC == NULL || presenceList == NULL){
        fprintf(stderr, "Impossible d'ouvrir les fichiers\n");
        exit(EXIT_FAILURE);
    }

    else {
        char line[CHARACTERBYLINE], row[20];
        fprintf(presenceList, "\t\t===Liste de presence du %s===\n\n", formattedDate);
        int succes;
        char quit[20];
        do {
            printf("Taper votre id unique ou 'q' pour quitter : ");
            scanf(" %s", quit);

            if(strcmp(quit, "q") == 0){
                char character;
                int length = 0;
                User admin;
                strcpy(admin.username, "AD-ADMIN");
                printf("Donner votre mot de passe : ");
                while(getchar() != '\n');
                struct termios old_term, new_term;
                tcgetattr(STDIN_FILENO, &old_term);
                new_term = old_term;
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
                if(isauthenticate(admin, "userLogin.txt")){
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
                    system("clear");
                    int choice = adminMenu();
                    manageAdmin(choice);
                } else{
                    printf("Mot de passe incorrect\n");
                    usleep(200000);
                    system("clear");
                }
            } else {
                sscanf(quit, "%d", &id);
                succes = verifyID(id, "ODC.txt");

                if (succes) {
                    rewind(fileODC);
                    char line[MAXLENGTH];
                    while (fgets(line, sizeof(line), fileODC) != NULL) {
                        int tempID;
                        char matricule[MAXLENGTH], lastname[MAXLENGTH], firstname[MAXLENGTH], classe[MAXLENGTH];
                        sscanf(line, "%d %s %s %s %s", &tempID, matricule, lastname, firstname, classe);

                        if (tempID == id) {
                            if(isAlreadyPresent(tempID, todaysPresence)){
                                printf("Vous vous êtes déjà enregistré.e\n");
                                usleep(200000);
                                system("clear");
                                break;
                            }
                            else {
                                time_t rawtime;
                                struct tm * timeinfo;
                                char timeString[MAXLENGTH];
                                time(&rawtime);
                                timeinfo = localtime(&rawtime);
                                strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
                                fprintf(presenceList, "\n%d\t %s\t %s\t %s\t present le %s\t a %s\n", tempID, matricule, lastname, firstname, formattedDate, timeString);
                                printf("Présence enregistrée pour %s %s à %s\n", lastname, firstname, timeString);
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



void markPresenceforStudent(int id){
    
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
    FILE *presenceList = fopen(todaysPresence, "a");
    if(fileODC == NULL || presenceList == NULL){
        fprintf(stderr, "Impossible d'ouvrir les fichiers\n");
        exit(EXIT_FAILURE);
    }

    else{
        char line[CHARACTERBYLINE], row[20];
        int succes;
        char quit;
        
        succes = verifyID(id, "ODC.txt");

        if (succes){
            rewind(fileODC);
            char line[MAXLENGTH];
            while (fgets(line, sizeof(line), fileODC) != NULL){
                        
                int tempID;
                char matricule[MAXLENGTH], lastname[MAXLENGTH], firstname[MAXLENGTH];
                sscanf(line, "%d %s %s %s", &tempID, matricule, lastname, firstname);

                if (tempID == id){
                    if(isAlreadyPresent(tempID, todaysPresence)){
                        printf("Vous vous etes deja enregistre.e\n");
                        usleep(300000);
                        system("clear");
                        // studentMenu();
                        break;
                    }

                    else{
                        time_t rawtime;
                        struct tm * timeinfo;
                        char timeString[MAXLENGTH];

                        time(&rawtime);
                        timeinfo = localtime(&rawtime);

                        strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
                                
                        fprintf(presenceList, "\n%d\t %s\t %s\t %s\t present le %s\t a %s\n", tempID, matricule, lastname, firstname, formattedDate, timeString);
                        printf("Presence enregistree pour %s %s a %s\n", lastname, firstname, timeString);

                        usleep(1000000);
                        system("clear");

                        fflush(presenceList);
                        studentMenu();
                    }
                }
            }
        }fclose(fileODC); fclose(presenceList);
    }
}




void writeToFile(FILE *fileInput, FILE *fileOutput){

    if(fileInput == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        char line[CHARACTERBYLINE];
        while(fgets(line, sizeof(line), fileInput) != NULL) {
            fprintf(fileOutput, "%s", line);
        }
    }
}


void generateAllPresenceList(char reference[]){
    FILE *fileToWrite = fopen("Liste_de_tous_les_presence.txt", "w");

    if(fileToWrite == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        DIR *directory = opendir(".");
        if(directory == NULL){
            fprintf(stderr, "Impossible d'ouvrir le repertoire\n");
            exit(EXIT_FAILURE);
        } else{
            struct dirent *input;
            while((input = readdir(directory)) != NULL){
                if(strncmp(input->d_name, reference, strlen(reference)) == 0){
                    FILE *fileToRead = fopen(input->d_name, "r");
                    if(fileToRead != NULL){
                        writeToFile(fileToRead, fileToWrite);
                        fclose(fileToRead);
                    } else {
                        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", input->d_name);
                    }
                }
            }
            closedir(directory);
        }
        fclose(fileToWrite);
    }
}

Message writeMessage(void){
    Message message;
    printf("Donner le matricule de l'etudiant: ");
    scanf("%s", message.receiver);
    getchar();
    printf("Ecriver vcotre message : ");
    fgets(message.content, sizeof(message.content), stdin);
return message;
}

Message writeMessageToAll(void){
    Message message;
    // getchar();
    printf("Ecriver vcotre message : ");
    fgets(message.content, sizeof(message.content), stdin);
    message.content[strcspn(message.content, "\n")] = '\0';
return message;
}

int sendMessage(Message message, char sender[]){
    char receiver[MAXLENGTH];
    strcpy(receiver, message.receiver);
    strcat(receiver, ".txt");
    strcpy(message.sender, sender);

    time_t date;
    struct tm *localTime;
    char formattedDate[MAXLENGTH];
    date = time(NULL);
    localTime = localtime(&date);
    strftime(formattedDate, sizeof(formattedDate), "%d-%m-%Y", localTime);

    time_t rawtime;
    struct tm * timeinfo;
    char timeString[MAXLENGTH];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
    FILE *fileReceiver = fopen(receiver, "w");

    if(fileReceiver == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        fprintf(fileReceiver, "Message de : %s\n\nDate : le %s a %s\n\nContenue : %s", message.sender, formattedDate, timeString, message.content);
        return 1;
    } fclose(fileReceiver);
return 0;
}

int sendMessageToAll(Message message, char sender[]){
    strcpy(message.sender, sender);

    time_t date;
    struct tm *localTime;
    char formattedDate[MAXLENGTH];
    date = time(NULL);
    localTime = localtime(&date);
    strftime(formattedDate, sizeof(formattedDate), "%d-%m-%Y", localTime);

    time_t rawtime;
    struct tm * timeinfo;
    char timeString[MAXLENGTH];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", timeinfo);
    FILE *fileReceiver = fopen("messageToAll.txt", "w");

    if(fileReceiver == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        fprintf(fileReceiver, "Message de : %s\n\nDate : le %s a %s\n\nContenue : %s", message.sender, formattedDate, timeString, message.content);
        return 1;
    } fclose(fileReceiver);
return 0;
}

void readMessage(char fileMessage[]){
    FILE *fileToRead = fopen(fileMessage, "r");
    char line[CHARACTERBYLINE], character;
    if(fileToRead == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        while(!feof(fileToRead)){
            fgets(line, sizeof(line), fileToRead);
            printf("%s", line);
        }
    } fclose(fileToRead);
}

int countMessageNumber(char fileMessage[]){
    int count;
    FILE *fileToRead = fopen(fileMessage, "r");
    char line[CHARACTERBYLINE];
    if(fileToRead == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier\n");
        exit(EXIT_FAILURE);
    } else{
        while(fgets(line, sizeof(line), fileToRead) != NULL){
            count++;
        }
    } fclose(fileToRead);
return count;
}