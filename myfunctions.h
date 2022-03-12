#ifndef MYFUNCTIONS_H_INCLUDED
#define MYFUNCTIONS_H_INCLUDED

#include "myheaders.h"

/*****************************************************************
                        BASICS
******************************************************************/
//Initialiser l'application gtk
void init_app(int c, char* v[])
{
    gtk_init(&c, &v);
}
//Afficher la fenetre ainsi que ses element fils
void display_all(GtkWidget* Widg)
{
    gtk_widget_show_all(Widg);
    gtk_main();
}
//Appliquer du style css sur les composents
void apply_css_style()
{
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider,"style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                   GTK_STYLE_PROVIDER(cssProvider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
}
//Changer la taille d'un composant
void change_size(GtkWidget* widget, gint width, gint height)
{
    gtk_widget_set_size_request(widget,width,height);
}
//Donner un nom a un widget
void give_name(GtkWidget* widget, gchar* nom)
{
    gtk_widget_set_name(widget,nom);
}


/*****************************************************************
                         WINDOW
******************************************************************/
/*
    Dans ce code on va definir la structure d'une fenetre
    ainsi les deux fonctions init_window et create_window
    La premiere nous permet d'initialiser les paramètre
    de la fenetre
    La deuxieme cree la fenetre avec les parametres deja
    initialisés
*/
//structure de donnees
typedef struct
{
    GtkWidget* window;   //La fenetre
    gchar* title;        //Titre de la fenetre
    gchar* icon;         //Icone pour la fenetre
    gint height;         //Hauteur de la fenetre
    gint width;          //Longueur de la fenetre
    gint position;       //Position de la fenetre
    gint posx;           //Position axe des x
    gint posy;           //Position axe des y
    gint resizable;      //Possibilite de changer la taille
}WindowObject;

    /**************************************************************
     La fonction : init_window
     -Entrees :  titre de la fenêtre
                 type de la fenêtre 't' TOPLEVEL ou 'p' POPUP
                 hauteur de notre fenêtre
                 largeur de notre fenêtre
                 position par defaults de la fenêtre :
                    0 = GTK_WIN_POS_NONE
                    1 = GTK_WIN_POS_CENTER
                    2 = GTK_WIN_POS_MOUSE
                    3 = GTK_WIN_POS_CENTER_ALWAYS
                    4 = GTK_WIN_POS_CENTER_ON_PARENT
                position dans l'axe des x
                position dans l'axe des y
                Possibilité de cha,ger la taille:
                    0=NON
                    1=OUI
     -Sortie: La structure WindowObjet apres initialisation
     -Description: Cette fonction permet d'allouer de l'espace memoire pour
                   notre variable de type WindowObject ,de l'initaliser
                   et la retourner
    ************************************************************************/
WindowObject* init_window(gchar* titre, gint h, gint w, gint pos,
                            gint x, gint y, gint res)
{
    WindowObject* Window = NULL;
    //Allocation memoire
    Window = (WindowObject*)malloc(sizeof(WindowObject));
    // Test d'allocation
    if(!Window)
    {
        printf("\nErreur d'allocation !!!\n");
        exit(1);
    }
    //Allocation du pointeur titre
    Window->title = (gchar*)malloc(30*sizeof(gchar));
    // affectation d'un titre a la fenetre
    if(titre)
        strcpy(Window->title,titre);
    else
        Window->title = NULL;

    Window->height = h; // initialisation de la hauteur
    Window->width = w; // initialisation de la largeur
    Window->position = pos; // initialisation de la position
    Window->posx = x;
    Window->posy = y;
    Window->resizable = res; // initialisation du parametre resizable
    return (WindowObject*)Window; //Retourner la structure
}
     /*****************************************************************
      La fonction : create_window
      Entree :  Un pointeur vers la structure WindowObject initialisée
      Sortie :  Un pointeur vers la variable WindowObject
      Description : Cette fonction permet de créer une fenêtre en fct
                    des paramètres récupérés de la structure WindowObject
    *******************************************************************/
