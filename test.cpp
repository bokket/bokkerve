#include<iostream>
#include<string>

using namespace std;

int main() {
    string s="abc";
    for(int i=0;i<s.length();i++)
    {
        switch(s[i])
        {
            //s="abc";
            case 'a':cout<<'a'<<s;
            case 'b':cout<<'b';
            case 'c':cout<<'c';

        }
        cout<<endl;
    }

}