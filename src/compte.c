#include "./../include/data.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkStringList *comptes_string_list = NULL;
GtkStringList *consultation_string_list = NULL;

//Consultation list view
GtkWidget* create_consultation_list_view(void) {
    consultation_string_list = gtk_string_list_new(NULL);
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(list_item_setup), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(list_item_bind), NULL);

    GtkWidget *list_view = gtk_list_view_new(
        GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(consultation_string_list))),
        factory
    );
    gtk_widget_set_hexpand(list_view, TRUE);
    gtk_widget_set_vexpand(list_view, TRUE);
    return list_view;
}
// Comptes list view
GtkWidget* create_comptes_list_view(void) {
    comptes_string_list = gtk_string_list_new(NULL);
        for (int i = 0; i < nb_comptes; i++) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), " Client Id : %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
        Comptes[i].Id_Client, Comptes[i].Id_Compte, Comptes[i].nom, Comptes[i].prenom, Comptes[i].Solde_de_base, Comptes[i].date_ouverture);
        gtk_string_list_append(comptes_string_list, buffer);
    }
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(list_item_setup), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(list_item_bind), NULL);
    GtkWidget *list_view = gtk_list_view_new(
        GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(comptes_string_list))),
        factory
    );
    gtk_widget_set_hexpand(list_view, TRUE);
    gtk_widget_set_vexpand(list_view, TRUE);
    return list_view;
}

