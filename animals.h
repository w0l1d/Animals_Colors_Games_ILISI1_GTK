#ifndef ANIMALS_H_INCLUDED
#define ANIMALS_H_INCLUDED

#include "myheaders.h"
#include "main_headers.h"
#include "anim_rules.h"

#define Max_de_ligne 130
#define Max_de_colonne 300

#define Max_width 1200
#define Max_height 650

 /* cette structure sert à stocker les coordonnées qui ont été modifiés et
     à ne pas toucher lors du traitement de la fonction "start_game_animals" */
typedef struct coord
{
    int colonne;
    int ligne;
    struct coord *svt;
}coord;

coord *create_coord(int x, int y)
{
    coord *nv=(coord*)malloc(sizeof(coord));
    if(!nv)
    {
        printf("erreur d'allocation!!\n");
        exit(EXIT_FAILURE);
    }
    nv->colonne=x;
    nv->ligne=y;
    nv->svt=NULL;
    return ((coord*)nv);
}

coord *ajouter_coord(coord *maliste,int x,int y)
{
    coord *nv=create_coord(x,y);
    if(!maliste)
        return ((coord*)nv);
    nv->svt=maliste;
    maliste=nv;
    return ((coord*)maliste);
}

int coord_existe(coord *maliste,int x, int y)
{
    coord *crt=maliste;
    while(crt)
    {
        if((crt->colonne==x)&&(crt->ligne==y))return 1;
        crt=crt->svt;
    }
    return 0;
}

//LA structure de la grille d'animaux
typedef struct
{
    GtkWidget *drawing_area; // l'espace de la grille
    cairo_t *CR; // objet qui dessine
    gint grid[Max_de_ligne][Max_de_colonne]; // matrice
    gint cel_size;//Taille de chaque cellule
    gint nb_cels_height;//Nombre de cases longueur
    gint nb_cels_width;//Nombre de cases largeur
    gint draw_size;//Taille de la matrice
    gint left,right,bottom;//Delimiteurs de la grille
    GdkColor color_cell; // couleurinitiale d'une cellule de la grille "BLANC"
    GdkColor lawn;
    int x,y;
}grille_animals;

//La structure du jeu
typedef struct
{
  grille_animals *ma_grille; //La grille
  WindowObject *W;//La fenetre
  TableObject *table_animaux;
  BoxObject *global_box; //Box global
  TableObject *table_buttons;
   ButtonObject *button_animals[10];
   ButtonObject *button_nature[15];
   BoxObject *box_grille;
   BoxObject *left_box;
   BoxObject *right_box;
   ButtonObject *retourner; // button de retour vers le menu du jeu
   ButtonObject *play;  // bouton delancement du jeu
   ButtonObject *next;  // bouton pour passer a l'etat suivant
   ButtonObject *restaurer; // bouton de reinitialisation de grille
   ButtonObject *save;
   ButtonObject *open;
   ButtonObject *parametres;   //here button parametres
   ButtonObject *supprimer;
   gboolean etat;  /* true = play | false = jeu en pause */
   gint timer;
   int choix;
   cairo_surface_t *nature_elements[15];  // rbi3
   cairo_surface_t *element[10];
   coord *maliste; // liste des coordonnes à ne pas toucher
   rule_anim *rules;
}animals_game;

grille_animals* init_grille_animals()
{
    int i,j;
    // allouer la grille
    grille_animals *GL=(grille_animals*)malloc(sizeof(grille_animals));
    //initialiser la taille des cellules
    GL->cel_size=24;  // 16
    // initialiser le nombre de cases
    GL->nb_cels_width=Max_width/GL->cel_size;
    GL->nb_cels_height=Max_height/GL->cel_size;
    // initialiser les delimiteurs
    GL->left=0;
    GL->right=(Max_de_colonne/2)+(GL->nb_cels_width/2);
    GL->bottom=(Max_de_ligne/2)-(GL->nb_cels_height/2);
    // creer l'espace de dessin
    GL->drawing_area=gtk_drawing_area_new();
    // initialiser les couleurs
    gdk_color_parse("WHITE",&GL->color_cell);
    gdk_color_parse("RED",&GL->lawn);

    //affecter la taille de la grille
    gtk_widget_set_size_request(GL->drawing_area,(GL->nb_cels_width*GL->cel_size),
                                (GL->nb_cels_height*GL->cel_size));
    //couleur de l'arriere plan
    GdkColor local_bg_color;
    gdk_color_parse("BLACK",&local_bg_color);
    gtk_widget_modify_bg(GL->drawing_area,0,&local_bg_color);


    // initialisation de la matrice
    for(i=0;i<Max_de_ligne;i++)
        for(j=0; j<Max_de_colonne;j++)
           GL->grid[i][j]=11;

    GL->x=0;
    GL->y=0;

    return ((grille_animals*)GL);
}

//Fonction de creation de l'interface du jeu
//Reçoit comme argument la structure du jeu
void create_interface_animals(animals_game *GW)
{
    //Creation de la fenetre
    GW->W=init_window("Animaux",200,900,4,0,0,0);
    GW->W=create_window(GW->W);

    GW->global_box=init_box(GW->W->window,"global box",'h',0,0);
    GW->global_box=create_box(GW->global_box);
    add_to_window(GW->W,GW->global_box->box);

    /** l'ensemble des elmnts de la grille et les boutons **/
    GW->box_grille=init_box(GW->global_box,"box_grille",'v',1,0);
    GW->box_grille = create_box(GW->box_grille);
    add_to_box_start(GW->global_box,GW->box_grille->box,1,1,0);

    GW->table_buttons=init_table(GW->box_grille->box,3,18,1);
    GW->table_buttons=create_table(GW->table_buttons);
    add_to_box_start(GW->box_grille,GW->table_buttons->table,1,1,0);


    GW->retourner=init_button(GW->table_buttons->table,"retourner","retourner","images/return.png");
    GW->retourner=create_button(GW->retourner);
    add_to_table(GW->table_buttons,GW->retourner->button,16,18,0,3);

    GW->save=init_button(GW->table_buttons->table,"sauvegarder","sauvegarger","images/save.png");
    GW->save=create_button(GW->save);
    add_to_table(GW->table_buttons,GW->save->button,6,8,0,3);

    GW->open=init_button(GW->table_buttons->table,"ouvrir","ouvrir","images/export.png");
    GW->open=create_button(GW->open);
    add_to_table(GW->table_buttons,GW->open->button,8,10,0,3);

    GW->restaurer=init_button(GW->table_buttons->table,"restaurer","restaurer","images/restore.png");
    GW->restaurer=create_button(GW->restaurer);
    add_to_table(GW->table_buttons,GW->restaurer->button,4,6,0,3);

    GW->next=init_button(GW->table_buttons->table,"next","next","images/next.png");
    GW->next=create_button(GW->next);
    add_to_table(GW->table_buttons,GW->next->button,2,4,0,3);

    GW->play=init_button(GW->table_buttons->table,"play","play","images/play.png");
    GW->play=create_button(GW->play);
    add_to_table(GW->table_buttons,GW->play->button,0,2,0,3);

    GW->left_box=init_box(GW->box_grille->box,"left box",'h',1,0);
    GW->left_box=create_box(GW->left_box);

    add_to_box_end(GW->left_box,GW->ma_grille->drawing_area,1,1,0);

        /** Mon tableau de regles et boutons d'animaux **/

    GW->right_box=init_box(GW->global_box->box,"right box",'v',1,10);
    GW->right_box=create_box(GW->right_box);
    add_to_box_start(GW->global_box,GW->right_box->box,1,1,0);

    GW->table_animaux=init_table(GW->right_box->box,10,5,1);
    GW->table_animaux=create_table(GW->table_animaux);
    add_to_box_end(GW->right_box,GW->table_animaux->table,1,1,0);

    //Creation et initialisation des boutons d'animaux

    GW->parametres=init_button(GW->table_animaux->table,NULL,NULL,"images/param.png");
    GW->parametres=create_button(GW->parametres);
    add_to_table(GW->table_animaux,GW->parametres->button,3,4,9,10);

    GW->button_animals[0]=init_button(GW->table_animaux->table,NULL,NULL,"images/lion.png");
    GW->button_animals[0]=create_button(GW->button_animals[0]);
    add_to_table(GW->table_animaux,GW->button_animals[0]->button,0,1,1,2);

    GW->button_animals[1]=init_button(GW->table_animaux->table,NULL,NULL,"images/leopard.png");
    GW->button_animals[1]=create_button(GW->button_animals[1]);
    add_to_table(GW->table_animaux,GW->button_animals[1]->button,1,2,1,2);

    GW->button_animals[2]=init_button(GW->table_animaux->table,NULL,NULL,"images/renard.png");
    GW->button_animals[2]=create_button(GW->button_animals[2]);
    add_to_table(GW->table_animaux,GW->button_animals[2]->button,2,3,1,2);

    GW->button_animals[3]=init_button(GW->table_animaux->table,NULL,NULL,"images/loup.png");
    GW->button_animals[3]=create_button(GW->button_animals[3]);
    add_to_table(GW->table_animaux,GW->button_animals[3]->button,3,4,1,2);

    GW->button_animals[4]=init_button(GW->table_animaux->table,NULL,NULL,"images/hyena.png");
    GW->button_animals[4]=create_button(GW->button_animals[4]);
    add_to_table(GW->table_animaux,GW->button_animals[4]->button,4,5,1,2);

    /**/

     GW->button_animals[5]=init_button(GW->table_animaux->table,NULL,NULL,"images/elephant.png");
    GW->button_animals[5]=create_button(GW->button_animals[5]);
    add_to_table(GW->table_animaux,GW->button_animals[5]->button,0,1,2,3);

    GW->button_animals[6]=init_button(GW->table_animaux->table,NULL,NULL,"images/lapin.png");
    GW->button_animals[6]=create_button(GW->button_animals[6]);
    add_to_table(GW->table_animaux,GW->button_animals[6]->button,1,2,2,3);

    GW->button_animals[7]=init_button(GW->table_animaux->table,NULL,NULL,"images/gazelle.png");
    GW->button_animals[7]=create_button(GW->button_animals[7]);
    add_to_table(GW->table_animaux,GW->button_animals[7]->button,2,3,2,3);

    GW->button_animals[8]=init_button(GW->table_animaux->table,NULL,NULL,"images/zebra.png");
    GW->button_animals[8]=create_button(GW->button_animals[8]);
    add_to_table(GW->table_animaux,GW->button_animals[8]->button,3,4,2,3);

    GW->button_animals[9]=init_button(GW->table_animaux->table,NULL,NULL,"images/sheep.png");
    GW->button_animals[9]=create_button(GW->button_animals[9]);
    add_to_table(GW->table_animaux,GW->button_animals[9]->button,4,5,2,3);

    /* boutons d'élements de nature*/

    GW->button_nature[0]=init_button(GW->table_animaux->table,NULL,NULL,"images/grass_button.png");
    GW->button_nature[0]=create_button(GW->button_nature[0]);
    add_to_table(GW->table_animaux,GW->button_nature[0]->button,0,1,3,4);

    GW->button_nature[1]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_up_button.png");
    GW->button_nature[1]=create_button(GW->button_nature[1]);
    add_to_table(GW->table_animaux,GW->button_nature[1]->button,0,1,4,5);

    GW->button_nature[2]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_down_button.png");
    GW->button_nature[2]=create_button(GW->button_nature[2]);
    add_to_table(GW->table_animaux,GW->button_nature[2]->button,1,2,4,5);

    GW->button_nature[3]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_left_button.png");
    GW->button_nature[3]=create_button(GW->button_nature[3]);
    add_to_table(GW->table_animaux,GW->button_nature[3]->button,2,3,4,5);

    GW->button_nature[4]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_right_button.png");
    GW->button_nature[4]=create_button(GW->button_nature[4]);
    add_to_table(GW->table_animaux,GW->button_nature[4]->button,3,4,4,5);

    GW->button_nature[5]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_upleft_button.png");
    GW->button_nature[5]=create_button(GW->button_nature[5]);
    add_to_table(GW->table_animaux,GW->button_nature[5]->button,0,1,5,6);

    GW->button_nature[6]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_upright_button.png");
    GW->button_nature[6]=create_button(GW->button_nature[6]);
    add_to_table(GW->table_animaux,GW->button_nature[6]->button,1,2,5,6);

    GW->button_nature[7]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_downleft_button.png");
    GW->button_nature[7]=create_button(GW->button_nature[7]);
    add_to_table(GW->table_animaux,GW->button_nature[7]->button,2,3,5,6);

    GW->button_nature[8]=init_button(GW->table_animaux->table,NULL,NULL,"images/watergrass_downright_button.png");
    GW->button_nature[8]=create_button(GW->button_nature[8]);
    add_to_table(GW->table_animaux,GW->button_nature[8]->button,3,4,5,6);

    GW->button_nature[9]=init_button(GW->table_animaux->table,NULL,NULL,"images/water_button.png");
    GW->button_nature[9]=create_button(GW->button_nature[9]);
    add_to_table(GW->table_animaux,GW->button_nature[9]->button,0,1,7,8);

    GW->button_nature[10]=init_button(GW->table_animaux->table,NULL,NULL,"images/rock_button.png");
    GW->button_nature[10]=create_button(GW->button_nature[10]);
    add_to_table(GW->table_animaux,GW->button_nature[10]->button,0,1,6,7);

    GW->button_nature[12]=init_button(GW->table_animaux->table,NULL,NULL,"images/deep water_button.png");
    GW->button_nature[12]=create_button(GW->button_nature[12]);
    add_to_table(GW->table_animaux,GW->button_nature[12]->button,1,2,7,8);

    GW->supprimer=init_button(GW->table_animaux->table,NULL,NULL,"images/delete.png");
    GW->supprimer=create_button(GW->supprimer);
    add_to_table(GW->table_animaux,GW->supprimer->button,1,2,9,10);

}

/** Fonction d'initialisation et de creation de
    le structure du jeu **/
