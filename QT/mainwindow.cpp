#include "mainwindow.h"
#include "ui_mainwindow.h"

void output_file(uint num, uint8_t lenbytefrch, uint8_t hashfile, QTextStream &stream, const Data_frch& data);
void key_mode(QTableWidget *tableq, Data_frch *data);
void struct_group_nabor(QTableWidget *tableq, int row, Data_frch *data);

Range_frch::Range_frch()
{
    QVector<QString> canal_range;
    canal_range.push_back("30.000-51.500");
    canal_range.push_back("51.550-87.950");
    canal_range.push_back("100.000-149.950");
    canal_range.push_back("220.000-399.950");
    canal_range.push_back("51.525-87.975");
    canal_range.push_back("100.000-149.975");
    canal_range.push_back("220.000-399.975");

    box = Ui::create_combobox(canal_range);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table_frch->setRowCount(40);
    ui->table_canals->setRowCount(40);
    ui->table_shos->setRowCount(10);

    ui->table_nabor_1->setRowCount(11);
    ui->table_nabor_2->setRowCount(11);
    ui->table_nabor_3->setRowCount(11);
    ui->table_nabor_4->setRowCount(11);
    ui->table_nabor_5->setRowCount(11);
    ui->table_nabor_6->setRowCount(11);
    ui->table_nabor_7->setRowCount(11);
    ui->table_nabor_8->setRowCount(11);

    ui->table_frch->setMaximumSize(441,400);
    Ui::create_tableKey(*(ui->tableKey));
    ui->radioDEC->setChecked(true);

    radio_nabor.push_back(ui->radioButton);
    radio_nabor.push_back(ui->radioButton_2);
    radio_nabor.push_back(ui->radioButton_3);
    radio_nabor.push_back(ui->radioButton_4);
    radio_nabor.push_back(ui->radioButton_5);
    radio_nabor.push_back(ui->radioButton_6);
    radio_nabor.push_back(ui->radioButton_7);
    radio_nabor.push_back(ui->radioButton_8);

    table_nabor.push_back(ui->table_nabor_1);
    table_nabor.push_back(ui->table_nabor_2);
    table_nabor.push_back(ui->table_nabor_3);
    table_nabor.push_back(ui->table_nabor_4);
    table_nabor.push_back(ui->table_nabor_5);
    table_nabor.push_back(ui->table_nabor_6);
    table_nabor.push_back(ui->table_nabor_7);
    table_nabor.push_back(ui->table_nabor_8);

    Ui::hide_object<QTableWidget *>(0,table_nabor);

    ui->table_canals->horizontalHeader()->resizeSection(0,50);
    ui->table_canals->horizontalHeader()->resizeSection(1,50);
    ui->table_canals->horizontalHeader()->resizeSection(4,50);
    ui->table_canals->horizontalHeader()->resizeSection(2,110);
    ui->table_canals->horizontalHeader()->resizeSection(3,125);

    ui->table_shos->horizontalHeader()->resizeSection(0,50);
    ui->table_shos->horizontalHeader()->resizeSection(1,50);
    ui->table_shos->horizontalHeader()->resizeSection(5,50);
    ui->table_shos->horizontalHeader()->resizeSection(11,50);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//******************************** ОБЩИЕ **********************************************/



void Ui::add_vector_range(QVector<QString>& vec, int start, int end,int step = 1)
// Функция заполняет вектор данными из дапозона start - end
// Фукнция ничего не возращает.
{
    for (; start <= end; start += step)
    {
        vec.push_back(QString::number(start));
    }
}

QComboBox* Ui::create_combobox(const QVector<QString>& text)
// Функция создает выпадающий список и заполняет его текстом из вектора (text)
// Функция возращает указатель заполненный выпадающий список.
{
    QComboBox *new_combo = new QComboBox;
    for(QString x: text)
        new_combo->addItem(x);

    return new_combo;
}

void Ui::create_item_table(QTableWidget& table, int row, const QVector<QComboBox *> box , int pos_select)
// Функция заполняет ячейки в таблице (table), выпадающими списками (box), на строке row
// на последней ячейке таблицы генерируем ячейку выбора.
// Функция ничего не возращает.
{
    unsigned int i = 0;                                          // номер ячейки которая заполняется РД


    for (QComboBox *x : box)
    {
        QTableWidgetItem *new_item = new QTableWidgetItem;
        table.setItem(row,i,new_item);
        table.setCellWidget(row,i, x);
        i++;
    }

    /* Генерируем ячейку выбор вставляя туда кнопку с галочкой */
    QTableWidgetItem *item_checkbox = new QTableWidgetItem;
    table.setItem(row,pos_select,item_checkbox);


    QCheckBox *new_check = new QCheckBox;
    QHBoxLayout *loyaout = new QHBoxLayout;

    loyaout->addWidget(new_check,0, Qt::AlignCenter);

    QWidget *item = new QWidget;
    item->setLayout(loyaout);

    table.setCellWidget(row,pos_select, item);

}

void MainWindow::parser_box_nabor(int& start, int& end)
// Функция парсит box_nabor и находит там  2 числа
// Функция ничего не возращает
{
    QString text = ui->box_nabor->currentText();
    int i = 0;


    for (; i < text.size(); i++)
    {
        if (text[i] == '.')
            continue;

        if (text[i].isNumber())
        {
            start *= 10;
            start += QString(text[i]).toInt();
        }

        if (text[i] == '-')
            break;
    }

    for (; i < text.size(); i++)
    {
        if (text[i] == '.')
            continue;

        if (text[i].isDigit())
        {
            end *= 10;
            end += QString(text[i]).toInt();
        }
    }
}

int Ui::get_check_nabor_radio(const QVector<QRadioButton *> radio_nabor)
// Функция проверяет какой radiobutton раздела набор активен
// Функция в случае успеха возращает номер выбранного radiobutton
{
    int i{};
    for(QRadioButton * x: radio_nabor)
    {
        if (x->isChecked())
        return i;

        ++i;
    }
    return -1;
}

template<typename T>
void Ui::hide_object(int pos, QVector<T>& vec)
// Функция скрывает элементы в векторе, кроме элемента под номером pos
// Функция ничего не возращает
{
    for (T x : vec)
        x->setVisible(false);

    vec[pos]->setVisible(true);
}

void MainWindow::title_name(QString fname)
// Функция изменяем название программы на имя открытого/сохраненного файла
// Функция ничего не возращает
{
    QString name = "";
    int len = fname.size();
    int znaq = len;
    for (; ; znaq--)
        if (fname[znaq] == "/") break;

    znaq++;

    for (;znaq <= len; znaq++)
        name += fname[znaq];

    setWindowTitle(name);
}

void Data_frch::sumheshfile(uint8_t *barrayq, int size)
// Функция складывает все байты для создания единной хеш сумму
// Функция ничего не возращает
{
    for (int i = 0; i < size; i++)
        heshsum += barrayq[i];
}

void output_file(uint num, uint8_t lenbytefrch, uint8_t hashfile, QTextStream &stream, const Data_frch& data)
// Функция служит для создания записили данных в файл в hex формате
//
{
    switch (num)
    {
     case 3:
        stream << ":"+QString("%0").arg(lenbytefrch,2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(hashfile,2,16,QChar('0')).toUpper();
        break;
     case 4:
        stream << ":"+QString("%0").arg(lenbytefrch,2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray4[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray4[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray4[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray4[3],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(hashfile,2,16,QChar('0')).toUpper();
        break;

    case 8:
       stream << ":"+QString("%0").arg(lenbytefrch,2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.adres[0],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.adres[1],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.adres[2],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[0],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[1],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[2],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[3],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[4],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[5],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[6],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(data.barray8[7],2,16,QChar('0')).toUpper()+
                 QString("%0").arg(hashfile,2,16,QChar('0')).toUpper();
       break;
     case 16:
        stream << ":"+QString("%0").arg(lenbytefrch,2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.adres[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[0],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[1],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[2],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[3],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[4],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[5],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[6],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[7],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[8],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[9],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[10],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[11],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[12],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[13],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[14],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(data.barray16[15],2,16,QChar('0')).toUpper()+
                  QString("%0").arg(hashfile,2,16,QChar('0')).toUpper();
        break;
    }
}

QString find_interval(int index)
// Функция проверяет индекс и генерирует строку
// нужно для изменения заголовка у таблиц table_nabor
// Функция возращает строку
{
    if (index >= 4)
        return "Интервал 25 кГц";
    else
        return "Интервал 50 кГц";

}







/********************************   ППРЧ **********************************************/


void MainWindow::on_add_frch_canal_clicked()
// Функция отслеживает нажатие клавиши (добавиь канал)
// Добавляет строку в таблице table_frch и заполняет ее через create_item_table
// функция ничего не возращает
{

    if (this->row_frch >= 40)
    {
        QMessageBox::information(this, "Внимание", "Достигнуто максимальное количество элементов");
        return;
    }

    QVector<QString>mode_frch {"АМ","АТ","ФМ","ЧМ","ЧТ3","ЧТ10","N/A"};
    QVector<QString>freq_frch;

    ui->lineEdit->setText(QString::number(freq_frch.capacity()));

    Ui::add_vector_range(freq_frch, 100000, 149975, 25);
    Ui::add_vector_range(freq_frch, 156000, 173975, 25);
    Ui::add_vector_range(freq_frch, 220000, 399975, 25);
    Ui::add_vector_range(freq_frch, 400000, 512000, 25);
    freq_frch.push_back("N/A");

    ui->lineEdit_2->setText(QString::number(freq_frch.capacity()));

    QVector<QComboBox *> frch_box {Ui::create_combobox(freq_frch),
                                   Ui::create_combobox(mode_frch)};

    Ui::create_item_table(*(ui->table_frch), this->row_frch, frch_box, ui->table_frch->columnCount()-1);
    ++this->row_frch;
}


void MainWindow::on_del_frch_canal_clicked()
// Функция отслеживает в table_frch нажатый квадратик в колонке Выбор
// если квадратик нажат то строка удаляеется.
// Функция ничего не возращает
{
    QCheckBox *new_check {};
    QWidget *itemq {};

    int end = ui->table_frch->rowCount();
    int last = ui->table_frch->columnCount()-1;

    for (int i = 0; i < end; i++)
    {

        if (ui->table_frch->cellWidget(i,0) == nullptr)
            return;

        itemq = ui->table_frch->cellWidget(i, last)->layout()->itemAt(0)->widget();
        new_check = qobject_cast<QCheckBox *>(itemq);

        if (new_check->isChecked())
        {
           ui->table_frch->removeRow(i);
           --this->row_frch;
            ui->table_frch->insertRow(ui->table_frch->rowCount());
        }
    }
}



/******************************** КАНАЛ **********************************************/



void MainWindow::on_add_canal_clicked()
// сеть [0,255]
// ключ [0,3]
// диапозоны как у box_nabor
// наборы пока под вопросом
// выбор
{
    if (this->row_canal >= 40)
    {
        QMessageBox::information(this, "Внимание", "Достигнуто максимальное количество элементов");
        return;
    }

    QVector<QString> canal_net;
    QVector<QString> canal_key;

    Ui::add_vector_range(canal_net, 0, 255);
    Ui::add_vector_range(canal_key, 0, 3);


    QVector<QComboBox *> canal_box {Ui::create_combobox(canal_net),
                                    Ui::create_combobox(canal_key)};

    Ui::create_item_table(*ui->table_canals, row_canal, canal_box, ui->table_canals->columnCount()-1);


    // класс
    QTableWidgetItem *new_item = new QTableWidgetItem;
    ui->table_canals->setItem(row_canal,2,new_item);

    Range_frch *rangeq = new Range_frch;

    connect(rangeq->getbox(), SIGNAL(currentIndexChanged(int)), this, SLOT(enable_layoal_box_slot(int)));
    ui->table_canals->setCellWidget(row_canal,2,rangeq->getbox());



    QWidget *item = Ui::item_box<QCheckBox>(8);
    ui->table_canals->setCellWidget(row_canal,3,item);
    enable_layoal_box(row_canal);

    ++row_canal;
}


void MainWindow::on_delete_canal_clicked()
// Функция отслеживает в table_canals нажатый квадратик в колонке Выбор
// если квадратик нажат то строка удаляеется.
// Функция ничего не возращает
{
    QCheckBox *new_check {};
    QWidget *itemq {};

    for (int i = 0; i < ui->table_canals->rowCount(); i++)
    {

        if (ui->table_canals->cellWidget(i,0) == nullptr)
            return;

        itemq =  ui->table_canals->cellWidget(i, ui->table_canals->columnCount()-1)->layout()->itemAt(0)->widget();
        new_check = qobject_cast<QCheckBox *>(itemq);

        if (new_check->isChecked())
        {
           ui->table_canals->removeRow(i);
           --this->row_canal;
            ui->table_canals->insertRow(ui->table_canals->rowCount());
        }
    }
}

void MainWindow::on_create_rd_clicked()
// Функция отчищает таблицу table_frch
// функция ничего не возращает
{
    ui->table_frch->clear();
    ui->table_frch->setHorizontalHeaderLabels(QStringList() << "Частота, кГц"
                                        << "Режим"
                                        << "Выбор");

    row_frch = 0;
}

void MainWindow::on_create_rd_canal_clicked()
// Функция отчищает таблицу table_сanal
{
    ui->table_canals->clear();
    ui->table_canals->setHorizontalHeaderLabels(QStringList() << "Сеть"
                                        << "Ключ"
                                        << "Диапазон"
                                        << "Набор"
                                        << "Выбор");

    row_canal = 0;
}

void MainWindow::enable_layoal_box(int row)
// Функция в таблице table_canal в ячейке набор делает кнопки активными или неактивными
// функция ничего не возращает
{

QComboBox* comitem = qobject_cast<QComboBox *>(ui->table_canals->cellWidget(row,2));

int index = comitem->currentIndex();


    for (int i = 0; i < range_nabor.size(); ++i)
    {
        if (index == range_nabor[i] && table_nabor[i]->cellWidget(0,0) != nullptr)
        {
            QWidget *itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(7-i)->widget();
            QCheckBox *box = qobject_cast<QCheckBox *>(itemq);
            box->setEnabled(true);
        }
        else
        {
            QWidget *itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(7-i)->widget();
            QCheckBox *box = qobject_cast<QCheckBox *>(itemq);
            box->setEnabled(false);
        }
    }
}

void MainWindow::enable_layoal_box_slot(int index)
// Функция в таблице table_canal в ячейке набор делает кнопки активными или неактивными
// данная функция рабает непосредственно в combobox и нужна для выполнения после получения сигнала
// функция ничего не возращает
{

    for (int i = 0; i < range_nabor.size(); ++i)
    {
        if (index == range_nabor[i] && table_nabor[i]->cellWidget(0,0) != nullptr)
        {
            QWidget *itemq = ui->table_canals->cellWidget(cell_row_canal,3)->layout()->itemAt(7-i)->widget();
            QCheckBox *box = qobject_cast<QCheckBox *>(itemq);
            box->setEnabled(true);
        }
        else
        {
            QWidget *itemq = ui->table_canals->cellWidget(cell_row_canal,3)->layout()->itemAt(7-i)->widget();
            QCheckBox *box = qobject_cast<QCheckBox *>(itemq);
            box->setEnabled(false);
        }
    }


}

void MainWindow::on_table_canals_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
// функция определяет какая строка была нажата в table_canal
{
    cell_row_canal = currentRow;
}

void MainWindow::on_box_nabor_currentIndexChanged(int index)
// Функция проверяет индекс в box_nabor и если индекс больше или равен 4 то изменяет
// в текущей видимой таблице текст заголовка на 25 кГц или 50 кГц
// так же проверяет заполнена ли была таблица до смены диапозона
// если да, то предлагает стереть все значения
// если нет то возращает значения индекса диапозона на место
// Функция ничего не возращает
{
   int pos_radio = Ui::get_check_nabor_radio(radio_nabor);
   QTableWidgetItem* text = new QTableWidgetItem(find_interval(index));

   table_nabor[pos_radio]->setHorizontalHeaderItem(1, text);

    int start_range{};
    int end_range{};

    parser_box_nabor(start_range,end_range);

    for(int i =0; i < table_nabor[pos_radio]->rowCount(); ++i)
    {
        if (table_nabor[pos_radio]->item(i,0) == nullptr)
            return;

        if( range_nabor[pos_radio] != index )
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Внимание", "Смена диапозона, текущие значения будут утерянны!"
                                                                      "\n Потверджаете дейстиве?",
                                                                      QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No)
            {
                ui->box_nabor->setCurrentIndex(range_nabor[pos_radio]);
                return;
            }
            else
            {
               table_nabor[pos_radio]->clear();
               row_nabor[pos_radio] = 0;
               table_nabor[pos_radio]->setHorizontalHeaderLabels(QStringList() << "Нач частота, кГц"
                                                                    << find_interval(index)
                                                                    << "Кол-во частот"
                                                                    << "Выбор");

               for (int i = 0; i < row_canal; ++i)
                            enable_layoal_box(i);
                return;
            }
        }
    }

}

void MainWindow::current_index_box_nabor(int index)
// Функция изменяет индекс диапозона, в зависимости от выбраного набора
// Функция ничего не возращает
{
   ui->box_nabor->setCurrentIndex(range_nabor[index]);
}



/******************************** НАБОР **********************************************/



void MainWindow::on_add_group_clicked()
// Функция заполняет таблицу table_nabor
// начальная частота зависит от значений в  box_nabor
// интервал [0,127]
// кол-во частот [1,256]
// Функция ничего не возращает
{
    int pos_radio = Ui::get_check_nabor_radio(radio_nabor);

    if (row_nabor[pos_radio] >= 11)
    {
        QMessageBox::information(this, "Внимание", "Достигнуто максимальное количество элементов в наборе: " + QString::number(pos_radio+1) );
        return;
    }

    int start_range{};
    int end_range{};
    int step{};

   parser_box_nabor(start_range,end_range);

   if (end_range % 50 == 0 && end_range != 51500)
       step = 50;
   else
       step = 25;

    QVector<QString> nabor_interval;
    QVector<QString> nabor_freq;
    QVector<QString> nabor_nach_freq;

    Ui::add_vector_range(nabor_interval, 0, 127);
    Ui::add_vector_range(nabor_freq, 1, 256);
    Ui::add_vector_range(nabor_nach_freq, start_range, end_range,step);

    QVector<QComboBox *> nabor_box {Ui::create_combobox(nabor_nach_freq),
                                    Ui::create_combobox(nabor_interval),
                                    Ui::create_combobox(nabor_freq)};

    Ui::create_item_table(*table_nabor[pos_radio], row_nabor[pos_radio], nabor_box, table_nabor[pos_radio]->columnCount()-1);

    ++row_nabor[pos_radio];

    range_nabor[pos_radio] = ui->box_nabor->currentIndex();


    for (int i = 0; i < row_canal; i++)
        enable_layoal_box(i);

}

void MainWindow::on_delete_group_clicked()
// Функция удаляет строку с пометкой "выбрать" в table_nabor
{

    QCheckBox *new_check {};
    QWidget *itemq {};

    int pos_radio = Ui::get_check_nabor_radio(radio_nabor);

    for (int i = 0; i < table_nabor[pos_radio]->rowCount(); i++)
    {

        if (table_nabor[pos_radio]->cellWidget(i,table_nabor[pos_radio]->columnCount()-1) == nullptr)
            return;

        itemq = table_nabor[pos_radio]->cellWidget(i,table_nabor[pos_radio]->columnCount()-1)->layout()->itemAt(0)->widget();
        new_check = qobject_cast<QCheckBox *>(itemq);

        if (new_check->isChecked())
        {
            table_nabor[pos_radio]->removeRow(i);
            --row_nabor[pos_radio];
            table_nabor[pos_radio]->insertRow(table_nabor[pos_radio]->rowCount());
        }
    }
}

template<typename T>
QWidget* Ui::item_box(int count)
// Функция создает макет в котором виджеты выстроены в горизонтальную линию
// заполняет макет виджатами вида Т, coutn раз
// Функция возращает пользовательский виджет созданный на основе выше перечисленных действий
{
    QHBoxLayout *loyaout = new QHBoxLayout;

    for (int i = 0; i < count; ++i)
        {
            T *boxch = new T;
            loyaout->addWidget(boxch);
        }

        loyaout->setSpacing(0);

        QWidget *item = new QWidget;
        item->setLayout(loyaout);

        return item;
}


/******************************** КЛЮЧИ **********************************************/



void Ui::create_tableKey(QTableWidget& table)
// Функция заполняет таблицу числами 255
// и выставляет фиксированный размер таблице
// функция ничего не возращает
{
    for (int i=0; i < table.rowCount(); i++)
        for (int j = 0; j < table.columnCount(); j++){
        QTableWidgetItem *item = new QTableWidgetItem("255");
        table.setItem(i,j,item);
        }

    // устанавливаем размер таблицы
    table.setMaximumSize(693,122);
    for (int i = 0; i < table.columnCount(); i++)
       table.horizontalHeader()->resizeSection(i,40);
}

void MainWindow::on_autogeneration_clicked()
// Функция генерирует 10-чные значения в диапозоне [0,255] если кнопка DEC активна и заполняет таблицу tableKey
// если DEC не активна , то заполняет таблицу 16-ными значеями
// функция ничего не возращает.

{
    srand(time(NULL));
    unsigned int urand;

    if (ui->radioDEC->isChecked())
    {
        for (int i=0; i < ui->tableKey->rowCount(); i++)
        for (int j = 0; j < ui->tableKey->columnCount(); j++)
        {
        urand = rand() % 256 + 0;
        ui->tableKey->item(i,j)->setText(QString::number(urand));
        }

    }
    else
    {
       for (int i=0; i < ui->tableKey->rowCount(); i++)
           for (int j = 0; j < ui->tableKey->columnCount(); j++)
           {
           urand =  rand() % 255 + 0;
           ui->tableKey->item(i,j)->setText(QString::number(urand, 16).toUpper());
           }
    }
}

void MainWindow::on_radioHEX_clicked()
//  Функция переводит числа в tableKey в 16-ричный вид при нажатии кнопка HEX
//  Функция ничего невозращает
{
    int num;

    if (key_switch == 0)
        key_switch++;
    else
        return;

    if (key_switch == 1)
    {
        for (int i=0; i < ui->tableKey->rowCount(); i++)
            for (int j = 0; j < ui->tableKey->columnCount(); j++)
            {
                num = (ui->tableKey->item(i,j)->text()).toInt(nullptr,10);
                ui->tableKey->item(i,j)->setText(QString::number(num,16).toUpper());
            }
    }
}

void MainWindow::on_radioDEC_clicked()
//  Функция переводит числа в tableKey в 10-ричный вид при нажатии кнопка DEX
//  Функция ничего невозращает
{
    int num;
    if (key_switch == 1)
        key_switch--;
    else
        return;

    if (key_switch == 0)
    {
        for (int i=0; i < ui->tableKey->rowCount(); i++)
            for (int j = 0; j < ui->tableKey->columnCount(); j++)
            {
                num = (ui->tableKey->item(i,j)->text()).toInt(nullptr,16);
                ui->tableKey->item(i,j)->setText(QString::number(num,10));
            }
    }
}



/******************************** СХОС **********************************************/



void MainWindow::on_add_shos_clicked()
// Функция заполняет строку в table_shos
// Функция ничего не возращает
{

    if (row_shos >= 10)
    {
        QMessageBox::information(this, "Внимание", "Достигнуто максимальное количество элементов");
        return;
    }

    QVector<QString> shos_nabor;
    QVector<QString> shos_num_shos;
    QVector<QString> shos_radio;

    QVector<QString> shos_prd;
    QVector<QString> shos_prm;

    QVector<QString> shos_koder;

    QVector<QString> shos_speed;
    QVector<QString> shos_canal;

    QVector<QString> shos_psp;
    QVector<QString> shos_kbu;
    QVector<QString> shos_skk;

    Ui::add_vector_range(shos_nabor, 0, 3);
    Ui::add_vector_range(shos_num_shos, 0, 9);
    Ui::add_vector_range(shos_canal, 1, 8);
    Ui::add_vector_range(shos_psp, 0, 99);

    shos_kbu.push_back("0");
    shos_kbu.push_back("1");

    shos_radio.push_back("Багет");
    shos_radio.push_back("Кордон");
    shos_radio.push_back("Цунами");

    shos_skk.push_back("0");

    Ui::add_vector_range(shos_prd, 345000, 385000, 10);
    Ui::add_vector_range(shos_prm, 276000, 289900, 100);


    shos_koder.push_back("0");
    shos_koder.push_back("4");
    shos_koder.push_back("7");
    shos_koder.push_back("9");

    shos_speed.push_back("1200");
    shos_speed.push_back("2400");
    shos_speed.push_back("4800");
    shos_speed.push_back("9600");

    QVector<QComboBox *> shos_box {Ui::create_combobox(shos_nabor),
                                    Ui::create_combobox(shos_num_shos),
                                    Ui::create_combobox(shos_radio),
                                    Ui::create_combobox(shos_prd),
                                    Ui::create_combobox(shos_prm),
                                    Ui::create_combobox(shos_koder),
                                    Ui::create_combobox(shos_speed),
                                    Ui::create_combobox(shos_canal),
                                    Ui::create_combobox(shos_psp),
                                    Ui::create_combobox(shos_kbu),
                                    Ui::create_combobox(shos_skk)};

    connect(shos_box.at(2),SIGNAL(currentIndexChanged(int)), this, SLOT(insert_shos_radio(int)));

    Ui::create_item_table(*ui->table_shos, row_shos, shos_box, ui->table_shos->columnCount()-1);
    ++row_shos;

}

void MainWindow::on_delete_shos_clicked()
// Функция отслеживает в table_shos нажатый квадратик в колонке Выбор
// если квадратик нажат то строка удаляеется.
// Функция ничего не возращает
{
    QCheckBox *new_check {};
    QWidget *itemq {};

    for (int i = 0; i < ui->table_shos->rowCount(); i++)
    {

        if (ui->table_shos->cellWidget(i,0) == nullptr)
            return;

        itemq = ui->table_shos->cellWidget(i,ui->table_shos->columnCount() -1)->layout()->itemAt(0)->widget();

        new_check = qobject_cast<QCheckBox *>(itemq);

        if (new_check->isChecked())
        {
           ui->table_shos->removeRow(i);
           --this->row_shos;
            ui->table_shos->insertRow(ui->table_shos->rowCount());
        }
    }
}

void MainWindow::on_create_new_shos_clicked()
// Функция отчищает таблицу table_shos
{
    ui->table_shos->clear();
    ui->table_shos->setHorizontalHeaderLabels(QStringList() << "Набор"
                                        << "СХОС"
                                        << "Режим"
                                        << "Частота ПРД кГц"
                                        << "Частота ПРМ кГц"
                                        << "Кодер"
                                        << "Скорость бит/с"
                                        << "Информ канал"
                                        << "Номер ПСП"
                                        << "Номер КБУ"
                                        << "Номер СКК"
                                        << "Выбор");

    row_shos = 0;
}

void MainWindow::insert_shos_radio(int index)
// Функция меняет значения в выпадающем спсике ПРМ и ПРД в таблице table_shos
// Функция ничего не возращает
{
    if (index == 0)             // багет
    {
        QComboBox* comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,3));  // ПРД
        QVector<QString> shos_prd;
        Ui::add_vector_range(shos_prd, 345000, 385000, 10);
        comitem =  Ui::create_combobox(shos_prd);

        ui->table_shos->setCellWidget(cell_row_canal,3, comitem);

        comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,4));             // ПРМ
        QVector<QString> shos_prm;
        Ui::add_vector_range(shos_prm, 276000, 289900, 100);
        comitem =  Ui::create_combobox(shos_prm);

        ui->table_shos->setCellWidget(cell_row_canal,4, comitem);

    }
    else if ( index == 1)       // кордон
    {
        QComboBox* comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,3));  // ПРД

        QVector<QString> shos_prd;
        Ui::add_vector_range(shos_prd, 345000, 385000, 5);
        comitem = Ui::create_combobox(shos_prd);

        ui->table_shos->setCellWidget(cell_row_canal,3, comitem);

        comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,4));  // ПРМ
        QVector<QString> shos_prm;
        Ui::add_vector_range(shos_prm, 276000, 289995, 5);
        comitem =  Ui::create_combobox(shos_prm);

        ui->table_shos->setCellWidget(cell_row_canal,4, comitem);
    }
    else                      // Цунами
    {
        QComboBox* comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,3));  // ПРД

        QVector<QString> shos_prd;
        Ui::add_vector_range(shos_prd, 441000, 442975, 25);
        comitem = Ui::create_combobox(shos_prd);

        ui->table_shos->setCellWidget(cell_row_canal,3, comitem);

        comitem = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(cell_row_canal,4));  // ПРМ
        QVector<QString> shos_prm;
        Ui::add_vector_range(shos_prm, 483000, 483975, 25);
        comitem =  Ui::create_combobox(shos_prm);

        ui->table_shos->setCellWidget(cell_row_canal,4, comitem);
    }
}

