#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct ocean_data_hashing //Δομή όπου αποθηκεύουμε τα δεδομένα
{
    //κάθε γραμμής του αρχείου CSV
    char Date[10];
    float T_degC;
    float PO4uM;
    float SiO3uM;
    float NO2uM;
    float NO3uM;
    float Salnty;
    float O2ml_L;
};

struct node_Hashing //AVL tree node
{

    char date[10];
    float T_degC[2];
    struct node_Hashing* next;

};

//Προτότυπο συνάρτησης όπου εξάγει τα
//δεδομένα από το αρχείο CSV
int data_extraction_hashing(struct ocean_data_hashing *Data);

//Πρότυπο συνάρτησης που εμφανίζει τα
//δεδομένα που έχουν εξαχθεί από το αρχείο CSV
void data_print_hashing(struct ocean_data_hashing *Data);

//Πρότυπο συνάρτησης που εισάγει ένα καινούργιο στοιχείο
void insert_Hash(char date[10],float T_degC,struct node_Hashing T[11]);

//Πρότυπο συνάρτησης αναζήτησης θερμοκρασίας
//βάση της ημερομηνίας
void search_Temp(struct node_Hashing T[11],char date[10]);

//Πρότυπο συνάρτησης τροποποίησης μιας θερμοκρασίας
//βάση ημερομηνίας
void modify_Temp(struct node_Hashing T[11],char date[10]);

//Διαγραφή μιας εγγραφής από τον πίνακα κατακερματισμού
//βάση μιας ημερομηνίας
void delete_Date(struct node_Hashing T[11],char date[10]);

int main()
{
    //Δηλώσεις μεταβλητών
    int error=0,select=0;
    char check;
    char date[10];

    //Αρχικοποίηση πίνακα δομών με δυναμική δέσμευση μνήμης
    struct ocean_data_hashing* Data= malloc(1405*sizeof *Data);

    //Εξαγωγή των δεδομένων από το αρχείο CSV
    //και αποθήκευση στον πίνακα δομών Data
    error=data_extraction_hashing(Data);


    if(error!=1)
    {

        //Δημιουργία του πίνακα κατακερματισμού
        struct node_Hashing *T=(struct node_Hashing*)malloc(11*sizeof(struct node_Hashing));

        //Αρχικοποίηση του πίνακα κατακερματισμού
        for(int i=0; i<11; i++)
        {
            T[i].next=NULL;
            for(int j=0; j<10; j++)
            {
                T[i].date[j]='0';
            }
            T[i].T_degC[0]=0;
            T[i].T_degC[1]=0;
        }

        int i;

        //Κλήση συνάρτησης για εισαγωγή των στοιχείων
        //στον πίνακα κατακερματισμού με αλυσίδες
        for(i=0; i<1405; i++)
        {
            insert_Hash(Data[i].Date,Data[i].T_degC,T);
        }

        //Menu, ο χρήστης επιλέγει μια ενέργεια
        do
        {
            printf("1)Search a Temperature\n");
            printf("2)Modify a Temperature\n");
            printf("3)Delete a Date\n");
            printf("4)Exit\n");
            printf("Select(Press a number 1-5): ");
            scanf("%c",&check);
            printf("\n\n");

            //Έλεγχος για το αν τοποθέτησε
            //γράμμα ή αριθμό
            if(isalpha(check))
            {
                select=0;
                fflush(stdin);
            }
            else
            {

                select=check-'0';
                fflush(stdin);
            }

            //Εάν ο χρήστης επέλεξε 1
            if(select==1)
            {
                //Ο χρήστης εισάγει μια ημερομηνία
                do
                {
                    printf("Give date(12/25/2005): ");
                    scanf("%s",date);
                    fflush(stdin);
                    if(strlen(date)!=10)
                    {
                        printf("\nThe input must be: xx/xx/xxxx\n");
                    }
                }
                while(strlen(date)!=10);

                //Κλήση συνάρτησης θα εύρεση θερμοκρασίας βάση
                //ημερομηνίας
                search_Temp(T,date);
                select=0;
                //Εάν ο χρήστης επέλεξε 2
            }
            else if(select==2)
            {
                //Ο χρήστης εισάγει μια ημερομηνία
                do
                {
                    printf("Give date(12/25/2005): ");
                    scanf("%s",date);
                    fflush(stdin);
                    if(strlen(date)!=10)
                    {
                        printf("\nThe input must be: xx/xx/xxxx\n");
                    }
                }
                while(strlen(date)!=10);
                //Κλήση συνάρτησης για τροποποίηση μιας θερμοκρασίας
                //βάση ημερομηνίας
                modify_Temp(T,date);
                select=0;
                //Εάν ο χρήστης επέλεξε 3
            }
            else if(select==3)
            {

                //Ο χρήστης εισάγει μια ημερομηνία
                do
                {
                    printf("Give date(12/25/2005): ");
                    scanf("%s",date);
                    fflush(stdin);
                    if(strlen(date)!=10)
                    {
                        printf("\nThe input must be: xx/xx/xxxx\n");
                    }
                }
                while(strlen(date)!=10);
                //Κλήση συνάρτησης για διαγραφή μιας
                //εγγραφής από τον πίνακα κατακερματισμού
                delete_Date(T,date);
                select=0;
            }

        }
        while(select<1 || select>4);

    }
    else if(error==1)
    {
        //Εάν δεν γίνει εξαγωγή των δεδομένων
        //τυπώνει ειδοποίηση προς τον χρήστη
        printf("File did not open\nError\n");
    }

    return 0;

}


