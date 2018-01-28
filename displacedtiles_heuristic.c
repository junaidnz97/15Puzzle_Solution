/*

JUNAID N Z(201601036)
ADITYA HARIDAS MENON(201601002)

*/


#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define UP	  0
#define DOWN  1
#define LEFT  2
#define RIGHT 3
#define N 4

int numnodes=0; 

typedef struct node{
	int mat[4][4];
	int h;
	int g;
	struct node * parent;
}matrix;

typedef struct nd{
	matrix * m;
	struct nd * next;
}pq;

pq *openlist;
pq *closedlist;

typedef struct z{
	int i;
	int j;
}zeroIndex;



pq * addq(pq *Q,matrix *m)
{
	numnodes++;
	pq * temp = (pq*)malloc(sizeof(pq));
	temp->m = m;
	temp->next = NULL;
	pq *head = Q;
	if (head == NULL || (head)->m->h >= m->h)
    {
        temp->next = head;
        head = temp;
    }
    else
    {
        /* Locate the node before the point of insertion */
        pq *current = head;
        while (current->next!=NULL && current->next->m->h < m->h)
        {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;
    }
    return head;
}


void calh(matrix *m)
{
	int i,j;
	m->h = 0;
	for(i = 0 ; i < 4 ; i++)
	{
		for(j = 0 ; j<4 ; j++)
		{
			if(m->mat[i][j] != 4*i + j + 1 && m->mat[i][j] != 0)
			{
				m->h++;
			}
		}
	}
	
}


matrix *assignVals(int a[][4])
{
	matrix *t = (matrix*)malloc(sizeof(matrix)); 
	int i,j;
	for(i = 0 ; i < 4; i++)
	{
		for(j = 0 ; j < 4 ; j++)
		{
			t->mat[i][j] = a[i][j];
		}
	}
	
	t->parent = NULL;
	calh(t);
	return t;
}



int isGoalstate(matrix *m)
{
	int i,j;
	for(i = 0 ; i < 4 ; i++)
	{
		for(j = 0 ; j<4 ; j++)
		{
			if(m->mat[i][j] != 4*i+j+1 && m->mat[i][j] != 0)
			{
				return 0;
			}
		}
	}
	return 1;
}
zeroIndex getZero(matrix *m)
{
	zeroIndex z;
	int i,j;
	for(i = 0 ; i < 4 ; i++)
	{
		for(j = 0 ; j<4 ; j++)
		{
			if(m->mat[i][j] == 0)
			{
				z.i = i;
				z.j = j;
			}
		}
	}

	return z;
}
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
matrix * findNextState(matrix *M, zeroIndex z, int state)
{
	matrix *m = assignVals(M->mat);
	switch(state)
	{
		case UP 	: 	swap(&m->mat[z.i][z.j],&m->mat[z.i-1][z.j]);
								break;
		case DOWN 	: 	swap(&m->mat[z.i][z.j],&m->mat[z.i+1][z.j]);
								break;
		case LEFT 	: 	swap(&m->mat[z.i][z.j],&m->mat[z.i][z.j-1]);
								break;
		case RIGHT : 	swap(&m->mat[z.i][z.j],&m->mat[z.i][z.j+1]);
								break;

	}
	calh(m);
	m->parent=M;
	return m;
}

void printlist(pq *list)
{
	pq *temp=list;
	//printf("hello\n");
	while(temp!=NULL)
	{
		printf("%d ",temp->m->h);
		temp=temp->next;
	}
	printf("\n");
}

int equateMatrix(matrix *a, matrix *b)
{
	int i,j;
	for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(a->mat[i][j]!=b->mat[i][j])
				{
					return 0;
				}
			}
		}
	return 1;
}

int ispresentcl(matrix *m)
{
	//printf("hi\n");
	pq *temp=closedlist,*prev = closedlist;

	if(closedlist==NULL)
		return 1;
	//printf("test1\n");
	int i,j;
	if(equateMatrix(m,temp->m))
	{
		if(temp->m->h > m->h)
		{
			pq *nodefree=closedlist;	
			closedlist=closedlist->next;
			free(nodefree);
			return 1;
		}
		return 0;
	}
	temp=temp->next;
	int flag=0;
	while(temp!=NULL)
	{
		//printf("test2\n");
		if(equateMatrix(m,temp->m))
		{
			if(temp->m->h > m->h)
			{	
				pq *nodefree=temp;
				prev->next = temp->next;
				free(nodefree);
				return 1;
			}
			return 0;
		}
		prev = temp;
		temp=temp->next;
	}
	return 1;		
}

