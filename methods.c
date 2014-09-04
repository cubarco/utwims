#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "defines.h"
#include "structs.h"
#include "methods.h"


/***********************************************
gtk+ & system
***********************************************/
void create_main()
{
    GtkWidget *menubar;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkWidget *type_popup_menu;
    GtkWidget *menu_item;


    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "大学教师工作量信息管理系统");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    /* Create a vertical box*/
    vbox= gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);


    /*Create the main menu*/
    menubar = gtk_menu_bar_new();
    create_menu(win, menubar);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);


    /*the label*/
    label= gtk_label_new("教师基本信息一览表");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);


    /*Create the tree view of types*/
    liststore= gtk_list_store_new(TEACHER_COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(treeview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("编号ID", render, "text", TEACHER_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("姓名", render, "text", TEACHER_NAME_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("单位", render, "text", TEACHER_UNIT_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("职称", render, "text", TEACHER_TITLE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    type_popup_menu= gtk_menu_new();
    menu_item= gtk_menu_item_new_with_label("查看详情");
    g_signal_connect(menu_item, "activate", teacher_view_details, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("编辑");
    g_signal_connect(menu_item, "activate", handle_edit_teacher_dialogue, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("删除");
    g_signal_connect(menu_item, "activate", handle_teacher_delete, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);

    struct popup_menu *type_menu= (struct popup_menu *)malloc(sizeof(struct popup_menu));
    type_menu->menu= type_popup_menu;
    type_menu->treeview= GTK_TREE_VIEW(treeview);
    type_menu->selection= selection;
    type_menu->active= (void **)&active_teacher;

    g_signal_connect_swapped(treeview, "button_press_event", G_CALLBACK(handle_popup_menu), type_menu);
    g_signal_connect(treeview, "row-activated", G_CALLBACK(handle_teacher_row_activated), selection);


    gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all (win);

}

void create_room_win()
{
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *roomview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *room_popup_menu;
    GtkWidget *menu_item;


    room_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (room_win), 8);
    gtk_window_set_title (GTK_WINDOW (room_win), "工作量详情");
    gtk_window_set_position (GTK_WINDOW (room_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (room_win);
    gtk_window_set_transient_for(GTK_WINDOW(room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(room_win), TRUE);

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    gtk_container_add (GTK_CONTAINER (room_win), box1);

    roomstore= gtk_list_store_new(WORK_COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    roomview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(roomstore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(roomview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("编号ID", render, "text", WORK_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("年份", render, "text", WORK_YEAR_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("科研工作量（万元）", render, "text", WORK_WORKLOAD_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("指导毕设（人）", render, "text", WORK_GUIDES_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("指导硕/博士（人）", render, "text", WORK_GUIDES_MAS_DOC_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("总分（分）", render, "text", WORK_MARK_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column6);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(roomview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    button1= gtk_button_new_with_label("新建");
    button2= gtk_button_new_with_label("编辑");
    button3= gtk_button_new_with_label("查看详情");
    button4= gtk_button_new_with_label("删除");
    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button4, TRUE, TRUE, 0);

    struct room_dialogue_check *data= (struct room_dialogue_check *)malloc(sizeof(struct room_dialogue_check));
    data->button1= button2;
    data->button2= button3;
    data->button3= button4;

    room_popup_menu= gtk_menu_new();
    menu_item= gtk_menu_item_new_with_label("查看详情");
    g_signal_connect(menu_item, "activate", G_CALLBACK(work_view_detail), NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("编辑");
    g_signal_connect(menu_item, "activate", G_CALLBACK(handle_work_edit_assistant), data);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("删除");
    g_signal_connect(menu_item, "activate", G_CALLBACK(handle_work_delete), NULL);
    g_signal_connect(menu_item, "activate", G_CALLBACK(room_dialogue_check), data);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);

    struct popup_menu *room_menu= (struct popup_menu *)malloc(sizeof(struct popup_menu));
    room_menu->menu= room_popup_menu;
    room_menu->treeview= GTK_TREE_VIEW(roomview);
    room_menu->selection= selection;
    room_menu->active= (void **)&active_work;

    g_signal_connect_swapped(roomview, "button_press_event", G_CALLBACK(handle_popup_menu), room_menu);
    g_signal_connect(roomview, "row-activated", G_CALLBACK(handle_work_row_activated), selection);

    gtk_box_pack_start(GTK_BOX(box1), roomview, TRUE, TRUE, 0);

    g_signal_connect(button2, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_work_new_assistant), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_work_edit_assistant), data);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_work_view_detail), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_work_delete), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(room_dialogue_check), data);
    g_signal_connect(room_win, "show", G_CALLBACK(room_dialogue_check), data);

    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    g_signal_connect(room_win, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), room_win);

}

void create_guest_win()
{
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *guestview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkTreeViewColumn *column7;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
//    GtkWidget *button4;


    guest_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (guest_win), 8);
    gtk_window_set_position (GTK_WINDOW (guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(guest_win), TRUE);

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    gtk_container_add (GTK_CONTAINER (guest_win), box1);

    gueststore= gtk_list_store_new(YEAR_COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
    guestview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(gueststore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(guestview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);

    column1= gtk_tree_view_column_new_with_attributes("编号ID", render, "text", YEAR_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("年份", render, "text", YEAR_NUMBER_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("课程名", render, "text", YEAR_COURSE_NAME_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("授课学时", render, "text", YEAR_COURSE_TIME_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("实验学时", render, "text", YEAR_EXP_TIME_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("学生人数", render, "text", YEAR_STU_NUM_COLUMN, NULL);
    column7= gtk_tree_view_column_new_with_attributes("班级", render, "text", YEAR_CLASS_COLUMN, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column6);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column7);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(guestview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    gtk_box_pack_start(GTK_BOX(box1), guestview, TRUE, TRUE, 0);

    button1= gtk_button_new_with_label("新基本工作");
    button2= gtk_button_new_with_label("编辑");
    button3= gtk_button_new_with_label("删除");
    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button3, TRUE, TRUE, 0);

    struct guest_dialogue_check *data= (struct guest_dialogue_check *)malloc(sizeof(struct guest_dialogue_check));
    data->button1= button2;
    data->button2= button3;

    g_signal_connect(button2, "clicked", G_CALLBACK(handle_guest_find_focus), selection);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_guest_find_focus), selection);
    g_signal_connect_swapped(button1, "clicked", G_CALLBACK(handle_year_new_assistant), data);
    g_signal_connect_swapped(button2, "clicked", G_CALLBACK(handle_year_edit_assistant), data);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_year_delete), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(guest_dialogue_check), data);
    g_signal_connect(guest_win, "show", G_CALLBACK(guest_dialogue_check), data);


    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    g_signal_connect(guest_win, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), guest_win);
}


void (*handle[])()= {file_new, file_open, file_save, file_save_as, file_close, file_quit, edit_new_teacher, edit_new_work, query_work, sta_total_workloads, sta_this_year_time, help_about, help_help};

void errmesg(const char *error)
{
    GtkWidget *dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, error);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void handle_destroy_dialogue(GtkWidget *ignored, GtkWidget *dialogue)
{
    gtk_widget_destroy(dialogue);
}

void handle_dialogue_hide(GtkWidget *ignored, GtkWidget *dialogue)
{
    gtk_widget_hide(dialogue);
}

void free_data(GtkWidget *ignored, void *data)
{
    free(data);
}

int handle_popup_menu(struct popup_menu *pop_struct, GdkEvent *event)
{
    if (!file)
        return FALSE;
    GtkMenu *menu;
    GdkEventButton *event_button;

    g_return_val_if_fail (pop_struct->menu != NULL, FALSE);
    g_return_val_if_fail (GTK_IS_MENU (pop_struct->menu), FALSE);
    g_return_val_if_fail (event != NULL, FALSE);

    if (event->type == GDK_BUTTON_PRESS)
    {
        event_button = (GdkEventButton *) event;
        GtkTreePath *path;
        if (gtk_tree_view_is_blank_at_pos(GTK_TREE_VIEW(pop_struct->treeview), event_button->x, event_button->y, &path, NULL, NULL, NULL))
            return FALSE;
        gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(pop_struct->treeview), event_button->x, event_button->y, &path, NULL, NULL, NULL);
        gtk_tree_model_get_iter(gtk_tree_view_get_model(pop_struct->treeview), &treeiter, path);
        gtk_tree_selection_select_iter(pop_struct->selection, &treeiter);
        gtk_tree_model_get(gtk_tree_view_get_model(pop_struct->treeview), &treeiter, 0, pop_struct->active, -1);
        menu= GTK_MENU(pop_struct->menu);
        if (event_button->button == GDK_BUTTON_SECONDARY)
        {
            gtk_menu_popup (menu, NULL, NULL, NULL, NULL, event_button->button, event_button->time);
            return TRUE;
        }
    }

    return FALSE;
}

void handle_room_find_focus(GtkWidget *ignored, GtkTreeSelection *selection)
{
    gtk_tree_selection_get_selected(selection,(GtkTreeModel **)(&roomstore), &treeiter);
    if (treeiter.stamp== 0)
    {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(roomstore), &treeiter);
        gtk_tree_selection_select_iter(selection, &treeiter);
    }
    gtk_tree_model_get(GTK_TREE_MODEL(roomstore), &treeiter, ROOM_POINTER_COLUMN, &active_work);
}

//void handle_new_type(GtkWidget *ignored, struct new_type_data *data)
//{
//    char type[20];
//    int most_in;
//    strcpy(type, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
//    most_in= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
//    free(data);
//    type_insert(type, most_in);
//    gtk_list_store_insert_with_values(liststore, &treeiter, 0,
//                                      TEACHER_POINTER_COLUMN, (gpointer)type_head,
//                                      TEACHER_ID_COLUMN, type,
//                                      TEACHER_NAME_COLUMN, most_in,
//                                      TEACHER_UNIT_COLUMN, type_head->total_,
//                                      TEACHER_TITLE_COLUMN, type_head->left_,
////                                      IN_COLUMN, type_head->total_-type_head->left_,
//                                      -1);
//    edited= 1;
//}
//Edit
void handle_new_teacher(GtkWidget *ignored, struct new_teacher_data *data)
{
    char id[12];
    char name[20];
    char unit[30];
    char title[30];
    strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    strcpy(name, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    strcpy(unit, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    strcpy(title, gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    free(data);
    teacher_insert(id, name, unit, title);
    gtk_list_store_insert_with_values(liststore, &treeiter, 0,
                                      TEACHER_POINTER_COLUMN, (gpointer)teacher_head,
                                      TEACHER_ID_COLUMN, id,
                                      TEACHER_NAME_COLUMN, name,
                                      TEACHER_UNIT_COLUMN, unit,
                                      TEACHER_TITLE_COLUMN, title,
                                      -1);
    edited= 1;
}


//void handle_type_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection)
//{
//    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&liststore, &treeiter);
//    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &treeiter, TEACHER_POINTER_COLUMN, &active_type, -1);
//    type_view_details();
//}
//Edit
void handle_teacher_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection)
{
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&liststore, &treeiter);
    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &treeiter, TEACHER_POINTER_COLUMN, &active_teacher, -1);
    teacher_view_details();
}

//void handle_edit_type_dialogue()
//{
//    teacher_dialogue(EDIT);
//}
//Edit
void handle_edit_teacher_dialogue()
{
    teacher_dialogue(EDIT);
}

//void handle_edit_type(GtkWidget *ignored, struct new_type_data *data)
//{
//    char type[20];
//    int most_in;
//    strcpy(type, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
//    most_in= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
//    free(data);
//    type_modify(type, most_in);
//    gtk_list_store_set(liststore, &treeiter,
//                       TEACHER_POINTER_COLUMN, (gpointer)active_type,
//                       TEACHER_ID_COLUMN, type,
//                       TEACHER_NAME_COLUMN, most_in,
//                       TEACHER_UNIT_COLUMN, active_type->total_,
//                       TEACHER_TITLE_COLUMN, active_type->left_,
////                       IN_COLUMN, active_type->total_- active_type->left_,
//                       -1);
//    edited= 1;
//}
//Edit
void handle_edit_teacher(GtkWidget *ignored, struct new_teacher_data *data)
{
    char id[12];
    char name[20];
    char unit[30];
    char title[30];
    strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    strcpy(name, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    strcpy(unit, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    strcpy(title, gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    free(data);
    teacher_modify(id, name, unit, title);
    gtk_list_store_set(liststore, &treeiter,
                       TEACHER_POINTER_COLUMN, (gpointer)active_teacher,
                       TEACHER_ID_COLUMN, id,
                       TEACHER_NAME_COLUMN, name,
                       TEACHER_UNIT_COLUMN, unit,
                       TEACHER_TITLE_COLUMN, unit,
                       -1);
    edited= 1;
}

//void handle_type_delete()
//{
//    gtk_list_store_remove(liststore, &treeiter);
//    type_delete();
//}
//Edit
void handle_teacher_delete()
{
    gtk_list_store_remove(liststore, &treeiter);
    teacher_delete();
}

void handle_work_new_assistant(void *ignored, struct room_dialogue_check *data)
{
    work_dialogue(NEW, data);
}

void handle_work_edit_assistant(void *ignored, struct room_dialogue_check *data)
{
    work_dialogue(EDIT, data);
}

void handle_year_new_assistant(void *ignored, struct guest_dialogue_check *data)
{
    year_dialogue(NEW, data);
}


void handle_year_edit_assistant(void *ignored, struct guest_dialogue_check *data)
{
    year_dialogue(EDIT, data);
}

//void handle_room_new(GtkWidget *ignored, struct new_room_data *data)
//{
//
//    struct basic_room_type_data *type;
//    char room_number[6];
//    char phone_number[20];
//    float area;
//    float price;
//    int is_left;
//
//    GtkTreeIter iter;
//    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
//    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(data->combo1)), &iter, TEACHER_POINTER_COLUMN, &type, -1);
//    strcpy(room_number, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
//    strcpy(phone_number, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
//    area= atof(gtk_entry_get_text(GTK_ENTRY(data->entry3)));
//    price= atof(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
//    is_left= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton));
//
//    free(data);
//
//    room_insert(type, room_number, phone_number, area, price, is_left);
//
//    GtkTreeIter titer;
//    GtkTreePath *path= gtk_tree_path_new_first();
//    struct basic_room_type_data *tmp;
//    gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
//    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, TEACHER_POINTER_COLUMN, &tmp, -1);
//    while (tmp!=type)
//    {
//        gtk_tree_path_next(path);
//        gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
//        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, TEACHER_POINTER_COLUMN, &tmp, -1);
//    }
//    gtk_list_store_set(liststore, &titer,
//                       TEACHER_POINTER_COLUMN, (gpointer) type,
//                       TEACHER_ID_COLUMN, type->type_,
//                       TEACHER_NAME_COLUMN, type->most_in_,
//                       TEACHER_UNIT_COLUMN, type->total_,
//                       TEACHER_TITLE_COLUMN, type->left_,
////                       IN_COLUMN, type->total_- type->left_,
//                       -1);
//    if (gtk_widget_get_visible(room_win))
//    {
//        gtk_list_store_clear(roomstore);
//        access_room_data();
//    }
//}
//Edit
void handle_work_new(GtkWidget *ignored, struct new_work_data *data)
{

    struct basic_teacher_data *teacher;
    char id[12];
    int year;
    int workload;
    int guides;
    int guides_mas_doc;
    int mark;

    GtkTreeIter iter;
    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(data->combo1)), &iter, TEACHER_POINTER_COLUMN, &teacher, -1);
    strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    year= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    workload= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    guides= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    guides_mas_doc= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
    mark= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry6)));

    free(data);

    work_insert(teacher, id, year, workload, guides, guides_mas_doc, mark);

//    GtkTreeIter titer;
//    GtkTreePath *path= gtk_tree_path_new_first();
//    struct basic_teacher_data *tmp;
//    gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
//    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, TEACHER_POINTER_COLUMN, &tmp, -1);
//    while (tmp!=teacher)
//    {
//        gtk_tree_path_next(path);
//        gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
//        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, TEACHER_POINTER_COLUMN, &tmp, -1);
//    }
    if (gtk_widget_get_visible(room_win))
    {
        gtk_list_store_clear(roomstore);
        access_work_data();
    }
}


void handle_work_modify(GtkWidget *ignored, struct new_work_data *data)
{
    struct basic_teacher_data *teacher;
    char id[12];
    int year;
    int workload;
    int guides;
    int guides_mas_doc;
    int mark;

    if ((active_work==NULL) || (active_work->teacher_!= active_teacher))
        return;

    GtkTreeIter iter;
    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(data->combo1)), &iter, TEACHER_POINTER_COLUMN, &teacher, -1);
    strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    year= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    workload= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    guides= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    guides_mas_doc= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
    mark= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry6)));

    free(data);

    work_modify(teacher, id, year, workload, guides, guides_mas_doc, mark);

    gtk_list_store_clear(roomstore);
    access_work_data();

//    gtk_list_store_clear(liststore);
//    access_type_data();
}

void handle_work_delete()
{
//    if ((active_work==NULL) || (active_work->teacher_!= active_teacher))
//        return;

    if (active_work==NULL)
        return;

    gtk_list_store_remove(GTK_LIST_STORE(roomstore), &treeiter);

    work_delete();

//    gtk_list_store_clear(liststore);
//    access_type_data();
}

void handle_work_view_detail()
{
    work_view_detail();
}

void handle_work_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection)
{
    GtkTreeIter iter;
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&roomstore, &iter);
    gtk_tree_model_get(GTK_TREE_MODEL(roomstore), &iter, WORK_POINTER_COLUMN, &active_work, -1);
    work_view_detail();
}

void handle_guest_find_focus(GtkWidget *ignored, GtkTreeSelection *selection)
{
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&gueststore, &treeiter);
    if (treeiter.stamp== 0)
    {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gueststore), &treeiter);
        gtk_tree_selection_select_iter(selection, &treeiter);
    }
    gtk_tree_model_get(GTK_TREE_MODEL(gueststore), &treeiter, GUEST_POINTER_COLUMN, &active_year);
}

