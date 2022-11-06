#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char data[] = "CSQ: 21,0";

int main()
{
    char res[10] = "";
    int j = 0;
    for(int i = 0; i < strnlen(data, sizeof(data)); ++i)
    {
        if (data[i] == ',') 
        {
            break;
        }
        if (data[i] >= 48 && data[i] <= 57)
        {
            res[j] += data[i];
            j++;
        }
    }
    printf("%s\n", res);

    int result = atoi(res);
    printf("%d hehe\n", result);
}
