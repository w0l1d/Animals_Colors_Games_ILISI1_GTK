#ifndef UR_OWN_RULES_H_INCLUDED
#define UR_OWN_RULES_H_INCLUDED

#include "myheaders.h"
#include "main_headers.h"

/*La structure des règles créées*/
typedef struct personal_rule
{
    int left; //Etat de la cellule gauche
    int middle;//Etat de la cellule traitée
    int right; //Etat de la cellule droite
    int result; //Nouvel état de la cellule
    struct personal_rule *svt;
}personal_rule;

/* Cette fonction permet de creer une cellule de type
    personal_rule de l'initialiser et d ela retourner.
    Elle reçoit les informations des differents champs de
    la structure **/
personal_rule *create_cell(int left, int middle, int right, int result)
{
    personal_rule *ne=(personal_rule*)malloc(sizeof(personal_rule));
    if(!ne)
    {
        printf("erreur d'allocation !!");
        exit(EXIT_FAILURE);
    }
    ne->left=left;
    ne->middle=middle;
    ne->right=right;
    ne->result=result;
    ne->svt=NULL;
    return ((personal_rule*)ne);
}

/** La fonction qui ajoute une cellule de règle dans la liste **/
personal_rule * add_personnal_rule(personal_rule *malist,int left, int middle, int right, int result)
{
    personal_rule *ne=create_cell(left,middle,right,result);
    if(!malist)return ((personal_rule*)ne);
    ne->svt=malist;
    malist=ne;
    return ((personal_rule*)ne);
}

//La structure de la fenetre
typedef struct
{
    WindowObject *w; //Fenetre
    BoxObject *rd_box; //Le box global
    TableObject *rd_table; //tableau
    ButtonObject *re_leave; //Le bouton quitter
    ButtonObject *re_okay;  //Le bouton de confirmation
    LabelObject *help;
    ComboBoxobject *combo[4]; //Un tableau de combobox
    ImageObject *img_plus1;
    ImageObject *img_plus2;
    ImageObject *img_equal;
    personal_rule *maliste;
    ComboBoxobject *vos_regles; //La liste des règles
                                //que l'utilisateur crée
}dr_win;


//La fonctions d'initialisation de la fenetre
dr_win *init_dr_win()
{
    //Allocation de la mémoire
    dr_win *DRWIN=(dr_win*)malloc(sizeof(dr_win));
    if(!DRWIN)
    {
        printf("erreur d'allocation xx !!");
        exit(EXIT_FAILURE);
    }

    DRWIN->maliste=NULL;

    //Initialisation et creation de la fenetre
    DRWIN->w=init_window("regles a definir",400,400,2,0,0,1);
    DRWIN->w=create_window(DRWIN->w);

    DRWIN->rd_box=init_box(DRWIN->w->window,"rdbox",'v',1,0);
    DRWIN->rd_box=create_box(DRWIN->rd_box);
    add_to_window(DRWIN->w,DRWIN->rd_box->box);

    DRWIN->rd_table=init_table(DRWIN->rd_box->box,6,9,1);
    DRWIN->rd_table=create_table(DRWIN->rd_table);
    add_to_box_start(DRWIN->rd_box,DRWIN->rd_table->table,1,1,0);

    DRWIN->re_leave=init_button(DRWIN->rd_table->table,"Retour","Retour","images/return.png");
    DRWIN->re_leave=create_button(DRWIN->re_leave);
    add_to_table(DRWIN->rd_table,DRWIN->re_leave->button,1,3,5,6);

    DRWIN->re_okay=init_button(DRWIN->rd_table->table,"Confirmer","Confirmer","images/conf.png");
    DRWIN->re_okay=create_button(DRWIN->re_okay);
    add_to_table(DRWIN->rd_table,DRWIN->re_okay->button,6,8,5,6);

    DRWIN->combo[0]=init_combobox(DRWIN->rd_table->table,"left",0,0);
    DRWIN->combo[0]=create_combobox(DRWIN->combo[0]);
    combobox_add(DRWIN->combo[0],"blanc");
    combobox_add(DRWIN->combo[0],"couleur1");
    combobox_add(DRWIN->combo[0],"couleur2");
    combobox_add(DRWIN->combo[0],"couleur3");
    combobox_add(DRWIN->combo[0],"couleur4");
    add_to_table(DRWIN->rd_table,DRWIN->combo[0]->comboBox,1,2,1,2);

    DRWIN->img_plus1=init_image("images/plus.png","plus",0,0);
    DRWIN->img_plus1=create_image(DRWIN->img_plus1);
    add_to_table(DRWIN->rd_table,DRWIN->img_plus1->image,2,3,1,2);

    DRWIN->combo[1] =init_combobox(DRWIN->rd_table->table,"middle",0,0);
    DRWIN->combo[1] =create_combobox(DRWIN->combo[1]);
    combobox_add(DRWIN->combo[1],"blanc");
    combobox_add(DRWIN->combo[1] ,"couleur1");
    combobox_add(DRWIN->combo[1] ,"couleur2");
    combobox_add(DRWIN->combo[1] ,"couleur3");
    combobox_add(DRWIN->combo[1] ,"couleur4");
    add_to_table(DRWIN->rd_table,DRWIN->combo[1]->comboBox,3,4,1,2);

    DRWIN->img_plus2=init_image("images/plus.png","plus2",0,0);
    DRWIN->img_plus2=create_image(DRWIN->img_plus2);
    add_to_table(DRWIN->rd_table,DRWIN->img_plus2->image,4,5,1,2);

    DRWIN->combo[2] =init_combobox(DRWIN->rd_table->table,"right",0,0);
    DRWIN->combo[2] =create_combobox(DRWIN->combo[2]);
    combobox_add(DRWIN->combo[2],"blanc");
    combobox_add(DRWIN->combo[2] ,"couleur1");
    combobox_add(DRWIN->combo[2] ,"couleur2");
    combobox_add(DRWIN->combo[2] ,"couleur3");
    combobox_add(DRWIN->combo[2] ,"couleur4");
    add_to_table(DRWIN->rd_table,DRWIN->combo[2]->comboBox,5,6,1,2);

    DRWIN->img_equal=init_image("images/equal.png","equal",0,0);
    DRWIN->img_equal=create_image(DRWIN->img_equal);
    add_to_table(DRWIN->rd_table,DRWIN->img_equal->image,6,7,1,2);

    DRWIN->combo[3] =init_combobox(DRWIN->rd_table->table,"result",0,0);
    DRWIN->combo[3] =create_combobox(DRWIN->combo[3]);
    combobox_add(DRWIN->combo[3],"blanc");
    combobox_add(DRWIN->combo[3] ,"couleur1");
    combobox_add(DRWIN->combo[3] ,"couleur2");
    combobox_add(DRWIN->combo[3] ,"couleur3");
    combobox_add(DRWIN->combo[3] ,"couleur4");
    add_to_table(DRWIN->rd_table,DRWIN->combo[3]->comboBox,7,8,1,2);

    DRWIN->vos_regles=init_combobox(DRWIN->rd_table->table,"vosregles",0,0);
    DRWIN->vos_regles=create_combobox(DRWIN->vos_regles);
    add_to_table(DRWIN->rd_table,DRWIN->vos_regles->comboBox,2,7,3,4);

    DRWIN->help=init_label(DRWIN->rd_table,"Couleur(i)=couleur choisi dans le bouton(i)","label",0,0);
    DRWIN->help=create_label(DRWIN->help);
    add_to_table(DRWIN->rd_table,DRWIN->help->label,2,7,0,1);

    return ((dr_win*)DRWIN);
}