WindowObject* create_window(WindowObject *WO)
{
    //Creation de la fenetre
    WO->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Attribution d'un titre
    if(WO->title) gtk_window_set_title(GTK_WINDOW(WO->window),WO->title);
    // Si le user introduit une hauteur et une largeur
    if((WO->height) && (WO->width))
    {
        gtk_window_set_default_size(GTK_WINDOW(WO->window),
                                (WO->height),(WO->width));
    }
    switch(WO->position)
    {
        // Position aleatoire
        case 1 : gtk_window_set_position(GTK_WINDOW(WO->window),
                                         GTK_WIN_POS_NONE);
                break ;
        // Au centre de l'ecran
        case 2 : gtk_window_set_position(GTK_WINDOW(WO->window),
                                         GTK_WIN_POS_CENTER);
                break;
        // La position du curseur
        case 3 : gtk_window_set_position(GTK_WINDOW(WO->window),
                                          GTK_WIN_POS_MOUSE);
                break;
        // Toujours au centre
        case 4 : gtk_window_set_position(GTK_WINDOW(WO->window),
                                         GTK_WIN_POS_CENTER_ALWAYS);
                break;
        // Au centre par rapport a l'élementt parent
        case 5 : gtk_window_set_position(GTK_WINDOW(WO->window),
                                        GTK_WIN_POS_CENTER_ON_PARENT);
                break;
        // Si l'utilisateur ne saisi pas la pos
        default :
                gtk_window_move(GTK_WINDOW(WO->window),
                                         WO->posx,WO->posy);
                break;
    }
    //Si res = 1, redimensionnement = VRAI
    if(WO->resizable)
        gtk_window_set_resizable(GTK_WINDOW(WO->window),TRUE);
    else     //Si res = 0, redimensionnement = FAUX
         gtk_window_set_resizable(GTK_WINDOW(WO->window),FALSE);
    return (WindowObject*)WO;
}
/*********************************************************************
  La fonction : add_to_window
  -Entrees :Un pointeur vers la variable WindowObject
            elem :le  GtkWidget à ajouter
  -Sortie : ajout d'un GtkWidget à la fenêtre
  -description: Cette fonction permet d'ajouter un widget à une fenetre
**********************************************************************/
void add_to_window(WindowObject* WO, GtkWidget* elem)
{
    //Ajout de elem à WO
    gtk_container_add(GTK_CONTAINER(WO->window),elem);
}

/************************************************************************
                               ICON
*************************************************************************/

//Structure de données
typedef struct
{
    GtkWidget* container;//L'objet racine de l'objet icone
    gchar* url;         //Le chemin de l'icone
}IconObject;

//Fonctions
/*********************************************************************
  La fonction : init_icon
  Entrees : le porteur de l'icone de type Gtkwidget*
            le chemin vers l'icone
  Sortie : un pointeur vers la structure IconeObject initialisée
 description: Cette fonction permet d'allouer de l'espace pour la
              structure de type IconObject , d'initialiser ses champs
              et de la retourner .
**********************************************************************/
IconObject* init_icon(GtkWidget* pere,gchar* chemin)
{
    //Allocation mémoire
    IconObject* IO = NULL;
    IO = (IconObject*)malloc(sizeof(IconObject));
    //Allocation pour le champs url
    IO->url = (gchar*)malloc(100*sizeof(gchar*));
    //Affectation des valeurs
    IO->container = pere;
    strcpy(IO->url,chemin);
    return (IconObject*)IO;
}
/*********************************************************************
La fonction : create_icon
-Entree : un pointeur vers une structure IconObject initialisé
-Sortie : un pointeur vers le GtkWidget
-decsription: Cette fonction permet la creation d'une icone
            en fct des paramètres récupérés de la structure
            IconObject initialisée
**********************************************************************/
GtkWidget* create_icon(IconObject* IO)
{
    gtk_window_set_icon_from_file(GTK_WINDOW(IO->container),IO->url,NULL);
    return((GtkWidget*)IO->container);
}


