#include<stdio.h>
#include<string.h>
#include"fonction.h"

int main(){
    Student student;

    inputStudent(&student);
    saveStudent(student, student.classe.name);
return 0;
}
