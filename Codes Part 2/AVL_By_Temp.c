#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct ocean_data //Δομή όπου αποθηκεύουμε τα δεδομένα
{                 //κάθε γραμμής του αρχείου CSV
    int Date[3];
    float T_degC;
    float PO4uM;
    float SiO3uM;
    float NO2uM;
    float NO3uM;
    float Salnty;
    float O2ml_L;
};

struct Node_Temp //AVL tree node
{
    int date[6][3];
    float T_degC;
    struct Node_Temp *left_child;
    struct Node_Temp *right_child;
    int height;
    int counter;

};

//Προτότυπο συνάρτησης όπου εξάγει τα
//δεδομένα από το αρχείο CSV
int data_extraction(struct ocean_data *Data);

//Πρότυπο συνάρτησης που εμφανίζει τα
//δεδομένα που έχουν εξαχθεί από το αρχείο CSV
void data_print(struct ocean_data *Data);

//Πρότυπο συναρτησης που διασπά την ημερομηνία
//και την τοποθετεί στην δομή ocean_data
void split(struct ocean_data *Data,char **date);

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

int main()
{
    //Δηλώσεις μεταβλητών
    int error=0,select=0;
    char check;

    //Δημιουργία πίνακα δομών με δυναμική δέσμευση μνήμης
    struct ocean_data* Data= malloc(1405*sizeof *Data);

    //Εξαγωγή των δεδομένων από το αρχείο CSV
    //και αποθήκευση στον πίνακα δομών Data
    error=data_extraction(Data);

    if(error!=1)
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
            if(select==1){
                //Κλήση συνάρτησης για εύρεση της ημερομηνίας με
                //ελάχιστη θερμοκρασία
                Find_Minimum_Temp(Data_AVL);
                select=0;
            //Εάν ο χρήστης επέλεξε 2
            }else if(select==2){
                //Κλήση συνάρτησης για εύρεση της ημερομηνίας με
                //μέγιστη θερμοκρασία
                Find_Maximum_Temp(Data_AVL);
                select=0;
            }


        }
        while(select<1 || select>3);

    }
    else if(error==1)
    {
        //Εάν το δέντρο είναι άδειο
        //ενημερώνουμε τον χρήστη
        printf("File did not open\nError\n");
    }

    return 0;
}

int data_extraction(struct ocean_data *Data)
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

void data_print(struct ocean_data *Data)
{
    int i,j;

    //Εκτύπωση των περιεχομένων του πίνακα δομών
    for(i=0; i<1405; i++)
    {
        printf("%d) ",i);
        for(j=0; j<3; j++)
        {
            if(j<2)
            {
                printf("%d/",Data[i].Date[j]);
            }
            else
            {
                printf("%d,",Data[i].Date[j]);
            }
        }
        printf(" %0.3f, %0.3f, ",Data[i].T_degC,Data[i].PO4uM);
        printf("%0.3f, %0.3f, ",Data[i].SiO3uM,Data[i].NO2uM);
        printf("%0.3f, %0.3f, ",Data[i].NO3uM,Data[i].Salnty);
        printf("%0.3f\n",Data[i].O2ml_L);
    }
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
    if(Data_AVL==NULL){
        return new_AVL_Node_Temp(date,T_degC);
    }

    //Τοποθέτηση του κόμβου στην σωστή θέση
    if(Data_AVL->T_degC>T_degC){
        Data_AVL->left_child=insert_AVL_Temp(Data_AVL->left_child,date,T_degC);
    }
    else if(Data_AVL->T_degC<T_degC){
        Data_AVL->right_child=insert_AVL_Temp(Data_AVL->right_child,date,T_degC);
    }else if(Data_AVL->T_degC==T_degC){
        for(int i=0;i<=Data_AVL->counter;i++){
                //Ημερομηνίες με ίδιες θερμοκρασίες τοποθετούνται στον ίδιο κόμβο
                if(Data_AVL->date[i][0]!=date[0] || Data_AVL->date[i][1]!=date[1] || Data_AVL->date[i][2]!=date[2]){
                    if(i==Data_AVL->counter){
                        Data_AVL->counter++;
                        Data_AVL->date[Data_AVL->counter][0]=date[0];
                        Data_AVL->date[Data_AVL->counter][1]=date[1];
                        Data_AVL->date[Data_AVL->counter][2]=date[2];
                    }
                }else {
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
     if(dif>1 && Data_AVL->left_child->T_degC>T_degC){
            return rightRotation_temp(Data_AVL);
     }

     //Αριστερή περιστροφή
     if(dif<-1 && Data_AVL->right_child->T_degC<T_degC){
            return leftRotation_temp(Data_AVL);
     }

     //Αριστερή δεξιά περιστροφή
     if(dif>1 && Data_AVL->left_child->T_degC<T_degC){
        Data_AVL->left_child=leftRotation_temp(Data_AVL->left_child);
        return rightRotation_temp(Data_AVL);
     }

     //Δεξιά αριστερή περιστροφή
     if(dif<-1 && Data_AVL->right_child->T_degC>T_degC){
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

void Find_Minimum_Temp(struct Node_Temp *Data_AVL){

    //Εύρεση του κόμβου που βρίσκεται
    //πιο αριστερα
    while(Data_AVL->left_child!=NULL){
        Data_AVL=Data_AVL->left_child;
    }

    //Εμφάνιση της ελάχιστης θερμοκρασίας και της ημερομηνίας
    printf("The minimum temperature is: %0.3f\n",Data_AVL->T_degC);
    if(Data_AVL->counter==0){
        printf("The date with this temperature is: %d/%d/%d\n",Data_AVL->date[0][0],Data_AVL->date[0][1],Data_AVL->date[0][2]);
    }else{
        printf("The dates with this temperatures are:\n");
        for(int i=0;i<=Data_AVL->counter;i++){
            printf("%d) %d/%d/%d\n",i+1,Data_AVL->date[i][0],Data_AVL->date[i][1],Data_AVL->date[i][2]);
        }
    }
    printf("\n");

}

void Find_Maximum_Temp(struct Node_Temp *Data_AVL){

    //Εύρεση τοy κόμβου που βρίσκεται
    //πιο δεξιά
    while(Data_AVL->right_child!=NULL){
        Data_AVL=Data_AVL->right_child;
    }

    //Εμφάνιση της μέγιστης θερμοκρασίας και της ημερομηνίας
    printf("The maximum temperature is: %0.3f\n",Data_AVL->T_degC);
    if(Data_AVL->counter==0){
        printf("The date with this temperature is: %d/%d/%d\n",Data_AVL->date[0][0],Data_AVL->date[0][1],Data_AVL->date[0][2]);
    }else{
        printf("The dates with this temperatures are:\n");
        for(int i=0;i<=Data_AVL->counter;i++){
            printf("%d) %d/%d/%d\n",i+1,Data_AVL->date[i][0],Data_AVL->date[i][1],Data_AVL->date[i][2]);
        }
    }
    printf("\n");
}



