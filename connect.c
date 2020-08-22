
#include    <stdlib.h>
#include    <ncurses.h>
#include    <string.h>
#include    <time.h>

#define     NADA        0
#define     NMEXEGIRA   1
#define     MEXE        2
#define     GIRA        3
#define     MEXEGIRA    4

typedef struct t_quadrado
{
    int cima;
    int baixo;
    int esq;
    int dir;
    int tipo;
} t_quadrado;

typedef struct t_tabuleiro
{
    t_quadrado *m[4][4];
} t_tabuleiro;

void inicializa_tabuleiro (t_tabuleiro *tab)
{
    int i,j;
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<4 ; j++)
        {
            tab->m[i][j] = (t_quadrado *) malloc (sizeof(t_quadrado));
            tab->m[i][j]->tipo  = NADA;
            tab->m[i][j]->cima  = 0;
            tab->m[i][j]->baixo = 0;
            tab->m[i][j]->esq   = 0;
            tab->m[i][j]->dir   = 0;
        }
    }
}

void faz_fase (t_tabuleiro *tab)
{
    tab->m[0][0]->tipo = MEXE;
    tab->m[0][0]->cima = 1;
    tab->m[0][0]->baixo= 0;
    tab->m[0][0]->esq  = 0;
    tab->m[0][0]->dir  = 3;

    tab->m[1][2]->tipo = MEXEGIRA;
    tab->m[1][2]->cima = 2;
    tab->m[1][2]->baixo= 0;
    tab->m[1][2]->esq  = 1;
    tab->m[1][2]->dir  = 0;

    tab->m[3][3]->tipo = GIRA;
    tab->m[3][3]->cima = 0;
    tab->m[3][3]->baixo= 3;
    tab->m[3][3]->esq  = 0;
    tab->m[3][3]->dir  = 0;

    tab->m[2][1]->tipo = NMEXEGIRA;
    tab->m[2][1]->cima = 0;
    tab->m[2][1]->baixo= 0;
    tab->m[2][1]->esq  = 0;
    tab->m[2][1]->dir  = 2;
}

int verifica_tabuleiro (t_tabuleiro *tab)
{
    int i,j;
    for (i=0 ; i<4 ; i++)           /*os fors identificam quando as junções não estão batendo. Caso todo o for seja executado, a fase foi completada.*/
    {
        for (j=0 ; j<4 ; j++)
        {
            if ( i!=0 )
            {
                if (tab->m[i][j]->cima != tab->m[i-1][j]->baixo)
                    return 0;
            }
            if ( i!=3 )
            {
                if (tab->m[i][j]->baixo != tab->m[i+1][j]->cima)
                    return 0;
            }
            if ( j!=0 )
            {
                if (tab->m[i][j]->esq != tab->m[i][j-1]->dir)
                    return 0;
            }
            if ( j!=3 )
            {
                if (tab->m[i][j]->dir != tab->m[i][j+1]->esq)
                    return 0;
            }
        }
    }
    return 1;
}

int verifica_movimento (t_tabuleiro *tab, int l_ori, int c_ori, int l_des, int c_des)
{
    if ((tab->m[l_ori][c_ori]->tipo == MEXE) || (tab->m[l_ori][c_ori]->tipo == MEXEGIRA))
    {
        if (tab->m[l_des][c_des]->tipo == NADA)     /*só retorna verdadeiro caso não há peça no destino*/
            return 1;
        else
            return 0;
    }
    else
        return 0;       /*caso a peça não possa mexer ou não há peça, retorna falso.*/
}

