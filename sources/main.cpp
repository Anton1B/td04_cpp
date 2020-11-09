#include <cstdlib>
#include "vibes.h"
#include "Path.h"
#include "Cell.h"
#include <iostream>
#include <utility>
#include <map>

using namespace std;

struct Maze
{
  Cell *c0, *cf;
};

void draw_gate(const Cell *n1, const Cell *n2)
{
  vibes::drawBox(min(n1->m_x, n2->m_x) + 0.1, max(n1->m_x, n2->m_x) + 0.9,
                 min(n1->m_y, n2->m_y) + 0.1, max(n1->m_y, n2->m_y) + 0.9,
                 "lightGray[lightGray]");
}

void display_cell(Cell *cell)
{
  vibes::drawBox(cell->m_x, cell->m_x + 1, cell->m_y, cell->m_y + 1, "[lightGray]");
  cell->m_displayed = true;

  for(int i = 0 ; i < cell->m_nb_neighb ; i++)
  {
    if(!cell->m_neighb[i]->m_displayed)
      display_cell(cell->m_neighb[i]);
    draw_gate(cell, cell->m_neighb[i]);
  }
}

void display(Maze maze)
{
  display_cell(maze.c0);
  vibes::drawCircle(maze.c0->m_x + 0.5, maze.c0->m_y + 0.5, 0.3, "#00AC07[#00AC07]");
  vibes::drawCircle(maze.cf->m_x + 0.5, maze.cf->m_y + 0.5, 0.3, "#F39F00[#F39F00]");
}

void display_path(Path *path)
{
  if(path->m_next != NULL)
  {
    vibes::drawBox(min(path->m_c->m_x, path->m_next->m_c->m_x) + 0.45, max(path->m_c->m_x, path->m_next->m_c->m_x) + 0.55,
                   min(path->m_c->m_y, path->m_next->m_c->m_y) + 0.45, max(path->m_c->m_y, path->m_next->m_c->m_y) + 0.55,
                   "#0088AC[#0088AC]");
    display_path(path->m_next);
  }
}

bool find_path(Cell *c, Cell *cf, Path *path)
{
  c->m_flag = true;

  if(c == cf)
    return true;

  for(int i = 0 ; i < c->m_nb_neighb ; i++)
  {
    if(c->m_neighb[i]->m_flag)
      continue;

    if(find_path(c->m_neighb[i], cf, path))
    {
      path->add_to_path(c);
      return true;
    }
  }

  return false;
}

Maze create_8x8_maze()
{
  Cell* a[8][8];
  for(int i = 0 ; i < 8 ; i++)
  {
    for(int j = 0 ; j < 8 ; j++)
    {
      a[i][j] = new Cell(i, j);
    }
  }

  a[1][0]->add_neighb(a[0][0], a[2][0]);
  a[2][1]->add_neighb(a[2][0], a[1][1]);
  a[0][1]->add_neighb(a[0][2], a[1][1]);
  a[0][3]->add_neighb(a[0][2], a[0][4]);
  a[0][5]->add_neighb(a[0][4], a[1][5]);
  a[1][6]->add_neighb(a[1][5], a[1][7], a[0][6]);
  a[0][7]->add_neighb(a[0][6]);
  a[2][5]->add_neighb(a[1][5], a[2][6]);
  a[2][7]->add_neighb(a[2][6], a[3][7]);
  a[3][6]->add_neighb(a[3][5], a[4][6], a[3][7]);
  a[4][7]->add_neighb(a[5][7], a[4][6]);
  a[6][7]->add_neighb(a[6][6]);
  a[5][6]->add_neighb(a[5][7], a[5][5], a[6][6]);
  a[4][5]->add_neighb(a[5][5], a[4][4]);
  a[3][4]->add_neighb(a[4][4], a[2][4]);
  a[1][4]->add_neighb(a[2][4], a[1][3]);
  a[1][2]->add_neighb(a[1][3], a[2][2]);
  a[3][2]->add_neighb(a[2][2], a[3][1], a[4][2]);
  a[4][3]->add_neighb(a[4][2], a[5][3]);
  a[5][4]->add_neighb(a[5][3]);
  a[6][3]->add_neighb(a[6][4], a[6][2], a[5][3]);
  a[5][2]->add_neighb(a[5][3], a[5][1]);
  a[5][0]->add_neighb(a[5][1]);
  a[4][1]->add_neighb(a[4][0], a[3][1]);
  a[3][0]->add_neighb(a[4][0]);
  a[1][4]->add_neighb(a[2][4], a[1][3]);
  a[1][2]->add_neighb(a[1][3], a[2][2]);
  a[3][2]->add_neighb(a[2][2], a[3][1]);
  a[7][6]->add_neighb(a[7][7], a[7][5]);
  a[7][4]->add_neighb(a[7][5], a[7][3]);
  a[7][2]->add_neighb(a[7][3], a[7][1]);
  a[7][0]->add_neighb(a[7][1], a[6][0]);
  a[6][1]->add_neighb(a[6][0], a[6][2]);
  a[6][3]->add_neighb(a[6][2], a[6][4]);
  a[6][5]->add_neighb(a[6][3]);
  a[2][3]->add_neighb(a[3][3], a[1][3]);

  Maze m;
  m.c0 = a[0][0]; m.cf = a[7][7];
  return m;
}