/** Cette fonctionde rapple est utilisée lorsqu'on clique sur
    le bouton d'ajout d'une nouvelle règle .
    Elle est donc stocké dans la liste ainsi dans le 5em
    combobox pour rappeler l'utilisateur des règles qu'il a créé**/
void on_agree_drwin(GtkWidget *w, gpointer data)
{
    dr_win *DRWIN=(dr_win*)data;
    gchar *choice;
    gchar *lab=(gchar*)malloc(30*sizeof(gchar));
    gchar *lab2=(gchar*)malloc(30*sizeof(gchar));
    gchar *lab3=(gchar*)malloc(30*sizeof(gchar));
    gchar *lab4=(gchar*)malloc(30*sizeof(gchar));
    int combination[4];
    for(int i=0;i<4;i++)combination[i]=0;

    for(int i=0;i<4;i++)
    {
         choice=combobox_get(DRWIN->combo[i]);
         switch(i)
         {
             case 0:strcpy(lab,choice); break;
             case 1:strcpy(lab2,choice); break;
             case 2:strcpy(lab3,choice); break;
             case 3:strcpy(lab4,choice); break;
         }
         if(strcmp(choice,"blanc")==0)combination[i]=0;
         if(strcmp(choice,"couleur1")==0)combination[i]=1;
         if(strcmp(choice,"couleur2")==0)combination[i]=2;
         if(strcmp(choice,"couleur3")==0)combination[i]=3;
         if(strcmp(choice,"couleur4")==0)combination[i]=4;
    }

         gchar *lab_glob=(gchar*)malloc(50*sizeof(gchar));
         strcpy(lab_glob,lab);
         strcat(lab_glob," + ");
         strcat(lab_glob,lab2);
         strcat(lab_glob," + ");
         strcat(lab_glob,lab3);
         strcat(lab_glob," = ");
         strcat(lab_glob,lab4);
         combobox_add(DRWIN->vos_regles,lab_glob);

         DRWIN->maliste=add_personnal_rule(DRWIN->maliste,combination[0],combination[1],combination[2],combination[3]);

}


void event_calls_drwin(dr_win *drwin)
{
    g_signal_connect(G_OBJECT(drwin->w->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(drwin->re_okay->button),"clicked",G_CALLBACK(on_agree_drwin),drwin);
}

#endif // UR_OWN_RULES_H_INCLUDED
