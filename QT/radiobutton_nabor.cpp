#include "mainwindow.h"
/*
 *  Тут находятся функции которые просто скрывают элементы соседних элементов,
 *  а свой элемент делуют видимым
*/


void MainWindow::on_radioButton_clicked()
{
   Ui::hide_object<QTableWidget *>(0,table_nabor);
   current_index_box_nabor(0);
}

void MainWindow::on_radioButton_2_clicked()
{
    Ui::hide_object<QTableWidget *>(1,table_nabor);
    current_index_box_nabor(1);
}

void MainWindow::on_radioButton_3_clicked()
{
    Ui::hide_object<QTableWidget *>(2,table_nabor);
    current_index_box_nabor(2);
}

void MainWindow::on_radioButton_4_clicked()
{
    Ui::hide_object<QTableWidget *>(3,table_nabor);
    current_index_box_nabor(3);
}

void MainWindow::on_radioButton_5_clicked()
{
    Ui::hide_object<QTableWidget *>(4,table_nabor);
    current_index_box_nabor(4);
}

void MainWindow::on_radioButton_6_clicked()
{
    Ui::hide_object<QTableWidget *>(5,table_nabor);
    current_index_box_nabor(5);
}

void MainWindow::on_radioButton_7_clicked()
{
    Ui::hide_object<QTableWidget *>(6,table_nabor);
    current_index_box_nabor(6);
}

void MainWindow::on_radioButton_8_clicked()
{
    Ui::hide_object<QTableWidget *>(7,table_nabor);
    current_index_box_nabor(7);
}