void imprime_tabuleiro (WINDOW *tab, t_tabuleiro *tabu)
{
    int i,j,k;
    char c;
    wclear(tab);
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<4 ; j++)
        {
            if (tabu->m[i][j]->tipo != NADA)                     /*verifica se tem peça*/
            {
                if (tabu->m[i][j]->tipo == NMEXEGIRA)
                    wattron (tab,COLOR_PAIR(1));
                else 
                {   
                    if (tabu->m[i][j]->tipo == GIRA)
                        wattron (tab,COLOR_PAIR(2));
                    else 
                    {
                        if (tabu->m[i][j]->tipo == MEXE)
                            wattron (tab,COLOR_PAIR(3));
                        else
                            wattron (tab,COLOR_PAIR(4));
                    }
                }    
                mvwaddch (tab, 1+i*9, 1+j*15 , ACS_ULCORNER);   /*imprime os cantos*/
                mvwaddch (tab, 1+i*9, 15+j*15, ACS_URCORNER);
                mvwaddch (tab, 9+i*9, 1+j*15 , ACS_LLCORNER);
                mvwaddch (tab, 9+i*9, 15+j*15, ACS_LRCORNER);
                for (k=2 ; k<9 ; k++)                           /*imprime os lados e os valores*/
                {
                    if (k==5)
                    {
                        if (tabu->m[i][j]->esq != 0)
                        {
                            c = tabu->m[i][j]->esq + '0';
                            mvwaddch (tab, k+i*9, 1+j*15 , c);
                        }
                        else
                            mvwaddch (tab, k+i*9, 1+j*15 , ACS_VLINE);
                        if (tabu->m[i][j]->dir != 0)
                        {   
                            c = tabu->m[i][j]->dir + '0';
                            mvwaddch (tab, k+i*9, 15+j*15, c);
                        }
                        else
                            mvwaddch (tab, k+i*9, 15+j*15, ACS_VLINE);
                    }
                    else
                    {
                        mvwaddch (tab, k+i*9, 1+j*15 , ACS_VLINE);
                        mvwaddch (tab, k+i*9, 15+j*15, ACS_VLINE);
                    }
                }
                for (k=2 ; k<15 ; k++)
                {
                    if (k==8)
                    {
                        if (tabu->m[i][j]->cima != 0)
                        {
                            c = tabu->m[i][j]->cima + '0';
                            mvwaddch (tab, 1+i*9, k+j*15, c);
                        }
                        else
                            mvwaddch (tab, 1+i*9, k+j*15, ACS_HLINE);
                        if (tabu->m[i][j]->baixo != 0)
                        {
                            c = tabu->m[i][j]->baixo + '0';
                            mvwaddch (tab, 9+i*9, k+j*15, c);
                        }
                        else
                            mvwaddch (tab, 9+i*9, k+j*15, ACS_HLINE);
                    }
                    else
                    {
                        mvwaddch (tab, 1+i*9, k+j*15, ACS_HLINE);
                        mvwaddch (tab, 9+i*9, k+j*15, ACS_HLINE);
                    }
                }
                if (tabu->m[i][j]->tipo == NMEXEGIRA)
                    wattroff (tab,COLOR_PAIR(1));
                else 
                {
                    if (tabu->m[i][j]->tipo == GIRA)
                        wattroff (tab,COLOR_PAIR(2));
                    else 
                    {
                        if (tabu->m[i][j]->tipo == MEXE)
                            wattroff (tab,COLOR_PAIR(3));
                        else
                            wattroff (tab,COLOR_PAIR(4)); 
                    }
                }
            }
        }
    }
    box (tab,0,0);
    wrefresh(tab);
}

void imprime_select (WINDOW *tab,int l,int c)       
{
    wattron(tab,COLOR_PAIR(5));
    mvwaddch (tab, 1+l*9, 1+c*15 , ACS_ULCORNER);
    mvwaddch (tab, 1+l*9, 15+c*15, ACS_URCORNER);
    mvwaddch (tab, 9+l*9, 1+c*15 , ACS_LLCORNER);
    mvwaddch (tab, 9+l*9, 15+c*15, ACS_LRCORNER);
    mvwaddch (tab, 1+l*9, 2+c*15 , ACS_HLINE);
    mvwaddch (tab, 9+l*9, 2+c*15 , ACS_HLINE);
    mvwaddch (tab, 1+l*9, 14+c*15, ACS_HLINE);
    mvwaddch (tab, 9+l*9, 14+c*15, ACS_HLINE);
    mvwaddch (tab, 2+l*9, 1+c*15 , ACS_VLINE);
    mvwaddch (tab, 2+l*9, 15+c*15, ACS_VLINE);
    mvwaddch (tab, 8+l*9, 1+c*15 , ACS_VLINE);
    mvwaddch (tab, 8+l*9, 15+c*15, ACS_VLINE);
    wattroff(tab,COLOR_PAIR(5));
    wrefresh(tab);
}

