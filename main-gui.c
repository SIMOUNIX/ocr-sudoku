#include "utils/image.h"
#include "utils/pixel_operations.h"
#include "utils/stack.h"

#include "image-processing/grayscale_correction.h"
#include "image-processing/blacknwhite_correction.h"
#include "image-processing/image_rotation.h"
#include "image-processing/noise_correction.h"
#include "image-processing/contrast_correction.h"
#include "image-processing/gamma_correction.h"
#include "image-processing/image_rotation.h"
#include "image-processing/sobel_correction.h"
#include "image-processing/final.h"
#include "nn/nn.h"

#include "imagesplitting/HoughTransformation.h"
#include "imagesplitting/Splitting.h"

#include "solver/solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct Interface
{
    GtkLabel* digit_find;
    GtkWindow* window;
    GtkImage* display;
    GtkImage* display2;
    GtkImage* display3;
    GtkImage* display4;
    GtkImage* logo;
    GtkFileChooserButton* chooser;
    GtkLabel* label1;
    GtkLabel* output;
    GtkLabel* input;
    GtkLabel* title;
    GtkButton* restart;
    GtkButton* quit;
    GtkButton* start;
    GtkButton* process;
    GtkButton* solve;
    GtkButton* neural;
    GtkButton* rotate;
    GtkButton* grid;
    GtkButton* detect;
    GtkButton* split;
    GtkButton* change;
    GtkStack* stack;
    GtkEntry* angle;
    GtkButton* solve2;
    GtkButton* quit2;
    GtkImage* digit_display;
    GtkButton* back;
    GtkFileChooserButton* digit;
    GtkLabel* digit_output;
    GtkButton* restart2;
} Interface;

void on_split_clicked(GtkButton *split, gpointer user_data)
{
    Interface *interface = user_data;

    (void)split;

    splitting("output/final_square.bmp");

    gtk_widget_set_sensitive(GTK_WIDGET(interface->solve2), TRUE);
}

void on_start_clicked(GtkButton *start, gpointer user_data)
{
    Interface *interface = user_data;
    (void)start;

    gtk_stack_set_visible_child_name(interface->stack,"page1");
}

void image_processing(gchar *filename)
{
    SDL_Surface *image = loadImage(filename);

    Gamma(image);

    image = loadImage("output/gamma.bmp");

    Contrast(image);

    image = loadImage("output/contrast.bmp");

    Gauss(image);

    image = loadImage("output/gauss.bmp");

    MedianFilter(image);

    image = loadImage("output/noise.bmp");

    Grayscale(image);

    image = loadImage("output/grayscale.bmp");

    Otsu(image, "output/blacknwhite.bmp");

    image = loadImage("output/blacknwhite.bmp");

    MedianFilter(image);

    image = loadImage("output/noise.bmp");

    double **M = calloc(image->w, sizeof(double));

    for(int i = 0; i < image->w;i++)
    {
        M[i] = calloc(image->h, sizeof(double));
    }

    Sobel(image, M);

    image = loadImage("output/sobel.bmp");

    Otsu(image, "output/blacknwhite2.bmp");

    // image = loadImage("output/rotate.bmp");

    // houghTransformation(image);
}

void on_detect_clicked(GtkButton *detect, gpointer user_data)
{
    Interface *interface = user_data;

    (void)detect;

    int i = AutoRotate("output/blacknwhite2.bmp");

    SDL_Surface* image = loadImage("output/rotate.bmp");
    houghTransformation(image);

    ManualRotate("output/blacknwhite.bmp",i);

    squaredetect();


    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/final_square.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display3, pixbuf);

    gtk_widget_set_sensitive (GTK_WIDGET(interface->split), TRUE);
}

void on_grid_clicked(GtkButton *grid, gpointer user_data)
{
    Interface *interface = user_data;

    (void)grid;

    // ManualRotate("output/blacknwhite2.bmp", 0);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/rotate.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display3, pixbuf);

    gtk_stack_set_visible_child_name(interface->stack, "page3");
}

