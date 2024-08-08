#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct _line
{
    char *buf;
    struct _line *prev;
    struct _line *next;
};
typedef struct _line line;
line *head, *tail;
int total,now;
char filename[256];
void init_line(void)
{
    head=(line *)malloc(sizeof(line));
    tail=(line *)malloc(sizeof(line));
    head->next=tail;
    head->prev=head;
    tail->next=tail;
    tail->prev=head;
}
void free_all(void)
{
line *p, *s;
p=head->next;
    while(p!=tail)
    {
        s=p;
        p=p->next;
        free(s);
    }
    head->next=tail;
    tail->prev=head;
}
void load_file(void)
{
    FILE *fp;
    char buf[256];
    line *t;
    if((fp=fopen(filename,"rt"))==NULL)
    {
        perror("\nCANNOT READ");
        exit(1);
    }
    total=0;
    while(!feof(fp))
    {
        fgets(buf,255,fp);
        if((char *)(t=(line *)malloc(sizeof(line)))==NULL)
        {
            perror("\nOUT OF MEMORY");
            exit(1);
        }
    if((t->buf=(char *)malloc(strlen(buf)))==NULL)
        {
            perror("\nOUT OF MEMORY");
            exit(1);
        }
    strcpy(t->buf,buf);
    t->prev=tail->prev;
    t->next=tail;
    t->next=tail;
    tail->prev->next=t;
    tail->prev=t;
    total++;
    }
    fclose(fp);
}
void show_page(line *t)
{
    int i=0;
    system("clear");
    while((i++<23)&(t!=tail))
    {
        printf("%-50s\n",t->buf);
        t=t->next;
    }
}
void move_line(int d, line **t)
{
    if(d<0)
        while(d++!=0&&(*t)->prev!=head)
        {
                *t=(*t)->prev;
                now--;
        }
    else
        while(d--!=0&&(*t)->next!=tail)
        {
            *t=(*t)->next;
            now++;
        }
}
void key_proc(void)
{
    char a,flag=0;
    line *t;
    t=head->next;
    now=1;
    system("stty cooked");
    show_page(t);
    while(1)
    {
        char a[4],x,flag=0;
        for(x=0;x<3;x++)
        {
            system("stty raw");
            a[x]=getchar();
            system("stty cooked");
            if(a[x]=='q')
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
            break;
        system("stty cooked");
        if(a[0]==27 & a[1]==91)
            switch(a[2])
            {
                case 65:
                    move_line(-1,&t);
                    break;
                case 66:
                    move_line(1,&t);
                    break;
                case 50:
                    break;
            }
        if(a[2]=='q')
            break;
        system("clear");
        show_page(t);
    }
}
int main(int argc, char **argv)
{
    int i;
    if(argc<2)
    {
        printf("\nUSAGE : txtv <filenames>");
       system("stty cooked");
        exit(0);
    }
    init_line();
    for(i=1;i<argc;i++)
    {
    strcpy(filename,argv[i]);
    load_file();
    key_proc();
    free_all();
    system("clear");
    }
    exit(0);
}