void printSuccess(matrix *m)
{
	printf("success\n");
}

void printmatrix(matrix *m)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%d ",m->mat[i][j] );
		}
		printf("\n");
	}
	printf("\n\n");
}

matrix *getNewStates(matrix *m)
{
	int count=0;
	while(1)
	{
	
	count++;
	//printmatrix(m);
	//printf("%d\n",count );
	zeroIndex z= getZero(m);
	

	if(z.i > 0)
	{
		//printf("hey1\n");
		matrix *M = findNextState(m,z,UP);

		if(isGoalstate(M))
		{
			printSuccess(M);
			printmatrix(M);
			return M;
		}
		//printf("1\n");
		//printf("%d\n",m->h );
		if(ispresentcl(M))
		{
			M->parent=m;
			openlist=addq(openlist,M);
		}
		
	}
	if(z.i < 3)
	{
		//printf("hey2\n");
		matrix *M = findNextState(m,z,DOWN);
		if(isGoalstate(M))
		{
			printSuccess(M);
			printmatrix(M);
			return M;
		}
		//printf("2\n");
		//printf("%d\n",m->h );
		if(ispresentcl(M))
		{
			M->parent=m;
			openlist=addq(openlist,M);
		}
		
	}
	if(z.j > 0)
	{
		//printf("hey3\n");
		matrix *M = findNextState(m,z,LEFT);
		if(isGoalstate(M))
		{
			printSuccess(M);
			printmatrix(M);
			return M;
		}
		//printf("3\n");
		//printf("%d\n",m->h );
		if(ispresentcl(M))
		{	
			M->parent=m;
			openlist=addq(openlist,M);
		}
		
	}
	if(z.j < 3)
	{
		//printf("hey4\n");
		matrix *M = findNextState(m,z,RIGHT);
		if(isGoalstate(M))
		{
			printSuccess(M);
			printmatrix(M);
			return M;
		}
		//printf("4\n");
		//printf("%d\n",m->h );
		if(ispresentcl(M))
		{
			openlist=addq(openlist,M);
			M->parent=m;
		}
	
	}
	closedlist=addq(closedlist,openlist->m);
	m=openlist->m;
	openlist=openlist->next;
	}


}
int count;
int printpath(matrix *temp)
{
	if(temp!=NULL)
	{
	printpath(temp->parent);
	printmatrix(temp);
	count++;
	}
	return 0;
}

int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
       	    if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}
int findXPosition(int puzzle[][4])
{
    for (int i =  3; i >= 0; i--)
        for (int j = 3; j >= 0; j--)
            if (puzzle[i][j] == 0)
                return 4 - i;
    return 0;
}
bool isSolvable(int puzzle[][4])
{
    int invCount = getInvCount((int*)puzzle);
    
    int pos = findXPosition(puzzle);
    if (pos & 1)
        return !(invCount & 1);
    else
        return invCount & 1;
    
}

void takeinput()
{
	 	//int a[4][4]={{0,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,1}};
		//int a[4][4]={{3,9,1,15},{14,11,4,6},{13,0,10,12},{2,7,8,5}};
		//int a[4][4]={{1,2,3,4},{5,6,7,8},{13,10,11,12},{9,15,14,0}};
		//int a[4][4]={{1,2,7,3},{5,6,0,4},{9,10,11,8},{13,14,15,12}};

		int a[4][4]={{5,1,7,3},{9,2,11,4},{13,6,15,8},{0,10,14,12}};

		matrix *m=assignVals(a);
		int i,j,k;
		printf("The given matrix\n");
		printmatrix(m);
		if(!isSolvable(m->mat))
		{	
			printf("not Solvable\n" );
			return;
		}
		printf("Solvable\n\n");

		openlist=NULL;
		closedlist=addq(closedlist,m);
		matrix *temp=getNewStates(m);
		printpath(temp);

		printf("number of nodes = %d\n",numnodes );
		pq *closetemp=closedlist;
		pq *opentemp=openlist;

		int ncl=0;
		while(closetemp!=NULL)
		{
			ncl++;
			closetemp=closetemp->next;
		}
		while(opentemp!=NULL)
		{
			ncl++;
			opentemp=opentemp->next;
		}
		printf("number of nodes in left at end = %d\n",ncl );

		
		//printf("%d \n",count);		
}
int main()
{
   

	takeinput();

}