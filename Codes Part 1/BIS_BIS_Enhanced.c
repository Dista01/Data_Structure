#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

struct ocean_data //���� ���� ������������ �� ��������
{                 //���� ������� ��� ������� CSV
    int Date[3];
    float T_degC;
    float PO4uM;
    float SiO3uM;
    float NO2uM;
    float NO3uM;
    float Salnty;
    float O2ml_L;
};

//��������� ���������� ���� ������ ��
//�������� ��� �� ������ CSV
int data_extraction(struct ocean_data *Data);

//������� ���������� ��� ��������� ��
//�������� ��� ����� ������� ��� �� ������ CSV
void data_print(struct ocean_data *Data);

//������� ���������� ��� ������ ��� ����������
//��� ��� ��������� ���� ���� ocean_data
void split(struct ocean_data *Data,char **date);

//������� ���������� ����������� ��� �����������
//�� ����� Buble Sort
void SortDates(struct ocean_data *Data);

//������� ���������� ��� Binary Interpolation Search ����������
int BIS(struct ocean_data *Data,int year,int month,int day);

//������� ���������� ��� Binary Interpolation Search ���������� ��
//�������� ������  ���������� ����������
int BIS_Enhanced(struct ocean_data *Data,int year,int month,int day);

int main()
{
    //�������� ����������
    int error=0,day,month,year;

    //�������� ���������� ��� ��� ������������ ���������
    int operation_BIS,operation_BIS_ench;
    struct timespec start_1,end_1;
    struct timespec start_2,end_2;

    //���������� ������ ����� �� �������� �������� ������
    struct ocean_data* Data= malloc(1405*sizeof *Data);

    //������� ��� ��������� ��� �� ������ CSV
    //��� ���������� ���� ������ ����� Data
    error=data_extraction(Data);

    if(error!=1) //������� ��� �� �� �������� ������ ���� �������
    {

        //� ������� ����� ��� ���������� ��� ���������
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


        //����� ���������� ��� ���������� ��� ������
        SortDates(Data);

        //������������ ��������� ��� �����
        //���������� ��� ���������� ����������
        //Binary Interpolation Search
        clock_gettime(CLOCK_MONOTONIC, &start_1);
        operation_BIS=BIS(Data,year,month,day);
        clock_gettime(CLOCK_MONOTONIC, &end_1);

        //������������ ��������� ��� �����
        //���������� ��� ���������� ����������
        //Binary Interpolation Search ��
        //�������� ���������� ����������
        clock_gettime(CLOCK_MONOTONIC, &start_2);
        operation_BIS_ench=BIS_Enhanced(Data,year,month,day);
        clock_gettime(CLOCK_MONOTONIC, &end_2);

        //����������� ��� ������������ ���������
        double time_BIS=(end_1.tv_sec-start_1.tv_sec)*1e9;
        time_BIS=(time_BIS+(end_1.tv_nsec-start_1.tv_nsec))*1e-9;

        double time_BIS_ench=(end_2.tv_sec-start_2.tv_sec)*1e9;
        time_BIS_ench=(time_BIS_ench+(end_2.tv_nsec-start_2.tv_nsec))*1e-9;

        //������ ��� ������������� ��� ������������ ���������
        printf("\n\n\nExecution time for BIS is: %f sec\n",time_BIS);
        printf("Operations: %d\n",operation_BIS);
        printf("Execution time for BIS Enhanced is: %f sec\n",time_BIS_ench);
        printf("Operations: %d\n",operation_BIS_ench);

    }
    else if(error==1)
    {
        //��� ��� ����� ������� ��� ���������
        //������� ���������� ���� ��� ������
        printf("File did not open\nError");
    }

    return 0;
}