void on_ok_add_compte(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *id_compte_entry = GTK_ENTRY(data->id_compte_entry); 
    GtkEntry *id_client_entry = GTK_ENTRY(data->id_client_entry); 
    GtkEntry *nom_entry = GTK_ENTRY(data->nom_entry_compte);
    GtkEntry *prenom_entry = GTK_ENTRY(data->prenom_entry_compte);
    GtkEntry *solde_entry = GTK_ENTRY(data->solde_entry); 
    GtkEntry *date_entry = GTK_ENTRY(data->date_entry); 
    GtkWidget *info_label = data->info_label_compte;              

    const char *id_compte_text = gtk_editable_get_text(GTK_EDITABLE(id_compte_entry));
    const char *id_client_text = gtk_editable_get_text(GTK_EDITABLE(id_client_entry));
    const char *nom_text = gtk_editable_get_text(GTK_EDITABLE(nom_entry));
    const char *prenom_text = gtk_editable_get_text(GTK_EDITABLE(prenom_entry));
    const char *solde_text = gtk_editable_get_text(GTK_EDITABLE(solde_entry));
    const char *date_text = gtk_editable_get_text(GTK_EDITABLE(date_entry));

    // ID Compte verification
    if (!is_digits(id_compte_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Account ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_compte = atoi(id_compte_text);
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Compte == id_compte) {
            gtk_label_set_text(GTK_LABEL(info_label), "This account ID already exists!");
            g_timeout_add_seconds(2, clear_info_label, info_label);
            return;
        }
    }

    // ID Client verification
    if (!is_digits(id_client_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_client = atoi(id_client_text);
    int client_found = 0;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_client) {
            client_found = 1;
            break;
        }
    }
    if (!client_found) {
        gtk_label_set_text(GTK_LABEL(info_label), "No client with this ID!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    // Nom/Prenom verification
    if (!is_lettre(nom_text) || !is_lettre(prenom_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Surname and first name: letters only!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    // Solde verification
    if (!is_digits(solde_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Balance must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int solde = atoi(solde_text);
    if (solde < 1000) {
        gtk_label_set_text(GTK_LABEL(info_label), "Opening balance must be >= 1000!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    // Date verification
    if (!is_valid_date(date_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Date must be in format 00/00/00 or 00/00/0000!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_add_compte) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_add_compte = 1;
        return;
    }
    data->confirm_add_compte = 0;
    // Save new compte
    Comptes[nb_comptes].Id_Compte = id_compte;
    Comptes[nb_comptes].Id_Client = id_client;
    strncpy(Comptes[nb_comptes].nom, nom_text, sizeof(Comptes[nb_comptes].nom)-1);
    Comptes[nb_comptes].nom[sizeof(Comptes[nb_comptes].nom)-1] = '\0';
    strncpy(Comptes[nb_comptes].prenom, prenom_text, sizeof(Comptes[nb_comptes].prenom)-1);
    Comptes[nb_comptes].prenom[sizeof(Comptes[nb_comptes].prenom)-1] = '\0';
    Comptes[nb_comptes].Solde_de_base = solde;
    strncpy(Comptes[nb_comptes].date_ouverture, date_text, sizeof(Comptes[nb_comptes].date_ouverture)-1);
    Comptes[nb_comptes].date_ouverture[sizeof(Comptes[nb_comptes].date_ouverture)-1] = '\0';
    nb_comptes++;

    // Update comptes list view
    if (comptes_string_list) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Client Id : %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
            id_client, id_compte, nom_text, prenom_text, (float)solde, date_text);
        gtk_string_list_append(comptes_string_list, buffer);
    }

    gtk_label_set_text(GTK_LABEL(info_label), "Account added successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);
}
void on_fermeture_compte_button(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *id_compte_entry = GTK_ENTRY(data->fermeture_id_compte_entry);
    GtkEntry *id_client_entry = GTK_ENTRY(data->fermeture_id_client_entry); 
    GtkWidget *info_label = data->info_label_fermeture;

    const char *id_compte_text = gtk_editable_get_text(GTK_EDITABLE(id_compte_entry));
    const char *id_client_text = gtk_editable_get_text(GTK_EDITABLE(id_client_entry));

    if (!is_digits(id_compte_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Account ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (!is_digits(id_client_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    int id_compte = atoi(id_compte_text);
    int id_client = atoi(id_client_text);

    // Check if client exists
    int client_found = 0;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_client) {
            client_found = 1;
            break;
        }
    }
    if (!client_found) {
        gtk_label_set_text(GTK_LABEL(info_label), "No client with this ID !");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    // Check if compte exists and belongs to client
    int compte_index = -1;
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Compte == id_compte && Comptes[i].Id_Client == id_client) {
            compte_index = i;
            break;
        }
    }
    if (compte_index == -1) {
        gtk_label_set_text(GTK_LABEL(info_label), "No account with this ID for this client!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_fermeture_compte) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_fermeture_compte = 1;
        return;
    }
    data->confirm_fermeture_compte = 0;

    for (int j = compte_index; j < nb_comptes - 1; j++) {
        Comptes[j] = Comptes[j + 1];
    }
    nb_comptes--;

    if (comptes_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(comptes_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(comptes_string_list, 0);
        }
        for (int i = 0; i < nb_comptes; i++) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Client Id : %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
            Comptes[i].Id_Client, Comptes[i].Id_Compte, Comptes[i].nom, Comptes[i].prenom, Comptes[i].Solde_de_base, Comptes[i].date_ouverture);
        gtk_string_list_append(comptes_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(info_label), "Account closed successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);
}
void on_ok_consultation(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkWidget *info_label = data->Info_label_consultation;
    GtkWidget *consultation_list = data->consultation_list;
    GtkWidget *id_client_consultation = data->id_client_consultation;
    GtkWidget *id_compte_consultation = data->id_compte_consultation;
    const char *id_client_text = gtk_editable_get_text(GTK_EDITABLE(id_client_consultation));
    const char *id_compte_text = gtk_editable_get_text(GTK_EDITABLE(id_compte_consultation));


    if (!is_digits(id_client_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (!is_digits(id_compte_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Account ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_client = atoi(id_client_text);
    int id_compte = atoi(id_compte_text);

    int client_found = 0;
    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_client) {
            client_found = 1;
            break;
        }
    }
    if (!client_found) {
        gtk_label_set_text(GTK_LABEL(info_label), "No client with this ID !");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    int compte_index = -1;
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Compte == id_compte && Comptes[i].Id_Client == id_client) {
            compte_index = i;
            break;
        }
    }
    if (compte_index == -1) {
        gtk_label_set_text(GTK_LABEL(info_label), "No account with this ID for this client!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    if (!data->confirm_consultation) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_consultation = 1;
        return;
    }
    data->confirm_consultation = 0;
    gtk_label_set_text(GTK_LABEL(info_label), "");
    gtk_widget_set_visible(GLOBAL_CONSULTATION_ENTRIES, FALSE);
    gtk_widget_set_visible(GLOBAL_VISIBILTY_CONSULTATION_ACUN, FALSE);
    gtk_widget_set_visible(consultation_list, TRUE);
    gtk_widget_set_visible(data->bottom_cancel_consultation, TRUE);

    if (consultation_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(consultation_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(consultation_string_list, 0);
        }
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Client Id : %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s | Withdrawal Max: %.2f | Transfer Max: %.2f",
            Comptes[compte_index].Id_Client, Comptes[compte_index].Id_Compte, Comptes[compte_index].nom, Comptes[compte_index].prenom,
            Comptes[compte_index].Solde_de_base, Comptes[compte_index].date_ouverture,
            Comptes[compte_index].maximal_retrait_fait, Comptes[compte_index].maximal_virement_fait);
        gtk_string_list_append(consultation_string_list, buffer);
    }
}

void on_cancel_consultation(GtkButton *button, gpointer user_data) {
    gtk_widget_set_visible(GLOBAL_VISIBILTY_CONSULTATION_ACUN, FALSE);
    ClientData *data = user_data;
    data->confirm_consultation = 0;
    gtk_widget_set_visible(data->consultation_list, FALSE);
    gtk_widget_set_visible(data->bottom_cancel_consultation, FALSE);
    if (consultation_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(consultation_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(consultation_string_list, 0);
        }
    }
    gtk_label_set_text(GTK_LABEL(data->Info_label_consultation), "");
}

void on_bottom_cancel_consultation(GtkButton *button, gpointer user_data) {
    // Reset everything
    gtk_widget_set_visible(GLOBAL_CONSULTATION_ENTRIES, TRUE);
    gtk_widget_set_visible(GLOBAL_VISIBILTY_CONSULTATION_ACUN, FALSE);
    ClientData *data = user_data;
    data->confirm_consultation = 0;
    gtk_widget_set_visible(data->consultation_list, FALSE);
    gtk_widget_set_visible(data->bottom_cancel_consultation, FALSE);
    if (consultation_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(consultation_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(consultation_string_list, 0);
        }
    }
    gtk_editable_set_text(GTK_EDITABLE(data->id_client_consultation), "");
    gtk_editable_set_text(GTK_EDITABLE(data->id_compte_consultation), "");
    gtk_label_set_text(GTK_LABEL(data->Info_label_consultation), "");
}

void on_cancel_add_compte_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_add_compte = 0;
    gtk_label_set_text(GTK_LABEL(data->info_label_compte), "");
}

void on_cancel_fermeture_compte_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_fermeture_compte = 0;
    gtk_label_set_text(GTK_LABEL(data->info_label_fermeture), "");
}
