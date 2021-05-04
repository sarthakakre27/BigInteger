#include <stdio.h>
#include<string.h>

#define	SIZE 320		// maximum length int_big 

typedef struct bigint_tag
{
    int NumberSign;			// 1 if positive, -1 if negative
    int MSBindex;			// index of MS digit 
	char numbers[SIZE];         // representing the number
} int_big;

void sum(int_big *n1, int_big *n2, int_big *n3);
void minus(int_big *n1, int_big *n2, int_big *n3);
int cmp(int_big *n1, int_big *n2);
void multiply(int_big *n1, int_big *n2, int_big *n3);
void take_bigint(int_big* a);
void print(int_big *a);
void initialize(int_big *a);


void take_bigint(int_big* a)
{
	int choice,i,j;
	char num[320];
	num[0] = '\0';
	printf("If the number is positive press 0 if negative press 1\n");
	scanf("%d",&choice);
	if(choice == 0)
	{
		a->NumberSign = 1;
	}
	else
    {
        a->NumberSign = -1;
    }

	for (i=0; i<SIZE; i++) 
    {
        a->numbers[i] =  0;
    }
    
	a->MSBindex = 0;

	printf("Enter the number\n");
	scanf("%s",num);
	a->MSBindex = strlen(num) - 1;
	j=0;
	for(i=a->MSBindex;i>=0;i--)
	{
		a->numbers[j] =  (num[i] - '0');//Subtraction of '0' to get corresponding integer to work upon
		j++;
	}
	
}

void print(int_big *a)
{
	int j;
	char ch;

	if (a->NumberSign == -1)
    {
        printf("- ");
    }
	j = a->MSBindex;
	while(j>=0)
    {
		ch = a->numbers[j] + '0';//Addition of '0' to get correct character
        printf("%c",ch);
		j--;
    }

	printf("\n");
}


void initialize(int_big *a)
{
	int j = 0;
	while(j<SIZE) //initializing all characters to 0 and MSBindex to 0
    {
        a->numbers[j] =  0;
		j++;
    }
	a->NumberSign = 1;
	a->MSBindex = 0;
}


int cmp(int_big *n1, int_big *n2)//returns 1 if n2 is greater and -1 if not 
{
    int ret_val = 0;				
    int flag = 0;

	if ((n1->NumberSign == 1) && (n2->NumberSign == -1))//n1 is greater
    {
        ret_val = -1;
    }
	else if ((n1->NumberSign == -1) && (n2->NumberSign == 1))//n2 is greater
    {
        ret_val = 1;
	}

	else if (n1->MSBindex > n2->MSBindex)
    {
        ret_val = -1 * n1->NumberSign;
    }

	else if (n2->MSBindex > n1->MSBindex)//same sign bit case- decided by 1 of MSBindex--
    {
        ret_val = 1 * n1->NumberSign;
    }
	else//same MSBindex dispute-
	{
		int i = n1->MSBindex;
		while( i>=0 && flag==0 )//while moving towards LSB whenever greater digit found--dispute resolved
    	{
			if(n1->numbers[i] > n2->numbers[i])
        	{
           		ret_val = -1 * n1->NumberSign;
           	 	flag = 1;
       		}
        	if(n2->numbers[i] > n1->numbers[i])
        	{
          	 	ret_val = 1 * n1->NumberSign;
          	  	flag = 1;
       		}
			i--;
		}
	}
	return ret_val;
}

void multiply(int_big *n1, int_big *n2, int_big *n3)
{
	int_big tempResult,Result,Result1;
	int i,j,k;
    int carry,temp,val;			
    initialize(&Result);
	initialize(&Result1);
	initialize(n3);

	for (i=0; i<=n2->MSBindex; i++) //for every digit (LS to MS) of n2
    {
        carry = 0;
        initialize(&tempResult);
        tempResult.MSBindex = 319;
		for (j=0; j<=n1->MSBindex; j++)
        {
			temp = ((n2->numbers[i])*(n1->numbers[j])) + carry;
            carry = temp/10;
            val = temp%10;
            tempResult.numbers[j] = val;
		}
        if(carry>0)
        {
            tempResult.numbers[j] = carry;
        }
		while ((tempResult.MSBindex > 0) && (tempResult.numbers[ tempResult.MSBindex ] == 0))//while element at current 1 is 0 move to LSB.
		{
        	tempResult.MSBindex --;
    	}

    	if ((tempResult.MSBindex == 0) && (tempResult.numbers[0] == 0))
    	{
        	tempResult.NumberSign = 1;	// to avoid -0 case
    	}
        // multiply by 10^i
		if (!((tempResult.MSBindex == 0) && (tempResult.numbers[0] == 0)))
    	{
        	for (k=tempResult.MSBindex; k>=0; k--)
        	{
           		tempResult.numbers[k + i] = tempResult.numbers[k];//shiftng exp numbers to the left(towards MSB) -- in Array towards right
        	}

        	for (k=0; k<i; k++)
        	{
            	tempResult.numbers[k] = 0;
        	}

        	tempResult.MSBindex = tempResult.MSBindex + i;//adjusting MSBindex accordingly
    	}

		Result = Result1;
        sum(&Result,&tempResult,&Result1);
        *n3 = Result1;
	}

	while ((n3->MSBindex > 0) && (n3->numbers[ n3->MSBindex ] == 0))//while element at current 1 is 0 move to LSB.
	{
        n3->MSBindex --;
    }

    if ((n3->MSBindex == 0) && (n3->numbers[0] == 0))
    {
        n3->NumberSign = 1;	// to avoid -0 case
    }
	n3->NumberSign = n1->NumberSign * n2->NumberSign;

}

