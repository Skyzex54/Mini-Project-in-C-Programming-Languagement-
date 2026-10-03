#include "./../include/data.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *POPUP_3_GLOBAL = NULL;
GtkWidget *POPUP_GLOBAL = NULL;
GtkWidget *POPUP_2_GLOBAL = NULL;
GtkWidget *POPUP_OPERATION_RETRAIT_GLOBAL = NULL;
GtkWidget *POPUP_OPERATION_VIREMENT_GLOBAL = NULL;
GtkWidget *POPUP_FERMETURE_COMPTE_GLOBAL = NULL;
GtkWidget *POPUP_COMPTE_GLOBAL = NULL;
GtkWidget *POPUP_CONSULTATION_GLOBAL = NULL;
GtkWidget *POPUP_SEARCH_CLIENT_GLOBAL = NULL;
GtkWidget *POPUP_SEARCH_CLIENT_CHOICE_GLOBAL = NULL;


void list_item_setup(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *label = gtk_label_new(NULL);
    gtk_list_item_set_child(list_item, label);
}


void list_item_bind(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *label = gtk_list_item_get_child(list_item);
    const char *text = gtk_string_object_get_string(GTK_STRING_OBJECT(gtk_list_item_get_item(list_item)));
    gtk_label_set_text(GTK_LABEL(label), text);
}

gboolean is_lettre(const char *str) {
    if (!str || !*str) return FALSE; 
    for (const char *p = str; *p; ++p) {
        if (!g_ascii_isalpha(*p)) return FALSE;
    }
    return TRUE;
}
gboolean is_digits(const char *str) {
    if (!str || !*str) return FALSE; 
    for (const char *p = str; *p; ++p) {
        if (!g_ascii_isdigit(*p)) return FALSE;
    }
    return TRUE;
}
gboolean is_float_digits(const char *str) {
    if (!str || !*str) return FALSE;
    int dot_count = 0;
    for (const char *p = str; *p; ++p) {
        if (*p == '.') {
            dot_count++;
        
        if (dot_count > 1 || p == str || *(p+1) == '\0') return FALSE;
        } else if (!g_ascii_isdigit(*p))
        {
            return FALSE;
        }
    }
    return TRUE;
}
gboolean clear_info_label(gpointer user_data) {
    gtk_label_set_text(GTK_LABEL(user_data), "");
    return G_SOURCE_REMOVE; 
}
void switch_page(GtkButton *button, gpointer user_data) {
    GtkStack *stack = GTK_STACK(user_data);
    const char *name = (const char *)g_object_get_data(G_OBJECT(button), "page");
    if (name) {
        gtk_stack_set_visible_child_name(stack, name);
    }
}
gboolean is_valid_date(const char *str) {
    if (strlen(str) != 10 && strlen(str) != 8 ) return FALSE; 
    for (int i = 0; i < 5; i++) {
        if (i == 2 || i == 5) {
            if (str[i] != '/') return FALSE;
        } else {
            if (!g_ascii_isdigit(str[i])) return FALSE;
        }
    }
    return TRUE;
}
void POP_UP_VISIBILITY(GtkButton *button, gpointer user_data) {
    const char *name = (const char *)g_object_get_data(G_OBJECT(button), "Setvisibility");
    if (POPUP_GLOBAL) gtk_widget_set_visible(POPUP_GLOBAL, FALSE);
    if (POPUP_2_GLOBAL) gtk_widget_set_visible(POPUP_2_GLOBAL, FALSE);
    if (POPUP_3_GLOBAL) gtk_widget_set_visible(POPUP_3_GLOBAL, FALSE);
    if (POPUP_COMPTE_GLOBAL) gtk_widget_set_visible(POPUP_COMPTE_GLOBAL, FALSE);
    if (POPUP_FERMETURE_COMPTE_GLOBAL) gtk_widget_set_visible(POPUP_FERMETURE_COMPTE_GLOBAL, FALSE);
    if (POPUP_OPERATION_RETRAIT_GLOBAL) gtk_widget_set_visible(POPUP_OPERATION_RETRAIT_GLOBAL, FALSE);
    if (POPUP_OPERATION_VIREMENT_GLOBAL) gtk_widget_set_visible(POPUP_OPERATION_VIREMENT_GLOBAL, FALSE);
    if (POPUP_SEARCH_CLIENT_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_GLOBAL, FALSE);
    if (POPUP_SEARCH_CLIENT_CHOICE_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_CHOICE_GLOBAL, FALSE);
    if (name && strcmp(name, "delete") == 0) {
        if (POPUP_3_GLOBAL) gtk_widget_set_visible(POPUP_3_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "add") == 0) {
        if (POPUP_GLOBAL) gtk_widget_set_visible(POPUP_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "edit") == 0) {
        if (POPUP_2_GLOBAL) gtk_widget_set_visible(POPUP_2_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "nouveau_compte") == 0) {
        if (POPUP_COMPTE_GLOBAL) gtk_widget_set_visible(POPUP_COMPTE_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "fermeture_compte") == 0) {
        if (POPUP_FERMETURE_COMPTE_GLOBAL) gtk_widget_set_visible(POPUP_FERMETURE_COMPTE_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "operation_virement") == 0) {
        if (POPUP_OPERATION_VIREMENT_GLOBAL) gtk_widget_set_visible(POPUP_OPERATION_VIREMENT_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "operation_retrait") == 0) {
        if (POPUP_OPERATION_RETRAIT_GLOBAL) gtk_widget_set_visible(POPUP_OPERATION_RETRAIT_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "consultation") == 0) {
        if (GLOBAL_CONSULTATION_ENTRIES) gtk_widget_set_visible(GLOBAL_CONSULTATION_ENTRIES, TRUE);
    }
    if (name && strcmp(name, "search_client") == 0) {
        if (POPUP_SEARCH_CLIENT_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_GLOBAL, TRUE);
    }
    if (name && strcmp(name, "search_client_choice") == 0) {
        if (POPUP_SEARCH_CLIENT_CHOICE_GLOBAL) gtk_widget_set_visible(POPUP_SEARCH_CLIENT_CHOICE_GLOBAL, TRUE);
    }
}
