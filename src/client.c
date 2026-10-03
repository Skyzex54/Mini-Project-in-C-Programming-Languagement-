#include "./../include/data.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkStringList *client_string_list = NULL;
GtkStringList *client_search_string_list = NULL;



GtkWidget* create_clients_list_view(void) {
    client_string_list = gtk_string_list_new(NULL);
    for (int i = 0; i < nb; i++) {
         char buffer[256];
         snprintf(buffer, sizeof(buffer), "|Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
         Clients[i].Id_Client, Clients[i].nom, Clients[i].prenom, Clients[i].num_tel, Clients[i].profession);
        gtk_string_list_append(client_string_list, buffer);
        }
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(list_item_setup), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(list_item_bind), NULL);

    GtkWidget *list_view = gtk_list_view_new(
        GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(client_string_list))),
        factory
    );
    gtk_widget_set_hexpand(list_view, TRUE);
    gtk_widget_set_vexpand(list_view, TRUE);
    return list_view;
}

GtkWidget* create_search_client_list_view(void) {
    client_search_string_list = gtk_string_list_new(NULL);
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(list_item_setup), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(list_item_bind), NULL);

    GtkWidget *list_view = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(client_search_string_list))),factory
    );
    gtk_widget_set_hexpand(list_view, TRUE);
    gtk_widget_set_vexpand(list_view, TRUE);
    return list_view;
}

void on_ok_add_client(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *id_entry = GTK_ENTRY(data->id_entry);
    GtkWidget *window = data->window;
    GtkWidget *Info_label = data->info_label;
    GtkEntry *tel_entry = GTK_ENTRY(data->tel_entry);
    GtkEntry *nom_entry = GTK_ENTRY(data->nom_entry);
    GtkEntry *prenom_entry = GTK_ENTRY(data->prenom_entry);
    GtkEntry *profession_entry = GTK_ENTRY(data->profession_entry);
    
    const char *id_text = gtk_editable_get_text(GTK_EDITABLE(id_entry));
    const char *tel_text = gtk_editable_get_text(GTK_EDITABLE(tel_entry));
    const char *nom_text = gtk_editable_get_text(GTK_EDITABLE(nom_entry));
    const char *prenom_text = gtk_editable_get_text(GTK_EDITABLE(prenom_entry));
    const char *profession_text = gtk_editable_get_text(GTK_EDITABLE(profession_entry));
    int id_s = atoi(id_text);

    // Check if ID exists
    int exist = 0;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_s) {
            exist = 1;
            break;
        }
    }

    if (exist) {
        gtk_label_set_text(GTK_LABEL(data->info_label), "This ID already exists!");
        g_timeout_add_seconds(2, clear_info_label, data->info_label);
        return;
    }
    if (strlen(tel_text) > 10 || strlen(tel_text) < 10 || !is_digits(tel_text)) {
        gtk_label_set_text(GTK_LABEL(Info_label), "Incorrect phone number format.");
        g_timeout_add_seconds(2, clear_info_label, Info_label);
        return;
    }
    if (!is_digits(id_text)) {
    gtk_label_set_text(GTK_LABEL(Info_label), "ID must be a number!");
    g_timeout_add_seconds(2, clear_info_label, Info_label);
    return;
    }
    if (!is_lettre(nom_text) || !is_lettre(prenom_text) || !is_lettre(profession_text)) {
        gtk_label_set_text(GTK_LABEL(Info_label), "Surname, first name, profession must contain only letters!");
        g_timeout_add_seconds(2, clear_info_label, Info_label);
        return;
    }
    if (!data->confirm_add_client) {
        gtk_label_set_text(GTK_LABEL(Info_label), "Confirm?");
        data->confirm_add_client = 1;
        return;
    }
    data->confirm_add_client = 0;
    // Save new client
    Clients[nb].Id_Client = id_s;
    strncpy(Clients[nb].nom, nom_text, sizeof(Clients[nb].nom)-1);
    Clients[nb].nom[sizeof(Clients[nb].nom)-1] = '\0';
    strncpy(Clients[nb].prenom, prenom_text, sizeof(Clients[nb].prenom)-1);
    Clients[nb].prenom[sizeof(Clients[nb].prenom)-1] = '\0';
    strncpy(Clients[nb].profession, profession_text, sizeof(Clients[nb].profession)-1);
    Clients[nb].profession[sizeof(Clients[nb].profession)-1] = '\0';
    strncpy(Clients[nb].num_tel, tel_text, sizeof(Clients[nb].num_tel)-1);
    Clients[nb].num_tel[sizeof(Clients[nb].num_tel)-1] = '\0';
    nb++;
    if (client_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_string_list, 0);
        }
        for (int i = 0; i < nb; i++) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
                Clients[i].Id_Client, Clients[i].nom, Clients[i].prenom, Clients[i].num_tel, Clients[i].profession);
            gtk_string_list_append(client_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(Info_label), "Client added successfully!");
    g_timeout_add_seconds(2, clear_info_label, Info_label);
}