animals_game* init_animals_game()
{
    //Allocation dynamique
   animals_game *GA=(animals_game*)malloc(sizeof(animals_game));
   if(!GA)
   {
       printf("erreur d'allocation !!");
       exit(EXIT_FAILURE);
   }
   //Initialiser la grille
   grille_acc_alt *GL=NULL;
   GL=init_grille_animals();
   GA->ma_grille=GL;

   //Initialiser la structure de parametrage
   rule_anim *RA=init_rule_anim();
   event_calls_ca(RA);
   GA->rules=RA;

   create_interface_animals(GA);

   //Affecter les images aux boutons
    GA->element[0]=cairo_image_surface_create_from_png("images/lion_grass.png");
    GA->element[1]=cairo_image_surface_create_from_png("images/leopard_grass.png");
    GA->element[2]=cairo_image_surface_create_from_png("images/fox_grass.png");
    GA->element[3]=cairo_image_surface_create_from_png("images/wolf_grass.png");
    GA->element[4]=cairo_image_surface_create_from_png("images/hyena_grass.png");


    GA->element[7]=cairo_image_surface_create_from_png("images/gazelle_grass.png");
    GA->element[8]=cairo_image_surface_create_from_png("images/zebra_grass.png");
    GA->element[9]=cairo_image_surface_create_from_png("images/sheep_grass.png");

   GA->nature_elements[0]=cairo_image_surface_create_from_png("images/grass.png");
   GA->nature_elements[1]=cairo_image_surface_create_from_png("images/watergrass_up.png");
   GA->nature_elements[2]=cairo_image_surface_create_from_png("images/watergrass_down.png");
   GA->nature_elements[3]=cairo_image_surface_create_from_png("images/watergrass_left.png");
   GA->nature_elements[4]=cairo_image_surface_create_from_png("images/watergrass_right.png");
   GA->nature_elements[5]=cairo_image_surface_create_from_png("images/watergrass_upleft.png");
   GA->nature_elements[6]=cairo_image_surface_create_from_png("images/watergrass_upright.png");
   GA->nature_elements[7]=cairo_image_surface_create_from_png("images/watergrass_downleft.png");
   GA->nature_elements[8]=cairo_image_surface_create_from_png("images/watergrass_downright.png");
   GA->nature_elements[9]=cairo_image_surface_create_from_png("images/water.png");
   GA->nature_elements[10]=cairo_image_surface_create_from_png("images/rock.png");
   //GA->nature_elements[11]=cairo_image_surface_create_from_png("images/shit.png");
   GA->nature_elements[12]=cairo_image_surface_create_from_png("images/deep water.png");

   GA->etat=FALSE;
   GA->timer=1000;
   GA->choix=0;
   GA->maliste=NULL;

   return ((animals_game*)GA);
}

