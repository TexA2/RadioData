#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QCheckBox>
#include <QMessageBox>
#include <QRadioButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <sstream>      // создание собсвенного потока ввода
#include <string>       // для работы с пользовательским потоком ввода
#include <stdlib.h>     //rand
#include <time.h>       //time


namespace Ui {

class MainWindow;
class Range_frch;
struct Data_frch;

void create_item_table(QTableWidget&, int row, const QVector<QComboBox *>, int pos_select);
QComboBox* create_combobox(const QVector<QString>&);
void add_vector_range(QVector<QString>&, int start, int end,int step);
void create_tableKey(QTableWidget& table);
int get_check_nabor_radio(const QVector<QRadioButton *> radio_nabor);

template<typename T>
void hide_object(int , QVector<T>&);

template<typename T>
QWidget* item_box(int count);

}


class Range_frch: public QWidget
{
    Q_OBJECT
private:
    QComboBox *box;
public:
    Range_frch();
    QComboBox* getbox() { return box;}
};


struct Data_frch{

    uint8_t barray[3];              // длина 3 байт
    uint8_t barray4[4];             // длина 4 байт
    uint8_t barray8[8];             // длина 8 байт
    uint8_t barray10[10];           // длина 10 байт
    uint8_t barray16[16];           // длина 16 байт
    uint8_t adres[3];               // поле адреса

    uint8_t byte120_frch[120];      // длина 120 байт
    uint8_t byte120_pprch[120];     // длина 120 байт
    uint8_t byte384_nabor[384];     // длина 384 байт
    uint8_t byte_key[64];           // длина 64 байт

    int heshsum{};

    void sumheshfile(uint8_t* data, int size);

};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void parser_box_nabor(int&, int&);

public slots:

    void enable_layoal_box_slot(int index);

private slots:

    void on_add_frch_canal_clicked();

    void on_del_frch_canal_clicked();

    void on_autogeneration_clicked();

    void on_radioHEX_clicked();

    void on_radioDEC_clicked();

    void on_add_group_clicked();

    void on_delete_group_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_8_clicked();

    void on_box_nabor_currentIndexChanged(int index);

    void on_add_canal_clicked();

    void on_delete_canal_clicked();

    void on_add_shos_clicked();

    void on_delete_shos_clicked();

    void current_index_box_nabor(int index);

    void on_create_rd_clicked();

    void on_create_rd_canal_clicked();

    void on_create_new_shos_clicked();

    void enable_layoal_box(int);

    void on_pushButton_7_clicked();

    void on_table_canals_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void insert_shos_radio(int);

    void on_table_shos_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_save_rd_clicked();

    void struct_frch(int row, Data_frch *data);

    void struct_canal_pprch(int row, Data_frch *data);

    void struct_key(int row, Data_frch *data);

    void on_save_shos_clicked();

    void struct_shos(int row, Data_frch *data);

    void title_name(QString fname);

    bool open_shos(int row,Data_frch *data);

    void on_open_shos_clicked();

    int open_frch(int *modul_index, const Data_frch& data);

    void open_pprch_canal(const int row, Data_frch *data);

    int open_pprch_nabor(int *quantiq, Data_frch *data);

    void on_open_rd_clicked();

private:
    Ui::MainWindow *ui;
    int row_frch {};
    int key_switch {};
    int row_canal {};
    int row_shos {};
    QVector<int> row_nabor {QVector<int>(8,0) };    // храним кол-во заполненных строк
    QVector<int> range_nabor {QVector<int>(8,0) };  // храним индексы
    QVector<QRadioButton *> radio_nabor {};
    QVector<QTableWidget *> table_nabor {};
    int cell_row_canal{};

};



#endif // MAINWINDOW_H