int data_extraction_hashing(struct ocean_data_hashing *Data)
{
    //Δήλωση pointer για την επικοινωνία
    //του προγράμματος με το αρχείο
    FILE* file;

    //’νοιγμα του αρχείου
    file=fopen("ocean.csv","r");

    //Έλεγχος για το αν το αρχείο είναι άδειο
    if(file==NULL)
    {
        return 1;
    }

    //Δηλώσεις μεταβλητών
    char buffer[128];
    int flag_1=-1,flag_2=0;
    char *info;
    int i=0;


    while(fgets(buffer,128,file))
    {
        flag_1++;
        flag_2=0;

        //Διάσπαση των δεδομένων με την χρήση του ','
        info = strtok(buffer,",");

        while(info)
        {
            if(flag_1!=0)
            {
                //Αποθήκευση των ημερομηνιών στον πίνακα δομών
                if(flag_2==0)
                {
                    strcpy(Data[flag_1-1].Date,info);
                }
                else
                {
                    //Αποθήκευση θερμοκρασιών στον πίνακα δομών
                    if(flag_2==1)
                    {
                        Data[flag_1-1].T_degC=atof(info);
                    }
                    //Αποθήκευση Phosphate στον πίνακα δομών
                    else if(flag_2==2)
                    {
                        Data[flag_1-1].PO4uM=atof(info);
                    }
                    //Αποθήκευση Silicate στον πίνακα δομών
                    else if(flag_2==3)
                    {
                        Data[flag_1-1].SiO3uM=atof(info);
                    }
                    //Αποθήκευση Nitrite στον πίνακα δομών
                    else if(flag_2==4)
                    {
                        Data[flag_1-1].NO2uM=atof(info);
                    }
                    //Αποθήκευση Nitrate στον πίνακα δομών
                    else if(flag_2==5)
                    {
                        Data[flag_1-1].NO3uM=atof(info);
                    }
                    //Αποθήκευση Salinity στον πίνακα δομών
                    else if(flag_2==6)
                    {
                        Data[flag_1-1].Salnty=atof(info);
                    }
                    //Αποθήκευση Oxygen στον πίνακα δομών
                    else if(flag_2==7)
                    {
                        Data[flag_1-1].O2ml_L=atof(info);

                    }
                }
            }

            info = strtok(NULL,",");
            flag_2++;

        }
    }

    //Το αρχείο κλείνει
    fclose(file);

    return 0;
}