void MainWindow::on_table_shos_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
// Функция определяет какая строка была нажата в table_shos
// функция использует переменную предназначенную для on_table_canals_currentCellChanged
// думаю багов не должно возникнуть
{
    cell_row_canal = currentRow;
}




//Блок №1
/**************************************************************
 * Программа должна подготавливать и загружать два блока РД:
 *    Блок №1
 *    -  40 каналов ФРЧ - 120 байт;
 *    -  40 каналов ППРЧ - 120 байт;
 *    -  8 наборов частот для ППРЧ по 48 байт каждый - 384 байта;
 *    -  4 ключа ППРЧ по 16 байт каждый - 64 байта;
 *    -  признак високосного года - 1 байт;
 *    -  контрольная сумма - 3 байта.
 *    -  свободные - 15 байт.
 * Полный  объем  набора №1 составляет 707 байт.
 * Структура загружаемого набора №1 РД приведена ниже.
 *
 * +---------------------------+---------------+---------+
 * |   Назначение группы слов  |  Номер байта  | Вид инф.|
 * +---------------------------+---------------+---------+
 * |         Каналы ФРЧ        |       1-3     | канал 0 |
 * |                           |       4-6     | канал 1 |
 * |                           |       ...     | ....    |
 * |                           |     118-120   | канал 39|
 * +---------------------------+---------------+---------+
 * |         Свободные         |     121-127   | Пишем 0 |
 * +---------------------------+---------------+---------+
 * |     Флаг високосного года |       128     |   "01"  |
 * +---------------------------+---------------+---------+
 * |        Каналы ППРЧ        |    129-131    | канал 0 |
 * |                           |    132-134    | канал 1 |
 * |                           |      ...      |   ...   |
 * |                           |    245-247    | канал 39|
 * +---------------------------+---------------+---------+
 * |         Свободные         |    248-256    | Пишем 0 |
 * +---------------------------+---------------+---------+
 * |        Наборы частот      |    257-304    | набор 0 |
 * |             для           |    305-352    | набор 1 |
 * |             ППРЧ          |      ...      |   ...   |
 * |                           |    593-640    | набор 7 |
 * +---------------------------+---------------+---------+
 * |            Ключи          |    641-656    | ключ 0  |
 * |                           |    657-672    | ключ 1  |
 * |                           |    673-688    | ключ 2  |
 * |                           |    689-704    | ключ 3  |
 * +---------------------------+---------------+---------+
 * |      Контрольная сумма    |      705      | 1 байт  |
 * |                           |      706      | 2 байт  |
 * |                           |      707      | 3 байт  |
 * +---------------------------+---------------+---------+
 *
 *
 **************************************************************/

