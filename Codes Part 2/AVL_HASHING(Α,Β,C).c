#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct ocean_data //Δομή όπου αποθηκεύουμε τα δεδομένα
{
    //κάθε γραμμής του αρχείου CSV για AVL δέντρο
    int Date[3];
    float T_degC;
    float PO4uM;
    float SiO3uM;
    float NO2uM;
    float NO3uM;
    float Salnty;
    float O2ml_L;
};

struct ocean_data_hashing //Δομή όπου αποθηκεύουμε τα δεδομένα
{
    //κάθε γραμμής του αρχείου CSV για Hashing
    char Date[10];
    float T_degC;
    float PO4uM;
    float SiO3uM;
    float NO2uM;
    float NO3uM;
    float Salnty;
    float O2ml_L;
};


struct Node_Date //AVL δέντρο βάση ημερομηνίας
{
    int date[3];
    float T_degC[2];
    struct Node_Date *left_child;
    struct Node_Date *right_child;
    int height;
};

struct Node_Temp //AVL δέντρο βάση θερμοκρασίας
{
    int date[6][3];
    float T_degC;
    struct Node_Temp *left_child;
    struct Node_Temp *right_child;
    int height;
    int counter;
};

struct node_Hashing //Κόμβος για λίστα
{

    char date[10];
    float T_degC[2];
    struct node_Hashing* next;

};

//Προτότυπο συνάρτησης όπου εξάγει τα
//δεδομένα από το αρχείο CSV για AVL
int data_extraction_AVL(struct ocean_data *Data);

//Πρότυπο συνάρτησης που εμφανίζει τα
//δεδομένα που έχουν εξαχθεί από το αρχείο CSV
void data_print(struct ocean_data *Data);

//Πρότυπο συναρτησης που διασπά την ημερομηνία
//και την τοποθετεί στην δομή ocean_data
void split(struct ocean_data *Data,char **date);

//Πρότυπο συνάρτησης που εισάγει ένα καινούργιο κόμβο
//στο δέντρο AVL βάση της ημερομηνίας
struct Node_Date* insert_AVL_date(struct Node_Date *Node,int date[3],float T_degC);

//Πρότυπο συνάρτησης που δημιουργεί ένα node βάση
//της ημερομηνίας
struct Node_Date* new_AVL_Node(int date[3],float T_degC);

//Πρότυπο συνάρτησης που υπολογίζει το
//ύψος του κόμβου
int height_AVL(struct Node_Date *node);

//Πρότυπο συνάρτησης που υπολογίζει την
//υψοζύγιση του κόμβου
int height_difference(struct Node_Date *node);

//Πρότυπο συνάρτησης  δεξιάς περιστροφής
//του κόμβου
struct Node_Date *rightRotation(struct Node_Date *node);

//Πρότυπο συνάρτησης αριστερής περιστροφής
//του κόμβου
struct Node_Date *leftRotation(struct Node_Date *node);

//Πρότυπο συνάρτησης για απεικόνιση του AVL με
//ενδο-διατεταγμένη διάσχιση
void Inorder_Print(struct Node_Date* Data_AVL);

//Πρότυπο συνάρτησης για αναζήτηση θερμοκρασίας στο
//AVL δέντρο βάση της ημερομηνίας
void Temperature_Search(struct Node_Date* Data_AVL,int search[3]);

//Πρότυπο συνάρτησης για τροποποίηση μιας
//θερμοκρασίας στο AVL δέντρο που αντιστοιχεί σε
//συγκεκριμένη ημερομηνία
void Temperature_Modify(struct Node_Date* Data_AVL,int search[3]);

//Πρότυπο συνάρτησης για διαγραφή κόμβου που αντιστοιχεί
//σε συγκεκριμένη ημερομηνία από το AVL δέντρο
struct Node_Date *delete_AVL(struct Node_Date *Data_AVL,int date[3]);

//Πρότυπο συνάρτησης που εισάγει ένα καινούργιο κόμβο
//στο δέντρο AVL
struct Node_Temp* insert_AVL_Temp(struct Node_Temp *Data_AVL,int date[3],float T_degC);

//Πρότυπο συνάρτησης που δημιουργεί ένα node
struct Node_Temp* new_AVL_Node_Temp(int date[3],float T_degC);

//Πρότυπο συνάρτησης που υπολογίζει το
//ύψος του κόμβου
int height_AVL_temp(struct Node_Temp *node);

//Πρότυπο συνάρτησης που υπολογίζει την
//υψοζύγιση του κόμβου
int height_difference_temp(struct Node_Temp *node);

//Πρότυπο συνάρτησης  δεξιάς περιστροφής
//του κόμβου
struct Node_Temp *rightRotation_temp(struct Node_Temp *node);

