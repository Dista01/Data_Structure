#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

//������� ���������� ��� Insertion Sort ����������
int Insertion_Sort(struct ocean_data *Data);

//������� ���������� ��� QuickSort ����������
int Quicksort(struct ocean_data *Data,int l,int r);

int main()
{

    int error=0,i; //�������� ����������

    //�������� ���������� ��� ��� ������������ ���������
    int operation_Insertion,operation_QuickSort;
    struct timespec start_1,end_1;
    struct timespec start_2,end_2;

    //���������� ������ ����� �� �������� �������� ������
    struct ocean_data* Data_1 = malloc(1405*sizeof *Data_1);
    struct ocean_data* Data_2 = malloc(1405*sizeof *Data_2);

    //������� ��� ��������� ��� �� ������ CSV
    //��� ���������� ���� ������ ����� Data_1
    error=data_extraction(Data_1);


    if(error!=1) //������� ��� �� �� �������� ������ ���� �������
    {
        //��������� ��� ��������� ��� �� Data_1 ��� Data_2
        for(i=0; i<1405; i++)
        {
            Data_2[i]=Data_1[i];
        }

        //������������ ��������� ��� �����
        //���������� ��� ���������� QuickSort
        clock_gettime(CLOCK_MONOTONIC, &start_1);
        operation_QuickSort=Quicksort(Data_2,0,1404);
        clock_gettime(CLOCK_MONOTONIC, &end_1);


        //������������ ��������� ��� �����
        //���������� ��� ���������� Insertion Sort
        clock_gettime(CLOCK_MONOTONIC, &start_2);
        operation_Insertion=Insertion_Sort(Data_1);
        clock_gettime(CLOCK_MONOTONIC, &end_2);

        //����� ���������� ��� ��� �������� ���
        //������ ����� Data_1(Insertion Sort)
        printf("Insertion Sort:\n");
        data_print(Data_1);

        //����� ���������� ��� ��� ������� ��� ������ ����� Data_2(QuickSort)
        printf("\n\n\nQuickSort:\n");
        data_print(Data_2);

        //����������� ��� ������������ ���������
        double time_Insertion=(end_2.tv_sec-start_2.tv_sec)*1e9;
        time_Insertion=(time_Insertion+(end_2.tv_nsec-start_2.tv_nsec))*1e-9;

        double time_QuickSort=(end_1.tv_sec-start_1.tv_sec)*1e9;
        time_QuickSort=(time_QuickSort+(end_1.tv_nsec-start_1.tv_nsec))*1e-9;

        //������ ��� ������������� ��� ������������ ���������
        printf("\n\n\nExecution time for Insertion Sort is: %f sec\n",time_Insertion);
        printf("Operations: %d\n",operation_Insertion);
        printf("Execution time for QuickSort is: %f sec\n",time_QuickSort);
        printf("Operations: %d\n",operation_QuickSort);

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
            //� ������� ��������� �������� ��� ���
            //������� ������ ��� �������
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
    //�������� ����������
    int counter=0;
    int i,j;
    struct ocean_data temp;

    for(j=1; j<1405; j++)
    {
        temp=Data[j];
        //�������� ��� ��������� Data[j] ���� ���������
        //���� ��� ������������� ��������� Data[0..j]
        i=j-1;

        //���������� ���� ��� Data[i].T_degC>temp
        //���� ��� ���� �����
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
    //�������� ����������
    int i,k;
    int counter=0;
    struct ocean_data temp;
    struct ocean_data swap;

    i=l;
    k=r+1;
    //������� ��� ������������� ���������
    //��� �������� �����������
    temp=Data[l];

    //�������
    while(i<k)
    {
        //��������� ������ �������� ���� ��
        //����� ����� �� ������ ���� ����������
        //��� ��������� �����������
        do
        {
            i++;
            counter++;
        }
        while(Data[i].T_degC<temp.T_degC);

        //��������� ������ ����� ���� ��
        //�������� ����� �� ������ ���� ���������
        //��� ��������� �����������
        do
        {
            k--;
            counter++;
        }
        while(Data[k].T_degC>temp.T_degC);

        //�������� Data[i] ���
        //Data[k]
        if(k>i)
        {
            swap=Data[k];
            Data[k]=Data[i];
            Data[i]=swap;
        }

    }

    //�������� Data[k] ���
    //Data[k]
    swap=Data[l];
    Data[l]=Data[k];
    Data[k]=swap;

    //��������
    //���������� ����� ��� QuickSort
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