/*******************************************************************************
                                   BOX
*******************************************************************************/
/*
    Structure d'un box
    Initialisation du box
    Creation du box
*/
//Structure de données
typedef struct
{
    GtkWidget* box;      //L'objet box
    GtkWidget* container;//Son objet père
    gchar* name;         //Nom du box
    gchar type;         //Typedu box
    gint homogene;     //Homogene ou pas
    gint spacing;     //Espacement de l'objet
}BoxObject;

//Fonctions
 /**********************************************************
 La fonction : init_box
 -Entrees : Le container ou nous allons mettre notre box
            Le nom du box
            type du box :(V : pour vertical)
                         (H : pour horizontal)
            indicateur si le box est homogène ou non.
                         (1 : TRUE, 0 : FALSE)
            Espacement donner pour les éléments du box.
 -Sortie :  Pointeur versune structure BoxObject initialisée
 -description: Cette fonction permet d'allouer de l'espace pour la
              structure de type BoxObject , d'initialiser ses champs
              et de la retourner .
***********************************************************/
BoxObject* init_box(GtkWidget* pere, gchar* nom, gchar type,
                    gint homo, gint espace)
{
    //Allocation memoire
    BoxObject* BO = NULL;
    BO = (BoxObject*)malloc(sizeof(BoxObject));
    //Allocation pour le champs du nom
    BO->name = (gchar*)malloc(30*sizeof(gchar));
    //Affectation des valeurs de paramètres
    BO->container = pere; //Conteneur
    strcpy(BO->name,nom); //nom
    BO->type = type;      //Type
    BO->homogene = homo;  //homogene
    BO->spacing = espace; //espacement
    return (BoxObject*)BO;
}
/**********************************************************
La fonction : create box
-Entrees : pointeur vers une structure BoxObject initialisée
-Sortie :  pointeur vers une structure BoxObject
-Description:Cette fonction permet la creation d'un box
            en fct des paramètres récuperés de la structure
            BoxObject initialisée.
***********************************************************/
BoxObject* create_box(BoxObject* BO)
{
    //Vertical box
    if((BO->type == 'v') || (BO->type == 'V'))
    {
        if(BO->homogene) //Box Homogene
        {
            BO->box = gtk_box_new(TRUE,BO->spacing);
            gtk_orientable_set_orientation(GTK_ORIENTABLE(BO->box),
                                           GTK_ORIENTATION_VERTICAL);
        }
        else //Box non homogene
        {
            BO->box= gtk_box_new(TRUE,BO->spacing);
            gtk_orientable_set_orientation(GTK_ORIENTABLE(BO->box),
                                           GTK_ORIENTATION_VERTICAL);
        }
    }
    //Horizontal Box
    else if((BO->type == 'h') || (BO->type == 'H'))
    {
        if(BO->homogene) //Box homogene
        {
            BO->box = gtk_box_new(TRUE,BO->spacing);
            gtk_orientable_set_orientation(GTK_ORIENTABLE(BO->box),
                                           GTK_ORIENTATION_HORIZONTAL);
        }
        else           //Box non homogene
        {
            BO->box= gtk_box_new(TRUE,BO->spacing);
            gtk_orientable_set_orientation(GTK_ORIENTABLE(BO->box),
                                           GTK_ORIENTATION_HORIZONTAL);
        }
    }
    else
    {
        printf("\nLe type n'xiste pas !");exit(0);
    }
    gtk_widget_set_name(BO->box,BO->name);
    gtk_container_add(GTK_CONTAINER(BO->container),BO->box);
    return(BoxObject*)BO;
}
/**********************************************************
La fonction : add_to_box_start
-Entrees : Le box ou on va ajouter l'element
           L'element à ajouter
           expand :TRUE :  le fils aura un espace supplementaire
                   FALSE : Il aura son espace
           FILL : TRUE : remplir l'espace qui a été donné avec expand
                 ( si expand = FALSE, FILL ne sert a rien).
           padding : l'espace entre l'element et son voisin
-Sortie :  L'ajout d'élement dans un box
-Description:Permet d'jouter l'element child à l'element BO
***********************************************************/
void add_to_box_start(BoxObject* BO,GtkWidget *child,gboolean expand,
                      gboolean FILL,gint padding)
{
    gtk_box_pack_start(GTK_BOX(BO->box),child,expand,FILL,padding);
}
/**********************************************************
La fonction : add_to_box_end
-Entrees : Le box ou on va ajouter l'element
           L'element à ajouter
           expand :TRUE :  le fils aura un espace supplementaire
                   FALSE : Il aura son espace
           FILL : TRUE : remplir l'espace qui a été donné avec expand
                 ( si expand = FALSE, FILL ne sert a rien).
           padding : l'espace entre l'element et son voisin
-Sortie :  L'ajout d'élement dans un box
-Description:Permet d'jouter l'element child à l'element BO
***********************************************************/
void add_to_box_end(BoxObject* BO,GtkWidget *child,gboolean expand,
                      gboolean FILL,gint padding)
{
    gtk_box_pack_end(GTK_BOX(BO->box),child,expand,FILL,padding);
}

