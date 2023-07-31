#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

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

//Προτότυπο συνάρτησης όπου εξάγει τα
//δεδομένα από το αρχείο CSV
int data_extraction(struct ocean_data *Data);

//Πρότυπο συνάρτησης που εμφανίζει τα
//δεδομένα που έχουν εξαχθεί από το αρχείο CSV
void data_print(struct ocean_data *Data);

//Πρότυπο συναρτησης που διασπά την ημερομηνία
//και την τοποθετεί στην δομή ocean_data
void split(struct ocean_data *Data,char **date);

//Πρότυπο συνάρτησης του Insertion Sort αλγορίθμου
int Insertion_Sort(struct ocean_data *Data);

//Πρότυπο συνάρτησης του QuickSort αλγορίθμου
int Quicksort(struct ocean_data *Data,int l,int r);

int main()
{

    int error=0,i; //Δηλώσεις μεταβλητών

    //Δηλώσεις μεταβλητών για τις πειραματικές μετρήσεις
    int operation_Insertion,operation_QuickSort;
    struct timespec start_1,end_1;
    struct timespec start_2,end_2;

    //Δημιουργία πίνακα δομών με δυναμική δέσμευση μνήμης
    struct ocean_data* Data_1 = malloc(1405*sizeof *Data_1);
    struct ocean_data* Data_2 = malloc(1405*sizeof *Data_2);

    //Εξαγωγή των δεδομένων από το αρχείο CSV
    //και αποθήκευση στον πίνακα δομών Data_1
    error=data_extraction(Data_1);


    if(error!=1) //Έλεγχος για το αν προκύψει σφάλμα στην εξαγωγή
    {
        //Αντιγραφή τον δεδομένων από το Data_1 στο Data_2
        for(i=0; i<1405; i++)
        {
            Data_2[i]=Data_1[i];
        }

        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου QuickSort
        clock_gettime(CLOCK_MONOTONIC, &start_1);
        operation_QuickSort=Quicksort(Data_2,0,1404);
        clock_gettime(CLOCK_MONOTONIC, &end_1);


        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου Insertion Sort
        clock_gettime(CLOCK_MONOTONIC, &start_2);
        operation_Insertion=Insertion_Sort(Data_1);
        clock_gettime(CLOCK_MONOTONIC, &end_2);

        //Κλήση συνάρτησης για την εμφάνιση του
        //πίνακα δομών Data_1(Insertion Sort)
        printf("Insertion Sort:\n");
        data_print(Data_1);

        //Κλήση συνάρτησης για την προβολή του πίνακα δομών Data_2(QuickSort)
        printf("\n\n\nQuickSort:\n");
        data_print(Data_2);

        //Υπολογισμός των πειραματικών μετρήσεων
        double time_Insertion=(end_2.tv_sec-start_2.tv_sec)*1e9;
        time_Insertion=(time_Insertion+(end_2.tv_nsec-start_2.tv_nsec))*1e-9;

        double time_QuickSort=(end_1.tv_sec-start_1.tv_sec)*1e9;
        time_QuickSort=(time_QuickSort+(end_1.tv_nsec-start_1.tv_nsec))*1e-9;

        //Τύπωση των αποτελεσμάτων των πειραματικών μετρήσεων
        printf("\n\n\nExecution time for Insertion Sort is: %f sec\n",time_Insertion);
        printf("Operations: %d\n",operation_Insertion);
        printf("Execution time for QuickSort is: %f sec\n",time_QuickSort);
        printf("Operations: %d\n",operation_QuickSort);

    }
    else if(error==1)
    {
        //Εάν δεν γίνει εξαγωγή των δεδομένων
        //τυπώνει ειδοποίηση προς τον χρήστη
        printf("File did not open\nError");
    }

    return 0;
}


int data_extraction(struct ocean_data *Data)
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
            //Η εξαγωγή δεδομένων ξεκινάει από την
            //δεύτερη γραμμή του αρχείου
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
        printf("%d) ",i+1);
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
        printf(" (%0.3f), %0.3f, ",Data[i].T_degC,Data[i].PO4uM);
        printf("%0.3f, %0.3f, ",Data[i].SiO3uM,Data[i].NO2uM);
        printf("%0.3f, %0.3f, ",Data[i].NO3uM,Data[i].Salnty);
        printf("%0.3f\n",Data[i].O2ml_L);


    }
}
int Insertion_Sort(struct ocean_data *Data)
{
    //Δηλώσεις μεταβλητών
    int counter=0;
    int i,j;
    struct ocean_data temp;

    for(j=1; j<1405; j++)
    {
        temp=Data[j];
        //Εισαγωγή του στοιχείου Data[j] στην κατάλληλη
        //θέση του ταξινομημένου υποπίνακα Data[0..j]
        i=j-1;

        //Μετακίνηση όλων των Data[i].T_degC>temp
        //κατά μια θέση δεξιά
        while(i>=0 && temp.T_degC<Data[i].T_degC)
        {
            Data[i+1]=Data[i];
            i--;
            counter++;
        }
        Data[i+1]=temp;
    }

    return counter;
}

int Quicksort(struct ocean_data *Data,int l,int r)
{
    //Δηλώσεις μεταβλητών
    int i,k;
    int counter=0;
    struct ocean_data temp;
    struct ocean_data swap;

    i=l;
    k=r+1;
    //Επιλογή του αριστερότερου στοιχείου
    //για στοιχείο διαχωρισμού
    temp=Data[l];

    //Διαίρει
    while(i<k)
    {
        //Διαπέραση πίνακα αριστερά προς τα
        //δεξιά μέχρι να βρεθεί τιμή μεγαλύτερη
        //του στοιχείου διαχωρισμού
        do
        {
            i++;
            counter++;
        }
        while(Data[i].T_degC<temp.T_degC);

        //Διαπέραση πίνακα δεξιά προς τα
        //αριστερα μέχρι να βρεθεί τιμή μικρότερη
        //του στοιχείου διαχωρισμού
        do
        {
            k--;
            counter++;
        }
        while(Data[k].T_degC>temp.T_degC);

        //Εναλλαγή Data[i] και
        //Data[k]
        if(k>i)
        {
            swap=Data[k];
            Data[k]=Data[i];
            Data[i]=swap;
        }

    }

    //Εναλλαγή Data[k] και
    //Data[k]
    swap=Data[l];
    Data[l]=Data[k];
    Data[k]=swap;

    //Βασίλευε
    //Αναδρομική κλήση του QuickSort
    if(l<k-1)
    {
        counter=counter+Quicksort(Data,l,k-1);
    }
    if(k+1<r)
    {
        counter=counter+Quicksort(Data,k+1,r);
    }

    return counter;
}
