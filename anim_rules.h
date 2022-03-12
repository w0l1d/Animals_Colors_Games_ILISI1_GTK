#ifndef ANIM_RULES_H_INCLUDED
#define ANIM_RULES_H_INCLUDED
#include "math.h"
#include "myheaders.h"

/*Structure pour stocker les caractéristiques
  d'un animal*/
typedef struct
{
    int p_vue; //sa portee de vue
    int vitesse;//Vitesse ou pas de deplacement
}caract;

//La structure de la fenetre de parametrage
typedef struct
{
    WindowObject *ac_window; //fenetre des regles
    BoxObject *ac_global_box; // horizontal box
    TableObject *ac_tableau;
    ComboBoxobject *combo[20];
    caract car[10]; //Un tableua pour stocker les car choisies
    ButtonObject *OKEY;
    ButtonObject *QUIT;
}rule_anim;


//Une fonction d'initialistaion et creation de la fenetre de parametres
rule_anim *init_rule_anim()
{
    //Allocation memoire
    rule_anim *RA=(rule_anim*)malloc(sizeof(rule_anim));
    if(!RA)
    {
        printf("erreur d'allocation !!");
        exit(EXIT_FAILURE);
    }

    RA->ac_window=init_window("Parametrage",400,400,2,0,0,1);
    RA->ac_window=create_window(RA->ac_window);

    RA->ac_global_box=init_box(RA->ac_window->window,"acbox",'v',1,0);
    RA->ac_global_box=create_box(RA->ac_global_box);
    add_to_window(RA->ac_window,RA->ac_global_box->box);

    RA->ac_tableau=init_table(RA->ac_global_box->box,15,5,1);
    RA->ac_tableau=create_table(RA->ac_tableau);
    add_to_box_start(RA->ac_global_box,RA->ac_tableau->table,1,1,0);

                   /**Les labels **/
    LabelObject *vitesse=init_label(RA->ac_tableau->table,"Pas","Pas",0,0);
    vitesse=create_label(vitesse);
    add_to_table(RA->ac_tableau,vitesse->label,1,2,1,2);

    LabelObject *vue=init_label(RA->ac_tableau->table,"Portee de vue","Portee de vue",0,0);
    vue=create_label(vue);
    add_to_table(RA->ac_tableau,vue->label,3,4,1,2);

              /*Creation des combobox de caracteristiques*/
    LabelObject *LION=init_label(RA->ac_tableau->table,"Lion","Lion",0,0);
    LION=create_label(LION);
    add_to_table(RA->ac_tableau,LION->label,0,1,2,3);

    RA->combo[0] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[0] =create_combobox(RA->combo[0]);
    combobox_add(RA->combo[0],"1");
    combobox_add(RA->combo[0] ,"2");
    combobox_add(RA->combo[0] ,"3");
    combobox_add(RA->combo[0] ,"4");
    combobox_add(RA->combo[0] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[0]->comboBox,1,2,2,3);

    RA->combo[1] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[1] =create_combobox(RA->combo[1]);
    combobox_add(RA->combo[1],"1");
    combobox_add(RA->combo[1] ,"3");
    combobox_add(RA->combo[1] ,"5");
    combobox_add(RA->combo[1] ,"10");
    combobox_add(RA->combo[1] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[1]->comboBox,3,4,2,3);

    LabelObject *LEO=init_label(RA->ac_tableau->table,"Leopard","Leopard",0,0);
    LEO=create_label(LEO);
    add_to_table(RA->ac_tableau,LEO->label,0,1,3,4);

    RA->combo[2] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[2] =create_combobox(RA->combo[2]);
    combobox_add(RA->combo[2],"1");
    combobox_add(RA->combo[2] ,"2");
    combobox_add(RA->combo[2] ,"3");
    combobox_add(RA->combo[2] ,"4");
    combobox_add(RA->combo[2] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[2]->comboBox,1,2,3,4);

    RA->combo[3] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[3] =create_combobox(RA->combo[3]);
    combobox_add(RA->combo[3],"1");
    combobox_add(RA->combo[3] ,"3");
    combobox_add(RA->combo[3] ,"5");
    combobox_add(RA->combo[3] ,"10");
    combobox_add(RA->combo[3] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[3]->comboBox,3,4,3,4);

    LabelObject *ELE=init_label(RA->ac_tableau->table,"Renard","Renard",0,0);
    ELE=create_label(ELE);
    add_to_table(RA->ac_tableau,ELE->label,0,1,4,5);

    RA->combo[4] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[4] =create_combobox(RA->combo[4]);
    combobox_add(RA->combo[4],"1");
    combobox_add(RA->combo[4] ,"2");
    combobox_add(RA->combo[4] ,"3");
    combobox_add(RA->combo[4] ,"4");
    combobox_add(RA->combo[4] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[4]->comboBox,1,2,4,5);

    RA->combo[5] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[5] =create_combobox(RA->combo[5]);
    combobox_add(RA->combo[5],"1");
    combobox_add(RA->combo[5] ,"3");
    combobox_add(RA->combo[5] ,"5");
    combobox_add(RA->combo[5] ,"10");
    combobox_add(RA->combo[5] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[5]->comboBox,3,4,4,5);

    LabelObject *GI=init_label(RA->ac_tableau->table,"Loup","Loup",0,0);
    GI=create_label(GI);
    add_to_table(RA->ac_tableau,GI->label,0,1,5,6);

    RA->combo[6] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[6] =create_combobox(RA->combo[6]);
    combobox_add(RA->combo[6],"1");
    combobox_add(RA->combo[6] ,"2");
    combobox_add(RA->combo[6] ,"3");
    combobox_add(RA->combo[6] ,"4");
    combobox_add(RA->combo[6] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[6]->comboBox,1,2,5,6);

    RA->combo[7] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[7] =create_combobox(RA->combo[7]);
    combobox_add(RA->combo[7],"1");
    combobox_add(RA->combo[7] ,"3");
    combobox_add(RA->combo[7] ,"5");
    combobox_add(RA->combo[7] ,"10");
    combobox_add(RA->combo[7] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[7]->comboBox,3,4,5,6);

    LabelObject *W=init_label(RA->ac_tableau->table,"Hyene","Hyene",0,0);
    W=create_label(W);
    add_to_table(RA->ac_tableau,W->label,0,1,6,7);

    RA->combo[8] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[8] =create_combobox(RA->combo[8]);
    combobox_add(RA->combo[8],"1");
    combobox_add(RA->combo[8] ,"2");
    combobox_add(RA->combo[8] ,"3");
    combobox_add(RA->combo[8] ,"4");
    combobox_add(RA->combo[8] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[8]->comboBox,1,2,6,7);

    RA->combo[9] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[9] =create_combobox(RA->combo[9]);
    combobox_add(RA->combo[9],"1");
    combobox_add(RA->combo[9] ,"3");
    combobox_add(RA->combo[9] ,"5");
    combobox_add(RA->combo[9] ,"10");
    combobox_add(RA->combo[9] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[9]->comboBox,3,4,6,7);

    LabelObject *E=init_label(RA->ac_tableau->table,"Elephant","Elephant",0,0);
    E=create_label(E);
    add_to_table(RA->ac_tableau,E->label,0,1,7,8);

    RA->combo[10] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[10] =create_combobox(RA->combo[10]);
    combobox_add(RA->combo[10],"1");
    combobox_add(RA->combo[10] ,"2");
    combobox_add(RA->combo[10] ,"3");
    combobox_add(RA->combo[10] ,"4");
    combobox_add(RA->combo[10] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[10]->comboBox,1,2,7,8);

    RA->combo[11] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[11] =create_combobox(RA->combo[11]);
    combobox_add(RA->combo[11],"1");
    combobox_add(RA->combo[11] ,"3");
    combobox_add(RA->combo[11] ,"5");
    combobox_add(RA->combo[11] ,"10");
    combobox_add(RA->combo[11] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[11]->comboBox,3,4,7,8);

    LabelObject *LI=init_label(RA->ac_tableau->table,"Lievre","Lievre",0,0);
    LI=create_label(LI);
    add_to_table(RA->ac_tableau,LI->label,0,1,8,9);

    RA->combo[12] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[12] =create_combobox(RA->combo[12]);
    combobox_add(RA->combo[12],"1");
    combobox_add(RA->combo[12] ,"2");
    combobox_add(RA->combo[12] ,"3");
    combobox_add(RA->combo[12] ,"4");
    combobox_add(RA->combo[12] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[12]->comboBox,1,2,8,9);

    RA->combo[13] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[13] =create_combobox(RA->combo[13]);
    combobox_add(RA->combo[13],"1");
    combobox_add(RA->combo[13] ,"3");
    combobox_add(RA->combo[13] ,"5");
    combobox_add(RA->combo[13] ,"10");
    combobox_add(RA->combo[13] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[13]->comboBox,3,4,8,9);

    LabelObject *AN=init_label(RA->ac_tableau->table,"Antilope","Antilope",0,0);
    AN=create_label(AN);
    add_to_table(RA->ac_tableau,AN->label,0,1,9,10);
    RA->combo[14] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[14] =create_combobox(RA->combo[14]);
    combobox_add(RA->combo[14],"1");
    combobox_add(RA->combo[14] ,"2");
    combobox_add(RA->combo[14] ,"3");
    combobox_add(RA->combo[14] ,"4");
    combobox_add(RA->combo[14] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[14]->comboBox,1,2,9,10);

    RA->combo[15] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[15] =create_combobox(RA->combo[15]);
    combobox_add(RA->combo[15],"1");
    combobox_add(RA->combo[15] ,"3");
    combobox_add(RA->combo[15] ,"5");
    combobox_add(RA->combo[15] ,"10");
    combobox_add(RA->combo[15] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[15]->comboBox,3,4,9,10);

    LabelObject *Z=init_label(RA->ac_tableau->table,"Zebre","Zebre",0,0);
    Z=create_label(Z);
    add_to_table(RA->ac_tableau,Z->label,0,1,10,11);

    RA->combo[16] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[16] =create_combobox(RA->combo[16]);
    combobox_add(RA->combo[16],"1");
    combobox_add(RA->combo[16] ,"2");
    combobox_add(RA->combo[16] ,"3");
    combobox_add(RA->combo[16] ,"4");
    combobox_add(RA->combo[16] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[16]->comboBox,1,2,10,11);

    RA->combo[17] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[17] =create_combobox(RA->combo[17]);
    combobox_add(RA->combo[17],"1");
    combobox_add(RA->combo[17] ,"3");
    combobox_add(RA->combo[17] ,"5");
    combobox_add(RA->combo[17] ,"10");
    combobox_add(RA->combo[17] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[17]->comboBox,3,4,10,11);

    LabelObject *M=init_label(RA->ac_tableau->table,"Mouton","Mouton",0,0);
    M=create_label(M);
    add_to_table(RA->ac_tableau,M->label,0,1,11,12);

    RA->combo[18] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[18] =create_combobox(RA->combo[18]);
    combobox_add(RA->combo[18],"1");
    combobox_add(RA->combo[18] ,"2");
    combobox_add(RA->combo[18] ,"3");
    combobox_add(RA->combo[18] ,"4");
    combobox_add(RA->combo[18] ,"5");
    add_to_table(RA->ac_tableau,RA->combo[18]->comboBox,1,2,11,12);

    RA->combo[19] =init_combobox(RA->ac_tableau->table,"middle",0,0);
    RA->combo[19] =create_combobox(RA->combo[19]);
    combobox_add(RA->combo[19],"1");
    combobox_add(RA->combo[19] ,"3");
    combobox_add(RA->combo[19] ,"5");
    combobox_add(RA->combo[19] ,"10");
    combobox_add(RA->combo[19] ,"15");
    add_to_table(RA->ac_tableau,RA->combo[19]->comboBox,3,4,11,12);




    RA->OKEY=init_button(RA->ac_tableau->table,"RETOUR","RETOUR","images/conf.png");
    RA->OKEY=create_button(RA->OKEY);
    add_to_table(RA->ac_tableau,RA->OKEY->button,1,2,14,15);

    RA->QUIT=init_button(RA->ac_tableau->table,"Confirmer","Configurer","images/conf.png");
    RA->QUIT=create_button(RA->QUIT);
    add_to_table(RA->ac_tableau,RA->QUIT->button,3,4,14,15);

    int i;
    for(i=0;i<10;i++) RA->car[i].p_vue=2;
    for(i=0;i<10;i++) RA->car[i].vitesse=2;

    return((rule_anim*)RA);
}