/******************************************************************************
                             TABLE
********************************************************************************/
/*
    Structure d'une table
    Fonction d'initialisation d'une table
    Fonction de création d'une table
    Fonction d'ajout à une table
*/
//Structure de données
typedef struct
{
  GtkWidget* table;    //l'objet table
  GtkWidget* container;//L'objet père
  gint nb_lines;       //nombre de lignes
  gint nb_columns;    //nombre de collones
  gint homogene;     //1 : TRUE , 0 : FALSE
}TableObject;
//Les fonctions


/********************************************************************
La fonction : init_table
-Entrees : l'objet qui va contenir notre table.
           nbl : le nombre de lignes
           nbc : le nombre de colonnes
           homo :l'homogenéité de la table(1:vrai,0:faux)
-Sortie : TO : objet table initialisé
-description: Cette fonction permet d'allouer de l'espace pour la
              structure de type TableObject, d'initialiser ses champs
              et de la retourner .
*********************************************************************/
TableObject* init_table(GtkWidget* pere, gint nbl, gint nbc, gint homo)
{
    TableObject* TO = NULL;
    //Allocation mémoire
    TO = (TableObject*)malloc(sizeof(TableObject));
    //Affectation des paramètres
    TO->container = pere;
    TO->nb_lines = nbl;
    TO->nb_columns = nbc;
    TO->homogene = homo;
    return (TableObject*)TO;
}
/**********************************************************
La fonction : create_table
-Entrees : objet table initialisé
-Sortie : Objet table créé
-description: Cette fonction permet la creation d'une table
            en fct des paramètres récuperés de la structure
            TableObject initialisée.
***********************************************************/
TableObject* create_table(TableObject* TO)
{
    if(TO->homogene == 1) //Table homogene
        TO->table=gtk_table_new(TO->nb_lines,TO->nb_columns,TRUE);
    else
        TO->table=gtk_table_new(TO->nb_lines,TO->nb_columns,FALSE);
    return (TableObject*)TO;
}
/**********************************************************
La fonction : add_to_table
-Entrees : TO :   l'objet table
           elem : l'element à ajouter
           left : Numéro de colonne auquel attacher
                  le côté gauche du widget enfant.
           right :Numéro de colonne auquel attacher
                  le côté droit du widget enfant.
           top :  Numéro de ligne auquel attacher
                  le haut du widget enfant.
           bottom :Numéro de ligne auquel attacher
                   le bas du widget enfant.
-Sortie : ajout d'un element à une table
-Description:Cette fonction permet d'ajouter un GtkWidet
             dans une table.
***********************************************************/
void add_to_table(TableObject* TO, GtkWidget* elem, gint left,
                  gint right, gint top, gint bottom)
{
    gtk_table_attach_defaults(GTK_TABLE(TO->table),elem,left,
                              right,top,bottom);
}

