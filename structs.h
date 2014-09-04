#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include <gtk/gtk.h>

struct basic_guest_info;
/**
    Edit
*/
struct basic_per_year_info;

struct basic_room_type_data;
/**
    Edit
*/
struct basic_teacher_data;

struct basic_room_data
{
    char room_number_[6];
    char phone_number_[20];
    struct basic_room_type_data *type_;
    float area_;
    float price_;
    char is_left_;
    struct basic_guest_info *guest_;
    struct basic_room_data *next_;
};
/**
    Edit
*/
struct basic_work_data
{
    char id_[12];
    struct basic_teacher_data *teacher_;
    int year_num_;
    int workload_;
    int guides_;
    int guides_mas_doc_;
    int mark_;
    struct basic_per_year_info *per_year_;
    struct basic_work_data *next_;
};

struct teacher_total_workload
{
    int total_workload_;
    struct basic_teacher_data *teacher_;
    struct teacher_total_workload *next_;
};

struct teacher_this_year_time
{
    int this_year_time_;
    struct basic_teacher_data *teacher_;
    struct teacher_this_year_time *next_;
};

struct room_interest
{
    char room_number_[6];
    char *room_type_;
    float fare_;
    float paid_;
    struct room_interest *next_;
};

struct basic_room_type_data
{
    char type_[20];
    int most_in_;
    int total_;
    int left_;
    struct basic_room_data *room_head_;
    struct basic_room_type_data *next_;
};
/**
    Edit
*/
struct basic_teacher_data
{
    char id_[12];
    char name_[20];
    char unit_[30];
    char title_[30];
    struct basic_work_data *work_head_;
    struct basic_teacher_data *next_;
};

struct basic_guest_info
{
    char id_[20];
    char name_[20];
    struct basic_room_data *room_info_;
    char in_time_[18];
    char out_time_[18];
    float day_spent_;
    float fare_;
    float pay_;
    struct basic_guest_info *next_;
};
/**
    Edit
*/
struct basic_per_year_info
{
    char id_[12];
    char year_num_[12];
    char course_name_[12];
    char class_[15];
    struct basic_work_data *work_info_;
    int course_time_;
    int exp_time_;
    int stu_num_;
    struct basic_per_year_info *next_;
};

struct popup_menu
{
    GtkWidget *menu;
    GtkTreeSelection *selection;
    GtkTreeView *treeview;
    void **active;
};

struct room_dialogue_check
{
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
};

struct guest_dialogue_check
{
    GtkWidget *button1;
    GtkWidget *button2;
};

struct new_type_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
};
/**
    Edit
*/
struct new_teacher_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
};

struct new_room_data
{
    GtkWidget *combo1;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton;
};
//Edit
struct new_work_data
{
    GtkWidget *combo1;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
};

//struct new_guest_in_data
//{
//    GtkWidget *entry1;
//    GtkWidget *entry2;
//    GtkWidget *entry3;
//    GtkWidget *entry4;
//};
//Edit
struct new_year_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
};

struct new_guest_out_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
};

struct new_guest_data
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
};

struct query_room
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
    GtkWidget *combo1;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkWidget *checkbutton4;
    GtkWidget *checkbutton5;
    GtkWidget *checkbutton6;
    GtkWidget *checkbutton7;
    GtkListStore *store;
};

struct query_guest
{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkListStore *store;
};
#endif // STRUCTS_H_INCLUDED