//Πρότυπο συνάρτησης αριστερής περιστροφής
//του κόμβου
struct Node_Temp *leftRotation_temp(struct Node_Temp *node);

//Πρότυπο συνάρτησης που βρίσκει την ημερομηνία
//με την ελάχιστη θερμοκρασία
void Find_Minimum_Temp(struct Node_Temp *Data_AVL);

//Πρότυπο συνάρτησης που βρίσκει την ημερομηνία
//με την μέγιστη θερμοκρασία
void Find_Maximum_Temp(struct Node_Temp* Data_AVL);

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
    int search[3];

    do
    {
        //Menu, ο χρήστης επιλέγει μια ενέργεια
        printf("Load File:\n");
        printf("1) AVL\n");
        printf("2) Hashing\n");
        printf("Select(Press a number 1 or 2): ");
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
    }
    while(select!=1 && select!=2);

    //Εάν ο χρήστης επέλεξε 1
    if(select==1)
    {

        //Δημιουργία πίνακα δομών με δυναμική δέσμευση μνήμης
        struct ocean_data* Data= malloc(1405*sizeof *Data);

        //Εξαγωγή των δεδομένων από το αρχείο CSV
        //και αποθήκευση στον πίνακα δομών Data
        error=data_extraction_AVL(Data);

        if(error!=1)
        {

            do
            {
                //Menu, ο χρήστης επιλέγει μια ενέργεια
                printf("Load AVL:\n");
                printf("1)Based on the date\n");
                printf("2)Based on temperature\n");
                printf("Select(Press a number 1 or 2): ");
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
            }
            while(select!=1 && select!=2);

            //Εάν ο χρήστης επέλεξε 1
            if(select==1)
            {

                struct Node_Date *Data_AVL = NULL;

                for(int i=0; i<1405; i++)
                {
                    Data_AVL=insert_AVL_date(Data_AVL,Data[i].Date,Data[i].T_degC);
                }

                do
                {
                    printf("1)Display AVL(in-order)\n");
                    printf("2)Search a Temperature\n");
                    printf("3)Modify a Temperature\n");
                    printf("4)Delete a Date\n");
                    printf("5)Exit\n");
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
                        //Εάν το δέντρο δεν είναι άδειο
                        if(Data_AVL!=NULL)
                        {
                            //Κλήση συνάρτησης για
                            //απεικόνιση του AVL
                            Inorder_Print(Data_AVL);
                        }
                        else
                        {
                            //Εάν το δέντρο είναι άδειο
                            //ενημερώνουμε τον χρήστη
                            printf("There are no data\n\n");
                        }
                        select=0;

                    }
                    //Εάν ο χρήστης επέλεξε 2,3 ή 4
                    else if(select==2 || select==3 || select==4)
                    {
                        if(Data_AVL!=NULL)
                        {
                            //Ο χρήστης εισάγει μια ημερομηνία
                            do
                            {
                                printf("Give date(12/25/2005): ");
                                scanf("%d/%d/%d",&search[0],&search[1],&search[2]);
                                fflush(stdin);
                                if(search[2]<=999||search[2]>=10000||search[0]<=0||search[0]>=13||search[1]<=0 ||search[1]>=32){
                                    printf("Invalid date\n");
                                }
                            }
                            while(search[2]<=999||search[2]>=10000||search[0]<=0||search[0]>=13||search[1]<=0 ||search[1]>=32);

                            //Αναζήτηση θερμοκρασίας βάση ημερομηνίας
                            if(select==2)
                            {
                                //Κλήση συνάρτησης
                                Temperature_Search(Data_AVL,search);

                            }
                            //Τροποποίηση μιας θερμοκρασίας που αντιστοιχεί
                            //σε συγκεκριμένη ημερομηνία
                            else if(select==3)
                            {
                                //Κλήση συνάρτησης
                                Temperature_Modify(Data_AVL,search);

                            }
                            //Διαγραφή ενός κόμβου που αντιστοιχεί σε
                            //συγκεκριμένη ημερομηνία
                            else if(select==4)
                            {
                                Data_AVL=delete_AVL(Data_AVL,search);
                            }

                            select=0;
                        }
                        else
                        {
                            //Εάν το δέντρο είναι άδειο
                            //ενημερώνουμε τον χρήστη
                            printf("There are no data\n\n");
                        }
                    }
                }
                while(select<1 || select>5);

            }
            else if(select==2)
            {

                struct Node_Temp *Data_AVL = NULL;

                //Κλήση συνάρτησης για δημιουργία του AVL
                //δέντρου εισάγωντας κάθε ημερομηνία από τον
                //πίνακα Data
                for(int i=0; i<1405; i++)
                {
                    Data_AVL=insert_AVL_Temp(Data_AVL,Data[i].Date,Data[i].T_degC);
                }

                //Menu, ο χρήστης επιλέγει μια ενέργεια
                do
                {
                    printf("1)Find a date/dates with the minimum temperatures\n");
                    printf("2)Find a date/dates with the maximum temperature\n");
                    printf("3)Exit\n");
                    printf("Select(Press a number 1-3): ");
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
                        //Κλήση συνάρτησης για εύρεση της ημερομηνίας με
                        //ελάχιστη θερμοκρασία
                        Find_Minimum_Temp(Data_AVL);
                        select=0;
                        //Εάν ο χρήστης επέλεξε 2
                    }
                    else if(select==2)
                    {
                        //Κλήση συνάρτησης για εύρεση της ημερομηνίας με
                        //μέγιστη θερμοκρασία
                        Find_Maximum_Temp(Data_AVL);
                        select=0;
                    }


                }
                while(select<1 || select>3);

            }

        }
        else if(error==1)
        {
            //Εάν δεν γίνει εξαγωγή των δεδομένων
            //τυπώνει ειδοποίηση προς τον χρήστη
            printf("File did not open\nError\n");
        }

    }
    else if(select==2)
    {

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

    }

    return 0;
}


