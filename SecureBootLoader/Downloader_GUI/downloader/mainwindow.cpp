#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->downloadProgressBar->setValue(0);
    this->get_ComPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchFile_triggered(QAction *arg1)
{

}

void MainWindow::on_searchFile_clicked()
{
    /*trigger the file search */
    this->FileName = QFileDialog::getOpenFileName(this,"Open .bin file",
                                                  "C:\\Users\\abhishek.pandey.TELXSI\\e2_studio\\Manual_test\\",
                                                  "*.bin");
    ui->fileLocation->setPlainText(this->FileName);

}

void MainWindow::on_selecFile_clicked()
{
    /* Open the selected file */
    FILE *fp = fopen(this->FileName.toStdString().c_str(),"rb");
    if(fp == NULL)
    {
        QMessageBox::information(this,"Error","Error in opening the file");
    }
    else
    {
        fseek (fp , 0 , SEEK_END);
        this->FileSize = ftell (fp);
        rewind(fp);
        QMessageBox::information(this,"File Size",QString::number(this->FileSize));
        ui->downloadProgressBar->setValue(0);
    }
}


void MainWindow::on_downloadProgressBar_valueChanged(int value)
{
    ui->downloadProgressBar->setValue(value);
}

void MainWindow::on_SelectComPort_clicked()
{

}

void MainWindow ::get_ComPorts()
{
    HANDLE hcom;
    char  ComPortName[15] = "\\\\.\\COM";  // Name of the Serial port(May Change) to be opened,
    char port_num[5];
    uint8_t run = 0;
    DWORD last_err;
    LPCWSTR port  ;
    ui->comboBox->addItem("add_to_box");
    while(run < 100)
    {
        memset(ComPortName,0x00,15);
        memcpy(ComPortName,"\\\\.\\COM",10);
        sprintf(port_num,"%d",run);
        strcat(ComPortName,port_num);
        /* Check only 100 com ports */

        hcom = CreateFile((LPCWSTR)"\\\\.\\COM13",    	//port name
                            GENERIC_READ | GENERIC_WRITE, 	//Read/Write
                            0,                            	// No Sharing
                            NULL,                         	// No Security
                            OPEN_EXISTING,					// Open existing port only
                            0,            					// Non Overlapped I/O
                            NULL);
        if(hcom == INVALID_HANDLE_VALUE)
        {
            last_err = GetLastError();
            if(last_err == ERROR_FILE_NOT_FOUND )
            {

            }
        }
        else
        {
            /* add the value to the list */
            QString add_to_box = ComPortName;
            ui->comboBox->addItem(add_to_box);
        }
        CloseHandle(hcom);
        run++;
    }
}