//******************************** СОХРАНЕНИЕ ФРЧ*/
void MainWindow::on_save_rd_clicked()
// кнопка сохранить РД
//
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить РД ФРЧ ППРЧ",nullptr,"Hex-file (*.hex)");


    if(filename.isEmpty()){
        QMessageBox::information(this, "Сообщение", "Файл не сохранен!");
        return;
    }

    Data_frch *data = new Data_frch;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, "Сообщение", "Ошибка открытия файла!");
        return;
    }


    uint8_t lenbytefrch = 3;    // длина
    uint8_t hashfile = 0;       // контрольная сумма

    memset(data->adres ,0,sizeof(data->adres));
    data->adres[1] = 16;

    QTextStream stream(&file);

    stream << ":100000001000D202FFFFFFFFFFFFFFFFFFFFFFFF18\n";



/****************** ЗАПОЛНЯЕМ ФРЧ*******************/
    for (int i = 0; i < 40; i++)
    {

    struct_frch(i, data);

    hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] + data->barray[0] + data->barray[1]
            + data->barray[2]) % 256);


    data->sumheshfile(data->barray,3);


    output_file(3, lenbytefrch, hashfile, stream, *data);

    stream << "\n";
    data->adres[1] += lenbytefrch;
    }

/*
 * напоминание что надо делать когда перейдем к инетернет передаче
 * нужно убдет собрать все байты
 * и отправит их в устройство
 * открытие файла нам не нужно будет
 *
    for (int i = 0; i < 3; i++)
    {
        data->byte120_frch[j] += data->barray[i];
        j++;
    }

*/
/***************** END ФРЧ *******************/

    // Свободные биты 127-127
     memset(data->barray8, 0, sizeof(data->barray8));

     hashfile = 0 - ((8 + data->adres[0] + data->adres[1] + data->adres[2] + data->barray8[0]+
             data->barray8[1] + data->barray8[2] + data->barray8[3]+
             data->barray8[4] + data->barray8[5] + data->barray8[6] + data->barray8[7]) % 256);

     data->sumheshfile(data->barray8, 8);
     output_file(8, 8, hashfile, stream, *data);
     stream << "\n";
     data->adres[1] += 8;