Maze create_4x4_maze()
{
  Cell *cA = new Cell(0, 0);
  Cell *cB = new Cell(0, 1);
  Cell *cC = new Cell(0, 2);
  Cell *cD = new Cell(0, 3);
  Cell *cE = new Cell(1, 3);
  Cell *cF = new Cell(1, 2);
  Cell *cG = new Cell(1, 1);
  Cell *cH = new Cell(1, 0);
  Cell *cI = new Cell(2, 0);
  Cell *cJ = new Cell(2, 1);
  Cell *cK = new Cell(2, 2);
  Cell *cL = new Cell(2, 3);
  Cell *cM = new Cell(3, 3);
  Cell *cN = new Cell(3, 2);
  Cell *cO = new Cell(3, 1);
  Cell *cP = new Cell(3, 0);

  cH->add_neighb(cA, cI);
  cB->add_neighb(cG, cA);
  cJ->add_neighb(cG, cK);
  cF->add_neighb(cG, cC);
  cD->add_neighb(cC, cE);
  cL->add_neighb(cE, cM);
  cN->add_neighb(cO, cM);
  cP->add_neighb(cO);

  Maze m;
  m.c0 = cA; m.cf = cM;
  return m;
}


void save_maze(const Maze& maze, const string& file_name)
{
  string filename = file_name;
  ofstream ofst(filename); // ouverture en écriture
  
  // ..tests d’ouverture..
  if (!ofst.is_open())
  {
    cout << "Erreur d’ouverture de " << filename << endl;
  }
  
  //Initialisation du fichier avec la cellule d'entrée et la cellule de sortie
  ofst << "# Start:" << endl << *maze.c0 << endl << "# End:" << endl << *maze.cf << endl << "# Cells:" << endl ;

  Cell* cellule = maze.c0; //On choisit la cellule d'entrée comme première cellule à l'origine d'une ligne
  Cell* unused = new Cell(40,40);
  Cell* trans_cell = cellule;
  int compteur = 0;
  int compteur2 = 0;


  while (compteur != 16) // Tant que les n*n lignes n'ont pas été écrites
  {
    cellule = trans_cell; //On réattribue la cellule de transfert

    //Arrêt en cas de boucle infinie
    compteur2++;
    if (compteur2 >20)
    {
      compteur=16;
    }
    
    if (cellule->m_flag2 == false) //Si la ligne de la cellule n'a pas déjà été écrite
    {
      ofst << *cellule << cellule->m_nb_neighb<<flush; //On écrit la cellule et le nombre de voisins
      compteur+=1; //On indique qu'il y a déjà +1 ligne qui va être écrite

      for (int j=0;j<cellule->m_nb_neighb;j++) //On parcourt la liste des voisines
      {
        ofst << *(cellule->m_neighb[j]); //On écrit les voisines
      }
      ofst<<endl;
    }

    
    cellule->m_flag2 = true; //On indique que la ligne a été écrite
    int test = 0; 

    for (int j=0;j<cellule->m_nb_neighb;j++) //Parmi les voisines
    {
      if (cellule->m_neighb[j]->m_flag2 == false && test==0) //On récupère la première voisine qui n'est pas à l'origine d'une ligne
        {
          trans_cell = cellule->m_neighb[j]; //On la choisit pour être à l'origine d'une nouvelle ligne
          test = 1; // On indique qu'on a trouvé une remplaçante

          if (cellule->m_neighb[j+1] != NULL && cellule->m_neighb[j+1]->m_flag2 == false) //la cellule voisine, si elle existe et qu'elle n'est pas déjà été à l'origine d'une ligne
          {
            unused = cellule->m_neighb[j+1];// On la prend en compte pour plus tard
          }
        }
    }
    if (test==0) // Si on ne trouve pas de cellule à l'origine d'une ligne
      {
        trans_cell = unused; // On prend la cellule qui avait été laissée de côté
      }
  }
  ofst.close(); // fermeture du fichier



}


Maze read_maze(const string& file_name)
{
  string filename = file_name;
  ifstream ifst(filename); // ouverture en lecture
  
  // ..tests d’ouverture..
  if (!ifst.is_open())
  {
    cout << "Erreur d’ouverture de " << filename << endl;
  }

  Maze shining;
  map<std::pair<int,int>,Cell*> xy_cell;

  string ligne;
  int compteur = 1;
  while(getline(ifst, ligne)) //Tant qu'on n'est pas à la fin du fichier, on lit
      {
        Cell* cellule = new Cell(40,40); //Création d'une cellule
        if (compteur>6)
          {
            ifst>>*cellule; //Récupération d'une ligne pour récupérer les coordonnées
          }
          //Ne fonctionne pas
          //xy_cell[<cellule->m_x,cellule->m_y>] = cellule; //ajout de la cellule à la map
        compteur++;
      }




}




int main()
{
  Maze maze = create_4x4_maze();
  Path *path = new Path(maze.cf);

  save_maze(maze,"lab");

  vibes::beginDrawing();
  vibes::newFigure("Maze");
  vibes::setFigureProperties("Maze", vibesParams("x", 200, "y", 200, "width", 700, "height", 700));
  vibes::axisLimits(0-0.5, 8+0.5, 0-0.5, 8+0.5);

  display(maze);
  find_path(maze.c0, maze.cf, path);
  display_path(path);
  
  vibes::endDrawing();

  return EXIT_SUCCESS;
}