void handle_year_new(GtkWidget *ignored, struct new_year_data *data)
{
    struct basic_teacher_data *teacher_iter= teacher_head;
    struct basic_work_data *work_iter;
//    int con= 1;
//    while (teacher_iter && con)
//    {
//        work_iter= teacher_iter->work_head_;
//        while (work_iter)
//        {
//
//            if (!(con =strcmp(work_iter->id_, gtk_entry_get_text(GTK_ENTRY(data->entry1)))))
//                break;
//            work_iter= work_iter->next_;
//        }
//        teacher_iter= teacher_iter->next_;
//    }
//    if (con)
//        errmesg("No such work info.");
//    else
//    {
        char id[12];
        char year[12];
        char course_name[12];
        int course_time;
        int exp_time;
        int stu_num;
        char class_name[15];

        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
        strcpy(year, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
        strcpy(course_name, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        course_time= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
        exp_time= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
        stu_num= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry6)));
        strcpy(class_name, gtk_entry_get_text(GTK_ENTRY(data->entry7)));

        free(data);

//        struct basic_per_year_info *year_iter= active_work->per_year_;
//        while (year_iter)
//        {
//            if (!strcmp(year_iter->id_, id))
//                break;
//            year_iter= year_iter->next_;
//        }
//        if (!year_iter)
//        {
//            errmesg("No year in info.");
//            return;
//        }

//        year_insert(year_iter, out_time, pay);
        year_insert(active_work, id, year, course_name, course_time, exp_time, stu_num, class_name);
        if (gtk_widget_get_visible(guest_win))
        {
            gtk_list_store_clear(GTK_LIST_STORE(gueststore));
            access_year_data();
        }
//    }
}

void handle_year_modify(GtkWidget *ignored, struct new_year_data *data)
{
    char id[12];
    char year[12];
    char course_name[12];
    int course_time;
    int exp_time;
    int stu_num;
    char class_name[15];

    strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    strcpy(year, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    strcpy(course_name, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    course_time= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    exp_time= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
    stu_num= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry6)));
    strcpy(class_name, gtk_entry_get_text(GTK_ENTRY(data->entry7)));

    free(data);

    year_modify(active_work, id, year, course_name, course_time, exp_time, stu_num, class_name);

    gtk_list_store_clear(GTK_LIST_STORE(gueststore));
    access_year_data();
}

void handle_year_delete()
{
//    if ((active_year==NULL) || (active_year->work_info_!= active_work))
//        return;
    if (active_year==NULL)
        return;
    gtk_list_store_remove(gueststore, &treeiter);

    year_delete();

//    gtk_list_store_clear(roomstore);
//    access_work_data();
//    gtk_list_store_clear(liststore);
//    access_type_data();
}

void handle_query_work(GtkWidget *ignored, struct query_room *data)
{
    char id[20];
    char name[20];
    struct basic_teacher_data *teacher;
    int is_teacher_limited;
    int is_id_limited;
    int is_name_limited;
//    int is_left;
//    int is_left_limited;
    int uyear;
    int lyear;
    int is_year_limited;
    int uworkload;
    int lworkload;
    int is_workload_limited;

    is_id_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton1));
    is_name_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton2));
    is_teacher_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton3));
//    is_left_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton4));
    is_year_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton5));
    is_workload_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton6));

    if (is_id_limited)
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    if (is_name_limited)
        strcpy(name, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    if (is_teacher_limited)
    {
        GtkTreeIter iter;
        gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, TEACHER_POINTER_COLUMN, &teacher, -1);
    }
//    if (is_left_limited)
//        is_left= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton7));
    if (is_year_limited)
    {
        lyear= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
        uyear= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
    }
    if (is_workload_limited)
    {
        lworkload= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry6)));
        uworkload= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry7)));
    }

    q_work_by_conditions(data->store, is_id_limited, is_name_limited, is_teacher_limited, is_year_limited, is_workload_limited, teacher, id, name, uyear, lyear, uworkload, lworkload);

}