int data_extraction_AVL(struct ocean_data *Data)
{
    //Δήλωση pointer για την επικοινωνία
    //του προγράμματος με το αρχείο
    FILE* file;

    //Άνοιγμα του αρχείου
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
    char *date[1405];

    //Αρχικοποίηση πολυδιάστατου πίνακα με
    //δυναμική δέσμευση μνήμης
    for(i=0; i<1405; i++)
    {
        date[i]=(char*)malloc(10* sizeof(char));
    }


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
                //Αποθήκευση των ημερομηνιών στον πίνακα date
                if(flag_2==0)
                {
                    for(i=0; i<10; i++)
                    {
                        date[flag_1-1][i]=info[i];
                    }
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

    //Κλήση συνάρτησης split
    split(Data,date);

    //Το αρχείο κλείνει
    fclose(file);

    return 0;
}

void split(struct ocean_data *Data,char **date)
{
    //Δηλώσεις μεταβλητλων
    int i=0,j=0,counter=0;
    char *split_1,split_2[10];

    for(j=0; j<1405; j++)
    {
        counter=0;
        for(i=0; i<10; i++)
        {
            split_2[i]=date[j][i];
        }

        //Διάσπαση ημερομηνιών σε
        //μήνα,μέρα,έτος
        split_1=strtok(split_2,"/");

        //Αποθήκευση στον πίνακα δομών
        while(split_1!=NULL)
        {
            Data[j].Date[counter]=atoi(split_1);
            counter++;
            split_1=strtok(NULL,"/");

        }
    }
}

struct Node_Date* new_AVL_Node_date(int date[3],float T_degC)
{

    //Δέσμευσεη χώρου για νέο κόμβο
    struct Node_Date* node = (struct Node_Date*)malloc(sizeof(struct Node_Date));
    int i;

    //Τοποθέτηση των τιμών στον κόμβο
    for(i=0; i<3; i++)
    {
        node->date[i]=date[i];
    }

    node->T_degC[0]=T_degC;
    node->T_degC[1]= -1;
    node->left_child=NULL;
    node->right_child=NULL;
    node->height=1;

    return node;
}

struct Node_Date* insert_AVL_date(struct Node_Date* node, int date[3],float T_degC)
{

    //Δημιουργία νέου κόμβου και τοποθέτηση στην σωστή θέση
    if(node==NULL)
    {
        return new_AVL_Node_date(date,T_degC);
    }
    else if(date[2]<node->date[2])
    {
        node->left_child=insert_AVL_date(node->left_child,date,T_degC);
    }
    else if(date[2]>node->date[2])
    {
        node->right_child=insert_AVL_date(node->right_child,date,T_degC);
    }
    else if(date[2]==node->date[2])
    {
        if(date[0]<node->date[0])
        {
            node->left_child=insert_AVL_date(node->left_child,date,T_degC);
        }
        else if(date[0]>node->date[0])
        {
            node->right_child=insert_AVL_date(node->right_child,date,T_degC);
        }
        else if(date[0]==node->date[0])
        {
            if(date[1]<node->date[1])
            {
                node->left_child=insert_AVL_date(node->left_child,date,T_degC);
            }
            else if(date[1]>node->date[1])
            {
                node->right_child=insert_AVL_date(node->right_child,date,T_degC);
            }
            else if(date[1]==node->date[1])
            {
                //Εάν υπάρχει κόμβος με την ίδια ημερομηνία
                //αποθήκευση μόνο της θερμοκρασίας
                if(T_degC!=node->T_degC[0])
                {
                    node->T_degC[1]=T_degC;
                    return node;
                }
                else
                {
                    return node;
                }
            }
        }
    }

    //Υπολογισμός του υψους του κόμβου
    if(height_AVL(node->left_child)>height_AVL(node->right_child))
    {
        node->height=1+height_AVL(node->left_child);
    }
    else
    {
        node->height=1+height_AVL(node->right_child);
    }

    //Υπολογισμός της υψοζύγισης
    int dif=height_difference(node);


    //Δεξιά περιστροφή
    if(dif>1 && date[2]<node->left_child->date[2])
    {
        return rightRotation(node);
    }
    else if(dif>1 && date[2]==node->left_child->date[2])
    {
        if(dif>1 && date[0]<node->left_child->date[0])
        {
            return rightRotation(node);
        }
        else if(dif>1 && date[0]==node->left_child->date[0])
        {
            if(dif>1 && date[1]<node->left_child->date[1])
            {
                return rightRotation(node);
            }
        }
    }

    //Αριστερή περιστροφή
    if(dif<-1 && date[2]>node->right_child->date[2])
    {
        return leftRotation(node);
    }
    else if(dif<-1 && date[2]==node->right_child->date[2])
    {
        if(dif<-1 && date[0]>node->right_child->date[0])
        {
            return leftRotation(node);
        }
        else if(dif<-1 && date[0]==node->right_child->date[0])
        {
            if(dif<-1 && date[1]>node->right_child->date[1])
            {
                return leftRotation(node);
            }
        }
    }

    //Αριστερή δεξιά περιστροφή
    if(dif>1 && date[2]>node->left_child->date[2])
    {
        node->left_child=leftRotation(node->left_child);
        return rightRotation(node);
    }
    else if(dif>1 && date[2]==node->left_child->date[2])
    {
        if(dif>1 && date[0]>node->left_child->date[0])
        {
            node->left_child=leftRotation(node->left_child);
            return rightRotation(node);
        }
        else if(dif>1 && date[0]==node->left_child->date[0])
        {
            if(dif>1 && date[1]>node->left_child->date[1])
            {
                node->left_child=leftRotation(node->left_child);
                return rightRotation(node);
            }
        }
    }

    //Δεξιά αριστερή περιστροφή
    if(dif<-1 && date[2]<node->right_child->date[2])
    {
        node->right_child=rightRotation(node->right_child);
        return leftRotation(node);
    }
    else if(dif<-1 && date[2]==node->right_child->date[2])
    {
        if(dif<-1 && date[0]<node->right_child->date[0])
        {
            node->right_child=rightRotation(node->right_child);
            return leftRotation(node);
        }
        else if(dif<-1 && date[0]==node->right_child->date[0])
        {
            if(dif<-1 && date[1]<node->right_child->date[1])
            {
                node->right_child=rightRotation(node->right_child);
                return leftRotation(node);
            }
        }
    }

    return node;
}

int height_AVL(struct Node_Date* node_AVL)
{

    int height;

    if(node_AVL==NULL)
    {
        height=0;
        return height;

    }

    //Επιστρέφει το ύωος
    //του κόμβου
    height=node_AVL->height;


    return height;
}

int height_difference(struct Node_Date *node)
{

    int dif;


    if(node==NULL)
    {
        dif=0;
        return dif;
    }
    else
    {
        //Εύρεση της υψοζύγισης
        dif=height_AVL(node->left_child)-height_AVL(node->right_child);
    }

    return dif;
}

struct Node_Date *rightRotation(struct Node_Date *node)
{
    //Δηλώσεις  μεταβλητών
    struct Node_Date *temp_1=node->left_child;
    struct Node_Date *temp_2=temp_1->right_child;

    //Δεξιά περιστροφή
    temp_1->right_child=node;
    node->left_child=temp_2;

    //Ανανέωση των υψών
    if(height_AVL(node->left_child)>height_AVL(node->right_child))
    {
        node->height=1+height_AVL(node->left_child);
    }
    else
    {
        node->height=1+height_AVL(node->right_child);
    }

    if(height_AVL(temp_1->left_child)>height_AVL(temp_1->right_child))
    {
        temp_1->height=1+height_AVL(temp_1->left_child);
    }
    else
    {
        temp_1->height=1+height_AVL(temp_1->right_child);
    }


    return temp_1;
}

struct Node_Date *leftRotation(struct Node_Date *node)
{
    //Δηλώσεις  μεταβλητών
    struct Node_Date *temp_1=node->right_child;
    struct Node_Date *temp_2=temp_1->left_child;

    //Αριστερή περιστροφή
    temp_1->left_child=node;
    node->right_child=temp_2;

    //Ανανέωση των υψών
    if(height_AVL(node->left_child)>height_AVL(node->right_child))
    {
        node->height=1+height_AVL(node->left_child);
    }
    else
    {
        node->height=1+height_AVL(node->right_child);
    }

    if(height_AVL(temp_1->left_child)>height_AVL(temp_1->right_child))
    {
        temp_1->height=1+height_AVL(temp_1->left_child);
    }
    else
    {
        temp_1->height=1+height_AVL(temp_1->right_child);
    }

    return temp_1;
}

void Inorder_Print(struct Node_Date* Data_AVL)
{

    //Εμφάνιση στου AVL δέντρου με ενδο-διατεταγμένη διάσχιση
    if(Data_AVL!=NULL)
    {
        Inorder_Print(Data_AVL->left_child);
        printf("Date: %d/%d/%d\n",Data_AVL->date[0],Data_AVL->date[1],Data_AVL->date[2]);
        printf("Temperature: %0.3f\n",Data_AVL->T_degC[0]);
        if(Data_AVL->T_degC[1]!=-1)
        {
            printf("Temperature: %0.3f\n",Data_AVL->T_degC[1]);
        }
        printf("height: %d\n",Data_AVL->height);
        printf("\n\n");

        Inorder_Print(Data_AVL->right_child);
    }
}

void Temperature_Search(struct Node_Date *Data_AVL,int search[3])
{

    //Εάν το δέντρο είναι άδειο
    //ενημερώνουμε τον χρήστη
    if(Data_AVL==NULL)
    {
        printf("Date not found\n\n");
    }

    //Αναζήτηση της ημερομηνίας στο δέντρο
    if(Data_AVL!=NULL)
    {
        //Αναζήτηση του έτους προς τα δεξιά
        if(Data_AVL->date[2]<search[2])
        {
            return Temperature_Search(Data_AVL->right_child,search);
        }
        //Αναζήτηση του μήνα προς τα δεξιά
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]<search[0])
            {
                return Temperature_Search(Data_AVL->right_child,search);
            }
            //Αναζήτηση της μέρας προς τα δεξιά
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]<search[1])
                {
                    return Temperature_Search(Data_AVL->right_child,search);
                }
            }
        }

        //Αναζήτηση του έτους προ τα αριστερά
        if(Data_AVL->date[2]>search[2])
        {
            return Temperature_Search(Data_AVL->left_child,search);
        }
        //Αναζήτηση του μήνα προς τα αριστερά
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]>search[0])
            {
                return Temperature_Search(Data_AVL->left_child,search);
            }
            //Αναζήτηση της μέρας προς τα αριστερά
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]>search[1])
                {
                    return Temperature_Search(Data_AVL->left_child,search);
                }
            }
        }

        //Εάν βρέθηκε η ημερομηνία τυπώνει την θερμοκρασία
        if(Data_AVL->date[0]==search[0] && Data_AVL->date[1]==search[1] && Data_AVL->date[2]==search[2])
        {
            printf("Date Found\n");
            printf("1.Temperature: %0.3f\n",Data_AVL->T_degC[0]);
            //Εάν υπάρχουν δύο θερμοκρασίες την ίδια μέρα
            if(Data_AVL->T_degC[1]!=-1)
            {
                printf("2.Temperature: %0.3f\n",Data_AVL->T_degC[1]);
            }
            printf("\n");
        }
    }

}

