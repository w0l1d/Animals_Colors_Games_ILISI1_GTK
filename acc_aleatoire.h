#ifndef ACC_ALEATOIRE_H_INCLUDED
#define ACC_ALEATOIRE_H_INCLUDED

#include "myheaders.h"
#include "lesregles.h"

#define Max_de_ligne 130
#define Max_de_colonne 300

#define Max_width 1200
#define Max_height 650


/** La structure de la grille*/
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
}grille_acc_alt;

/**La structure du jeu , interface et variables **/
typedef struct
{
   grille_acc_alt *ma_grille; // l'objet grille
   WindowObject* W;//La fenetre
   IconObject* IC;//L'icone
   TableObject *regle_colors_table;//table pour boutons règle et couleurs
   BoxObject *global_box; //Box global
   TableObject *table_acc;
   GtkWidget *button_colors[4]; //4 boutons de couleur
   BoxObject *box_acc;
   BoxObject *left_box;
   BoxObject *right_box;
   ButtonObject *retourner; // button de retour vers le menu du jeu acc
   ButtonObject *play;  // bouton delancement du jeu
   ButtonObject *next;  // bouton pour passer a l'etat suivant
   ButtonObject *restaurer; // bouton de reinitialisation de grille
   ButtonObject *enregistrer; //bouton pour enregistrer le dernier etat de la grille
   ButtonObject *ouvrir;

   LabelObject *COUNT;   /** label designant l'iteration actuelle**/
   LabelObject *compteur_name;  /**label: nombre d'itérations **/

   gboolean etat;  /** true = play | false = jeu en pause **/
   gint counter;
   gint timer;
   ButtonObject *regles; //bouton re règles
   rules_structure *rules;

   int choix;         //Indique quel couleur l'utilisateur a choisi
   GdkColor mycol[4]; //pour stocker les couleurs choisies
                      //A CHAQUE fois par l'utilisateur
}acc_alt;

/** Initialisation de la grille **/
grille_acc_alt* init_grille_acc_alt()
{
    int i,j;
    // allouer la grille
    grille_acc_alt *GL=(grille_acc_alt*)malloc(sizeof(grille_acc_alt));
    //initialiser la taille des cellules
    GL->cel_size=16;
    // initialiser le nombre de cases
    GL->nb_cels_width=Max_width/GL->cel_size;
    GL->nb_cels_height=Max_height/GL->cel_size;
    // initialiser les delimiteurs
    GL->left=(Max_de_colonne/2)-(GL->nb_cels_width/2);
    GL->right=(Max_de_colonne/2)+(GL->nb_cels_width/2);
    GL->bottom=(Max_de_ligne/2)-(GL->nb_cels_height/2);
    // creer l'espace de dessin
    GL->drawing_area=gtk_drawing_area_new();
    // initialiser les couleurs
    gdk_color_parse("WHITE",&GL->color_cell);

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
           GL->grid[i][j]=0;



    return ((grille_acc_alt*)GL);
}


