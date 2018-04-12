#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
struct Process {
	int time,Atime,Btime,id;
	clock_t arrival;
	int flag,completed,p;
	sem_t se;
	struct Process *next;
};
int i=0,k=0;
typedef struct Process node;
clock_t start;
float TAT=0,WT=0;
node *sp1=NULL,*sp2=NULL,*temp;
void *processor(node *S) {
	
	clock_t count;
	while(1) {
		sem_wait(&S->se);
		if((S->Atime<=(clock()-start)/CLOCKS_PER_SEC && S->p==1)) {
			S->p=0;
			count=clock();
		}
		if(S->flag==1) {
			printf("\nProcess-%d Running \nTimer :%d",S->id,(clock()-start)/CLOCKS_PER_SEC);
			S->flag=0;
			S->arrival=clock();
		}
		if((clock()-count)/CLOCKS_PER_SEC==1) {
			count=clock();
			printf("\nTimer :%d",(clock()-start)/CLOCKS_PER_SEC);
			S->time-=1;
			if(S->time==0) {
				TAT+=(clock()-start)/CLOCKS_PER_SEC-S->Atime;
				WT+=((clock()-start)/CLOCKS_PER_SEC)-S->Btime-S->Atime;
				sleep(2);
				node *start=sp2;
				while(start!=NULL) {
					if(start->next==S) {
						start->next=S->next;
						break;
					}
					if(sp2==S) {
						sp2=sp2->next;
						break;
					}
					start=start->next;
				}
				printf("\nProcess-%d Completed ",S->id);
				if(sp2!=NULL){
					printf("next Process-%d",sp2->id);
				}
				S->completed=7;
				if(sp2!=NULL){
					sem_post(&sp2->se);
				}
			}
			}
					if(S->completed==7) {
			break;
		}
	sem_post(&S->se);
	}
} 
void sort_burst(node *temp) {	//sort by burst time
	node *start=sp2;
	if(sp2==NULL) {
		sp2=temp;
		sp2->next=NULL;
	}
	else{
	int t=temp->time;
	if (start->time > t) {
        	temp->next = sp2;
        	sp2=temp;
    	}
    	else {
        	while (start->next != NULL && start->next->time< t) {
            		start = start->next;
        	}
        temp->next = start->next;
        start->next = temp;
    }
	}
}
void sort_arrival() {
	temp=(node *)malloc(sizeof(node));
	printf("\nEnter Arrival Time of %d Process :",(i+1));
	scanf("%d",&temp->Atime);
	printf("Enter Burst Time :");
	scanf("%d",&temp->time);
	temp->id=i+1;
	temp->p=1;
	temp->flag=1;
	temp->Btime=temp->time;
	sem_init(&temp->se,0,0);
	int t=temp->Atime;
	node *start=sp1;
	if (start->Atime > t) {
        	temp->next = sp1;
        	sp1=temp;
    	}
    	else {
        	while (start->next != NULL && start->next->Atime < t) {
            		start = start->next;
        	}
        temp->next = start->next;
        start->next = temp;
    }
}
void main() {
	printf("\t\t\t\t\t\t\t\t\t\t\t*********Shortest Job First**********\n");
	int n,l=1;
	pthread_t p[10];
	printf("\nEnter No.of Processes :");
	scanf("%d",&n);
	while(i<n) {
		if(sp1==NULL) {
			sp1=(node *)malloc(sizeof(node));
			printf("Enter Arrival Time of %d Process :",(i+1));
			scanf("%d",&sp1->Atime);
			printf("Enter Burst Time :");
			scanf("%d",&sp1->time);
			sp1->id=i+1;
			sp1->flag=1;
			sp1->p=1;
			sp1->Btime=sp1->time;
			sem_init(&sp1->se,0,0);
			sp1->next=NULL;
		}
		else {
			sort_arrival();
		}
		i++;
	}
	i=0;
	system("cls");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t\**********PROCESSOR************\n\n");
	start=clock();
	while(i<n) {
		temp=sp1;	
		if(temp->Atime<=0) {
			printf("Process-%d is Dicarded Due to Incorrect Arrival Time\n",temp->id);
			sp1=temp->next;
			temp=sp1;
			i++;
		}
		if(l==1) {
				l=0;
				sem_post(&temp->se);
			}
		if((clock()-start)/CLOCKS_PER_SEC==temp->Atime) {
			printf("Process-%d is created\n",temp->id);
			pthread_create(&p[i],NULL,processor,temp);
			sp1=sp1->next;
			sort_burst(temp);
			i++;
		}
	}
	for(i=0;i<n;i++) {
		pthread_join(p[i],NULL);
	}
	printf("\nAverage Waiting Time :%f\nAverage Turn Around Time :%f",(float)WT/n,(float)TAT/n);
}