void handle_query_guest(GtkWidget *ignored, struct query_guest *data)
{
    char id[20];
    char name_piece[20];
    char lin_time[18];
    char uin_time[18];
    int is_id_limited;
    int is_name_limited;
    int is_in_time_limited;

    is_id_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton1));
    is_name_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton2));
    is_in_time_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton3));

    if (is_id_limited)
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    if (is_name_limited)
        strcpy(name_piece, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    if (is_in_time_limited)
    {
        strcpy(lin_time, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        strcpy(uin_time, gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    }

    q_guest_info_by_conditions(data->store, is_id_limited, is_name_limited, is_in_time_limited, id, name_piece, lin_time, uin_time);

}

void sta_total_workloads()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    GtkWidget *sta_total_workloads_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *label1;
    GtkWidget *treview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;

    sta_total_workloads_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_total_workloads_win), 8);
    gtk_window_set_position (GTK_WINDOW (sta_total_workloads_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_total_workloads_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_total_workloads_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_total_workloads_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_total_workloads_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_total_workloads_win), "工作量统计");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_total_workloads_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    label1= gtk_label_new("总工作量排名：");

    store= gtk_list_store_new(STA_WORKLOADS_COLUMN_NUMBER, G_TYPE_STRING, G_TYPE_INT);
    treview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("姓名", render, "text", STA_WORKLOADS_NAME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);
    column= gtk_tree_view_column_new_with_attributes("总工作量", render, "text", STA_WORKLOADS_WORKLOADS_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);

//    cal_rooms_sta(store, year);
    cal_total_workload_sta(store);

    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_total_workloads_win);
}

void sta_this_year_time()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    GtkWidget *sta_this_year_time_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *label1;
    GtkWidget *treview1;
    GtkListStore *store1;
    GtkCellRenderer *render1;
    GtkWidget *treview2;
    GtkListStore *store2;
    GtkCellRenderer *render2;
    GtkTreeViewColumn *column;

    sta_this_year_time_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_this_year_time_win), 8);
    gtk_window_set_position (GTK_WINDOW (sta_this_year_time_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_this_year_time_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_this_year_time_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_this_year_time_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_this_year_time_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_this_year_time_win), "本年度教学总学时统计");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_this_year_time_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    label1= gtk_label_new("年度教学总学时排名：");

    store1= gtk_list_store_new(STA_WORKLOADS_COLUMN_NUMBER, G_TYPE_STRING, G_TYPE_INT);
    treview1= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store1));
    render1= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render1), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("姓名", render1, "text", STA_WORKLOADS_NAME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview1), column);
    column= gtk_tree_view_column_new_with_attributes("教学总学时", render1, "text", STA_WORKLOADS_WORKLOADS_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview1), column);

    cal_this_year_time_sta(store1);

    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treview1, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_this_year_time_win);
}

void room_dialogue_check(GtkMenuItem *ignored, struct room_dialogue_check *data)
{
    if (active_teacher && active_teacher->work_head_)
    {
        gtk_widget_set_sensitive(data->button1, TRUE);
        gtk_widget_set_sensitive(data->button2, TRUE);
        gtk_widget_set_sensitive(data->button3, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(data->button1, FALSE);
        gtk_widget_set_sensitive(data->button2, FALSE);
        gtk_widget_set_sensitive(data->button3, FALSE);
    }
}

void guest_dialogue_check(GtkMenuItem *ignored, struct guest_dialogue_check *data)
{
//    if (active_teacher && active_work && active_work->per_year_)
//    {
//        gtk_widget_set_sensitive(data->button1, TRUE);
//        gtk_widget_set_sensitive(data->button2, TRUE);
//    }
//    else
//    {
//        gtk_widget_set_sensitive(data->button1, FALSE);
//        gtk_widget_set_sensitive(data->button2, FALSE);
//    }
}

void file_close(GtkWidget *win)
{
    if (file)
    {
        gtk_list_store_clear(liststore);
        fclose(file);
        file= NULL;
    }
}


void file_new(GtkWidget *win)
{
    char *filename= choose_file(SAVE_FILE);
    if (filename)
    {
        file_close(win);
        remove(filename);
        file= fopen(filename, "wb+");
        free(filename);
        if (!file)
        {
            errmesg("Access Denied.");
        }
    }

}

void file_open(GtkWidget *win)
{
    char *filename= choose_file(OPEN_FILE);
    if (filename)
    {
        file_close(win);
        file= fopen(filename, "rb+");
        if (!file)
        {
            errmesg("Access Denied.");
        }
        else
        {
            /**
                Edit
            */
//            type_head= load_data();
            teacher_head=load_all_data();
//            access_type_data();
            access_teacher_data();
        }
        free(filename);
    }
}

void file_save(GtkWidget *win)
{
    save_data();
}

void file_save_as(GtkWidget *win)
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    char *filename= choose_file(SAVE_FILE_AS);
    if (filename)
    {
        FILE *tfile;
        tfile= file;
        remove(filename);
        file= fopen(filename, "wb+");
        free(filename);
        if (!file)
        {
            errmesg("Access Denied.");
        }
        else
        {
            save_data();
        }
        fclose(file);
        file= tfile;
    }
}

void file_quit(GtkWidget *win)
{
    file_close(win);
    gtk_main_quit();
}

//void edit_new_type(GtkWidget *win)
//{
//    if (file)
//        type_dialogue(NEW);
//    else
//        errmesg("No File Opened.");
//}
//Edit
void edit_new_teacher(GtkWidget *win)
{
    if (file)
        teacher_dialogue(NEW);
    else
        errmesg("No File Opened.");
}

void edit_new_work(GtkWidget *win)
{
    if (file)
    {
        if (teacher_head)
            work_dialogue(NEW, NULL);
        else
            errmesg("No room type available.");
    }
    else
        errmesg("No File Opened.");
}

void edit_new_year(GtkWidget *win)
{
    if (file)
        year_dialogue(NULL);
}

void query_work()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }

    GtkWidget *query_work_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *box4;
    GtkWidget *grid;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
//    GtkTreeViewColumn *column6;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
//    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *label7;
    GtkWidget *label8;
    GtkWidget *label9;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
    GtkWidget *combo1;
    GtkCellRenderer *listrender;
    GtkWidget *checkbutton1;
//    GtkWidget *checkbutton7;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
//    GtkWidget *checkbutton4;
    GtkWidget *checkbutton5;
    GtkWidget *checkbutton6;
    GtkWidget *button1;
    GtkWidget *button2;

    query_work_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (query_work_win), 8);
    gtk_window_set_position (GTK_WINDOW (query_work_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (query_work_win);
    gtk_window_set_transient_for(GTK_WINDOW(query_work_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(query_work_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(query_work_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(query_work_win), "查询");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(query_work_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    box4= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box4), TRUE);

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_column(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_grid_insert_row(GTK_GRID(grid), 6);

    label1= gtk_label_new("按条件查找基本工作信息");

    label2= gtk_label_new("编号ID");
    entry1= gtk_entry_new();
    checkbutton1= gtk_check_button_new_with_label("限定");

    label3= gtk_label_new("姓名");
    entry2= gtk_entry_new();
    checkbutton2= gtk_check_button_new_with_label("限定");

    label4=gtk_label_new("教师编号ID");
    combo1= gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo1), TEACHER_ID_COLUMN);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1), 0);
    listrender= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo1), listrender, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo1), listrender, "text", TEACHER_ID_COLUMN, NULL);
    checkbutton3= gtk_check_button_new_with_label("限定");

//    label5= gtk_label_new("是否空闲");
//    checkbutton7= gtk_check_button_new_with_label("空闲");
//    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton7), TRUE);
//    checkbutton4= gtk_check_button_new_with_label("限定");

    label6= gtk_label_new("年份");
    entry4= gtk_entry_new();
    label8= gtk_label_new("至");
    entry5= gtk_entry_new();
    checkbutton5= gtk_check_button_new_with_label("限定");

    label7= gtk_label_new("科研工作量");
    entry6= gtk_entry_new();
    label9= gtk_label_new("至");
    entry7= gtk_entry_new();
    checkbutton6= gtk_check_button_new_with_label("限定");

    gtk_box_pack_start(GTK_BOX(box2), entry4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), label8, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), entry5, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box3), entry6, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), label9, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry7, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo1, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton3, 2, 3, 1, 1);
//    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
//    gtk_grid_attach(GTK_GRID(grid), checkbutton7, 1, 4, 1, 1);
//    gtk_grid_attach(GTK_GRID(grid), checkbutton4, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label6, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box2, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton5, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label7, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box3, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton6, 2, 5, 1, 1);

    store= gtk_list_store_new(6, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("编号ID", render, "text", 1, NULL);
    column2= gtk_tree_view_column_new_with_attributes("姓名", render, "text", 2, NULL);
    column3= gtk_tree_view_column_new_with_attributes("年份", render, "text", 3, NULL);
    column4= gtk_tree_view_column_new_with_attributes("科研工作量", render, "text", 4, NULL);
    column5= gtk_tree_view_column_new_with_attributes("教师编号ID", render, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);

    button1= gtk_button_new_with_label("查询");
    button2= gtk_button_new_with_label("关闭");

    gtk_box_pack_start(GTK_BOX(box4), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box4), button2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treeview, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box4, FALSE, TRUE, 0);

    struct query_room *data= (struct query_room *)malloc(sizeof(struct query_room));
    data->entry1= entry1;
    data->entry2= entry2;
    data->combo1= combo1;
    data->entry4= entry4;
    data->entry5= entry5;
    data->entry6= entry6;
    data->entry7= entry7;
    data->checkbutton1= checkbutton1;
    data->checkbutton2= checkbutton2;
    data->checkbutton3= checkbutton3;
//    data->checkbutton4= checkbutton4;
    data->checkbutton5= checkbutton5;
    data->checkbutton6= checkbutton6;
//    data->checkbutton7= checkbutton7;
    data->store= store;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_query_work), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(free_data), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), query_work_win);

    gtk_widget_show_all(query_work_win);
}