/** La creation de l'interface globale**/
void create_interface_line(acc_alt *GW)
{
    //Creation de la fenetre
    GW->W=init_window("ACC",200,900,4,0,0,0);
    GW->W=create_window(GW->W);

    //icone de la fenetre
    GW->IC=init_icon(GW->W->window,"images/icon_acc_alt.png");
    GW->W->window=create_icon(GW->IC);

    //Le box global
    GW->global_box=init_box(GW->W->window,"global box",'h',0,0);
    GW->global_box=create_box(GW->global_box);
    add_to_window(GW->W,GW->global_box->box);

    /** l'ensemble des elmnts de la grille et les boutons**/
    GW->box_acc =init_box(GW->global_box,"box_alt",'v',1,0);
    GW->box_acc = create_box(GW->box_acc);
    add_to_box_start(GW->global_box,GW->box_acc->box,1,1,0);

    GW->table_acc=init_table(GW->box_acc->box,3,18,1);
    GW->table_acc=create_table(GW->table_acc);
    add_to_box_start(GW->box_acc,GW->table_acc->table,1,1,0);

    gchar *name_help=g_locale_to_utf8("nombre d'itérations",-1,NULL,NULL,NULL);

    GW->compteur_name=init_label(NULL,name_help,name_help,0,0);
    g_free(name_help);
    GW->compteur_name=create_label(GW->compteur_name);
    add_to_table(GW->table_acc,GW->compteur_name->label,10,12,0,3);

    GW->COUNT = init_label(NULL,"0","count",0,0);
    GW->COUNT = create_label(GW->COUNT);
    add_to_table(GW->table_acc,GW->COUNT->label,12,13,0,3);

    GW->retourner=init_button(GW->table_acc->table,"retourner","retourner","images/return.png");
    GW->retourner=create_button(GW->retourner);
    add_to_table(GW->table_acc,GW->retourner->button,16,18,0,3);

    GW->restaurer=init_button(GW->table_acc->table,"restaurer","restaurer","images/restore.png");
    GW->restaurer=create_button(GW->restaurer);
    add_to_table(GW->table_acc,GW->restaurer->button,4,6,0,3);

    GW->next=init_button(GW->table_acc->table,"next","next","images/next.png");
    GW->next=create_button(GW->next);
    add_to_table(GW->table_acc,GW->next->button,2,4,0,3);

    GW->play=init_button(GW->table_acc->table,"play","play","images/play.png");
    GW->play=create_button(GW->play);
    add_to_table(GW->table_acc,GW->play->button,0,2,0,3);


    GW->enregistrer=init_button(GW->table_acc->table,"enregistrer","enregistrer","images/save.png");
    GW->enregistrer=create_button(GW->enregistrer);
    add_to_table(GW->table_acc,GW->enregistrer->button,6,8,0,3);

    GW->ouvrir=init_button(GW->table_acc->table,"ouvrir","ouvrir","images/export.png");
    GW->ouvrir=create_button(GW->ouvrir);
    add_to_table(GW->table_acc,GW->ouvrir->button,8,10,0,3);



    GW->left_box=init_box(GW->box_acc->box,"left box",'h',1,0);
    GW->left_box=create_box(GW->left_box);

    add_to_box_end(GW->left_box,GW->ma_grille->drawing_area,1,1,0);

        /** les regles et couleurs **/

    GW->right_box=init_box(GW->global_box->box,"right box",'v',1,10);
    GW->right_box=create_box(GW->right_box);
    add_to_box_start(GW->global_box,GW->right_box->box,1,1,0);

    GW->regle_colors_table=init_table(GW->right_box->box,10,20,1);
    GW->regle_colors_table=create_table(GW->regle_colors_table);
    add_to_box_end(GW->right_box,GW->regle_colors_table->table,1,1,0);

    //Initialiser le tableau de couleurs
    gdk_color_parse("RED",&GW->mycol[0]);
    gdk_color_parse("BLUE",&GW->mycol[1]);
    gdk_color_parse("GREEN",&GW->mycol[2]);

    //Creation et initialisation des boutons couleur
    GW->button_colors[0]=gtk_color_button_new_with_color(&GW->mycol[0]);
    add_to_table(GW->regle_colors_table,GW->button_colors[0],0,5,4,5);

    GW->button_colors[1]=gtk_color_button_new_with_color(&GW->mycol[1]);
    add_to_table(GW->regle_colors_table,GW->button_colors[1],5,10,4,5);

    GW->button_colors[2]=gtk_color_button_new_with_color(&GW->mycol[2]);
    add_to_table(GW->regle_colors_table,GW->button_colors[2],10,15,4,5);

    GW->button_colors[3]=gtk_color_button_new_with_color(&GW->ma_grille->color_cell);
    add_to_table(GW->regle_colors_table,GW->button_colors[3],15,20,4,5);

    //Creation et initialisation du bouton de règles
    GtkWidget *regles_label=gtk_label_new("les regles : ");
    add_to_table(GW->regle_colors_table,regles_label,0,6,1,2);

    gchar *titre_help=g_locale_to_utf8("régles",-1,NULL,NULL,NULL);
    GW->regles=init_button(GW->regle_colors_table->table,titre_help,titre_help,"images/rules.png");
    GW->regles=create_button(GW->regles);
    add_to_table(GW->regle_colors_table,GW->regles->button,6,15,1,2);
    g_free(titre_help);

    //Initialiser le choix à 0;choix dela couleur blanche
    GW->choix=0;
}