void on_ok_search_client(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkWidget *info_label = data->search_client_info_label;
    GtkWidget *list_view = data->search_client_list;
    GtkWidget *id_entry = data->search_client_id_entry;
    const char *id_text = gtk_editable_get_text(GTK_EDITABLE(id_entry));

    if (!is_digits(id_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }


    if (!data->confirm_search_client) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_search_client = 1;
        return;
    }
    data->confirm_search_client = 0;
      int id_client = atoi(id_text);
    // searching for the id 
    int client_found = 0;
    int client_index = -1;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_client) {
            client_found = 1;
            client_index = i;
            break;
        }
    }
    if (!client_found) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client not found!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        gtk_widget_set_visible(list_view, FALSE);
        return;
    }
    gtk_label_set_text(GTK_LABEL(info_label), "");

    if (client_search_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_search_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_search_string_list, 0);
        }
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "|Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
            Clients[client_index].Id_Client, Clients[client_index].nom, Clients[client_index].prenom,
            Clients[client_index].num_tel, Clients[client_index].profession);
        gtk_string_list_append(client_search_string_list, buffer);
    }

    gtk_editable_set_text(GTK_EDITABLE(id_entry), "");
    gtk_widget_set_visible(list_view, TRUE);
}

void on_ok_search_client_by_name(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkWidget *info_label = data->search_client_info_label;
    GtkWidget *list_view = data->search_client_list;
    GtkWidget *nom_entry = data->search_client_nom_entry;
    GtkWidget *prenom_entry = data->search_client_prenom_entry;
    const char *nom_text = gtk_editable_get_text(GTK_EDITABLE(nom_entry));
    const char *prenom_text = gtk_editable_get_text(GTK_EDITABLE(prenom_entry));

    if (!is_lettre(nom_text) || !is_lettre(prenom_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Surname and first name must contain only letters!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_search_client) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_search_client = 1;
        return;
    }
    data->confirm_search_client = 0;

    if (client_search_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_search_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_search_string_list, 0);
        }
    }

    int matches = 0;
    for (int i = 0; i < nb; i++) {
        if (strcmp(Clients[i].nom, nom_text) == 0 && strcmp(Clients[i].prenom, prenom_text) == 0) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "|Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
                Clients[i].Id_Client, Clients[i].nom, Clients[i].prenom,
                Clients[i].num_tel, Clients[i].profession);
            gtk_string_list_append(client_search_string_list, buffer);
            matches++;
        }
    }

    if (matches == 0) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client not found!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        gtk_widget_set_visible(list_view, FALSE);
        return;
    }

    gtk_label_set_text(GTK_LABEL(info_label), "");
    gtk_editable_set_text(GTK_EDITABLE(nom_entry), "");
    gtk_editable_set_text(GTK_EDITABLE(prenom_entry), "");
    gtk_widget_set_visible(list_view, TRUE);
}

void on_cancel_search_client(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    if (POPUP_SEARCH_CLIENT_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_GLOBAL, FALSE);
    data->confirm_search_client = 0;
    gtk_widget_set_visible(data->search_client_list, FALSE);
    if (client_search_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_search_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_search_string_list, 0);
        }
    }
    gtk_editable_set_text(GTK_EDITABLE(data->search_client_id_entry), "");
    gtk_editable_set_text(GTK_EDITABLE(data->search_client_nom_entry), "");
    gtk_editable_set_text(GTK_EDITABLE(data->search_client_prenom_entry), "");
    gtk_label_set_text(GTK_LABEL(data->search_client_info_label), "");
}

void on_open_search_client_by_id(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    if (POPUP_SEARCH_CLIENT_CHOICE_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_CHOICE_GLOBAL, FALSE);
    if (POPUP_SEARCH_CLIENT_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_GLOBAL, TRUE);
    if (data->search_client_id_container) gtk_widget_set_visible(data->search_client_id_container, TRUE);
    if (data->search_client_id_buttons) gtk_widget_set_visible(data->search_client_id_buttons, TRUE);
    if (data->search_client_name_container) gtk_widget_set_visible(data->search_client_name_container, FALSE);
    if (data->search_client_name_buttons) gtk_widget_set_visible(data->search_client_name_buttons, FALSE);
    data->confirm_search_client = 0;
    gtk_widget_set_visible(data->search_client_list, FALSE);
    if (client_search_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_search_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_search_string_list, 0);
        }
    }
    gtk_label_set_text(GTK_LABEL(data->search_client_info_label), "");
}