/****************** ЗАПОЛНЯЕМ КАНАЛЫ ППРЧ *******************/
     for (unsigned int i = 0; i< 40; i++)
     {


     struct_canal_pprch(i, data);

     hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] +
             data->barray[0] + data->barray[1] +
             data->barray[2]) % 256);

     data->sumheshfile(data->barray, 3);
     output_file(3,lenbytefrch,hashfile, stream, *data);

     stream << "\n";
     data->adres[1] += lenbytefrch;

     if (i == 37) data->adres[0] += 1;
     }

/***************** END КАНАЛЫ ППРЧ *******************/


     /*добавить свободные бит 248-256*/
     memset(data->barray8, 0, sizeof(data->barray8));

     hashfile = 0 - ((8 + data->adres[0] + data->adres[1] + data->adres[2] + data->barray8[0]+
             data->barray8[1] + data->barray8[2] + data->barray8[3]+
             data->barray8[4] + data->barray8[5] + data->barray8[6] + data->barray8[7]) % 256);

     data->sumheshfile(data->barray8, 8);


     output_file(8 , 8, hashfile, stream, *data);
     stream << "\n";
     data->adres[1] += 8;


/****************** ЗАПОЛНЯЕМ НАБОРЫ ЧАСТОТ **************/
     auto iter = table_nabor.begin();
     int i{};

     for (; iter != table_nabor.end(); ++iter, ++i)
     {
         if (i == 5)
             data->adres[0] += 1;

            lenbytefrch = 4;

            key_mode(*iter, data);

            hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] + data->barray4[0] +
                     data->barray4[1] + data->barray4[2] + data->barray4[3]) % 256);

            data->sumheshfile(data->barray4, 4);


            output_file(4 , lenbytefrch, hashfile, stream, *data);

             stream << "\n";

             //сделать 1 раз
             data->adres[1] += lenbytefrch;

             for(int i = 0; i < 11; ++i){

                struct_group_nabor(*iter, i, data);

                hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] + data->barray4[0] +
                         data->barray4[1] + data->barray4[2] + data->barray4[3]) % 256);

                data->sumheshfile(data->barray4, 4);


                output_file(4, lenbytefrch, hashfile, stream, *data);

                stream << "\n";
                data->adres[1] += lenbytefrch;
               }

       }
/***************** END НАБОРЫ ЧАСТОТ *******************/

/************************ ЗАПОЛНЯЕМ КЛЮЧИ *************************/
    lenbytefrch = 16;
    for (int i=0; i < 4; i++){
            struct_key(i, data);

            hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] + data->barray16[0] +
                    data->barray16[1] + data->barray16[2] + data->barray16[3]+
                    data->barray16[4] + data->barray16[5] + data->barray16[6]+
                    data->barray16[7] + data->barray16[8] + data->barray16[9]+
                    data->barray16[10] + data->barray16[11] + data->barray16[12]+
                    data->barray16[13] + data->barray16[14] + data->barray16[15]) % 256);

            data->sumheshfile(data->barray16, 16);


            output_file(16, lenbytefrch, hashfile, stream, *data);

            stream << "\n";
            data->adres[1] +=lenbytefrch;
    }
/************************ END КЛЮЧИ *************************/

// Контрольная сумма
     lenbytefrch = 3;
      memset(data->barray, 0, sizeof (data->barray));
     QString str2 = QString("%0").arg(data->heshsum, 6, 16, QChar('0'));

     QString temp = QString(str2[4]) + QString(str2[5]);
     data->barray[0] = temp.toInt(nullptr,16);
     temp = QString(str2[2]) + QString(str2[3]);
     data->barray[1] = temp.toInt(nullptr,16);
     temp = QString(str2[0]) + QString(str2[1]);
     data->barray[2] = temp.toInt(nullptr,16);


     hashfile = 0 - ((lenbytefrch + data->adres[0] + data->adres[1] + data->adres[2] + data->barray[0] + data->barray[1]
             + data->barray[2]) % 256);

     output_file(3, lenbytefrch, hashfile, stream, *data);
     stream << "\n";
     stream << ":00000001FF";

     QMessageBox::information(this, "Сообщение", "Файл сохранен!");

     file.close();
     delete data;

    title_name(filename);
}

void MainWindow::struct_key(int row, Data_frch *data)
// Функция берет значения таблиц table_key
// и заполняем массив barray по тз нужными значениями
{
    memset(data->barray16, 0, sizeof (data->barray16));
    for (int i = 0; i < 16 ; i++)
    {
        if (ui->radioDEC->isChecked())
        {
        data->barray16[i] |= ui->tableKey->item(row,i)->text().toInt(nullptr,10);
        } else {
        data->barray16[i] |= ui->tableKey->item(row,i)->text().toInt(nullptr,16);
        }
    }

}

void struct_group_nabor(QTableWidget *tableq, int row, Data_frch *data)
// Функция берет значения таблиц table_nabor
// и заполняем массив barray по тз нужными значениями

{

    uint sotnum;
    uint decnum;
    uint ednum;
    uint numsot;
    uint numdec;

    memset(data->barray4, 0, sizeof (data->barray4));

    if (tableq->cellWidget(row,0) == nullptr) return;

    QComboBox *box = qobject_cast<QComboBox *> (tableq->cellWidget(row,0));

    uint num = box->currentText().toInt();

    box = qobject_cast<QComboBox *> (tableq->cellWidget(row,2));
    uint freq = box->currentText().toInt(); //частота

    box = qobject_cast<QComboBox *> (tableq->cellWidget(row,1));
    uint interval = box->currentText().toInt(); // интервал

    sotnum = num / 100000; // разряд сотни в МГц

    decnum = num / 10000; // разряд десятки в МГц
    decnum %= 10;

    ednum = num / 1000; // разряд еденицы в МГц
    ednum %= 10;

    numsot = num /100; // разряд сотни в кГц
    numsot %= 10;

    numdec = num % 100 ; // десятки + еденицы в кГц

// Признак последней группы.
    /*
    int pos_not_empty = 0;

    for (int i = 0; i < 11; ++i)
    {
        if (tableq->cellWidget(i,0) != nullptr) pos_not_empty = i;
        if (pos_not_empty == row) interval |= (1 << 7);
    }

*/
    if (tableq->cellWidget(row+1,0) == nullptr)
    {
        interval |= (1 << 7);
    }

    data->barray4[2] |= interval;

    if (tableq->cellWidget(row,0) != nullptr)   data->barray4[3] |= (freq - 1);

    switch (sotnum)
    {
        case 1:     data->barray4[1] |= (1 << 6);                                   break;
        case 2:     data->barray4[1] |= (1 << 7);                                   break;
        case 3:     data->barray4[1] |= (1 << 6) | (1 << 7);                        break;
    }


    switch (decnum)
    {
        case 1:     data->barray4[1] |= (1 << 2);                                   break;
        case 2:     data->barray4[1] |= (1 << 3);                                   break;
        case 3:     data->barray4[1] |= (1 << 2) | (1 << 3);                        break;
        case 4:     data->barray4[1] |= (1 << 4);                                   break;
        case 5:     data->barray4[1] |= (1 << 4) | (1 << 2);                        break;
        case 6:     data->barray4[1] |= (1 << 4) | (1 << 3);                        break;
        case 7:     data->barray4[1] |= (1 << 4) | (1 << 3) | (1 << 2);             break;
        case 8:     data->barray4[1] |= (1 << 5);                                   break;
        case 9:     data->barray4[1] |= (1 << 5) | (1 << 2);                        break;
    }


    switch (ednum)
    {
        case 1:     data->barray4[0] |= (1 << 6);                                   break;
        case 2:     data->barray4[0] |= (1 << 7);                                   break;
        case 3:     data->barray4[0] |= (1 << 7) | (1 << 6);                        break;
        case 4:     data->barray4[1] |= 1;                                          break;
        case 5:     data->barray4[1] |= 1; data->barray4[0] |= (1 << 6);            break;
        case 6:     data->barray4[1] |= 1;data->barray4[0] |= (1 << 7);             break;
        case 7:     data->barray4[1] |= 1; data->barray4[0] |= (1 << 7) | (1 << 6); break;
        case 8:     data->barray4[1] |= (1 << 1);                                   break;
        case 9:     data->barray4[1] |= (1 << 1); data->barray4[0] |= (1 << 6);     break;
    }


    switch (numsot)
    {
        case 1:     data->barray4[0] |= (1 << 2);                                   break;
        case 2:     data->barray4[0] |= (1 << 3);                                   break;
        case 3:     data->barray4[0] |= (1 << 3) | (1 << 2);                        break;
        case 4:     data->barray4[0] |= (1 << 4);                                   break;
        case 5:     data->barray4[0] |= (1 << 4) | (1 << 2);                        break;
        case 6:     data->barray4[0] |= (1 << 4) | (1 << 3);                        break;
        case 7:     data->barray4[0] |= (1 << 4) | (1 << 3) | (1 << 2);             break;
        case 8:     data->barray4[0] |= (1 << 5);                                   break;
        case 9:     data->barray4[0] |= (1 << 5) | (1 << 2);                        break;
    }

    switch (numdec)
    {
        case 25:    data->barray4[0] |= 1;                                          break;
        case 50:    data->barray4[0] |= (1 << 1);                                   break;
        case 75:    data->barray4[0] |= (1 << 1) | 1;                               break;
    }

}

void key_mode(QTableWidget *tableq, Data_frch *data)
// Функция смотрит на таблицу ППРЧ набор на название 2 колонки
// И вытаскивает от туда значение Интервала 50 или 25
{
    /* 01-М1-50кГц * 11-М3-25кГц */
    memset(data->barray4, 0 , sizeof(data->barray4));

    QString st = tableq->horizontalHeaderItem(1)->text();

    std::istringstream textq {st.toStdString()};

    int num;

    std::string txt;

    textq >> txt >> num;


   if (num == 50){
       data->barray4[3] |= 1;
    } else {
       data->barray4[3] |= 1 | (1 << 1);
   }


}