/******************************************************************************
                                BUTTON
********************************************************************************/
/*
    Dans ce code nous allons definir la strucuture boutton
    Et la fonction qui nous permet d'initialiser les parametres
    d'un bouton
    La fonction qui cree le bouton
*/
//Structure de donnes
typedef struct
{
    GtkWidget* button;    //L'objet bouton
    GtkWidget* container; //L'objet pere de bouton
    gchar* name;          //Nom du bouton
    gchar* label;         //L'etiquette du bouton
    gchar* url;           //Chemin icone du boutton
}ButtonObject;
//Fonctions
/**********************************************************
La fonction : init_button
-Entrees : pere : Conteneur du boutoon
           nom : Nom du boutton
           etiq : Etiquette du boutton
           url: chemin de l'icone du boutton
-Sortie : WO : Objet bouton
-Description: Cette fonction permet d'allouer de l'espace pour la
              structure de type ButtonObject, d'initialiser ses
              champs et de la retourner .
***********************************************************/
ButtonObject* init_button(GtkWidget* pere, gchar* nom, gchar* etiq,
                           gchar url[80])
{
    //Allocation memoire
    ButtonObject* BO = NULL;
    BO = (ButtonObject*)malloc(sizeof(ButtonObject));
    //Test d'allocation
    if(!BO)
    {
        printf("\nErreur d'allocation !!\n"); exit(0);
    }
    //Allocation du label et nom
    BO->label = (gchar*)malloc(30*sizeof(gchar));
    BO->name = (gchar*)malloc(30*sizeof(gchar));
    BO->url = (gchar*)malloc(100*sizeof(gchar));
    //affectation e l'etiquette et du nom
    if(etiq) strcpy(BO->label,etiq);
    else BO->label = NULL;
    if(nom) strcpy(BO->name,nom);
    else BO->name = NULL;
    //Url de l'image
    if(url) strcpy(BO->url,url);
    else BO->url = NULL;

    //Affecttion de la racine du bouton
    BO->container = pere;
     return(ButtonObject*)BO;
}
/**********************************************************
La fonction : create_button
-Entrees : BO : objet bouton initialisé
-Sortie : BO : Objet bouton créé
-Description:Cette fonction permet la creation d'un boutton
            en fct des paramètres récuperés de la structure
            ButtonObject initialisée.
***********************************************************/
ButtonObject* create_button(ButtonObject* BO)
{

    //Creation du bouton avec label
    if(BO->label)
       BO->button=gtk_button_new_with_label(BO->label);
    else BO->button = gtk_button_new();
    //l'image du boutton
    if(BO->url != NULL)
    {
        GtkWidget* image = gtk_image_new_from_file(BO->url);
        gtk_button_set_always_show_image(GTK_BUTTON(BO->button),TRUE);
        gtk_button_set_image(GTK_BUTTON(BO->button),image);
    }
    //Creation du nom
    if(BO->name) gtk_widget_set_name(BO->button,BO->name);
    return(ButtonObject*)BO;
}

/******************************************************************************
                          IMAGE
********************************************************************************/
/*
    Structure de données d'une image
    Fonction d'initialisation d'une image
    Fonction qui gère l'evenement sur l'image
    Fonction de creation d'une image
*/
//Structure de données
typedef struct
{
    GtkWidget* image;     //L'objet image
    GtkWidget* event_img; //L'evenement relatif à l'image
    gchar* name;          //Le nom de l'image
    gchar* path;          //chemin de l'image
    gint posx;
    gint posy;           //Position de l'mage
}ImageObject;
//Fonctions
/********************************************************************
La fonction : init_image
-Entrees : url : Le chemin de l'image
           nom: Le nom de l'image
           x, y : entiers : definissent la position de l'image
-Sortie : IO : objet image
-Description:Cette fonction permet d'allouer de l'espace memoire
             pour une structure de type ImageObject et d'initialiser
             ses champs.
*********************************************************************/
ImageObject* init_image(gchar* url, gchar* nom, gint x, gint y)
{
    //Allocation memoire
    ImageObject* IO = NULL;
    IO = (ImageObject*)malloc(sizeof(ImageObject));
    IO->path = (gchar*)malloc(100*sizeof(gchar));
    IO->name = (gchar*)malloc(30*sizeof(gchar));
    //Affectation du chemin
    strcpy(IO->path,url);
    //Affectation de positions
    IO->posx = x; IO->posy = y;
    return (ImageObject*)IO;
}
/********************************************************************
La fonction : create_image
-Entree : IO : objet image initialisé
-Sortie : IO : objet image créé
-Description:Cette fonction permet la creation d'une image
             en fct des paramètres récuperés de la structure
             ImageObject initialisée.
*********************************************************************/
ImageObject* create_image(ImageObject* IO)
{
    //Creation de l'image à partir du chemin
    IO->image = gtk_image_new_from_file(IO->path);
    if(IO->name) gtk_widget_set_name(IO->image,IO->name);
    return (ImageObject*)IO;
}