void on_open_search_client_by_name(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    if (POPUP_SEARCH_CLIENT_CHOICE_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_CHOICE_GLOBAL, FALSE);
    if (POPUP_SEARCH_CLIENT_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_GLOBAL, TRUE);
    if (data->search_client_id_container) gtk_widget_set_visible(data->search_client_id_container, FALSE);
    if (data->search_client_id_buttons) gtk_widget_set_visible(data->search_client_id_buttons, FALSE);
    if (data->search_client_name_container) gtk_widget_set_visible(data->search_client_name_container, TRUE);
    if (data->search_client_name_buttons) gtk_widget_set_visible(data->search_client_name_buttons, TRUE);
    data->confirm_search_client = 0;
    gtk_widget_set_visible(data->search_client_list, FALSE);
    if (client_search_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_search_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_search_string_list, 0);
        }
    }
    gtk_label_set_text(GTK_LABEL(data->search_client_info_label), "");
}


 void on_ok_edit_client(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *id_entry = GTK_ENTRY(data->M_id_entry);
    GtkEntry *nom_entry = GTK_ENTRY(data->M_nom_entry);
    GtkEntry *prenom_entry = GTK_ENTRY(data->M_prenom_entry);
    GtkEntry *profession_entry = GTK_ENTRY(data->M_profession_entry);
    GtkEntry *tel_entry = GTK_ENTRY(data->M_tel_entry);
    GtkWidget *info_label = data->M_info_label;

    const char *id_text = gtk_editable_get_text(GTK_EDITABLE(id_entry));
    const char *nom_text = gtk_editable_get_text(GTK_EDITABLE(nom_entry));
    const char *prenom_text = gtk_editable_get_text(GTK_EDITABLE(prenom_entry));
    const char *profession_text = gtk_editable_get_text(GTK_EDITABLE(profession_entry));
    const char *tel_text = gtk_editable_get_text(GTK_EDITABLE(tel_entry));

    if (!is_digits(id_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_s1 = atoi(id_text);

    // Validate fields
    if (!is_lettre(nom_text) || !is_lettre(prenom_text) || !is_lettre(profession_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Surname, first name, profession: letters only!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (strlen(tel_text) != 10 || !is_digits(tel_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Incorrect phone number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_edit_client) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_edit_client = 1;
        return;
    }
    
    // Search for client
    client *p = NULL;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_s1) {
            p = &Clients[i];
            break;
        }
    }
    if (p == NULL) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client does not exist!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    data->confirm_edit_client = 0;
    // Update client
    strncpy(p->nom, nom_text, sizeof(p->nom)-1);
    p->nom[sizeof(p->nom)-1] = '\0';
    strncpy(p->prenom, prenom_text, sizeof(p->prenom)-1);
    p->prenom[sizeof(p->prenom)-1] = '\0';
    strncpy(p->profession, profession_text, sizeof(p->profession)-1);
    p->profession[sizeof(p->profession)-1] = '\0';
    strncpy(p->num_tel, tel_text, sizeof(p->num_tel)-1);
    p->num_tel[sizeof(p->num_tel)-1] = '\0';

    if (client_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_string_list, 0);
        }
        for (int i = 0; i < nb; i++) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
                Clients[i].Id_Client, Clients[i].nom, Clients[i].prenom, Clients[i].num_tel, Clients[i].profession);
            gtk_string_list_append(client_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(info_label), "Client updated successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);
}

void on_cancel_add_client_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_add_client = 0;
    gtk_label_set_text(GTK_LABEL(data->info_label), "");
}

void on_cancel_edit_client_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_edit_client = 0;
    gtk_label_set_text(GTK_LABEL(data->M_info_label), "");
}

void on_cancel_delete_client_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_delete_client = 0;
    gtk_label_set_text(GTK_LABEL(data->D_info_label), "");
}

void on_ok_delete_client(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *id_entry = GTK_ENTRY(data->D_id_entry);
    GtkWidget *info_label = data->D_info_label; 

    const char *id_text = gtk_editable_get_text(GTK_EDITABLE(id_entry));
    if (!is_digits(id_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_saisie = atoi(id_text);

    client *p = NULL;
    
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_saisie) {
            p = &Clients[i];
            break;
        }
    }
    if (p == NULL) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client not found!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_delete_client) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_delete_client = 1;
        return;
    }
    data->confirm_delete_client = 0;
    
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Client == id_saisie) {
            for (int j = i; j < nb_comptes - 1; j++) {
                Comptes[j] = Comptes[j + 1];
            }
            nb_comptes--;
            i--;
        }
    }
    int index = p - Clients;
    for (int j = index; j < nb - 1; j++) {
        Clients[j] = Clients[j + 1];
    }
    nb--;

    if (client_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(client_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(client_string_list, 0);
        }
        for (int i = 0; i < nb; i++) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Client Id : %d | Surname: %s | First name: %s | Phone: %s | Profession: %s",
                Clients[i].Id_Client, Clients[i].nom, Clients[i].prenom, Clients[i].num_tel, Clients[i].profession);
            gtk_string_list_append(client_string_list, buffer);
        }
    }
    if (comptes_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(comptes_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(comptes_string_list, 0);
        }
        for (int i = 0; i < nb_comptes; i++) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), " Client Id : %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
                Comptes[i].Id_Client, Comptes[i].Id_Compte, Comptes[i].nom, Comptes[i].prenom, Comptes[i].Solde_de_base, Comptes[i].date_ouverture);
            gtk_string_list_append(comptes_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(info_label), "Client deleted successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);
}

