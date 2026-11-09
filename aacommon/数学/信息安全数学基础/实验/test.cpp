#include<bits/stdc++.h>
#include<vector>

using namespace std;

int main()
{
	int n,k,temp,flag;
   vector<int> buff;
   
   cin >> n;
   for (int i = 0; i < n; i++)
   {
      cin >> temp;
      buff.push_back(temp);
   }
   cin >> k;
   
    flag = n % 4;
   
   for (int i = 0; i < n - flag; i++)
   {
   	  if (i % 4 == 0)
     {
        temp = buff[i];
        continue;
     }
     cout << buff[i] << " " ;
     if (i % 4 == 3)
     	 cout << temp << " ";
   }

   for (int i = n-flag; i < n; i ++)
    cout << buff[i]<<" ";
   
   return 0;
}