//Initialiser tous les élements du jeu
acc_alt* init_ACC()
{
   //Allocation de la mémoire
   acc_alt *GACC_ALT=(acc_alt*)malloc(sizeof(acc_alt));
   if(!GACC_ALT)
   {
       printf("erreur d'allocation !!");
       exit(EXIT_FAILURE);
   }
   //Initialisation de la grille
   grille_acc_alt *GL=NULL;
   GL=init_grille_acc_alt();
   GACC_ALT->ma_grille=GL;

   //Initialisation des règles
   rules_structure *RW=init_rules_window();
   event_calls_rules(RW);
   GACC_ALT->rules=RW;

   //Creation de l'interface
   create_interface_line(GACC_ALT);
   GACC_ALT->etat=FALSE; //Le jeu en pause
   GACC_ALT->counter=0;  //Initialiser le conteur
   GACC_ALT->timer=100;

   return ((acc_alt*)GACC_ALT);
}

/**Une fonction qui permet de dessiner une cellule
   Entree: outil de desin cairo
           position x dela cellule à dessiner
           position y de la cellule à dessine
           La structure du jeu
   Sortie: Le dessin d'une cellule
**/

void draw_cel_line(cairo_t* CR, gint posx, gint posy, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
     switch(GW->ma_grille->grid[posx][posy])
    {
        //premier etat de la cellule 'blanc'
        case 0 : gdk_cairo_set_source_color(CR,&GW->ma_grille->color_cell); break;
        //si l utilisateur a choisi la couleur du bouton 1
        case 1 : gdk_cairo_set_source_color(CR,&GW->mycol[0]); break;
        //si l utilisateur a choisi la couleur du bouton 2
        case 2 :  gdk_cairo_set_source_color(CR,&GW->mycol[1]); break;
        //si l utilisateur a choisi la couleur du bouton 3
        case 3 :  gdk_cairo_set_source_color(CR,&GW->mycol[2]); break;
        //si l utilisateur a choisi la couleur du bouton 4
        case 4 :  gdk_cairo_set_source_color(CR,&GW->mycol[3]); break;

    }

    //Decrementer le compteur
    posy -= GW->ma_grille->left;
        //Definir les bordures des cellules
        cairo_set_line_width(CR,2);
        //Dessiner les carreaux
        cairo_rectangle(CR,posy*(GW->ma_grille->cel_size),posx*(GW->ma_grille->cel_size),
                        (GW->ma_grille->cel_size)-3,(GW->ma_grille->cel_size)-3);
        cairo_stroke_preserve(CR);
        cairo_fill(CR);
}


void change_counter_line(acc_alt* GW)
{
    char str[5];
        //Recuperer le nombre d'terations
        sprintf(str,"%d ",GW->counter);
        //Mettre à jour le label
        gtk_label_set_text(GW->COUNT->label,str);
}

/**  Une fonction pour redessiner la grille à chaque ittération **/
static void update_grid_line(GtkWidget* widget, cairo_t* CR, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    int i,j,b;
    //parcourir la grille
    for(i=0;i<GW->ma_grille->nb_cels_height;i++)
        for(j=0;j<GW->ma_grille->nb_cels_width;j++)
        {
            //Calculer le nouvel indice
            b = j + GW->ma_grille->left;
            //Dessiner la case
            draw_cel_line(CR,i,b,GW);
        }
}


/** Une fonction qui zoom sur la grille
    Quand on atteint l'un des délimiteurs**/

static void zoom_grid_line(acc_alt* GW)
{
    int j;
    int res_h, res_w;
    //si la taille de cellule > 5
    if(GW->ma_grille->cel_size > 5)
    {
        //Tester sur l'avant dernière cellule à gauche et à droite
        if(((GW->ma_grille->grid[GW->counter+1][(GW->ma_grille->left)+1] == 1) ||
           (GW->ma_grille->grid[GW->counter+1][(GW->ma_grille->right)-1] == 1)) ||
           //Et sur la dernière ligne
           ((GW->counter == GW->ma_grille->nb_cels_height-1) && (GW->ma_grille->nb_cels_height != Max_de_ligne)))
        {
            do
            {
                //Decrementer la taille de la cellule
                GW->ma_grille->cel_size--;
                //Calculer le nombre de cases
                res_h = (int)Max_height/GW->ma_grille->cel_size;
                res_w = (int)Max_width/GW->ma_grille->cel_size;
                //jusqu'a avoir un nombre de cases divisible par 2
            }while((Max_width%GW->ma_grille->cel_size != 0) || (Max_height%GW->ma_grille->cel_size != 0));
            //Affecter le nouveau nombre de cases
            GW->ma_grille->nb_cels_width = res_w;
            GW->ma_grille->nb_cels_height = res_h;
            //Calculer les nouveaux delimiteurs
            GW->ma_grille->left = (Max_de_colonne/2)-(GW->ma_grille->nb_cels_width/2);
            GW->ma_grille->right = (Max_de_colonne/2)+(GW->ma_grille->nb_cels_width/2);
            GW->ma_grille->bottom = (Max_de_ligne/2)+(GW->ma_grille->nb_cels_height/2);
        }
    }
}