int data_extraction(struct ocean_data *Data)
{

    //������ pointer ��� ��� �����������
    //��� ������������ �� �� ������
    FILE* file;

    //������� ��� �������
    file=fopen("ocean.csv","r");

    //������� ��� �� �� �� ������ ����� �����
    if(file==NULL)
    {
        return 1;
    }

    //�������� ����������
    char buffer[128];
    int flag_1=-1,flag_2=0;
    char *info;
    int i=0;
    char *date[1405];

    //������������ ������������� ������ ��
    //�������� �������� ������
    for(i=0; i<1405; i++)
    {
        date[i]=(char*)malloc(10* sizeof(char));
    }


    while(fgets(buffer,128,file))
    {
        flag_1++;
        flag_2=0;

        //�������� ��� ��������� �� ��� ����� ��� ','
        info = strtok(buffer,",");

        while(info)
        {
            if(flag_1!=0)
            {
                //���������� ��� ����������� ���� ������ date
                if(flag_2==0)
                {
                    for(i=0; i<10; i++)
                    {
                        date[flag_1-1][i]=info[i];
                    }
                }
                else
                {
                    //���������� ������������ ���� ������ �����
                    if(flag_2==1)
                    {
                        Data[flag_1-1].T_degC=atof(info);
                    }
                    //���������� Phosphate ���� ������ �����
                    else if(flag_2==2)
                    {
                        Data[flag_1-1].PO4uM=atof(info);
                    }
                    //���������� Silicate ���� ������ �����
                    else if(flag_2==3)
                    {
                        Data[flag_1-1].SiO3uM=atof(info);
                    }
                    //���������� Nitrite ���� ������ �����
                    else if(flag_2==4)
                    {
                        Data[flag_1-1].NO2uM=atof(info);
                    }
                    //���������� Nitrate ���� ������ �����
                    else if(flag_2==5)
                    {
                        Data[flag_1-1].NO3uM=atof(info);
                    }
                    //���������� Salinity ���� ������ �����
                    else if(flag_2==6)
                    {
                        Data[flag_1-1].Salnty=atof(info);
                    }
                    //���������� Oxygen ���� ������ �����
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

    //����� ���������� split
    split(Data,date);

    //�� ������ �������
    fclose(file);

    return 0;
}

void split(struct ocean_data *Data,char **date)
{
    //�������� �����������
    int i=0,j=0,counter=0;
    char *split_1,split_2[10];

    for(j=0; j<1405; j++)
    {
        counter=0;
        for(i=0; i<10; i++)
        {
            split_2[i]=date[j][i];
        }

        //�������� ����������� ��
        //����,����,����
        split_1=strtok(split_2,"/");

        //���������� ���� ������ �����
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

    //�������� ��� ������������ ��� ������ �����
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

void SortDates(struct ocean_data *Data)
{

    //�������� ����������
    int i,j;
    struct ocean_data temp;

    //���������� ��� �����
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

    //���������� ������
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

int BIS(struct ocean_data *Data,int year,int month,int day)
{
    //�������� ����������
    int l,r,size,next,i,temp;
    int counter=0;

    //���������� ��� ����� ��� ������
    l=0;
    r=1404;
    size=r-l+1;

    //������� ��� �� �� �� ���� ���������
    //����� ��� ������
    if(year>=Data[l].Date[2] && year<=Data[r].Date[2])
    {

        //Binary Interpolation Search
        next=(year-Data[l].Date[2])/(Data[r].Date[2]-Data[l].Date[2]);
        next=(next*size)+1;

        //������ ��� ����� ���� ������
        while(year!=Data[next].Date[2])
        {
            i=0;
            size=r-l+1;

            //��� �� size ����� �����, �� �����
            //�������� ���������
            if(size<=5)
            {
                next=l;
                while(next<=r && Data[next].Date[2]!=year)
                {
                    counter++;
                    next++;
                }
            }

            //����������� ��� r ��� l
            if(year>=Data[next].Date[2])
            {
                while(year>Data[(int)(next+i*sqrt(size)-1)].Date[2])
                {
                    counter++;
                    i++;
                }

                r=next+i*sqrt(size);
                l=next+(i-1)*sqrt(size);

                if(l<0)
                {
                    l=0;
                }

            }
            else if(year<Data[next].Date[2])
            {
                while(year<Data[(int)(next-i*sqrt(size)+1)].Date[2])
                {
                    counter++;
                    i++;
                }
                r=next-(i-1)*sqrt(size);
                l=next-i*sqrt(size);
                if(r<0)
                {
                    r=1;
                }
            }

            //Binary Interpolation Search ��� � ������������
            //����� �������� ��� 0
            if(Data[r].Date[2]!=Data[l].Date[2])
            {
                next=(year-Data[l].Date[2])/(Data[r].Date[2]-Data[l].Date[2]);
                next=next*(r-l+1);
                next=next+l-1;
            }
            //������ �� next ������� ��� ���� ��� l
            else
            {
                next=l;
            }

        }

        l=next;
        r=next;

        //���������� ���� ����� ���
        //��������� ���� ������
        while(Data[l].Date[2]==year)
        {
            counter++;
            l--;
        }
        while(Data[r].Date[2]==year)
        {
            counter++;
            r++;
        }
        l++;
        r--;


        size=r-l+1;

        //�������� ����������
        int months[size];
        int l_2,r_2;
        int j,k=0;

        //���������� ��� ����� ��� ����� ����
        //��������� ���� ������ months
        for(j=l; j<=r; j++)
        {
            months[k]=Data[j].Date[0];
            k++;
        }

        //���������� ����� ���
        //��������� ���� ������
        //months
        l_2=0;
        r_2=size-1;

        size=r_2-l_2+1;
        int flag=0;

        //������� ��� �� �� � ����� ���������
        //����� ��� ������
        if(month>=months[l_2] && month<=months[r_2])
        {
            //Binary Interpolation Search
            next=(month-months[l_2])/(months[r_2]-months[l_2]);
            next=(next*size)+1;

            //��� �� next ����� ���������� ��� ����� r_2
            //���� ������� ����� ��� ����
            if(next>r_2)
            {
                next=r_2;
            }

            //������ ��� ����� ���� ������ months
            while(month!=months[next] && flag==0)
            {
                i=0;
                size=r_2-l_2+1;

                //��� �� size ����� �����, �� �����
                //�������� ���������
                if(size<=10)
                {
                    next=l_2;
                    while(next<=r_2 && months[next]!=month)
                    {
                        counter++;
                        next++;
                    }
                    flag=1;
                }

                //����������� ��� r_2 ��� l_2
                if(month>months[next])
                {
                    while(month>months[(int)(next+i*sqrt(size)-1)])
                    {
                        counter++;
                        i++;
                    }


                    r_2=next+i*sqrt(size);
                    l_2=next+(i-1)*sqrt(size);

                    if(l_2<0)
                    {
                        l_2=0;
                    }

                }
                else if(month<months[next])
                {
                    while(month<months[(int)(next-i*sqrt(size)+1)])
                    {
                        counter++;
                        i++;
                    }
                    r_2=next-(i-1)*sqrt(size);
                    l_2=next-i*sqrt(size);
                    if(r_2<0)
                    {
                        r_2=1;
                    }
                }

                //Binary Interpolation Search ��� � ������������
                //����� �������� ��� 0 ��� ��� ���� ������ � ����������
                if(months[r_2]!=months[l_2] && months[next]!=month)
                {
                    next=(month-months[l_2])/(months[r_2]-months[l_2]);
                    next=next*(r_2-l_2+1);
                    next=next+l_2-1;
                }
                //������ �� next ������� ��� ���� ��� l
                else if(months[next]!=month)
                {
                    next=l_2;
                }

            }

            //����������� ��� next ��� ���
            //������ Data
            next=next+l;


            l=next;
            r=next;

            //���������� ���� ����� ���
            //��������� ���� ������
            while(Data[l].Date[0]==month)
            {
                l--;
                counter++;
            }

            while(Data[r].Date[0]==month)
            {
                r++;
                counter++;
            }

            l++;
            r--;

            size=r-l+1;

            //�������� ����������
            int days[size];
            k=0;

            //���������� ��� ������ ��� ����� ��� ���
            //���� ���� ��������� ���� ������ days
            for(j=l; j<=r; j++)
            {
                days[k]=Data[j].Date[1];
                k++;

            }

            //���������� ����� ���
            //��������� ���� ������
            //days
            l_2=0;
            r_2=size-1;

            size=r_2-l_2+1;
            int flag=0;

            //������� ��� �� �� � ���� ���������
            //����� ��� ������
            if(day>=days[l_2] && day<=days[r_2])
            {
                if(size!=1)
                {
                    next=(day-days[l_2])/(days[r_2]-days[l_2]);
                    next=(next*size)+1;
                }
                //��� �� next ����� ���������� ��� ����� r_2
                //���� ������� ����� ��� ����
                if(next>r_2)
                {
                    next=r_2;
                }

                //������ ��� ����� ���� ������ days
                while(day!=days[next] && flag==0)
                {
                    i=0;
                    size=r_2-l_2+1;
                    counter++;

                    //��� �� size ����� �����, �� �����
                    //�������� ���������
                    if(size<=6)
                    {
                        next=l_2;

                        while(next<=r_2 && days[next]!=day)
                        {
                            next++;
                        }
                        flag=1;

                    }



                    //����������� ��� r_2 ��� l_2
                    if(day>days[next])
                    {
                        while(day>days[(int)(next+i*sqrt(size)-1)] && (int)(next+i*sqrt(size)-1)<size)
                        {
                            i++;
                            counter++;
                        }

                        if((int)(next+i*sqrt(size)-1)>size)
                        {
                            i--;
                        }

                        r_2=next+i*sqrt(size);
                        l_2=next+(i-1)*sqrt(size);

                        if(l_2<0)
                        {
                            l_2=0;
                        }

                    }
                    else if(day<days[next])
                    {
                        while(day<days[(int)(next-i*sqrt(size)+1)])
                        {
                            i++;
                            counter++;
                        }
                        r_2=next-(i-1)*sqrt(size);
                        l_2=next-i*sqrt(size);

                        if(r_2<0)
                        {
                            r_2=1;
                            l_2=0;
                        }
                        else if(l_2<0)
                        {
                            l_2=0;
                        }

                    }
                    //Binary Interpolation Search ��� � ������������
                    //����� �������� ��� 0 ��� ��� ���� ������ � ����������
                    if(days[r_2]!=days[l_2] && days[next]!=day)
                    {
                        next=(day-days[l_2])/(days[r_2]-days[l_2]);
                        next=next*(r_2-l_2+1);
                        next=next+l_2-1;

                    }
                    //������ �� next ������� ��� ���� ��� l
                    else if(days[next]!=day)
                    {
                        next=l_2;
                    }

                }

                //����������� ��� next ��� ���
                //������ Data
                next=next+l;



                //��� ������� � ���������� ������� ������������ ��� Phosphate
                if(year==Data[next].Date[2] && month==Data[next].Date[0] && day==Data[next].Date[1] && next!=-1)
                {
                    printf("\nDate found (BIS)\n");
                    printf("Temperature: %.3f\n",Data[next].T_degC);
                    printf("Phosphate: %.03f\n",Data[next].PO4uM);

                    //��� �������� ��� �������� ��� ��� ���������� ��������� ��� �����
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
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS)\n");

                }
            }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS)\n");

                }
        }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS)\n");

                }
    }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS)\n");

                }

    return counter;
}