void Temperature_Modify(struct Node_Date* Data_AVL,int search[3])
{

    //Δήλωση μεταβλητών
    float temp;
    int select=1;

    //Εάν το δέντρο είναι άδειο
    //ενημερώνουμε τον χρήστη
    if(Data_AVL==NULL)
    {
        printf("Date not found\n\n");
    }

    //Αναζήτηση της ημερομηνίας στο δέντρο
    if(Data_AVL!=NULL)
    {

        //Αναζήτηση του έτους προς τα δεξιά
        if(Data_AVL->date[2]<search[2])
        {
            return Temperature_Modify(Data_AVL->right_child,search);
        }
        //Αναζήτηση του μήνα προς τα δεξιά
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]<search[0])
            {
                return Temperature_Modify(Data_AVL->right_child,search);
            }
            //Αναζήτηση της μέρας προς τα δεξιά
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]<search[1])
                {
                    return Temperature_Modify(Data_AVL->right_child,search);
                }
            }
        }

        //Αναζήτηση του έτους προ τα αριστερά
        if(Data_AVL->date[2]>search[2])
        {
            return Temperature_Modify(Data_AVL->left_child,search);
        }
        //Αναζήτηση του μήνα προς τα αριστερά
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]>search[0])
            {
                return Temperature_Modify(Data_AVL->left_child,search);
            }
            //Αναζήτηση της μέρας προς τα αριστερά
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]>search[1])
                {
                    return Temperature_Modify(Data_AVL->left_child,search);
                }
            }
        }

        //Εάν βρέθηκε η θερμοκρασία τυπώνει την θερμοκρασία
        if(Data_AVL->date[0]==search[0] && Data_AVL->date[1]==search[1] && Data_AVL->date[2]==search[2])
        {
            printf("Date found\n");
            printf("1)Temperature: %0.3f\n",Data_AVL->T_degC[0]);
            //Εάν υπάρχουν δύο θερμοκρασίες την ίδια μέρα
            //ο χρήστης διαλέγει πια θέλει να τροποποιήσει
            if(Data_AVL->T_degC[1]!=-1)
            {
                printf("2)Temperature: %0.3f\n",Data_AVL->T_degC[1]);
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

            //Εμφάνιση της νέας θερμοκρασίας
            if(select==1 && Data_AVL->T_degC[1]==-1)
            {
                Data_AVL->T_degC[0]=temp;
                printf("\nNew Temperature (%d/%d/%d)\n",search[0],search[1],search[2]);
                printf("1)Temperature: %0.3f\n",Data_AVL->T_degC[0]);
            }
            else if(select==2 && Data_AVL->T_degC[1]!=-1)
            {
                Data_AVL->T_degC[1]=temp;
                printf("\nNew Temperature (%d/%d/%d)\n",search[0],search[1],search[2]);
                printf("1)Temperature: %0.3f\n",Data_AVL->T_degC[0]);
                printf("2)Temperature: %0.3f\n",Data_AVL->T_degC[1]);
            }
            else if(select==1 && Data_AVL->T_degC[1]!=-1)
            {
                Data_AVL->T_degC[0]=temp;
                printf("\nNew Temperature (%d/%d/%d)\n",search[0],search[1],search[2]);
                printf("1)Temperature: %0.3f\n",Data_AVL->T_degC[0]);
                printf("2)Temperature: %0.3f\n",Data_AVL->T_degC[1]);
            }
            printf("\n");
        }
    }

}

