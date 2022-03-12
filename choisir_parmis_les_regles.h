#ifndef CHOISIR_PARMIS_LES_REGLES_H_INCLUDED
#define CHOISIR_PARMIS_LES_REGLES_H_INCLUDED


#include "myheaders.h"
#include "main_headers.h"
#include "help_rp.h"


/** La structure de la fenetre de choix parmi
    de choix et les informations récupérées **/
typedef struct
{
  WindowObject *w;   //La fenetre de reglage
  BoxObject *rp_box; //le box global
  TableObject *rp_table; //Une table
  ImageObject *r_img[74]; //Les images des règles
  checkbuttonobject *r_check[74]; //Les checkboutons des règles
  int verifier[74]; //Un tableau pour stocker les règles choisies
  ButtonObject *okay_btn; //Le bouton okey
  ButtonObject *leave_btn; //Le bouton quiter
  help_rp *help;
  ButtonObject *help_button;
  ButtonObject *all_button; //Pour choisir toutes les règles à la fois
} rp_win;  // la structure de la fenetre des choix de regles predefinis

/**Une fonction pour initialiser et créer la fenetre
   de coix parmi les regles**/
 rp_win *init_rp_win()
{
    //Allocation de la memoire
    rp_win *RPWIN=(rp_win*)malloc(sizeof(rp_win));
    if(!RPWIN)
    {
        printf("erreur d'allocation !!");
        exit(EXIT_FAILURE);
    }

    //Initialiser le tableau de choix
    for(int i=0; i<74; i++) RPWIN->verifier[i]=0;

    RPWIN->w=init_window("regles predefinies",300,300,2,0,0,1);
    RPWIN->w=create_window(RPWIN->w);

    RPWIN->rp_box=init_box(NULL,"rpbox",'v',1,0);
    RPWIN->rp_box=create_box(RPWIN->rp_box);

    help_rp *myhelp=init_help_win();
    event_calls_help_rp(myhelp);
    RPWIN->help=myhelp;

    GtkWidget *scrollbare;
    scrollbare=gtk_scrolled_window_new(NULL,NULL);
    add_to_window(RPWIN->w,scrollbare);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbare),RPWIN->rp_box->box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbare),GTK_POLICY_NEVER,GTK_POLICY_ALWAYS);

    RPWIN->rp_table=init_table(RPWIN->rp_box->box,103,10,1);
    RPWIN->rp_table=create_table(RPWIN->rp_table);
    add_to_box_start(RPWIN->rp_box,RPWIN->rp_table->table,1,1,0);

    RPWIN->help_button=init_button(RPWIN->rp_table->table,"HELP","HELP",NULL);
    RPWIN->help_button=create_button(RPWIN->help_button);
    add_to_table(RPWIN->rp_table,RPWIN->help_button->button,3,6,0,1);

       /** L'enesmeble des images de rèles**/
    RPWIN->r_img[0]=init_image("images/rule1.JPG","prule1",0,0);
    RPWIN->r_img[0]=create_image(RPWIN->r_img[0]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[0]->image,1,3,1,3);

    RPWIN->r_img[1] =init_image("images/rule2.png","prule2",0,0);
    RPWIN->r_img[1] =create_image(RPWIN->r_img[1]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[1]->image,3,6,1,3);

    RPWIN->r_img[2] =init_image("images/rule3.png","prule3",0,0);
    RPWIN->r_img[2] =create_image(RPWIN->r_img[2]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[2]->image,6,9,1,3);

    RPWIN->r_img[3] =init_image("images/rule4.png","prule4",0,0);
    RPWIN->r_img[3] =create_image(RPWIN->r_img[3]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[3]->image,1,3,4,7);

    RPWIN->r_img[4] =init_image("images/rule5.png","prule5",0,0);
    RPWIN->r_img[4] =create_image(RPWIN->r_img[4]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[4]->image,3,6,4,7);

    RPWIN->r_img[5]=init_image("images/rule6.png","prule6",0,0);
    RPWIN->r_img[5]=create_image(RPWIN->r_img[5]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[5]->image,6,9,4,7);

    RPWIN->r_img[6]=init_image("images/rule7.png","prule67",0,0);
    RPWIN->r_img[6]=create_image(RPWIN->r_img[6]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[6]->image,1,3,8,11);

    RPWIN->r_img[7]=init_image("images/rule8.png","prule8",0,0);
    RPWIN->r_img[7]=create_image(RPWIN->r_img[7]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[7]->image,3,6,8,11);

    RPWIN->r_img[8]=init_image("images/rule9.png","prule9",0,0);
    RPWIN->r_img[8]=create_image(RPWIN->r_img[8]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[8]->image,6,9,8,11);

    RPWIN->r_img[9]=init_image("images/rule10.png","prule10",0,0);
    RPWIN->r_img[9]=create_image(RPWIN->r_img[9]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[9]->image,1,3,12,15);

    RPWIN->r_img[10]=init_image("images/rule11.png","prule11",0,0);
    RPWIN->r_img[10]=create_image(RPWIN->r_img[10]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[10]->image,3,6,12,15);

    RPWIN->r_img[11]=init_image("images/rule12.png","prule12",0,0);
    RPWIN->r_img[11]=create_image(RPWIN->r_img[11]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[11]->image,6,9,12,15);

    RPWIN->r_img[12]=init_image("images/rule13.png","prule13",0,0);
    RPWIN->r_img[12]=create_image(RPWIN->r_img[12]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[12]->image,1,3,16,19);

    RPWIN->r_img[13]=init_image("images/rule14.png","prule14",0,0);
    RPWIN->r_img[13]=create_image(RPWIN->r_img[13]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[13]->image,3,6,16,19);

    RPWIN->r_img[14]=init_image("images/rule15.png","prule15",0,0);
    RPWIN->r_img[14]=create_image(RPWIN->r_img[14]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[14]->image,6,9,16,19);

    RPWIN->r_img[15]=init_image("images/rule16.png","prule16",0,0);
    RPWIN->r_img[15]=create_image(RPWIN->r_img[15]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[15]->image,1,3,20,23);

    RPWIN->r_img[16]=init_image("images/rule17.png","prule17",0,0);
    RPWIN->r_img[16]=create_image(RPWIN->r_img[16]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[16]->image,3,6,20,23);

    RPWIN->r_img[17]=init_image("images/rule18.png","prule18",0,0);
    RPWIN->r_img[17]=create_image(RPWIN->r_img[17]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[17]->image,6,9,20,23);

    RPWIN->r_img[18]=init_image("images/rule19.png","prule19",0,0);
    RPWIN->r_img[18]=create_image(RPWIN->r_img[18]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[18]->image,1,3,24,27);

    RPWIN->r_img[19]=init_image("images/rule20.png","prule20",0,0);
    RPWIN->r_img[19]=create_image(RPWIN->r_img[19]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[19]->image,3,6,24,27);

    RPWIN->r_img[20]=init_image("images/rule21.png","prule21",0,0);
    RPWIN->r_img[20]=create_image(RPWIN->r_img[20]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[20]->image,6,9,24,27);

    RPWIN->r_img[21]=init_image("images/rule22.png","prule22",0,0);
    RPWIN->r_img[21]=create_image(RPWIN->r_img[21]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[21]->image,1,3,28,31);

    RPWIN->r_img[22]=init_image("images/rule23.png","prule23",0,0);
    RPWIN->r_img[22]=create_image(RPWIN->r_img[22]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[22]->image,3,6,28,31);

    RPWIN->r_img[23]=init_image("images/rule24.png","prule24",0,0);
    RPWIN->r_img[23]=create_image(RPWIN->r_img[23]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[23]->image,6,9,28,31);

    RPWIN->r_img[24]=init_image("images/rule25.png","prule25",0,0);
    RPWIN->r_img[24]=create_image(RPWIN->r_img[24]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[24]->image,1,3,32,35);

    RPWIN->r_img[25]=init_image("images/rule26.png","prule26",0,0);
    RPWIN->r_img[25]=create_image(RPWIN->r_img[25]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[25]->image,3,6,32,35);

    RPWIN->r_img[26]=init_image("images/rule27.png","prule27",0,0);
    RPWIN->r_img[26]=create_image(RPWIN->r_img[26]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[26]->image,6,9,32,35);

    RPWIN->r_img[27]=init_image("images/rule28.png","prule28",0,0);
    RPWIN->r_img[27]=create_image(RPWIN->r_img[27]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[27]->image,1,3,36,39);

    RPWIN->r_img[28]=init_image("images/rule29.png","prule29",0,0);
    RPWIN->r_img[28]=create_image(RPWIN->r_img[28]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[28]->image,3,6,36,39);

    RPWIN->r_img[29]=init_image("images/rule30.png","prule30",0,0);
    RPWIN->r_img[29]=create_image(RPWIN->r_img[29]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[29]->image,6,9,36,39);

    RPWIN->r_img[30]=init_image("images/rule31.png","prule31",0,0);
    RPWIN->r_img[30]=create_image(RPWIN->r_img[30]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[30]->image,1,3,40,43);

    RPWIN->r_img[31]=init_image("images/rule32.png","prule32",0,0);
    RPWIN->r_img[31]=create_image(RPWIN->r_img[31]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[31]->image,3,6,40,43);

    RPWIN->r_img[32]=init_image("images/rule33.png","prule33",0,0);
    RPWIN->r_img[32]=create_image(RPWIN->r_img[32]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[32]->image,6,9,40,43);

    RPWIN->r_img[33]=init_image("images/rule34.png","prule34",0,0);
    RPWIN->r_img[33]=create_image(RPWIN->r_img[33]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[33]->image,1,3,44,47);

    RPWIN->r_img[34]=init_image("images/rule35.png","prule35",0,0);
    RPWIN->r_img[34]=create_image(RPWIN->r_img[34]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[34]->image,3,6,44,47);

    RPWIN->r_img[35]=init_image("images/rule36.png","prule36",0,0);
    RPWIN->r_img[35]=create_image(RPWIN->r_img[35]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[35]->image,6,9,44,47);

    RPWIN->r_img[36]=init_image("images/rule37.png","prule37",0,0);
    RPWIN->r_img[36]=create_image(RPWIN->r_img[36]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[36]->image,1,3,48,51);

    RPWIN->r_img[37]=init_image("images/rule38.png","prule38",0,0);
    RPWIN->r_img[37]=create_image(RPWIN->r_img[37]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[37]->image,3,6,48,51);

    RPWIN->r_img[38]=init_image("images/rule39.png","prule39",0,0);
    RPWIN->r_img[38]=create_image(RPWIN->r_img[38]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[38]->image,6,9,48,51);

    RPWIN->r_img[39]=init_image("images/rule40.png","prule40",0,0);
    RPWIN->r_img[39]=create_image(RPWIN->r_img[39]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[39]->image,1,3,52,55);

    RPWIN->r_img[40]=init_image("images/rule41.png","prule41",0,0);
    RPWIN->r_img[40]=create_image(RPWIN->r_img[40]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[40]->image,3,6,52,55);

    RPWIN->r_img[41]=init_image("images/rule42.png","prule42",0,0);
    RPWIN->r_img[41]=create_image(RPWIN->r_img[41]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[41]->image,6,9,52,55);

    RPWIN->r_img[42]=init_image("images/rule43.png","prule43",0,0);
    RPWIN->r_img[42]=create_image(RPWIN->r_img[42]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[42]->image,1,3,56,59);

    RPWIN->r_img[43]=init_image("images/rule44.png","prule44",0,0);
    RPWIN->r_img[43]=create_image(RPWIN->r_img[43]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[43]->image,3,6,56,59);

    RPWIN->r_img[44]=init_image("images/rule45.png","prule45",0,0);
    RPWIN->r_img[44]=create_image(RPWIN->r_img[44]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[44]->image,6,9,56,59);

    RPWIN->r_img[45]=init_image("images/rule46.png","prule46",0,0);
    RPWIN->r_img[45]=create_image(RPWIN->r_img[45]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[45]->image,1,3,60,63);

    RPWIN->r_img[46]=init_image("images/rule47.png","prule47",0,0);
    RPWIN->r_img[46]=create_image(RPWIN->r_img[46]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[46]->image,3,6,60,63);

    RPWIN->r_img[47]=init_image("images/rule48.png","prule48",0,0);
    RPWIN->r_img[47]=create_image(RPWIN->r_img[47]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[47]->image,6,9,60,63);

    RPWIN->r_img[48]=init_image("images/rule49.png","prule49",0,0);
    RPWIN->r_img[48]=create_image(RPWIN->r_img[48]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[48]->image,1,3,64,67);

    RPWIN->r_img[49]=init_image("images/rule50.png","prule50",0,0);
    RPWIN->r_img[49]=create_image(RPWIN->r_img[49]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[49]->image,3,6,64,67);

    RPWIN->r_img[50]=init_image("images/rule51.png","prule51",0,0);
    RPWIN->r_img[50]=create_image(RPWIN->r_img[50]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[50]->image,6,9,64,67);

    RPWIN->r_img[51]=init_image("images/rule52.png","prule52",0,0);
    RPWIN->r_img[51]=create_image(RPWIN->r_img[51]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[51]->image,1,3,68,71);

    RPWIN->r_img[52]=init_image("images/rule53.png","prule53",0,0);
    RPWIN->r_img[52]=create_image(RPWIN->r_img[52]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[52]->image,3,6,68,71);

    RPWIN->r_img[53]=init_image("images/rule54.png","prule54",0,0);
    RPWIN->r_img[53]=create_image(RPWIN->r_img[53]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[53]->image,6,9,68,71);

    RPWIN->r_img[54]=init_image("images/rule55.png","prule55",0,0);
    RPWIN->r_img[54]=create_image(RPWIN->r_img[54]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[54]->image,1,3,72,75);

    RPWIN->r_img[55]=init_image("images/rule56.png","prule56",0,0);
    RPWIN->r_img[55]=create_image(RPWIN->r_img[55]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[55]->image,3,6,72,75);

    RPWIN->r_img[56]=init_image("images/rule57.png","prule57",0,0);
    RPWIN->r_img[56]=create_image(RPWIN->r_img[56]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[56]->image,6,9,72,75);

    RPWIN->r_img[57]=init_image("images/rule58.png","prule58",0,0);
    RPWIN->r_img[57]=create_image(RPWIN->r_img[57]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[57]->image,1,3,76,79);

    RPWIN->r_img[58]=init_image("images/rule59.png","prule59",0,0);
    RPWIN->r_img[58]=create_image(RPWIN->r_img[58]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[58]->image,3,6,76,79);

    RPWIN->r_img[59]=init_image("images/rule60.png","prule60",0,0);
    RPWIN->r_img[59]=create_image(RPWIN->r_img[59]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[59]->image,6,9,76,79);

    RPWIN->r_img[60]=init_image("images/rule61.png","prule61",0,0);
    RPWIN->r_img[60]=create_image(RPWIN->r_img[60]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[60]->image,1,3,80,83);

    RPWIN->r_img[61]=init_image("images/rule62.png","prule62",0,0);
    RPWIN->r_img[61]=create_image(RPWIN->r_img[61]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[61]->image,3,6,80,83);

    RPWIN->r_img[62]=init_image("images/rule63.png","prule63",0,0);
    RPWIN->r_img[62]=create_image(RPWIN->r_img[62]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[62]->image,6,9,80,83);

    RPWIN->r_img[63]=init_image("images/rule64.png","prule64",0,0);
    RPWIN->r_img[63]=create_image(RPWIN->r_img[63]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[63]->image,1,3,84,87);

    RPWIN->r_img[64]=init_image("images/rule65.png","prule65",0,0);
    RPWIN->r_img[64]=create_image(RPWIN->r_img[64]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[64]->image,3,6,84,87);

    RPWIN->r_img[65]=init_image("images/rule66.png","prule66",0,0);
    RPWIN->r_img[65]=create_image(RPWIN->r_img[65]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[65]->image,6,9,84,87);

    RPWIN->r_img[66]=init_image("images/rule67.png","prule67",0,0);
    RPWIN->r_img[66]=create_image(RPWIN->r_img[66]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[66]->image,1,3,88,91);

    RPWIN->r_img[67]=init_image("images/rule68.png","prule68",0,0);
    RPWIN->r_img[67]=create_image(RPWIN->r_img[67]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[67]->image,3,6,88,91);

    RPWIN->r_img[68]=init_image("images/rule69.png","prule69",0,0);
    RPWIN->r_img[68]=create_image(RPWIN->r_img[68]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[68]->image,6,9,88,91);

    RPWIN->r_img[69]=init_image("images/rule70.png","prule70",0,0);
    RPWIN->r_img[69]=create_image(RPWIN->r_img[69]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[69]->image,1,3,92,95);

    RPWIN->r_img[70]=init_image("images/rule71.png","prule71",0,0);
    RPWIN->r_img[70]=create_image(RPWIN->r_img[70]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[70]->image,3,6,92,95);

    RPWIN->r_img[71]=init_image("images/rule72.png","prule72",0,0);
    RPWIN->r_img[71]=create_image(RPWIN->r_img[71]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[71]->image,6,9,92,95);

     RPWIN->r_img[72]=init_image("images/rule73.png","prule73",0,0);
    RPWIN->r_img[72]=create_image(RPWIN->r_img[72]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[72]->image,1,3,96,99);

    RPWIN->r_img[73]=init_image("images/rule74.png","prule74",0,0);
    RPWIN->r_img[73]=create_image(RPWIN->r_img[73]);
    add_to_table(RPWIN->rp_table,RPWIN->r_img[73]->image,3,6,96,99);

                 /** L'ensembledes boutons de la fenetre **/
    RPWIN->leave_btn=init_button(RPWIN->rp_table->table,"sortir","sortir",NULL);
    RPWIN->leave_btn=create_button(RPWIN->leave_btn);
    add_to_table(RPWIN->rp_table,RPWIN->leave_btn->button,2,3,101,102);

    RPWIN->all_button=init_button(RPWIN->rp_table->table,"all","all",NULL);
    RPWIN->all_button=create_button(RPWIN->all_button);
    add_to_table(RPWIN->rp_table,RPWIN->all_button->button,4,5,101,102);

    RPWIN->okay_btn=init_button(RPWIN->rp_table->table,"Conf","Conf",NULL);
    RPWIN->okay_btn=create_button(RPWIN->okay_btn);
    add_to_table(RPWIN->rp_table,RPWIN->okay_btn->button,6,7,101,102);

                  /**Les checkboutons des règles**/
    RPWIN->r_check[0] =init_checkbutton(RPWIN->rp_table->table,"rule 1","rule 1");
    RPWIN->r_check[0] =create_checkbutton(RPWIN->r_check[0]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[0]->checkbutton,1,3,3,4);

    RPWIN->r_check[1] =init_checkbutton(RPWIN->rp_table->table,"rule 2","rule 2");
    RPWIN->r_check[1] =create_checkbutton(RPWIN->r_check[1]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[1]->checkbutton,4,6,3,4);

    RPWIN->r_check[2] =init_checkbutton(RPWIN->rp_table->table,"rule 3","rule 3");
    RPWIN->r_check[2] =create_checkbutton(RPWIN->r_check[2]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[2]->checkbutton,7,10,3,4);

    RPWIN->r_check[3] =init_checkbutton(RPWIN->rp_table->table,"rule 4","rule 4");
    RPWIN->r_check[3] =create_checkbutton(RPWIN->r_check[3]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[3]->checkbutton,1,3,7,8);

    RPWIN->r_check[4] =init_checkbutton(RPWIN->rp_table->table,"rule 5","rule 5");
    RPWIN->r_check[4] =create_checkbutton(RPWIN->r_check[4]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[4]->checkbutton,4,6,7,8);

    RPWIN->r_check[5] =init_checkbutton(RPWIN->rp_table->table,"rule 6","rule 6");
    RPWIN->r_check[5] =create_checkbutton(RPWIN->r_check[5]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[5]->checkbutton,7,10,7,8);

    RPWIN->r_check[6]=init_checkbutton(RPWIN->rp_table->table,"rule 7","rule 7");
    RPWIN->r_check[6]=create_checkbutton(RPWIN->r_check[6]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[6]->checkbutton,1,3,11,12);

    RPWIN->r_check[7]=init_checkbutton(RPWIN->rp_table->table,"rule 8","rule 8");
    RPWIN->r_check[7]=create_checkbutton(RPWIN->r_check[7]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[7]->checkbutton,4,6,11,12);

    RPWIN->r_check[8]=init_checkbutton(RPWIN->rp_table->table,"rule 9","rule 9");
    RPWIN->r_check[8]=create_checkbutton(RPWIN->r_check[8]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[8]->checkbutton,7,10,11,12);

    RPWIN->r_check[9]=init_checkbutton(RPWIN->rp_table->table,"rule 10","rule 10");
    RPWIN->r_check[9]=create_checkbutton(RPWIN->r_check[9]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[9]->checkbutton,1,3,15,16);

    RPWIN->r_check[10]=init_checkbutton(RPWIN->rp_table->table,"rule 11","rule 11");
    RPWIN->r_check[10]=create_checkbutton(RPWIN->r_check[10]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[10]->checkbutton,4,6,15,16);

    RPWIN->r_check[11]=init_checkbutton(RPWIN->rp_table->table,"rule 12","rule 12");
    RPWIN->r_check[11]=create_checkbutton(RPWIN->r_check[11]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[11]->checkbutton,7,10,15,16);

    RPWIN->r_check[12]=init_checkbutton(RPWIN->rp_table->table,"rule 13","rule 13");
    RPWIN->r_check[12]=create_checkbutton(RPWIN->r_check[12]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[12]->checkbutton,1,3,19,20);

    RPWIN->r_check[13]=init_checkbutton(RPWIN->rp_table->table,"rule 14","rule 14");
    RPWIN->r_check[13]=create_checkbutton(RPWIN->r_check[13]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[13]->checkbutton,4,6,19,20);

    RPWIN->r_check[14]=init_checkbutton(RPWIN->rp_table->table,"rule 15","rule 15");
    RPWIN->r_check[14]=create_checkbutton(RPWIN->r_check[14]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[14]->checkbutton,7,10,19,20);

    RPWIN->r_check[15]=init_checkbutton(RPWIN->rp_table->table,"rule 16","rule 16");
    RPWIN->r_check[15]=create_checkbutton(RPWIN->r_check[15]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[15]->checkbutton,1,3,23,24);

    RPWIN->r_check[16]=init_checkbutton(RPWIN->rp_table->table,"rule 17","rule 17");
    RPWIN->r_check[16]=create_checkbutton(RPWIN->r_check[16]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[16]->checkbutton,4,6,23,24);

    RPWIN->r_check[17]=init_checkbutton(RPWIN->rp_table->table,"rule 18","rule 18");
    RPWIN->r_check[17]=create_checkbutton(RPWIN->r_check[17]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[17]->checkbutton,7,10,23,24);

    RPWIN->r_check[18]=init_checkbutton(RPWIN->rp_table->table,"rule 19","rule 19");
    RPWIN->r_check[18]=create_checkbutton(RPWIN->r_check[18]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[18]->checkbutton,1,3,27,28);

    RPWIN->r_check[19]=init_checkbutton(RPWIN->rp_table->table,"rule 20","rule 20");
    RPWIN->r_check[19]=create_checkbutton(RPWIN->r_check[19]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[19]->checkbutton,4,6,27,28);

    RPWIN->r_check[20]=init_checkbutton(RPWIN->rp_table->table,"rule 21","rule 21");
    RPWIN->r_check[20]=create_checkbutton(RPWIN->r_check[20]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[20]->checkbutton,7,10,27,28);

    RPWIN->r_check[21]=init_checkbutton(RPWIN->rp_table->table,"rule 22","rule 22");
    RPWIN->r_check[21]=create_checkbutton(RPWIN->r_check[21]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[21]->checkbutton,1,3,31,32);

    RPWIN->r_check[22]=init_checkbutton(RPWIN->rp_table->table,"rule 23","rule 23");
    RPWIN->r_check[22]=create_checkbutton(RPWIN->r_check[22]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[22]->checkbutton,4,6,31,32);

    RPWIN->r_check[23]=init_checkbutton(RPWIN->rp_table->table,"rule 24","rule 24");
    RPWIN->r_check[23]=create_checkbutton(RPWIN->r_check[23]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[23]->checkbutton,7,10,31,32);

    RPWIN->r_check[24]=init_checkbutton(RPWIN->rp_table->table,"rule 25","rule 25");
    RPWIN->r_check[24]=create_checkbutton(RPWIN->r_check[24]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[24]->checkbutton,1,3,35,36);

    RPWIN->r_check[25]=init_checkbutton(RPWIN->rp_table->table,"rule 26","rule 26");
    RPWIN->r_check[25]=create_checkbutton(RPWIN->r_check[25]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[25]->checkbutton,4,6,35,36);

    RPWIN->r_check[26]=init_checkbutton(RPWIN->rp_table->table,"rule 27","rule 27");
    RPWIN->r_check[26]=create_checkbutton(RPWIN->r_check[26]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[26]->checkbutton,7,10,35,36);

    RPWIN->r_check[27]=init_checkbutton(RPWIN->rp_table->table,"rule 28","rule 28");
    RPWIN->r_check[27]=create_checkbutton(RPWIN->r_check[27]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[27]->checkbutton,1,3,39,40);

    RPWIN->r_check[28]=init_checkbutton(RPWIN->rp_table->table,"rule 29","rule 29");
    RPWIN->r_check[28]=create_checkbutton(RPWIN->r_check[28]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[28]->checkbutton,4,6,39,40);

    RPWIN->r_check[29]=init_checkbutton(RPWIN->rp_table->table,"rule 30","rule 30");
    RPWIN->r_check[29]=create_checkbutton(RPWIN->r_check[29]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[29]->checkbutton,7,10,39,40);

    RPWIN->r_check[30]=init_checkbutton(RPWIN->rp_table->table,"rule 31","rule 31");
    RPWIN->r_check[30]=create_checkbutton(RPWIN->r_check[30]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[30]->checkbutton,1,3,43,44);

    RPWIN->r_check[31]=init_checkbutton(RPWIN->rp_table->table,"rule 32","rule 32");
    RPWIN->r_check[31]=create_checkbutton(RPWIN->r_check[31]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[31]->checkbutton,4,6,43,44);

    RPWIN->r_check[32]=init_checkbutton(RPWIN->rp_table->table,"rule 33","rule 33");
    RPWIN->r_check[32]=create_checkbutton(RPWIN->r_check[32]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[32]->checkbutton,7,10,43,44);

    RPWIN->r_check[33]=init_checkbutton(RPWIN->rp_table->table,"rule 34","rule 34");
    RPWIN->r_check[33]=create_checkbutton(RPWIN->r_check[33]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[33]->checkbutton,1,3,47,48);

    RPWIN->r_check[34]=init_checkbutton(RPWIN->rp_table->table,"rule 35","rule 35");
    RPWIN->r_check[34]=create_checkbutton(RPWIN->r_check[34]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[34]->checkbutton,4,6,47,48);

    RPWIN->r_check[35]=init_checkbutton(RPWIN->rp_table->table,"rule 36","rule 36");
    RPWIN->r_check[35]=create_checkbutton(RPWIN->r_check[35]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[35]->checkbutton,7,10,47,48);

    RPWIN->r_check[36]=init_checkbutton(RPWIN->rp_table->table,"rule 37","rule 37");
    RPWIN->r_check[36]=create_checkbutton(RPWIN->r_check[36]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[36]->checkbutton,1,3,51,52);

    RPWIN->r_check[37]=init_checkbutton(RPWIN->rp_table->table,"rule 38","rule 38");
    RPWIN->r_check[37]=create_checkbutton(RPWIN->r_check[37]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[37]->checkbutton,4,6,51,52);

    RPWIN->r_check[38]=init_checkbutton(RPWIN->rp_table->table,"rule 39","rule 39");
    RPWIN->r_check[38]=create_checkbutton(RPWIN->r_check[38]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[38]->checkbutton,7,10,51,52);

    RPWIN->r_check[39]=init_checkbutton(RPWIN->rp_table->table,"rule 40","rule 40");
    RPWIN->r_check[39]=create_checkbutton(RPWIN->r_check[39]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[39]->checkbutton,1,3,55,56);

    RPWIN->r_check[40]=init_checkbutton(RPWIN->rp_table->table,"rule 41","rule 41");
    RPWIN->r_check[40]=create_checkbutton(RPWIN->r_check[40]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[40]->checkbutton,4,6,55,56);

    RPWIN->r_check[41]=init_checkbutton(RPWIN->rp_table->table,"rule 42","rule 42");
    RPWIN->r_check[41]=create_checkbutton(RPWIN->r_check[41]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[41]->checkbutton,7,10,55,56);

    RPWIN->r_check[42]=init_checkbutton(RPWIN->rp_table->table,"rule 43","rule 43");
    RPWIN->r_check[42]=create_checkbutton(RPWIN->r_check[42]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[42]->checkbutton,1,3,59,60);

    RPWIN->r_check[43]=init_checkbutton(RPWIN->rp_table->table,"rule 44","rule 44");
    RPWIN->r_check[43]=create_checkbutton(RPWIN->r_check[43]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[43]->checkbutton,4,6,59,60);

    RPWIN->r_check[44]=init_checkbutton(RPWIN->rp_table->table,"rule 45","rule 45");
    RPWIN->r_check[44]=create_checkbutton(RPWIN->r_check[44]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[44]->checkbutton,7,10,59,60);

    RPWIN->r_check[45]=init_checkbutton(RPWIN->rp_table->table,"rule 46","rule 46");
    RPWIN->r_check[45]=create_checkbutton(RPWIN->r_check[45]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[45]->checkbutton,1,3,63,64);

    RPWIN->r_check[46]=init_checkbutton(RPWIN->rp_table->table,"rule 47","rule 47");
    RPWIN->r_check[46]=create_checkbutton(RPWIN->r_check[46]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[46]->checkbutton,4,6,63,64);

    RPWIN->r_check[47]=init_checkbutton(RPWIN->rp_table->table,"rule 48","rule 48");
    RPWIN->r_check[47]=create_checkbutton(RPWIN->r_check[47]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[47]->checkbutton,7,10,63,64);

    RPWIN->r_check[48]=init_checkbutton(RPWIN->rp_table->table,"rule 49","rule 49");
    RPWIN->r_check[48]=create_checkbutton(RPWIN->r_check[48]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[48]->checkbutton,1,3,67,68);

    RPWIN->r_check[49]=init_checkbutton(RPWIN->rp_table->table,"rule 50","rule 50");
    RPWIN->r_check[49]=create_checkbutton(RPWIN->r_check[49]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[49]->checkbutton,4,6,67,68);

    RPWIN->r_check[50]=init_checkbutton(RPWIN->rp_table->table,"rule 51","rule 51");
    RPWIN->r_check[50]=create_checkbutton(RPWIN->r_check[50]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[50]->checkbutton,7,10,67,68);

    RPWIN->r_check[51]=init_checkbutton(RPWIN->rp_table->table,"rule 52","rule 52");
    RPWIN->r_check[51]=create_checkbutton(RPWIN->r_check[51]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[51]->checkbutton,1,3,71,72);

    RPWIN->r_check[52]=init_checkbutton(RPWIN->rp_table->table,"rule 53","rule 53");
    RPWIN->r_check[52]=create_checkbutton(RPWIN->r_check[52]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[52]->checkbutton,4,6,71,72);

    RPWIN->r_check[53]=init_checkbutton(RPWIN->rp_table->table,"rule 54","rule 54");
    RPWIN->r_check[53]=create_checkbutton(RPWIN->r_check[53]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[53]->checkbutton,7,10,71,72);

    RPWIN->r_check[54]=init_checkbutton(RPWIN->rp_table->table,"rule 55","rule 55");
    RPWIN->r_check[54]=create_checkbutton(RPWIN->r_check[54]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[54]->checkbutton,1,3,75,76);

    RPWIN->r_check[55]=init_checkbutton(RPWIN->rp_table->table,"rule 56","rule 56");
    RPWIN->r_check[55]=create_checkbutton(RPWIN->r_check[55]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[55]->checkbutton,4,6,75,76);

    RPWIN->r_check[56]=init_checkbutton(RPWIN->rp_table->table,"rule 57","rule 57");
    RPWIN->r_check[56]=create_checkbutton(RPWIN->r_check[56]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[56]->checkbutton,7,10,75,76);

    RPWIN->r_check[57]=init_checkbutton(RPWIN->rp_table->table,"rule 58","rule 58");
    RPWIN->r_check[57]=create_checkbutton(RPWIN->r_check[57]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[57]->checkbutton,1,3,79,80);

    RPWIN->r_check[58]=init_checkbutton(RPWIN->rp_table->table,"rule 59","rule 59");
    RPWIN->r_check[58]=create_checkbutton(RPWIN->r_check[58]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[58]->checkbutton,4,6,79,80);

    RPWIN->r_check[59]=init_checkbutton(RPWIN->rp_table->table,"rule 60","rule 60");
    RPWIN->r_check[59]=create_checkbutton(RPWIN->r_check[59]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[59]->checkbutton,7,10,79,80);

    RPWIN->r_check[60]=init_checkbutton(RPWIN->rp_table->table,"rule 61","rule 61");
    RPWIN->r_check[60]=create_checkbutton(RPWIN->r_check[60]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[60]->checkbutton,1,3,83,84);

    RPWIN->r_check[61]=init_checkbutton(RPWIN->rp_table->table,"rule 62","rule 62");
    RPWIN->r_check[61]=create_checkbutton(RPWIN->r_check[61]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[61]->checkbutton,4,6,83,84);

    RPWIN->r_check[62]=init_checkbutton(RPWIN->rp_table->table,"rule 63","rule 63");
    RPWIN->r_check[62]=create_checkbutton(RPWIN->r_check[62]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[62]->checkbutton,7,10,83,84);

    RPWIN->r_check[63]=init_checkbutton(RPWIN->rp_table->table,"rule 64","rule 64");
    RPWIN->r_check[63]=create_checkbutton(RPWIN->r_check[63]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[63]->checkbutton,1,3,87,88);

    RPWIN->r_check[64]=init_checkbutton(RPWIN->rp_table->table,"rule 65","rule 65");
    RPWIN->r_check[64]=create_checkbutton(RPWIN->r_check[64]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[64]->checkbutton,4,6,87,88);

    RPWIN->r_check[65]=init_checkbutton(RPWIN->rp_table->table,"rule 66","rule 66");
    RPWIN->r_check[65]=create_checkbutton(RPWIN->r_check[65]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[65]->checkbutton,7,10,87,88);

    RPWIN->r_check[66]=init_checkbutton(RPWIN->rp_table->table,"rule 67","rule 67");
    RPWIN->r_check[66]=create_checkbutton(RPWIN->r_check[66]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[66]->checkbutton,1,3,91,92);

    RPWIN->r_check[67]=init_checkbutton(RPWIN->rp_table->table,"rule 68","rule 68");
    RPWIN->r_check[67]=create_checkbutton(RPWIN->r_check[67]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[67]->checkbutton,4,6,91,92);

    RPWIN->r_check[68]=init_checkbutton(RPWIN->rp_table->table,"rule 69","rule 69");
    RPWIN->r_check[68]=create_checkbutton(RPWIN->r_check[68]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[68]->checkbutton,7,10,91,92);

    RPWIN->r_check[69]=init_checkbutton(RPWIN->rp_table->table,"rule 70","rule 70");
    RPWIN->r_check[69]=create_checkbutton(RPWIN->r_check[69]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[69]->checkbutton,1,3,95,96);

    RPWIN->r_check[70]=init_checkbutton(RPWIN->rp_table->table,"rule 71","rule 71");
    RPWIN->r_check[70]=create_checkbutton(RPWIN->r_check[70]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[70]->checkbutton,4,6,95,96);

    RPWIN->r_check[71]=init_checkbutton(RPWIN->rp_table->table,"rule 72","rule 72");
    RPWIN->r_check[71]=create_checkbutton(RPWIN->r_check[71]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[71]->checkbutton,7,10,95,96);

    RPWIN->r_check[72]=init_checkbutton(RPWIN->rp_table->table,"rule 73","rule 73");
    RPWIN->r_check[72]=create_checkbutton(RPWIN->r_check[72]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[72]->checkbutton,1,3,99,100);

    RPWIN->r_check[73]=init_checkbutton(RPWIN->rp_table->table,"rule 74 ","rule 74 ");
    RPWIN->r_check[73]=create_checkbutton(RPWIN->r_check[73]);
    add_to_table(RPWIN->rp_table,RPWIN->r_check[73]->checkbutton,4,6,99,100);


    return ((rp_win*)RPWIN);
}


