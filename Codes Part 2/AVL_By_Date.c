#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

struct Node //AVL tree node
{
    int date[3];
    float T_degC[2];
    struct Node *left_child;
    struct Node *right_child;
    int height;
};

//��������� ���������� ���� ������ ��
//�������� ��� �� ������ CSV
int data_extraction_AVL(struct ocean_data *Data);

//������� ���������� ��� ��������� ��
//�������� ��� ����� ������� ��� �� ������ CSV
void data_print(struct ocean_data *Data);

//������� ���������� ��� ������ ��� ����������
//��� ��� ��������� ���� ���� ocean_data
void split(struct ocean_data *Data,char **date);

//������� ���������� ��� ������� ��� ���������� �����
//��� ������ AVL
struct Node* insert_AVL_date(struct Node *Node,int date[3],float T_degC);

//������� ���������� ��� ���������� ��� node
struct Node* new_AVL_Node_date(int date[3],float T_degC);

//������� ���������� ��� ���������� ��
//���� ��� ������
int height_AVL(struct Node *node);

//������� ���������� ��� ���������� ���
//��������� ��� ������
int height_difference(struct Node *node);

//������� ����������  ������ �����������
//��� ������
struct Node *rightRotation(struct Node *node);

//������� ���������� ��������� �����������
//��� ������
struct Node *leftRotation(struct Node *node);

//������� ���������� ��� ���������� ��� AVL ��
//����-������������ ��������
void Inorder_Print(struct Node* Data_AVL);

//������� ���������� ��� ��������� ������������ ���
//AVL ������ ���� ��� �����������
void Temperature_Search(struct Node* Data_AVL,int search[3]);

//������� ���������� ��� ����������� ����
//������������ ��� AVL ������ ��� ����������� ��
//������������ ����������
void Temperature_Modify(struct Node* Data_AVL,int search[3]);

//������� ���������� ��� �������� ������ ��� �����������
//�� ������������ ���������� ��� �� AVL ������
struct Node *delete_AVL(struct Node *Data_AVL,int date[3]);

int main()
{

    //�������� ����������
    int error=0,select=0;
    char check;
    int search[3];

    //���������� ������ ����� �� �������� �������� ������
    struct ocean_data* Data= malloc(1405*sizeof *Data);

    //������� ��� ��������� ��� �� ������ CSV
    //��� ���������� ���� ������ ����� Data
    error=data_extraction_AVL(Data);


    if(error!=1)
    {

        struct Node *Data_AVL = NULL;

        //����� ���������� ��� ���������� ��� AVL
        //������� ���������� ���� ���������� ��� ���
        //������ Data
        for(int i=0; i<1405; i++)
        {
            Data_AVL=insert_AVL_date(Data_AVL,Data[i].Date,Data[i].T_degC);
        }

        //Menu, � ������� �������� ��� ��������
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
                //��� �� ������ ��� ����� �����
                if(Data_AVL!=NULL)
                {
                    //����� ���������� ���
                    //���������� ��� AVL
                    Inorder_Print(Data_AVL);
                }
                else
                {
                    //��� �� ������ ����� �����
                    //������������ ��� ������
                    printf("There are no data\n\n");
                }
                select=0;

            }
            //��� � ������� ������� 2,3 � 4
            else if(select==2 || select==3 || select==4)
            {
                if(Data_AVL!=NULL)
                {
                    //� ������� ������� ��� ����������
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

                    //��������� ������������ ���� �����������
                    if(select==2)
                    {
                        //����� ����������
                        Temperature_Search(Data_AVL,search);

                    }
                    //����������� ���� ������������ ��� �����������
                    //�� ������������ ����������
                    else if(select==3)
                    {
                        //����� ����������
                        Temperature_Modify(Data_AVL,search);

                    }
                    //�������� ���� ������ ��� ����������� ��
                    //������������ ����������
                    else if(select==4)
                    {
                        Data_AVL=delete_AVL(Data_AVL,search);
                    }

                    select=0;
                }
                else
                {
                    //��� �� ������ ����� �����
                    //������������ ��� ������
                    printf("There are no data\n\n");
                }
            }
        }
        while(select<1 || select>5);


    }
    else if(error==1)
    {
        //��� ��� ����� ������� ��� ���������
        //������� ���������� ���� ��� ������
        printf("File did not open\nError\n");
    }

    return 0;
}

int data_extraction_AVL(struct ocean_data *Data)
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

struct Node* new_AVL_Node_date(int date[3],float T_degC)
{

    //��������� ����� ��� ��� �����
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    int i;

    //���������� ��� ����� ���� �����
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

struct Node* insert_AVL_date(struct Node* node, int date[3],float T_degC)
{

    //���������� ���� ������ ��� ���������� ���� ����� ����
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
                //��� ������� ������ �� ��� ���� ����������
                //���������� ���� ��� ������������
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