void on_solve2_clicked(GtkButton *solve2, gpointer user_data)
{
    Interface *interface = user_data;

    (void)solve2;

    // NEED TO DO THE PROCESS TO SOLVE
    	int **array = malloc(sizeof(int *) * (9 + 1)); // malloc-ing the array
        //int array[10][10] = {0};

    for (unsigned char i = 0; i < (9 + 1); ++i) {  // and setting the bounds
        array[i] = malloc(sizeof(int) * (9 + 1));  // UwU
        array[i][9] = '\0';
    }
    array[9] = NULL;

    from_photo_to_array(array);
    write_input(array);
    solve_array(array, 0, 0);
    write_output(array);
    display();

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/final/final.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display4, pixbuf);
    
    gtk_stack_set_visible_child_name(interface->stack, "page4");
}

void on_solve_clicked(GtkButton *solve, gpointer user_data)
{
    Interface *interface = user_data;

    (void)solve;

    // NEED TO DO ALL THE PROCESS HERE

    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface->chooser));
    
    image_processing(filename);
    int i = AutoRotate("output/blacknwhite2.bmp");

    SDL_Surface* image = loadImage("output/rotate.bmp");
    houghTransformation(image);

    ManualRotate("output/blacknwhite.bmp",i);

    squaredetect();
    
    splitting("output/final_square.bmp");
    // NEED TO DO THE PROCESS TO SOLVE
    int **array = malloc(sizeof(int *) * (9 + 1)); // malloc-ing the array
    //int array[10][10] = {0};

    for (unsigned char i = 0; i < (9 + 1); ++i) {  // and setting the bounds
        array[i] = malloc(sizeof(int) * (9 + 1));  // UwU
        array[i][9] = '\0';
    }
    array[9] = NULL;

    from_photo_to_array(array);
    write_input(array);
    solve_array(array, 0, 0);
    write_output(array);
    display();
    
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/final/final.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display4, pixbuf);
    
    gtk_stack_set_visible_child_name(interface->stack, "page4");
}

void on_process_clicked(GtkButton *process, gpointer user_data)
{
    Interface *interface = user_data;
    (void)process;
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface->chooser));
    
    // gtk_spinner_start(interface->spinner);
    image_processing(filename);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/noise.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display2, pixbuf);
    // gtk_spinner_stop(interface->spinner);
    
    gtk_stack_set_visible_child_name(interface->stack,"page2");
}

void on_file_choosed(GtkFileChooser *chooser, gpointer user_data)
{
    Interface *interface = user_data;
    gchar *filename = gtk_file_chooser_get_filename(chooser);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display, pixbuf);

    gtk_widget_set_sensitive (GTK_WIDGET(interface->solve), TRUE);
    gtk_widget_set_sensitive (GTK_WIDGET(interface->process), TRUE);
    g_free(filename);
}

void on_digit_choosed(GtkFileChooser *chooser, gpointer user_data)
{
    Interface *interface = user_data;
    gchar *filename = gtk_file_chooser_get_filename(chooser);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 500, 500, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->digit_display, pixbuf);

    if(isValid(filename))
    {
        int value = NN(filename);

        char buffer[2] = {0};
        snprintf(buffer, 2, "%i", value);
        
        gtk_widget_show(GTK_WIDGET(interface->digit_find));
        gtk_label_set_label(interface->digit_output, buffer);
    }
    else
    {
        gtk_label_set_label(interface->digit_output, "Nothing found!");
    }
    g_free(filename);
}

void on_restart_clicked(GtkButton *restart, gpointer user_data)
{
    Interface *interface = user_data;
    (void)restart;
    remove("output/gamma.bmp");
    remove("output/contrast.bmp");
    remove("output/blacknwhite.bmp");
    remove("output/noise.bmp");
    remove("output/gauss.bmp");
    remove("output/grayscale.bmp");
    remove("output/final_square.bmp");
    remove("output/sobel.bmp");

    gtk_stack_set_visible_child_name(interface->stack, "page1");
}