void MainWindow::struct_frch(int row, Data_frch *data)
// Функция берет значения частоты и модуляции из table_frch из строки row
// и заполняем массив barray по тз нужными значениями
{

    QString mode;
    uint num;
    uint sotnum;
    uint decnum;
    uint ednum;
    uint numsot;
    uint numdec;


    int i = 0;// номер байта
    memset(data->barray, 0, sizeof(data->barray));

    QComboBox *freq = qobject_cast<QComboBox*> (ui->table_frch->cellWidget(row,0));
    QComboBox *box_mode = qobject_cast<QComboBox*> (ui->table_frch->cellWidget(row,1));

    if (freq == nullptr) return;

    mode = box_mode->currentText();

    num = freq->currentText().toInt();

    sotnum = num / 100000; // разряд сотни в МГц

    decnum = num / 10000; // разряд десятки в МГц
    decnum %= 10;

    ednum = num / 1000; // разряд еденицы в МГц
    ednum %= 10;

    numsot = num /100; // разряд сотни в кГц
    numsot %= 10;

    numdec = num % 100 ;


    switch(sotnum)
    {
     case 1:    data->barray[i+1] |= (1 << 6);                                      break;
     case 2:    data->barray[i+1] |= (1 << 7);                                      break;
     case 3:    data->barray[i+1] |= (1 << 7) | (1 << 6);                           break;
     case 4:    data->barray[i+2] |= (1 << 4);                                      break;
     case 5:    data->barray [i+2] |= (1 << 4); data->barray [i+1] |= (1 << 6);     break;
    }

    switch(decnum)
    {
     case 1:    data->barray[i+1] |= (1 << 2);                                      break;
     case 2:    data->barray[i+1] |= (1 << 3);                                      break;
     case 3:    data->barray[i+1] |= (1 << 2) | (1 << 3);                           break;
     case 4:    data->barray[i+1] |= (1 << 4);                                      break;
     case 5:    data->barray[i+1] |= (1 << 4) | (1 << 2);                           break;
     case 6:    data->barray[i+1] |= (1 << 4) | (1 << 3);                           break;
     case 7:    data->barray[i+1] |= (1 << 4) | (1 << 3) | (1 << 2);                break;
     case 8:    data->barray[i+1] |= (1 << 5);                                      break;
     case 9:    data->barray[i+1] |= (1 << 5) | (1 << 2);                           break;
    }

    switch (ednum)
    {
     case 1:    data->barray[i] |= (1 << 6);                                        break;
     case 2:    data->barray[i] |= (1 << 7);                                        break;
     case 3:    data->barray[i] |= (1 << 6) | (1 << 7) ;                            break;
     case 4:    data->barray[i+1] |= 1;                                             break;
     case 5:    data->barray[i+1] |= 1; data->barray[i] |= (1 << 6);                break;
     case 6:    data->barray[i+1] |= 1; data->barray[i] |= (1 << 7);                break;
     case 7:    data->barray[i+1] |= 1;  data->barray[i] |= (1 << 6) | (1 << 7);    break;
     case 8:    data->barray[i+1] |= (1 << 1);                                      break;
     case 9:    data->barray[i+1] |= (1 << 1); data->barray[i] |= (1 << 6);         break;
    }

    switch (numsot)
    {
     case 1:    data->barray[i] |= (1 << 2);                                        break;
     case 2:    data->barray[i] |= (1 << 3);                                        break;
     case 3:    data->barray[i] |= (1 << 2) | (1 << 3);                             break;
     case 4:    data->barray[i] |= (1 << 4);                                        break;
     case 5:    data->barray[i] |= (1 << 4) | (1 << 2);                             break;
     case 6:    data->barray[i] |= (1 << 4) | (1 << 3);                             break;
     case 7:    data->barray[i] |= (1 << 4) | (1 << 3) | (1 << 2);                  break;
     case 8:    data->barray[i] |= (1 << 5);                                        break;
     case 9:    data->barray[i] |= (1 << 5) | (1 << 2);                             break;
    }

    switch (numdec)
    {
     case 25:   data->barray[i] |= 1;                                               break;
     case 50:   data->barray[i] |= (1 << 1);                                        break;
     case 75:   data->barray[i] |= 1 | (1 << 1);                                    break;
    }

//Привязывать mode к тексут была ошибка лучше потом переделать на индекс

        if (mode == "АТ")       data->barray[i+2] |= (1 << 3);

        if (mode == "АМ")       data->barray[i+2] |= (1 << 2);

        if (mode == "АМШ")      data->barray[i+2] |= 1;

        if (mode == "ЧМ")       data->barray[i+2] |= (1 << 1);

        if (mode == "ЧТ3")      data->barray[i+2] |= (1 << 2) | (1 << 1) | 1;

        if (mode == "ЧТ10")     data->barray[i+2] |= (1 << 1) | 1;

        if (mode == "ФМ")       data->barray[i+2] |= (1 << 3) | 1;

}

void MainWindow::struct_canal_pprch(int row, Data_frch *data)
// Функция берет значения из table_canal
// и заполняет массив barray согласно тз
{

    uint8_t net;                        //сеть
    uint8_t key;                        // ключ
    QString kit (8, '0');               // набор
    QString modekey;                    // режим-диапазон

    int start_range{};
    int end_range{};

    memset(data->barray, 0, sizeof(data->barray));

    if (ui->table_canals->cellWidget(row,0) == nullptr) return;

    QComboBox *box = qobject_cast<QComboBox *> (ui->table_canals->cellWidget(row,0));
    net = box->currentText().toInt();

    box = qobject_cast<QComboBox *> (ui->table_canals->cellWidget(row,1));
    key = box->currentText().toInt();

    box = qobject_cast<QComboBox *> (ui->table_canals->cellWidget(row,2));
    modekey = box->currentText();

    QCheckBox *cbox {};

    for (int i = 0; i <= 7; ++i)
    {
    QWidget *itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(7-i)->widget();
    cbox = qobject_cast<QCheckBox *>(itemq);
    if (cbox->isChecked())
        kit[i] = '1';
    }


    data->barray[0] |= net;

    switch (key)
    {
        case 1:     data->barray[2] |= (1 << 4);            break;
        case 2:     data->barray[2] |= (1 << 5);            break;
        case 3:     data->barray[2] |= (1 << 4) | (1 << 5); break;
    }


   parser_box_nabor(start_range,end_range);

  if (kit[0] == "1")    data->barray[1] |= 1;
  if (kit[1] == "1")    data->barray[1] |= (1 << 1);
  if (kit[2] == "1")    data->barray[1] |= (1 << 2);
  if (kit[3] == "1")    data->barray[1] |= (1 << 3);
  if (kit[4] == "1")    data->barray[1] |= (1 << 4);
  if (kit[5] == "1")    data->barray[1] |= (1 << 5);
  if (kit[6] == "1")    data->barray[1] |= (1 << 6);
  if (kit[7] == "1")    data->barray[1] |= (1 << 7);


  if (start_range % 50 == 0)
      data->barray[2] |= 1;

}


//******************************** ОТКРЫТИЕ ФРЧ*/
void MainWindow::on_open_rd_clicked()
// Функция заполняет все таблицы раздела ФРЧ
// на основе полученных данных из выбранного файла
{

    QString line;
    QString path; //путь открываемого файла

    QString onenumstr;
    QString twonumstr;
    QString treenumstr;
    QString fournumstr;

    QString numstr5;
    QString numstr6;
    QString numstr7;
    QString numstr8;

    QString numstr9;
    QString numstr10;
    QString numstr11;
    QString numstr12;

    QString numstr13;
    QString numstr14;
    QString numstr15;
    QString numstr16;

    int quanti;
    int modul_index;
    int chastota;

    Data_frch *data = new Data_frch;


    path = QFileDialog::getOpenFileName(this, "Выбрать файл", nullptr,"Hex-file (*.hex)");


    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
         QMessageBox::information(this, "Внимение", "Файл не открывается!");
        return;
    }

    line = file.readLine(); //дефолтная первая строка пропускаем
    // можно сделать проверку на первую строку

/**********************************КАНАЛ ФРЧ***************************/
    for (int i = 0; i < 40; ++i)
    {

    line = file.readLine(); // рабочая первас трока

    onenumstr = QString(line[9]) + line[10];
    twonumstr = QString(line[11]) + line[12];
    treenumstr = QString(line[13]) + line[14];

    memset(data->barray, 0, sizeof(data->barray));

    data->barray[0] = onenumstr.toUInt(nullptr,16);
    data->barray[1] = twonumstr.toUInt(nullptr,16);
    data->barray[2] = treenumstr.toUInt(nullptr,16);

    if (treenumstr.toInt() == 0) continue;

    chastota = open_frch(&modul_index, *data);

    on_add_frch_canal_clicked();

    qobject_cast<QComboBox *>(ui->table_frch->cellWidget(i,0))->setCurrentText(QString::number(chastota));
    qobject_cast<QComboBox *>(ui->table_frch->cellWidget(i,1))->setCurrentIndex(modul_index);
    }

    line = file.readLine(); // высокосный год

    /* Проверка високосного года
    treenumstr = QString(line[23]) + line[24];
    memset(data->barray, 0, sizeof(data->barray));
    data->barray[2] = treenumstr.toUInt(nullptr,16);
    if ( (data->barray[2] & 1) == 1) ui->checkBox_9->setChecked(1);
    */


/**********************************КАНАЛ ППРЧ ***************************/
    for(int i=0; i < 40; ++i)
    {
     line = file.readLine(); // рабочая первас трока

     onenumstr = QString(line[9]) + line[10];
     twonumstr = QString(line[11]) + line[12];
     treenumstr = QString(line[13]) + line[14];

     memset(data->barray, 0, sizeof(data->barray));

     data->barray[0] = onenumstr.toUInt(nullptr,16);
     data->barray[1] = twonumstr.toUInt(nullptr,16);
     data->barray[2] = treenumstr.toUInt(nullptr,16);

     if (data->barray[0] + data->barray[1] + data->barray[2] == 0) continue;

     on_add_canal_clicked();
     open_pprch_canal(i, data);

     qobject_cast<QComboBox *> (ui->table_canals->cellWidget(i,0))->setCurrentIndex(data->barray[0]);
     qobject_cast<QComboBox *> (ui->table_canals->cellWidget(i,1))->setCurrentIndex(data->barray[1]);
    }

    line = file.readLine(); // свободные байты

/**********************************Наборы ППРЧ ***************************/
    int mode = 0;


