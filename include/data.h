#ifndef Client_H
#define Client_H
#include <gtk/gtk.h>
typedef struct client {
    int Id_Client;
    char nom[50];
    char prenom[50];
    char profession[50];
    char num_tel[15];


}client;
typedef struct Compte {
    int Id_Client;
    char nom[50];
    char prenom[50];
    float Solde_de_base;
    int Id_Compte;
    char date_ouverture[11];
    float maximal_retrait_fait;
    float maximal_virement_fait;

}Compte;
extern int nb;
extern int nb_comptes;
extern GtkWidget *POPUP_FERMETURE_COMPTE_GLOBAL;
extern GtkWidget *POPUP_3_GLOBAL;
extern GtkWidget *POPUP_GLOBAL;
extern GtkWidget *POPUP_2_GLOBAL;
extern GtkWidget *POPUP_COMPTE_GLOBAL;
extern GtkWidget *POPUP_OPERATION_RETRAIT_GLOBAL;
extern GtkWidget *POPUP_OPERATION_VIREMENT_GLOBAL;
extern GtkWidget *POPUP_SEARCH_CLIENT_GLOBAL;
extern GtkWidget *POPUP_SEARCH_CLIENT_CHOICE_GLOBAL;
extern GtkStringList *client_string_list;
extern GtkStringList *comptes_string_list;
extern GtkStringList *consultation_string_list;
extern GtkStringList *client_search_string_list;
extern client Clients[100];
extern Compte Comptes[100];
extern GtkWidget *GLOBAL_VISIBILTY_CONSULTATION_ACUN;
extern GtkWidget *GLOBAL_VISIBILTY_CONSULTATION;
extern GtkWidget *GLOBAL_CONSULTATION_ENTRIES;
extern GtkWidget *POPUP_CONSULTATION_GLOBAL;
extern GtkWidget *GLOBAL_VISIBILiTY_CONSULTATION_ENTRIES;


typedef struct {
    GtkWidget *window;
    GtkWidget *id_entry;
    GtkWidget *info_label;
    GtkWidget *tel_entry;
    GtkWidget *profession_entry;
    GtkWidget *nom_entry;
    GtkWidget *prenom_entry;
    GtkWidget *M_id_entry;
    GtkWidget *M_info_label;
    GtkWidget *M_tel_entry;
    GtkWidget *M_profession_entry;
    GtkWidget *M_nom_entry;
    GtkWidget *M_prenom_entry;
    GtkWidget *D_id_entry;
    GtkWidget *D_info_label;
    GtkWidget *id_compte_entry;
    GtkWidget *info_label_compte;
    GtkWidget *id_client_entry;
    GtkWidget *nom_entry_compte;
    GtkWidget *prenom_entry_compte;
    GtkWidget *solde_entry;
    GtkWidget *date_entry;
    GtkWidget *fermeture_id_compte_entry;
    GtkWidget *fermeture_id_client_entry;
    GtkWidget *info_label_fermeture;
    GtkWidget *retrait_entry;
    GtkWidget *virement_entry;
    GtkWidget *info_label_retrait;
    GtkWidget *info_label_virement;
    GtkWidget *id_compte_retrait;
    GtkWidget *id_compte_virement;
    GtkWidget *id_client_retrait;
    GtkWidget *id_client_virement;
    GtkWidget *id_virement_compte_TO;
    GtkWidget *id_virement_client_TO;
    GtkWidget *consultation_list;
    GtkWidget *OK_BUTTON_CONSULTATION;
    GtkWidget *CANCEL_BUTTON_CONSULTATION;
    GtkWidget *id_client_consultation;
    GtkWidget *id_compte_consultation;
    GtkWidget *Info_label_consultation;
    GtkWidget *bottom_cancel_consultation;
    GtkWidget *search_client_list;
    GtkWidget *search_client_id_entry;
    GtkWidget *search_client_info_label;
    GtkWidget *search_client_nom_entry;
    GtkWidget *search_client_prenom_entry;
    GtkWidget *search_client_id_container;
    GtkWidget *search_client_name_container;
    GtkWidget *search_client_id_buttons;
    GtkWidget *search_client_name_buttons;
    int confirm_add_client;
    int confirm_edit_client;
    int confirm_delete_client;
    int confirm_add_compte;
    int confirm_fermeture_compte;
    int confirm_retrait;
    int confirm_virement;
    int confirm_consultation;
    int confirm_search_client;


} ClientData;




GtkWidget* create_comptes_list_view(void);
void list_item_setup(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data);
void list_item_bind(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data);
GtkWidget* create_clients_list_view(void);
gboolean is_lettre(const char *str);
gboolean is_digits(const char *str);
gboolean is_float_digits(const char *str);
gboolean clear_info_label(gpointer user_data);
void switch_page(GtkButton *button, gpointer user_data);
gboolean is_valid_date(const char *str);
void on_ok_add_client(GtkButton *button, gpointer user_data);
void on_ok_edit_client(GtkButton *button, gpointer user_data);
void POP_UP_VISIBILITY(GtkButton *button, gpointer user_data);
void on_ok_delete_client(GtkButton *button, gpointer user_data);
void on_ok_add_compte(GtkButton *button, gpointer user_data);
void on_fermeture_compte_button(GtkButton *button, gpointer user_data);
void on_ok_retrait_compte(GtkButton *button, gpointer user_data);
void on_ok_virement_compte(GtkButton *button, gpointer user_data);
GtkWidget* create_consultation_list_view(void);
void on_ok_consultation(GtkButton *button, gpointer user_data);
void on_bottom_cancel_consultation(GtkButton *button, gpointer user_data);
void on_cancel_consultation(GtkButton *button, gpointer user_data);
GtkWidget* create_search_client_list_view(void);
void on_ok_search_client(GtkButton *button, gpointer user_data);
void on_ok_search_client_by_name(GtkButton *button, gpointer user_data);
void on_cancel_search_client(GtkButton *button, gpointer user_data);
void on_open_search_client_by_id(GtkButton *button, gpointer user_data);
void on_open_search_client_by_name(GtkButton *button, gpointer user_data);
void on_cancel_add_client_confirm(GtkButton *button, gpointer user_data);
void on_cancel_edit_client_confirm(GtkButton *button, gpointer user_data);
void on_cancel_delete_client_confirm(GtkButton *button, gpointer user_data);
void on_cancel_add_compte_confirm(GtkButton *button, gpointer user_data);
void on_cancel_fermeture_compte_confirm(GtkButton *button, gpointer user_data);
void on_cancel_retrait_confirm(GtkButton *button, gpointer user_data);
void on_cancel_virement_confirm(GtkButton *button, gpointer user_data);
#endif