void on_change_clicked(GtkButton *change, gpointer user_data)
{
    Interface *interface = user_data;
    (void)change;

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/blacknwhite.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display2, pixbuf);

    remove("output/rotate.bmp");
}

void on_rotate_clicked(GtkButton *rotate, gpointer user_data)
{
    Interface *interface = user_data;
    (void)rotate;
    char *ptr;
    double angle;
    const gchar *input = gtk_entry_get_text(interface->angle);
    
    angle = strtod(input, &ptr);
    ManualRotate("output/blacknwhite2.bmp", angle);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("output/rotate.bmp", 600, 450, TRUE, NULL);
    gtk_image_set_from_pixbuf(interface->display2, pixbuf);

    gtk_widget_set_sensitive(GTK_WIDGET(interface->grid), TRUE);
}

void on_quit_clicked(GtkButton *quit, gpointer user_data)
{
    Interface *interface = user_data;
    (void)quit;
    gtk_widget_destroy(GTK_WIDGET(interface->window));
}

void on_quit2_clicked(GtkButton *quit, gpointer user_data)
{
    Interface *interface = user_data;
    (void)quit;
    gtk_widget_destroy(GTK_WIDGET(interface->window));
}

void on_neural_clicked(GtkButton *neural, gpointer user_data)
{
    Interface *interface = user_data;
    (void)neural;

    gtk_stack_set_visible_child_name(interface->stack, "page5");
}

void on_back_clicked(GtkButton *back, gpointer user_data)
{
    Interface *interface = user_data;
    (void)back;

    gtk_stack_set_visible_child_name(interface->stack, "page1");
}