struct Node_Date *delete_AVL(struct Node_Date *Data_AVL,int date[3])
{
    //Έλεγχος εάν το δέντρο είναι άδειο
    if(Data_AVL==NULL)
    {
        return Data_AVL;
    }

    //Αναζήτηση της ημερομηνίας στο δέντρο

    //Αναζήτηση του έτους προς τα δεξιά
    if(Data_AVL->date[2]<date[2])
    {
        Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
    }
    //Αναζήτηση του μήνα προς τα δεξιά
    else if(Data_AVL->date[2]==date[2])
    {
        if(Data_AVL->date[0]<date[0])
        {
            Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
        }
        //Αναζήτηση της μέρας προς τα δεξιά
        else if(Data_AVL->date[0]==date[0])
        {
            if(Data_AVL->date[1]<date[1])
            {
                Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
            }
        }
    }

    //Αναζήτηση του έτους προ τα αριστερά
    if(Data_AVL->date[2]>date[2])
    {
        Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
    }
    //Αναζήτηση του μήνα προς τα αριστερά
    else if(Data_AVL->date[2]==date[2])
    {
        if(Data_AVL->date[0]>date[0])
        {
            Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
        }
        //Αναζήτηση της μέρας προς τα αριστερά
        else if(Data_AVL->date[0]==date[0])
        {
            if(Data_AVL->date[1]>date[1])
            {
                Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
            }
        }
    }