    //����������� ��� ����� ��� ������
    if(height_AVL(node->left_child)>height_AVL(node->right_child))
    {
        node->height=1+height_AVL(node->left_child);
    }
    else
    {
        node->height=1+height_AVL(node->right_child);
    }

    //����������� ��� ����������
    int dif=height_difference(node);


    //����� ����������
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

    //�������� ����������
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

    //�������� ����� ����������
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

    //����� �������� ����������
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

int height_AVL(struct Node* node_AVL)
{

    int height;

    if(node_AVL==NULL)
    {
        height=0;
        return height;

    }

    //���������� �� ����
    //��� ������
    height=node_AVL->height;


    return height;
}

int height_difference(struct Node *node)
{

    int dif;


    if(node==NULL)
    {
        dif=0;
        return dif;
    }
    else
    {
        //������ ��� ����������
        dif=height_AVL(node->left_child)-height_AVL(node->right_child);
    }

    return dif;
}

struct Node *rightRotation(struct Node *node)
{
    //��������  ����������
    struct Node *temp_1=node->left_child;
    struct Node *temp_2=temp_1->right_child;

    //����� ����������
    temp_1->right_child=node;
    node->left_child=temp_2;

    //�������� ��� ����
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

struct Node *leftRotation(struct Node *node)
{
    //��������  ����������
    struct Node *temp_1=node->right_child;
    struct Node *temp_2=temp_1->left_child;

    //�������� ����������
    temp_1->left_child=node;
    node->right_child=temp_2;

    //�������� ��� ����
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

void Inorder_Print(struct Node* Data_AVL)
{

    //�������� ���� AVL ������� �� ����-������������ ��������
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

void Temperature_Search(struct Node *Data_AVL,int search[3])
{

    //��� �� ������ ����� �����
    //������������ ��� ������
    if(Data_AVL==NULL)
    {
        printf("Date not found\n\n");
    }

    //��������� ��� ����������� ��� ������
    if(Data_AVL!=NULL)
    {
        //��������� ��� ����� ���� �� �����
        if(Data_AVL->date[2]<search[2])
        {
            return Temperature_Search(Data_AVL->right_child,search);
        }
        //��������� ��� ���� ���� �� �����
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]<search[0])
            {
                return Temperature_Search(Data_AVL->right_child,search);
            }
            //��������� ��� ����� ���� �� �����
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]<search[1])
                {
                    return Temperature_Search(Data_AVL->right_child,search);
                }
            }
        }

        //��������� ��� ����� ��� �� ��������
        if(Data_AVL->date[2]>search[2])
        {
            return Temperature_Search(Data_AVL->left_child,search);
        }
        //��������� ��� ���� ���� �� ��������
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]>search[0])
            {
                return Temperature_Search(Data_AVL->left_child,search);
            }
            //��������� ��� ����� ���� �� ��������
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]>search[1])
                {
                    return Temperature_Search(Data_AVL->left_child,search);
                }
            }
        }

        //��� ������� � ���������� ������� ��� �����������
        if(Data_AVL->date[0]==search[0] && Data_AVL->date[1]==search[1] && Data_AVL->date[2]==search[2])
        {
            printf("Date Found\n");
            printf("1.Temperature: %0.3f\n",Data_AVL->T_degC[0]);
            //��� �������� ��� ������������ ��� ���� ����
            if(Data_AVL->T_degC[1]!=-1)
            {
                printf("2.Temperature: %0.3f\n",Data_AVL->T_degC[1]);
            }
            printf("\n");
        }
    }

}

