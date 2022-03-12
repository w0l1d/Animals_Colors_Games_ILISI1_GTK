#ifndef LESREGLES_H_INCLUDED
#define LESREGLES_H_INCLUDED

#include "myheaders.h"
#include "choisir_parmis_les_regles.h"
#include "ur_own_rules.h"

/** la structure de la fenetre des règles**/
typedef struct
{
   WindowObject *Rules_window; //fenetre des regles
   IconObject *r_icon;
   BoxObject *r_global_box; // horizontal box
   TableObject *r_tableau;

   LabelObject *aleatoire_label;
   ButtonObject *aleatoire;   //bouton aleatoire

   LabelObject *creer_label;
   ButtonObject *creer;      //bouton creer

   LabelObject *parmi_label;
   ButtonObject *parmi;      //bouton du choix parmi


   GtkWidget *btnradio1;
   GtkWidget *btnradio2;
   GtkWidget *btnradio3;

   int mes_choix;      //indique quel mode a été choisi
   int etat;

   rp_win *parmis_regles;
   dr_win *creer_regles;

   ButtonObject *configure_button;
   ButtonObject *leave_button;

   checkbuttonobject *etat_init; //Le mode d'état initial

}rules_structure;

rules_structure * init_rules_window()
{
    rules_structure *RW=(rules_structure*)malloc(sizeof(rules_structure));
    if(!RW)
    {
        printf("erreur d'allocation !!");
        exit(EXIT_FAILURE);
    }

    RW->mes_choix=1;  //Mode aleatoire par defaut
    //initialiser la fenetre de choix parmi les regles
    rp_win *rpwin=init_rp_win();
    event_calls_rpwin(rpwin);
    RW->parmis_regles=rpwin;
    //initialisation dela fenetre de creation de règles
    dr_win *drwin=init_dr_win();
    event_calls_drwin(drwin);
    RW->creer_regles=drwin;

    RW->Rules_window=init_window("PARAMETRES",400,400,2,0,0,1);
    RW->Rules_window=create_window(RW->Rules_window);

    RW->r_icon=init_icon(RW->Rules_window->window,"images/60473.png");
    RW->r_icon=create_icon(RW->r_icon);

    RW->r_global_box=init_box(RW->Rules_window->window,"global",'h',1,20);
    RW->r_global_box=create_box(RW->r_global_box);
    add_to_window(RW->Rules_window,RW->r_global_box->box);

    RW->r_tableau=init_table(RW->r_global_box->box,7,7,1);
    RW->r_tableau=create_table(RW->r_tableau);
    add_to_box_end(RW->r_global_box,RW->r_tableau->table,1,1,0);

        /** leave & configuration button **/

    RW->leave_button=init_button(RW->r_tableau->table,"leave","leave",NULL);
    RW->leave_button=create_button(RW->leave_button);
    add_to_table(RW->r_tableau,RW->leave_button->button,1,3,5,6);

    RW->configure_button=init_button(RW->r_tableau->table,"configure","configure",NULL);
    RW->configure_button=create_button(RW->configure_button);
    add_to_table(RW->r_tableau,RW->configure_button->button,4,6,5,6);

       /**  les buttons de la fenetre **/

    RW->aleatoire=init_button(RW->r_tableau->table,"aleatoire","aleatoire",NULL);
    RW->aleatoire=create_button(RW->aleatoire);
    add_to_table(RW->r_tableau,RW->aleatoire->button,4,6,1,2);

    RW->creer=init_button(RW->r_tableau->table,"Creer","Creer",NULL);
    RW->creer=create_button(RW->creer);
    add_to_table(RW->r_tableau,RW->creer->button,4,6,2,3);

    RW->parmi=init_button(RW->r_tableau->table,"Choisir","Choisir",NULL);
    RW->parmi=create_button(RW->parmi);
    add_to_table(RW->r_tableau,RW->parmi->button,4,6,3,4);

    RW->btnradio1=gtk_radio_button_new_with_label(NULL,"1");
    RW->btnradio2=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(RW->btnradio1),"2");
    RW->btnradio3=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(RW->btnradio1),"3");

    RW->etat_init=init_checkbutton(RW->r_tableau,"etat initial aleatoire","etat initial aleatoire");
    RW->etat_init=create_checkbutton(RW->etat_init);
    gtk_toggle_button_set_active(RW->etat_init->checkbutton,TRUE);

    add_to_table(RW->r_tableau,RW->etat_init->checkbutton,2,4,0,1);
    add_to_table(RW->r_tableau,RW->btnradio1,6,7,1,2);
    add_to_table(RW->r_tableau,RW->btnradio2,6,7,2,3);
    add_to_table(RW->r_tableau,RW->btnradio3,6,7,3,4);

       /** les labels des boutons  **/

    RW->aleatoire_label=init_label(RW->r_tableau->table,"Regle Aleatoire","Regle Aleatoire :",0,0);
    RW->aleatoire_label=create_label(RW->aleatoire_label);
    add_to_table(RW->r_tableau,RW->aleatoire_label->label,0,4,1,2);

    RW->creer_label=init_label(RW->r_tableau->table,"Ajouter Des Regles","Ajouter Des Regles :",0,0);
    RW->creer_label=create_label(RW->creer_label);
    add_to_table(RW->r_tableau,RW->creer_label->label,0,4,2,3);

    RW->parmi_label=init_label(RW->r_tableau->table,"Choisir des regles","Choisir des regles:",0,0);
    RW->parmi_label=create_label(RW->parmi_label);
    add_to_table(RW->r_tableau,RW->parmi_label->label,0,4,3,4);

    RW->etat=1; //Etat aléatoire

    return ((rules_structure*)RW);
}