    //Εάν βρέθηκε η ημερομηνία ο κόμβος διαγράφεται
    if(Data_AVL->date[0]==date[0] && Data_AVL->date[1]==date[1] && Data_AVL->date[2]==date[2])
    {

        //Εάν ο κόμβος έχει ένα ή κανένα παιδί
        if(Data_AVL->left_child==NULL || Data_AVL->right_child==NULL)
        {

            struct Node_Date *delete_node;

            if(Data_AVL->left_child==NULL)
            {
                delete_node=Data_AVL->right_child;
            }
            else if(Data_AVL->right_child==NULL)
            {
                delete_node=Data_AVL->left_child;
            }

            //Εάν ο κόμβος δεν έχει παιδιά
            if(delete_node==NULL)
            {
                delete_node=Data_AVL;
                Data_AVL=NULL;
                free(delete_node); //Διαγραφή κόμβου
                printf("Data deleted\n\n");
            }//Εάν ο κόμβος έχει ένα παιδί
            else
            {
                *Data_AVL=*delete_node;
                free(delete_node); //Διαγραφή κόμβου
                printf("Data deleted\n\n");
            }
        }
        //Εάν ο κόμβος έχει δύο παιδιά
        else
        {
            struct Node_Date *delete_node;

            delete_node=Data_AVL->right_child;

            while(delete_node->left_child!=NULL)
            {
                delete_node=delete_node->left_child;
            }

            Data_AVL->date[0]=delete_node->date[0];
            Data_AVL->date[1]=delete_node->date[1];
            Data_AVL->date[2]=delete_node->date[2];
            Data_AVL->T_degC[0]=delete_node->T_degC[0];
            Data_AVL->T_degC[1]=delete_node->T_degC[1];

            //Διαγραφή του κόμβου
            Data_AVL->right_child=delete_AVL(Data_AVL->right_child,delete_node->date);
        }
    }


