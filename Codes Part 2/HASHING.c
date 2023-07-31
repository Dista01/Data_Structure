#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct ocean_data_hashing //���� ���� ������������ �� ��������
{
    //���� ������� ��� ������� CSV
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

//��������� ���������� ���� ������ ��
//�������� ��� �� ������ CSV
int data_extraction_hashing(struct ocean_data_hashing *Data);

//������� ���������� ��� ��������� ��
//�������� ��� ����� ������� ��� �� ������ CSV
void data_print_hashing(struct ocean_data_hashing *Data);

//������� ���������� ��� ������� ��� ���������� ��������
void insert_Hash(char date[10],float T_degC,struct node_Hashing T[11]);

//������� ���������� ���������� ������������
//���� ��� �����������
void search_Temp(struct node_Hashing T[11],char date[10]);

//������� ���������� ������������ ���� ������������
//���� �����������
void modify_Temp(struct node_Hashing T[11],char date[10]);

//�������� ���� �������� ��� ��� ������ ���������������
//���� ���� �����������
void delete_Date(struct node_Hashing T[11],char date[10]);

int main()
{
    //�������� ����������
    int error=0,select=0;
    char check;
    char date[10];

    //������������ ������ ����� �� �������� �������� ������
    struct ocean_data_hashing* Data= malloc(1405*sizeof *Data);

    //������� ��� ��������� ��� �� ������ CSV
    //��� ���������� ���� ������ ����� Data
    error=data_extraction_hashing(Data);


    if(error!=1)
    {

        //���������� ��� ������ ���������������
        struct node_Hashing *T=(struct node_Hashing*)malloc(11*sizeof(struct node_Hashing));

        //������������ ��� ������ ���������������
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

        //����� ���������� ��� �������� ��� ���������
        //���� ������ ��������������� �� ��������
        for(i=0; i<1405; i++)
        {
            insert_Hash(Data[i].Date,Data[i].T_degC,T);
        }

        //Menu, � ������� �������� ��� ��������
        do
        {
            printf("1)Search a Temperature\n");
            printf("2)Modify a Temperature\n");
            printf("3)Delete a Date\n");
            printf("4)Exit\n");
            printf("Select(Press a number 1-5): ");
            scanf("%c",&check);
            printf("\n\n");

            //������� ��� �� �� ����������
            //������ � ������
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

            //��� � ������� ������� 1
            if(select==1)
            {
                //� ������� ������� ��� ����������
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

                //����� ���������� �� ������ ������������ ����
                //�����������
                search_Temp(T,date);
                select=0;
                //��� � ������� ������� 2
            }
            else if(select==2)
            {
                //� ������� ������� ��� ����������
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
                //����� ���������� ��� ����������� ���� ������������
                //���� �����������
                modify_Temp(T,date);
                select=0;
                //��� � ������� ������� 3
            }
            else if(select==3)
            {

                //� ������� ������� ��� ����������
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
                //����� ���������� ��� �������� ����
                //�������� ��� ��� ������ ���������������
                delete_Date(T,date);
                select=0;
            }

        }
        while(select<1 || select>4);

    }
    else if(error==1)
    {
        //��� ��� ����� ������� ��� ���������
        //������� ���������� ���� ��� ������
        printf("File did not open\nError\n");
    }

    return 0;

}


int data_extraction_hashing(struct ocean_data_hashing *Data)
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
                //���������� ��� ����������� ���� ������ �����
                if(flag_2==0)
                {
                    strcpy(Data[flag_1-1].Date,info);
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

    //�� ������ �������
    fclose(file);

    return 0;
}

void data_print_hashing(struct ocean_data_hashing *Data)
{
    int i,j;

    //�������� ��� ������������ ��� ������ �����
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
    //������ ����������
    int i;
    int hash_key=0;
    int flag=0;
    int flag_1=0;

    //����������� ��� ����������
    //���������������
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //������ ��� ����� ���� ������
    //���������������
    hash_key=hash_key%11;

    //���������� ���� ���������
    struct node_Hashing *new_data=(struct node_Hashing*)malloc(sizeof(struct node_Hashing));

    //��� � ������� ��������������� ���� ���� ��� ������
    //� ��������� ��������������� ��� ������� �� ������ ��������
    if(T[hash_key].next==NULL)
    {
        strcpy(new_data->date,date);
        new_data->T_degC[0]=T_degC;
        new_data->T_degC[1]=0;
        new_data->next=NULL;
        T[hash_key].next=new_data;

    }
    //������ ���������� ��� ��������� ��� ����� ��� ������
    else
    {
        struct node_Hashing *check;
        check=T[hash_key].next;

        while(check!=NULL && flag==0 && flag_1==0)
        {
            //��� ������� � ���������� ����������
            //���� ��� ������������ ��� ��������
            if(strcmp(check->date,date)==0)
            {
                check->T_degC[1]=T_degC;
                flag=1;
            }
            //������ ���������� ��� ���������
            //��� ����� ��� ������
            else if(check->next!=NULL)
            {
                check=check->next;
            }
            else
            {
                flag_1=1;
            }
        }

        //���������� ��� ��������� ��� ���
        //�������� ��� ������
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

    //������� ����������
    int i,flag=0;
    int hash_key=0;

    //����������� ��� ����������
    //���������������
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //������ ��� ����� ���� ������
    //���������������
    hash_key=hash_key%11;

    //���������� ���� ��������� ���
    //��������� ��� ������ ��� ������� �
    //��������� ���������������
    struct node_Hashing *search=T[hash_key].next;

    //������ ��� �����������
    while(search!=NULL && flag==0)
    {
        //�������� ��� ������������
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

    //��� ��� ������ � ����������
    //������������ � �������
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}

void modify_Temp(struct node_Hashing T[11],char date[10])
{

    //�������� ����������
    int i,flag=0;
    int hash_key=0;
    int select=1;
    float temp;

    //����������� ��� ����������
    //���������������
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //������ ��� ����� ���� ������
    //���������������
    hash_key=hash_key%11;

    //���������� ���� ��������� ���
    //��������� ��� ������ ��� ������� �
    //��������� ���������������
    struct node_Hashing *search=T[hash_key].next;

    //������ ��� �����������
    while(search!=NULL && flag==0)
    {
        //�������� ��� ������������
        if(strcmp(search->date,date)==0)
        {
            printf("Date Found: %s\n",search->date);
            printf("1) Temperature: %0.3f\n",search->T_degC[0]);
            //��� �������� ��� ������������ �� ��� ����������
            //� ������� �������� ���� ��� ��� ��� �� �������
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
            //� ������� ������� ��� ��� �����������
            printf("Enter a new value:");
            scanf("%f",&temp);
            fflush(stdin);

            //�������� ��� ������� ���� �����������
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

    //��������� ��� ������ ��� ��� �������
    //� ����������
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}

void delete_Date(struct node_Hashing T[11],char date[10])
{

    //������ ����������
    int i,flag=0;
    int hash_key=0;

    //����������� ��� ����������
    //���������������
    for(i=0; i<10; i++)
    {
        hash_key=hash_key+date[i];
    }

    //������ ��� ����� ���� ������
    //���������������
    hash_key=hash_key%11;


    //���������� ���� ��������� ���
    //��������� ��� ������ ��� ������� �
    //��������� ���������������
    struct node_Hashing *search=T[hash_key].next;

    //���������� ���� ���������
    struct node_Hashing *temp;

    //������ ��� �����������
    while(search!=NULL && flag==0)
    {
        //�������� ��� ��������� ��� ��� �����
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

    //��� ��� ������ � ����������
    //������������ � �������
    if(flag==0)
    {
        printf("Date not found\n\n");
    }

}
