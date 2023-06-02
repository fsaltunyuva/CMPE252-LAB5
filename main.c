#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned int id; // researcher id
    char name[20]; // researcher name
    char surname[20]; // researcher surname
    char department[20]; // department of researcher
    int citIndex; // citation index of the researcher
} researcher;

void showRecords(FILE *filePtr);

int modifyCitIndex(FILE *filePtr, unsigned int id, int increaseCit);
int insertResearcher(FILE *filePtr, unsigned int id, char name[], char surname[], char department[], int citIndex);
int removeResearcher(FILE *filePtr, unsigned int id);
int viewDepartmentCits(FILE *filePtr, char department[], int maxCit);

int main()
{
    unsigned int id;
    int increaseCit;
    int status;

    char name[20];
    char surname[20];
    char department[20];
    int citIndex;

    int count;
    int maxCitation;

    FILE *filePtr;
    filePtr = fopen("researcher.bin","rb+");
    if (filePtr == NULL)
    {
        printf("Could not open researcher.bin");
        return 0;
    }

    showRecords(filePtr);

    int choice;

    printf("\nWhich operation do you choose?\n");
    printf("1 : Update Citation Index\n");
    printf("2 : Add Researcher\n");
    printf("3 : Delete researcher\n");
    printf("4 : View Department Info with Citation\n");
    printf("> ");
    scanf("%d",&choice);

    switch (choice)
    {
        case 1:
            printf("\nResearcher id: ");
            scanf("%d",&id);
            printf("Increase value for citation index: ");
            scanf("%d",&increaseCit);
            status = modifyCitIndex(filePtr, id, increaseCit);
            if (status == 1)
                showRecords(filePtr);
            else
                printf("No researcher with id %d\n", id);
            break;
        case 2:
            printf("\nResearcher id: ");
            scanf("%d",&id);
            printf("Name: ");
            scanf("%s",name);
            printf("Surname: ");
            scanf("%s",surname);
            printf("Department: ");
            scanf("%s",department);
            printf("Citation Index: ");
            scanf("%d",&citIndex);
            status = insertResearcher(filePtr, id, name, surname, department, citIndex);
            if (status == 1)
                showRecords(filePtr);
            else
                printf("There is already an researcher with id %d\n", id);
            break;
        case 3:
            printf("\nResearcher id: ");
            scanf("%d",&id);
            status = removeResearcher(filePtr, id);
            if (status == 1)
                showRecords(filePtr);
            else
                printf("No researcher with id %d\n", id);
            break;
        case 4:
            printf("\nDepartment: ");
            scanf("%s",department);
            printf("\nMax Citation Index: ");
            scanf("%d",&maxCitation);
            count = viewDepartmentCits(filePtr, department, maxCitation);
            if (count == 0)
                printf("No researcher in department %s with citation index <= %d\n", department, maxCitation);
            else
                printf("There are %d researchers in department %s with citation index <= %d\n", count, department, maxCitation);
            break;
    }

    fclose(filePtr);
    return 0;
}

void showRecords(FILE *filePtr)
{
    fseek(filePtr, 0, SEEK_SET);

    printf("\n%-3s %-20s %-20s %-20s %s\n",
           "ID",
           "Name",
           "Surname",
           "Department",
           "Citation Index");

    while (!feof(filePtr))
    {
        researcher r;
        int result = fread(&r, sizeof(researcher), 1, filePtr);
        if (result != 0 && r.id != 0)
        {
            printf("%-3d %-20s %-20s %-20s %d\n",
                   r.id,
                   r.name,
                   r.surname,
                   r.department,
                   r.citIndex);
        }
    }
}

int modifyCitIndex(FILE *filePtr, unsigned int id, int increaseCit)
{
    fseek(filePtr, 0, SEEK_SET);

    while(!feof(filePtr)){
        researcher read;
        fread(&read, sizeof(read), 1, filePtr);
        if(read.id == id){
            read.citIndex += increaseCit;
            fseek(filePtr, -sizeof(read), SEEK_CUR);
            fwrite(&read, sizeof(read), 1, filePtr);
            return 1;
        }
    }

    return 0;
}

int insertResearcher(FILE *filePtr, unsigned int id, char name[], char surname[], char department[], int citIndex)
{
    researcher new_researcher;

    new_researcher.id = id;
    strcpy(new_researcher.name, name);
    strcpy(new_researcher.surname, surname);
    strcpy(new_researcher.department, department);
    new_researcher.citIndex = citIndex;

    rewind(filePtr);

    int line_count = 1;

    while(!feof(filePtr)){

        researcher read;
        fread(&read, sizeof(read), 1, filePtr);

        if(read.id == 0 && line_count == id){
            fseek(filePtr, -sizeof(new_researcher), SEEK_CUR);
            fwrite(&new_researcher, sizeof(new_researcher), 1, filePtr);
            return 1;
        }

        if(read.id == id){
            return 0;
        }
        line_count++;
    }
}

int removeResearcher(FILE *filePtr, unsigned int id)
{
    researcher temp = {0, "", "", "", 0};
    rewind(filePtr);

    while(!feof(filePtr)){
        researcher read;
        fread(&read, sizeof(read), 1, filePtr);

        if(read.id == id){
            fseek(filePtr, -sizeof(read), SEEK_CUR);
            fwrite(&temp, sizeof(temp), 1, filePtr);
            return 1;
        }
    }
    return 0;
}

int viewDepartmentCits(FILE *filePtr, char department[], int maxCit)
{
    rewind(filePtr);
    int count = 0;
    while(!feof(filePtr)){
        researcher read;
        fread(&read, sizeof(read), 1, filePtr);

        printf("\n%-3s %-20s %-20s %-20s %s\n",
               "ID",
               "Name",
               "Surname",
               "Department",
               "Citation Index");

        if(strcmp(read.department, department) == 0){
            if(read.citIndex <= maxCit){
                count++;


                printf("%-3d %-20s %-20s %-20s %d\n",
                       read.id,
                       read.name,
                       read.surname,
                       read.department,
                       read.citIndex);
            }
        }
    }
    return count;

}