void data_print_hashing(struct ocean_data_hashing *Data)
{
    int i,j;

    //Εκτύπωση των περιεχομένων του πίνακα δομών
    for(i=0; i<1405; i++)
    {
        printf("%d) ",i+1);
        printf("%s",Data[i].Date);
        printf(" %0.3f, %0.3f, ",Data[i].T_degC,Data[i].PO4uM);
        printf("%0.3f, %0.3f, ",Data[i].SiO3uM,Data[i].NO2uM);
        printf("%0.3f, %0.3f, ",Data[i].NO3uM,Data[i].Salnty);
        printf("%0.3f\n",Data[i].O2ml_L);
    }
}

void insert_Hash(char date[10],float T_degC,struct node_Hashing T[11])
{
    //Δήλωση μεταβλητών
    int i;
    int hash_key=0;
    int flag=0;
    int flag_1=0;

    //Υπολογισμός της συνάρτησης
    //κατακερματισμού
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //Εύρεση της θέσης στον πίνακα
    //κατακερματισμού
    hash_key=hash_key%11;

    //Δημιουργία νέου στοιχείου
    struct node_Hashing *new_data=(struct node_Hashing*)malloc(sizeof(struct node_Hashing));

    //Εάν ο πίνακας κατακερματισμού στην θέση που έδειξε
    //η συνάρτηση κατακερματισμού δεν δείχνει σε κάποιο στοιχείο
    if(T[hash_key].next==NULL)
    {
        strcpy(new_data->date,date);
        new_data->T_degC[0]=T_degC;
        new_data->T_degC[1]=0;
        new_data->next=NULL;
        T[hash_key].next=new_data;

    }
    //Αλλιώς τοποθέτηση του στοιχείου στο τέλος της λίστας
    else
    {
        struct node_Hashing *check;
        check=T[hash_key].next;

        while(check!=NULL && flag==0 && flag_1==0)
        {
            //Εάν υπάρχει η ημερομηνία τοποθέτηση
            //μόνο της θερμοκρασίας στο στοιχείο
            if(strcmp(check->date,date)==0)
            {
                check->T_degC[1]=T_degC;
                flag=1;
            }
            //Αλλιώς τοποθέτηση του στοιχείου
            //στο τέλος της λίστας
            else if(check->next!=NULL)
            {
                check=check->next;
            }
            else
            {
                flag_1=1;
            }
        }

        //Τοποθέτηση των δεδομένων στο νέο
        //στοιχείο της λίστας
        if(flag==0)
        {
            strcpy(new_data->date,date);
            new_data->T_degC[0]=T_degC;
            new_data->T_degC[1]=0;
            new_data->next=NULL;
            check->next=new_data;
        }
    }

}

void search_Temp(struct node_Hashing T[11],char date[10])
{

    //Δηώσεις μεταβλητών
    int i,flag=0;
    int hash_key=0;

    //Υπολογισμός της συνάρτησης
    //κατακερματισμού
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //Εύρεση της θέσης στον πίνακα
    //κατακερματισμού
    hash_key=hash_key%11;

    //Δημιουργία νέου στοιχείου και
    //αναζήτηση της λίστας που δείχνει η
    //συνάρτηση κατακερματισμού
    struct node_Hashing *search=T[hash_key].next;

    //Εύρεση της ημερομινίας
    while(search!=NULL && flag==0)
    {
        //Εμφάνιση της θερμοκρασίας
        if(strcmp(search->date,date)==0)
        {
            printf("Date Found: %s\n",search->date);
            printf("1) Temperature: %0.3f",search->T_degC[0]);
            if(search->T_degC[1]!=0)
            {
                printf("\n2) Temperature: %0.3f",search->T_degC[1]);
            }
            printf("\n\n");
            flag=1;
        }
        search=search->next;
    }

    //Εάν δεν βρεθέι η ημερομηνία
    //ειδοποιείται ο χρήστης
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}