/** La fonction de rappel appelée lorsqu'on clique
    le bouton "conf" afin de récupérer l'ensemble
    des règles que l'utilisateur a choisi
**/
void on_agree_rpwin(GtkWidget *w, gpointer data)
{
    rp_win *RPWIN=(rp_win*)data;
    int i;
    gboolean etatbtn;
    for(i=0;i<74;i++)
    {
        //tester si la règle est choisie ou pas
         if(gtk_toggle_button_get_active(RPWIN->r_check[i]->checkbutton))RPWIN->verifier[i]=1;
         else
            RPWIN->verifier[i]=0;
    }

    for(i=0; i<74; i++)
    {
        if(RPWIN->verifier[i]==1)printf("vous avez choisit la regle %d \n",i+1);
    }
    printf("\n");
}

/** Une fonction de rappel appelée quand on clique
    sur le bouton de help afin d'expliquer les règles
    proposées **/
void on_help(GtkWidget *w, gpointer data)
{
    rp_win *rpwin=(rp_win*)data;

    gtk_widget_hide(rpwin->w->window);

    gtk_widget_show_all(rpwin->help->win->window);
}

void on_return_from_help(GtkWidget *w, gpointer data)
{
    rp_win *rpwin=(rp_win*)data;

    gtk_widget_hide(rpwin->help->win->window);

    gtk_widget_show_all(rpwin->w->window);
}


//La fonction qui sert à selectionner toute
//les règles proposées
void on_all(GtkWidget *w,gpointer data)
{
    rp_win *rpwin=(rp_win*)data;
    int i;
    for(i=0;i<74;i++)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rpwin->r_check[i]->checkbutton),TRUE);
    }
}

void event_calls_rpwin(rp_win *rpwin)
{
    g_signal_connect(G_OBJECT(rpwin->w->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(rpwin->okay_btn->button),"clicked",G_CALLBACK(on_agree_rpwin),rpwin);
    g_signal_connect(G_OBJECT(rpwin->help_button->button),"clicked",G_CALLBACK(on_help),rpwin);
    g_signal_connect(G_OBJECT(rpwin->all_button->button),"clicked",G_CALLBACK(on_all),rpwin);
    g_signal_connect(G_OBJECT(rpwin->help->help_leave->button),"clicked",G_CALLBACK(on_return_from_help),rpwin);
}




#endif // CHOISIR_PARMIS_LES_REGLES_H_INCLUDED