for (int i =0; i < 8; ++i)  // кол-во таблицы
{

    line = file.readLine(); // свободные + код режима

    onenumstr = QString(line[9]) + line[10];
    twonumstr = QString(line[11]) + line[12];
    treenumstr = QString(line[13]) + line[14];
    fournumstr = QString(line[15]) + line[16];

    data->barray4[0] = onenumstr.toUInt(nullptr,16);
    data->barray4[1] = twonumstr.toUInt(nullptr,16);
    data->barray4[2] = treenumstr.toUInt(nullptr,16);
    data->barray4[3] = fournumstr.toUInt(nullptr,16);

    if ( (data->barray4[2] & 1) == 1)
        mode = 0;

    if ( (data->barray4[3] & (1 << 1)) != 0)
        mode = 1;

    radio_nabor[i]->setChecked(true);

    for (int j = 0; j < 11; ++j)
    {
    line = file.readLine();

    onenumstr = QString(line[9]) + line[10];
    twonumstr = QString(line[11]) + line[12];
    treenumstr = QString(line[13]) + line[14];
    fournumstr = QString(line[15]) + line[16];

    memset(data->barray4, 0, sizeof(data->barray4));
    memset(data->barray, 0, sizeof(data->barray));

    data->barray4[0] = onenumstr.toUInt(nullptr,16);
    data->barray4[1] = twonumstr.toUInt(nullptr,16);
    data->barray4[2] = treenumstr.toUInt(nullptr,16);
    data->barray4[3] = fournumstr.toUInt(nullptr,16);

    if (data->barray4[0] + data->barray4[1] + data->barray4[2] + data->barray4[3] == 0) continue;

    int num = open_pprch_nabor(&quanti, data);

    // переделать нужно еще поддершка М3
    if (num > 0)
    {
        if (num <= 399950 && mode == 0)
        {
            range_nabor[i] = 3;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 149950 && mode == 0)
        {
            range_nabor[i] = 2;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 87950 && mode == 0)
        {
            range_nabor[i] = 1;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 399975 && mode == 1)
        {
            range_nabor[i] = 6;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 149975 && mode == 1)
        {
            range_nabor[i] = 5;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 87975 && mode == 1)
        {
            range_nabor[i] = 4;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }

        if (num <= 51500)
        {
            range_nabor[i] = 0;
            ui->box_nabor->setCurrentIndex(range_nabor[i]);
        }
    }

      on_add_group_clicked();

   qobject_cast<QComboBox *>(table_nabor[i]->cellWidget(j,1))->setCurrentText(QString::number(data->barray[1]));
   qobject_cast<QComboBox *>(table_nabor[i]->cellWidget(j,2))->setCurrentText(QString::number(quanti));

    if (data->barray4[2] << 7 == 1) continue;
    }

}

/********************************** ППРЧ ключи ***************************/
for (int i = 0; i < 4; i++)
{
    line = file.readLine();

    onenumstr = QString(line[9]) + line[10];
    twonumstr = QString(line[11]) + line[12];
    treenumstr = QString(line[13]) + line[14];
    fournumstr = QString(line[15]) + line[16];

    numstr5 = QString(line[17]) + line[18];
    numstr6 = QString(line[19]) + line[20];
    numstr7 = QString(line[21]) + line[22];
    numstr8 = QString(line[23]) + line[24];

    numstr9 = QString(line[25]) + line[26];
    numstr10 = QString(line[27]) + line[28];
    numstr11 = QString(line[29]) + line[30];
    numstr12 = QString(line[31]) + line[32];

    numstr13 = QString(line[33]) + line[34];
    numstr14 = QString(line[35]) + line[36];
    numstr15 = QString(line[37]) + line[38];
    numstr16 = QString(line[39]) + line[40];

    data->barray16[0] = onenumstr.toUInt(nullptr,16);
    data->barray16[1] = twonumstr.toUInt(nullptr,16);
    data->barray16[2] = treenumstr.toUInt(nullptr,16);
    data->barray16[3] = fournumstr.toUInt(nullptr,16);

    data->barray16[4] = numstr5.toUInt(nullptr,16);
    data->barray16[5] = numstr6.toUInt(nullptr,16);
    data->barray16[6] = numstr7.toUInt(nullptr,16);
    data->barray16[7] = numstr8.toUInt(nullptr,16);

    data->barray16[8] = numstr9.toUInt(nullptr,16);
    data->barray16[9] = numstr10.toUInt(nullptr,16);
    data->barray16[10] = numstr11.toUInt(nullptr,16);
    data->barray16[11] = numstr12.toUInt(nullptr,16);

    data->barray16[12] = numstr13.toUInt(nullptr,16);
    data->barray16[13] = numstr14.toUInt(nullptr,16);
    data->barray16[14] = numstr15.toUInt(nullptr,16);
    data->barray16[15] = numstr16.toUInt(nullptr,16);

    for (int j = 0; j<= 15; j++)
        ui->tableKey->item(i,j)->setText(QString::number(data->barray16[j]));
}


file.close();
delete data;
title_name(path);


QWidget *itemq {};
// тут сделаем заполнение  ППРЧ каналы режим,диапозон
    for (int i = 0; i < 40; ++i)
    {
        if (ui->table_canals->cellWidget(i,0) != nullptr)
        {
                for (int j = 0; j < 8 ; ++j)
                {
                    itemq = ui->table_canals->cellWidget(i,3)->layout()->itemAt(7-j)->widget();
                    if (qobject_cast<QCheckBox *>(itemq)->isChecked())
                    {
                        qobject_cast<QComboBox *> (ui->table_canals->cellWidget(i,2))->setCurrentIndex(range_nabor[j]);
                        break;
                    }
                }
        }
        else
        {
            break;
        }

    }


}

int MainWindow::open_pprch_nabor(int *quantiq, Data_frch* data)
// Переводит данные полученные из файла в данные для программу
// данные для таблицы table_nabor
{
    int fnach = 0; // начальная частота
    int interval = 0; // интервал
     *quantiq = 1; // кол-во частот


// первый байт
    if ((data->barray4[0] & (1 << 7)) != 0)
        fnach += 2000;

    if ((data->barray4[0] & (1 << 6)) != 0)
        fnach += 1000;

    if ((data->barray4[0] & (1 << 5)) != 0)
        fnach += 800;

    if ((data->barray4[0] & (1 << 4)) != 0)
        fnach += 400;

    if ((data->barray4[0] & (1 << 3)) != 0)
        fnach += 200;

    if ((data->barray4[0] & (1 << 2)) != 0)
        fnach += 100;

    if ((data->barray4[0] & (1 << 1)) != 0)
        fnach += 50;

    if ((data->barray4[0] & 1) == 1)
        fnach += 25;


// второй байт

    if ((data->barray4[1] & (1 << 7)) != 0)
        fnach += 200000;

    if ((data->barray4[1] & (1 << 6)) != 0)
        fnach += 100000;

    if ((data->barray4[1] & (1 << 5)) != 0)
        fnach += 80000;

    if ((data->barray4[1] & (1 << 4)) != 0)
        fnach += 40000;

    if ((data->barray4[1] & (1 << 3)) != 0)
        fnach += 20000;

    if ((data->barray4[1] & (1 << 2)) != 0)
        fnach += 10000;

    if ((data->barray4[1] & (1 << 1)) != 0)
        fnach += 8000;

    if ((data->barray4[1] & 1) == 1)
        fnach += 4000;

// третий байт

    if ((data->barray4[2] & (1 << 6)) != 0)
        interval += 64;

    if ((data->barray4[2] & (1 << 5)) != 0)
        interval += 32;

    if ((data->barray4[2] & (1 << 4)) != 0)
        interval += 16;

    if ((data->barray4[2] & (1 << 3)) != 0)
        interval += 8;

    if ((data->barray4[2] & (1 << 2)) != 0)
        interval += 4;

    if ((data->barray4[2] & (1 << 1)) != 0)
        interval += 2;

    if ((data->barray4[2] & 1) == 1)
        interval += 1;

    data->barray[1] = interval;

// четвертый байт

    if ((data->barray4[3] & (1 << 7)) != 0)
        *quantiq += 128;

    if ((data->barray4[3] & (1 << 6)) != 0)
         *quantiq += 64;

    if ((data->barray4[3] & (1 << 5)) != 0)
         *quantiq += 32;

    if ((data->barray4[3] & (1 << 4)) != 0)
         *quantiq += 16;

    if ((data->barray4[3] & (1 << 3)) != 0)
         *quantiq += 8;

    if ((data->barray4[3] & (1 << 2)) != 0)
         *quantiq += 4;

    if ((data->barray4[3] & (1 << 1)) != 0)
         *quantiq += 2;

    if ((data->barray4[2] & 1) == 1)
         *quantiq += 1;


    return fnach;
}

void MainWindow::open_pprch_canal(const int row, Data_frch* data)
// Переводит данные полученные из файла в данные для программу
// данные для таблицы table_canal
{
    uint net = 0; // сеть
    uint key = 0; // ключ
    uint8_t mode = 0; // код режима ( 0 для М1 и 1 для М3) будем использовать индекс


    if ( (data->barray[0] & (1 << 7)) != 0)
        net += 128;

    if ( (data->barray[0] & (1 << 6)) != 0)
        net += 64;

    if ( (data->barray[0] & (1 << 5)) != 0)
        net += 32;

    if ( (data->barray[0] & (1 << 4)) != 0)
        net += 16;

    if ( (data->barray[0] & (1 << 3)) != 0)
        net += 8;

    if ( (data->barray[0] & (1 << 2)) != 0)
        net += 4;

    if ( (data->barray[0] & (1 << 1)) != 0)
        net += 2;

    if ( (data->barray[0] & 1 ) == 1)
        net += 1;

    data->barray[0] = net;

    QWidget *itemq;

    if ( (data->barray[1] & (1 << 7)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(7)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }


    if ( (data->barray[1] & (1 << 6)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(6)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & (1 << 5)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(5)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & (1 << 4)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(4)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & (1 << 3)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(3)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & (1 << 2)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(2)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & (1 << 1)) != 0)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(1)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }

    if ( (data->barray[1] & 1 ) == 1)
    {
        itemq = ui->table_canals->cellWidget(row,3)->layout()->itemAt(0)->widget();
        qobject_cast<QCheckBox *>(itemq)->setEnabled(true);
        qobject_cast<QCheckBox *>(itemq)->setChecked(true);
    }


    if ( (data->barray[2] & (1 << 5)) != 0)
        key += 2;

    if ( (data->barray[2] & (1 << 4)) != 0)
        key += 1;

    data->barray[1] = key;


    if ( (data->barray[2] & 1) == 1)
        mode = 0;

    if ( (data->barray[2] & (1 << 1)) != 0)
        mode = 1;

    data->barray[2] = mode;


}

