#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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

//Πρότυπο συνάρτησης του Heap Sort αλγορίθμου
int HeapSort(struct ocean_data *Data);

//Πρότυπο  συνάρτησης του Counting Sort αλγορίθμου
int CountingSort(struct ocean_data *Data);

int main()
{

    int error=0,i; //Δηλώσεις μεταβλητών

    //Δηλώσεις μεταβλητών για τις πειραματικές μετρήσεις
    int operation_HeapSort,operation_CountingSort;
    struct timespec start_1,end_1;
    struct timespec start_2,end_2;

    //Δημιουργία πίνακα δομών με δυναμική δέσμευση μνήμης
    struct ocean_data* Data_2= malloc(1405*sizeof *Data_2);
    struct ocean_data* Data_1= malloc(1405*sizeof *Data_1);

    //Εξαγωγή των δεδομένων από το αρχείο CSV
    //και αποθήκευση στον πίνακα δομών Data_1 και Data_2
    error=data_extraction(Data_1);
    error=data_extraction(Data_2);

    if(error!=1)
    {

        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου Heap Sort
        clock_gettime(CLOCK_MONOTONIC, &start_1);
        operation_HeapSort=HeapSort(Data_1);
        clock_gettime(CLOCK_MONOTONIC, &end_1);

        //Κλήση συνάρτησης για την εμφάνιση του
        //πίνακα δομών Data_1(Heap Sort)
        printf("Heap Sort:\n");
        data_print(Data_1);

        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου Counting Sort
        clock_gettime(CLOCK_MONOTONIC, &start_2);
        operation_CountingSort=CountingSort(Data_2);
        clock_gettime(CLOCK_MONOTONIC, &end_2);

        //Κλήση συνάρτησης για την εμφάνιση του
        //πίνακα δομών Data_2(Counting Sort)
        printf("\n\n\nCounting Sort:\n");
        data_print(Data_2);

        //Υπολογισμός των πειραματικών μετρήσεων
        double time_HeapSort=(end_1.tv_sec-start_1.tv_sec)*1e9;
        time_HeapSort=(time_HeapSort+(end_1.tv_nsec-start_1.tv_nsec))*1e-9;

        double time_CountingSort=(end_2.tv_sec-start_2.tv_sec)*1e9;
        time_CountingSort=(time_CountingSort+(end_2.tv_nsec-start_2.tv_nsec))*1e-9;

        //Τύπωση των αποτελεσμάτων των πειραματικών μετρήσεων
        printf("\n\n\nExecution time for Heap Sort is: %f sec\n",time_HeapSort);
        printf("Operations: %d\n",operation_HeapSort);
        printf("Execution time for Counting Sort is: %f sec\n",time_CountingSort);
        printf("Operations: %d\n",operation_CountingSort);


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
        printf(" %0.3f, (%0.3f), ",Data[i].T_degC,Data[i].PO4uM);
        printf("%0.3f, %0.3f, ",Data[i].SiO3uM,Data[i].NO2uM);
        printf("%0.3f, %0.3f, ",Data[i].NO3uM,Data[i].Salnty);
        printf("%0.3f\n",Data[i].O2ml_L);
    }
}

int HeapSort(struct ocean_data *Data)
{
    //Δηλώσεις μεταβλητών
    int l,r,i,j,flag=0,k;
    int counter=0;
    struct ocean_data temp;

    //Το l δείχνει την θέση από την οποία ισχύει η δομή σωρού
    //και το r τη θέση από την οποία τα στοιχεία Data[r+1],...,Data[n]
    //είναι ταξινομημένα
    l=(1404/2)+1;
    r=1404;

    while(r>=1)
    {
        flag=0;

        //Φάση δόμησης και προστίθεται το
        //στοιχείο Data[l-1]
        if(l>0)
        {
            l--;
            j=l;
            counter++;

        }
        //Τα στοιχεία Data[0],...,Data[r] σχηματίζουν έναν σωρό. Το Data[0]
        //είναι το μέγιστο στοιχείο αυτού του τμήματος. Εναλλάσουμε Data[0]
        //με Data[r] και επαναφέρουμε την ιδιότητα σωρού στο Data[0..r-1]
        else
        {
            temp=Data[0];
            Data[0]=Data[r];
            Data[r]=temp;
            r--;
            j=0;
            counter++;
        }

        temp=Data[j];

        //Το Data[j] βυθίζεται προς τα κάτω στο σωρό και εναλλάσσεται
        //επαναλαμβανόμενα με το μεγαλύτερο παιδί του
        while(2*j<=r && flag==0)
        {

            k=2*j;
            if(k<r && Data[k].PO4uM<Data[k+1].PO4uM)
            {
                counter++;
                k++;
            }
            if(temp.PO4uM<Data[k].PO4uM)
            {
                counter++;
                Data[j]=Data[k];
                j=k;
            }
            else
            {
                counter++;
                flag=1;
            }
        }
        Data[j]=temp;
    }

    return counter;
}

int CountingSort(struct ocean_data *Data)
{
    //Δηλώσεις μεταβλητών
    int i,j,count=0;

    //Δήλωση πίνακα με δυναμική δέσμευση μνήμης
    //για την μέτρηση των τιμών
    int *counter = (int*) malloc(10000*sizeof(int));

    //Δήλωση πίνακα δομών με δυναμκή δέσμευση μνήμης
    //για την παραγωγή εξόδου
    struct ocean_data* out = malloc(1405*sizeof *out);

    //Μετατροπή των δεκαδικών αριθμών σε
    //ακέραιους
    for(i=0; i<1405; i++)
    {
        Data[i].PO4uM=round(Data[i].PO4uM*100);
    }

    //Αρχικοποίηση του πίνακα counter με '0'
    for(i=0;i<10000;i++){
        count++;
        counter[i]=0;
    }

    //Αποθήκευση του πλήθους κάθε στοιχείου
    //Data[j].PO4uM
    for(j=0;j<1405;j++){
        count++;
        counter[(int)Data[j].PO4uM]++;
    }

    for (i = 1;i<10000;i++) {
        count++;
        counter[i]=counter[i]+counter[i-1];
    }

    //Δημιουργία του ταξιονομιμένου πίνακα out
    for (j=1404;j>=0;j--) {
        count++;
        out[counter[(int)Data[j].PO4uM]-1] = Data[j];
        counter[(int)Data[j].PO4uM]--;
    }

    //Αντιγραφή του ταξιονομημένου πίνακα out στον
    //Data
    for (i=0;i<1405;i++) {
        Data[i]=out[i];
    }

    //Επαναφορά των δεκαδικών τιμών
    for(i=0; i<1405; i++)
    {
        Data[i].PO4uM=Data[i].PO4uM/100;
    }

    return count;
}