void query_guest()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }

    GtkWidget *query_guest_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *grid;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkTreeViewColumn *column7;
    GtkTreeViewColumn *column8;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkWidget *button1;
    GtkWidget *button2;

    query_guest_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (query_guest_win), 8);
    gtk_window_set_position (GTK_WINDOW (query_guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (query_guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(query_guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(query_guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(query_guest_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(query_guest_win), "查询");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(query_guest_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_column(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);

    label1= gtk_label_new("查询住宿信息");

    label2= gtk_label_new("身份证号");
    entry1= gtk_entry_new();
    checkbutton1= gtk_check_button_new_with_label("限定");

    label3= gtk_label_new("姓名或姓名片段");
    entry2= gtk_entry_new();
    checkbutton2= gtk_check_button_new_with_label("限定");

    label4=gtk_label_new("入住时间段");
    entry3= gtk_entry_new();
    label5= gtk_label_new("至");
    entry4=gtk_entry_new();
    box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    gtk_box_pack_start(GTK_BOX(box3), entry3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), label5, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry4, TRUE, TRUE, 0);
    checkbutton3= gtk_check_button_new_with_label("限定");

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton3, 2, 3, 1, 1);

    store= gtk_list_store_new(GUEST_COLUMN_NUMBER+1, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_STRING);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("身份证号", render, "text", GUEST_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("姓名", render, "text", GUEST_NAME_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("入住时间", render, "text", GUEST_IN_TIME_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("退房时间", render, "text", GUEST_OUT_TIME_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("总计时间", render, "text", GUEST_DAY_SPENT_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("应付费用", render, "text", GUEST_FARE_COLUMN, NULL);
    column7= gtk_tree_view_column_new_with_attributes("实付费用", render, "text", GUEST_PAID_COLUMN, NULL);
    column8= gtk_tree_view_column_new_with_attributes("所住房间", render, "text", GUEST_COLUMN_NUMBER, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column8);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column6);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column7);


    button1= gtk_button_new_with_label("查询");
    button2= gtk_button_new_with_label("关闭");

    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treeview, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    struct query_guest *data= (struct query_guest *)malloc(sizeof(struct query_guest));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->checkbutton1= checkbutton1;
    data->checkbutton2= checkbutton2;
    data->checkbutton3= checkbutton3;
    data->store= store;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_query_guest), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(free_data), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), query_guest_win);

    gtk_widget_show_all(query_guest_win);
}

void sta_by_month()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    GtkWidget *sta_month_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_month_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_month_win), 8);
    gtk_window_set_position (GTK_WINDOW (sta_month_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_month_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_month_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_month_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_month_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_month_win), "营业额统计");

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_month_win), box);

    time_t now;
    int year;
    char stime[50];
    const char *p= stime;
    time(&now);
    strftime(stime, 50, "统计时间: %Y 年%m月%d日%H时%M分%S秒", localtime(&now));
    get_num(&p, ' ');
    year=get_num(&p, ' ');

    label= gtk_label_new(stime);

    int types= cal_types();
    GType type_list[types];
    fill_type(types, type_list);
    store= gtk_list_store_newv(types+1, type_list);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    int i;
    struct basic_room_type_data *type_iter= type_head;
    column= gtk_tree_view_column_new_with_attributes("月份", render, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    for (i=1; i<= types; i++, type_iter= type_iter->next_)
    {
        column= gtk_tree_view_column_new_with_attributes(type_iter->type_, render, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    GtkTreeIter iter;
    GValue value[types+1];
    float total[types+1];
    memset(total, 0, sizeof(total));
    int column_num[types+1];
    for (i=1; i<=12; i++)
    {
        memset(value, 0, sizeof(value));
        cal_values(types, year, i, column_num, value, total);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set_valuesv(store, &iter, column_num, value, types+1);
    }
    memset(value, 0, sizeof(value));
    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, "总计");
    for (i=1; i<=types; i++)
    {
        g_value_init(value+i, G_TYPE_FLOAT);
        g_value_set_float(value+i, total[i]);
    }
    gtk_list_store_append(store, &iter);
    gtk_list_store_set_valuesv(store, &iter, column_num, value, types+1);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_month_win);
}
//
//void sta_by_year()
//{
//    if (!file)
//    {
//        errmesg("No File Opened.");
//        return;
//    }
//    GtkWidget *sta_year_win;
//    GtkWidget *box1;
//    GtkWidget *box2;
//    GtkWidget *label;
//    GtkWidget *entry;
//    GtkWidget *button;
//
//    sta_year_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
//    gtk_container_set_border_width (GTK_CONTAINER (sta_year_win), 0);
//    gtk_window_set_position (GTK_WINDOW (sta_year_win), GTK_WIN_POS_CENTER);
//    gtk_widget_realize (sta_year_win);
//    gtk_window_set_transient_for(GTK_WINDOW(sta_year_win), GTK_WINDOW(win));
//    gtk_window_set_modal(GTK_WINDOW(sta_year_win), TRUE);
//    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_year_win), TRUE);
//    gtk_window_set_title(GTK_WINDOW(sta_year_win), "营业额统计");
//    gtk_window_resize(GTK_WINDOW(sta_year_win), 4, 2);
//
//    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
//    gtk_container_add(GTK_CONTAINER(sta_year_win), box1);
//
//    label= gtk_label_new("请输入查询年份:");
//
//    entry= gtk_entry_new();
//    button= gtk_button_new_with_label("确定");
//    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
//    gtk_box_pack_start(GTK_BOX(box2), entry, TRUE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box2), button, FALSE, FALSE, 0);
//
//    gtk_box_pack_start(GTK_BOX(box1), label, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);
//
//    g_signal_connect(button, "clicked", G_CALLBACK(handle_sta_total_workloads), NULL);
//    g_signal_connect(button, "clicked", G_CALLBACK(handle_destroy_dialogue), sta_year_win);
//
//    gtk_widget_show_all(sta_year_win);
//
//}

void sta_by_guest()
{
    GtkWidget *sta_guest_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_guest_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_guest_win), 0);
    gtk_window_set_position (GTK_WINDOW (sta_guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_guest_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_guest_win), "旅客统计");
    gtk_window_resize(GTK_WINDOW(sta_guest_win), 4, 2);

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_guest_win), box);

    label= gtk_label_new("Top 10旅客:");

    store= gtk_list_store_new(STA_GUEST_COLUMN_NUMBER, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("排名", render, "text", STA_GUEST_RANK_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("身份证号", render, "text", STA_GUEST_ID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("旅客姓名", render, "text", STA_GUEST_NAME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("累计住宿天数", render, "text", STA_GUEST_IN_TIME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("应缴费用总额", render, "text", STA_GUEST_FARE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("实缴费用总额", render, "text", STA_GUEST_PAID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("折扣率", render, "text", STA_DISCOUNT_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    cal_top_10_guest(store);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_guest_win);

}

void sta_by_room()
{
    GtkWidget *sta_room_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_room_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_room_win), 0);
    gtk_window_set_position (GTK_WINDOW (sta_room_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_room_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_room_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_room_win), "客房统计");
    gtk_window_resize(GTK_WINDOW(sta_room_win), 4, 2);

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_room_win), box);

    label= gtk_label_new("客房盈利排序:");

    store= gtk_list_store_new(STA_ROOM_COLUMN_NUMBER2, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("排名", render, "text", STA_ROOM_RANK_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("房间号", render, "text", STA_ROOM_NUMBER_COLUMN2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("客房类型", render, "text", STA_ROOM_TYPE_COLUMN2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("应缴费用总额", render, "text", STA_ROOM_FARE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("实缴费用总额", render, "text", STA_ROOM_PAID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("折扣率", render, "text", STA_ROOM_DISCOUNT_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    cal_top_room(store);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_room_win);
}

void help_about()
{
    GtkWidget *about_win;
    about_win= gtk_about_dialog_new();
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_win), version);
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_win), "utwims");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(about_win), GTK_LICENSE_GPL_2_0);
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(about_win), url);
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG(about_win), "github:https://github.com/cubarco/utwims");
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(about_win), auther);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_win), NULL);
    gtk_widget_show_all(about_win);
}

void help_help(GtkWidget *win)
{
    errmesg("暂无.");
}

//void access_type_data()
//{
//    struct basic_room_type_data *type_itor= type_head;
//    while (type_itor)
//    {
//        gtk_list_store_insert_with_values(liststore, &treeiter, -1,
//                                          TEACHER_POINTER_COLUMN, (gpointer)type_itor,
//                                          TEACHER_ID_COLUMN, type_itor->type_,
//                                          TEACHER_NAME_COLUMN, type_itor->most_in_,
//                                          TEACHER_UNIT_COLUMN, type_itor->total_,
//                                          TEACHER_TITLE_COLUMN, type_itor->left_,
////                                          IN_COLUMN, type_itor->total_- type_itor->left_,
//                                          -1);
//        type_itor= type_itor->next_;
//    }
//}
/**
    Edit
*/
void access_teacher_data()
{
    struct basic_teacher_data *teacher_itor= teacher_head;
    while(teacher_itor)
    {
        gtk_list_store_insert_with_values(liststore, &treeiter, -1,
                                          TEACHER_POINTER_COLUMN, (gpointer)teacher_itor,
                                          TEACHER_ID_COLUMN, teacher_itor->id_,
                                          TEACHER_NAME_COLUMN, teacher_itor->name_,
                                          TEACHER_UNIT_COLUMN, teacher_itor->unit_,
                                          TEACHER_TITLE_COLUMN, teacher_itor->title_,
                                          -1);
        teacher_itor= teacher_itor->next_;
    }
}

//void access_room_data()
//{
//    struct basic_room_data *room_iter= active_type->room_head_;
//    GtkTreeIter iter;
//    while (room_iter)
//    {
//        gtk_list_store_insert_with_values(roomstore, &iter, -1,
//                                          ROOM_POINTER_COLUMN, room_iter,
//                                          ROOM_NUMBER_COLUMN, room_iter->room_number_,
//                                          PHONE_NUMBER_COLUMN, room_iter->phone_number_,
//                                          AREA_COLUMN, room_iter->area_,
//                                          PRICE_COLUMN, room_iter->price_,
//                                          IS_LIFT_COLUMN, room_iter->is_left_? "是": "否",
//                                          -1);
//        room_iter= room_iter->next_;
//    }
//}
//Edit
void access_work_data()
{
    struct basic_work_data *work_iter= active_teacher->work_head_;
    GtkTreeIter iter;
    while (work_iter)
    {
        gtk_list_store_insert_with_values(roomstore, &iter, -1,
                                          WORK_POINTER_COLUMN, work_iter,
                                          WORK_ID_COLUMN, work_iter->id_,
                                          WORK_YEAR_COLUMN, work_iter->year_num_,
                                          WORK_WORKLOAD_COLUMN, work_iter->workload_,
                                          WORK_GUIDES_COLUMN, work_iter->guides_,
                                          WORK_GUIDES_MAS_DOC_COLUMN, work_iter->guides_mas_doc_,
                                          WORK_MARK_COLUMN, work_iter->mark_,
                                          -1);
        work_iter= work_iter->next_;
    }
}

void access_year_data()
{
    struct basic_per_year_info *year_iter= active_work->per_year_;
    GtkTreeIter iter;
    while (year_iter)
    {
        gtk_list_store_insert_with_values(gueststore, &iter, -1,
                                          YEAR_POINTER_COLUMN, year_iter,
                                          YEAR_ID_COLUMN, year_iter->id_,
                                          YEAR_NUMBER_COLUMN, year_iter->year_num_,
                                          YEAR_COURSE_NAME_COLUMN, year_iter->course_name_,
                                          YEAR_COURSE_TIME_COLUMN, year_iter->course_time_,
                                          YEAR_EXP_TIME_COLUMN, year_iter->exp_time_,
                                          YEAR_STU_NUM_COLUMN, year_iter->stu_num_,
                                          YEAR_CLASS_COLUMN, year_iter->class_,
                                          -1);
        year_iter= year_iter->next_;
    }
}

void create_menu(GtkWidget *win, GtkWidget *menubar)
{
    int i, j, k=0;
    char **strings= (char **)menu_string;
    for (i= 0; i< MENU_NUM; i++)
    {
        GtkWidget *menu_item= gtk_menu_item_new_with_label(*strings++);
        gtk_menu_shell_append((GtkMenuShell *)menubar, menu_item);
        GtkWidget *submenus= gtk_menu_new();
        gtk_menu_item_set_submenu((GtkMenuItem *)menu_item, submenus);
        for (j=0; j< sub_menu_num[i]; j++, k++)
        {
            GtkWidget *submenu= gtk_menu_item_new_with_label(*strings++);
            g_signal_connect(submenu, "activate", G_CALLBACK(*(handle+k)), (gpointer)(win));
            gtk_menu_shell_append(GTK_MENU_SHELL(submenus), submenu);
        }
    }
}