/***********************************************************************
                            Label
***********************************************************************/
/*
    Structure de données d'une label
    La fonction d'initialisation d'un objet label
    La fonction de création d'un objet label
*/
typedef struct
{
    GtkWidget* label;    //L'objet label
    GtkWidget* container;//Conteneur du label
    gchar* text;        //Le texte du label
    gchar* name;        //Nom du label pour css
    gint posx; gint posy; //Position du label
}LabelObject;
//Fonctions
/*******************************************************
La fonction : init_label
-Entrees : Le conteneur (pere) du label
           label : texte de l'objet label
           nom : le nom du label
           x, y : position du label
-Sortie : LO : objet label
-Description:Cette fonction permet d'allouer de l'espace memoire
             poura structure LabelObject, d'initialiser ses
             champs et de la retourner
********************************************************/
LabelObject* init_label(GtkWidget* pere, gchar* label, gchar* nom,
                         gint x, gint y)
{
    LabelObject* LO = NULL;
    //Allocation memoire
    LO = (LabelObject*)malloc(sizeof(LabelObject));
    LO->text = (gchar*)malloc(50*sizeof(gchar));
    LO->name = (gchar*)malloc(30*sizeof(gchar));
    //Affectation du texte, objet racine et position
    strcpy(LO->text,label);
    LO->container = pere;
    strcpy(LO->name,nom);
    LO->posx = x; LO->posy = y;
    return(LabelObject*)LO;
}
/*******************************************************
La fonction create_label
-Entree : LO : Objet label initialisé
-Sortie : LO : Objet label créé
-Description:Cette fonction permet la creation d'un Label
            en fct des paramètres récuperés de la structure
            LabelObject initialisée.
********************************************************/
LabelObject* create_label(LabelObject* LO)
{
    //Affectation du texte et ajout de l'objet à la son conteneur
    LO->label=gtk_label_new(LO->text);
    if(LO->name) gtk_widget_set_name(LO->label,LO->name);
    return (LabelObject*)LO;
}

/**************************************************************************
                            SCALE
**************************************************************************/
/*
    Structure de données d'un scale
    Fonction d'initialisation d'un scale
    Fonction de creation d'un scale
*/

//Structure de donnees
typedef struct
{
    GtkWidget* scale;    //L'objet scale
    GtkWidget* container;//L'objet parent
    gchar* name;         //Le nom du scale
    gchar type;          //Son type (H,V)
    gint Min;            //Valeur minimale
    gint Max;            //Valeur maximale
    gint step;           //Le pas du scale
}ScaleObject;


/*******************************************************
La fonction : init_scale
-Entrees : Le conteneur (pere) du scale
           nom : le nom du label
           type du scale (H / V)
           Sa valeur minimale
           Sa valeur maximale
           Lepas du scale
-Sortie : un pointeur vers une structure ScaleObject initialisée
-Description:Cette fonction permet d'allouer de l'espace memoire
             poura structure ScaleObject, d'initialiser ses
             champs et de la retourner
********************************************************/
ScaleObject* init_scale(GtkWidget* pere, gchar* nom, gchar type,
                        gint Min, gint Max, gint step)
{
    ScaleObject* SO = NULL;
    //Allocation memoire
    SO = (ScaleObject*)malloc(sizeof(ScaleObject));
    SO->name = (gchar*)malloc(30*sizeof(gchar));
    //Affectation des avaleurs
    SO->container = pere;
    strcpy(SO->name,nom);
    SO->type = type;
    SO->Min = Min;
    SO->Max = Max;
    SO->step = step;
    return (ScaleObject*)SO;
}