void imprime_select2 (WINDOW *tab,int l,int c)       
{
    wattron(tab,COLOR_PAIR(6));
    mvwaddch (tab, 1+l*9, 1+c*15 , ACS_ULCORNER);
    mvwaddch (tab, 1+l*9, 15+c*15, ACS_URCORNER);
    mvwaddch (tab, 9+l*9, 1+c*15 , ACS_LLCORNER);
    mvwaddch (tab, 9+l*9, 15+c*15, ACS_LRCORNER);
    mvwaddch (tab, 1+l*9, 2+c*15 , ACS_HLINE);
    mvwaddch (tab, 9+l*9, 2+c*15 , ACS_HLINE);
    mvwaddch (tab, 1+l*9, 14+c*15, ACS_HLINE);
    mvwaddch (tab, 9+l*9, 14+c*15, ACS_HLINE);
    mvwaddch (tab, 2+l*9, 1+c*15 , ACS_VLINE);
    mvwaddch (tab, 2+l*9, 15+c*15, ACS_VLINE);
    mvwaddch (tab, 8+l*9, 1+c*15 , ACS_VLINE);
    mvwaddch (tab, 8+l*9, 15+c*15, ACS_VLINE);
    wattroff(tab,COLOR_PAIR(6));
    wrefresh(tab);
}

void mover_select (char tecla,int *l,int *c)
{
    if (tecla == 'w')
    {
        if (*l > 0)
            *l = *l - 1;
    }
    else if (tecla == 'a')
    {
        if (*c > 0)
            *c = *c - 1;
    }
    else if (tecla == 's')
    {
        if (*l < 3)
            *l = *l + 1;
    }
    else if (tecla == 'd')
    {
        if (*c < 3)
            *c = *c + 1;
    }
}

void move_peca (t_tabuleiro *tab,int linha1,int coluna1,int linha2,int coluna2)
{
    tab->m[linha2][coluna2]->tipo   = tab->m[linha1][coluna1]->tipo   ;
    tab->m[linha2][coluna2]->cima   = tab->m[linha1][coluna1]->cima   ;
    tab->m[linha2][coluna2]->baixo  = tab->m[linha1][coluna1]->baixo  ;
    tab->m[linha2][coluna2]->dir    = tab->m[linha1][coluna1]->dir    ;
    tab->m[linha2][coluna2]->esq    = tab->m[linha1][coluna1]->esq    ;
    tab->m[linha1][coluna1]->tipo   = NADA;
    tab->m[linha1][coluna1]->cima   = 0;
    tab->m[linha1][coluna1]->baixo  = 0;
    tab->m[linha1][coluna1]->dir    = 0;
    tab->m[linha1][coluna1]->esq    = 0;
}

void gira_peca (t_tabuleiro *tab,int linha,int coluna)
{
    int sup = tab->m[linha][coluna]->cima;
    tab->m[linha][coluna]->cima  = tab->m[linha][coluna]->esq;
    tab->m[linha][coluna]->esq   = tab->m[linha][coluna]->baixo; 
    tab->m[linha][coluna]->baixo = tab->m[linha][coluna]->dir;
    tab->m[linha][coluna]->dir   = sup;
}

void algoritmo_cria_fase (t_tabuleiro *tab)
{
    int chance,i,j;
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<4 ; j++)
        {   
            if (i != 0)
            {
                if (tab->m[i-1][j]->baixo != 0)
                    tab->m[i][j]->cima = tab->m[i-1][j]->baixo;
            }
            if (j != 0)
            {
                if (tab->m[i][j-1]->dir != 0)
                    tab->m[i][j]->esq = tab->m[i][j-1]->dir;
            }
            if (i != 3)
            {
                chance = rand() % 3;
                if (chance == 1) /* 33% de chance */
                    tab->m[i][j]->baixo = (rand() % 4) + 1;
            }
            if (j != 3)
            {
                chance = rand() % 3;
                if (chance == 1) /* 33% de chance */
                    tab->m[i][j]->dir = (rand() % 4) + 1;
            }
            if ((tab->m[i][j]->baixo != 0) || (tab->m[i][j]->cima != 0) || (tab->m[i][j]->dir != 0) || (tab->m[i][j]->esq != 0))
            {
                chance = (rand() % 10);
                if (chance < 2)
                    tab->m[i][j]->tipo = 1;
                else if ((chance > 1)&&(chance <4))
                    tab->m[i][j]->tipo = 2;
                else if ((chance > 3)&&(chance <6))
                    tab->m[i][j]->tipo = 3;
                else
                    tab->m[i][j]->tipo = 4;
            }
        }
    }
}

