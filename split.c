#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

char **allocsplit(char *src, int del, ...)
{
    int fig = 0, line = 0, delN, delF;
    char **out, *p = src;
    int dels = del, delc[BUFSIZ];

    va_list args;
    //区切り文字格納
    va_start(args, del);
    for (delN = 0; dels != 0; delN++)
    {
        delc[delN] = dels;
        dels = va_arg(args, int); //次の区切り文字
    }
    va_end(args);
    //配列の必要行数確認
    while (*src != '\0') //終端まで見る
    {
        delF = 0;                      //初期化
        for (int i = 0; i < delN; i++) //区切り探し
        {
            if (*src == delc[i])
            {
                delF = 1;
            }
        }
        if (delF == 1) //区切り文字有
        {
            line++;
        }
        *src++; //次の文字
    }
    src = p; //初期化
    //動的確保(行)
    out = (char **)malloc(sizeof(char *) * line);
    if (!out)
    {
        exit(7);
    }
    //動的確保(要素数)
    for (int i = 0; i < line; i++) //改行の文字数分
    {
        for (fig = 0; *src; fig++)
        {
            delF = 0;                      //初期化
            for (int i = 0; i < delN; i++) //区切り探し
            {
                if (*src == delc[i])
                {
                    delF = 1;
                }
            }
            if (delF == 1) //区切り文字有
            {
                break;
            }
            *src++;
        }
        out[i] = (char *)malloc(sizeof(char) * fig);
        if (!out[i])
        {
            exit(9);
        }
        *src++; //del飛ばし
    }
    src = p; //初期化
    //代入
    for (int i = 0, j; i < line; i++)
    {
        for (j = 0; *src; j++)
        {
            delF = 0;                      //初期化
            for (int k = 0; k < delN; k++) //区切り探し
            {
                if (*src == delc[k])
                {
                    delF = 1;
                }
            }
            if (delF == 1) //区切り文字有
            {
                break;
            }
            out[i][j] = *src++;
        }
        src++;            //del飛ばし
        out[i][j] = '\0'; //終端記号
    }
    return out;
}

int split(int n, int w, char **dar, char *src, int del, ...)
{
    int k = 0, l = 0, delN, delF;
    int dels = del, delc[BUFSIZ];
    va_list args;

    //区切り文字格納
    va_start(args, del);
    for (delN = 0; dels != 0; delN++)
    {
        delc[delN] = dels;
        dels = va_arg(args, int); //次の区切り文字
    }
    va_end(args);

    while (l < n && *src)
    {
        k = 0;
        delF = 0;
        while (k < w && *src) //終端まで
        {
            for (int i = 0; i < delN; i++) //区切り文字
            {
                if (*src == delc[i])
                {
                    delF = 1;
                }
            }
            if (delF == 1)
            {
                break;
            }
            dar[l][k] = *src++;
            k++;
        }
        dar[l][k] = '\0'; //終端記号

        if (dar[0])
        {
            l++;
        }
        if (*src)
        {
            src++; //del
        }
    }
    return l;
}

int show(int nf, char *far[])
{
    int i;
    for (i = 0; i < nf; i++)
    {
        printf("%3d: %3ld |%s|\n", i, strlen(far[i]), far[i]);
    }
}

int main(void)
{
#define NF (10)
    char **chunk;
    char **out;
    int i;
    int j;
    int m;
    int ik;
#if 0
    srand(time(NULL));
#endif
    chunk = (char **)malloc(sizeof(char *) * NF);
    if (!chunk)
    {
        exit(3);
    }
    for (i = 0; i < NF; i++)
    {
        chunk[i] = (char *)malloc(sizeof(char) * BUFSIZ);
        if (!chunk[i])
        {
            exit(5);
        }
        m = rand() % 36 + 3;
        for (j = 0; j < m; j++)
        {
            chunk[i][j] = 'A' + (rand() % 26);
        }
        chunk[i][j] = '\0';
    }
    show(NF, chunk);
    ik = split(NF, BUFSIZ, chunk, "ABC DEF GHI-dogse", 'E', ' ', '-', 0);
    printf("ik %d\n", ik);
    show(ik, chunk);
    ik = split(NF, BUFSIZ, chunk, "JKL MNO ", ' ', '-', 0);
    printf("ik %d\n", ik);
    show(ik, chunk);
    ik = split(NF, BUFSIZ, chunk, "DOG CAT\nBIRD\tFOX\nPIG", ' ', 0);
    printf("ik %d\n", ik);
    show(ik, chunk);
    free(chunk);

    out = allocsplit("ABC DEF-GHI-", '-', ' ', 0);
#if 1
    printf("allocsplit\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%3d:|%s|\n", i, out[i]);
    }
#endif
    free(out);
    return 0;
}
