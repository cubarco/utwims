#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#define MENU_NUM 5

const char *version= "0.0.0";

const char *url= "https://github.com/cubarco/utwims";

const char *auther[]= {"Jiajun Lin(cubarco)", NULL};

const int sub_menu_num[MENU_NUM]= {6, 2, 1, 2, 2};

const int day_since[13]={0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

const char * const menu_string[]=
{
    "文件", "新建", "打开", "保存", "另存为", "关闭", "退出",
    "编辑", "新教师", "新工作量",
    "查询", "工作量查询",
    "统计", "工作量统计", "教学总学时统计",
    "帮助", "关于", "帮助"
};


enum
{
    OPEN_FILE,
    SAVE_FILE,
    SAVE_FILE_AS
};

//enum
//{
//    POINTER_COLUMN,
//    TYPE_COLUMN,
//    MOST_IN_COLUMN,
//    TOTAL_COLUMN,
//    LEFT_COLUMN,
////    IN_COLUMN,
//    COLUMN_NUMBER
//};
/**
    Edit
*/
enum
{
    TEACHER_POINTER_COLUMN,
    TEACHER_ID_COLUMN,
    TEACHER_NAME_COLUMN,
    TEACHER_UNIT_COLUMN,
    TEACHER_TITLE_COLUMN,
    TEACHER_COLUMN_NUMBER
};

enum
{
    ROOM_POINTER_COLUMN,
    ROOM_NUMBER_COLUMN,
    PHONE_NUMBER_COLUMN,
    AREA_COLUMN,
    PRICE_COLUMN,
    IS_LIFT_COLUMN,
    ROOM_COLUMN_NUMBER
};
//Edit
enum
{
    WORK_POINTER_COLUMN,
    WORK_ID_COLUMN,
    WORK_YEAR_COLUMN,
    WORK_WORKLOAD_COLUMN,
    WORK_GUIDES_COLUMN,
    WORK_GUIDES_MAS_DOC_COLUMN,
    WORK_MARK_COLUMN,
    WORK_COLUMN_NUMBER
};

enum
{
    GUEST_POINTER_COLUMN,
    GUEST_ID_COLUMN,
    GUEST_NAME_COLUMN,
    GUEST_IN_TIME_COLUMN,
    GUEST_OUT_TIME_COLUMN,
    GUEST_DAY_SPENT_COLUMN,
    GUEST_FARE_COLUMN,
    GUEST_PAID_COLUMN,
    GUEST_COLUMN_NUMBER
};
//Edit
enum
{
    YEAR_POINTER_COLUMN,
    YEAR_ID_COLUMN,
    YEAR_NUMBER_COLUMN,
    YEAR_COURSE_NAME_COLUMN,
    YEAR_COURSE_TIME_COLUMN,
    YEAR_EXP_TIME_COLUMN,
    YEAR_STU_NUM_COLUMN,
    YEAR_CLASS_COLUMN,
    YEAR_COLUMN_NUMBER
};

enum
{
    STA_ROOM_NUMBER_COLUMN,
    STA_ROOM_TYPE_COLUMN,
    STA_W_COLUMN,
    STA_IN_TIME_COLUME,
    STA_IN_REAL_CULUME,
    STA_ROOM_COLUMN_NUMBER
};
/**
    Edit
*/
enum
{
    STA_WORKLOADS_NAME_COLUMN,
    STA_WORKLOADS_WORKLOADS_COLUMN,
    STA_WORKLOADS_COLUMN_NUMBER
};
enum
{
    STA_TIME_NAME_COLUMN,
    STA_TIME_TIME_COLUMN,
    STA_TIME_COLUMN_NUMBER
};



enum
{
    STA_GUEST_RANK_COLUMN,
    STA_GUEST_ID_COLUMN,
    STA_GUEST_NAME_COLUMN,
    STA_GUEST_IN_TIME_COLUMN,
    STA_GUEST_FARE_COLUMN,
    STA_GUEST_PAID_COLUMN,
    STA_DISCOUNT_COLUMN,
    STA_GUEST_COLUMN_NUMBER

};

enum
{
    STA_ROOM_RANK_COLUMN,
    STA_ROOM_NUMBER_COLUMN2,
    STA_ROOM_TYPE_COLUMN2,
    STA_ROOM_FARE_COLUMN,
    STA_ROOM_PAID_COLUMN,
    STA_ROOM_DISCOUNT_COLUMN,
    STA_ROOM_COLUMN_NUMBER2

};

enum
{
    NEW,
    EDIT
};

FILE *file;

int edited= 0;

struct basic_room_type_data *type_head;
/**
Edit
*/
struct basic_teacher_data *teacher_head;

struct basic_room_type_data *active_type= NULL;
/**
    Edit
*/
struct basic_teacher_data *active_teacher= NULL;

struct basic_room_data *active_room= NULL;
struct basic_work_data *active_work= NULL;

struct basic_guest_info *active_guest= NULL;
struct basic_per_year_info *active_year= NULL;

GtkWidget *win;

GtkWidget *room_win;

GtkWidget *guest_win;

GtkListStore *liststore;

GtkListStore *roomstore;

GtkListStore *gueststore;

GtkTreeIter treeiter;

#endif // DEFINES_H_INCLUDED
