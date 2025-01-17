#include<bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include "insthandler.h"
#include <fstream>
using namespace std;

int main()
{

    ifstream re("asmcode.txt");
    ofstream we("asm2hex.txt");

    string s;

    while(getline(re,s))
    {
        getoutinhex(s,we);
    }
  
    return 0;
}