/**Une fonction qui permet de remplir une cellule
   Entree: outil de desin cairo
           position x dela cellule à dessiner
           position y de la cellule à dessine
           La structure du jeu
   Sortie: Le dessin d'une cellule remplie d'une photo
**/
void draw_cel_animals(cairo_t* CR, gint posx, gint posy, gpointer data)
{
    animals_game* GW = (animals_game*)data;

     switch(GW->ma_grille->grid[posy][posx])
    {
        //premier etat de la cellule 'blanc'
        case 0 : gdk_cairo_set_source_color(CR,&GW->ma_grille->color_cell); break;

        case 1 : {cairo_set_source_surface(CR,GW->element[0],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 2 : {cairo_set_source_surface(CR,GW->element[1],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 3 : {cairo_set_source_surface(CR,GW->element[2],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 4 : {cairo_set_source_surface(CR,GW->element[3],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 5 : {cairo_set_source_surface(CR,GW->element[4],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 6 : {cairo_set_source_surface(CR,GW->element[5],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 7 : {cairo_set_source_surface(CR,GW->element[6],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 8 : {cairo_set_source_surface(CR,GW->element[7],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 9 : {cairo_set_source_surface(CR,GW->element[8],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 10 : {cairo_set_source_surface(CR,GW->element[9],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 11 : {cairo_set_source_surface(CR,GW->nature_elements[0],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size);
                  cairo_paint(CR);}break ;

        case 12 : {cairo_set_source_surface(CR,GW->nature_elements[1],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 13 : {cairo_set_source_surface(CR,GW->nature_elements[2],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 14 : {cairo_set_source_surface(CR,GW->nature_elements[3],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 15 : {cairo_set_source_surface(CR,GW->nature_elements[4],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 16 : {cairo_set_source_surface(CR,GW->nature_elements[5],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 17 : {cairo_set_source_surface(CR,GW->nature_elements[6],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 18 : {cairo_set_source_surface(CR,GW->nature_elements[7],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 19 : {cairo_set_source_surface(CR,GW->nature_elements[8],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 20 : {cairo_set_source_surface(CR,GW->nature_elements[9],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 21 : {cairo_set_source_surface(CR,GW->nature_elements[10],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 22 : {cairo_set_source_surface(CR,GW->nature_elements[11],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;

        case 23 : {cairo_set_source_surface(CR,GW->nature_elements[12],posy*GW->ma_grille->cel_size,posx*GW->ma_grille->cel_size); cairo_paint(CR);}break ;
    }


        //Definir les bordures des cellules
        cairo_set_line_width(CR,2);
        //Dessiner les carreaux
        cairo_rectangle(CR,posy*(GW->ma_grille->cel_size),posx*(GW->ma_grille->cel_size),
                        (GW->ma_grille->cel_size)-3,(GW->ma_grille->cel_size)-3);

        cairo_stroke_preserve(CR);
        cairo_fill(CR);
}

/** la fonction permet de dessiner la grille entière
    en faisant appel a la fonction de dessin de cellules **/
static void update_grid_animals(GtkWidget* widget, cairo_t* CR, gpointer data)
{
    animals_game* GW = (animals_game*)data;
    int i,j,b;
    //parcourir la grille
    for(i=0;i<GW->ma_grille->nb_cels_height;i++)
        for(j=0;j<GW->ma_grille->nb_cels_width;j++)
        {
            //Dessiner la case
            draw_cel_animals(CR,i,j,GW);
        }
}

/** Une fonction remplieune cellule d'une
    image quand on clique dessus selon
    l'animal choisi par l'utilisateur**/
static gboolean switch_animals_line(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    animals_game* GW = (animals_game*)data;
    int x,y;
    //si c'est un clique gaucher
    if (event->button == 1)
    {
        //recuperer les coordonnees
        y = (int)event->x;
        x = (int)event->y;


        //Calculer l'emplacement
        x/=(GW->ma_grille->cel_size);
        y/=(GW->ma_grille->cel_size);
       // y += GW->ma_grille->left;
          printf("%d,%d\n\n",x,y);
        //Changer la couleur de la case
        //Mettre à jour la grille
            GW->ma_grille->grid[y][x]=GW->choix;

        gtk_widget_queue_draw(GW->ma_grille->drawing_area);
    }
    return TRUE;
}

/*Fonction de rappel,quand on clique sur le bouton
  du lion , elle permet de changer la valeur du choix*/

void on_lion_button(GtkWidget *w, gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix =1;
}


/*Fonction de rappel,quand on clique sur le bouton
  du leopard , elle permet de changer la valeur du choix*/
void on_leopard_button(GtkWidget *w, gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix =2;
}

/*Fonction de rappel,quand on clique sur le bouton
  du renard , elle permet de changer la valeur du choix*/
void on_fox_button(GtkWidget *w, gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix =3;
}

/*Fonction de rappel,quand on clique sur le bouton
  du loup , elle permet de changer la valeur du choix*/
void on_wolf_button(GtkWidget *w, gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix =4;
}

/*Fonction de rappel,quand on clique sur le bouton
  du hyene, elle permet de changer la valeur du choix*/
void on_hyena_button(GtkWidget *w, gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix =5;

}

/*Fonction de rappel,quand on clique sur le bouton
  de l'élephant , elle permet de changer la valeur du choix*/
void on_elephant_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=6;

    GA->element[5]=cairo_image_surface_create_from_png("images/eleph.png");  //here
}

/*Fonction de rappel,quand on clique sur le bouton
  du lapin , elle permet de changer la valeur du choix*/
void on_lapin_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=7;

     GA->element[6]=cairo_image_surface_create_from_png("images/lapin_grass.png"); //here
}

/*Fonction de rappel,quand on clique sur le bouton
  de la gazelle , elle permet de changer la valeur du choix*/
void on_gazelle_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=8;
}

/*Fonction de rappel,quand on clique sur le bouton
  du zebre , elle permet de changer la valeur du choix*/
void on_zebra_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=9;
}

/*Fonction de rappel,quand on clique sur le bouton
  du mouton , elle permet de changer la valeur du choix*/
void on_sheep_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=10;
}

/*Fonction de rappel,quand on clique sur le bouton
  del'herbe , elle permet de changer la valeur du choix*/
void on_grass_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=11;
}

void on_watergrass_up_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=12;

}

void on_watergrass_down_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=13;

}

void on_watergrass_left_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=14;

}

void on_watergrass_right_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=15;

}

void on_watergrass_upleft_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=16;

}

void on_watergrass_upright_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=17;

}

void on_watergrass_downleft_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=18;

}

void on_watergrass_downright_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=19;

}

void on_water_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=20;

}

void on_rock_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=21;

}


void on_deepwater_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=23;

}

/*Fonction de rappel,quand on clique sur le bouton
  de supprimer , elle permet de changer la valeur du choix
  à zero et par la suite supprimer l'animaux sur lequel on clique
  dans la zone de dessin*/
void on_delete_button(GtkWidget *w,gpointer data)
{
    animals_game *GA=(animals_game*)data;

    GA->choix=11;
}


/** La fonction prinicpalede traitement
    Pour chaque cellule elle determine le nouvel emplacement
    de l'animal qui y se trouve**/
gint animals_rules(animals_game *GA,gint posx ,gint posy)
{
    int y=posx, x=posy;
    int alea=rand()%8;
    int shit_counter=rand();
    int compt=0,res=0;
    int i;
    int trouve=0 ,obstacle=0, obstacles[8]={0,0,0,0,0,0,0,0} , pred=0;
    int dist_drt=90,dist_gch=90,dist_nord=90,dist_sur=90;
    int dist_up_right=90,dist_up_left=90,dist_down_right=90,dist_down_left=90;
    switch(GA->ma_grille->grid[x][y])
    {
       case 1: {
                 //LE Lion cherche une proie à sa droite
                 while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y]==20 || GA->ma_grille->grid[x+compt][y]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x+compt][y]==2 ||
                         GA->ma_grille->grid[x+compt][y]==3 ||
                         GA->ma_grille->grid[x+compt][y]==4 ||
                         GA->ma_grille->grid[x+compt][y]==5  ||
                         GA->ma_grille->grid[x+compt][y]==7 ||
                         GA->ma_grille->grid[x+compt][y]==8 ||
                         GA->ma_grille->grid[x+compt][y]==9 ||
                         GA->ma_grille->grid[x+compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y);
                        return((int)11);
                    }
                    dist_drt=compt;
                 }
                 //Maintenant il cherche à sa gauche si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y]==20 || GA->ma_grille->grid[x-compt][y]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x-compt][y]==2 ||
                         GA->ma_grille->grid[x-compt][y]==3 ||
                         GA->ma_grille->grid[x-compt][y]==4 ||
                         GA->ma_grille->grid[x-compt][y]==5  ||
                         GA->ma_grille->grid[x-compt][y]==6  ||
                         GA->ma_grille->grid[x-compt][y]==7 ||
                         GA->ma_grille->grid[x-compt][y]==8 ||
                         GA->ma_grille->grid[x-compt][y]==9 ||
                         GA->ma_grille->grid[x-compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                        GA->ma_grille->grid[x-compt][y]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y);
                        return((int)11);
                    }
                    dist_gch=compt;
                 }
                //Maintenant il cherche au nord si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y-compt]==20 || GA->ma_grille->grid[x][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x][y-compt]==2 ||
                         GA->ma_grille->grid[x][y-compt]==3 ||
                         GA->ma_grille->grid[x][y-compt]==4 ||
                         GA->ma_grille->grid[x][y-compt]==5  ||
                         GA->ma_grille->grid[x][y-compt]==6  ||
                         GA->ma_grille->grid[x][y-compt]==7 ||
                         GA->ma_grille->grid[x][y-compt]==8 ||
                         GA->ma_grille->grid[x][y-compt]==9 ||
                         GA->ma_grille->grid[x][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                        GA->ma_grille->grid[x][y-compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x,y-compt);
                        return((int)11);
                    }
                    dist_nord=compt;
                 }
                 //voyons voyons donc le sud ! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y+compt]==20 || GA->ma_grille->grid[x][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x][y+compt]==2 ||
                         GA->ma_grille->grid[x][y+compt]==3 ||
                         GA->ma_grille->grid[x][y+compt]==4 ||
                         GA->ma_grille->grid[x][y+compt]==5  ||
                         GA->ma_grille->grid[x][y+compt]==6  ||
                         GA->ma_grille->grid[x][y+compt]==7 ||
                         GA->ma_grille->grid[x][y+compt]==8 ||
                         GA->ma_grille->grid[x][y+compt]==9 ||
                         GA->ma_grille->grid[x][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                        GA->ma_grille->grid[x][y+compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x,y+compt);
                        return((int)11);
                    }
                    dist_sur=compt;
                 }

                  //voyons voyons donc la droite en bas peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y+compt]==20 || GA->ma_grille->grid[x+compt][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x+compt][y+compt]==2 ||
                         GA->ma_grille->grid[x+compt][y+compt]==3 ||
                         GA->ma_grille->grid[x+compt][y+compt]==4 ||
                         GA->ma_grille->grid[x+compt][y+compt]==5  ||
                         GA->ma_grille->grid[x+compt][y+compt]==6  ||
                         GA->ma_grille->grid[x+compt][y+compt]==7 ||
                         GA->ma_grille->grid[x+compt][y+compt]==8 ||
                         GA->ma_grille->grid[x+compt][y+compt]==9 ||
                         GA->ma_grille->grid[x+compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y+compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y+compt);
                        return((int)11);
                    }
                    dist_down_right=compt;
                 }
                 //voyons voyons donc la droite en haut peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y-compt]==20 || GA->ma_grille->grid[x+compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x+compt][y-compt]==2 ||
                         GA->ma_grille->grid[x+compt][y-compt]==3 ||
                         GA->ma_grille->grid[x+compt][y-compt]==4 ||
                         GA->ma_grille->grid[x+compt][y-compt]==5  ||
                         GA->ma_grille->grid[x+compt][y-compt]==6  ||
                         GA->ma_grille->grid[x+compt][y-compt]==7 ||
                         GA->ma_grille->grid[x+compt][y-compt]==8 ||
                         GA->ma_grille->grid[x+compt][y-compt]==9 ||
                         GA->ma_grille->grid[x+compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y-compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y-compt);
                        return((int)11);
                    }
                    dist_up_right=compt;
                 }

                 //voyons voyons donc la gauche en haut peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y-compt]==20 || GA->ma_grille->grid[x-compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x-compt][y-compt]==2 ||
                         GA->ma_grille->grid[x-compt][y-compt]==3 ||
                         GA->ma_grille->grid[x-compt][y-compt]==4 ||
                         GA->ma_grille->grid[x-compt][y-compt]==5  ||
                         GA->ma_grille->grid[x-compt][y-compt]==6  ||
                         GA->ma_grille->grid[x-compt][y-compt]==7 ||
                         GA->ma_grille->grid[x-compt][y-compt]==8 ||
                         GA->ma_grille->grid[x-compt][y-compt]==9 ||
                         GA->ma_grille->grid[x-compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y-compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y-compt);
                        return((int)11);
                    }
                    dist_up_left=compt;
                 }

                 //voyons voyons donc la gauche en bas peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[0].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y+compt]==20 || GA->ma_grille->grid[x-compt][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x-compt][y+compt]==2 ||
                         GA->ma_grille->grid[x-compt][y+compt]==3 ||
                         GA->ma_grille->grid[x-compt][y+compt]==4 ||
                         GA->ma_grille->grid[x-compt][y+compt]==5  ||
                         GA->ma_grille->grid[x-compt][y+compt]==7 ||
                         GA->ma_grille->grid[x-compt][y+compt]==8 ||
                         GA->ma_grille->grid[x-compt][y+compt]==9 ||
                         GA->ma_grille->grid[x-compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[0].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y+compt]=1;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y+compt);
                        return((int)11);
                    }
                    dist_down_left=compt;
                 }

                 //cherchons la plus proche proie
                 res=min_dist(dist_drt,dist_gch,dist_nord,dist_sur,dist_up_right,
                              dist_up_left,dist_down_right,dist_down_left);

                 switch(res)
                 {
                     case 0: GA->ma_grille->grid[x+GA->rules->car[0].vitesse][y]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[0].vitesse,y);
                             return((int)11);
                     case 1: GA->ma_grille->grid[x-GA->rules->car[0].vitesse][y]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[0].vitesse,y);
                             return((int)11);
                     case 2: GA->ma_grille->grid[x][y-GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y-GA->rules->car[0].vitesse);
                             return((int)11);
                     case 3: GA->ma_grille->grid[x][y+GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y+GA->rules->car[0].vitesse);
                             return((int)11);
                     case 4: GA->ma_grille->grid[x+GA->rules->car[0].vitesse][y-GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[0].vitesse,
                                                       y-GA->rules->car[0].vitesse);
                             return((int)11);
                     case 5: GA->ma_grille->grid[x-GA->rules->car[0].vitesse][y-GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[0].vitesse,
                                                       y-GA->rules->car[0].vitesse);
                             return((int)11);
                     case 6: GA->ma_grille->grid[x+GA->rules->car[0].vitesse][y+GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[0].vitesse,
                                                       y+GA->rules->car[0].vitesse);
                             return((int)11);
                     case 7: GA->ma_grille->grid[x-GA->rules->car[0].vitesse][y+GA->rules->car[0].vitesse]=1;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[0].vitesse,
                                                       y+GA->rules->car[0].vitesse);
                             return((int)11);
                     case 8: switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 1;
                                             if(GA->ma_grille->grid[x-1][y]!=1 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 1;
                                              if(GA->ma_grille->grid[x+1][y]!=1 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 1;
                                              if(GA->ma_grille->grid[x][y-1]!=1 &&
                                                GA->ma_grille->grid[x][y-1]!=6 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 1;
                                               if(GA->ma_grille->grid[x][y+1]!=1 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 1;
                                               if(GA->ma_grille->grid[x-1][y-1]!=1 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 1;

                                               if(GA->ma_grille->grid[x+1][y+1]!=1 &&
                                                GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 1;
                                                if(GA->ma_grille->grid[x-1][y+1]!=1 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 1;
                                                if(GA->ma_grille->grid[x+1][y-1]!=1 &&
                                                GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=1;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)1);
                                            }break;
                                }
                             break;
                 }

                  return 1;
                } break;
       case 2: {

                    /**Le leopard cherche ses prédateurs **/
                //Tant qu'on ne depasse pas le zone &&
                //Tant qu'on est dans la portee de vue du leopard
                while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    //S'il rencotre un lion
                    if(GA->ma_grille->grid[x+compt][y]==1) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==2  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        (x-compt)==0) )obstacles[0]=1;
                    if((pred)||(obstacles[0] ))break; //Si il y a un predateur ou un obstacle
               }
                   //S'il trouve un prédateur a droite il fuit a gauche
                   if(pred==1)
                 {
                    if((x-GA->rules->car[1].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[1].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==2  ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[1].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[1].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==2  ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[1].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[1].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;
                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==2  ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[1].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[1].vitesse);
                        return((int)11);
                     }
                 }

                  // chercher dans la gauche en haut
                 compt=0;
                 trouve=0;
              //   obstacle=0;
                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==2  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4] =1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[1].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[1].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y+GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[1].vitesse,y+GA->rules->car[1].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans la droite en bas
                 compt=0;
                 trouve=0;
                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==2  ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[1].vitesse)>=0)&&
                    ((y-GA->rules->car[1].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y-GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[1].vitesse,y-GA->rules->car[1].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans la droite en haut

                compt=0;
                 trouve=0;
                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==2  ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }
             /**  if(obstacle==1)// s'il y a un obstacle il s'arrete
               {
                  return ((int)2);
               }
               else
               {  **/
                  if(pred)
                   {
                 if(((x-GA->rules->car[1].vitesse)>=0)&&
                    ((y+GA->rules->car[1].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y+GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[1].vitesse,y+GA->rules->car[1].vitesse);
                        return((int)11);
                    }
                   }

                // chercher dans la gauche en bas
                 compt=0;
                 trouve=0;
                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[1].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==2  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[1].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[1].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y-GA->rules->car[1].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[1].vitesse,y-GA->rules->car[1].vitesse);
                        return((int)11);
                   }
                  }

                    int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 2;

                    /** Il cherche alors des proies **/

                     while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6 ||
                        GA->ma_grille->grid[x+compt][y]==2 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y]==3 ||
                         GA->ma_grille->grid[x+compt][y]==7 ||
                         GA->ma_grille->grid[x+compt][y]==8 ||
                         GA->ma_grille->grid[x+compt][y]==9 ||
                         GA->ma_grille->grid[x+compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y);
                        return((int)11);
                    }
                    dist_drt=compt;
                 }
                 //Maintenant il cherche à sa gauche si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y]==20 ||
                         GA->ma_grille->grid[x-compt][y]==6 ||
                         GA->ma_grille->grid[x-compt][y]==2)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y]==3 ||
                         GA->ma_grille->grid[x-compt][y]==7 ||
                         GA->ma_grille->grid[x-compt][y]==8 ||
                         GA->ma_grille->grid[x-compt][y]==9 ||
                         GA->ma_grille->grid[x-compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                        GA->ma_grille->grid[x-compt][y]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y);
                        return((int)11);
                    }
                    dist_gch=compt;
                 }
                //Maintenant il cherche au nord si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==2)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y-compt]==3 ||
                         GA->ma_grille->grid[x][y-compt]==7 ||
                         GA->ma_grille->grid[x][y-compt]==8 ||
                         GA->ma_grille->grid[x][y-compt]==9 ||
                         GA->ma_grille->grid[x][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                        GA->ma_grille->grid[x][y-compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x,y-compt);
                        return((int)11);
                    }
                    dist_nord=compt;
                 }
                 //voyons voyons donc le sud ! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6 ||
                        GA->ma_grille->grid[x][y+compt]==2)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y+compt]==3 ||
                         GA->ma_grille->grid[x][y+compt]==7 ||
                         GA->ma_grille->grid[x][y+compt]==8 ||
                         GA->ma_grille->grid[x][y+compt]==9 ||
                         GA->ma_grille->grid[x][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                        GA->ma_grille->grid[x][y+compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x,y+compt);
                        return((int)11);
                    }
                    dist_sur=compt;
                 }

                  //voyons voyons donc le ->DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==2 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y+compt]==3 ||
                         GA->ma_grille->grid[x+compt][y+compt]==7 ||
                         GA->ma_grille->grid[x+compt][y+compt]==8 ||
                         GA->ma_grille->grid[x+compt][y+compt]==9 ||
                         GA->ma_grille->grid[x+compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y+compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y+compt);
                        return((int)11);
                    }
                    dist_down_right=compt;
                 }
                 //voyons voyons donc le ->UP! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==2 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y-compt]==3 ||
                         GA->ma_grille->grid[x+compt][y-compt]==7 ||
                         GA->ma_grille->grid[x+compt][y-compt]==8 ||
                         GA->ma_grille->grid[x+compt][y-compt]==9 ||
                         GA->ma_grille->grid[x+compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y-compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y-compt);
                        return((int)11);
                    }
                    dist_up_right=compt;
                 }

                 //voyons voyons donc le UP LEFT! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==2 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y-compt]==3 ||
                         GA->ma_grille->grid[x-compt][y-compt]==7 ||
                         GA->ma_grille->grid[x-compt][y-compt]==8 ||
                         GA->ma_grille->grid[x-compt][y-compt]==9 ||
                         GA->ma_grille->grid[x-compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y-compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y-compt);
                        return((int)11);
                    }
                    dist_up_left=compt;
                 }

                 //voyons voyons donc le LEFT DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[1].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==2 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y+compt]==3 ||
                         GA->ma_grille->grid[x-compt][y+compt]==7 ||
                         GA->ma_grille->grid[x-compt][y+compt]==8 ||
                         GA->ma_grille->grid[x-compt][y+compt]==9 ||
                         GA->ma_grille->grid[x-compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[1].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y+compt]=2;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y+compt);
                        return((int)11);
                    }
                    dist_down_left=compt;
                 }

                 //cherchons la plus proche proie
                 res=min_dist(dist_drt,dist_gch,dist_nord,dist_sur,dist_up_right,
                              dist_up_left,dist_down_right,dist_down_left);

                 switch(res)
                 {
                     case 0: GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[1].vitesse,y);
                             return((int)11);
                     case 1: GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[1].vitesse,y);
                             return((int)11);
                     case 2: GA->ma_grille->grid[x][y-GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y-GA->rules->car[1].vitesse);
                             return((int)11);
                     case 3: GA->ma_grille->grid[x][y+GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y+GA->rules->car[1].vitesse);
                             return((int)11);
                     case 4: GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y-GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[1].vitesse,
                                                       y-GA->rules->car[1].vitesse);
                             return((int)11);
                     case 5: GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y-GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[1].vitesse,
                                                       y-GA->rules->car[1].vitesse);
                             return((int)11);
                     case 6: GA->ma_grille->grid[x+GA->rules->car[1].vitesse][y+GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[1].vitesse,
                                                       y+GA->rules->car[1].vitesse);
                             return((int)11);
                     case 7: GA->ma_grille->grid[x-GA->rules->car[1].vitesse][y+GA->rules->car[1].vitesse]=2;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[1].vitesse,
                                                       y+GA->rules->car[1].vitesse);
                             return((int)11);
                     case 8: switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 2;
                                             if(GA->ma_grille->grid[x-1][y]!=2 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 2;
                                              if(GA->ma_grille->grid[x+1][y]!=2 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 2;
                                              if(GA->ma_grille->grid[x][y-1]!=2 &&
                                                GA->ma_grille->grid[x][y-1]!=6 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 2;
                                               if(GA->ma_grille->grid[x][y+1]!=2 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 2;
                                               if(GA->ma_grille->grid[x-1][y-1]!=2 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 2;

                                               if(GA->ma_grille->grid[x+1][y+1]!=2 &&
                                                GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 2;
                                                if(GA->ma_grille->grid[x-1][y+1]!=2 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 2;
                                                if(GA->ma_grille->grid[x+1][y-1]!=2 &&
                                                GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=2;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)2);
                                            }break;
                                }
                             break;
                 }

                  return 2;

               }break;
       case 3: {
                                 for(int j=0;j<8;j++)obstacles[j]=0;

                         while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x+compt][y]==1 ||
                       GA->ma_grille->grid[x+compt][y]==2 ||
                       GA->ma_grille->grid[x+compt][y]==4 ||
                       GA->ma_grille->grid[x+compt][y]==5) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==3  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        GA->ma_grille->grid[x-compt][y]==8  ||
                        GA->ma_grille->grid[x-compt][y]==9  ||
                        GA->ma_grille->grid[x-compt][y]==10  ||

                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[2].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[2].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x-compt][y]==1 ||
                       GA->ma_grille->grid[x-compt][y]==2 ||
                       GA->ma_grille->grid[x-compt][y]==4 ||
                       GA->ma_grille->grid[x-compt][y]==5 ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==3  ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==8  ||
                        GA->ma_grille->grid[x+compt][y]==9  ||
                        GA->ma_grille->grid[x+compt][y]==10  ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1]=1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[2].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[2].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if( GA->ma_grille->grid[x][y+compt]==1 ||
                       GA->ma_grille->grid[x][y+compt]==2 ||
                       GA->ma_grille->grid[x][y+compt]==4 ||
                       GA->ma_grille->grid[x][y+compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==3  ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10  ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[2].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[2].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x][y-compt]==1 ||
                       GA->ma_grille->grid[x][y-compt]==2 ||
                       GA->ma_grille->grid[x][y-compt]==4 ||
                       GA->ma_grille->grid[x][y-compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==3  ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==8  ||
                        GA->ma_grille->grid[x][y+compt]==9  ||
                        GA->ma_grille->grid[x][y+compt]==10  ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[2].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[2].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x-compt][y-compt]==1 ||
                       GA->ma_grille->grid[x-compt][y-compt]==2 ||
                       GA->ma_grille->grid[x-compt][y-compt]==4 ||
                       GA->ma_grille->grid[x-compt][y-compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==3  ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4] =1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[2].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[2].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y+GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[2].vitesse,y+GA->rules->car[2].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x+compt][y+compt]==1 ||
                       GA->ma_grille->grid[x+compt][y+compt]==2 ||
                       GA->ma_grille->grid[x+compt][y+compt]==4 ||
                       GA->ma_grille->grid[x+compt][y+compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==3  ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==8  ||
                        GA->ma_grille->grid[x-compt][y-compt]==9  ||
                        GA->ma_grille->grid[x-compt][y-compt]==10  ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[2].vitesse)>=0)&&
                    ((y-GA->rules->car[2].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y-GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[2].vitesse,y-GA->rules->car[2].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x+compt][y-compt]==1 ||
                       GA->ma_grille->grid[x+compt][y-compt]==2  ||
                       GA->ma_grille->grid[x+compt][y-compt]==4 ||
                       GA->ma_grille->grid[x+compt][y-compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==3  ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==8  ||
                        GA->ma_grille->grid[x-compt][y+compt]==9  ||
                        GA->ma_grille->grid[x-compt][y+compt]==10  ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[2].vitesse)>=0)&&
                    ((y+GA->rules->car[2].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y+GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[2].vitesse,y+GA->rules->car[2].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[2].p_vue )
               {
                    compt++;
                    if(GA->ma_grille->grid[x-compt][y+compt]==1 ||
                       GA->ma_grille->grid[x-compt][y+compt]==2 ||
                       GA->ma_grille->grid[x-compt][y+compt]==4 ||
                       GA->ma_grille->grid[x-compt][y+compt]==5 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==3  ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==8  ||
                        GA->ma_grille->grid[x+compt][y-compt]==9  ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[2].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[2].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y-GA->rules->car[2].vitesse]=3;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[2].vitesse,y-GA->rules->car[2].vitesse);
                        return((int)11);
                   }
                  }
                      int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 3;


                     while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==3 ||
                        GA->ma_grille->grid[x+compt][y]==6 ||
                        GA->ma_grille->grid[x+compt][y]==8 ||
                        GA->ma_grille->grid[x+compt][y]==9 ||
                        GA->ma_grille->grid[x+compt][y]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x+compt][y]==7 ) ) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y);
                        return((int)11);
                    }
                    dist_drt=compt;
                 }
                 //Maintenant il cherche à sa gauche si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y]==20 ||
                         GA->ma_grille->grid[x-compt][y]==6 ||
                         GA->ma_grille->grid[x-compt][y]==8 ||
                         GA->ma_grille->grid[x-compt][y]==9 ||
                         GA->ma_grille->grid[x-compt][y]==10 ||
                         GA->ma_grille->grid[x-compt][y]==3 )
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x-compt][y]==7 )) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                        GA->ma_grille->grid[x-compt][y]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y);
                        return((int)11);
                    }
                    dist_gch=compt;
                 }
                //Maintenant il cherche au nord si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==3  ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         (GA->ma_grille->grid[x][y-compt]==7 ) ) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                        GA->ma_grille->grid[x][y-compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x,y-compt);
                        return((int)11);
                    }
                    dist_nord=compt;
                 }
                 //voyons voyons donc le sud ! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==3 ||
                        GA->ma_grille->grid[x][y+compt]==6 ||
                        GA->ma_grille->grid[x][y+compt]==8 ||
                        GA->ma_grille->grid[x][y+compt]==9 ||
                        GA->ma_grille->grid[x][y+compt]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         ( GA->ma_grille->grid[x][y+compt]==7  ) ) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                        GA->ma_grille->grid[x][y+compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x,y+compt);
                        return((int)11);
                    }
                    dist_sur=compt;
                 }

                  //voyons voyons donc le ->DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==3  ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y+compt]==7  )) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y+compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y+compt);
                        return((int)11);
                    }
                    dist_down_right=compt;
                 }
                 //voyons voyons donc le ->UP! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==3 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6 ||
                        GA->ma_grille->grid[x+compt][y-compt]==8 ||
                        GA->ma_grille->grid[x+compt][y-compt]==9 ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y-compt]==7 )) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y-compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y-compt);
                        return((int)11);
                    }
                    dist_up_right=compt;
                 }

                 //voyons voyons donc le UP LEFT! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==3 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6 ||
                        GA->ma_grille->grid[x-compt][y-compt]==8 ||
                        GA->ma_grille->grid[x-compt][y-compt]==9 ||
                        GA->ma_grille->grid[x-compt][y-compt]==10 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y-compt]==7 )) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y-compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y-compt);
                        return((int)11);
                    }
                    dist_up_left=compt;
                 }

                 //voyons voyons donc le LEFT DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[2].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==3 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6 ||
                        GA->ma_grille->grid[x-compt][y+compt]==8 ||
                        GA->ma_grille->grid[x-compt][y+compt]==9 ||
                        GA->ma_grille->grid[x-compt][y+compt]==10)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y+compt]==7 )) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[2].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y+compt]=3;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y+compt);
                        return((int)11);
                    }
                    dist_down_left=compt;
                 }

                 //cherchons la plus proche proie
                 res=min_dist(dist_drt,dist_gch,dist_nord,dist_sur,dist_up_right,
                              dist_up_left,dist_down_right,dist_down_left);

                 switch(res)
                 {
                     case 0: GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[2].vitesse,y);
                             return((int)11);
                     case 1: GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[2].vitesse,y);
                             return((int)11);
                     case 2: GA->ma_grille->grid[x][y-GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y-GA->rules->car[2].vitesse);
                             return((int)11);
                     case 3: GA->ma_grille->grid[x][y+GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y+GA->rules->car[2].vitesse);
                             return((int)11);
                     case 4: GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y-GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[2].vitesse,
                                                       y-GA->rules->car[2].vitesse);
                             return((int)11);
                     case 5: GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y-GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[2].vitesse,
                                                       y-GA->rules->car[2].vitesse);
                             return((int)11);
                     case 6: GA->ma_grille->grid[x+GA->rules->car[2].vitesse][y+GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[2].vitesse,
                                                       y+GA->rules->car[2].vitesse);
                             return((int)11);
                     case 7: GA->ma_grille->grid[x-GA->rules->car[2].vitesse][y+GA->rules->car[2].vitesse]=3;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[2].vitesse,
                                                       y+GA->rules->car[2].vitesse);
                             return((int)11);
                     case 8: switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 3;
                                             if(GA->ma_grille->grid[x-1][y]!=3 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 3;
                                              if(GA->ma_grille->grid[x+1][y]!=3 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=8 &&
                                                GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 3;
                                              if(GA->ma_grille->grid[x][y-1]!=3 &&
                                                GA->ma_grille->grid[x][y-1]!=6 &&
                                                GA->ma_grille->grid[x][y-1]!=8 &&
                                                GA->ma_grille->grid[x][y-1]!=9 &&
                                                GA->ma_grille->grid[x][y-1]!=10 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 3;
                                               if(GA->ma_grille->grid[x][y+1]!=3 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=8 &&
                                                GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 3;
                                               if(GA->ma_grille->grid[x-1][y-1]!=3 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 3;

                                               if(GA->ma_grille->grid[x+1][y+1]!=3 &&
                                                GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 3;
                                                if(GA->ma_grille->grid[x-1][y+1]!=3 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 3;
                                                if(GA->ma_grille->grid[x+1][y-1]!=3 &&
                                                GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=3;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)3);
                                            }break;
                                }
                             break;
                 }

                  return 3;
               }break;
       case 4: {
                        for(int j=0;j<8;j++)obstacles[j]=0;

                         while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==4  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[3].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[3].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==4  ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1]=1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[3].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[3].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==4  ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[3].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[3].vitesse]=4;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[3].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==4  ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[1].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[3].vitesse]=2;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[3].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==4  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4] =1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[3].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[3].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y+GA->rules->car[3].vitesse]=4;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[3].vitesse,y+GA->rules->car[3].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==4  ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[3].vitesse)>=0)&&
                    ((y-GA->rules->car[3].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y-GA->rules->car[3].vitesse]=4;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[3].vitesse,y-GA->rules->car[3].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==4  ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[3].vitesse)>=0)&&
                    ((y+GA->rules->car[3].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y+GA->rules->car[3].vitesse]=4;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[3].vitesse,y+GA->rules->car[3].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[3].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==4  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[3].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[3].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y-GA->rules->car[3].vitesse]=4;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[3].vitesse,y-GA->rules->car[3].vitesse);
                        return((int)11);
                   }
                  }
                      int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 4;

                     while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6 ||
                        GA->ma_grille->grid[x+compt][y]==4 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y]==3 ||
                         GA->ma_grille->grid[x+compt][y]==7 ||
                         GA->ma_grille->grid[x+compt][y]==8 ||
                         GA->ma_grille->grid[x+compt][y]==9 ||
                         GA->ma_grille->grid[x+compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y);
                        return((int)11);
                    }
                    dist_drt=compt;
                 }
                 //Maintenant il cherche à sa gauche si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y]==20 ||
                         GA->ma_grille->grid[x-compt][y]==6 ||
                         GA->ma_grille->grid[x-compt][y]==4 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y]==3 ||
                         GA->ma_grille->grid[x-compt][y]==7 ||
                         GA->ma_grille->grid[x-compt][y]==8 ||
                         GA->ma_grille->grid[x-compt][y]==9 ||
                         GA->ma_grille->grid[x-compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                        GA->ma_grille->grid[x-compt][y]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y);
                        return((int)11);
                    }
                    dist_gch=compt;
                 }
                //Maintenant il cherche au nord si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==4 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y-compt]==3 ||
                         GA->ma_grille->grid[x][y-compt]==7 ||
                         GA->ma_grille->grid[x][y-compt]==8 ||
                         GA->ma_grille->grid[x][y-compt]==9 ||
                         GA->ma_grille->grid[x][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                        GA->ma_grille->grid[x][y-compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x,y-compt);
                        return((int)11);
                    }
                    dist_nord=compt;
                 }
                 //voyons voyons donc le sud ! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6 ||
                        GA->ma_grille->grid[x][y+compt]==4)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y+compt]==3 ||
                         GA->ma_grille->grid[x][y+compt]==7 ||
                         GA->ma_grille->grid[x][y+compt]==8 ||
                         GA->ma_grille->grid[x][y+compt]==9 ||
                         GA->ma_grille->grid[x][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                        GA->ma_grille->grid[x][y+compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x,y+compt);
                        return((int)11);
                    }
                    dist_sur=compt;
                 }

                  //voyons voyons donc le ->DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==4 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y+compt]==3 ||
                         GA->ma_grille->grid[x+compt][y+compt]==7 ||
                         GA->ma_grille->grid[x+compt][y+compt]==8 ||
                         GA->ma_grille->grid[x+compt][y+compt]==9 ||
                         GA->ma_grille->grid[x+compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y+compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y+compt);
                        return((int)11);
                    }
                    dist_down_right=compt;
                 }
                 //voyons voyons donc le ->UP! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==4 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y-compt]==3 ||
                         GA->ma_grille->grid[x+compt][y-compt]==7 ||
                         GA->ma_grille->grid[x+compt][y-compt]==8 ||
                         GA->ma_grille->grid[x+compt][y-compt]==9 ||
                         GA->ma_grille->grid[x+compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y-compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y-compt);
                        return((int)11);
                    }
                    dist_up_right=compt;
                 }

                 //voyons voyons donc le UP LEFT! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==4 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y-compt]==3 ||
                         GA->ma_grille->grid[x-compt][y-compt]==7 ||
                         GA->ma_grille->grid[x-compt][y-compt]==8 ||
                         GA->ma_grille->grid[x-compt][y-compt]==9 ||
                         GA->ma_grille->grid[x-compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y-compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y-compt);
                        return((int)11);
                    }
                    dist_up_left=compt;
                 }

                 //voyons voyons donc le LEFT DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[3].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==4 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y+compt]==3 ||
                         GA->ma_grille->grid[x-compt][y+compt]==7 ||
                         GA->ma_grille->grid[x-compt][y+compt]==8 ||
                         GA->ma_grille->grid[x-compt][y+compt]==9 ||
                         GA->ma_grille->grid[x-compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[3].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y+compt]=4;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y+compt);
                        return((int)11);
                    }
                    dist_down_left=compt;
                 }

                 //cherchons la plus proche proie
                 res=min_dist(dist_drt,dist_gch,dist_nord,dist_sur,dist_up_right,
                              dist_up_left,dist_down_right,dist_down_left);

                 switch(res)
                 {
                     case 0: GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[3].vitesse,y);
                             return((int)11);
                     case 1: GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[3].vitesse,y);
                             return((int)11);
                     case 2: GA->ma_grille->grid[x][y-GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y-GA->rules->car[3].vitesse);
                             return((int)11);
                     case 3: GA->ma_grille->grid[x][y+GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y+GA->rules->car[3].vitesse);
                             return((int)11);
                     case 4: GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y-GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[3].vitesse,
                                                       y-GA->rules->car[3].vitesse);
                             return((int)11);
                     case 5: GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y-GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[3].vitesse,
                                                       y-GA->rules->car[3].vitesse);
                             return((int)11);
                     case 6: GA->ma_grille->grid[x+GA->rules->car[3].vitesse][y+GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[3].vitesse,
                                                       y+GA->rules->car[3].vitesse);
                             return((int)11);
                     case 7: GA->ma_grille->grid[x-GA->rules->car[3].vitesse][y+GA->rules->car[3].vitesse]=4;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[3].vitesse,
                                                       y+GA->rules->car[3].vitesse);
                             return((int)11);
                     case 8: switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 4;
                                             if(GA->ma_grille->grid[x-1][y]!=4 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 4;
                                              if(GA->ma_grille->grid[x+1][y]!=4 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 4;
                                              if(GA->ma_grille->grid[x][y-1]!=4 &&
                                                GA->ma_grille->grid[x][y-1]!=6 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 4;
                                               if(GA->ma_grille->grid[x][y+1]!=4 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 4;
                                               if(GA->ma_grille->grid[x-1][y-1]!=4 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 4;

                                               if(GA->ma_grille->grid[x+1][y+1]!=4 &&
                                                GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 4;
                                                if(GA->ma_grille->grid[x-1][y+1]!=4 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 4;
                                                if(GA->ma_grille->grid[x+1][y-1]!=4 &&
                                                GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=4;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)4);
                                            }break;
                                }
                             break;
                 }

                  return 4;
               }break;
       case 5: {
                            for(int j=0; j<8;j++)obstacles[j]=0;

                         while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==5  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[4].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[4].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==5  ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[4].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[4].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==5  ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[4].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[4].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==5  ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3]=1;
                    if((pred)||(obstacles[3]))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[4].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[4].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==5  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4]=1;
                    if((pred)||(obstacles[4]))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[4].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[4].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y+GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[4].vitesse,y+GA->rules->car[4].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==5  ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5]=1;
                    if((pred)||(obstacles[5]))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[4].vitesse)>=0)&&
                    ((y-GA->rules->car[4].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y-GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[4].vitesse,y-GA->rules->car[4].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==5  ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6]=1;
                    if((pred)||(obstacles[6]))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[4].vitesse)>=0)&&
                    ((y+GA->rules->car[4].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y+GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[4].vitesse,y+GA->rules->car[4].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[4].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==5  ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7]=1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[4].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[4].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y-GA->rules->car[4].vitesse]=5;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[4].vitesse,y-GA->rules->car[4].vitesse);
                        return((int)11);
                   }
                  }
                        int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 5;

                     while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6 ||
                        GA->ma_grille->grid[x+compt][y]==5 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y]==3 ||
                         GA->ma_grille->grid[x+compt][y]==7 ||
                         GA->ma_grille->grid[x+compt][y]==8 ||
                         GA->ma_grille->grid[x+compt][y]==9 ||
                         GA->ma_grille->grid[x+compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y);
                        return((int)11);
                    }
                    dist_drt=compt;
                 }
                 //Maintenant il cherche à sa gauche si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y]==20 ||
                         GA->ma_grille->grid[x-compt][y]==6 ||
                         GA->ma_grille->grid[x-compt][y]==5 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y]==3 ||
                         GA->ma_grille->grid[x-compt][y]==7 ||
                         GA->ma_grille->grid[x-compt][y]==8 ||
                         GA->ma_grille->grid[x-compt][y]==9 ||
                         GA->ma_grille->grid[x-compt][y]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                        GA->ma_grille->grid[x-compt][y]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y);
                        return((int)11);
                    }
                    dist_gch=compt;
                 }
                //Maintenant il cherche au nord si il y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==5 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y-compt]==3 ||
                         GA->ma_grille->grid[x][y-compt]==7 ||
                         GA->ma_grille->grid[x][y-compt]==8 ||
                         GA->ma_grille->grid[x][y-compt]==9 ||
                         GA->ma_grille->grid[x][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                        GA->ma_grille->grid[x][y-compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x,y-compt);
                        return((int)11);
                    }
                    dist_nord=compt;
                 }
                 //voyons voyons donc le sud ! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6 ||
                        GA->ma_grille->grid[x][y+compt]==5)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x][y+compt]==3 ||
                         GA->ma_grille->grid[x][y+compt]==7 ||
                         GA->ma_grille->grid[x][y+compt]==8 ||
                         GA->ma_grille->grid[x][y+compt]==9 ||
                         GA->ma_grille->grid[x][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                        GA->ma_grille->grid[x][y+compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x,y+compt);
                        return((int)11);
                    }
                    dist_sur=compt;
                 }

                  //voyons voyons donc le ->DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==5 )
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y+compt]==3 ||
                         GA->ma_grille->grid[x+compt][y+compt]==7 ||
                         GA->ma_grille->grid[x+compt][y+compt]==8 ||
                         GA->ma_grille->grid[x+compt][y+compt]==9 ||
                         GA->ma_grille->grid[x+compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y+compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y+compt);
                        return((int)11);
                    }
                    dist_down_right=compt;
                 }
                 //voyons voyons donc le ->UP! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==5 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x+compt][y-compt]==3 ||
                         GA->ma_grille->grid[x+compt][y-compt]==7 ||
                         GA->ma_grille->grid[x+compt][y-compt]==8 ||
                         GA->ma_grille->grid[x+compt][y-compt]==9 ||
                         GA->ma_grille->grid[x+compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                         GA->ma_grille->grid[x+compt][y-compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x+compt,y-compt);
                        return((int)11);
                    }
                    dist_up_right=compt;
                 }

                 //voyons voyons donc le UP LEFT! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y-compt)>0) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==5 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y-compt]==3 ||
                         GA->ma_grille->grid[x-compt][y-compt]==7 ||
                         GA->ma_grille->grid[x-compt][y-compt]==8 ||
                         GA->ma_grille->grid[x-compt][y-compt]==9 ||
                         GA->ma_grille->grid[x-compt][y-compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y-compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y-compt);
                        return((int)11);
                    }
                    dist_up_left=compt;
                 }

                 //voyons voyons donc le LEFT DOWN! peut etre y a une proie plus proche
                 compt=0;
                 trouve=0;
                 obstacle=0;
                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          ((x-compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[4].p_vue )
                 {
                     compt++;
                     if(GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==5 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6)
                        obstacle=1;
                     if((!obstacle) &&
                         (
                         GA->ma_grille->grid[x-compt][y+compt]==3 ||
                         GA->ma_grille->grid[x-compt][y+compt]==7 ||
                         GA->ma_grille->grid[x-compt][y+compt]==8 ||
                         GA->ma_grille->grid[x-compt][y+compt]==9 ||
                         GA->ma_grille->grid[x-compt][y+compt]==10)) trouve=1;
                     if(obstacle ==1 || trouve==1) break;
                 }
                 if(trouve==1)
                 {
                    if(compt<=GA->rules->car[4].vitesse)
                    {
                         GA->ma_grille->grid[x-compt][y+compt]=5;
                         GA->maliste=ajouter_coord(GA->maliste,x-compt,y+compt);
                        return((int)11);
                    }
                    dist_down_left=compt;
                 }

                 //cherchons la plus proche proie
                 res=min_dist(dist_drt,dist_gch,dist_nord,dist_sur,dist_up_right,
                              dist_up_left,dist_down_right,dist_down_left);

                 switch(res)
                 {
                     case 0: GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[4].vitesse,y);
                             return((int)11);
                     case 1: GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[4].vitesse,y);
                             return((int)11);
                     case 2: GA->ma_grille->grid[x][y-GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y-GA->rules->car[4].vitesse);
                             return((int)11);
                     case 3: GA->ma_grille->grid[x][y+GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x,y+GA->rules->car[4].vitesse);
                             return((int)11);
                     case 4: GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y-GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[4].vitesse,
                                                       y-GA->rules->car[4].vitesse);
                             return((int)11);
                     case 5: GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y-GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[4].vitesse,
                                                       y-GA->rules->car[4].vitesse);
                             return((int)11);
                     case 6: GA->ma_grille->grid[x+GA->rules->car[4].vitesse][y+GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x+GA->rules->car[4].vitesse,
                                                       y+GA->rules->car[4].vitesse);
                             return((int)11);
                     case 7: GA->ma_grille->grid[x-GA->rules->car[4].vitesse][y+GA->rules->car[4].vitesse]=5;
                             GA->maliste=ajouter_coord(GA->maliste,
                                                       x-GA->rules->car[4].vitesse,
                                                       y+GA->rules->car[4].vitesse);
                             return((int)11);
                     case 8: switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 5;
                                             if(GA->ma_grille->grid[x-1][y]!=5 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 5;
                                              if(GA->ma_grille->grid[x+1][y]!=5 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 5;
                                              if(GA->ma_grille->grid[x][y-1]!=5 &&
                                                GA->ma_grille->grid[x][y-1]!=6 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 5;
                                               if(GA->ma_grille->grid[x][y+1]!=5 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 5;
                                               if(GA->ma_grille->grid[x-1][y-1]!=5 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 5;

                                               if(GA->ma_grille->grid[x+1][y+1]!=5 &&
                                                GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 5;
                                                if(GA->ma_grille->grid[x-1][y+1]!=5 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 5;
                                                if(GA->ma_grille->grid[x+1][y-1]!=5 &&
                                                GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=5;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)5);
                                            }break;
                                }
                             break;
                 }

                  return 5;
               }break;
       case 6: {
                        switch(alea)
                                {
                                   case 0:
                                           {
                                             if(x==0)return 6;
                                             if(GA->ma_grille->grid[x-1][y]!=1 &&
                                                GA->ma_grille->grid[x-1][y]!=2 &&
                                                GA->ma_grille->grid[x-1][y]!=3 &&
                                                GA->ma_grille->grid[x-1][y]!=4 &&
                                                GA->ma_grille->grid[x-1][y]!=5 &&
                                                GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=7 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                           }break;
                                   case 1:
                                            {
                                              if(x==GA->ma_grille->nb_cels_width-1)return 6;
                                              if(GA->ma_grille->grid[x+1][y]!=1 &&
                                                 GA->ma_grille->grid[x+1][y]!=2 &&
                                                 GA->ma_grille->grid[x+1][y]!=3 &&
                                                 GA->ma_grille->grid[x+1][y]!=4 &&
                                                 GA->ma_grille->grid[x+1][y]!=5 &&
                                                GA->ma_grille->grid[x+1][y]!=6 &&
                                                GA->ma_grille->grid[x+1][y]!=7 &&
                                                GA->ma_grille->grid[x+1][y]!=8 &&
                                                GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 6;
                                              if(GA->ma_grille->grid[x][y-1]!=1 &&
                                                 GA->ma_grille->grid[x][y-1]!=2 &&
                                                 GA->ma_grille->grid[x][y-1]!=3 &&
                                                 GA->ma_grille->grid[x][y-1]!=4 &&
                                                 GA->ma_grille->grid[x][y-1]!=5 &&
                                                 GA->ma_grille->grid[x][y-1]!=6 &&
                                                 GA->ma_grille->grid[x][y-1]!=7 &&
                                                 GA->ma_grille->grid[x][y-1]!=8 &&
                                                 GA->ma_grille->grid[x][y-1]!=9 &&
                                                 GA->ma_grille->grid[x][y-1]!=10 &&
                                                 GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 3:
                                            {
                                               if(y==GA->ma_grille->nb_cels_height-1)return 6;
                                               if(GA->ma_grille->grid[x][y+1]!=1 &&
                                                  GA->ma_grille->grid[x][y+1]!=2 &&
                                                  GA->ma_grille->grid[x][y+1]!=3 &&
                                                  GA->ma_grille->grid[x][y+1]!=4 &&
                                                  GA->ma_grille->grid[x][y+1]!=5 &&
                                                GA->ma_grille->grid[x][y+1]!=6 &&
                                                GA->ma_grille->grid[x][y+1]!=7 &&
                                                GA->ma_grille->grid[x][y+1]!=8 &&
                                                GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 4:
                                            {
                                                if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 6;
                                               if(GA->ma_grille->grid[x-1][y-1]!=1 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=2 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=3 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=4 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=5 &&
                                                GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                GA->ma_grille->grid[x-1][y-1]!=7 &&
                                                GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 5:
                                            {
                                               if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 6;

                                               if(GA->ma_grille->grid[x+1][y+1]!=1 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=2 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=3 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=4 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=5 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=7 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 6;
                                                if(GA->ma_grille->grid[x-1][y+1]!=1 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=2 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=3 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=4 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=5 &&
                                                GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                GA->ma_grille->grid[x-1][y+1]!=7 &&
                                                GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                                   case 7:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 6;
                                                if(GA->ma_grille->grid[x+1][y-1]!=1 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=2 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=3 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=4 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=5 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=7 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=6;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)6);
                                            }break;
                           }break; // switch aleatoire
                   }break;  // case: 5
       case 7: {
                         for(int j=0; j<8 ; j++)obstacles[j]=0;
                //le lapin cherche si il y a un predateur a sa droite ou un obstacle a sa gauche
               while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1 ||
                       GA->ma_grille->grid[x+compt][y]==2 ||
                       GA->ma_grille->grid[x+compt][y]==3 ||
                       GA->ma_grille->grid[x+compt][y]==4 ||
                       GA->ma_grille->grid[x+compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==7  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        GA->ma_grille->grid[x-compt][y]==8  ||
                        GA->ma_grille->grid[x-compt][y]==9  ||
                        GA->ma_grille->grid[x-compt][y]==10 ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0]))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[6].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[6].vitesse][y]=7;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[6].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ||
                       GA->ma_grille->grid[x-compt][y]==2 ||
                       GA->ma_grille->grid[x-compt][y]==3 ||
                       GA->ma_grille->grid[x-compt][y]==4 ||
                       GA->ma_grille->grid[x-compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==7  ||
                        GA->ma_grille->grid[x+compt][y]==8  ||
                        GA->ma_grille->grid[x+compt][y]==9  ||
                        GA->ma_grille->grid[x+compt][y]==10 ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[6].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[6].vitesse][y]=7;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[6].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ||
                       GA->ma_grille->grid[x][y+compt]==2 ||
                       GA->ma_grille->grid[x][y+compt]==3 ||
                       GA->ma_grille->grid[x][y+compt]==4 ||
                       GA->ma_grille->grid[x][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==7  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10 ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[6].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[6].vitesse);
                        return((int)11);
                     }
                 }

                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ||
                       GA->ma_grille->grid[x][y-compt]==2 ||
                       GA->ma_grille->grid[x][y-compt]==3 ||
                       GA->ma_grille->grid[x][y-compt]==4 ||
                       GA->ma_grille->grid[x][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==7  ||
                        GA->ma_grille->grid[x][y+compt]==8  ||
                        GA->ma_grille->grid[x][y+compt]==9  ||
                        GA->ma_grille->grid[x][y+compt]==10 ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[6].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[6].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ||
                       GA->ma_grille->grid[x-compt][y-compt]==2 ||
                       GA->ma_grille->grid[x-compt][y-compt]==3 ||
                       GA->ma_grille->grid[x-compt][y-compt]==4 ||
                       GA->ma_grille->grid[x-compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==7  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4]=1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[6].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[6].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[6].vitesse][y+GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[6].vitesse,y+GA->rules->car[6].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ||
                       GA->ma_grille->grid[x+compt][y+compt]==2 ||
                       GA->ma_grille->grid[x+compt][y+compt]==3 ||
                       GA->ma_grille->grid[x+compt][y+compt]==4 ||
                       GA->ma_grille->grid[x+compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==7  ||
                        GA->ma_grille->grid[x-compt][y-compt]==8  ||
                        GA->ma_grille->grid[x-compt][y-compt]==9  ||
                        GA->ma_grille->grid[x-compt][y-compt]==10 ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5]=1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[6].vitesse)>=0)&&
                    ((y-GA->rules->car[6].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[6].vitesse][y-GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[6].vitesse,y-GA->rules->car[6].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ||
                       GA->ma_grille->grid[x+compt][y-compt]==2 ||
                       GA->ma_grille->grid[x+compt][y-compt]==3 ||
                       GA->ma_grille->grid[x+compt][y-compt]==4 ||
                       GA->ma_grille->grid[x+compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==7  ||
                        GA->ma_grille->grid[x-compt][y+compt]==8  ||
                        GA->ma_grille->grid[x-compt][y+compt]==9  ||
                        GA->ma_grille->grid[x-compt][y+compt]==10 ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))     ) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[6].vitesse)>=0)&&
                    ((y+GA->rules->car[6].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[6].vitesse][y+GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[6].vitesse,y+GA->rules->car[6].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ||
                       GA->ma_grille->grid[x-compt][y+compt]==2 ||
                       GA->ma_grille->grid[x-compt][y+compt]==3 ||
                       GA->ma_grille->grid[x-compt][y+compt]==4 ||
                       GA->ma_grille->grid[x-compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==7  ||
                        GA->ma_grille->grid[x+compt][y-compt]==8  ||
                        GA->ma_grille->grid[x+compt][y-compt]==9  ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[6].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[6].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[6].vitesse][y-GA->rules->car[6].vitesse]=7;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[6].vitesse,y-GA->rules->car[6].vitesse);
                        return((int)11);
                   }
                  }
                     int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 7;


                switch(alea)
                                {
                                   case 0:
                                           { if(x==0)return 7;
                                             if(GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=7 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                           }break;
                                   case 1:
                                            {
                                              if(x==(GA->ma_grille->nb_cels_width-1))return 7;
                                              if(GA->ma_grille->grid[x+1][y]!=6 &&
                                                 GA->ma_grille->grid[x+1][y]!=7 &&
                                                 GA->ma_grille->grid[x+1][y]!=8 &&
                                                 GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 7;
                                              if(GA->ma_grille->grid[x][y-1]!=6 &&
                                                 GA->ma_grille->grid[x][y-1]!=7 &&
                                                 GA->ma_grille->grid[x][y-1]!=8 &&
                                                 GA->ma_grille->grid[x][y-1]!=9 &&
                                                GA->ma_grille->grid[x][y-1]!=10 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 3:
                                            {
                                               if(y==(GA->ma_grille->nb_cels_height-1))return 7;
                                               if(GA->ma_grille->grid[x][y+1]!=6 &&
                                                  GA->ma_grille->grid[x][y+1]!=7 &&
                                                  GA->ma_grille->grid[x][y+1]!=8 &&
                                                  GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 4:
                                            {
                                               if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 7;
                                               if(GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=7 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=7;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 5:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                   ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                   ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 7;
                                               if(GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=7 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 7;
                                                if(GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=7 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=7;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                   case 7:
                                            {
                                                 if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 7;
                                                if(GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=7 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=7;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)7);
                                            }break;
                                }
                  return ((int)7);
               }break;
       case 8: {
                        for(int j=0; j<8; j++)obstacles[j]=0;

                    while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1 ||
                       GA->ma_grille->grid[x+compt][y]==2 ||
                       GA->ma_grille->grid[x+compt][y]==4 ||
                       GA->ma_grille->grid[x+compt][y]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==7  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        GA->ma_grille->grid[x-compt][y]==3  ||
                        GA->ma_grille->grid[x-compt][y]==8  ||
                        GA->ma_grille->grid[x-compt][y]==9  ||
                        GA->ma_grille->grid[x-compt][y]==10 ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[7].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[7].vitesse][y]=8;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[7].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ||
                       GA->ma_grille->grid[x-compt][y]==2 ||
                       GA->ma_grille->grid[x-compt][y]==4 ||
                       GA->ma_grille->grid[x-compt][y]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==3  ||
                        GA->ma_grille->grid[x+compt][y]==7  ||
                        GA->ma_grille->grid[x+compt][y]==8  ||
                        GA->ma_grille->grid[x+compt][y]==9  ||
                        GA->ma_grille->grid[x+compt][y]==10 ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[7].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[7].vitesse][y]=8;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[7].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ||
                       GA->ma_grille->grid[x][y+compt]==2 ||
                       GA->ma_grille->grid[x][y+compt]==4 ||
                       GA->ma_grille->grid[x][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==3  ||
                        GA->ma_grille->grid[x][y-compt]==7  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10 ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[7].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[7].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ||
                       GA->ma_grille->grid[x][y-compt]==2 ||
                       GA->ma_grille->grid[x][y-compt]==4 ||
                       GA->ma_grille->grid[x][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==3  ||
                        GA->ma_grille->grid[x][y+compt]==7  ||
                        GA->ma_grille->grid[x][y+compt]==8  ||
                        GA->ma_grille->grid[x][y+compt]==9  ||
                        GA->ma_grille->grid[x][y+compt]==10 ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[7].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[7].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ||
                       GA->ma_grille->grid[x-compt][y-compt]==2 ||
                       GA->ma_grille->grid[x-compt][y-compt]==4 ||
                       GA->ma_grille->grid[x-compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==3  ||
                        GA->ma_grille->grid[x+compt][y+compt]==7  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4] =1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[7].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[7].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[7].vitesse][y+GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[7].vitesse,y+GA->rules->car[7].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ||
                       GA->ma_grille->grid[x+compt][y+compt]==2 ||
                       GA->ma_grille->grid[x+compt][y+compt]==4 ||
                       GA->ma_grille->grid[x+compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==3  ||
                        GA->ma_grille->grid[x-compt][y-compt]==7  ||
                        GA->ma_grille->grid[x-compt][y-compt]==8  ||
                        GA->ma_grille->grid[x-compt][y-compt]==9  ||
                        GA->ma_grille->grid[x-compt][y-compt]==10 ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[7].vitesse)>=0)&&
                    ((y-GA->rules->car[7].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[7].vitesse][y-GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[7].vitesse,y-GA->rules->car[7].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[7].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ||
                       GA->ma_grille->grid[x+compt][y-compt]==2 ||
                       GA->ma_grille->grid[x+compt][y-compt]==4 ||
                       GA->ma_grille->grid[x+compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==3  ||
                        GA->ma_grille->grid[x-compt][y+compt]==7  ||
                        GA->ma_grille->grid[x-compt][y+compt]==8  ||
                        GA->ma_grille->grid[x-compt][y+compt]==9  ||
                        GA->ma_grille->grid[x-compt][y+compt]==10 ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[7].vitesse)>=0)&&
                    ((y+GA->rules->car[7].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[7].vitesse][y+GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[7].vitesse,y+GA->rules->car[7].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[6].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ||
                       GA->ma_grille->grid[x-compt][y+compt]==2 ||
                       GA->ma_grille->grid[x-compt][y+compt]==4 ||
                       GA->ma_grille->grid[x-compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==3  ||
                        GA->ma_grille->grid[x+compt][y-compt]==7  ||
                        GA->ma_grille->grid[x+compt][y-compt]==8  ||
                        GA->ma_grille->grid[x+compt][y-compt]==9  ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7]=1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[7].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[7].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[7].vitesse][y-GA->rules->car[7].vitesse]=8;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[7].vitesse,y-GA->rules->car[7].vitesse);
                        return((int)11);
                   }
                  }

                             int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 8;


                switch(alea)
                                {
                                   case 0:
                                           { if(x==0)return 8;
                                             if(GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=7 &&
                                                GA->ma_grille->grid[x-1][y]!=3 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                           }break;
                                   case 1:
                                            {
                                              if(x==(GA->ma_grille->nb_cels_width-1))return 8;
                                              if(GA->ma_grille->grid[x+1][y]!=6 &&
                                                 GA->ma_grille->grid[x+1][y]!=7 &&
                                                 GA->ma_grille->grid[x+1][y]!=3 &&
                                                 GA->ma_grille->grid[x+1][y]!=8 &&
                                                 GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 8;
                                              if(GA->ma_grille->grid[x][y-1]!=6 &&
                                                 GA->ma_grille->grid[x][y-1]!=7 &&
                                                 GA->ma_grille->grid[x][y-1]!=3 &&
                                                 GA->ma_grille->grid[x][y-1]!=8 &&
                                                 GA->ma_grille->grid[x][y-1]!=9 &&
                                                GA->ma_grille->grid[x][y-1]!=10 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 3:
                                            {
                                               if(y==(GA->ma_grille->nb_cels_height-1))return 8;
                                               if(GA->ma_grille->grid[x][y+1]!=6 &&
                                                  GA->ma_grille->grid[x][y+1]!=7 &&
                                                  GA->ma_grille->grid[x][y+1]!=3 &&
                                                  GA->ma_grille->grid[x][y+1]!=8 &&
                                                  GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 4:
                                            {
                                               if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 8;
                                               if(GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=7 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=3 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=8;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 5:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                   ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                   ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 8;
                                               if(GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=7 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=3 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 8;
                                                if(GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=7 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=3 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=8;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                   case 7:
                                            {
                                                 if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 8;
                                                if(GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=7 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=3 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=8;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)8);
                                            }break;
                                }
                  return ((int)8);
               }break;
       case 9: {
                        for(int j=0; j<8; j++)obstacles[j]=0;

                      while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1 ||
                       GA->ma_grille->grid[x+compt][y]==2 ||
                       //GA->ma_grille->grid[x+compt][y]==3 ||
                       GA->ma_grille->grid[x+compt][y]==4 ||
                       GA->ma_grille->grid[x+compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==7  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        GA->ma_grille->grid[x-compt][y]==3  ||
                        GA->ma_grille->grid[x-compt][y]==8  ||
                        GA->ma_grille->grid[x-compt][y]==9  ||
                        GA->ma_grille->grid[x-compt][y]==10 ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[8].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[8].vitesse][y]=9;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[8].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ||
                       GA->ma_grille->grid[x-compt][y]==2 ||
                       //GA->ma_grille->grid[x-compt][y]==3 ||
                       GA->ma_grille->grid[x-compt][y]==4 ||
                       GA->ma_grille->grid[x-compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==3  ||
                        GA->ma_grille->grid[x+compt][y]==7  ||
                        GA->ma_grille->grid[x+compt][y]==8  ||
                        GA->ma_grille->grid[x+compt][y]==9  ||
                        GA->ma_grille->grid[x+compt][y]==10 ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[8].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[8].vitesse][y]=9;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[8].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ||
                       GA->ma_grille->grid[x][y+compt]==2 ||
                       //GA->ma_grille->grid[x][y+compt]==3 ||
                       GA->ma_grille->grid[x][y+compt]==4 ||
                       GA->ma_grille->grid[x][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==3  ||
                        GA->ma_grille->grid[x][y-compt]==7  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10 ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[8].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[8].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ||
                       GA->ma_grille->grid[x][y-compt]==2 ||
                       //GA->ma_grille->grid[x][y-compt]==3 ||
                       GA->ma_grille->grid[x][y-compt]==4 ||
                       GA->ma_grille->grid[x][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==3  ||
                        GA->ma_grille->grid[x][y+compt]==7  ||
                        GA->ma_grille->grid[x][y+compt]==8  ||
                        GA->ma_grille->grid[x][y+compt]==9  ||
                        GA->ma_grille->grid[x][y+compt]==10 ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3]=1;
                    if((pred)||(obstacles[3]))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[8].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[8].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ||
                       GA->ma_grille->grid[x-compt][y-compt]==2 ||
                       //GA->ma_grille->grid[x-compt][y-compt]==3 ||
                       GA->ma_grille->grid[x-compt][y-compt]==4 ||
                       GA->ma_grille->grid[x-compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==3  ||
                        GA->ma_grille->grid[x+compt][y+compt]==7  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4]=1;
                    if((pred)||(obstacles[4]))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[8].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[8].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[8].vitesse][y+GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[8].vitesse,y+GA->rules->car[8].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ||
                       GA->ma_grille->grid[x+compt][y+compt]==2 ||
                       //GA->ma_grille->grid[x+compt][y+compt]==3 ||
                       GA->ma_grille->grid[x+compt][y+compt]==4 ||
                       GA->ma_grille->grid[x+compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==3  ||
                        GA->ma_grille->grid[x-compt][y-compt]==7  ||
                        GA->ma_grille->grid[x-compt][y-compt]==8  ||
                        GA->ma_grille->grid[x-compt][y-compt]==9  ||
                        GA->ma_grille->grid[x-compt][y-compt]==10 ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[8].vitesse)>=0)&&
                    ((y-GA->rules->car[8].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[8].vitesse][y-GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[8].vitesse,y-GA->rules->car[8].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ||
                       GA->ma_grille->grid[x+compt][y-compt]==2 ||
                       //GA->ma_grille->grid[x+compt][y-compt]==3 ||
                       GA->ma_grille->grid[x+compt][y-compt]==4 ||
                       GA->ma_grille->grid[x+compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==3  ||
                        GA->ma_grille->grid[x-compt][y+compt]==7  ||
                        GA->ma_grille->grid[x-compt][y+compt]==8  ||
                        GA->ma_grille->grid[x-compt][y+compt]==9  ||
                        GA->ma_grille->grid[x-compt][y+compt]==10 ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6]=1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[8].vitesse)>=0)&&
                    ((y+GA->rules->car[8].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[8].vitesse][y+GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[8].vitesse,y+GA->rules->car[8].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[8].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ||
                       GA->ma_grille->grid[x-compt][y+compt]==2 ||
                       //GA->ma_grille->grid[x-compt][y+compt]==3 ||
                       GA->ma_grille->grid[x-compt][y+compt]==4 ||
                       GA->ma_grille->grid[x-compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==3  ||
                        GA->ma_grille->grid[x+compt][y-compt]==7  ||
                        GA->ma_grille->grid[x+compt][y-compt]==8  ||
                        GA->ma_grille->grid[x+compt][y-compt]==9  ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7]))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[8].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[8].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[8].vitesse][y-GA->rules->car[8].vitesse]=9;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[8].vitesse,y-GA->rules->car[8].vitesse);
                        return((int)11);
                   }
                  }

                            int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 7;


                switch(alea)
                                {
                                   case 0:
                                           { if(x==0)return 9;
                                             if(GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=7 &&
                                                GA->ma_grille->grid[x-1][y]!=3 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                           }break;
                                   case 1:
                                            {
                                              if(x==(GA->ma_grille->nb_cels_width-1))return 9;
                                              if(GA->ma_grille->grid[x+1][y]!=6 &&
                                                 GA->ma_grille->grid[x+1][y]!=7 &&
                                                 GA->ma_grille->grid[x+1][y]!=3 &&
                                                 GA->ma_grille->grid[x+1][y]!=8 &&
                                                 GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 9;
                                              if(GA->ma_grille->grid[x][y-1]!=6 &&
                                                 GA->ma_grille->grid[x][y-1]!=7 &&
                                                 GA->ma_grille->grid[x][y-1]!=3 &&
                                                 GA->ma_grille->grid[x][y-1]!=8 &&
                                                 GA->ma_grille->grid[x][y-1]!=9 &&
                                                GA->ma_grille->grid[x][y-1]!=10 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 3:
                                            {
                                               if(y==(GA->ma_grille->nb_cels_height-1))return 9;
                                               if(GA->ma_grille->grid[x][y+1]!=6 &&
                                                  GA->ma_grille->grid[x][y+1]!=7 &&
                                                  GA->ma_grille->grid[x][y+1]!=3 &&
                                                  GA->ma_grille->grid[x][y+1]!=8 &&
                                                  GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 4:
                                            {
                                               if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 9;
                                               if(GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=7 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=3 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=9;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 5:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                   ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                   ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 9;
                                               if(GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=7 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=3 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 9;
                                                if(GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=7 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=3 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=9;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                   case 7:
                                            {
                                                 if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 9;
                                                if(GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=7 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=3 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=9;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)9);
                                            }break;
                                }
                  return ((int)9);
               }break;
       case 10: {
                        for(int j=0; j<8; j++)obstacles[j]=0;

                      while( ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y]==1 ||
                       GA->ma_grille->grid[x+compt][y]==2 ||
                       //GA->ma_grille->grid[x+compt][y]==3 ||
                       GA->ma_grille->grid[x+compt][y]==4 ||
                       GA->ma_grille->grid[x+compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x-compt][y]==20 ||
                        GA->ma_grille->grid[x-compt][y]==7  ||
                        GA->ma_grille->grid[x-compt][y]==6  ||
                        GA->ma_grille->grid[x-compt][y]==3  ||
                        GA->ma_grille->grid[x-compt][y]==8  ||
                        GA->ma_grille->grid[x-compt][y]==9  ||
                        GA->ma_grille->grid[x-compt][y]==10 ||
                        (x-compt)==0) )obstacles[0] =1;
                    if((pred)||(obstacles[0] ))break;
               }

                   if(pred==1)
                 {
                    if((x-GA->rules->car[9].vitesse)>=0)
                    {
                         GA->ma_grille->grid[x-GA->rules->car[69].vitesse][y]=10;
                         GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[9].vitesse,y);
                        return((int)11);
                    }
                 }


                 // cherche a gauche maintenant
                 compt=0;
                 trouve=0;

                 while( ((x-compt)>0) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y]==1 ||
                       GA->ma_grille->grid[x-compt][y]==2 ||
                       //GA->ma_grille->grid[x-compt][y]==3 ||
                       GA->ma_grille->grid[x-compt][y]==4 ||
                       GA->ma_grille->grid[x-compt][y]==5  ) pred=1;
                    if(/**(!pred)&&**/
                       (GA->ma_grille->grid[x+compt][y]==20 ||
                        GA->ma_grille->grid[x+compt][y]==6  ||
                        GA->ma_grille->grid[x+compt][y]==3  ||
                        GA->ma_grille->grid[x+compt][y]==7  ||
                        GA->ma_grille->grid[x+compt][y]==8  ||
                        GA->ma_grille->grid[x+compt][y]==9  ||
                        GA->ma_grille->grid[x+compt][y]==10 ||
                        (x+compt)==(GA->ma_grille->nb_cels_width-1)) )obstacles[1] =1;
                    if((pred)||(obstacles[1] ))break;
               }

                    if(pred==1)
                 {
                    if((x+GA->rules->car[9].vitesse)<=(GA->ma_grille->nb_cels_width-1))
                    {
                         GA->ma_grille->grid[x+GA->rules->car[9].vitesse][y]=10;
                         GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[9].vitesse,y);
                        return((int)11);
                    }
                 }


                 // chercher en bas
                 compt=0;
                 trouve=0;

                 while( ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y+compt]==1 ||
                       GA->ma_grille->grid[x][y+compt]==2 ||
                       //GA->ma_grille->grid[x][y+compt]==3 ||
                       GA->ma_grille->grid[x][y+compt]==4 ||
                       GA->ma_grille->grid[x][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y-compt]==20 ||
                        GA->ma_grille->grid[x][y-compt]==6  ||
                        GA->ma_grille->grid[x][y-compt]==3  ||
                        GA->ma_grille->grid[x][y-compt]==7  ||
                        GA->ma_grille->grid[x][y-compt]==8  ||
                        GA->ma_grille->grid[x][y-compt]==9  ||
                        GA->ma_grille->grid[x][y-compt]==10 ||
                        (y-compt)==0) )obstacles[2] =1;
                    if((pred)||(obstacles[2] ))break;
               }

                   if(pred==1)
                 {
                     if((y-GA->rules->car[9].vitesse)>=0)
                     {
                        GA->ma_grille->grid[x][y-GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x,y-GA->rules->car[9].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher en haut
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x][y-compt]==1 ||
                       GA->ma_grille->grid[x][y-compt]==2 ||
                       //GA->ma_grille->grid[x][y-compt]==3 ||
                       GA->ma_grille->grid[x][y-compt]==4 ||
                       GA->ma_grille->grid[x][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x][y+compt]==20 ||
                        GA->ma_grille->grid[x][y+compt]==6  ||
                        GA->ma_grille->grid[x][y+compt]==3  ||
                        GA->ma_grille->grid[x][y+compt]==7  ||
                        GA->ma_grille->grid[x][y+compt]==8  ||
                        GA->ma_grille->grid[x][y+compt]==9  ||
                        GA->ma_grille->grid[x][y+compt]==10 ||
                        (y+compt)==GA->ma_grille->nb_cels_height-1) )obstacles[3] =1;
                    if((pred)||(obstacles[3] ))break;
               }

                 if(pred==1)
                 {
                     if((y+GA->rules->car[9].vitesse)<=(GA->ma_grille->nb_cels_height-1))
                     {
                        GA->ma_grille->grid[x][y+GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x,y+GA->rules->car[9].vitesse);
                        return((int)11);
                     }
                 }


                  // chercher dans le left up
                 compt=0;
                 trouve=0;

                   while( ((y-compt)>0) && ((x-compt)>0) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y-compt]==1 ||
                       GA->ma_grille->grid[x-compt][y-compt]==2 ||
                       //GA->ma_grille->grid[x-compt][y-compt]==3 ||
                       GA->ma_grille->grid[x-compt][y-compt]==4 ||
                       GA->ma_grille->grid[x-compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y+compt]==20 ||
                        GA->ma_grille->grid[x+compt][y+compt]==6  ||
                        GA->ma_grille->grid[x+compt][y+compt]==3  ||
                        GA->ma_grille->grid[x+compt][y+compt]==7  ||
                        GA->ma_grille->grid[x+compt][y+compt]==8  ||
                        GA->ma_grille->grid[x+compt][y+compt]==9  ||
                        GA->ma_grille->grid[x+compt][y+compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))  ) )obstacles[4] =1;
                    if((pred)||(obstacles[4] ))break;
               }

                 if(pred==1)
                 {
                  if(((x+GA->rules->car[9].vitesse)<=(GA->ma_grille->nb_cels_width-1))&&
                    ((y+GA->rules->car[9].vitesse)<=(GA->ma_grille->nb_cels_height-1)))
                  {
                        GA->ma_grille->grid[x+GA->rules->car[9].vitesse][y+GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[9].vitesse,y+GA->rules->car[9].vitesse);
                        return((int)11);
                  }
                 }


                 // chercher dans le down right
                 compt=0;
                 trouve=0;

                   while( ((y+compt)<GA->ma_grille->nb_cels_height) && ((x+compt)<GA->ma_grille->nb_cels_width) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y+compt]==1 ||
                       GA->ma_grille->grid[x+compt][y+compt]==2 ||
                       //GA->ma_grille->grid[x+compt][y+compt]==3 ||
                       GA->ma_grille->grid[x+compt][y+compt]==4 ||
                       GA->ma_grille->grid[x+compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y-compt]==20 ||
                        GA->ma_grille->grid[x-compt][y-compt]==6  ||
                        GA->ma_grille->grid[x-compt][y-compt]==3  ||
                        GA->ma_grille->grid[x-compt][y-compt]==7  ||
                        GA->ma_grille->grid[x-compt][y-compt]==8  ||
                        GA->ma_grille->grid[x-compt][y-compt]==9  ||
                        GA->ma_grille->grid[x-compt][y-compt]==10 ||
                        (((x-compt)==(0))&&((y-compt)==(0)))       ))obstacles[5] =1;
                    if((pred)||(obstacles[5] ))break;
               }

                    if(pred)
                    {
                     if(((x-GA->rules->car[9].vitesse)>=0)&&
                    ((y-GA->rules->car[9].vitesse)>=0))
                      {
                        GA->ma_grille->grid[x-GA->rules->car[9].vitesse][y-GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[9].vitesse,y-GA->rules->car[9].vitesse);
                        return((int)11);
                     }
                    }


                // chercher dans le up right

                compt=0;
                 trouve=0;

                   while( ((x+compt)<GA->ma_grille->nb_cels_width) && ((y-compt)>0) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x+compt][y-compt]==1 ||
                       GA->ma_grille->grid[x+compt][y-compt]==2 ||
                       //GA->ma_grille->grid[x+compt][y-compt]==3 ||
                       GA->ma_grille->grid[x+compt][y-compt]==4 ||
                       GA->ma_grille->grid[x+compt][y-compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x-compt][y+compt]==20 ||
                        GA->ma_grille->grid[x-compt][y+compt]==6  ||
                        GA->ma_grille->grid[x-compt][y+compt]==3  ||
                        GA->ma_grille->grid[x-compt][y+compt]==7  ||
                        GA->ma_grille->grid[x-compt][y+compt]==8  ||
                        GA->ma_grille->grid[x-compt][y+compt]==9  ||
                        GA->ma_grille->grid[x-compt][y+compt]==10 ||
                        (((x-compt)==(0))&&((y+compt)==(GA->ma_grille->nb_cels_height-1)))) )obstacles[6] =1;
                    if((pred)||(obstacles[6] ))break;
               }

                  if(pred)
                   {
                 if(((x-GA->rules->car[9].vitesse)>=0)&&
                    ((y+GA->rules->car[9].vitesse)<= GA->ma_grille->nb_cels_height-1))
                    {
                        GA->ma_grille->grid[x-GA->rules->car[9].vitesse][y+GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x-GA->rules->car[9].vitesse,y+GA->rules->car[9].vitesse);
                        return((int)11);
                    }
                   }


                // chercher dans le down left
                 compt=0;
                 trouve=0;

                   while( ((x-compt)>0) && ((y+compt)<GA->ma_grille->nb_cels_height) &&
                          compt<GA->rules->car[9].p_vue )
               {
                    compt++;
                    if(
                       GA->ma_grille->grid[x-compt][y+compt]==1 ||
                       GA->ma_grille->grid[x-compt][y+compt]==2 ||
                       //GA->ma_grille->grid[x-compt][y+compt]==3 ||
                       GA->ma_grille->grid[x-compt][y+compt]==4 ||
                       GA->ma_grille->grid[x-compt][y+compt]==5  ) pred=1;
                    if(
                       (GA->ma_grille->grid[x+compt][y-compt]==20 ||
                        GA->ma_grille->grid[x+compt][y-compt]==6  ||
                        GA->ma_grille->grid[x+compt][y-compt]==3  ||
                        GA->ma_grille->grid[x+compt][y-compt]==7  ||
                        GA->ma_grille->grid[x+compt][y-compt]==8  ||
                        GA->ma_grille->grid[x+compt][y-compt]==9  ||
                        GA->ma_grille->grid[x+compt][y-compt]==10 ||
                        (((x+compt)==(GA->ma_grille->nb_cels_width-1))&&((y-compt)==(0)))  ) )obstacles[7] =1;
                    if((pred)||(obstacles[7] ))break;
               }

                  if(pred){
                 if(((x+GA->rules->car[9].vitesse)<=GA->ma_grille->nb_cels_width-1)&&
                    ((y-GA->rules->car[9].vitesse)>=0))
                   {
                        GA->ma_grille->grid[x+GA->rules->car[9].vitesse][y-GA->rules->car[9].vitesse]=10;
                        GA->maliste=ajouter_coord(GA->maliste,x+GA->rules->car[9].vitesse,y-GA->rules->car[9].vitesse);
                        return((int)11);
                   }
                  }

                        int test=1;
                    for(int i=0; i<8; i++)
                    {
                        if(obstacles[i]==0)test=0;
                        if(test==0)break;
                    }
                    if(test==1)return 10;


                switch(alea)
                                {
                                   case 0:
                                           { if(x==0)return 10;
                                             if(GA->ma_grille->grid[x-1][y]!=6 &&
                                                GA->ma_grille->grid[x-1][y]!=7 &&
                                                GA->ma_grille->grid[x-1][y]!=3 &&
                                                GA->ma_grille->grid[x-1][y]!=8 &&
                                                GA->ma_grille->grid[x-1][y]!=9 &&
                                                GA->ma_grille->grid[x-1][y]!=10 &&
                                                GA->ma_grille->grid[x-1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x-1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                           }break;
                                   case 1:
                                            {
                                              if(x==(GA->ma_grille->nb_cels_width-1))return 10;
                                              if(GA->ma_grille->grid[x+1][y]!=6 &&
                                                 GA->ma_grille->grid[x+1][y]!=7 &&
                                                 GA->ma_grille->grid[x+1][y]!=3 &&
                                                 GA->ma_grille->grid[x+1][y]!=8 &&
                                                 GA->ma_grille->grid[x+1][y]!=9 &&
                                                GA->ma_grille->grid[x+1][y]!=10 &&
                                                GA->ma_grille->grid[x+1][y]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 2:
                                            {
                                              if(y==0)return 10;
                                              if(GA->ma_grille->grid[x][y-1]!=6 &&
                                                 GA->ma_grille->grid[x][y-1]!=7 &&
                                                 GA->ma_grille->grid[x][y-1]!=3 &&
                                                 GA->ma_grille->grid[x][y-1]!=8 &&
                                                 GA->ma_grille->grid[x][y-1]!=9 &&
                                                GA->ma_grille->grid[x][y-1]!=10 &&
                                                GA->ma_grille->grid[x][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y-1]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 3:
                                            {
                                               if(y==(GA->ma_grille->nb_cels_height-1))return 10;
                                               if(GA->ma_grille->grid[x][y+1]!=6 &&
                                                  GA->ma_grille->grid[x][y+1]!=7 &&
                                                  GA->ma_grille->grid[x][y+1]!=3 &&
                                                  GA->ma_grille->grid[x][y+1]!=8 &&
                                                  GA->ma_grille->grid[x][y+1]!=9 &&
                                                GA->ma_grille->grid[x][y+1]!=10 &&
                                                GA->ma_grille->grid[x][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x][y+1]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 4:
                                            {
                                               if(((x==0)&&(y==0))||
                                                    ((x==0)&&(y!=0))||
                                                    ((x!=0)&&(y==0)))return 10;
                                               if(GA->ma_grille->grid[x-1][y-1]!=6 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=7 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=3 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=8 &&
                                                  GA->ma_grille->grid[x-1][y-1]!=9 &&
                                                GA->ma_grille->grid[x-1][y-1]!=10 &&
                                                GA->ma_grille->grid[x-1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y-1]=10;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 5:
                                            {
                                                if(((x==GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1))||
                                                   ((x==GA->ma_grille->nb_cels_width-1)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                                   ((x!=GA->ma_grille->nb_cels_width-1)&&(y==GA->ma_grille->nb_cels_height-1)))return 10;
                                               if(GA->ma_grille->grid[x+1][y+1]!=6 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=7 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=3 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=8 &&
                                                  GA->ma_grille->grid[x+1][y+1]!=9 &&
                                                GA->ma_grille->grid[x+1][y+1]!=10 &&
                                                GA->ma_grille->grid[x+1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y+1]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 6:
                                            {
                                                if(((x==0)&&(y==GA->ma_grille->nb_cels_height-1))||
                                    ((x==0)&&(y!=GA->ma_grille->nb_cels_height-1))||
                                    ((x!=0)&&(y==GA->ma_grille->nb_cels_height-1)))return 10;
                                                if(GA->ma_grille->grid[x-1][y+1]!=6 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=7 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=3 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=8 &&
                                                   GA->ma_grille->grid[x-1][y+1]!=9 &&
                                                GA->ma_grille->grid[x-1][y+1]!=10 &&
                                                GA->ma_grille->grid[x-1][y+1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x-1][y+1]=10;
                                                     GA->maliste=ajouter_coord(GA->maliste,x-1,y+1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                   case 7:
                                            {
                                                 if(((x==GA->ma_grille->nb_cels_width-1)&&(y==0))||
                                                    ((x==GA->ma_grille->nb_cels_width-1)&&(y!=0))||
                                                    ((x!=GA->ma_grille->nb_cels_width-1)&&(y==0)) )return 10;
                                                if(GA->ma_grille->grid[x+1][y-1]!=6 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=7 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=3 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=8 &&
                                                   GA->ma_grille->grid[x+1][y-1]!=9 &&
                                                GA->ma_grille->grid[x+1][y-1]!=10 &&
                                                GA->ma_grille->grid[x+1][y-1]!=20   )
                                                {
                                                    GA->ma_grille->grid[x+1][y-1]=10;
                                                    GA->maliste=ajouter_coord(GA->maliste,x+1,y-1);
                                                    return ((int)11);
                                                }
                                                return ((int)10);
                                            }break;
                                }
                  return ((int)10);
                }break;
    }
}


/** UNe fonction d'aide pour calculer la proie la plus proche au predateur
     ON reccueil les distances des proies qui sans dans sa portee de vue
     mais auquelle il faudrait plus d'un pas et on s'approche d'elle **/

     int min_dist(int r , int l, int u, int d, int ur,int ul,int dr,int dl)
     {
         //vu que les distances sont initialisé avec "90"
         //Prendre le nombre min est suffisant
         int mini=90;
         int ind_m=8;
         int tab[8]={r,l,u,d,ur,ul,dr,dl};
         for(int i=0;i<8;i++)
         {
             printf("\n %d",tab[i]);
             if(tab[i]<mini)
             {
                 mini=tab[i];
                 ind_m=i;
             }
         }
         return((int)ind_m);
     }


/** Une fonction qui reçoit la structure du jeu
    et applique à l'ensemble des cellules de la grille
    la fonction animal_rules
**/
static void start_game_animals(animals_game* GW)
{
    int j,b;

        //Si le jeu est en mode play
        if(GW->etat == TRUE)
        {
            for(j=0;j<GW->ma_grille->nb_cels_height;j++)
                for(b=0;b<GW->ma_grille->nb_cels_width;b++)
                {
                    if(coord_existe(GW->maliste,b,j))continue;
                    GW->ma_grille->grid[b][j] = animals_rules(GW,j,b);
                }
             GW->maliste=NULL;
            //Dessiner la grille
            g_signal_connect(G_OBJECT(GW->ma_grille->drawing_area),"draw",G_CALLBACK(update_grid_animals),GW);
            //Mettre à jour la grille
            gtk_widget_queue_draw(GW->ma_grille->drawing_area);
            //Incrémenter le compteur
        }

}


//La fonction qui permet a la grille de passer de son etat actuel
//à l'état suivant
void on_next_state_animals(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    animals_game* GW = (animals_game*)data;
    //L'etat play
    GW->etat = TRUE;
    //Lancer le jeu
    start_game_animals(GW);
    //Letat pause
    GW->etat = FALSE;

    gtk_widget_set_sensitive(GW->play->button,TRUE);
    //changer le bouton en play
    gtk_button_set_label(GTK_BUTTON(GW->play->button),"PLAY");
    GtkWidget* image = gtk_image_new_from_file("Images/play.jpg");
    //ctiver le bouton play et la vitesse
    gtk_button_set_image(GTK_BUTTON(GW->play->button),GTK_IMAGE(image));
}

/**La fonction pour lancer le jeu **/
void on_play_animals(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    animals_game* GW = (animals_game*)data;
    //Si l'etat du jeu est pause
    if(GW->etat == FALSE)
    {
        //Rendre l'etat play
        GW->etat = TRUE;
         //appeler la fonction du demarrage du jeu
        g_timeout_add(GW->timer,(GSourceFunc)start_game_animals,GW);
        //Changer le bouton play en pause
        gtk_button_set_label(GTK_BUTTON(GW->play->button),"PAUSE");
        GtkWidget* image = gtk_image_new_from_file("Images/pause.png");
        gtk_button_set_image(GTK_BUTTON(GW->play->button),GTK_IMAGE(image));
    }
    else  //Si l'etat du jeu est play
    {
        //Rendre l'etat pause
        GW->etat = FALSE;
        //activer la vitesse
        //Changer le bouton pause en play
        gtk_button_set_label(GTK_BUTTON(GW->play->button),"PLAY");
        GtkWidget* image = gtk_image_new_from_file("Images/play.png");
        gtk_button_set_image(GTK_BUTTON(GW->play->button),GTK_IMAGE(image));
    }
}

void on_restore_animals(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    animals_game* GW = (animals_game*)data;
    int i,j;
    //remettre la grille à 0
    for(i=0;i<Max_de_ligne;i++)
        for(j=0;j<Max_de_colonne;j++)
        GW->ma_grille->grid[i][j] = 11;
    //Reinitialiser les parametres du jeu
    //Taille des cases
    GW->ma_grille->cel_size = 24;
    //nombre de cases
    GW->ma_grille->nb_cels_width = Max_width/GW->ma_grille->cel_size;
    GW->ma_grille->nb_cels_height = Max_height/GW->ma_grille->cel_size;
    //Delimiteurs
    //GW->ma_grille->left = (Max_de_colonne/2)-(GW->ma_grille->nb_cels_width/2);
    GW->ma_grille->right = (Max_de_colonne/2)+(GW->ma_grille->nb_cels_width/2);
    GW->ma_grille->bottom = (Max_de_ligne/2)+(GW->ma_grille->nb_cels_height/2);

    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

void save_animals(GtkWidget *b,gpointer data)
{
    animals_game *GW=(animals_game*)data;
    GW->etat=FALSE;
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    gint res;
    dialog=gtk_file_chooser_dialog_new("save FILE",GW->W->window,GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",
                                       GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT,NULL);
    chooser=GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser,TRUE);
    gtk_file_chooser_set_current_name(chooser,"acelem.line");

    res=gtk_dialog_run(GTK_DIALOG(dialog));
    if(res== GTK_RESPONSE_ACCEPT)
    {
         char *filename;
         int i , j ;
         filename=gtk_file_chooser_get_filename(chooser);
         FILE *f=fopen(filename,"w");
         //fprintf(f,"%d\n",GW->counter);

         fprintf(f,"%d\t%d\t%d\t%d\t%d\n",GW->ma_grille->cel_size,GW->ma_grille->left,GW->ma_grille->right,
                                          GW->ma_grille->nb_cels_height,GW->ma_grille->nb_cels_width);

         printf("\n\n\n*************************\n\n\n");
         for(i=0;i<GW->ma_grille->nb_cels_width;i++)
         {
             for(j=0;j<GW->ma_grille->nb_cels_height;j++)
                printf("%d  ",GW->ma_grille->grid[i][j]);
             printf("\n");
         }
         printf("\n\n\n*************************\n\n\n");

            for(j=0;j<GW->ma_grille->nb_cels_height;j++)

         for(i=0;i<GW->ma_grille->nb_cels_width;i++)
            for(j=0;j<GW->ma_grille->nb_cels_height;j++)
               fprintf(f,"%c",GW->ma_grille->grid[i][j]);
         fclose(f);
         g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void open_animals(GtkWidget *b,gpointer data)
{
    animals_game *GW=(animals_game*)data;
    GW->etat=FALSE;
    GtkWidget *dialog;
    gint res;
    dialog=gtk_file_chooser_dialog_new("open FILE",GW->W->window,GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",
                                       GTK_RESPONSE_CANCEL,"_open",GTK_RESPONSE_ACCEPT,NULL);

    res=gtk_dialog_run(GTK_DIALOG(dialog));
    if(res== GTK_RESPONSE_ACCEPT)
    {
         char *filename;
         int i , j ;
         GtkFileChooser *chooser=GTK_FILE_CHOOSER(dialog);
         filename=gtk_file_chooser_get_filename(chooser);

         FILE *f=fopen(filename,"r");
         //fscanf(f,"%d\n",&GW->counter);

         fscanf(f,"%d\t%d\t%d\t%d\t%d\n",&GW->ma_grille->cel_size,&GW->ma_grille->left,&GW->ma_grille->right,
                                          &GW->ma_grille->nb_cels_height,&GW->ma_grille->nb_cels_width);
         fseek(f,1,SEEK_CUR);
         for(i=0;i<GW->ma_grille->nb_cels_width;i++)
            for(j=0;j<GW->ma_grille->nb_cels_height;j++)
               fscanf(f,"%c",&GW->ma_grille->grid[i][j]);
         fclose(f);
         g_free(filename);
    }
  /*  char str[5];
    sprintf(str,"%d",GW->counter);
    gtk_label_set_text(GTK_LABEL(GW->COUNT->label),str); */
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
    gtk_widget_destroy(dialog);
}

    void on_param(GtkWidget *w,gpointer data)
    {
        animals_game *GA=(animals_game*)data;

        gtk_widget_hide(GA->W->window);

        gtk_widget_show_all(GA->rules->ac_window->window);
    }

    void on_return_from_param(GtkWidget *w,gpointer data)
    {
         animals_game *GA=(animals_game*)data;

         gtk_widget_hide(GA->rules->ac_window->window);

         gtk_widget_show_all(GA->W->window);
    }

void event_calls_animals_game(animals_game *GA)
{
    gtk_widget_add_events(G_OBJECT(GA->ma_grille->drawing_area),GDK_BUTTON_PRESS_MASK);

    g_signal_connect(G_OBJECT(GA->ma_grille->drawing_area),"draw",G_CALLBACK(update_grid_animals),GA);

    g_signal_connect(G_OBJECT(GA->ma_grille->drawing_area),"button-press-event",G_CALLBACK(switch_animals_line),GA);

    g_signal_connect(G_OBJECT(GA->play->button),"button-press-event",G_CALLBACK(on_play_animals),GA);
    g_signal_connect(G_OBJECT(GA->next->button),"button-press-event",G_CALLBACK(on_next_state_animals),GA);
    g_signal_connect(G_OBJECT(GA->restaurer->button),"button-press-event",G_CALLBACK(on_restore_animals),GA);
    g_signal_connect(G_OBJECT(GA->save->button),"clicked",G_CALLBACK(save_animals),GA);
    g_signal_connect(G_OBJECT(GA->open->button),"clicked",G_CALLBACK(open_animals),GA);

    g_signal_connect(G_OBJECT(GA->parametres->button),"clicked",G_CALLBACK(on_param),GA);
    g_signal_connect(G_OBJECT(GA->rules->OKEY->button),"clicked",G_CALLBACK(on_return_from_param),GA);

    g_signal_connect(G_OBJECT(GA->button_animals[0]->button),"clicked",G_CALLBACK(on_lion_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[1]->button),"clicked",G_CALLBACK(on_leopard_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[2]->button),"clicked",G_CALLBACK(on_fox_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[3]->button),"clicked",G_CALLBACK(on_wolf_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[4]->button),"clicked",G_CALLBACK(on_hyena_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[5]->button),"clicked",G_CALLBACK(on_elephant_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[6]->button),"clicked",G_CALLBACK(on_lapin_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[7]->button),"clicked",G_CALLBACK(on_gazelle_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[8]->button),"clicked",G_CALLBACK(on_zebra_button),GA);
    g_signal_connect(G_OBJECT(GA->button_animals[9]->button),"clicked",G_CALLBACK(on_sheep_button),GA);

    g_signal_connect(G_OBJECT(GA->button_nature[0]->button),"clicked",G_CALLBACK(on_grass_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[1]->button),"clicked",G_CALLBACK(on_watergrass_up_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[2]->button),"clicked",G_CALLBACK(on_watergrass_down_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[3]->button),"clicked",G_CALLBACK(on_watergrass_left_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[4]->button),"clicked",G_CALLBACK(on_watergrass_right_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[5]->button),"clicked",G_CALLBACK(on_watergrass_upleft_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[6]->button),"clicked",G_CALLBACK(on_watergrass_upright_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[7]->button),"clicked",G_CALLBACK(on_watergrass_downleft_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[8]->button),"clicked",G_CALLBACK(on_watergrass_downright_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[9]->button),"clicked",G_CALLBACK(on_water_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[10]->button),"clicked",G_CALLBACK(on_rock_button),GA);
    //g_signal_connect(G_OBJECT(GA->button_nature[11]->button),"clicked",G_CALLBACK(on_shit_button),GA);
    g_signal_connect(G_OBJECT(GA->button_nature[12]->button),"clicked",G_CALLBACK(on_deepwater_button),GA);

    g_signal_connect(G_OBJECT(GA->supprimer->button),"clicked",G_CALLBACK(on_delete_button),GA);

    g_signal_connect(G_OBJECT(GA->W->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
}






#endif // ANIMALS_H_INCLUDED