/** Une fonction qui change la couleur
    d'une cellule quand on clique dessus selon
    le couleur choisie de la palette **/
static gboolean switch_color_line(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    int x,y;
    //si c'est un clique gaucher
    if (event->button == 1)
    {
        //recuperer les coordonnees
        y = (int)event->x;
        x = (int)event->y;

        if(x/GW->ma_grille->cel_size != GW->counter) return TRUE;

        //Calculer l'emplacement
        x/=(GW->ma_grille->cel_size);
        y/=(GW->ma_grille->cel_size);
        y += GW->ma_grille->left;
        //Changer la couleur de la case
        //Mettre à jour la grille
          GW->ma_grille->grid[x][y]=GW->choix;
        gtk_widget_queue_draw(GW->ma_grille->drawing_area);
    }
    return TRUE;
}

 /* La fonction : on_alive_line1
    Une fonction de rappel appelé quand on choisi une couleur
    dans le premier bouton .Ajoute la couleur à notre tableau
    de couleurs et change la variable choix.
 */
void on_alive_line1(GtkWidget* widget, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    GdkColor color;
    //Actualiser la variable du choix
    GW->choix=1;
    //Recuperer la couleur du bouton
    gtk_color_button_get_color(GTK_COLOR_BUTTON(GW->button_colors[0]),&color);
    //Changer la couleur
    GW->mycol[0]= color;
    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

/* La fonction : on_alive_line2
    Une fonction de rappel appelé quand on choisi une couleur
    dans le second bouton .Ajoute la couleur à notre tableau
    de couleurs et change la variable choix.
 */
void on_alive_line2(GtkWidget* widget, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    GdkColor color;

    GW->choix=2;
    //Recuperer la couleur du bouton
    gtk_color_button_get_color(GTK_COLOR_BUTTON(GW->button_colors[1]),&color);
    //Changer la couleur
    GW->mycol[1]= color;
    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

/* La fonction : on_alive_line3
    Une fonction de rappel appelé quand on choisi une couleur
    dans le troisième bouton .Ajoute la couleur à notre tableau
    de couleurs et change la variable choix.
 */
void on_alive_line3(GtkWidget* widget, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    GdkColor color;
    GW->choix=3;
    //Recuperer la couleur du bouton
    gtk_color_button_get_color(GTK_COLOR_BUTTON(GW->button_colors[2]),&color);
    //Changer la couleur
    GW->mycol[2]= color;
    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

/* La fonction : on_alive_line4
    Une fonction de rappel appelé quand on choisi une couleur
    dans le quatriem bouton .Ajoute la couleur à notre tableau
    de couleurs et change la variable choix.
 */
void on_alive_line4(GtkWidget* widget, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    GdkColor color;
    GW->choix=4;
    //Recuperer la couleur du bouton
    gtk_color_button_get_color(GTK_COLOR_BUTTON(GW->button_colors[3]),&color);
    //Changer la couleur
    GW->mycol[3]= color;
    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

/** Une fonction de rappel appelé quand on clique
    sur le bouton de règles .
    Affiche la fenetre de règles et cache celle du jeu
**/
void on_rule(GtkWidget* w, gpointer data)
{
   acc_alt *GW=(acc_alt*)data;

    gtk_widget_hide(GW->W->window);

   gtk_widget_show_all(GW->rules->Rules_window->window);
}

/* Une fonction de rappel appelée quand on clique
   sur le bouton de retour de la fenetre de règles.
   Affiche la fenetre du jeu et cache celle de règles
*/
void on_return_from_rule(GtkWidget* w, gpointer data)
{
   acc_alt *GW=(acc_alt*)data;

    gtk_widget_hide(GW->rules->Rules_window->window);

   gtk_widget_show_all(GW->W->window);
}


/** La fonction line_rules
    ENTREE: La structure du jeu
            les position x,y de la cellule a traité
    Sortie: la valeur correspendante à la nouvelle couleur de la cellule
    Cette fonction applique les règles choisies/crées par l'utlisateur
    à une cellule donnée
**/
gint line_rules(acc_alt* GW, gint posx, gint posy)
{
   switch(GW->rules->mes_choix) //selon quel mode l'utilisateur a choisi de jouer
   {
    //Mode aleatoire
    case 1: {   int i;
                if(GW->rules->etat) i=4; else i=5;
                if((posy==0)||(posy==Max_de_colonne-1)) return 0;
                else
                    return (((GW->ma_grille->grid[posx][posy-1])+
                         (GW->ma_grille->grid[posx][posy])+
                         (GW->ma_grille->grid[posx][posy+1])+
                         ((GW->ma_grille->grid[posx][posy])*
                         (GW->ma_grille->grid[posx][posy])))%i);
            }break;
    //Creer ses propre règles
    case 2: {
              if((posy==0)||(posy==Max_de_colonne-1)) return 0;
                personal_rule *crt=GW->rules->creer_regles->maliste;
                while(crt)
                {
                    if((GW->ma_grille->grid[posx][posy-1]==crt->left)&&
                        (GW->ma_grille->grid[posx][posy]==crt->middle)&&
                        (GW->ma_grille->grid[posx][posy+1]==crt->right)) return ((int)crt->result);
                    else
                        crt=crt->svt;
                }
            }break;

    //Choisir parmi des règles prédéfinies
    case 3: {
              if((posy==0)||(posy==Max_de_colonne-1)) return 0;

                if((GW->rules->parmis_regles->verifier[0])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==0)&&
                   (GW->ma_grille->grid[posx][posy+1]==0))
                    return 0;
                if((GW->rules->parmis_regles->verifier[1])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==0)&&
                   (GW->ma_grille->grid[posx][posy+1]==1))
                    return 1;
                if((GW->rules->parmis_regles->verifier[2])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==0)&&
                   (GW->ma_grille->grid[posx][posy+1]==2))
                    return 2;
                if((GW->rules->parmis_regles->verifier[3])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==0)&&
                   (GW->ma_grille->grid[posx][posy+1]==3))
                    return 3;
                if((GW->rules->parmis_regles->verifier[4])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==1)&&
                   (GW->ma_grille->grid[posx][posy+1]==0))
                    return 1;
                if((GW->rules->parmis_regles->verifier[5])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==1)&&
                   (GW->ma_grille->grid[posx][posy+1]==1))
                    return 2;
                if((GW->rules->parmis_regles->verifier[6])&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]==1)&&
                   (GW->ma_grille->grid[posx][posy+1]==2))
                    return 3;
                if((GW->rules->parmis_regles->verifier[7] )&&
                   (GW->ma_grille->grid[posx][posy-1]==0 )&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]==3 ))
                    return 0;
                if((GW->rules->parmis_regles->verifier[8] )&&
                   (GW->ma_grille->grid[posx][posy-1]==0 )&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 2;
                if((GW->rules->parmis_regles->verifier[9] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==1 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[10] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 0;
                if((GW->rules->parmis_regles->verifier[11] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 1;
                if((GW->rules->parmis_regles->verifier[12] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return ;
                if((GW->rules->parmis_regles->verifier[13] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 0;
                if((GW->rules->parmis_regles->verifier[14] )&&
                   (GW->ma_grille->grid[posx][posy-1]==0)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==2 ))
                    return 1;
                if((GW->rules->parmis_regles->verifier[15] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 2;
                if((GW->rules->parmis_regles->verifier[16] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 1;
                if((GW->rules->parmis_regles->verifier[17] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 2;
                if((GW->rules->parmis_regles->verifier[18] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 3;
                if((GW->rules->parmis_regles->verifier[19] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 0;
                if((GW->rules->parmis_regles->verifier[20] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 2;
                if((GW->rules->parmis_regles->verifier[21] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 3;
                if((GW->rules->parmis_regles->verifier[22] )&&
                   (GW->ma_grille->grid[posx][posy-1]==1)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 0;
                if((GW->rules->parmis_regles->verifier[23] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 1;
                if((GW->rules->parmis_regles->verifier[24] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 3;
                if((GW->rules->parmis_regles->verifier[25] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 0;
                if((GW->rules->parmis_regles->verifier[26] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 1;
                if((GW->rules->parmis_regles->verifier[27] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 2;
                if((GW->rules->parmis_regles->verifier[28] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 0;
                if((GW->rules->parmis_regles->verifier[29] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 1;
                if((GW->rules->parmis_regles->verifier[30] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 2;
                if((GW->rules->parmis_regles->verifier[31] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 3;
                if((GW->rules->parmis_regles->verifier[32] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 2;
                if((GW->rules->parmis_regles->verifier[33] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 3;
                if((GW->rules->parmis_regles->verifier[34] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 0;
                if((GW->rules->parmis_regles->verifier[35] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 1;
                if((GW->rules->parmis_regles->verifier[36] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 3;
                if((GW->rules->parmis_regles->verifier[37] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 0;
                if((GW->rules->parmis_regles->verifier[38] )&&
                   (GW->ma_grille->grid[posx][posy-1]==2)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 1;
                if((GW->rules->parmis_regles->verifier[39] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 2;
                if((GW->rules->parmis_regles->verifier[40] )&&
                   (GW->ma_grille->grid[posx][posy-1]==2 )&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 0;
                if((GW->rules->parmis_regles->verifier[41] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==1 ))
                    return 1;
                if((GW->rules->parmis_regles->verifier[42] )&&
                   (GW->ma_grille->grid[posx][posy-1]==2 )&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==2 ))
                    return 2;
                if((GW->rules->parmis_regles->verifier[43] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==3 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[44] )&&
                   (GW->ma_grille->grid[posx][posy-1]==2 )&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 1;
                if((GW->rules->parmis_regles->verifier[45] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 2;
                if((GW->rules->parmis_regles->verifier[46] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==2 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[47] )&&
                   (GW->ma_grille->grid[posx][posy-1]==2)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 0;
                if((GW->rules->parmis_regles->verifier[48] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[49] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 0;
                if((GW->rules->parmis_regles->verifier[50] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 1;
                if((GW->rules->parmis_regles->verifier[51] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 2;
                if((GW->rules->parmis_regles->verifier[52] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 0;
                if((GW->rules->parmis_regles->verifier[53] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 1;
                if((GW->rules->parmis_regles->verifier[54] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 2;
                if((GW->rules->parmis_regles->verifier[55] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 3;
                if((GW->rules->parmis_regles->verifier[56] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 1;
                if((GW->rules->parmis_regles->verifier[57] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==1 ))
                    return 2;
                if((GW->rules->parmis_regles->verifier[58] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3 )&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==2 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[59] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 2)&&
                   (GW->ma_grille->grid[posx][posy+1]==3 ))
                    return 0;
                if((GW->rules->parmis_regles->verifier[60] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==0 ))
                    return 2;
                if((GW->rules->parmis_regles->verifier[61] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3 )&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]==1 ))
                    return 3;
                if((GW->rules->parmis_regles->verifier[62] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 0;
                if((GW->rules->parmis_regles->verifier[63] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 3))
                    return 1;
                if((GW->rules->parmis_regles->verifier[64] )&&
                   (GW->ma_grille->grid[posx][posy-1]==4 )&&
                   (GW->ma_grille->grid[posx][posy]==0)&&
                   (GW->ma_grille->grid[posx][posy+1]==0))
                    return 4;
                 if((GW->rules->parmis_regles->verifier[65] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 0))
                    return 0;
                if((GW->rules->parmis_regles->verifier[66] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 0)&&
                   (GW->ma_grille->grid[posx][posy]== 0)&&
                   (GW->ma_grille->grid[posx][posy+1]== 4))
                    return 4;
                if((GW->rules->parmis_regles->verifier[67] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 1)&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 2;
                if((GW->rules->parmis_regles->verifier[68] )&&
                   (GW->ma_grille->grid[posx][posy-1]==3 )&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 4))
                    return 1;
                if((GW->rules->parmis_regles->verifier[69] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 4)&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 1))
                    return 4;
                 if((GW->rules->parmis_regles->verifier[70] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 4)&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 4))
                    return 3;
                if((GW->rules->parmis_regles->verifier[71] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 4)&&
                   (GW->ma_grille->grid[posx][posy]== 1)&&
                   (GW->ma_grille->grid[posx][posy+1]== 4))
                    return 2;
                if((GW->rules->parmis_regles->verifier[72] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 3)&&
                   (GW->ma_grille->grid[posx][posy]== 3)&&
                   (GW->ma_grille->grid[posx][posy+1]== 4))
                    return 4;
                if((GW->rules->parmis_regles->verifier[73] )&&
                   (GW->ma_grille->grid[posx][posy-1]== 2)&&
                   (GW->ma_grille->grid[posx][posy]== 4)&&
                   (GW->ma_grille->grid[posx][posy+1]== 2))
                    return 3;
            }break;
   }
}

/** Une fonction qui reçoit la structure du jeu
    et en fonction du conteur detecte la ligne à traiter
    fait appel a line_rules pour l'ensemble des cellules
    de la ligne à traite.
**/
static void start_game_line(acc_alt* GW)
{
    int j,b;
    //Si on est dans la premiere ligne et l'etat initial est aleatoire
    if(GW->counter==0)
    {
        if(GW->rules->etat)
            for(j=GW->ma_grille->left;j<GW->ma_grille->right;j++)
            GW->ma_grille->grid[0][j]=rand()%4;
    }

    //Si on a pas atteint la fin de la grille
    if(GW->counter<Max_de_ligne-1)
    {
        //Si le jeu est en mode play
        if(GW->etat == TRUE)
        {
            for(j=0;j<GW->ma_grille->nb_cels_width;j++)
            {
                //Calculer le nouvel indice
                b = j + GW->ma_grille->left;
                //Appliquer les règles sur chaque cellule de la ligne
                GW->ma_grille->grid[GW->counter+1][b] = line_rules(GW,GW->counter,b);
            }
            printf("before\n");
            //Dessiner la grille
            g_signal_connect(G_OBJECT(GW->ma_grille->drawing_area),"draw",G_CALLBACK(update_grid_line),GW);
            printf("after");
            //Appeler la fonction zoom
            zoom_grid_line(GW);
            //Mettre à jour la grille
            gtk_widget_queue_draw(GW->ma_grille->drawing_area);
            //Incrémenter le compteur
            GW->counter++;
            change_counter_line(GW);
        }
    }
}

/**la fonction de rappel qui fait appel une fois a la fct start game
   en changeant par la suite l etat de la line suivante **/
void on_next_line(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    //L'etat play
    GW->etat = TRUE;
    //Lancer le jeu
    start_game_line(GW);
    //Letat pause
    GW->etat = FALSE;
    gtk_widget_set_sensitive(GW->play->button,TRUE);
   /** gtk_widget_set_sensitive(GW->SPEED->scale,TRUE);  **/
    //changer le bouton en play
    gtk_button_set_label(GTK_BUTTON(GW->play->button),"PLAY");
    GtkWidget* image = gtk_image_new_from_file("Images/play.jpg");
    //ctiver le bouton play et la vitesse
    gtk_button_set_image(GTK_BUTTON(GW->play->button),GTK_IMAGE(image));
   /** gtk_widget_set_sensitive(GW->SPEED->scale,TRUE); **/
}



void on_play_line(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    //Si l'etat du jeu est pause
    if(GW->etat == FALSE)
    {
        //Rendre l'etat play
        GW->etat = TRUE;
        //Desactiver la vitesse
       /** gtk_widget_set_sensitive(GW->SPEED->scale,FALSE); **/
        //appeler la fonction du demarrage du jeu
        g_timeout_add(GW->timer,(GSourceFunc)start_game_line,GW);
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
      /**  gtk_widget_set_sensitive(GW->SPEED->scale,TRUE);  **/
        //Changer le bouton pause en play
        gtk_button_set_label(GTK_BUTTON(GW->play->button),"PLAY");
        GtkWidget* image = gtk_image_new_from_file("Images/play.png");
        gtk_button_set_image(GTK_BUTTON(GW->play->button),GTK_IMAGE(image));
    }
}

void on_restore_line(GtkWidget* widget,GdkEventButton *event, gpointer data)
{
    acc_alt* GW = (acc_alt*)data;
    int i,j;
    //remettre la grille à 0
    for(i=0;i<Max_de_ligne;i++)
        for(j=0;j<Max_de_colonne;j++)
        GW->ma_grille->grid[i][j] = 0;
    //Reinitialiser les parametres du jeu
    //Taille des cases
    GW->ma_grille->cel_size = 16;
    //nombre de cases
    GW->ma_grille->nb_cels_width = Max_width/GW->ma_grille->cel_size;
    GW->ma_grille->nb_cels_height = Max_height/GW->ma_grille->cel_size;
    //Delimiteurs
    GW->ma_grille->left = (Max_de_colonne/2)-(GW->ma_grille->nb_cels_width/2);
    GW->ma_grille->right = (Max_de_colonne/2)+(GW->ma_grille->nb_cels_width/2);
    GW->ma_grille->bottom = (Max_de_ligne/2)+(GW->ma_grille->nb_cels_height/2);
    //Compteur
     GW->counter = 0;
    change_counter_line(GW);
    //Mettre à jour la grille
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);
}

void save_ligne(GtkWidget *b,gpointer data)
{
    acc_alt *GW=(acc_alt*)data;
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
         fprintf(f,"%d\n",GW->counter);

         fprintf(f,"%d\t%d\t%d\t%d\t%d\n",GW->ma_grille->cel_size,GW->ma_grille->left,GW->ma_grille->right,
                                          GW->ma_grille->nb_cels_height,GW->ma_grille->nb_cels_width);

         for(i=0;i<200;i++)
            for(j=0;j<300;j++)
               fprintf(f,"%c",GW->ma_grille->grid[i][j]);
         fclose(f);
         g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

void open_ligne(GtkWidget *b,gpointer data)
{
    acc_alt *GW=(acc_alt*)data;
    GW->etat=FALSE;
    GtkWidget *dialog;
    gint res;
    dialog=gtk_file_chooser_dialog_new("open FILE",GW->W->window,GTK_FILE_CHOOSER_ACTION_OPEN,"_Cancel",
                                       GTK_RESPONSE_CANCEL,"_open",GTK_RESPONSE_ACCEPT,NULL);

    res=gtk_dialog_run(GTK_DIALOG(dialog));
    if(res== GTK_RESPONSE_ACCEPT)
    {
         char *filename;
         int i , j ;
         GtkFileChooser *chooser=GTK_FILE_CHOOSER(dialog);
         filename=gtk_file_chooser_get_filename(chooser);

         FILE *f=fopen(filename,"r");
         fscanf(f,"%d\n",&GW->counter);

         fscanf(f,"%d\t%d\t%d\t%d\t%d\n",&GW->ma_grille->cel_size,&GW->ma_grille->left,&GW->ma_grille->right,
                                          &GW->ma_grille->nb_cels_height,&GW->ma_grille->nb_cels_width);
         fseek(f,1,SEEK_CUR);
         for(i=0;i<200;i++)
            for(j=0;j<300;j++)
               fscanf(f,"%c",&GW->ma_grille->grid[i][j]);
         fclose(f);
         g_free(filename);
    }
    char str[5];
    sprintf(str,"%d",GW->counter);
    gtk_label_set_text(GTK_LABEL(GW->COUNT->label),str);
    gtk_widget_queue_draw(GW->ma_grille->drawing_area);

    gtk_widget_destroy(dialog);
}


void event_calls_acc(acc_alt *GW)
{
    gtk_widget_add_events(G_OBJECT(GW->ma_grille->drawing_area),GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(GW->ma_grille->drawing_area),"draw",G_CALLBACK(update_grid_line),GW);
    g_signal_connect(G_OBJECT(GW->button_colors[0]),"color-set",G_CALLBACK(on_alive_line1),GW);
    g_signal_connect(G_OBJECT(GW->button_colors[1]),"color-set",G_CALLBACK(on_alive_line2),GW);
    g_signal_connect(G_OBJECT(GW->button_colors[2]),"color-set",G_CALLBACK(on_alive_line3),GW);
    g_signal_connect(G_OBJECT(GW->button_colors[3]),"color-set",G_CALLBACK(on_alive_line4),GW);
    g_signal_connect(G_OBJECT(GW->ma_grille->drawing_area),"button-press-event",G_CALLBACK(switch_color_line),GW);
    g_signal_connect(G_OBJECT(GW->play->button),"button-press-event",G_CALLBACK(on_play_line),GW);
    g_signal_connect(G_OBJECT(GW->next->button),"button-press-event",G_CALLBACK(on_next_line),GW);
    g_signal_connect(G_OBJECT(GW->restaurer->button),"button-press-event",G_CALLBACK(on_restore_line),GW);
    g_signal_connect(G_OBJECT(GW->enregistrer->button),"clicked",G_CALLBACK(save_ligne),GW);
    g_signal_connect(G_OBJECT(GW->ouvrir->button),"clicked",G_CALLBACK(open_ligne),GW);
    g_signal_connect(G_OBJECT(GW->regles->button),"clicked",G_CALLBACK(on_rule),GW);
    g_signal_connect(G_OBJECT(GW->rules->leave_button->button),"clicked",G_CALLBACK(on_return_from_rule),GW);
}






























#endif // ACC_ALEATOIRE_H_INCLUDED
