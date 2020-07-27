#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>  
#include <fstream>
#include <iomanip>

using namespace std;

ifstream inp; 
ofstream outp;

char c[30][21]; 
int n[30][21]; 
int highscore;
int contr,tuk=0,score=0,t=0,bt=0,birdx=0,birdy=0;
bool err; 

void game();  
void screen();
void pipes();
void bird();
bool gameover();
void checkscore();
void help();
void menu();
void endgame();
void credits();

int main()
{
srand(time(0)); 
inp.open("/Program Files/FlappyBird/options.txt");  
if(inp.is_open())
{
    inp>>highscore;
    inp.close();
    err=false;
}
else
{
    highscore=0; 
    err=true;
}

int a=0,b;
char sl; 
while(1) 
{
    if(a==0) goto play;
    if(a>0)              
    {
        score=0;
        cout<<"Do you want to play again? [y/n] ";
        cin>>sl;
        if(sl=='n') goto quit;
        else goto play;
    }
    play:
    menu(); //calling menu function
    cin>>sl;
    switch(sl) //menu selections
    {
        case '1':
        {
            game(); //if you choose play, it calls function game
            break;
        }
        case '2': //other selections-other functions
        {
            help();
            goto play;
            break;
        }
        case '3':
        {
            credits();
            goto play;
            break;
        }
        case '4':
        {
            goto quit; //exits game
            break;
        }
        default:
        {
            goto play;
            break;
        }
    }
    a++; //variable for checking how many times you've played
}
quit:
{
   cout<<"I quit."; //stops game, app closes.
}

return 0;
}

void game()  //function for playing game
{
    int x,y;
    char s;
    for(y=0;y<21;y++)  //setting screen
    {
        for(x=0;x<30;x++)
        {
            if(y<20)
            {
            c[x][y]=' ';
            n[x][y]=0;
            }
            if(y==20)
            {
                c[x][y]='-';
                n[x][y]=2;
            }
        }
    }
    c[10][10]='*';  
    screen();      
    while(1)       
    {
        s='~'; 
        Sleep(0.2*1000);  
        t++;              
        if(kbhit()) 
        {
            s=getch();        
            if(s!='~') tuk=1; 
        }
        for(x=0;x<30;x++) 
        {
            c[x][20]='-';
            n[x][20]=2;
        }
        bird();                       
        checkscore();                 
        if(gameover()==true) goto gameEnd;   
        pipes();                             
        if(score>highscore) highscore=score;  
        screen();                           

        if(tuk>0) tuk++;           
        if(tuk==3) tuk=0;          
    }
    gameEnd:   //ends game
    {
        if(score>highscore) highscore=score;
        if(err==false)              //if hi-score file exists, it writes your new highscore there.
        {
            outp.open("/Program Files/FlappyBird/options.txt");
            outp<<highscore;
            outp.close();
        }
        screen();    //shows ending screen, and returns to int main
        endgame();
        return;
    }
}

void screen()    //func for showing screen
{
    int x,y;
    system("cls");    //clears console
    for(y=0;y<21;y++) 
    {
        for(x=0;x<30;x++)
        {
            if(x<29) cout<<c[x][y];
            if(x==29) cout<<c[x][y]<<endl;
        }
    }
    cout<<""<<endl;
    cout<<"Your Score: "<<score;
}

void pipes()  //pipe movement and spawn func
{
    int x,y,r;
    if(t==10)  
    {
        r=(rand()%11)+5;  
        for(y=0;y<20;y++)  
        {
            c[29][y]='|';  
            n[29][y]=2;    
        }
        c[29][r-1]=' ';  
        c[29][r]=' ';
        c[29][r+1]=' ';
        n[29][r-1]=0;
        n[29][r]=0;
        n[29][r+1]=0;
        t=0;
        goto mv; //moves pipes
    }
    else goto mv;
    mv:                 //pipe movement
    {
        for(y=0;y<20;y++)  //loops for generating coordinates
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='|')  //all the pipes will be moved left by 1;
                {
                    if(x>0)
                    {
                        c[x-1][y]='|';
                        n[x-1][y]=2;
                        c[x][y]=' ';
                        n[x][y]=0;
                    }
                    if(x==0)  //if screen ends (x=0) pipe will dissapear, to prevent errors
                    {
                        c[x][y]=' ';
                        n[x][y]=0;
                    }
                }
            }
        }
    }
}

