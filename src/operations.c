#include "./../include/data.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void on_ok_retrait_compte(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *retrait_entry = GTK_ENTRY(data->retrait_entry); 
    GtkEntry *id_client_entry = GTK_ENTRY(data->id_client_retrait); 
    GtkEntry *id_compte_entry = GTK_ENTRY(data->id_compte_retrait); 
    GtkWidget *info_label = data->info_label_retrait;              

    const char *id_compte_text = gtk_editable_get_text(GTK_EDITABLE(id_compte_entry));
    const char *id_client_text = gtk_editable_get_text(GTK_EDITABLE(id_client_entry));
    const char *retrait_text = gtk_editable_get_text(GTK_EDITABLE(retrait_entry));
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
    if (!is_float_digits(retrait_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Withdrawal amount must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_compte = atoi(id_compte_text);
    int id_client = atoi(id_client_text);
    float montant_retrait = atof(retrait_text);

    int client_found = 0;
    float *p = NULL;
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
            p = &Comptes[i].Solde_de_base;
            break;
        }
    }
    if (compte_index == -1) {
        gtk_label_set_text(GTK_LABEL(info_label), "No account with this ID for this client!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (montant_retrait > 700)
    {
        gtk_label_set_text(GTK_LABEL(info_label), "Withdrawal amount must be at most 700!");
         g_timeout_add_seconds(2, clear_info_label, info_label);
         return;
    }
    if (*p < montant_retrait) {
        gtk_label_set_text(GTK_LABEL(info_label), "Insufficient balance for this withdrawal!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (!data->confirm_retrait) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_retrait = 1;
        return;
    }
    data->confirm_retrait = 0;
    Comptes[compte_index].maximal_retrait_fait += montant_retrait;
    *p -= montant_retrait;
    if (comptes_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(comptes_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(comptes_string_list, 0);
        }
        for (int i = 0; i < nb_comptes; i++) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Client ID: %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
            Comptes[i].Id_Client, Comptes[i].Id_Compte, Comptes[i].nom, Comptes[i].prenom, Comptes[i].Solde_de_base, Comptes[i].date_ouverture);
        gtk_string_list_append(comptes_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(info_label), "Withdrawal completed successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);

    
}
void on_ok_virement_compte(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    GtkEntry *virement_entry = GTK_ENTRY(data->virement_entry); 
    GtkEntry *id_client_entry = GTK_ENTRY(data->id_client_virement); 
    GtkEntry *id_compte_entry = GTK_ENTRY(data->id_compte_virement); 
    GtkWidget *info_label = data->info_label_virement; 
    GtkEntry *id_to_client = GTK_ENTRY(data->id_virement_client_TO);
    GtkEntry *id_virement_compte_TO = GTK_ENTRY(data->id_virement_compte_TO);
    const char *id_compte_text = gtk_editable_get_text(GTK_EDITABLE(id_compte_entry));
    const char *id_client_text = gtk_editable_get_text(GTK_EDITABLE(id_client_entry));
    const char *virement_text = gtk_editable_get_text(GTK_EDITABLE(virement_entry));
    const char *id_to_client_text = gtk_editable_get_text(GTK_EDITABLE(id_to_client));
    const char *id_virement_compte_TO_text = gtk_editable_get_text(GTK_EDITABLE(id_virement_compte_TO));
    if (!is_digits(id_to_client_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Client ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
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
    if (!is_digits(id_virement_compte_TO_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Account ID must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (!is_float_digits(virement_text)) {
        gtk_label_set_text(GTK_LABEL(info_label), "Transfer amount must be a number!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int id_compte = atoi(id_compte_text);
    int id_client = atoi(id_client_text);
    int id_client_to = atoi(id_to_client_text);
    int id_virement_compte_TO_N = atoi(id_virement_compte_TO_text);

    float montant_virement = atof(virement_text);
    printf("Transfer: %f from client %d account %d to client %d account %d\n",
     montant_virement, id_client, id_compte, id_client_to, id_virement_compte_TO_N);

    int client_found = 0;
    int client_found_TO = 0;
    float *p = NULL;
    float *k = NULL;
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

    for (int i = 0; i < nb; i++) {
        if (Clients[i].Id_Client == id_client_to) {
            client_found_TO = 1;
            break;
        }
    }
    if (!client_found_TO) {
        gtk_label_set_text(GTK_LABEL(info_label), "Recipient client ID does not exist!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }

    int compte_index = -1;
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Compte == id_compte && Comptes[i].Id_Client == id_client) {
            compte_index = i;
            p = &Comptes[i].Solde_de_base;
            break;
        }
    }
    if (compte_index == -1) {
        gtk_label_set_text(GTK_LABEL(info_label), "No account with this ID for this client!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    int compte_index_TO = -1;
    for (int i = 0; i < nb_comptes; i++) {
        if (Comptes[i].Id_Compte == id_virement_compte_TO_N && Comptes[i].Id_Client == id_client_to) {
            compte_index_TO = i;
            k = &Comptes[i].Solde_de_base;
            break;
        }
    }
    if (compte_index_TO == -1) {
        gtk_label_set_text(GTK_LABEL(info_label), "No account with this ID for the recipient client!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (*p < montant_virement) {
        gtk_label_set_text(GTK_LABEL(info_label), "Insufficient funds for this transfer!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (montant_virement > 5000)
    {
        gtk_label_set_text(GTK_LABEL(info_label), "Transfer amount must not exceed 5000!");
        g_timeout_add_seconds(2, clear_info_label, info_label);
        return;
    }
    if (!data->confirm_virement) {
        gtk_label_set_text(GTK_LABEL(info_label), "Confirm?");
        data->confirm_virement = 1;
        return;
    }
    data->confirm_virement = 0;
    Comptes[compte_index].maximal_virement_fait += montant_virement;
    //printf("Before: %f, %f\n", *p, *k);
    *p = (float) *p - montant_virement;
    *k = (float) *k + montant_virement;
    //printf("After: %f, %f\n", *p, *k);
    if (comptes_string_list) {
        int n = g_list_model_get_n_items(G_LIST_MODEL(comptes_string_list));
        for (int i = 0; i < n; i++) {
            gtk_string_list_remove(comptes_string_list, 0);
        }
        for (int i = 0; i < nb_comptes; i++) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "|Client ID: %d | Account ID: %d | Client: %s %s | Balance: %.2f | Date: %s",
            Comptes[i].Id_Client, Comptes[i].Id_Compte, Comptes[i].nom, Comptes[i].prenom, Comptes[i].Solde_de_base, Comptes[i].date_ouverture);
        gtk_string_list_append(comptes_string_list, buffer);
        }
    }
    gtk_label_set_text(GTK_LABEL(info_label), "Transfer completed successfully!");
    g_timeout_add_seconds(2, clear_info_label, info_label);

    
}

void on_cancel_retrait_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_retrait = 0;
    gtk_label_set_text(GTK_LABEL(data->info_label_retrait), "");
}

void on_cancel_virement_confirm(GtkButton *button, gpointer user_data) {
    ClientData *data = user_data;
    data->confirm_virement = 0;
    gtk_label_set_text(GTK_LABEL(data->info_label_virement), "");
}
