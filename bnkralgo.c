//Program to implement Banker's algorithm with p processes and r resources.
#include<stdio.h>
int main()
{
int p,r, allocation[100][100], max[100][100], need[100][100], available[100],i,j,k,flag[100],x=0,sequence[100],c=0,count;
printf("Enter the number of process \n");
scanf("%d",&p);
printf("Enter the number of resources \n");
scanf("%d",&r);
for (i=0;i<p;i++)
{
    flag[i]=0;  //none of the process's need is satisfied initially
}
for(i=0;i<p;i++) //loop for process
{
    for(j=0;j<r;j++)  //loop for resources
    {
    printf("Enter the allocation for process P[%d] of resource %d\n",i,j);
    scanf("%d",&allocation[i][j]);
    printf("Enter the max for process P[%d] of resource %d\n",i,j);
    scanf("%d",&max[i][j]);
    }
}
for(i=0;i<r;i++)
{
    printf("Enter the available instances of resource %d\n", i);
    scanf("%d",&available[i]);
}
for(i=0;i<p;i++)
{
    for(j=0;j<r;j++)
    {
    need[i][j]=max[i][j]-allocation[i][j]; //need of each process
    }
}

for(i=0;i<p;i++) //loop to print the calculated need
{
    printf("Need of P[%d] is:",i);
    for(j=0;j<r;j++)
    {
    printf("%d ",need[i][j]);
    }
    printf("\n");
}
for(i=0;i<p;i++) //loop for iteration
{
    for(j=0;j<p;j++) //loop for process
    {
        count=0;
        for(k=0;k<r;k++) //loop for resource
        {
        if(flag[j]==0&&need[j][k]<=available[k])  //need[j][k] means need of jth process's kth resource
        {
            available[k]=available[k]+allocation[j][k];
            count++;
            if(count==r)  //if true means need for all the resources can be satisfied
            {
                flag[j]=1;  // process has been allocated resources
                printf("Process %d has been allocated resources\n",j);
            
                sequence[c]=j;  // array sequence stores the safe sequence. jth process is added to safe sequence
                c++;
            }
        continue;  //if need of one resource is satisfied then we check for next resource
        }
        break;  // if need  for any resource of a process can not be satisfied, break the loop and check the  
                      //need of next process
        }
    }
}
for(i=0;i<p;i++)
{
    if(flag[i]==0)
    {
    printf("system is in unsafe state\n");
    x=1;
    break;
    }
    
}
if(x==0)
{
printf("System is in safe state\n");
printf("Safe sequence is \n");
for(i=0;i<p;i++)
{
    printf("P[%d],",sequence[i]);
}
}
printf("\n");

}