/*******************************************************
La fonction create_scale
-Entree : SO : Objet scale initialisé
-Sortie : SO : Objet scale créé
-Description:Cette fonction permet la creation d'un scale
            en fct des paramètres récuperés de la structure
            ScaleObject initialisée.
********************************************************/
ScaleObject* create_scale(ScaleObject* SO)
{
    //Creer le scale
    if(SO->type == 'H' || SO->type == 'h')
        SO->scale = gtk_hscale_new_with_range(SO->Min,SO->Max,SO->step);
    else  SO->scale = gtk_vscale_new_with_range(SO->Min,SO->Max,SO->step);
    //Definir la position du scale
    gtk_scale_set_value_pos(GTK_SCALE(SO->scale),GTK_POS_TOP);
    //Donner un nom
    if(SO->name) gtk_widget_set_name(SO->scale,SO->name);

    return (ScaleObject*)SO;
}

/****************************************************************************

*****************************************************************************/

/****************************************************************************
                            checkbutton
*****************************************************************************/

typedef struct
{
    GtkWidget *checkbutton; //L'objet checkboutton
    GtkWidget *container;   //son conteneur
    gchar *name;            //Nom
    gchar *label;           //Label
} checkbuttonobject;

/*****************************************************************************
           la fonction init_checkbutton
/**********************************************************
La fonction : init_checkbutton
-Entrees : pere : Conteneur du checkboutoon
           nom : Nom du checkboutton
           etiq : Etiquette du checkboutton
-Sortie : BO : Objet checkbouton
-Description: Cette fonction permet d'allouer de l'espace pour la
              structure de type checkButtonObject, d'initialiser ses
              champs et de la retourner .
***********************************************************/

checkbuttonobject* init_checkbutton(GtkWidget* pere, gchar* nom, gchar* etiq)
{
    //Allocation memoire
    checkbuttonobject* BO = NULL;
    BO = (checkbuttonobject*)malloc(sizeof(checkbuttonobject));
    //Test d'allocation
    if(!BO)
    {
        printf("\nErreur d'allocation !!\n"); exit(0);
    }
    //Allocation du label et nom
    BO->label = (gchar*)malloc(30*sizeof(gchar));
    BO->name = (gchar*)malloc(30*sizeof(gchar));
    //affectation e l'etiquette et du nom
    if(etiq) strcpy(BO->label,etiq);
    else BO->label = NULL;
    if(nom) strcpy(BO->name,nom);
    else BO->name = NULL;
    //Affecttion de la racine du bouton
    BO->container = pere;
     return(checkbuttonobject*)BO;
}

/******************************************************************
        la fonction create checkbutton
 /**********************************************************
La fonction : create_checkbutton
-Entrees : BO : objet checkbouton initialisé
-Sortie : BO : Objet checkbouton créé
-Description:Cette fonction permet la creation d'un checkboutton
            en fct des paramètres récuperés de la structure
            checkButtonObject initialisée.
***********************************************************/


checkbuttonobject *create_checkbutton(checkbuttonobject* BO)
{
  if(BO->label)
    BO->checkbutton=gtk_check_button_new_with_label(BO->label);
  else
    BO->checkbutton=gtk_check_button_new();
  return ((checkbuttonobject*)BO);
}