void Temperature_Modify(struct Node* Data_AVL,int search[3])
{

    //������ ����������
    float temp;
    int select=1;

    //��� �� ������ ����� �����
    //������������ ��� ������
    if(Data_AVL==NULL)
    {
        printf("Date not found\n\n");
    }

    //��������� ��� ����������� ��� ������
    if(Data_AVL!=NULL)
    {

        //��������� ��� ����� ���� �� �����
        if(Data_AVL->date[2]<search[2])
        {
            return Temperature_Modify(Data_AVL->right_child,search);
        }
        //��������� ��� ���� ���� �� �����
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]<search[0])
            {
                return Temperature_Modify(Data_AVL->right_child,search);
            }
            //��������� ��� ����� ���� �� �����
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]<search[1])
                {
                    return Temperature_Modify(Data_AVL->right_child,search);
                }
            }
        }

        //��������� ��� ����� ��� �� ��������
        if(Data_AVL->date[2]>search[2])
        {
            return Temperature_Modify(Data_AVL->left_child,search);
        }
        //��������� ��� ���� ���� �� ��������
        else if(Data_AVL->date[2]==search[2])
        {
            if(Data_AVL->date[0]>search[0])
            {
                return Temperature_Modify(Data_AVL->left_child,search);
            }
            //��������� ��� ����� ���� �� ��������
            else if(Data_AVL->date[0]==search[0])
            {
                if(Data_AVL->date[1]>search[1])
                {
                    return Temperature_Modify(Data_AVL->left_child,search);
                }
            }
        }

        //��� ������� � ����������� ������� ��� �����������
        if(Data_AVL->date[0]==search[0] && Data_AVL->date[1]==search[1] && Data_AVL->date[2]==search[2])
        {
            printf("Date found\n");
            printf("1)Temperature: %0.3f\n",Data_AVL->T_degC[0]);
            //��� �������� ��� ������������ ��� ���� ����
            //� ������� �������� ��� ����� �� ������������
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

            //� ������� ������� ��� ��� �����������
            printf("Enter a new value:");
            scanf("%f",&temp);
            fflush(stdin);

            //�������� ��� ���� ������������
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

struct Node *delete_AVL(struct Node *Data_AVL,int date[3])
{
    //������� ��� �� ������ ����� �����
    if(Data_AVL==NULL)
    {
        return Data_AVL;
    }

    //��������� ��� ����������� ��� ������

    //��������� ��� ����� ���� �� �����
    if(Data_AVL->date[2]<date[2])
    {
        Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
    }
    //��������� ��� ���� ���� �� �����
    else if(Data_AVL->date[2]==date[2])
    {
        if(Data_AVL->date[0]<date[0])
        {
            Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
        }
        //��������� ��� ����� ���� �� �����
        else if(Data_AVL->date[0]==date[0])
        {
            if(Data_AVL->date[1]<date[1])
            {
                Data_AVL->right_child=delete_AVL(Data_AVL->right_child,date);
            }
        }
    }

    //��������� ��� ����� ��� �� ��������
    if(Data_AVL->date[2]>date[2])
    {
        Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
    }
    //��������� ��� ���� ���� �� ��������
    else if(Data_AVL->date[2]==date[2])
    {
        if(Data_AVL->date[0]>date[0])
        {
            Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
        }
        //��������� ��� ����� ���� �� ��������
        else if(Data_AVL->date[0]==date[0])
        {
            if(Data_AVL->date[1]>date[1])
            {
                Data_AVL->left_child=delete_AVL(Data_AVL->left_child,date);
            }
        }
    }

    //��� ������� � ���������� � ������ �����������
    if(Data_AVL->date[0]==date[0] && Data_AVL->date[1]==date[1] && Data_AVL->date[2]==date[2])
    {

        //��� � ������ ���� ��� � ������ �����
        if(Data_AVL->left_child==NULL || Data_AVL->right_child==NULL)
        {

            struct Node *delete_node;

            if(Data_AVL->left_child==NULL)
            {
                delete_node=Data_AVL->right_child;
            }
            else if(Data_AVL->right_child==NULL)
            {
                delete_node=Data_AVL->left_child;
            }

            //��� � ������ ��� ���� ������
            if(delete_node==NULL)
            {
                delete_node=Data_AVL;
                Data_AVL=NULL;
                free(delete_node); //�������� ������
                printf("Data deleted\n\n");
            }//��� � ������ ���� ��� �����
            else
            {
                *Data_AVL=*delete_node;
                free(delete_node); //�������� ������
                printf("Data deleted\n\n");
            }
        }
        //��� � ������ ���� ��� ������
        else
        {
            struct Node *delete_node;

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

            //�������� ��� ������
            Data_AVL->right_child=delete_AVL(Data_AVL->right_child,delete_node->date);
        }
    }


    if(Data_AVL==NULL)
    {
        return Data_AVL;
    }


    //����������� ��� ����� ��� ������
    if(height_AVL(Data_AVL->left_child)>height_AVL(Data_AVL->right_child))
    {
        Data_AVL->height=1+height_AVL(Data_AVL->left_child);
    }
    else
    {
        Data_AVL->height=1+height_AVL(Data_AVL->right_child);
    }


    //����������� ��� ����������
    int dif=height_difference(Data_AVL);


    //����� ����������
    if(dif>1 && height_difference(Data_AVL->left_child)>=0)
    {
        return rightRotation(Data_AVL);
    }

    //�������� ����������
    if(dif<-1 && height_difference(Data_AVL->right_child)<=0)
    {
        return leftRotation(Data_AVL);
    }

    //�������� ����� ����������
    if(dif>1 && height_difference(Data_AVL->left_child)<0)
    {
        Data_AVL->left_child=leftRotation(Data_AVL->left_child);
        return rightRotation(Data_AVL);
    }

    //����� �������� ����������
    if(dif<-1 && height_difference(Data_AVL->right_child)>0)
    {
        Data_AVL->right_child=rightRotation(Data_AVL->right_child);
        return leftRotation(Data_AVL);
    }

    return Data_AVL;
}