int BIS_Enhanced(struct ocean_data *Data,int year,int month,int day)
{
    //�������� ����������
    int l,r,size,next,i,temp;
    int counter=0;

    //���������� ��� ����� ��� ������
    l=0;
    r=1404;
    size=r-l+1;

    //������� ��� �� �� �� ���� ���������
    //����� ��� ������
    if(year>=Data[l].Date[2] && year<=Data[r].Date[2])
    {
        //Binary Interpolation Search
        next=(year-Data[l].Date[2])/(Data[r].Date[2]-Data[l].Date[2]);
        next=(next*size)+1;

        //������ ��� ����� ���� ������
        while(year!=Data[next].Date[2])
        {
            i=0;
            size=r-l+1;

            //��� �� size ����� �����, �� �����
            //�������� ���������
            if(size<=8)
            {

                next=l;
                while(next<=r && Data[next].Date[2]!=year)
                {
                    counter++;
                    next++;
                }
            }

            //����������� ��� r ��� l �� �������� ��� ������ ����������
            //����������
            if(year>=Data[next].Date[2])
            {
                while(year>Data[(int)(next+i*sqrt(size)-1)].Date[2])
                {

                    if(i==0){
                        i++;
                    }

                    counter++;
                    i=2*i;

                    if((int)(next+i*sqrt(size)-1)>1404){
                        while((int)(next+i*sqrt(size)-1)>1404){
                            i--;
                        }
                    }

                }

                r=next+i*sqrt(size);
                l=next+(i-1)*sqrt(size);

                if(l<0)
                {
                    l=0;
                }

            }
            else if(year<Data[next].Date[2])
            {
                while(year<Data[(int)(next-i*sqrt(size)+1)].Date[2])
                {
                    if(i==0){
                        i++;
                    }
                    counter++;
                    i=2*i;
                }
                r=next-(i-1)*sqrt(size);
                l=next-i*sqrt(size);

                if(r<0)
                {
                    r=1;
                }
            }

            //Binary Interpolation Search ��� � ������������
            //����� �������� ��� 0
            if(Data[r].Date[2]!=Data[l].Date[2])
            {
                next=(year-Data[l].Date[2])/(Data[r].Date[2]-Data[l].Date[2]);
                next=next*(r-l+1);
                next=next+l-1;

            }
            //������ �� next ������� ��� ���� ��� l
            else
            {
                next=l;
            }

        }

        l=next;
        r=next;

        //���������� ���� ����� ���
        //��������� ���� ������
        while(Data[l].Date[2]==year)
        {
            counter++;
            l--;
        }

        while(Data[r].Date[2]==year)
        {
            counter++;
            r++;
        }

        l++;
        r--;

        size=r-l+1;

        //�������� ����������
        int months[size];
        int l_2,r_2;
        int j,k=0;

        //���������� ��� ����� ��� ����� ����
        //��������� ���� ������ months
        for(j=l; j<=r; j++)
        {
            months[k]=Data[j].Date[0];
            k++;

        }


        //���������� ����� ���
        //��������� ���� ������
        //months
        l_2=0;
        r_2=size-1;
        size=r_2-l_2+1;
        int flag=0;


        //������� ��� �� �� � ����� ���������
        //����� ��� ������
        if(month>=months[l_2] && month<=months[r_2])
        {
            //Binary Interpolation Search
            next=(month-months[l_2])/(months[r_2]-months[l_2]);
            next=(next*size)+1;

            //��� �� next ����� ���������� ��� ����� r_2
            //���� ������� ����� ��� ����
            if(next>r_2)
            {
                next=r_2;
            }

            //������ ��� ����� ���� ������ months
            while(month!=months[next] && flag==0)
            {
                i=0;
                size=r_2-l_2+1;
                counter++;

                //��� �� size ����� �����, �� �����
                //�������� ���������
                if(size<=10)
                {
                    next=l_2;
                    while(next<=r_2 && months[next]!=month)
                    {
                        counter++;
                        next++;
                    }
                    flag=1;
                }

                //����������� ��� r_2 ��� l_2
                if(month>months[next])
                {
                    while(month>months[(int)(next+i*sqrt(size)-1)])
                    {
                        if(i==0){
                            i++;
                        }
                        counter++;
                        i=2*i;
                    }


                    r_2=next+i*sqrt(size);
                    l_2=next+(i-1)*sqrt(size);

                    if(l_2<0)
                    {
                        l_2=0;
                    }

                }
                else if(month<months[next])
                {
                    while(month<months[(int)(next-i*sqrt(size)+1)])
                    {
                        if(i==0){
                            i++;
                        }
                        counter++;
                        i=2*i;
                    }
                    r_2=next-(i-1)*sqrt(size);
                    l_2=next-i*sqrt(size);
                    if(r_2<0)
                    {
                        r_2=1;
                    }
                }

                //Binary Interpolation Search ��� � ������������
                //����� �������� ��� 0 ��� ��� ���� ������ � ����������
                if(months[r_2]!=months[l_2] && months[next]!=month)
                {
                    next=(month-months[l_2])/(months[r_2]-months[l_2]);
                    next=next*(r_2-l_2+1);
                    next=next+l_2-1;
                }
                //������ �� next ������� ��� ���� ��� l
                else if(months[next]!=month)
                {
                    next=l_2;
                }

            }

            //����������� ��� next ��� ���
            //������ Data
            next=next+l;

            l=next;
            r=next;

            //���������� ���� ����� ���
            //��������� ���� ������
            while(Data[l].Date[0]==month)
            {
                l--;
                counter++;
            }

            while(Data[r].Date[0]==month)
            {
                r++;
                counter++;
            }

            l++;
            r--;

            size=r-l+1;

            //�������� ����������
            int days[size];
            k=0;

            //���������� ��� ������ ��� ����� ��� ���
            //���� ���� ��������� ���� ������ days
            for(j=l; j<=r; j++)
            {
                days[k]=Data[j].Date[1];
                k++;
            }

            //���������� ����� ���
            //��������� ���� ������
            //days
            l_2=0;
            r_2=size-1;

            size=r_2-l_2+1;
            int max=r_2;

            //������� ��� �� �� � ���� ���������
            //����� ��� ������
            if(day>=days[l_2] && day<=days[r_2])
            {

                //Binary Interpolation Search
                if(size!=1)
                {
                    next=(day-days[l_2])/(days[r_2]-days[l_2]);
                    next=(next*size)+1;
                }
                //��� �� next ����� ���������� ��� ����� r_2
                //���� ������� ����� ��� ����
                if(next>r_2)
                {
                    next=r_2;
                }

                int flag=0;

                //������ ��� ����� ���� ������ days
                while(day!=days[next] && flag==0)
                {
                    i=0;
                    size=r_2-l_2+1;

                    //��� �� size ����� �����, �� �����
                    //�������� ���������
                    if(size<=6 && days[l_2]<day || days[r_2]>day)
                    {

                        next=l_2;
                        while(next<=r_2 && days[next]!=day)
                        {
                            next++;
                            counter++;
                        }
                        flag=1;

                    }
                    //����������� ��� r_2 ��� l_2 �� �������� ��� ������ ����������
                    //����������
                    else if(day>days[next])
                    {
                        while(day>days[(int)(next+i*sqrt(size)-1)] && (int)(next+i*sqrt(size)-1)<size)
                        {
                            if(i==0){
                                i++;
                            }
                            i=2*i;
                            counter++;

                        }

                        if((int)(next+i*sqrt(size)-1)>max)
                        {
                            i--;
                        }

                        r_2=next+i*sqrt(size);
                        l_2=next+(i-1)*sqrt(size);


                        if(l_2<0)
                        {
                            l_2=0;
                        }

                    }
                    else if(day<days[next])
                    {
                        while(day<days[(int)(next-i*sqrt(size)+1)])
                        {
                            if(i==0){
                                i++;
                            }
                            i=2*i;
                            counter++;
                        }
                        r_2=next-(i-1)*sqrt(size);
                        l_2=next-i*sqrt(size);
                        if(r_2<0)
                        {
                            r_2=1;
                            l_2=0;
                        }
                        else if(l_2<0)
                        {
                            l_2=0;
                        }

                    }
                    //Binary Interpolation Search ��� � ������������
                    //����� �������� ��� 0 ��� ��� ���� ������ � ����������
                    if(days[r_2]!=days[l_2] && days[next]!=day)
                    {
                        next=(day-days[l_2])/(days[r_2]-days[l_2]);
                        next=next*(r_2-l_2+1);
                        next=next+l_2-1;
                    }
                    //������ �� next ������� ��� ���� ��� l
                    else if(days[next]!=day && size>6)
                    {
                        next=l_2;
                    }

                }

                //����������� ��� next ��� ���
                //������ Data
                next=next+l;

                //��� ������� � ���������� ������� ������������ ��� Phosphate
                if(year==Data[next].Date[2] && month==Data[next].Date[0] && day==Data[next].Date[1] && next!=-1)
                {
                    printf("\nDate found (BIS Enhanced)\n");
                    printf("Temperature: %.3f\n",Data[next].T_degC);
                    printf("Phosphate: %.03f\n",Data[next].PO4uM);


                    //��� �������� ��� �������� ��� ��� ���������� ��������� ��� �����
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
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS Enhanced)\n");
                }
            }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS Enhanced)\n");
                }
        }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS Enhanced)\n");

                }
    }else
                {
                    //��� � ���������� ��� �������
                    //������������ ��� ������
                    printf("\nDate not found (BIS Enhanced)\n");
                }

    return counter;
}