/**********************************************************************
                            COMBOBOX
***********************************************************************/
/**********************************************************
*   Structure   :   ComboBoxobject
*   --------------------------
*       Le prComboBox est le widget qui permete de creer
*   des listes dirolantes (dropdown)
*       les composants de cette structure sont :
*     l'objet combobox
*     le conteneur
*     nom de l'objet
*     la position de l'objet
***********************************************************/
typedef struct ComboBoxobject
{
    GtkWidget   *comboBox;  // notre objet
    GtkWidget   *container;    // le conteneur
    gchar       *nom;       // le nom de l'objet
    int   posx;
    int   posy;           // la position
} ComboBoxobject;



/******************************* prComboBox*****************************/

/***************************************************************************
*   Fonction   :   ComboBoxobject *init_combobox(GtkWidget*, gchar*)
*   ------------------------------------------------------------------
*
*       Cette fonction fait l allocation et initialisation des champs
*   de la structure prComboBox
*   Entre   :   - Le container nous allons mettre notre prComboBox
*               - Le selectioneur du comboBox (pour le CSS)
*
*   Sortie  :   Un objet ComboBoxobject apres allocation et initialisation
***************************************************************************/

ComboBoxobject  *init_combobox(GtkWidget *pere, gchar *nom,int x,int y)
{
    ComboBoxobject *cb = NULL;
    cb = (ComboBoxobject*) malloc (sizeof (ComboBoxobject));
    cb->comboBox = NULL;
    cb->container = pere;

    if (nom)
    {
        cb->nom = (gchar*) malloc (35*sizeof (gchar));
        strcpy (cb->nom, nom);
    }
    else cb->nom = NULL;
    cb->posx=x;
    cb->posy=y;
    return ((ComboBoxobject*) cb);
}
/***************************************************************************
*   Fonction : ComboBoxobject *create_combobox(ComboBoxobject *cb)
*   ------------------------------------------------------------------
*
*       Cette fonction fait l allocation et initialisation des champs
*   de la structure prComboBox.
*
*   Entre   :   - Les donnees de l objet a creer
*   Sortie  :   - L'objet creer
***************************************************************************/
  ComboBoxobject  *create_combobox (ComboBoxobject *cb)
{
    cb->comboBox = gtk_combo_box_text_new();
    gtk_widget_set_name (cb->comboBox, cb->nom);

    return ((ComboBoxobject*) cb);
}
/***************************************************************************
*   Fonction   : void combobox_add (ComboBoxobject*, gchar*)
*   ---------------------------------------------------------------
*       Cette fonction fait l ajout d un nouveau champs a au comboBox
*   passe en arguments.
*
*   Entre   :   - L'objet comboBox des items
*               - Le text de l item a ajouter
*   Sortie  :   void
***************************************************************************/
void    combobox_add (ComboBoxobject *cb, gchar *text)
{
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(cb->comboBox), NULL, text);
    gtk_combo_box_set_active(GTK_COMBO_BOX(cb->comboBox), 0);
}
/***************************************************************************
*   Fonction   : void combobox_get (ComboBoxobject*)
*   ---------------------------------------------------------------
*       Cette fonction retourne le text de l item selectionner dans
*   un comboBox donne.
*
*   Entre   :   L objet comboBox des items
*   Sortie  :   Le text de l item selectionnee
***************************************************************************/
gchar   *combobox_get (ComboBoxobject *cb)
{
    return ((gchar*) gtk_combo_box_text_get_active_text(
                                        GTK_COMBO_BOX_TEXT(cb->comboBox)));
}

gchar * combobox_value(ComboBoxobject *combo)
{

    gboolean b_ret = FALSE;
    GtkTreeIter iter;
    gchar *conteneur;
    g_return_val_if_fail (combo->comboBox != NULL, conteneur);
    b_ret = gtk_combo_box_get_active_iter (combo->comboBox, &iter);
    if (b_ret)
        {
          GtkTreeModel *p_model = NULL;
          p_model = gtk_combo_box_get_model (combo->comboBox);
            if (p_model != NULL)
                  gtk_tree_model_get (p_model, &iter, 0, &conteneur, -1);
        }
    ((gchar*)conteneur);
}




#endif // MYFUNCTIONS_H_INCLUDED
