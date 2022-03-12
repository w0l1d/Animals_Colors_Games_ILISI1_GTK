#ifndef INTERFACE_GLOBALE_H_INCLUDED
#define INTERFACE_GLOBALE_H_INCLUDED

#include "myheaders.h"
#include "acc_aleatoire.h"
#include "animals.h"

/** La structure d el'interface **/
typedef struct
{
    WindowObject* W_GAMES;//Fenetre du jeu
    IconObject* IC;//L'icone de la page principale
    TableObject* TM;//Table du jeu
    LabelObject* LAB;//Label du menu principal
    ButtonObject* jeu_ACC;//Bouton automate cellulaire coloré
    acc_alt *automate_cel_with_colors; // pointeur vers le jeu
    ButtonObject *jeu_animaux; // button jeu d'animaux
    animals_game *jeu_d_animaux; // pointeur vers le jeu
    ButtonObject* quitter;
}interface;

/**Initialisation de l'interface globale **/
interface *init_interface()
{
    interface *INT=(interface*)malloc(sizeof(interface));
    if(!INT)
    {
        printf("erreur d'allocation !!");
        exit(EXIT_FAILURE);
    }
    // liaison du jeu automate cellulaire coloré
    acc_alt*game_ACC=init_ACC();
    event_calls_acc(game_ACC);
    INT->automate_cel_with_colors=game_ACC;
    // liaison du jeu d'animaux
    animals_game *GA=init_animals_game();
    event_calls_animals_game(GA);
    INT->jeu_d_animaux=GA;
    // creation de la fenetre globale
    INT->W_GAMES=init_window("GAMES",300,300,2,0,0,1);
    INT->W_GAMES=create_window(INT->W_GAMES);
    // creation d'icone du jeu
    INT->IC=init_icon(INT->W_GAMES->window,"images/icon.png");
    INT->W_GAMES->window=create_icon(INT->IC);
    // creation du tableau contenant les buttons
    INT->TM=init_table(INT->W_GAMES->window,8,7,1);
    INT->TM=create_table(INT->TM);
    add_to_window(INT->W_GAMES,INT->TM->table);
    // ajout du label
    INT->LAB=init_label(INT->TM->table,"BIENVENUE!","welcome",0,0);
    INT->LAB=create_label(INT->LAB);
    add_to_table(INT->TM,INT->LAB->label,2,5,1,2);
    // ajout des buttons
        // button du jeu acc
           INT->jeu_ACC=init_button(INT->TM->table,"Jeu de Couleurs","Jeu de Couleurs",NULL);
           INT->jeu_ACC=create_button(INT->jeu_ACC);
           add_to_table(INT->TM,INT->jeu_ACC->button,2,5,3,4);
        // button du jeu d'animaux
           INT->jeu_animaux=init_button(INT->TM->table,"Jeu d'animaux","Jeu d'animaux",NULL);
           INT->jeu_animaux=create_button(INT->jeu_animaux);
           add_to_table(INT->TM,INT->jeu_animaux->button,2,5,4,5);
        // button pour quitter
           INT->quitter=init_button(INT->TM->table,"quitter","QUITTER",NULL);
           INT->quitter=create_button(INT->quitter);
           add_to_table(INT->TM,INT->quitter->button,2,5,5,6);

    return ((interface*)INT);
}

/** Une fonction de rappel liée au bouton du jeu de couleurs**/
void on_jeu_acc(GtkWidget *widget,gpointer data)
{
    // allocation de l'interface
    interface* INT = (interface*)data;
    // cacher la fenetre de l'interface
    gtk_widget_hide(INT->W_GAMES->window);
    // afficher la fenetre du jeu acc
    gtk_widget_show_all(INT->automate_cel_with_colors->W->window);
}

void on_return_from_jeu_acc(GtkWidget *widget,gpointer data)
{
    // allocation de l'interface
    interface *INT = (interface*)data;
    // cacher la fenêtre du jeu acc
    gtk_widget_hide(INT->automate_cel_with_colors->W->window);
    // afficher la fenêtre de l'interface globale
    gtk_widget_show_all(INT->W_GAMES->window);
}

 /** La fonction d erappel liée au bouton
     du jeu des animaux **/
void on_jeu_animaux(GtkWidget *widget,gpointer data)
{
    // allocation de l'interface
    interface* INT = (interface*)data;
    // cacher la fenetre de l'interface
    gtk_widget_hide(INT->W_GAMES->window);
    // afficher la fenetre du jeu acc
    gtk_widget_show_all(INT->jeu_d_animaux->W->window);
}

void on_return_from_jeu_animals(GtkWidget *widget,gpointer data)
{
    // allocation de l'interface
    interface *INT = (interface*)data;
    // cacher la fenêtre du jeu acc
    gtk_widget_hide(INT->jeu_d_animaux->W->window);
    // afficher la fenêtre de l'interface globale
    gtk_widget_show_all(INT->W_GAMES->window);
}

void event_calls_main_interface(interface *INT)
{
    g_signal_connect(G_OBJECT(INT->W_GAMES->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

    g_signal_connect(G_OBJECT(INT->quitter->button),"clicked",G_CALLBACK(gtk_main_quit),NULL);

    g_signal_connect(G_OBJECT(INT->jeu_ACC->button),"clicked",G_CALLBACK(on_jeu_acc),INT);

    g_signal_connect(G_OBJECT(INT->automate_cel_with_colors->retourner->button),"clicked",G_CALLBACK(on_return_from_jeu_acc),INT);

    g_signal_connect(G_OBJECT(INT->jeu_animaux->button),"clicked",G_CALLBACK(on_jeu_animaux),INT);

    g_signal_connect(G_OBJECT(INT->jeu_d_animaux->retourner->button),"clicked",G_CALLBACK(on_return_from_jeu_animals),INT);
}



#endif // INTERFACE_GLOBALE_H_INCLUDED