    if(Data_AVL==NULL)
    {
        return Data_AVL;
    }


    //Υπολογισμός του υψους του κόμβου
    if(height_AVL(Data_AVL->left_child)>height_AVL(Data_AVL->right_child))
    {
        Data_AVL->height=1+height_AVL(Data_AVL->left_child);
    }
    else
    {
        Data_AVL->height=1+height_AVL(Data_AVL->right_child);
    }


    //Υπολογισμός της υψοζύγισης
    int dif=height_difference(Data_AVL);


    //Δεξιά περιστροφή
    if(dif>1 && height_difference(Data_AVL->left_child)>=0)
    {
        return rightRotation(Data_AVL);
    }

    //Αριστερή περιστροφή
    if(dif<-1 && height_difference(Data_AVL->right_child)<=0)
    {
        return leftRotation(Data_AVL);
    }

    //Αριστερή δεξιά περιστροφή
    if(dif>1 && height_difference(Data_AVL->left_child)<0)
    {
        Data_AVL->left_child=leftRotation(Data_AVL->left_child);
        return rightRotation(Data_AVL);
    }

    //Δεξιά αριστερή περιστροφή
    if(dif<-1 && height_difference(Data_AVL->right_child)>0)
    {
        Data_AVL->right_child=rightRotation(Data_AVL->right_child);
        return leftRotation(Data_AVL);
    }

    return Data_AVL;
}

struct Node_Temp* new_AVL_Node_Temp(int date[3],float T_degC)
{

    //Δέσμευσεη χώρου για νέο κόμβο
    struct Node_Temp* node = (struct Node_Temp*)malloc(sizeof(struct Node_Temp));
    int i;

    //Τοποθέτηση των τιμών στον κόμβο
    for(i=0; i<3; i++)
    {
        node->date[0][i]=date[i];
    }

    node->T_degC=T_degC;
    node->left_child=NULL;
    node->right_child=NULL;
    node->height=1;

    //Αποθηκεύει το πλήθος των ημερομηνιών
    //που περιέχει ο κόμβος
    node->counter=0;

    return node;
}

struct Node_Temp* insert_AVL_Temp(struct Node_Temp* Data_AVL, int date[3],float T_degC)
{
    //Δημιουργία νέου κόμβου
    if(Data_AVL==NULL)
    {
        return new_AVL_Node_Temp(date,T_degC);
    }

    //Τοποθέτηση του κόμβου στην σωστή θέση
    if(Data_AVL->T_degC>T_degC)
    {
        Data_AVL->left_child=insert_AVL_Temp(Data_AVL->left_child,date,T_degC);
    }
    else if(Data_AVL->T_degC<T_degC)
    {
        Data_AVL->right_child=insert_AVL_Temp(Data_AVL->right_child,date,T_degC);
    }
    else if(Data_AVL->T_degC==T_degC)
    {
        for(int i=0; i<=Data_AVL->counter; i++)
        {
            //Ημερομηνίες με ίδιες θερμοκρασίες τοποθετούνται στον ίδιο κόμβο
            if(Data_AVL->date[i][0]!=date[0] || Data_AVL->date[i][1]!=date[1] || Data_AVL->date[i][2]!=date[2])
            {
                if(i==Data_AVL->counter)
                {
                    Data_AVL->counter++;
                    Data_AVL->date[Data_AVL->counter][0]=date[0];
                    Data_AVL->date[Data_AVL->counter][1]=date[1];
                    Data_AVL->date[Data_AVL->counter][2]=date[2];
                }
            }
            else
            {
                return Data_AVL;
            }
        }
    }

    //Υπολογισμός του υψους του κόμβου
    if(height_AVL_temp(Data_AVL->left_child)>height_AVL_temp(Data_AVL->right_child))
    {
        Data_AVL->height=1+height_AVL_temp(Data_AVL->left_child);
    }
    else
    {
        Data_AVL->height=1+height_AVL_temp(Data_AVL->right_child);
    }

    //Υπολογισμός της υψοζύγισης
    int dif=height_difference_temp(Data_AVL);


    //Δεξιά περιστροφή
    if(dif>1 && Data_AVL->left_child->T_degC>T_degC)
    {
        return rightRotation_temp(Data_AVL);
    }

    //Αριστερή περιστροφή
    if(dif<-1 && Data_AVL->right_child->T_degC<T_degC)
    {
        return leftRotation_temp(Data_AVL);
    }

    //Αριστερή δεξιά περιστροφή
    if(dif>1 && Data_AVL->left_child->T_degC<T_degC)
    {
        Data_AVL->left_child=leftRotation_temp(Data_AVL->left_child);
        return rightRotation_temp(Data_AVL);
    }