void troca_peca (t_tabuleiro *tab, int lin1, int col1, int lin2, int col2)
{
    int sup;
    sup = tab->m[lin1][col1]->tipo;
    tab->m[lin1][col1]->tipo = tab->m[lin2][col2]->tipo;
    tab->m[lin2][col2]->tipo = sup;   
    sup = tab->m[lin1][col1]->cima;
    tab->m[lin1][col1]->cima = tab->m[lin2][col2]->cima;
    tab->m[lin2][col2]->cima = sup;                     
    sup = tab->m[lin1][col1]->baixo;
    tab->m[lin1][col1]->baixo = tab->m[lin2][col2]->baixo;
    tab->m[lin2][col2]->baixo = sup;                     
    sup = tab->m[lin1][col1]->dir;
    tab->m[lin1][col1]->dir = tab->m[lin2][col2]->dir;
    tab->m[lin2][col2]->dir = sup;                     
    sup = tab->m[lin1][col1]->esq;
    tab->m[lin1][col1]->esq = tab->m[lin2][col2]->esq;
    tab->m[lin2][col2]->esq = sup;                     
}

void embaralha_fase (t_tabuleiro *tab)
{
    int i,j,k,n;
    int lin,col;
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<4 ; j++)
        {
            if ((tab->m[i][j]->tipo == GIRA) || (tab->m[i][j]->tipo == MEXEGIRA))
            {
                n = rand() % 3 + 1;
                for (k=0 ; k<n ; k++)
                {
                    gira_peca (tab,i,j);
                } 
            }
            if ((tab->m[i][j]->tipo == MEXE) || (tab->m[i][j]->tipo == MEXEGIRA))
            {
                n = rand() % 5;
                if (n!=4) /*80% de chance de mover a peca*/
                {
                    lin = rand() % 4;
                    col = rand() % 4;
                    while (!((tab->m[lin][col]->tipo == MEXE) || (tab->m[lin][col]->tipo == MEXEGIRA) || (tab->m[lin][col]->tipo == NADA)))
                    {
                        lin = rand() % 4;
                        col = rand() % 4;
                    }
                    troca_peca (tab,i,j,lin,col);
                }
            }
        }
    }
}

void imprime_menu (WINDOW *wtab)
{
    werase(wtab);
    int lin, col;
    lin = 10;
    col = 10;
    box(wtab,0,0); 
    mvwprintw (wtab, lin  , col, "  _____                            _   ");
    mvwprintw (wtab, lin+1, col, " / ____|                          | |  ");
    mvwprintw (wtab, lin+2, col, "| |     ___  _ __  _ __   ___  ___| |_ ");
    mvwprintw (wtab, lin+3, col, "| |    / _ \\| '_ \\| '_ \\ / _ \\/ __| __|");
    mvwprintw (wtab, lin+4, col, "| |___| (_) | | | | | | |  __/ (__| |_ ");
    mvwprintw (wtab, lin+5, col, " \\_____\\___/|_| |_|_| |_|\\___|\\___|\\__|");

    mvwprintw (wtab, lin+10, col   , "Pecas ");
    wattron (wtab,COLOR_PAIR(1));
    mvwprintw (wtab, lin+10, col+6 , "vermelhas ");
    wattroff(wtab,COLOR_PAIR(1));
    mvwprintw (wtab, lin+10, col+16, "nao mexem, nem giram");

    mvwprintw (wtab, lin+11, col   , "Pecas ");
    wattron (wtab,COLOR_PAIR(2));
    mvwprintw (wtab, lin+11, col+6 , "azuis ");
    wattroff(wtab,COLOR_PAIR(2));
    mvwprintw (wtab, lin+11, col+16, "somente giram");

    mvwprintw (wtab, lin+12, col   , "Pecas ");
    wattron (wtab,COLOR_PAIR(3));
    mvwprintw (wtab, lin+12, col+6 , "verdes ");
    wattroff(wtab,COLOR_PAIR(3));
    mvwprintw (wtab, lin+12, col+16, "somente mexem");

    mvwprintw (wtab, lin+13, col   , "Pecas ");
    wattron (wtab,COLOR_PAIR(4));
    mvwprintw (wtab, lin+13, col+6 , "laranjas ");
    wattroff(wtab,COLOR_PAIR(4));
    mvwprintw (wtab, lin+13, col+16, "giram e mexem");

    mvwprintw (wtab, lin+15, col, "Conecte todos os pares de numeros iguais");
    mvwprintw (wtab, lin+16, col, "e conclua o nível!                      ");

    mvwprintw (wtab, lin+18, col, "Aperte espaco para comecar!");
    mvwprintw (wtab, lin+19, col, "Aperte q para fechar o jogo.");
    wrefresh(wtab);
}