void on_restart2_clicked(GtkButton *restart, gpointer user_data)
{
    Interface *interface = user_data;
    (void)restart;

    gtk_stack_set_visible_child_name(interface->stack, "page1");
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new();

    GError* error = NULL;
    if(gtk_builder_add_from_file(builder, "main.glade", &error) == 0)
    {
        g_printerr("Error: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    g_object_set(gtk_settings_get_default(),
                 "gtk-application-prefer-dark-theme", TRUE,
                 NULL);

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    // GtkFixed* fixed = GTK_FIXED(gtk_builder_get_object(builder, "fixed"));
  
    GtkImage* display = GTK_IMAGE(gtk_builder_get_object(builder, "display"));
    GtkImage* display2 = GTK_IMAGE(gtk_builder_get_object(builder, "display2"));
    GtkImage* display3 = GTK_IMAGE(gtk_builder_get_object(builder, "display3"));
    GtkImage* display4 = GTK_IMAGE(gtk_builder_get_object(builder, "display4"));
    GtkImage* digit_display = GTK_IMAGE(gtk_builder_get_object(builder, "digit_display"));
  
    GtkFileChooserButton* chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "chooser"));
    GtkFileChooserButton* digit = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "digit"));
  
    GtkLabel* label1 = GTK_LABEL(gtk_builder_get_object(builder, "label1"));
    GtkLabel* title = GTK_LABEL(gtk_builder_get_object(builder, "title"));
    GtkLabel* output = GTK_LABEL(gtk_builder_get_object(builder, "output"));
    GtkLabel* input = GTK_LABEL(gtk_builder_get_object(builder, "input"));
    GtkLabel* digit_output = GTK_LABEL(gtk_builder_get_object(builder, "digit_output"));
    GtkLabel* find_digit = GTK_LABEL(gtk_builder_get_object(builder, "find_digit"));
  
    GtkButton* restart = GTK_BUTTON(gtk_builder_get_object(builder, "restart"));
    GtkButton *process = GTK_BUTTON(gtk_builder_get_object(builder, "process"));
    GtkButton *quit = GTK_BUTTON(gtk_builder_get_object(builder, "quit"));
    GtkButton *start = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    GtkButton *solve = GTK_BUTTON(gtk_builder_get_object(builder, "solve"));
    GtkButton *neural = GTK_BUTTON(gtk_builder_get_object(builder, "neural"));
    GtkButton *rotate = GTK_BUTTON(gtk_builder_get_object(builder, "rotate"));
    GtkButton *change = GTK_BUTTON(gtk_builder_get_object(builder, "change"));
    GtkButton *grid = GTK_BUTTON(gtk_builder_get_object(builder, "grid"));
    GtkButton *detect = GTK_BUTTON(gtk_builder_get_object(builder, "detect"));
    GtkButton *split = GTK_BUTTON(gtk_builder_get_object(builder, "split"));
    GtkButton *solve2 = GTK_BUTTON(gtk_builder_get_object(builder, "solve2"));
    GtkButton *quit2 = GTK_BUTTON(gtk_builder_get_object(builder, "quit2"));
    GtkButton *back = GTK_BUTTON(gtk_builder_get_object(builder, "back"));
    GtkButton *restart2 = GTK_BUTTON(gtk_builder_get_object(builder, "restart2"));

    GtkEntry *angle = GTK_ENTRY(gtk_builder_get_object(builder, "angle"));
  
    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    Interface interface =
        {
            .restart2 = restart2,
            .back = back,
            .digit_output = digit_output,
            .digit = digit,
            .digit_display = digit_display,
            .window = window,
            .display = display,
            .label1 = label1,
            .chooser = chooser,
            .restart = restart,
            .quit = quit,
            .process = process,
            .start = start,
            .stack = stack,
            .display2 = display2,
            .solve = solve,
            .rotate = rotate,
            .angle = angle,
            .change = change,
            .title = title,
            .display3 = display3,
            .grid = grid,
            .detect = detect,
            .split = split,
            .display4 = display4,
            .output = output,
            .input = input,
            .solve2 = solve2,
            .quit2 = quit2,
            .digit_find = find_digit
        };

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(chooser, "selection-changed", G_CALLBACK(on_file_choosed), &interface);
    g_signal_connect(digit, "selection-changed", G_CALLBACK(on_digit_choosed), &interface);
    g_signal_connect(restart, "clicked", G_CALLBACK(on_restart_clicked), &interface);
    g_signal_connect(process, "clicked", G_CALLBACK(on_process_clicked), &interface);
    g_signal_connect(quit, "clicked", G_CALLBACK(on_quit_clicked), &interface);
    g_signal_connect(start, "clicked", G_CALLBACK(on_start_clicked), &interface);
    g_signal_connect(rotate, "clicked", G_CALLBACK(on_rotate_clicked), &interface);
    g_signal_connect(change, "clicked", G_CALLBACK(on_change_clicked), &interface);
    g_signal_connect(grid, "clicked", G_CALLBACK(on_grid_clicked), &interface);
    g_signal_connect(detect, "clicked", G_CALLBACK(on_detect_clicked), &interface);
    g_signal_connect(solve, "clicked", G_CALLBACK(on_solve_clicked), &interface);
    g_signal_connect(split, "clicked", G_CALLBACK(on_split_clicked), &interface);
    g_signal_connect(solve2, "clicked", G_CALLBACK(on_solve2_clicked), &interface);
    g_signal_connect(quit2, "clicked", G_CALLBACK(on_quit2_clicked), &interface);
    g_signal_connect(neural, "clicked", G_CALLBACK(on_neural_clicked), &interface);
    g_signal_connect(back, "clicked", G_CALLBACK(on_back_clicked), &interface);
    g_signal_connect(restart2, "clicked", G_CALLBACK(on_restart2_clicked), &interface);

    gtk_widget_show(GTK_WIDGET(window));

    gtk_main();

    return 0;
}