    //Δεξιά αριστερή περιστροφή
    if(dif<-1 && Data_AVL->right_child->T_degC>T_degC)
    {
        Data_AVL->right_child=rightRotation_temp(Data_AVL->right_child);
        return leftRotation_temp(Data_AVL);
    }

    return Data_AVL;
}

int height_AVL_temp(struct Node_Temp* node_AVL)
{

    int height;

    if(node_AVL==NULL)
    {
        height=0;
        return height;

    }

    //Επιστρέφει το ύωος
    //του κόμβου
    height=node_AVL->height;


    return height;
}

int height_difference_temp(struct Node_Temp *node)
{

    int dif;

    if(node==NULL)
    {
        dif=0;
        return dif;
    }
    else
    {
        //Εύρεση της υψοζύγισης
        dif=height_AVL_temp(node->left_child)-height_AVL_temp(node->right_child);
    }

    return dif;
}

struct Node_Temp *rightRotation_temp(struct Node_Temp *node)
{

    //Δηλώσεις  μεταβλητών
    struct Node_Temp *temp_1=node->left_child;
    struct Node_Temp *temp_2=temp_1->right_child;

    //Δεξιά περιστροφή
    temp_1->right_child=node;
    node->left_child=temp_2;


    //Ανανέωση των υψών
    if(height_AVL_temp(node->left_child)>height_AVL_temp(node->right_child))
    {
        node->height=1+height_AVL_temp(node->left_child);
    }
    else
    {
        node->height=1+height_AVL_temp(node->right_child);
    }

    if(height_AVL_temp(temp_1->left_child)>height_AVL_temp(temp_1->right_child))
    {
        temp_1->height=1+height_AVL_temp(temp_1->left_child);
    }
    else
    {
        temp_1->height=1+height_AVL_temp(temp_1->right_child);
    }

    return temp_1;
}

struct Node_Temp *leftRotation_temp(struct Node_Temp *node)
{
    //Δηλώσεις  μεταβλητών
    struct Node_Temp *temp_1=node->right_child;
    struct Node_Temp *temp_2=temp_1->left_child;

    //Αριστερή περιστροφή
    temp_1->left_child=node;
    node->right_child=temp_2;

    //Ανανέωση των υψών
    if(height_AVL_temp(node->left_child)>height_AVL_temp(node->right_child))
    {
        node->height=1+height_AVL_temp(node->left_child);
    }
    else
    {
        node->height=1+height_AVL_temp(node->right_child);
    }

    if(height_AVL_temp(temp_1->left_child)>height_AVL_temp(temp_1->right_child))
    {
        temp_1->height=1+height_AVL_temp(temp_1->left_child);
    }
    else
    {
        temp_1->height=1+height_AVL_temp(temp_1->right_child);
    }

    return temp_1;
}

void Find_Minimum_Temp(struct Node_Temp *Data_AVL)
{

    //Εύρεση του κόμβου που βρίσκεται
    //πιο αριστερα
    while(Data_AVL->left_child!=NULL)
    {
        Data_AVL=Data_AVL->left_child;
    }

    //Εμφάνιση της ελάχιστης θερμοκρασίας και της ημερομηνίας
    printf("The minimum temperature is: %0.3f\n",Data_AVL->T_degC);
    if(Data_AVL->counter==0)
    {
        printf("The date with this temperature is: %d/%d/%d\n",Data_AVL->date[0][0],Data_AVL->date[0][1],Data_AVL->date[0][2]);
    }
    else
    {
        printf("The dates with this temperatures are:\n");
        for(int i=0; i<=Data_AVL->counter; i++)
        {
            printf("%d) %d/%d/%d\n",i+1,Data_AVL->date[i][0],Data_AVL->date[i][1],Data_AVL->date[i][2]);
        }
    }
    printf("\n");

}

void Find_Maximum_Temp(struct Node_Temp *Data_AVL)
{

    //Εύρεση τοy κόμβου που βρίσκεται
    //πιο δεξιά
    while(Data_AVL->right_child!=NULL)
    {
        Data_AVL=Data_AVL->right_child;
    }

    //Εμφάνιση της μέγιστης θερμοκρασίας και της ημερομηνίας
    printf("The maximum temperature is: %0.3f\n",Data_AVL->T_degC);
    if(Data_AVL->counter==0)
    {
        printf("The date with this temperature is: %d/%d/%d\n",Data_AVL->date[0][0],Data_AVL->date[0][1],Data_AVL->date[0][2]);
    }
    else
    {
        printf("The dates with this temperatures are:\n");
        for(int i=0; i<=Data_AVL->counter; i++)
        {
            printf("%d) %d/%d/%d\n",i+1,Data_AVL->date[i][0],Data_AVL->date[i][1],Data_AVL->date[i][2]);
        }
    }
    printf("\n");
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