void minus(int_big *n1, int_big *n2, int_big *n3)
{
	initialize(n3);
	int borrow;			
	int val;			
	int i;			

	if(cmp(n1,n2) == 1)
    {
		minus(n2,n1,n3);
		n3->NumberSign = -1;
	}

	else if((n1->NumberSign == -1) || (n2->NumberSign == -1))// (-n1)-n2 or n1-(-n2)
    {
        n2->NumberSign = -1 * n2->NumberSign;
        sum(n1,n2,n3);
        n2->NumberSign = -1 * n2->NumberSign;
    }
    
    else
    {
		n3->MSBindex = (n1->MSBindex >= n2->MSBindex) ? n1->MSBindex : n2->MSBindex;
        borrow = 0;
		i = 0;
        while(i <= n3->MSBindex)
        {
		    val = (n1->numbers[i] - n2->numbers[i] - borrow);
		    if(val >= 0)
			{
                borrow = 0;
            }
            if(val < 0)
            {
			    val = val + 10;
			    borrow = 1;
		    }

            n3->numbers[i] =  val;
			i++;
        }

		while((n3->MSBindex > 0) && (n3->numbers[ n3->MSBindex ] == 0))//while element at current 1 is 0 move to LSB.
		{
        	n3->MSBindex --;
    	}

    	if((n3->MSBindex == 0) && (n3->numbers[0] == 0))
    	{
        	n3->NumberSign = 1;	// to avoid -0 case
    	}
    }    
}


void sum(int_big *n1, int_big *n2, int_big *n3)
{		
	initialize(n3);	
	int j;		
	int carry;
	int temp;		

	if(n1->NumberSign != n2->NumberSign) 
    {
		if(n1->NumberSign == 1)//-n2+n1 == n1-n2
        {
            n2->NumberSign = 1;
            minus(n1,n2,n3);
            n2->NumberSign = -1;
		}
		if(n1->NumberSign == -1)//-n1+n2 == n2-n1
        {
			n1->NumberSign = 1;
			minus(n2,n1,n3);
			n1->NumberSign = -1;
		}
	}

	else if (n1->NumberSign == n2->NumberSign)
    {

		n3->MSBindex = (n1->MSBindex > n2->MSBindex) ? (n1->MSBindex + 1) : (n2->MSBindex + 1);//setting to max+1 in case of carry
	    carry = 0;

		j = 0;
	    while(j <= n3->MSBindex)
        {
			temp = (n1->numbers[j] + n2->numbers[j] + carry);
		    n3->numbers[j] =  temp % 10;
		    carry = temp / 10;
			j++;
	    }

	    //adjusting MSBindex if no carry is at MSB
		while((n3->MSBindex > 0) && (n3->numbers[ n3->MSBindex ] == 0))//while element at current 1 is 0 move to LSB.
		{
        	n3->MSBindex --;
    	}

    	if((n3->MSBindex == 0) && (n3->numbers[0] == 0))
    	{
       		n3->NumberSign = 1;	// to avoid -0 case
    	}
		n3->NumberSign = n1->NumberSign;//assigning result sign to that of n1 or n2
    }
}


int main()
{
	int_big i,j,k;

	take_bigint(&i);
	take_bigint(&j);

	multiply(&i,&j,&k);
	printf("\nmultiplication of the two bigintegers is equal to ---\n\n ");
    print(&k);

	minus(&i,&j,&k);
	printf("\nsubtraction of two bigintegers is equal to ---\n\n  ");
	print(&k);

	sum(&i,&j,&k);
	printf("\naddition of two bigintegers is equal to ---\n\n ");
	print(&k);
    return 0;
}