int MainWindow::open_frch(int *modul_index, const Data_frch& data)
// Переводит данные полученные из файла в данные для программу
// данные для таблицы table_frch
{

   int chastota = 0;

    uint temp = data.barray[0];
// первый байт
    if ( (temp & ( 1 << 7)) != 0)
        chastota += 2000;


    if ( (temp & ( 1 << 6)) != 0)
        chastota += 1000;


    if ( (temp & ( 1 << 5)) != 0)
        chastota += 800;


    if ( (temp & ( 1 << 4)) != 0)
        chastota += 400;


    if ( (temp & ( 1 << 3)) != 0)
        chastota += 200;


    if ( (temp & ( 1 << 2)) != 0)
        chastota += 100;


    if ( (temp & ( 1 << 1)) != 0)
        chastota += 50;


    if ( (temp & 1)  == 1)
        chastota += 25;



// второй байт
    temp = data.barray[1];
    if ( (temp & ( 1 << 7)) != 0)
        chastota += 200000;


    if ( (temp & ( 1 << 6)) != 0)
        chastota += 100000;


    if ( (temp & ( 1 << 5)) != 0)
        chastota += 80000;


    if ( (temp & ( 1 << 4)) != 0)
        chastota += 40000;


    if ( (temp & ( 1 << 3)) != 0)
        chastota += 20000;


    if ( (temp & ( 1 << 2)) != 0)
        chastota += 10000;


    if ( (temp & ( 1 << 1)) != 0)
        chastota += 8000;


    if ( (temp & 1)  == 1)
        chastota += 4000;


// третий байт

    if ( (data.barray[2] & ( 1 << 7)) != 0)
        chastota += 10;

    if ( (data.barray[2] & ( 1 << 6)) != 0)
        chastota += 5;

// 5 бит свободен

    if ( (data.barray[2] & ( 1 << 4)) != 0)
        chastota += 400000;

//выводим модуляцию

    if ( (data.barray[2] & ( 1 << 3)) != 0)
        *modul_index = 1;      // 1 АТ

    if ( (data.barray[2] & ( 1 << 2)) != 0)
        *modul_index = 0;      //0 АМ

    if ( (data.barray[2] & ( 1 << 1)) != 0)
        *modul_index = 3;      // 3 ЧМ

    if ( ((data.barray[2] & 1)  == 1 ) && ((data.barray[2] & (1 << 1)) != 0))
        *modul_index = 5;       //5 ЧТ(10кГц)

    if ( ((data.barray[2] & 1)  == 1 ) && ((data.barray[2] & (1 << 1)) != 0) && ((data.barray[2] & (1 << 2)) != 0))
        *modul_index = 4;        //4 ЧТ(3кГЦ)

    if ( ((data.barray[2] & 1)  == 1 ) && ((data.barray[2] & (1 << 3)) != 0))
        *modul_index = 2;      //2 ФМ

        return chastota;
}




//Блок №2
/*******************************************************************************
 * Блок №2
 *    -  4 комплекта по 10 СХОС для спутниковой связи по 10 байт каждый - 400 байта;
 *    -  контрольная сумма - 3 байта.
 * Полный  объем  набора №2 составляет 403 байта.
 * Структура загружаемого набора №2 радиоданных приведена ниже
 * +---------------------------+---------------+---------+
 * |   Назначение группы слов  |  Номер байта  | Вид инф.|
 * +---------------------------+---------------+---------+
 * |     Комплект 1            |     1-10      | СХОС 1  |
 * |                           |    11-20      | СХОС 2  |
 * |                           |     ...       | ....    |
 * |                           |    91-100     | СХОС 10 |
 * +---------------------------+---------------+---------+
 * |     Комплект 2            |    101-110    | СХОС 1  |
 * |                           |    111-120    | СХОС 2  |
 * |                           |     ...       | ....    |
 * |                           |    191-200    | СХОС 10 |
 * +---------------------------+---------------+---------+
 * |     Комплект 3            |    201-210    | СХОС 1  |
 * |                           |    211-220    | СХОС 2  |
 * |                           |     ...       | ....    |
 * |                           |    291-300    | СХОС 10 |
 * +---------------------------+---------------+---------+
 * |     Комплект 4            |    301-310    | СХОС 1  |
 * |                           |    311-320    | СХОС 2  |
 * |                           |     ...       | ....    |
 * |                           |    391-400    | СХОС 10 |
 * +---------------------------+---------------+---------+
 * |      Контрольная сумма    |      401      | 1 байт  |
 * |                           |      402      | 2 байт  |
 * |                           |      403      | 3 байт  |
 * +---------------------------+---------------+---------+
*/

void MainWindow::on_save_shos_clicked()
// Функция на основе сгенерированных байтов с таблицы table_shos создает файл
// Функция ничего не возращает
{
    QString temp = "FFFFFFFFFFFFFFFF00";

    QString filename = QFileDialog::getSaveFileName(this, "Сохранить СХОС",nullptr,"Hex-file (*.hex)");

    if(filename.isEmpty())
    {
        QMessageBox::information(this, "Сообщение", "Файл не сохранен!");
        return;
    }

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, "Сообщение", "Ошибка открытия файла!");
        return;
    }

    uint8_t lenbyteshos = 10;                                   // длина байтов схос
    uint8_t hashfile = 0;
    QComboBox *tempq;

    Data_frch *data = new Data_frch;

    memset(data->adres, 0, sizeof(data->adres));

    data->adres[1] = 16;

    QTextStream stream(&file);
    stream << ":100000001000D202FFFFFFFFFFFFFFFFFFFFFFFF18\n";  // уточнить текст


    int count_row = ui->table_shos->rowCount();

        for (int i = 0; i < count_row; ++i)
        {

            if (ui->table_shos->cellWidget(i,0) != nullptr)
            {

                struct_shos(i, data);

                tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(i,3));

                QString peredacha = QString("%0").arg(tempq->currentIndex(), 4, 16, QChar('0')).toUpper();
                QString byte4 = QString(peredacha[0]) + peredacha[1];
                QString byte5 = QString(peredacha[2]) + peredacha[3];
                data->barray10[4] = byte4.toInt(nullptr,16);
                data->barray10[5] = byte5.toInt(nullptr,16);

                tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(i,4));

                QString priem= QString("%0").arg(tempq->currentIndex(), 4, 16, QChar('0')).toUpper();
                QString byte2 = QString(priem[0]) + priem[1];
                QString byte3 = QString(priem[2]) + priem[3];
                data->barray10[2] = byte2.toInt(nullptr,16);
                data->barray10[3] = byte3.toInt(nullptr,16);



                hashfile = 0 - ((lenbyteshos + data->adres[0] + data->adres[1] + data->adres[2]
                    + data->barray10[0] + data->barray10[1]
                    + data->barray10[2] + data->barray10[3]
                    + data->barray10[4] + data->barray10[5]
                    + data->barray10[6] + data->barray10[7]
                    + data->barray10[8] + data->barray10[9]) % 256);

                data->sumheshfile(data->barray10, 10);

                stream << ":"+QString("%0").arg(lenbyteshos, 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->adres[0], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->adres[1], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->adres[2], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[0], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[1], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[2], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[3], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[4], 2, 16 ,QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[5], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[6], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[7], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[8], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(data->barray10[9], 2, 16, QChar('0')).toUpper()+
                      QString("%0").arg(hashfile, 2, 16, QChar('0')).toUpper();

                stream << "\n";

            }
            else
            {
                memset(data->barray10, 0, sizeof(data->barray10));

                hashfile = 0 - ((lenbyteshos + data->adres[0] + data->adres[1] + data->adres[2]
                                + data->barray10[0] + data->barray10[1]
                                + data->barray10[2] + data->barray10[3]
                                + data->barray10[4] + data->barray10[5]
                                + data->barray10[6] + data->barray10[7]
                                + data->barray10[8] + data->barray10[9]) % 256);

                data->sumheshfile(data->barray10, 10);

                stream << ":"+QString("%0").arg(lenbyteshos, 2, 16, QChar('0')).toUpper()+
                           QString("%0").arg(data->adres[0], 2, 16, QChar('0')).toUpper()+
                           QString("%0").arg(data->adres[1], 2, 16, QChar('0')).toUpper()+
                           QString("%0").arg(data->adres[2], 2, 16, QChar('0')).toUpper()+
                           QString("%0").arg(data->barray10[0], 2, 16, QChar('0')).toUpper()+
                           temp + QString("%0").arg(hashfile, 2, 16, QChar('0')).toUpper();

                 stream << "\n";
             }


            data->adres[1] += lenbyteshos;

            }// конец for

            // Контрольная сумма
                lenbyteshos = 3;
                memset(data->barray, 0, sizeof (data->barray));
                QString str2 = QString("%0").arg(data->heshsum, 6, 16, QChar('0'));

                temp = QString(str2[4]) + QString(str2[5]);
                data->barray[0] = temp.toInt(nullptr,16);
                temp = QString(str2[2]) + QString(str2[3]);
                data->barray[1] = temp.toInt(nullptr,16);
                temp = QString(str2[0]) + QString(str2[1]);
                data->barray[2] = temp.toInt(nullptr,16);

                hashfile = 0 - ((lenbyteshos + data->adres[0] + data->adres[1] + data->adres[2]
                     + data->barray[0] + data->barray[1]
                     + data->barray[2]) % 256);

                output_file(3, lenbyteshos, hashfile, stream, *data);

                stream << "\n";
                stream << ":00000001FF";

             QMessageBox::information(this, "Сообщение", "Файл сохранен!");

            file.close();
            delete data;
            tempq = nullptr;

    title_name(filename);
}

/*
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |Байт/разряд|7    |6    |5    |4    |3    |2    |1     |0    |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   1 байт  |  0  |  0  | № набора  |        Номер СХОС      |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   2 байт  |    номер СКК          |Скорость передачи кбит/с|
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   3 байт  |       Номер частоты приема (младшая часть)     |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   4 байт  |       Номер частоты приема (старшая часть)     |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   5 байт  |       Номер частоты передачи ( младшая часть)  |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   6 байт  |       Номер частоты передачи ( старшая часть)  |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   7 байт  |  0  |  0  |    СК     |    Режим радиолинии    |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   8 байт  |                     Номер ПСП                  |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |   9 байт  |        Номер КБУ      |   Информационный канал |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
// |  10 байт  |  0  |  0  |  0  |  0  |  0  |  0  |  0   |  0  |
// +-----------+-----+-----+-----+-----+-----+-----+------+-----+
 */