char *choose_file(int type)
{
    GtkWidget *filechooser;
    if (type==OPEN_FILE)
    {
        filechooser=gtk_file_chooser_dialog_new("打开", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
    }
    else if (type== SAVE_FILE)
    {
        filechooser=gtk_file_chooser_dialog_new("新建", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filechooser), TRUE);
    }
    else if (type== SAVE_FILE_AS)
    {
        filechooser=gtk_file_chooser_dialog_new("另存为", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filechooser), TRUE);
    }
    GtkFileFilter *filter= gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.dord");
    gtk_file_filter_set_name(filter, "Dormitory Data Files(*.dord)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), filter);
    if (gtk_dialog_run (GTK_DIALOG (filechooser)) == GTK_RESPONSE_ACCEPT)
    {
        char *tfilename, *filename= malloc(300);
        tfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (filechooser));
        strcpy(filename, tfilename);
        g_free(tfilename);
        if ((type==SAVE_FILE) || (type== SAVE_FILE_AS))
        {
            int len= strlen(filename);
            if ((filename[len-5]!='.') || (filename[len-4]!='d') || (filename[len-3]!='o') || (filename[len-2]!='r') || (filename[len-1]!='d'))
                strcat(filename, ".dord");
        }
        gtk_widget_destroy(filechooser);
        return filename;
    }
    else
    {
        gtk_widget_destroy(filechooser);
        return NULL;
    }

}



//void type_dialogue(int is_edit)
//{
//    GtkWidget *new_type_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    gtk_window_set_transient_for(GTK_WINDOW(new_type_win), GTK_WINDOW(win));
//    gtk_window_set_modal(GTK_WINDOW(new_type_win), TRUE);
//    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_type_win), TRUE);
//
//    gtk_window_set_position (GTK_WINDOW (new_type_win), GTK_WIN_POS_CENTER);
//    if (is_edit)
//    {
//        gtk_window_set_title (GTK_WINDOW (new_type_win), "修改教师信息");
//    }
//    else
//    {
//        gtk_window_set_title (GTK_WINDOW (new_type_win), "添加教师信息");
//    }
//
//    GtkWidget *box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
//    gtk_container_add(GTK_CONTAINER(new_type_win), box1);
//
//    GtkWidget *box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
//    GtkWidget *label1= gtk_label_new("    编号ID：");
//    GtkWidget *entry1= gtk_entry_new();
//    GtkWidget *confirmbutton= gtk_button_new_with_label("确定");
//    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box2), entry1, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box2), confirmbutton, FALSE, TRUE,0);
//
//    GtkWidget *box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
//    GtkWidget *label2= gtk_label_new("最多容纳人数：");
//    GtkWidget *entry2= gtk_entry_new();
//    gtk_entry_set_input_purpose(GTK_ENTRY(entry2), GTK_INPUT_PURPOSE_DIGITS);
//    GtkWidget *cancelbutton= gtk_button_new_with_label("取消");
//    gtk_box_pack_start(GTK_BOX(box3), label2, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box3), entry2, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box3), cancelbutton, FALSE, TRUE,0);
//
//    gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box1), box3, TRUE, TRUE, 0);
//
//    if (is_edit)
//    {
//        gtk_entry_set_text(GTK_ENTRY(entry1), active_type->type_);
//        char most_in[10];
//        sprintf(most_in, "%d", active_type->most_in_);
//        gtk_entry_set_text(GTK_ENTRY(entry2), most_in);
//    }
//
//    struct new_type_data *data=(struct new_type_data *)malloc(sizeof(struct new_type_data));
//    data->entry1= entry1;
//    data->entry2= entry2;
//    if (is_edit)
//        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_edit_type), (gpointer) data);
//    else
//        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_new_type), (gpointer) data);
//    g_signal_connect_after(confirmbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_type_win);
//    g_signal_connect(cancelbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_type_win);
//    gtk_widget_show_all (new_type_win);
//
//};
/**
    Edit
*/
void teacher_dialogue(int is_edit)
{
    GtkWidget *new_teacher_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_teacher_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_teacher_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_teacher_win), TRUE);

    gtk_window_set_position (GTK_WINDOW (new_teacher_win), GTK_WIN_POS_CENTER);
    if (is_edit)
    {
        gtk_window_set_title (GTK_WINDOW (new_teacher_win), "修改教师信息");
    }
    else
    {
        gtk_window_set_title (GTK_WINDOW (new_teacher_win), "添加教师信息");
    }

    GtkWidget *box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(new_teacher_win), box1);

    GtkWidget *box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label1= gtk_label_new("编号ID：");
    GtkWidget *entry1= gtk_entry_new();
    GtkWidget *confirmbutton= gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), entry1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), confirmbutton, FALSE, TRUE,0);

    GtkWidget *box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label2= gtk_label_new("姓  名：");
    GtkWidget *entry2= gtk_entry_new();
    GtkWidget *cancelbutton= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box3), label2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), cancelbutton, FALSE, TRUE,0);

    GtkWidget *box4= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label3= gtk_label_new("单  位：");
    GtkWidget *entry3= gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box4), label3, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box4), entry3, FALSE, TRUE, 0);

    GtkWidget *box5= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label4= gtk_label_new("职  称：");
    GtkWidget *entry4= gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box5), label4, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box5), entry4, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box5, TRUE, TRUE, 0);

    if (is_edit)
    {
        gtk_entry_set_text(GTK_ENTRY(entry1), active_teacher->id_);
        gtk_entry_set_text(GTK_ENTRY(entry2), active_teacher->name_);
        gtk_entry_set_text(GTK_ENTRY(entry3), active_teacher->unit_);
        gtk_entry_set_text(GTK_ENTRY(entry4), active_teacher->title_);
    }

    struct new_teacher_data *data=(struct new_teacher_data *)malloc(sizeof(struct new_teacher_data));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    if (is_edit)
        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_edit_teacher), (gpointer) data);
    else
        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_new_teacher), (gpointer) data);
    g_signal_connect_after(confirmbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_teacher_win);
    g_signal_connect(cancelbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_teacher_win);
    gtk_widget_show_all (new_teacher_win);
};

void work_dialogue(int is_edit, struct room_dialogue_check *checkdata)
{
    GtkWidget *new_work_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_work_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_work_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_work_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_work_win), GTK_WIN_POS_CENTER);

    if (is_edit)
    {
        gtk_window_set_title(GTK_WINDOW(new_work_win), "修改工作量信息");
    }
    else
    {
        gtk_window_set_title(GTK_WINDOW(new_work_win), "添加新工作量信息");
    }

    GtkWidget *grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_grid_insert_row(GTK_GRID(grid), 6);
    gtk_container_add(GTK_CONTAINER(new_work_win), grid);

    GtkWidget *box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    GtkWidget *label1= gtk_label_new("教师：");
    GtkWidget *combo1= gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo1), TEACHER_ID_COLUMN);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1), 0);
    GtkCellRenderer *render= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo1), render, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo1), render, "text", TEACHER_ID_COLUMN, NULL);

    GtkWidget *label2= gtk_label_new("编号ID：");
    GtkWidget *entry1= gtk_entry_new();

    GtkWidget *label3= gtk_label_new("年份：");
    GtkWidget *entry2= gtk_entry_new();

    GtkWidget *label4= gtk_label_new("科研工作量（万元）：");
    GtkWidget *entry3= gtk_entry_new();

    GtkWidget *label5= gtk_label_new("指导毕设（人）：");
    GtkWidget *entry4= gtk_entry_new();


    GtkWidget *label6= gtk_label_new("指导硕/博士（人）：");
    GtkWidget *entry5= gtk_entry_new();


    GtkWidget *label7= gtk_label_new("总分（分）：");
    GtkWidget *entry6= gtk_entry_new();

    if (is_edit)
    {
        char year[20];
        char workload[30];
        char guides[10];
        char guides_mas_doc[10];
        char mark[10];
        sprintf(year, "%d", active_work->year_num_);
        sprintf(workload, "%d", active_work->workload_);
        sprintf(guides, "%d", active_work->guides_);
        sprintf(guides_mas_doc, "%d", active_work->guides_mas_doc_);
        sprintf(mark, "%d", active_work->mark_);
        gtk_entry_set_text(GTK_ENTRY(entry1), active_work->id_);
        gtk_entry_set_text(GTK_ENTRY(entry2), year);
        gtk_entry_set_text(GTK_ENTRY(entry3), workload);
        gtk_entry_set_text(GTK_ENTRY(entry4), guides);
        gtk_entry_set_text(GTK_ENTRY(entry5), guides_mas_doc);
        gtk_entry_set_text(GTK_ENTRY(entry6), mark);
    }

    GtkWidget *button1= gtk_button_new_with_label("确定");
    GtkWidget *button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0 ,3 ,1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label6, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry5, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label7, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry6, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 7, 1, 1);

    struct new_work_data *data=(struct new_work_data *)malloc(sizeof(struct new_work_data));
    data->combo1= combo1;
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->entry5= entry5;
    data->entry6= entry6;

    if (is_edit)
    {
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_work_modify), data);
    }
    else
    {
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_work_new), data);
    }
    if (checkdata)
        g_signal_connect(button1, "clicked", G_CALLBACK(room_dialogue_check), checkdata);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_work_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_work_win);

    gtk_widget_show_all (new_work_win);
}

void year_dialogue(int is_edit, struct guest_dialogue_check *checkdata)
{
    GtkWidget *new_year_win;
    GtkWidget *grid;
    GtkWidget *box1;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *label7;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
    GtkWidget *button1;
    GtkWidget *button2;

    new_year_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_year_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_year_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_year_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_year_win), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(new_year_win), "添加基本工作");

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_grid_insert_row(GTK_GRID(grid), 6);
    gtk_grid_insert_row(GTK_GRID(grid), 7);
    gtk_container_add(GTK_CONTAINER(new_year_win), grid);

    box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    label1= gtk_label_new("编号ID：");
    entry1= gtk_entry_new();

    label2= gtk_label_new("年份：");
    entry2= gtk_entry_new();

    label3= gtk_label_new("课程名：");
    entry3= gtk_entry_new();

    label4= gtk_label_new("授课学时：");
    entry4= gtk_entry_new();


    label5= gtk_label_new("实验学时");
    entry5= gtk_entry_new();


    label6= gtk_label_new("学生人数");
    entry6= gtk_entry_new();


    label7= gtk_label_new("班级");
    entry7= gtk_entry_new();


    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry5, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label6, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry6, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label7, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry7, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 7, 1, 1);

    if(is_edit){

        gtk_entry_set_text(GTK_ENTRY(entry1), active_year->id_);
        gtk_entry_set_text(GTK_ENTRY(entry2), active_year->year_num_);
        gtk_entry_set_text(GTK_ENTRY(entry3), active_year->course_name_);
        char course_time[10];
        char exp_time[10];
        char stu_num[10];
        sprintf(course_time, "%d", active_year->course_time_);
        sprintf(exp_time, "%d", active_year->exp_time_);
        sprintf(stu_num, "%d", active_year->stu_num_);
        gtk_entry_set_text(GTK_ENTRY(entry4), course_time);
        gtk_entry_set_text(GTK_ENTRY(entry5), exp_time);
        gtk_entry_set_text(GTK_ENTRY(entry6), stu_num);
        gtk_entry_set_text(GTK_ENTRY(entry7), active_year->class_);
    }

    struct new_year_data *data=(struct new_year_data *)malloc(sizeof(struct new_year_data));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->entry5= entry5;
    data->entry6= entry6;
    data->entry7= entry7;

    button1= gtk_button_new_with_label("确定");
    button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    if(is_edit){
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_year_modify), data);
    }else{
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_year_new), data);
    }
    if (checkdata)
        g_signal_connect(button1, "clicked", G_CALLBACK(guest_dialogue_check), checkdata);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_year_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_year_win);

    gtk_widget_show_all(new_year_win);
}

