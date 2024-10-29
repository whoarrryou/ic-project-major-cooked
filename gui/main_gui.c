#include <gtk/gtk.h>

// Function prototype for the signal handler
static void on_activate(GtkApplication *app, gpointer user_data);

// Initialization function for the GUI
void init_gui() {
    GtkApplication *app;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    // Connect the activate signal to the on_activate function
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Launch the application
    g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}

// Function that will handle the "activate" signal
static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Project Cooked: The Recipe App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Show the window (no need for gtk_widget_show_all in GTK4)
    gtk_widget_show(window);
}
