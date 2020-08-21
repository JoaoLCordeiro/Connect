
#include    <stdlib.h>
#include    <ncurses.h>
#include    <string.h>

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
    tab->m[1][2]->cima = 0;
    tab->m[1][2]->baixo= 0;
    tab->m[1][2]->esq  = 1;
    tab->m[1][2]->dir  = 0;

    tab->m[3][3]->tipo = GIRA;
    tab->m[3][3]->cima = 0;
    tab->m[3][3]->baixo= 3;
    tab->m[3][3]->esq  = 0;
    tab->m[3][3]->dir  = 0;
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
                mvwaddch (tab, 1+i*9, 1+j*15 , ACS_ULCORNER);   /*imprime os cantos*/
                mvwaddch (tab, 1+i*9, 15+j*15, ACS_URCORNER);
                mvwaddch (tab, 9+i*9, 1+j*15 , ACS_LLCORNER);
                mvwaddch (tab, 9+i*9, 15+j*15, ACS_LRCORNER);
                for (k=2 ; k<9 ; k++)                           /*imprime os lados e os valores*/
                {
                    if (k==5)
                    {
                        c = tabu->m[i][j]->esq + '0';
                        mvwaddch (tab, k+i*9, 1+j*15 , c);
                        c = tabu->m[i][j]->dir + '0';
                        mvwaddch (tab, k+i*9, 15+j*15, c);
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
                        c = tabu->m[i][j]->cima + '0';
                        mvwaddch (tab, 1+i*9, k+j*15, c);
                        c = tabu->m[i][j]->baixo + '0';
                        mvwaddch (tab, 9+i*9, k+j*15, c);
                    }
                    else
                    {
                        mvwaddch (tab, 1+i*9, k+j*15, ACS_HLINE);
                        mvwaddch (tab, 9+i*9, k+j*15, ACS_HLINE);
                    }
                }
            }
        }
    }
    box (tab,0,0);
    wrefresh(tab);
}

int main ()
{
    initscr();
    noecho();
    curs_set(FALSE);

    WINDOW *wtabuleiro = newwin (38,62,0,0);

    t_tabuleiro tabuleiro;
    inicializa_tabuleiro (&tabuleiro);
    int linha1,coluna1,linha2,coluna2;
    linha1  = 0;
    coluna1 = 0;
    linha2  = 0;
    coluna2 = 0;
    char tecla;

    faz_fase (&tabuleiro);      /*retirado na versão final, apenas para testes*/
    
    while (! (verifica_tabuleiro (&tabuleiro)))
    {
        imprime_tabuleiro(wtabuleiro,&tabuleiro);
        tecla = getch(); 
    }
    return 0;
}