void teacher_view_details()
{
    gtk_list_store_clear(roomstore);
    access_work_data();
    char title[25]= "工作量详情:";
    strcat(title, active_teacher->id_);
    gtk_window_set_title (GTK_WINDOW (room_win), title);
    gtk_widget_show_all(room_win);

}

void work_view_detail()
{
    gtk_list_store_clear(gueststore);
    access_year_data();
    char title[25]= "教学工作信息:";
    strcat(title, active_work->id_);
    gtk_window_set_title (GTK_WINDOW (guest_win), title);
    gtk_widget_show_all(guest_win);
}



/****************************************************
maintain data
****************************************************/
struct basic_guest_info *load_guest_data(struct basic_room_data *room)
{
    if (!feof(file))
    {
        struct basic_guest_info *loc_head, *loc_tail;
        loc_head= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
        fread(loc_head, sizeof(struct basic_guest_info), 1, file);
        loc_tail= loc_head;
        loc_tail->room_info_= room;
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
            fread(loc_tail->next_, sizeof(struct basic_guest_info), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->room_info_= room;
        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        return NULL;
    }
};
/**
    Edit
*/
struct basic_year_info *load_year_data(struct basic_work_data *work)
{
    if (!feof(file))
    {
        struct basic_per_year_info *loc_head, *loc_tail;
        loc_head= (struct basic_per_year_info *)malloc(sizeof(struct basic_per_year_info));
        fread(loc_head, sizeof(struct basic_per_year_info), 1, file);
        loc_tail= loc_head;
        loc_tail->work_info_= work;
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_per_year_info *)malloc(sizeof(struct basic_per_year_info));
            fread(loc_tail->next_, sizeof(struct basic_per_year_info), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->work_info_= work;
        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        return NULL;
    }
};

struct basic_room_data *load_room_data(struct basic_room_type_data *type, int *total, int *left)
{
    if (!feof(file))
    {
        struct basic_room_data *loc_head, *loc_tail;
        loc_head= (struct basic_room_data *)malloc(sizeof(struct basic_room_data));
        fread(loc_head, sizeof(struct basic_room_data), 1, file);
        loc_tail= loc_head;
        loc_tail->type_=type;
        (*total)++;
        if (loc_tail->is_left_)
            (*left)++;
        if (loc_tail->guest_)
        {
            loc_tail->guest_= load_guest_data(loc_tail);
            if (!loc_tail->guest_)
                return NULL;
        }
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_room_data *)malloc(sizeof(struct basic_room_data));
            fread(loc_tail->next_, sizeof(struct basic_room_data), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->type_=type;
            (*total)++;
            if (loc_tail->is_left_)
                (*left)++;
            if (loc_tail->guest_)
            {
                loc_tail->guest_= load_guest_data(loc_tail);
                if (!loc_tail->guest_)
                    return NULL;
            }

        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        errmesg("File Damaged.");
        return NULL;
    }
};
/**
    Edit
*/
struct basic_work_data *load_work_data(struct basic_teacher_data *teacher)
{
    if (!feof(file))
    {
        struct basic_work_data *loc_head, *loc_tail;
        loc_head= (struct basic_work_data *)malloc(sizeof(struct basic_work_data));
        fread(loc_head, sizeof(struct basic_work_data), 1, file);
        loc_tail= loc_head;
        loc_tail->teacher_= teacher;

        if (loc_tail->per_year_)
        {
            loc_tail->per_year_= load_year_data(loc_tail);
            if (!loc_tail->per_year_)
                return NULL;
        }
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_work_data *)malloc(sizeof(struct basic_work_data));
            fread(loc_tail->next_, sizeof(struct basic_work_data), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->teacher_= teacher;
            if (loc_tail->per_year_)
            {
                loc_tail->per_year_= load_year_data(loc_tail);
                if (!loc_tail->per_year_)
                    return NULL;
            }

        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        errmesg("File Damaged.");
        return NULL;
    }
};

struct basic_teacher_data *load_all_data()
{
    if (file)
    {

        if (!feof(file))
        {
            struct basic_teacher_data *loc_head, *loc_tail;
            loc_head= (struct basic_teacher_data *)malloc(sizeof(struct basic_teacher_data));
            memset(loc_head, 0, sizeof(struct basic_teacher_data));
            if (!fread(loc_head, sizeof(struct basic_teacher_data), 1, file))
            {
                free(loc_head);
                return NULL;
            }
            loc_tail= loc_head;
            if (loc_tail->work_head_)
            {
                loc_tail->work_head_= load_work_data(loc_tail);
                if (!loc_tail->work_head_)
                    return NULL;
            }
            while ((loc_tail->next_) && !feof(file))
            {
                loc_tail->next_= (struct basic_teacher_data *)malloc(sizeof(struct basic_teacher_data));
                fread(loc_tail->next_, sizeof(struct basic_teacher_data), 1, file);
                loc_tail= loc_tail->next_;
                if (loc_tail->work_head_)
                {
                    loc_tail->work_head_= load_work_data(loc_tail);
                    if (!loc_tail->work_head_)
                        return NULL;
                }

            }
            if (loc_tail->next_)
                return NULL;
            else
                return loc_head;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
};

void save_data()
{
    if (!file)
    {
        errmesg("No File Opened.");
    }
    else
    {
        rewind(file);
        struct basic_teacher_data *teacher_itor= teacher_head;
        while (teacher_itor)
        {
            fwrite(teacher_itor, sizeof(struct basic_teacher_data), 1, file);
            struct basic_work_data *work_itor= teacher_itor->work_head_;
            while (work_itor)
            {
                fwrite(work_itor, sizeof(struct basic_work_data), 1, file);
                struct basic_per_year_info *year_itor= work_itor->per_year_;
                while (year_itor)
                {
                    fwrite(year_itor, sizeof(struct basic_per_year_info), 1, file);
                    year_itor= year_itor->next_;
                }
                work_itor= work_itor->next_;
            }
            teacher_itor= teacher_itor->next_;
        }
        edited= 0;
    }
}

void type_insert(const char *type, int most_in)
{
    struct basic_room_type_data *tmp= (struct basic_room_type_data *)malloc(sizeof(struct basic_room_type_data));
    strcpy(tmp->type_, type);
    tmp->most_in_= most_in;
    tmp->total_= 0;
    tmp->left_= 0;
    tmp->room_head_=NULL;
    tmp->next_= type_head;
    type_head= tmp;
}
//Edit
void teacher_insert(const char *id, const char *name, const char *unit, const char *title)
{
    struct basic_teacher_data *tmp= (struct basic_teacher_data *)malloc(sizeof(struct basic_teacher_data));
    strcpy(tmp->id_, id);
    strcpy(tmp->name_, name);
    strcpy(tmp->unit_, unit);
    strcpy(tmp->title_, title);
    tmp->work_head_=NULL;
    tmp->next_= teacher_head;
    teacher_head= tmp;
}


//void type_modify(const char *type, int most_in)
//{
//    strcpy(active_teacher->type_, type);
//    active_type->most_in_= most_in;
//}
/**
    Edit
*/
void teacher_modify(const char *id, const char *name, const char *unit, const char *title)
{
    strcpy(active_teacher->id_, id);
    strcpy(active_teacher->name_, name);
    strcpy(active_teacher->unit_, unit);
    strcpy(active_teacher->title_, title);
}

//void type_delete()
//{
//    struct basic_room_type_data *iter= type_head;
//    struct basic_room_data *room_iter= active_type->room_head_;
//    while (room_iter)
//    {
//        struct basic_guest_info *guest_iter= room_iter->guest_;
//        while (guest_iter)
//        {
//            struct basic_guest_info *tmp= guest_iter;
//            guest_iter= guest_iter->next_;
//            free(tmp);
//        }
//        struct basic_room_data *tmp= room_iter;
//        room_iter= room_iter->next_;
//        free(tmp);
//    };
//    if (iter== active_type)
//    {
//        type_head= type_head->next_;
//        free(iter);
//    }
//    else
//    {
//        for (; iter->next_!= active_type; iter= iter->next_)
//            ;
//        iter->next_= active_type->next_;
//        free(active_type);
//    }
//
//}
//Edit
void teacher_delete()
{
    struct basic_teacher_data *iter= teacher_head;
    struct basic_work_data *work_iter= active_teacher->work_head_;
    while (work_iter)
    {
        struct basic_per_year_info *year_iter= work_iter->per_year_;
        while (year_iter)
        {
            struct basic_guest_info *tmp= year_iter;
            year_iter= year_iter->next_;
            free(tmp);
        }
        struct basic_work_data *tmp= work_iter;
        work_iter= work_iter->next_;
        free(tmp);
    };
    if (iter== active_teacher)
    {
        teacher_head= teacher_head->next_;
        free(iter);
    }
    else
    {
        for (; iter->next_!= active_teacher; iter= iter->next_)
            ;
        iter->next_= active_teacher->next_;
        free(active_teacher);
    }

}

//void room_insert(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left)
//{
//    struct basic_room_data *tmp=malloc(sizeof(struct basic_room_data));
//    tmp->type_= type;
//    strcpy(tmp->room_number_, room_number);
//    strcpy(tmp->phone_number_, phone_number);
//    tmp->area_= area;
//    tmp->price_= price;
//    tmp->is_left_= is_left;
//    tmp->guest_= NULL;
//    tmp->next_= type->room_head_;
//    type->room_head_= tmp;
//    type->total_++;
//    if (is_left)
//        type->left_++;
//
//}
//Edit
void work_insert(struct basic_teacher_data *teacher, const char *id, int year, int workload, int guides, int guides_mas_doc, int mark)
{
    struct basic_work_data *tmp=malloc(sizeof(struct basic_work_data));
    tmp->teacher_= teacher;
    strcpy(tmp->id_, id);
    tmp->year_num_= year;
    tmp->workload_= workload;
    tmp->guides_= guides;
    tmp->guides_mas_doc_= guides_mas_doc;
    tmp->mark_= mark;
    tmp->per_year_= NULL;
    tmp->next_= teacher->work_head_;
    teacher->work_head_= tmp;
}

//void room_modify(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left)
//{
//    if (type== active_type)
//    {
//        active_room->area_= area;
//        strcpy(active_room->phone_number_, phone_number);
//        strcpy(active_room->room_number_, room_number);
//        active_room->price_= price;
//        if (active_room->is_left_> is_left)
//            active_type->left_--;
//        else if (active_room->is_left_ < is_left)
//            active_type->left_++;
//        active_room->is_left_= is_left;
//    }
//    else
//    {
//        room_delete();
//        room_insert(type, room_number, phone_number, area, price, is_left);
//    }
//}
//Edit
void work_modify(struct basic_teacher_data *teacher, const char *id, int year, int workload, int guides, int guides_mas_doc, int mark)
{
    if (teacher== active_teacher)
    {
        strcpy(active_work->id_, id);
        active_work->year_num_= year;
        active_work->workload_= workload;
        active_work->guides_= guides;
        active_work->guides_mas_doc_= guides_mas_doc;
        active_work->mark_= mark;
    }
    else
    {
        work_delete();
        work_insert(teacher, id, year, workload, guides, guides_mas_doc, mark);
    }
}

void work_delete()
{
    struct basic_per_year_info *year_iter= active_work->per_year_;
    while (year_iter)
    {
        struct basic_per_year_info *tmp= year_iter;
        year_iter= year_iter->next_;
        free(tmp);
    }
    if (active_teacher->work_head_== active_work)
    {
        active_teacher->work_head_= active_work->next_;
        free(active_work);
    }
    else
    {
        struct basic_work_data *work_iter= active_teacher->work_head_;
        while (work_iter->next_!= active_work)
        {
            work_iter= work_iter->next_;
        }
        work_iter->next_= active_work->next_;
        free(active_work);
    }

}

void year_insert(struct basic_work_data *work, const char *id, const char *year, const char *course_name, int course_time, int exp_time, int stu_num, const char *class_name)
{
    struct basic_per_year_info *tmp= (struct basic_per_year_info *)malloc(sizeof(struct basic_per_year_info));
    tmp->work_info_= work;
    strcpy(tmp->id_, id);
    strcpy(tmp->year_num_, year);
    strcpy(tmp->course_name_, course_name);
    tmp->course_time_= course_time;
    tmp->exp_time_= exp_time;
    tmp->stu_num_= stu_num;
    strcpy(tmp->class_, class_name);
    tmp->next_= work->per_year_;
    work->per_year_= tmp;
}

void year_modify(struct basic_work_data *work, const char *id, const char *year, const char *course_name, int course_time, int exp_time, int stu_num, const char *class_name)
{
    if (work== active_work)
    {
        strcpy(active_year->id_, id);
        strcpy(active_year->year_num_, year);
        strcpy(active_year->course_name_, course_name);
        active_year->course_time_= course_time;
        active_year->exp_time_= exp_time;
        active_year->stu_num_= stu_num;
        strcpy(active_year->class_, class_name);
    }
    else
    {
        year_delete();
        year_insert(work, id, year, course_name, course_time, exp_time, stu_num, class_name);
    }
}

void year_delete()
{
    if (active_work->per_year_ == active_year)
    {
        active_work->per_year_= active_year->next_;
        free(active_year);
    }
    else
    {
        struct basic_per_year_info *year_iter= active_year->work_info_->per_year_;
        while (year_iter->next_!= active_year)
            year_iter=year_iter->next_;
        year_iter->next_= active_year->next_;
        free(active_year);
    }

}
/****************************************************
query
****************************************************/
void q_work_by_conditions(GtkListStore *store,  int is_id_limited, int is_name_limited, int is_teacher_limited, int is_year_limited, int is_workload_limited, struct basic_teacher_data *teacher, const char *id, const char *name, int uyear, int lyear, int uworkload, int lworkload)
{
    struct basic_teacher_data *teacher_iter= teacher_head;
    struct basic_work_data *work_iter;
    GtkTreeIter iter;
    gtk_list_store_clear(store);
    while (teacher_iter)
    {
        if (!is_teacher_limited || (teacher_iter== teacher))
        {
            work_iter= teacher_iter->work_head_;
            while (work_iter)
            {
                if ((((!is_id_limited)||!strcmp(id, work_iter->id_)) && ((!is_name_limited)||!strcmp(name, work_iter->teacher_->name_))) && ((!is_year_limited)||((work_iter->year_num_ <= uyear) && (work_iter->year_num_>= lyear))) && ((!is_workload_limited)||((work_iter->workload_ <= uworkload) && (work_iter->workload_>= lworkload))))
                    gtk_list_store_insert_with_values(store, &iter, -1,
                                                      0, work_iter,
                                                      1, work_iter->id_,
                                                      2, work_iter->teacher_->name_,
                                                      3, work_iter->year_num_,
                                                      4, work_iter->workload_,
                                                      5, work_iter->teacher_->id_,
                                                      -1);
                work_iter= work_iter->next_;
            }
        }
        teacher_iter= teacher_iter->next_;
    }
}

void q_guest_info_by_conditions(GtkListStore *store, int is_id_limited, int is_name_limited, int is_in_time_limited, const char *id, const char *name_piece, const char *lin_time, const char *uin_time)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    struct basic_guest_info *guest_iter;
    GtkTreeIter iter;
    gtk_list_store_clear(store);
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            guest_iter= room_iter->guest_;
            while (guest_iter)
            {
                if (((!is_id_limited)||(!strcmp(id, guest_iter->id_))) && ((!is_name_limited) || (strstr(guest_iter->name_, name_piece))) && ((!is_in_time_limited) || ((cal_day(lin_time, guest_iter->in_time_)>=0) && (cal_day(uin_time, guest_iter->in_time_) <=0))))
                    gtk_list_store_insert_with_values(store, &iter, -1,
                                                      GUEST_POINTER_COLUMN, guest_iter,
                                                      GUEST_ID_COLUMN, guest_iter->id_,
                                                      GUEST_NAME_COLUMN, guest_iter->name_,
                                                      GUEST_IN_TIME_COLUMN, guest_iter->in_time_,
                                                      GUEST_OUT_TIME_COLUMN, (guest_iter->out_time_[0]==0)?"在住":guest_iter->out_time_,
                                                      GUEST_DAY_SPENT_COLUMN, guest_iter->day_spent_,
                                                      GUEST_FARE_COLUMN, guest_iter->fare_,
                                                      GUEST_PAID_COLUMN, guest_iter->pay_,
                                                      GUEST_COLUMN_NUMBER, guest_iter->room_info_->room_number_,
                                                      -1);
                guest_iter= guest_iter->next_;
            }
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
}
/*****************************************************
Assistant
*****************************************************/
int get_year()
{
    struct tm *current;
    time_t timenow;
    time(&timenow);
    current = localtime(&timenow);
    return current->tm_year+1900;
}

int get_hour(const char timech[])
{
    const char * iter= timech;
    while (*iter++!= '-')
        ;
    int hour= 0;
    while (*iter!= ':')
    {
        hour*= 10;
        hour+= *iter- '0';
        iter++;
    }
    return hour;
}

int is_year(int year)
{
    return (!(year % 4) && (year % 100))|| !(year % 400);
}

int day_of_month(int year, int month)
{
    static const int daymonth[13]= {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((month==2) && is_year(year))
        return daymonth[month]+1;
    else
        return  daymonth[month];
}

int day_of_year(int year)
{
    return is_year(year)?366:365;
}

int date_2_day(int year, int month, int day)
{
    int result= 0;
    result+= day;
    result+= day_since[month];
    result+=is_year(year);
    return result;
}

int get_num(const char **chs, char ch)
{
    int result= 0;
    while (**chs!= ch)
    {
        result*= 10;
        result+= **chs - '0';
        (*chs)++;
    }
    (*chs)++;
    return result;
}

float cal_day(const char time1[], const char time2[])
{
    int year1= 0, year2= 0, month1= 0, month2= 0, day1= 0, day2= 0;
    const char *ch1= time1, *ch2= time2;

    year1= get_num(&ch1, '/');
    month1= get_num(&ch1, '/');
    day1= get_num(&ch1, '-');
    if (*ch2 == 0)
    {
        time_t now;
        time(&now);
        char stime[50];
        strftime(stime, 50, "%Y/%m/%d-%H:%M", localtime(&now));
        ch2= stime;
        year2= get_num(&ch2, '/');
        month2= get_num(&ch2, '/');
        day2= get_num(&ch2, '-');
    }
    else
    {
        year2= get_num(&ch2, '/');
        month2= get_num(&ch2, '/');
        day2= get_num(&ch2, '-');
    }

    float result=0;
    if (year1!= year2)
    {
        int i;
        result+= day_of_year(year1)- date_2_day(year1, month1, day1);
        for (i= year1+1; i< year2; i++)
            result+= day_of_year(i);
        result+=date_2_day(year2, month2, day2);

    }
    else
    {
        result= date_2_day(year2, month2, day2)- date_2_day(year1, month1, day1);
    }

    if (get_hour(time2) >= 18)
        result++;
    else if (get_hour(time2) >= 12)
        result+= 0.5;

    return result;
}

int cal_types()
{
    int result= 0;
    struct basic_room_type_data *type_iter= type_head;
    while (type_iter)
    {
        result++;
        type_iter= type_iter->next_;
    }
    return result;
}

void fill_type(int types, GType *type_list)
{
    int i;
    *type_list= G_TYPE_STRING;
    for (i=1; i<= types; i++)
        type_list[i]= G_TYPE_FLOAT;
}

float cal_guest(struct basic_guest_info *guest_iter, int year, int month)
{
    const char *in_time= guest_iter->in_time_, *out_time= guest_iter->out_time_;
    if (out_time[0]== 0)
    {
        guest_iter= guest_iter->next_;
        return 0;
    }
    int in_year= get_num(&in_time, '/'), in_month= get_num(&in_time, '/'), in_day= get_num(&in_time, '-');
    int out_year= get_num(&out_time, '/'), out_month= get_num(&out_time, '/'), out_day= get_num(&out_time, '-');
    float day= 0;
    if (((in_year> year) || ((in_year== year) && (in_month> month)))||((out_year< year) || ((out_year== year) && (out_month< month))))
    {
        guest_iter= guest_iter->next_;
        return 0;
    }
    int out_hour= get_num(&out_time, ':');
    if ((in_year< year) || ((in_year== year) && (in_month< month)))
    {
        in_year= year;
        in_month= month;
        in_day= 1;
    }
    else
        day-=1;
    if ((out_year> year) || ((out_year== year) && (out_month> month)))
    {
        out_year= year;
        out_month= month;
        out_day= day_of_month(year, month);
        out_hour=0;
    }
    day+= out_day-in_day+1;
    if (out_hour>=12)
        day+=0.5;
    if (out_hour>= 18)
        day+=0.5;
    return day;
}

float cal_room(struct basic_room_data *room_iter, int year, int month)
{
    struct basic_guest_info *guest_iter= room_iter->guest_;
    float result= 0;
    while (guest_iter)
    {
        result+= guest_iter->pay_* cal_guest(guest_iter, year, month)/ cal_day(guest_iter->in_time_, guest_iter->out_time_);
        guest_iter= guest_iter->next_;
    }
    return result;
}

float cal_value(struct basic_room_type_data *type, int year, int month)
{
    struct basic_room_data *room_iter= type->room_head_;
    float result= 0;
    while (room_iter)
    {
        result+= cal_room(room_iter, year, month);
        room_iter= room_iter->next_;
    }
    return result;
}

void cal_values(int types, int year, int month, int *column_num, GValue *value, float *total)
{
    g_value_init(value, G_TYPE_STRING);
    char smonth[3];
    sprintf(smonth, "%d", month);
    g_value_set_string(value, smonth);
    column_num[0]=0;
    struct basic_room_type_data *type_iter= type_head;
    int i;
    for (i=1; i<= types; i++, type_iter= type_iter->next_)
    {
        column_num[i]= i;
        g_value_init(value+i, G_TYPE_FLOAT);
        float v= cal_value(type_iter, year, month);
        total[i]+= v;
        g_value_set_float(value+i, v);
    }
}

void cal_room_sta(GtkListStore *store, struct basic_room_data *room, int year, float *total_w, float *total_day)
{
    struct basic_guest_info *guest_iter= room->guest_;
    float w= 0, in_time= 0, in_time_real= 0;
    while (guest_iter)
    {
        int i;
        for (i=1; i<=12; i++)
        {
            float stime= cal_guest(guest_iter, year, i);
            in_time+= stime;
            w+= guest_iter->pay_ * stime / cal_day(guest_iter->in_time_, guest_iter->out_time_);
        }
        guest_iter= guest_iter->next_;
    }
    in_time_real= in_time / day_of_year(year);
    GtkTreeIter iter;
    gtk_list_store_insert_with_values(store, &iter, -1,
                                      STA_ROOM_NUMBER_COLUMN, room->room_number_,
                                      STA_ROOM_TYPE_COLUMN, room->type_,
                                      STA_W_COLUMN, w,
                                      STA_IN_TIME_COLUME, in_time,
                                      STA_IN_REAL_CULUME, in_time_real,
                                      -1);
    *total_w+= w;
    *total_day+= in_time;
}

void cal_rooms_sta(GtkListStore *store, int year)
{
    cal_total_workload_sta(NULL);
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    float total_w= 0, total_day= 0;
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            cal_room_sta(store, room_iter, year, &total_w, &total_day);
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    GtkTreeIter iter;
    gtk_list_store_insert_with_values(store, &iter, -1,
                                      STA_ROOM_NUMBER_COLUMN, "总计",
                                      STA_W_COLUMN, total_w,
                                      STA_IN_TIME_COLUME, total_day,
                                      STA_IN_REAL_CULUME, total_day/day_of_year(year),
                                      -1);
}

struct teacher_total_workload *workloads_sort(struct teacher_total_workload *workloads_list)
{
    if ((!workloads_list) || (!workloads_list->next_))
        return workloads_list;
    struct teacher_total_workload *tmph= (struct teacher_total_workload*) malloc(sizeof(struct teacher_total_workload)), *max, *itor, *now1, *pre1;
    tmph->next_= workloads_list;
    for (pre1=tmph, now1= workloads_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->total_workload_ > max->next_->total_workload_)
                max= itor;
        if (now1==max)
        {
            struct teacher_total_workload *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct teacher_total_workload *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct teacher_total_workload *result= tmph->next_;
    free(tmph);
    return result;
}

struct teacher_this_year_time *year_time_sort(struct teacher_this_year_time *year_time_list)
{
    if ((!year_time_list) || (!year_time_list->next_))
        return year_time_list;
    struct teacher_this_year_time *tmph= (struct teacher_this_year_time*) malloc(sizeof(struct teacher_this_year_time)), *max, *itor, *now1, *pre1;
    tmph->next_= year_time_list;
    for (pre1=tmph, now1= year_time_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->this_year_time_ > max->next_->this_year_time_)
                max= itor;
        if (now1==max)
        {
            struct teacher_this_year_time *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct teacher_this_year_time *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct teacher_this_year_time *result= tmph->next_;
    free(tmph);
    return result;
}

void cal_total_workload_sta(GtkListStore *store)
{
    struct basic_teacher_data *teacher_iter= teacher_head;
    struct basic_work_data *work_iter;
    struct teacher_total_workload *total_workload_iter= (struct teacher_total_workload*)malloc(sizeof(struct teacher_total_workload));
    struct teacher_total_workload *total_workload_head= total_workload_iter;

    while(teacher_iter)
    {
        total_workload_iter->teacher_= teacher_iter;
        total_workload_iter->total_workload_= 0;
        total_workload_iter->next_= (struct teacher_total_workload*)malloc(sizeof(struct teacher_total_workload));
        work_iter= teacher_iter->work_head_;
        while(work_iter)
        {
            total_workload_iter->total_workload_ += work_iter->workload_;
            work_iter= work_iter->next_;
        }
        if(teacher_iter->next_)
            total_workload_iter= total_workload_iter->next_;
        teacher_iter= teacher_iter->next_;
    }
    total_workload_iter->next_= NULL;

    total_workload_iter= workloads_sort(total_workload_head);

    while(total_workload_iter)
    {
        GtkTreeIter iter;
        gtk_list_store_insert_with_values(store, &iter, -1,
                                          STA_WORKLOADS_NAME_COLUMN, total_workload_iter->teacher_->name_,
                                          STA_WORKLOADS_WORKLOADS_COLUMN, total_workload_iter->total_workload_,
                                          -1);
        total_workload_iter= total_workload_iter->next_;
    }

}

void cal_this_year_time_sta(GtkListStore *store1)
{
    int time_under_64= 0;
    int curr_year=get_year();
    struct basic_teacher_data *teacher_iter= teacher_head;
    struct basic_work_data *work_iter;
    struct basic_per_year_info *year_iter;
    struct teacher_this_year_time *this_year_time_iter= (struct teacher_this_year_time*)malloc(sizeof(struct teacher_this_year_time));
    struct teacher_this_year_time *this_year_time_head= this_year_time_iter;

    while(teacher_iter)
    {
        this_year_time_iter->teacher_= teacher_iter;
        this_year_time_iter->this_year_time_= 0;
        this_year_time_iter->next_= (struct teacher_this_year_time*)malloc(sizeof(struct teacher_this_year_time));
        work_iter= teacher_iter->work_head_;
        while(work_iter && work_iter->year_num_ == curr_year)
        {
            year_iter= work_iter->per_year_;
            while(year_iter)
            {
                this_year_time_iter->this_year_time_ += year_iter->course_time_ + year_iter->exp_time_;
                year_iter= year_iter->next_;
            }
            work_iter= work_iter->next_;
        }
        if(teacher_iter->next_)
            this_year_time_iter= this_year_time_iter->next_;
        teacher_iter= teacher_iter->next_;
    }
    this_year_time_iter->next_= NULL;

    this_year_time_iter= year_time_sort(this_year_time_head);
    this_year_time_head = this_year_time_iter;
    while(this_year_time_iter)
    {
        GtkTreeIter iter;
        gtk_list_store_insert_with_values(store1, &iter, -1,
                                          STA_TIME_NAME_COLUMN, this_year_time_iter->teacher_->name_,
                                          STA_TIME_TIME_COLUMN, this_year_time_iter->this_year_time_,
                                          -1);
        this_year_time_iter= this_year_time_iter->next_;
    }

    this_year_time_iter= this_year_time_head;
    while(this_year_time_iter)
    {
        if(this_year_time_iter->this_year_time_ < 64)
            time_under_64++;
        this_year_time_iter= this_year_time_iter->next_;
    }

    GtkTreeIter iter;
    gtk_list_store_insert_with_values(store1, &iter, -1,
                                      STA_TIME_NAME_COLUMN, "未达64学时教师人数：",
                                      STA_TIME_TIME_COLUMN, time_under_64,
                                      -1);

    this_year_time_iter= this_year_time_head;
    while(this_year_time_iter)
    {
        GtkTreeIter iter;
        if(this_year_time_iter->this_year_time_ < 64)
            gtk_list_store_insert_with_values(store1, &iter, -1,
                                              STA_TIME_NAME_COLUMN, this_year_time_iter->teacher_->name_,
                                              STA_TIME_TIME_COLUMN, this_year_time_iter->this_year_time_,
                                              -1);
        this_year_time_iter= this_year_time_iter->next_;
    }
}

struct basic_guest_info *ssort(struct basic_guest_info *guest_list)
{
    if ((!guest_list) || (!guest_list->next_))
        return guest_list;
    struct basic_guest_info *tmph= (struct basic_guest_info*) malloc(sizeof(struct basic_guest_info)), *max, *itor, *now1, *pre1;
    tmph->next_= guest_list;
    for (pre1=tmph, now1= guest_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->pay_> max->next_->pay_)
                max= itor;
        if (now1==max)
        {
            struct basic_guest_info *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct basic_guest_info *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct basic_guest_info *result= tmph->next_;
    free(tmph);
    return result;
}

void find_insert(struct basic_guest_info *guest, struct basic_guest_info **list_head)
{
    struct basic_guest_info *list= *list_head;
    while (list)
    {
        if (!strcmp(guest->id_, list->id_))
        {
            list->day_spent_+= guest->day_spent_;
            list->fare_+= guest->fare_;
            list->pay_+= guest->pay_;
            break;
        }
        list= list->next_;
    }
    if (!list)
    {
        struct basic_guest_info *tmp= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
        *tmp= *guest;
        tmp->next_= *list_head;
        *list_head= tmp;
    }
}

void cal_top_10_guest(GtkListStore *store)
{
    struct basic_guest_info *result_list= NULL;
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    struct basic_guest_info *guest_iter;

    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            guest_iter= room_iter->guest_;
            while (guest_iter)
            {
                find_insert(guest_iter, &result_list);
                guest_iter= guest_iter->next_;
            }
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    result_list= ssort(result_list);
    int i=0;
    GtkTreeIter iter;
    while ((i<10) && (result_list))
    {
        gtk_list_store_insert_with_values(store, &iter, -1,
                                          STA_GUEST_RANK_COLUMN, i+1,
                                          STA_GUEST_ID_COLUMN, result_list->id_,
                                          STA_GUEST_NAME_COLUMN, result_list->name_,
                                          STA_GUEST_IN_TIME_COLUMN, result_list->day_spent_,
                                          STA_GUEST_FARE_COLUMN, result_list->fare_,
                                          STA_GUEST_PAID_COLUMN, result_list->pay_,
                                          STA_DISCOUNT_COLUMN, result_list->pay_/ result_list->fare_,
                                          -1);
        struct basic_guest_info *tmp =result_list;
        result_list= result_list->next_;
        free(tmp);
        i++;
    }
}

void cal_room_full(struct basic_room_data *room, struct room_interest *interest)
{
    struct basic_guest_info *guest_iter= room->guest_;
    interest->fare_= 0;
    interest->paid_ =0;
    strcpy(interest->room_number_, room->room_number_);
    interest->room_type_= room->type_->type_;
    while (guest_iter)
    {
        interest->fare_+= guest_iter->fare_;
        interest->paid_+= guest_iter->pay_;
        guest_iter= guest_iter->next_;
    }
}

struct room_interest *ssort2(struct room_interest *room_list)
{
    if ((!room_list) || (!room_list->next_))
        return room_list;
    struct room_interest *tmph= (struct room_interest*) malloc(sizeof(struct room_interest)), *max, *itor, *now1, *pre1;
    tmph->next_= room_list;
    for (pre1=tmph, now1= room_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->paid_> max->next_->paid_)
                max= itor;
        if (now1==max)
        {
            struct room_interest *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct room_interest *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct room_interest *result= tmph->next_;
    free(tmph);
    return result;
}


void cal_top_room(GtkListStore *store)
{
    struct room_interest *list_head= NULL;
    struct basic_room_type_data *type_iter;
    struct basic_room_data *room_iter;

    type_iter= type_head;
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            struct room_interest *tmp= (struct room_interest *)malloc(sizeof(struct room_interest));
            cal_room_full(room_iter, tmp);
            tmp->next_= list_head;
            list_head= tmp;
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }

    list_head= ssort2(list_head);

    int i= 1;
    while (list_head)
    {
        GtkTreeIter iter;
        gtk_list_store_insert_with_values(store, &iter, -1,
                                          STA_ROOM_RANK_COLUMN, i,
                                          STA_ROOM_NUMBER_COLUMN2, list_head->room_number_,
                                          STA_ROOM_TYPE_COLUMN2, list_head->room_type_,
                                          STA_ROOM_FARE_COLUMN, list_head->fare_,
                                          STA_ROOM_PAID_COLUMN, list_head->paid_,
                                          STA_ROOM_DISCOUNT_COLUMN, list_head->fare_?list_head->paid_/list_head->fare_:0,
                                          -1);
        struct room_interest *tmp= list_head;
        list_head= list_head->next_;
        free(tmp);
        i++;
    }
}
