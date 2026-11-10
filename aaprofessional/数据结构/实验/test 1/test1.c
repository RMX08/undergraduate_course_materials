# include <stdio.h>
# include <stdlib.h>

typedef struct Node
{
    int coef;     //ÏîÊý
    int expn;     //Ö¸Êý
    struct Node *next;
}LNode,*Polynomial;

Polynomial Create_Polyn (int n);//½¨Á¢¶àÏîÊ½

void PrintPolyn (Polynomial p);//Êä³ö¶àÏîÊ½

Polynomial Mul_Polyn(Polynomial pa, Polynomial pb);//¼ÆËã¶àÏîÊ½³Ë»ý

Polynomial Add_Polyn(Polynomial pa, Polynomial pb);//¼ÆËã¶àÏîÊ½Ïà¼Ó


int main()
{
    Polynomial pa,pb,pc;
    int lena,lenb;
    
    printf("Length of Polynomial A = ");
    scanf("%d", &lena);//¶àÏîÊ½AµÄ³¤¶È
    printf("Length of Polynomial B = ");
    scanf("%d", &lenb);//¶àÏîÊ½BµÄ³¤¶È

    printf("Please input coefficients and expnoents of A:");
    pa = Create_Polyn(lena);
    printf("Please input coefficients and expnoents of A:");
    pb = Create_Polyn(lenb);

    printf("Polynomial A = ");
    PrintPolyn(pa);
    printf("Polynomial B = ");
    PrintPolyn(pb);

    pc = Mul_Polyn(pa,pb);

    printf("\nresult:\n");
    PrintPolyn(pc);
        
    return 0;
}

Polynomial Create_Polyn (int n)
{
    Polynomial p,temp = NULL,head = NULL;
    int x,y;

    while(n --)
    {
        scanf("%d %d",&x,&y);
        p = (Polynomial)malloc(sizeof(Polynomial));

        p -> coef = x;
        p -> expn = y;
        p -> next = NULL;

        if (head == NULL)  //å¦‚æžœå½“å‰è¾“å…¥ä¸ºç¬¬ä¸€ä¸ªå…ƒç´?
            head = p;
        else
            temp -> next = p;
        temp = p; //ä¸´æ—¶æŒ‡é’ˆç§»åˆ°è¡¨å°¾
    }
    return head;//è¿”å›žåŸºåœ°å€
}

void PrintPolyn (Polynomial p)
{
    Polynomial temp = (Polynomial)malloc(sizeof(Polynomial));
    temp = p;

    if (temp == NULL)
        printf("0 0");
    
    while (temp)
    {
        printf("%d %d",temp -> coef, temp ->expn);
        if (temp -> next)//ç»“å°¾æ—¶ä¸è¾“å‡ºç©ºæ ¼
            printf(" ");
        temp = temp -> next;
    }

    printf("\n");
}

Polynomial Mul_Polyn(Polynomial pa, Polynomial pb)
{
    Polynomial ta = pa,tb = pb,T1 = NULL,T2 = NULL,temp,p;
    if (!pa || !pb) return NULL;

    while (tb)//å¾ªçŽ¯pbï¼Œä¹˜ä»¥paçš„ç¬¬ä¸€é¡¹ï¼Œå»ºç«‹T1
        {
            int x,y;
            
            x = ta -> coef * tb -> coef;  //æ–°ç³»æ•?
            y = ta -> expn + tb -> expn;  //æ–°æŒ‡æ•?

            p = (Polynomial)malloc(sizeof(Polynomial));
            p -> coef = x;
            p -> expn = y;
            p -> next = NULL;

            if (T1 == NULL) //T1çš„ç¬¬ä¸€é¡¹ï¼Œå°†T1æŒ‡å‘å¤´å…ƒç´?
                T1 = p;
            else
                temp -> next = p;

            temp = p;
            tb = tb -> next;
        }
    ta = ta -> next;//æŒ‡å‘paçš„ç¬¬äºŒé¡¹

    while (ta)//æ¯ç”Ÿæˆä¸€ä¸ªT2ï¼Œå°±å°†T2+T1çš„ç»“æžœå­˜æ”¾åˆ°T1
    {
        p = NULL;
        temp = NULL;
        tb = pb;
        while (tb)//æ¯æ¬¡å»ºç«‹ä¸€ä¸ªæ–°çš„T2
        {
            int x,y;

            x = ta -> coef * tb -> coef;
            y = ta -> expn + tb -> expn;

            p = (Polynomial)malloc(sizeof(Polynomial));
            p -> coef = x;
            p -> expn = y;
            p -> next = NULL;

            if (!T2)
                T2 = p;
            else
                temp -> next = p;

            temp = p;
            tb = tb -> next;
        }
        T1 = Add_Polyn(T1,T2);
        ta = ta -> next;
    }

    return T1;
}

Polynomial Add_Polyn(Polynomial pa, Polynomial pb)//é»˜è®¤é€’å‡
{
    Polynomial pc = NULL,temp = NULL,ta = pa,tb = pb;
    int n=0;

    while (ta && tb)
    {
        if (ta -> expn > tb -> expn)//paçš„æŒ‡æ•°è¾ƒå¤§ï¼Œå°†paçš„å…ƒç´ åŠ å…¥pc
        {
            Polynomial p = (Polynomial)malloc(sizeof(Polynomial));
            p -> coef = ta -> coef;
            p -> expn = ta -> expn;
            p -> next = NULL;

            if (!pc)
                pc = p;
            else
                temp -> next = p;
            temp = p;
            ta = ta -> next;
        }
        else if(ta -> expn < tb -> expn)//pbçš„æŒ‡æ•°è¾ƒå¤?
        {
            Polynomial p = (Polynomial)malloc(sizeof(Polynomial));
            p -> coef = tb -> coef;
            p -> expn = tb -> expn;
            p -> next = NULL;

            if (!pc)
                pc = p;
            else
                temp -> next = p;
            temp = p;
            tb = tb -> next;
        }
        else //ç›¸ç­‰
        {
            Polynomial p = (Polynomial)malloc(sizeof(Polynomial));
            p -> coef = tb -> coef + ta -> coef;
            p -> expn = tb -> expn;
            p -> next = NULL;

            if (p -> coef)//ä¸ç­‰äº?0
            {
                if (!pc)
                    pc = p;
                else
                    temp -> next = p;
                temp = p;
            }
            ta = ta -> next;
            tb = tb -> next;
        }
    }

    if(ta)
    {
        if (!pc)
            pc = ta;
        else
            temp -> next = ta;
    }
    if (tb)
    {
        if (!pc)
            pc = tb;
        else
            temp -> next = tb;
    }

    return pc;
}