void MainWindow::struct_shos(int row, Data_frch *data)
// Функция генерирует последовательность байт согласно тз для таблицы table_shos
// Функция ничего не возращает
{

    memset(data->barray10, 0, sizeof(data->barray10));

    QComboBox *tempq;

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,2));
    uint8_t radio = tempq->currentIndex();        //режим радиолинии

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,5));
    uint8_t coder = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,6));
    uint8_t speed = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,7));
    uint8_t canal = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,8));
    uint8_t psp = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,9));
    uint8_t kbu = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,0));
    int nabor = tempq->currentIndex();

    tempq = qobject_cast<QComboBox *>(ui->table_shos->cellWidget(row,1));
    int shos = tempq->currentIndex();


    data->barray10[7] |= psp;


    switch (nabor)
    {
        case 1:     data->barray10[0] |= (1 << 4);                  break;
        case 2:     data->barray10[0] |= (1 << 5);                  break;
        case 3:     data->barray10[0] |= (1 << 4) | (1 << 5);       break;
        default:    break;
    }

    switch (shos)
    {
        case 1:     data->barray10[0] |= 1;                         break;
        case 2:     data->barray10[0] |= (1 << 1);                  break;
        case 3:     data->barray10[0] |= (1 << 1) | 1;              break;
        case 4:     data->barray10[0] |= (1 << 2);                  break;
        case 5:     data->barray10[0] |= (1 << 2) | 1;              break;
        case 6:     data->barray10[0] |= (1 << 2) | (1 << 1);       break;
        case 7:     data->barray10[0] |= (1 << 2) | (1 << 1) | 1;   break;
        case 8:     data->barray10[0] |= (1 << 3);                  break;
        case 9:     data->barray10[0] |= (1 << 3) | 1;              break;
        default:    break;
    }

    switch (speed)
    {
        case 1:     data->barray10[1] |= (1 << 1);                  break;
        case 2:     data->barray10[1] |= (1 << 2);                  break;
        case 3:     data->barray10[1] |= (1 << 3);                  break;
        case 0:     data->barray10[1] |= 1;                         break;
    }

    switch (radio)
    {
        case 1:     data->barray10[6] |= (1 << 1);                  break;
        case 2:     data->barray10[6] |= (1 << 2);                  break;
        default:    data->barray10[6] |= 1;                         break;
    }

    switch (coder)
    {
        case 1:     data->barray10[6] |= (1 << 4);                  break;
        case 2:     data->barray10[6] |= (1 << 5);                  break;
        case 3:     data->barray10[6] |= (1 << 5) | (1 << 4);       break;
        default:    break;
    }

    switch (kbu)
    {
        case 1:     data->barray10[8] |= (1 << 7) | (1 << 6);       break;
        case 0:     data->barray10[8] |= (1 << 5) | (1 << 4);       break;
        default:    break;
    }

    switch (canal)
    {
        case 1:     data->barray10[8] |= (1 << 1);                  break;          //2
        case 2:     data->barray10[8] |= 1 | (1 << 1);              break;          //3
        case 3:     data->barray10[8] |= (1 << 2);                  break;          //4
        case 4:     data->barray10[8] |= 1 | (1 << 2);              break;          //5
        case 5:     data->barray10[8] |= (1 << 1) | (1 << 2);       break;          //6
        case 6:     data->barray10[8] |= 1 | (1 << 1) | (1 << 2);   break;          //7
        case 7:     data->barray10[8] |= (1 << 3);                  break;          //8
        case 0:     data->barray10[8] |= 1;                         break;          //1
        default: break;
    }

}

void MainWindow::on_open_shos_clicked()
// Функция открывает файл
// и заполняет table_shos
{

    QString path;
    QString line;

    QString byte0;
    QString byte1;
    QString byte6;
    QString byte7;
    QString byte8;
    QString byte9;

    QString priemq;
    QString peredachaq;

    Data_frch *data = new Data_frch;

    path = QFileDialog::getOpenFileName(this, "Выбрать файл", nullptr,"Hex-file (*.hex)");

    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
    {
         QMessageBox::information(this, "Внимение", "Файл не открывается!");
        return;
    }

    line = file.readLine();                                     //дефолтная первая строка пропускаем

    //рабочий чикл тут
   for (int i = 0; i < 10; ++i)
   {
    line = file.readLine();

    byte0 = QString(line[9]) + line[10];                                    //1
    byte1 = QString(line[11]) + line[12];                                   //2

    priemq = QString(line[13]) + line[14] + line[15]+ line[16];             //3-4
    peredachaq = QString(line[17]) + line[18] + line[19] + line[20];        //5-6

    byte6 = QString(line[21]) + line[22];                                   //7
    byte7 = QString(line[23]) + line[24];                                   //8
    byte8 = QString(line[25]) + line[26];                                   //9
    byte9 = QString(line[27]) + line[28];                                   //10

    int priem = priemq.toInt(nullptr,16);
    int peredacha = peredachaq.toInt(nullptr,16);

    memset(data->barray10, 0, sizeof(data->barray10));

    data->barray10[0] = byte0.toInt(nullptr,16);
    data->barray10[1] = byte1.toInt(nullptr,16);
    data->barray10[6] = byte6.toInt(nullptr,16);
    data->barray10[7] = byte7.toInt(nullptr,16);
    data->barray10[8] = byte8.toInt(nullptr,16);
    data->barray10[9] = byte9.toInt(nullptr,16);

       if (open_shos(i, data))
        {

        qobject_cast<QComboBox*> (ui->table_shos->cellWidget(i,3))->setCurrentIndex(peredacha);
        qobject_cast<QComboBox*> (ui->table_shos->cellWidget(i,4))->setCurrentIndex(priem);
        qobject_cast<QComboBox*> (ui->table_shos->cellWidget(i,8))->setCurrentIndex(data->barray10[3]);
        }

    }

    file.close();
    delete data;
    title_name(path);
}

bool MainWindow::open_shos(int row,Data_frch *data)
// Функция заполняет массив данными байт из файла
// возращает true если строка с полезной информацией не пуста
// возращает false если пуста
// проверка осуществляется просматривая 9 байт если он заполнен "1"-цами
// то значит строка пуста.
{

    uint8_t kbu = -1;                        // храним индексы
    uint8_t canal = -1;
    uint8_t radio;
    uint8_t coder;
    uint8_t speed;
    uint8_t nabor;
    uint8_t shos = 0;
    uint8_t psp = 0;
    uint8_t skk = 0;


    if (data->barray10[8] != 255)
    {
    if ((data->barray10[8] & (1 << 4)) != 0 && (data->barray10[8] & (1 << 5)) != 0)
            kbu = 0;

    if ((data->barray10[8] & (1 << 6)) != 0 && (data->barray10[8] & (1 << 7)) != 0)
            kbu = 1;

    if ((data->barray10[8] & 1) != 0)
            canal = 0;

    if ((data->barray10[8] & (1 << 1)) != 0)
            canal = 1;

    if ((data->barray10[8] & (1 << 1)) != 0 && (data->barray10[8] & 1) != 0)
            canal = 2;

    if ((data->barray10[8] & (1 << 2)) != 0)
            canal = 3;

    if ((data->barray10[8] & (1 << 2)) != 0 && (data->barray10[8] & 1) != 0)
            canal = 4;

    if ((data->barray10[8] & (1 << 2)) != 0 && (data->barray10[8] & (1 << 1)) != 0)
            canal = 5;

    if ((data->barray10[8] & (1 << 1)) != 0 && (data->barray10[8] & (1 << 1)) != 0
            && (data->barray10[8] & 1) != 0)
            canal = 6;

    if ((data->barray10[8] & (1 << 3)) != 0)
            canal = 7;

   /******************Радио линии************************/

    if ((data->barray10[6] & 1) != 0)
        radio = 0;

    if ((data->barray10[6] & (1 << 1)) != 0)
        radio = 1;

    if ((data->barray10[6] & (1 << 2)) != 0)
        radio = 2;

    /**************** Кодер **************************/

    if ((data->barray10[6] & (1 << 4)) != 0)
        coder = 1;

    if ((data->barray10[6] & (1 << 5)) != 0)
        coder = 2;

    if ((((data->barray10[6] & (1 << 4)) != 0)
         && ((data->barray10[6] & (1 << 5)) != 0)))
        coder = 3;

    if ((((data->barray10[6] & (1 << 4)) == 0)
         && ((data->barray10[6] & (1 << 5)) == 0)))
        coder = 0;

    /*********************** Скорость ******************************/
if (data->barray10[1] != 255)
    {
    if ((data->barray10[1] & 1) != 0)
        speed = 0;

    if ((data->barray10[1] & (1 << 1)) != 0)
        speed = 1;

    if ((data->barray10[1] & (1 << 2)) != 0)
        speed = 2;

    if ((data->barray10[1] & (1 << 3)) != 0)
        speed = 3;
    }

    /************************* Набор  ***********************/
    if ((data->barray10[0] & (1 << 4)) == 0 && (data->barray10[0] & (1 << 5)) == 0)
        nabor = 0;

    if ((data->barray10[0] & (1 << 4)) != 0)
        nabor = 1;

    if ((data->barray10[0] & (1 << 5)) != 0)
        nabor = 2;

    if ((data->barray10[0] & (1 << 4)) != 0
            && (data->barray10[0] & (1 << 5)) != 0)
        nabor = 3;

    /************************* СХОС *****************************/
    if ((data->barray10[0] & 1) == 0)
            shos = 0;

    if ((data->barray10[0] & 1) != 0)
            shos = 1;

    if ((data->barray10[0] & (1 << 1)) != 0)
            shos = 2;

    if ((data->barray10[0] & (1 << 1)) != 0
            && (data->barray10[0] & 1) != 0)
            shos = 3;

    if ((data->barray10[0] & (1 << 2)) != 0)
            shos = 4;

    if ((data->barray10[0] & (1 << 2)) != 0 && (data->barray10[0] & 1) == 1)
            shos = 5;

    if ((data->barray10[0] & (1 << 2)) != 0 && (data->barray10[0] & (1 << 1)) != 0)
            shos = 6;

    if ((data->barray10[0] & (1 << 2)) != 0
            && (data->barray10[0] & (1 << 1)) != 0
                && (data->barray10[0] & 1) != 0)
            shos = 7;

    if ((data->barray10[0] & (1 << 3)) != 0)
            shos = 8;

    if ((data->barray10[0] & (1 << 3)) != 0 && (data->barray10[0] & 1) != 0)
            shos = 9;


    /****************************** PSP ************************/

    if ((data->barray10[7] & 1) != 0)
            psp += 1;

    if ((data->barray10[7] & (1 << 1)) != 0)
            psp += 2;

    if ((data->barray10[7] & (1 << 2)) != 0)
            psp += 4;

    if ((data->barray10[7] & (1 << 3)) != 0)
            psp += 8;

    if ((data->barray10[7] & (1 << 4)) != 0)
            psp += 16;

    if ((data->barray10[7] & (1 << 5)) != 0)
            psp += 32;

    if ((data->barray10[7] & (1 << 6)) != 0)
            psp += 64;


    // создает
    on_table_canals_currentCellChanged(row,0,0,0);
    on_add_shos_clicked();


    //заполняем

    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,0))->setCurrentIndex(nabor);
    insert_shos_radio(nabor);

    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,1))->setCurrentIndex(shos);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,2))->setCurrentIndex(radio);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,5))->setCurrentIndex(coder);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,6))->setCurrentIndex(speed);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,7))->setCurrentIndex(canal);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,9))->setCurrentIndex(kbu);
    qobject_cast<QComboBox*> (ui->table_shos->cellWidget(row,10))->setCurrentIndex(skk);

    data->barray10[3] = psp;

    return true;
    }

    return false;
}



void MainWindow::on_pushButton_7_clicked()
{
  ui->lineEdit_2->setText("AYAYAYAY");
  ui->lineEdit->setText("AYAYAYAY");
}