//Une fenetre qui affiche la règles aléatoire utilisée
//La fenetre est affichée lors de l'appel de cette fonction
//de rappel en cliquant sur le bouton "aleatoire"
void on_aleatoire_rules()
{
    WindowObject *fenetre=init_window("regles aleatoires",500,500,2,0,0,1);
    fenetre=create_window(fenetre);

    BoxObject *gbox=init_box(fenetre->window,"box",'v',1,0);
    gbox=create_box(gbox);
    add_to_window(fenetre,gbox->box);

    TableObject *table_x=init_table(gbox->box,10,6,1);
    table_x=create_table(table_x);
    add_to_box_start(gbox,table_x->table,1,1,0);

    LabelObject *glabel=init_label(table_x->table,"la regle utilisee :","la regles utilisee :",0,0);
    glabel=create_label(glabel);
    add_to_table(table_x,glabel->label,1,5,0,1);

    ImageObject *ruleimg=init_image("images/rl1.png","regle aleatoire",0,0);
    ruleimg=create_image(ruleimg);
    add_to_table(table_x,ruleimg->image,0,6,1,2);

    ImageObject *ruleimg2=init_image("images/random_rule.png","regle aleatoire",0,0);
    ruleimg2=create_image(ruleimg2);
    add_to_table(table_x,ruleimg2->image,1,5,2,9);

    display_all(fenetre->window);
}

/* la fonction rappel appelée quandon clique
   sur le bouton de choix parmi les regles proposées */
void on_parmi_rules(GtkWidget *w, gpointer data)
{
   rules_structure *RS=(rules_structure*)data;
   gtk_widget_hide(RS->Rules_window->window);
   gtk_widget_show_all(RS->parmis_regles->w->window);
}

void on_return_from_parmi_rules(GtkWidget *w, gpointer data)
{
   rules_structure *RS=(rules_structure*)data;
   gtk_widget_hide(RS->parmis_regles->w->window);
    gtk_widget_show_all(RS->Rules_window->window);
}

/* la fonction rappel appelée quand on clique
   sur le bouton de création de règles */
 void on_creer_rules(GtkWidget *w, gpointer data)
{
   rules_structure *RS=(rules_structure*)data;
   gtk_widget_hide(RS->Rules_window->window);
   gtk_widget_show_all(RS->creer_regles->w->window);
}

void on_return_from_creer_rules(GtkWidget *w, gpointer data)
{
   rules_structure *RS=(rules_structure*)data;
   gtk_widget_hide(RS->creer_regles->w->window);
   gtk_widget_show_all(RS->Rules_window->window);
}

/*Cette fonction de rappel permet de récupérer
  le choix qu'à effectuer l'utilisateur concernant
  l'état initial du jeu ainsi le mode de règles */
void on_configure_click(GtkWidget *w , gpointer data)
{
    rules_structure *RS=(rules_structure*)data;

    RS->mes_choix=0;

    GSList *malist=NULL;
    const gchar *slabel;
    malist=gtk_radio_button_get_group(GTK_RADIO_BUTTON(RS->btnradio1));
    while(malist)
    {
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(malist->data)))
          {
            slabel=gtk_button_get_label(GTK_BUTTON(malist->data));
            malist=NULL;
          }
          else
          {
              malist=g_slist_next(malist);
          }
    }
    if(strcmp(slabel,"1")==0)RS->mes_choix=1;
    if(strcmp(slabel,"2")==0)RS->mes_choix=2;
    if(strcmp(slabel,"3")==0)RS->mes_choix=3;

       printf("vous aves choisi le mode %d\n",RS->mes_choix);

    if(gtk_toggle_button_get_active(RS->etat_init->checkbutton) )RS->etat=1;
    else RS->etat=0;
}


void event_calls_rules(rules_structure *RW)
{

    g_signal_connect(G_OBJECT(RW->Rules_window->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(RW->configure_button->button),"clicked",G_CALLBACK(on_configure_click),RW);

    g_signal_connect(G_OBJECT(RW->aleatoire->button),"clicked",G_CALLBACK(on_aleatoire_rules),NULL);

    g_signal_connect(G_OBJECT(RW->parmi->button),"clicked",G_CALLBACK(on_parmi_rules),RW);
    g_signal_connect(G_OBJECT(RW->parmis_regles->leave_btn->button),"clicked",G_CALLBACK(on_return_from_parmi_rules),RW);

    g_signal_connect(G_OBJECT(RW->creer->button),"clicked",G_CALLBACK(on_creer_rules),RW);
    g_signal_connect(G_OBJECT(RW->creer_regles->re_leave->button),"clicked",G_CALLBACK(on_return_from_creer_rules),RW);
}



#endif // LESREGLES_H_INCLUDED
