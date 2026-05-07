#include <stdio.h>

int factorial(int n){
    int result=1;
    for(int i=1;i<=n;i++){
        result *= i;        
    }
    return result;
}

int main(){
    for(int i=0;i<13;i++){
        printf("%d\n", factorial(i));
    }
    return 0;
}