/** Une fonction qui change une chaine de caractère
    en chiffre**/

int change(char test[50])
{
    char numbr[50]="";
    strcpy(numbr,test);
    int taille=strlen(numbr)-1;
    int puissance=1;
    int mynum=0;
    while(taille>=0)
    {
        mynum+= ((int)(numbr[taille]-'0'))*puissance;
        taille--;
        puissance*=10;
    }
    return ((int)mynum);
}

/** La fonction de rappel appelée lorsqu'on confirme nos choix
    La fct stock les differentes caractéristiques
**/
void on_conf(GtkWidget *w,gpointer data)
{
    rule_anim *RA=(rule_anim*)data;
    int i;
    gchar *choice;
    //Affichage des vitesse avant la configuration
    for(i=0;i<10;i++)
    {
        printf("vitesse %d vue %d\n",RA->car[i].vitesse,RA->car[i].p_vue);
    }
    for(i=0;i<20;i++)
    {
        choice =combobox_get(RA->combo[i]);
        //Stockage des vitesses
        if    (i%2==0)  RA->car[i/2].vitesse=(int)change(choice);
        //Stocker les portees de vue
        else  RA->car[i/2].p_vue=(int)change(choice);
    }
    //Affichage des vitesses apres la configuration
    for(i=0;i<10;i++)
    {
        printf("vitesse %d vue %d\n",RA->car[i].vitesse,RA->car[i].p_vue);
    }
}

void event_calls_ca(rule_anim *RA)
{
    g_signal_connect(G_OBJECT(RA->ac_window->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(RA->QUIT->button),"clicked",G_CALLBACK(on_conf),RA);
}

#endif // ANIM_RULES_H_INCLUDED