int main ()
{
    initscr();
    noecho();
    curs_set(FALSE);

    start_color();
    init_color(COLOR_BLACK,0,0,0);
    init_color(10,1000,600 ,0   );
    init_pair (1,COLOR_RED    ,COLOR_BLACK  );  /*nmexegira*/
    init_pair (2,COLOR_BLUE   ,COLOR_BLACK  );  /*gira*/
    init_pair (3,COLOR_GREEN  ,COLOR_BLACK  );  /*mexe*/
    init_pair (4,10           ,COLOR_BLACK  );  /*mexegira*/
    init_pair (5,COLOR_MAGENTA,COLOR_WHITE  );  /*select*/
    init_pair (6,COLOR_WHITE  ,COLOR_MAGENTA);

    WINDOW *wtabuleiro = newwin (38,62,0,0);

    t_tabuleiro tabuleiro;
    inicializa_tabuleiro (&tabuleiro);
    int linha1,coluna1,linha2,coluna2;
    linha1  = 0;
    coluna1 = 0;
    linha2  = 0;
    coluna2 = 0;
    char tecla;
    tecla = 'o';

    while (tecla != 'q')
    {
        imprime_menu (wtabuleiro);
        nodelay(stdscr, TRUE);
        while ((tecla != ' ')&&(tecla != 'q'))
        {   
            imprime_menu (wtabuleiro);
            tecla = getch();
        }
        nodelay (stdscr, FALSE);
    
        if (tecla == ' ')
        {
            int movendo = 0;
            srand(time(0));

            algoritmo_cria_fase (&tabuleiro);
            embaralha_fase (&tabuleiro);
    
            while (!(verifica_tabuleiro (&tabuleiro))) /*verifica se o jogo acabou*/
            {
                if (! movendo)                              /*parte que executa quando não estamos movendo uma peça*/
                {
                    imprime_tabuleiro(wtabuleiro,&tabuleiro);   
                    imprime_select(wtabuleiro,linha1,coluna1);
                    wrefresh(wtabuleiro);
                    refresh();
                    tecla = getch();
                    mover_select (tecla,&linha1,&coluna1);
                    if ((tecla == 'j')&&((tabuleiro.m[linha1][coluna1]->tipo == MEXE)||(tabuleiro.m[linha1][coluna1]->tipo == MEXEGIRA)))
                    {
                        movendo = 1;
                        linha2 = linha1;
                        coluna2 = coluna1;
                    }
                    if ((tecla == 'k')&&((tabuleiro.m[linha1][coluna1]->tipo == GIRA)||(tabuleiro.m[linha1][coluna1]->tipo == MEXEGIRA)))
                        gira_peca (&tabuleiro,linha1,coluna1);
                }   
                else                                        /*parte que executa quando estamosmovendo uma peça*/
                {
                    imprime_tabuleiro(wtabuleiro,&tabuleiro);                  
                    imprime_select(wtabuleiro,linha1,coluna1);
                    imprime_select2(wtabuleiro,linha2,coluna2);
                    tecla = getch();
                    mover_select (tecla,&linha2,&coluna2);
                    if (tecla == 'j')
                    {
                        if (tabuleiro.m[linha2][coluna2]->tipo == NADA)
                            move_peca (&tabuleiro,linha1,coluna1,linha2,coluna2);
                        movendo = 0;
                        linha1 = linha2;
                        coluna1 = coluna2;
                    }
                }
            }
        
            imprime_tabuleiro(wtabuleiro,&tabuleiro);
            mvwprintw (wtabuleiro, 0, 2, "Parabens, voce concluiu a fase! Aperte j pra continuar!");
            wrefresh(wtabuleiro);
            tecla = getch();
            while (tecla != 'j')
                tecla = getch();
            wclear(wtabuleiro);
        }
    }
    endwin();
    return 0;
}
