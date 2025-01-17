#include<bits/stdc++.h>
#include <stdio.h>
#include "string.h"
using namespace std;


ofstream *we;
vector<char>v{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

string dtoh(int n)
{
    string s = "";
    if(n==0)
        return "0";
    while(n!=0)
    {
        s+=v[n%16];
        n = n/16;
    }
    reverse(s.begin(),s.end());
    return s;
}

void removesp(string s,int &ix)
{
    while(ix<s.size() && s[ix]==' '){
        ix++;
    }
}

void fun2(string s,int ix)
{
    ix++;
    removesp(s,ix);
    string yy = string(s.begin(),s.begin()+ix-1);
  
    if(s[ix]=='R')
    {
        ix++;
        removesp(s,ix);
        int n = 0;
        removesp(s,ix);
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n = n*10 + (s[ix]-'0');
            ix++;
        }
        if(yy=="PUSH")
            *we<<"0"<<dtoh(n)<<"000";
        else
            *we<<dtoh(n)<<"0000";
    }
    else
    {
        removesp(s,ix);
        string n = "000";

        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n+=s[ix];
            ix++;
        }
        *we<<n;
    }

}


void fun1(string s,int ix)
{   
    string op = string(s.begin(),s.begin()+ix);

    int a[2]={0},b[2]={0};
    removesp(s,ix);
    ix++;
    removesp(s,ix);
    string tem = "";
    string s1 = "";

    if(s[ix]=='R')
    {
        ix++;
        a[0] = 1;
        removesp(s,ix);
        int n = 0;
        removesp(s,ix);
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n = n*10 + (s[ix]-'0');
            ix++;
        }

        s1 = dtoh(n);
        if(op!="STOREI")
            *we<<s1;
    }
    else
    {
        removesp(s,ix);
        a[1] = 1;
        string n = "";
        
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n+=s[ix];
            ix++;
        }
        tem = n;
    }

    ix++;
    removesp(s,ix);
    string s2 = "";

    if(s[ix]=='R')
    {
        ix++;
        b[0] = 1;
        removesp(s,ix);
        int n = 0;
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n=n*10 + (s[ix]-'0');
            ix++;
        }
        if(op!="STOREI")
        {
            if(a[1])
                *we<<"0";

            *we<<dtoh(n);
            if(a[1]){
                *we<<"0"<<tem;
            }
        }
        else
            s2 = dtoh(n);
    }
    else
    {
        string n = "00";
        b[1] = 1; 

        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n+=s[ix];
            ix++;
        }
        *we<<n;
    }
    if(op!="STOREI")
    {
        if(a[0] && b[0])
            *we<<"000";
    }
    else
    {
        *we<<"0"<<s1<<s2<<"00";
    }

}



void fun(string s,int ix)
{
    ix++;
    while(ix<s.size() && s[ix]==' '){
        ix++;
    }
    ix+=1;
    int n = 0;
    while(ix<s.size() && s[ix]!=',')
    {
        n = n*10+(s[ix]-'0');
        ix++;
    }

    *we<<dtoh(n);
    ix+=1;
    while(ix<s.size() && s[ix]==' '){
        ix++;
    }

    n = 0;
    ix++;
    while(ix<s.size() && s[ix]!=',')
    {
        n = n*10+(s[ix]-'0');
        ix++;
    }

    *we<<dtoh(n);
    ix+=1;
    while(ix<s.size() && s[ix]==' '){
        ix++;
    }



    if(s[ix]=='R')
    {
        ix++;   
        n = 0;
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n = n*10+(s[ix]-'0');
            ix++;
        }

        *we<<dtoh(n);
        *we<<"00";        
    }
    else
    {
        
        string n1 = "0";
        while(ix<s.size() && s[ix]!=',' && s[ix]!=';')
        {
            n1+=s[ix];
            ix++;
        }
        *we<<n1<<" ";
    }


}


void getoutinhex(string s,ofstream &wee)
{
 
    we = &wee;
    string op = "";
    int ix = 0;
    while(ix<s.size() && s[ix]!=',' && s[ix]!=';'){
      op+=s[ix];
      ix++;
    }
    if(op=="NOP")
    {
      *we<<"0000000"<<endl;
    }
    else if(op=="AND")
    {
      *we<<"01";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="OR")
    {
      *we<<"02";
      fun(s,ix);
      *we<<endl;
    }
    else if(op=="EXOR")
    {
      *we<<"03";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="ADD")
    {
      *we<<"04";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="ANDI")
    {
      *we<<"05";

      fun(s,ix);
      *we<<endl;
    } 
    else if(op=="ORI")
    {
      *we<<"06";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="EXORI")
    {
      *we<<"07";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="ADDI")
    {
      *we<<"08";

      fun(s,ix);
      *we<<endl;
    }
    else if(op=="MOV")
    {
        *we<<"09";
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="MOVI")
    {
        *we<<"0";
        *we<<dtoh(10);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="LOAD")
    {
        *we<<"0";
        *we<<dtoh(11);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="STORE")
    {
        *we<<"0";
        *we<<dtoh(12);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="JMP")
    {
        *we<<"0";
        *we<<dtoh(13);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="JMPZ")
    {
        *we<<"0";
        *we<<dtoh(14);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="JMPNZ")
    {
        *we<<"0";
        *we<<dtoh(15);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="JMPC")
    {
        *we<<dtoh(16);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="JMPNC")
    {
        *we<<dtoh(17);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="PUSH")
    {
        *we<<dtoh(18);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="POP")
    {
        *we<<dtoh(19);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="IN")
    {
        *we<<dtoh(20);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="OUT")
    {
        *we<<dtoh(21);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="LOADI")
    {
        *we<<dtoh(22);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="STOREI")
    {
        *we<<dtoh(23);
        fun1(s,ix);
        *we<<endl;
    }
    else if(op=="SUB")
    {
        *we<<dtoh(24);
        fun(s,ix);
        *we<<endl;
    }
    else if(op=="SHIFTR")
    {
        *we<<dtoh(25);
        fun(s,ix);
        *we<<endl;
    }
    else if(op=="SHIFTL")
    {
        *we<<dtoh(26);
        fun(s,ix);
        *we<<endl;
    }
    else if(op=="JMPPCRZ")
    {
        *we<<dtoh(27);
        fun2(s,ix);
        *we<<endl;
    }
    else if(op=="JMPPCRNZ")
    {
        *we<<dtoh(28);
        fun2(s,ix);
        *we<<endl;
    }
    else
    {
        *we<<"ERROR"<<endl;
    }
}