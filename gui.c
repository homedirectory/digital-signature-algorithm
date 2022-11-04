#include <gtk/gtk.h>
#include "sign.h"

GtkWidget *window;
GtkWidget* signLabel;
GtkWidget* selectedKeyLabel;
Signature sig;
uint64 q, p, g, sk; // read from key file

static void set_margin(GtkWidget* widget, int margin) {
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
    gtk_widget_set_margin_start(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
}

static void on_open_response(GtkDialog* dialog, int response) {
    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

        g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser);
        char* path = g_file_get_path(file);

        printf("Secret key file: %s\n", path);
        gtk_label_set_label(GTK_LABEL(selectedKeyLabel), path);
        read_key(path, &p, &q, &g, &sk);
        printf("Secret key was read\n");

        g_free(path);
        g_object_unref(file);
    }

    gtk_window_destroy(GTK_WINDOW (dialog));
}

static void on_click_open_file_dialog(GtkWidget* btn, gpointer user_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_dialog_new("Select file",
            window,
            action,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Open", GTK_RESPONSE_ACCEPT,
            NULL);

    gtk_widget_show(dialog);

    g_signal_connect(dialog, "response", G_CALLBACK (on_open_response), NULL);
}

static void on_click_calculate_signature(GtkWidget* btn, gpointer user_data) {
    const char* text = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(user_data));

    Signature_init(&sig, q, p, g, sk, text);
    char sig_str[33];
    Signature_to_str(&sig, sig_str);
    gtk_label_set_label(GTK_LABEL(signLabel), sig_str);
}

static void activate (GtkApplication* app, gpointer user_data) {
    printf("GTK version: %d.%d\n", GTK_MAJOR_VERSION, GTK_MICRO_VERSION);
    // DEBUGd read key file
    read_key("lab.sec", &p, &q, &g, &sk);

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "DSS Signature Creator");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget* msgLabel = gtk_label_new("Message");
    gtk_label_set_xalign(GTK_LABEL(msgLabel), 0);
    set_margin(msgLabel, 15);
    gtk_widget_set_margin_bottom(msgLabel, 2);
    gtk_box_append(GTK_BOX(box), msgLabel);

    GtkWidget* textInput = gtk_text_new();
    gtk_text_set_placeholder_text(GTK_TEXT(textInput), "Text...");
    set_margin(textInput, 15);
    gtk_widget_set_margin_bottom(textInput, 2);
    gtk_box_append(GTK_BOX(box), textInput);

    GtkWidget* btn = gtk_button_new_with_label("Sign");
    set_margin(btn, 15);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_click_calculate_signature), 
            gtk_text_get_buffer(GTK_TEXT(textInput)));
    gtk_box_append(GTK_BOX(box), btn);

    // signature text
    signLabel = gtk_label_new("");
    set_margin(signLabel, 15);
    gtk_box_append(GTK_BOX(box), signLabel);

    GtkWidget* btnSelKey = gtk_button_new_with_label("Select secret key");
    set_margin(btnSelKey, 15);
    g_signal_connect(btnSelKey, "clicked", G_CALLBACK(on_click_open_file_dialog), 
            gtk_text_get_buffer(GTK_TEXT(textInput)));
    gtk_box_append(GTK_BOX(box), btnSelKey);

    selectedKeyLabel = gtk_label_new("");
    set_margin(selectedKeyLabel, 15);
    gtk_widget_set_margin_top(selectedKeyLabel, 5);
    gtk_box_append(GTK_BOX(box), selectedKeyLabel);

    gtk_widget_show (window);
}

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("cs.cryptohacker", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

