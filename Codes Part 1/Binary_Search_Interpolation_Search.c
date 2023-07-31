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

//Πρότυπο συνάρτησης του Binary Search αλγορίθμου
int BinSearch(struct ocean_data *Data,int year,int month,int day);

//Πρότυπο συνάρτησης ταξινόμησης των ημερομηνιών
//με χρήση Buble Sort
void SortDates(struct ocean_data *Data);

//Πρότυπο συνάρτησης του Interpolation Search αλγορίθμου
int InteSearch(struct ocean_data *Data,int year,int month,int day);


int main()
{

    int error=0,day,month,year; //Δηλώσεις μεταβλητών

    //Δημιουργία πίνακα δομών με δυναμική δέσμευση μνήμης
    struct ocean_data* Data= malloc(1405*sizeof *Data);

    //Δηλώσεις μεταβλητών για τις πειραματικές μετρήσεις
    int operation_BinSearch,operation_InterSearch;
    struct timespec start_1,end_1;
    struct timespec start_2,end_2;

    //Εξαγωγή των δεδομένων από το αρχείο CSV
    //και αποθήκευση στον πίνακα δομών Data
    error=data_extraction(Data);


    if(error!=1) //Έλεγχος για το αν προκύψει σφάλμα στην εξαγωγή
    {

        //Ο χρήστης δίνει μια ημερομηνία για αναζήτηση
        do
        {
            printf("Give date(12/25/2005): ");
            scanf("%d/%d/%d",&month,&day,&year);
            fflush(stdin);
            if(year<=999 || year>=10000 || month<=0 || month>=13 || day<=0 || day>=32){
                printf("Invalid Date\n");
            }
        }
        while(year<=999 || year>=10000 || month<=0 || month>=13 || day<=0 || day>=32);

        //Κλήση συνάρτησης για ταξινόμηση του πίνακα
        SortDates(Data);

        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου αναζήτησης
        //Binary Search
        clock_gettime(CLOCK_MONOTONIC, &start_1);
        operation_BinSearch=BinSearch(Data,year,month,day);
        clock_gettime(CLOCK_MONOTONIC, &end_1);

        //Πειραματικές μετρήσεις και κλήση
        //συνάρτησης του αλγορίθμου αναζήτησης
        //Interpolation Search
        clock_gettime(CLOCK_MONOTONIC, &start_2);
        operation_InterSearch=InteSearch(Data,year,month,day);
        clock_gettime(CLOCK_MONOTONIC, &end_2);

        //Υπολογισμός των πειραματικών μετρήσεων
        double time_InterSearch=(end_2.tv_sec-start_2.tv_sec)*1e9;
        time_InterSearch=(time_InterSearch+(end_2.tv_nsec-start_2.tv_nsec))*1e-9;

        double time_BinSearch=(end_1.tv_sec-start_1.tv_sec)*1e9;
        time_BinSearch=(time_BinSearch+(end_1.tv_nsec-start_1.tv_nsec))*1e-9;

        //Τύπωση των αποτελεσμάτων των πειραματικών μετρήσεων
        printf("\n\nExecution time for Binary Search Sort is: %f sec\n",time_BinSearch);
        printf("Operations: %d\n",operation_BinSearch);
        printf("Execution time for Interpolation Search is: %f sec\n",time_InterSearch);
        printf("Operations: %d\n",operation_InterSearch);


    }
    else if(error==1)
    {
        //Εάν δεν γίνει εξαγωγή των δεδομένων
        //τυπώνει ειδοποίηση προς τον χρήστη
        printf("File did not open\nError\n");
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

int BinSearch(struct ocean_data *Data,int year,int month,int day)
{
    //Δηλώσεις μεταβλητών
    int l,r,next;
    int counter=0;

    //Τοποθέτηση των ακρών του πίνακα
    r=1404;
    l=0;

    //Δυαδική αναζήτηση
    next=(r+l)/2;

    //Ελεγχος για το αν το έτος βρίσκεται
    //εντός του πίνακα
    if(year>=Data[l].Date[2] && Data[r].Date[2]>=year)
    {

        //Εύρεση του έτους στον πίνακα
        while(year!=Data[next].Date[2] && l<r)
        {
            counter++;
            if(year<=Data[next].Date[2])
            {
                r=next-1;
            }
            else
            {
                l=next+1;
            }

            //Δυαδική αναζήτηση
            next=(r+l)/2;
        }

        //ΈΛεγχος για το αν βρέθηκε το
        //έτος
        if(year==Data[next].Date[2])
        {

            l=next;
            r=next;

            //Τοποθέτηση νέων ορίων για
            //αναζήτηση στον πίνακα
            while(year==Data[l].Date[2])
            {
                counter++;
                l--;
            }
            while(year==Data[r].Date[2])
            {
                counter++;
                r++;
            }

            l++;
            r--;

            //Δυαδική αναζήτηση
            next=(r+l)/2;

            //Έλεγχος για το αν ο μήνας βρίσκεται εντός
            //του πίνακα
            if(month>=Data[l].Date[0] && Data[r].Date[0]>=month)
            {

                //Εύρεση του μήνα στον πίνακα
                while(month!=Data[next].Date[0] && l<r)
                {
                    if(month<=Data[next].Date[0])
                    {
                        r=next-1;
                    }
                    else
                    {
                        l=next+1;
                    }
                    counter++;

                    //Δυαδική αναζήτηση
                    next=(r+l)/2;
                }

                //Έλεγχος για το αν βρέθηκε ο
                //μήνας
                if(month==Data[next].Date[0])
                {
                    l=next;
                    r=next;


                    //Τοποθέτηση νέων ορίων για
                    //αναζήτηση στον πίνακα
                    while(month==Data[l].Date[0])
                    {
                        counter++;
                        l--;
                    }

                    while(month==Data[r].Date[0])
                    {
                        counter++;
                        r++;
                    }

                    l++;
                    r--;

                    //Δυαδική αναζήτηση
                    next=(r+l)/2;

                    //Έλεγχος για το αν η μέρα βρίσκεται εντός
                    //του πίνακα
                    if(day>=Data[l].Date[1] && Data[r].Date[1]>=day)
                    {

                        //Εύρεση της μέρας στον πίνακα
                        while(day!=Data[next].Date[1] && l<r)
                        {
                            if(day<=Data[next].Date[1])
                            {
                                r=next-1;
                            }
                            else
                            {
                                l=next+1;
                            }

                            counter++;

                            //Δυαδική αναζήτηση
                            next=(r+l)/2;
                        }
                    }
                }
            }
        }
    }

    //Εάν βρέθηκε η ημερομηνία εφάνιση θερμοκρασίας και Phosphate
    if(year==Data[next].Date[2] && month==Data[next].Date[0] && day==Data[next].Date[1])
    {
        printf("\nBinary Search:\n");
        printf("Date found\n");
        printf("Temperature: %.3f\n",Data[next].T_degC);
        printf("Phosphate: %.03f\n",Data[next].PO4uM);


        //Εάν υπάρχουν δύο μετρήσει για μια ημερομηνία εμφάνισει και αυτών
        if(year==Data[next+1].Date[2] && month==Data[next+1].Date[0] && day==Data[next+1].Date[1])
        {
            next++;
            printf("\nDate found\n");
            printf("Temperature: %.3f\n",Data[next].T_degC);
            printf("Phosphate: %.03f\n",Data[next].PO4uM);

        }
        else if(year==Data[next-1].Date[2] && month==Data[next-1].Date[0] && day==Data[next-1].Date[1])
        {
            next--;
            printf("\nDate found\n");
            printf("Temperature: %.3f\n",Data[next].T_degC);
            printf("Phosphate: %.03f\n",Data[next].PO4uM);
        }
    }
    else
    {
        //Εάν η ημερομηνία δεν βρέθηκε
        //ενημερώνουμε τον χρήστη
        printf("\nBinary Search:\n");
        printf("Date not found\n");
    }

    return counter;
}

void SortDates(struct ocean_data *Data)
{
    //Δηλώσεις μεταβλητών
    int i,j;
    struct ocean_data temp;

    //Ταξινόμηση των μηνών
    for(i=0; i<1404; i++)
    {
        for(j=0; j<1404-i; j++)
        {
            if(Data[j].Date[2]>=Data[j+1].Date[2])
            {
                if(Data[j].Date[0]>=Data[j+1].Date[0])
                {
                    temp=Data[j];
                    Data[j]=Data[j+1];
                    Data[j+1]=temp;
                }
            }
        }
    }

    //Ταξινόμηση ημερών
    for(i=0; i<1404; i++)
    {
        for(j=0; j<1404-i; j++)
        {
            if(Data[j].Date[2]==Data[j+1].Date[2] && Data[j].Date[0]==Data[j+1].Date[0] && Data[j].Date[1]>=Data[j+1].Date[1])
            {
                temp=Data[j];
                Data[j]=Data[j+1];
                Data[j+1]=temp;
            }
        }
    }



}

int  InteSearch(struct ocean_data *Data,int year,int month,int day)
{

    //Δηλώσεις μεταβλητών
    int l,r,next;
    int temp=0,flag=0,flag_1=0;
    int counter=0;

    //Τοποθέτηση των ακρών του πίνακα
    r=1404;
    l=0;

    //Ελεγχος για το αν το έτος βρίσκεται
    //εντός του πίνακα
    if(year>=Data[l].Date[2] && year<=Data[r].Date[2])
    {
        //Αναζήτση παρεμβολής
        next=((r-l)*(year-Data[l].Date[2]))/(Data[r].Date[2]-Data[l].Date[2]);
        next=next+l;

        //Εύρεση του έτους στον πίνακα
        while(year!=Data[next].Date[2] && l<r && year<=Data[r].Date[2])
        {
            if(year<=Data[next].Date[2])
            {
                r=next-1;
            }
            else
            {
                l=next+1;
            }
            counter++;

            //Αναζήτση παρεμβολής
            next=((r-l)*(year-Data[l].Date[2]))/(Data[r].Date[2]-Data[l].Date[2]);
            next=next+l;
        }

        //ΈΛεγχος για το αν βρέθηκε το
        //έτος
        if(year==Data[next].Date[2])
        {

            l=next;
            r=next;

            //Τοποθέτηση νέων ορίων για
            //αναζήτηση στον πίνακα
            while(year==Data[l].Date[2])
            {
                counter++;
                l--;
            }
            while(year==Data[r].Date[2])
            {
                counter++;
                r++;
            }
            r--;
            l++;

            //Έλεγχος για το αν ο μήνας βρίσκεται εντός
            //του πίνακα
            if(month>=Data[l].Date[0] && Data[r].Date[0]>=month)
            {

                //Αναζήτση παρεμβολής εάν ο παρανομαστής είναι
                //διάφορος του 0
                if(Data[r].Date[0]!=Data[l].Date[0])
                {
                    next=((r-l)*(month-Data[l].Date[0]))/(Data[r].Date[0]-Data[l].Date[0]);
                    next=next+l;
                }
                //Αλλιώς το next παίρνει την τιμή του l
                else
                {
                    next=l;
                }

                //Έλεγχος για το αν ο μήνας βρίσκεται εντός
                //του πίνακα
                while(month!=Data[next].Date[0] && l<r && month>=Data[l].Date[0])
                {
                    counter++;

                    //Εύρεση του μήνα στον πίνακα
                    if(month<=Data[next].Date[0])
                    {
                        r=next-1;
                    }
                    else
                    {
                        l=next+1;
                    }

                    //Αναζήτση παρεμβολής εάν ο παρανομαστής είναι
                    //διάφορος του 0
                    if(Data[r].Date[0]!=Data[l].Date[0])
                    {
                        next=((r-l)*(month-Data[l].Date[0]))/(Data[r].Date[0]-Data[l].Date[0]);
                        next=next+l;
                    }
                    //Αλλιώς το next παίρνει την τιμή του l
                    else
                    {
                        next=l;
                    }

                }

                //Έλεγχος για το αν βρέθηκε ο
                //μήνας
                if(month==Data[next].Date[0])
                {
                    l=next;
                    r=next;

                    //Τοποθέτηση νέων ορίων για
                    //αναζήτηση στον πίνακα
                    while(month==Data[l].Date[0])
                    {
                        counter++;
                        l--;
                    }

                    while(month==Data[r].Date[0])
                    {
                        counter++;
                        r++;
                    }


                    r--;
                    l++;

                    //Αναζήτση παρεμβολής εάν ο παρανομαστής είναι
                    //διάφορος του 0
                    if(Data[r].Date[1]!=Data[l].Date[1])
                    {
                        next=(day-Data[l].Date[1])/(Data[r].Date[1]-Data[l].Date[1]);
                        next=(next*(r-l))+l;
                    }
                    //Αλλιώς το next παίρνει την τιμή του l
                    else
                    {
                        next=l;
                    }

                    //Εύρεση της μέρας στον πίνακα
                    while(day!=Data[next].Date[1] && l<r && flag_1==0 && day>=Data[l].Date[1] && day<=Data[r].Date[1])
                    {
                        counter++;
                        temp=next;
                        if(day<=Data[next].Date[1])
                        {
                            r=next-1;
                        }
                        else
                        {
                            l=next+1;
                        }

                        //Αναζήτση παρεμβολής εάν ο παρανομαστής είναι
                        //διάφορος του 0
                        if(Data[r].Date[1]!=Data[l].Date[1])
                        {
                            next=(day-Data[l].Date[1])/(Data[r].Date[1]-Data[l].Date[1]);
                            next=(next*(r-l))+l;
                        }
                        //Αλλιώς το next παίρνει την τιμή του l
                        else
                        {
                            next=l;
                        }
                    }
                }
            }
        }
    }


    //Εάν βρέθηκε η ημερομηνία εφάνιση θερμοκρασίας και Phosphate
    if(year==Data[next].Date[2] && month==Data[next].Date[0] && day==Data[next].Date[1])
    {
        printf("\nInterpolation Search:\n");
        printf("Date found\n");
        printf("Temperature: %.3f\n",Data[next].T_degC);
        printf("Phosphate: %.03f\n",Data[next].PO4uM);

        //Εάν υπάρχουν δύο μετρήσει για μια ημερομηνία εμφάνισει και αυτών
        if(year==Data[next+1].Date[2] && month==Data[next+1].Date[0] && day==Data[next+1].Date[1])
        {
            next++;
            printf("\nDate found\n");
            printf("Temperature: %.3f\n",Data[next].T_degC);
            printf("Phosphate: %.03f\n",Data[next].PO4uM);
        }
        else if(year==Data[next-1].Date[2] && month==Data[next-1].Date[0] && day==Data[next-1].Date[1])
        {
            next--;
            printf("\nDate found\n");
            printf("Temperature: %.3f\n",Data[next].T_degC);
            printf("Phosphate: %.03f\n",Data[next].PO4uM);
        }

    }
    else
    {
        //Εάν η ημερομηνία δεν βρέθηκε
        //ενημερώνουμε τον χρήστη
        printf("\nInterpolation Search:\n");
        printf("Date not found\n");
    }
    return counter;
}