void modify_Temp(struct node_Hashing T[11],char date[10])
{

    //Δηλώσεις μεταβλητών
    int i,flag=0;
    int hash_key=0;
    int select=1;
    float temp;

    //Υπολογισμός της συνάρτησης
    //κατακερματισμού
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //Εύρεση της θέσης στον πίνακα
    //κατακερματισμού
    hash_key=hash_key%11;

    //Δημιουργία νέου στοιχείου και
    //αναζήτηση της λίστας που δείχνει η
    //συνάρτηση κατακερματισμού
    struct node_Hashing *search=T[hash_key].next;

    //Εύρεση της ημερομινίας
    while(search!=NULL && flag==0)
    {
        //Εμφάνιση της θερμοκρασίας
        if(strcmp(search->date,date)==0)
        {
            printf("Date Found: %s\n",search->date);
            printf("1) Temperature: %0.3f\n",search->T_degC[0]);
            //Εάν υπάρχουν δύο θερμοκρασίες σε μια ημερομηνία
            //ο χρήστης επιλέγει ποια από τις δύο θα αλλάξει
            if(search->T_degC[1]!=0)
            {
                printf("2) Temperature: %0.3f\n",search->T_degC[1]);
                do
                {
                    printf("Which on do you want to change (1 or 2):");
                    scanf("%d",&select);
                    fflush(stdin);
                }
                while(select<1 || select>2);
            }
            //Ο χρήστης εισάγει την νέα θερμοκρασία
            printf("Enter a new value:");
            scanf("%f",&temp);
            fflush(stdin);

            //Εμφάνιση της αλλαγής στην θερμοκρασία
            if(select==1 && search->T_degC[1]==0)
            {
                search->T_degC[0]=temp;
                printf("\nNew Temperature (%s)\n",search->date);
                printf("1) Temperature: %0.3f\n",search->T_degC[0]);
            }
            else if(select==2 && search->T_degC[1]!=0)
            {
                search->T_degC[1]=temp;
                printf("\nNew Temperature (%s)\n",search->date);
                printf("1) Temperature: %0.3f\n",search->T_degC[0]);
                printf("2) Temperature: %0.3f\n",search->T_degC[1]);
            }
            else if(select==1 && search->T_degC[1]!=0)
            {
                search->T_degC[0]=temp;
                printf("\nNew Temperature (%s)\n",search->date);
                printf("1)Temperature: %0.3f\n",search->T_degC[0]);
                printf("2)Temperature: %0.3f\n",search->T_degC[1]);
            }

            printf("\n");
            flag=1;
        }
        search=search->next;
    }

    //Ενημέρωση του χρήστη εάν δεν βρέθηκε
    //η ημερομηνία
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}

void delete_Date(struct node_Hashing T[11],char date[10])
{

    //Δήλωση μεταβλητών
    int i,flag=0;
    int hash_key=0;

    //Υπολογισμός της συνάρτησης
    //κατακερματισμού
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //Εύρεση της θέσης στον πίνακα
    //κατακερματισμού
    hash_key=hash_key%11;


    //Δημιουργία νέου στοιχείου και
    //αναζήτηση της λίστας που δείχνει η
    //συνάρτηση κατακερματισμού
    struct node_Hashing *search=T[hash_key].next;

    //Δημιουργία νέου στοιχείου
    struct node_Hashing *temp;

    //Εύρεση της ημερομινίας
    while(search!=NULL && flag==0)
    {
        //Διαγραφή του στοιχείου από την λίστα
        if(search->next!=NULL)
        {
            if(strcmp(search->next->date,date)==0)
            {
                temp=search->next;
                search->next=temp->next;
                free(temp);
                flag=1;
                printf("Date deleted\n\n");
            }
        }

        search=search->next;

    }

    //Εάν δεν βρεθέι η ημερομηνία
    //ειδοποιείται ο χρήστης
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}