void bird()   //bird movement function!
{
    int x,y;
    if(tuk>0) //if key is pressed, bird moves up
    {
        bt=0;
        for(y=0;y<20;y++)   //loops for finding bird coordinates
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y>0)
                    {
                    c[x][y-1]='*';  //bird moves up by 1;
                    c[x][y]=' ';
                    birdx=x;        //sets bird x coordinate
                    birdy=y-1;      //sets bird y coord
                    return;         //retuns to game func
                    }
                }
            }
        }
    }
    else   //if no key is pressed, bird falls
    {
        bt++;
        for(y=0;y<20;y++)
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y<20)  //if bird is not on the ground
                    {
                        if(bt<3)   
                        {
                            c[x][y+1]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+1;
                            return;
                        }
                        else if(bt>2 && bt<5)  //more time has passed, faster bird falls (acceleration)
                        {
                            c[x][y+2]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+2;
                            return;
                        }
                        else if(bt>4)
                        {
                            c[x][y+3]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+3;
                            return;
                        }
                    }
                    else
                    {
                        return;  //if bird is already on the ground, function returns to check for game over.
                    }
                }
            }
        }
    }
}
void checkscore()  //checks if bird gained score
{
    int y;
    for(y=0;y<20;y++)
    {
        if(c[birdx][y]=='|')  //if bird x coord is equal to pipe's coord, you get 1 point
        {
            score++;
            return;
        }
    }
}

bool gameover()  //checks if bird has hit something
{
    int x,y,f=0;
    if(birdy>19) //checks if bird hits ground
    {
        c[birdx][19]='*';  //sets bird and ground again, prevents errors
        c[birdx][20]='-';
        f=1;           //f=1, means function will return true
        goto quit;
    }
    else
    {     //checks if bird has hit pipes, here the 'n' variable is needed (pipe's coordinate's n is equal 2 (more than 0))
        if(n[birdx][birdy]>0 && (c[birdx][birdy]=='|' || c[birdx][birdy]=='*'))
        {
            c[birdx][birdy]='|';
            c[birdx-1][19]='*';
            f=1;
            goto quit;
        }
    }
    quit:
    if(f==1) return true;
    else return false;
}

void endgame() //just some screens for certain actions
{
    screen();   //this one pops up if game ends
    cout<<""<<endl<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<"|    *****      *     *       * ******       ****  *       ****** ****    |"<<endl;
    cout<<"|   *          * *    * *   * * *           *    *  *     * *     *   *   |"<<endl;
    cout<<"|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |"<<endl;
    cout<<"|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |"<<endl;
    cout<<"|    *****  *       * *       * ******       ****      *    ***** *   *   |"<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<""<<endl<<endl;
    cout<<"                        Y O U R   S C O R E : "<<score<<endl<<endl;
    cout<<"                        H I G H   S C O R E : "<<highscore<<endl;
    cout<<""<<endl<<endl;
}

void menu()  //shows menu
{
    system("cls");
    cout<<""<<endl;
    cout<<" --------------------------------------------------------  "<<endl;
    cout<<"|                                                        | "<<endl;
    cout<<"|   **** *    **** **** **** *   *    ***  * ***  ***    | "<<endl;
    cout<<"|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | "<<endl;
    cout<<"|   ***  *    **** **** **** *****    ***  * ***  *  *   | "<<endl;
    cout<<"|   *    *    *  * *    *      *      *  * * *  * *  *   | "<<endl;
    cout<<"|   *    **** *  * *    *      *      ***  * *  * ***    | "<<endl;
    cout<<"|                                                  v 1.0 | "<<endl;
    cout<<" --------------------------------------------------------  "<<endl;
    cout<<""<<endl<<endl;
    cout<<"                  High Score:  "<<highscore<<endl<<endl;
    cout<<""<<endl<<endl;
    cout<<"                     M E N U:    "<<endl<<endl;
    cout<<"                  1: Start Game  "<<endl<<endl;
    cout<<"                  2: Help        "<<endl<<endl;
    cout<<"                  3: Credits     "<<endl<<endl;
    cout<<"                  4: Exit        "<<endl<<endl;
}

void credits()
{
    char sel;
    system("cls");
    while(true)
    {
    cout<<""<<endl<<endl;
    cout<<"               Programmers : Yash Saitwal & Devarsh Shah "<<endl<<endl;
    cout<<"Go back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    }
}

void help()
{
    char sel;
    system("cls");
    while(true)
    {
    cout<<""<<endl<<endl;
    cout<<"                   Controls: Press any key to fly up.         "<<endl<<endl;
    cout<<"             Goal: Fly through the holes between the pipes.   "<<endl;
    cout<<"             When you pass through the hole,you get 1 point.  "<<endl;
    cout<<"                    Try to pass as much as you can.           "<<endl;
    cout<<"            But be careful, don't hit the pipes or the ground!"<<endl<<endl;
    cout<<"                          Are you ready? Go!                  "<<endl<<endl<<endl;
    cout<<"Go back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    }
}
