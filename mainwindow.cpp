#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    connect(ui->Dial_Q_in_tanq_princ,SIGNAL(sliderMoved(int)),this,SLOT(uptdateQinindicator_princ(int)));
    connect(ui->Dial_Q_out_tanq_princ,SIGNAL(sliderMoved(int)),this,SLOT(uptdateQoutindicator_princ(int))); //tanque principal
    connect(ui->Dial_Q_out_tanq_1,SIGNAL(sliderMoved(int)),this,SLOT(uptdateQoutindicator_1(int)));   //tanque 1
    connect(ui->Dial_Q_out_tanq_2,SIGNAL(sliderMoved(int)),this,SLOT(uptdateQoutindicator_2(int)));   //tanque 2
    connect(ui->check_arranque,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->actionSave_File,SIGNAL(triggered(bool)),this,SLOT(safe_arch()));
    connect(ui->actionLoad_File,SIGNAL(triggered(bool)),this,SLOT(load_arch()));
    connect(ui->tank_max_princ,SIGNAL(valueChanged(double)),this,SLOT(Tank_princ_max_value()));
    connect(ui->tank_max_1,SIGNAL(valueChanged(double)),this,SLOT(Tank_1_max_value(double)));
    connect(ui->tank_max_2,SIGNAL(valueChanged(double)),this,SLOT(Tank_2_max_value(double)));
    connect(ui->Q_in_max_tanq_prin,SIGNAL(valueChanged(double)),this,SLOT(Q_in_max_value(double)));
    connect(ui->Q_out_Max_tanq_princ,SIGNAL(valueChanged(double)),this,SLOT(Q_out_prin_max_value(double)));
    connect(ui->Q_out_tanq_1,SIGNAL(valueChanged(double)),this,SLOT(Q_out_1_max_value(double)));
    connect(ui->Q_out_tanq_2,SIGNAL(valueChanged(double)),this,SLOT(Q_out_2_max_value(double)));
    timer.start(1000);
    timer_2.start(500);
    ui->Dial_Q_in_tanq_princ->setEnabled(false);
    ui->Dial_Q_out_tanq_princ->setEnabled(false);
    ui->Dial_Q_out_tanq_1->setEnabled(false);
    ui->Dial_Q_out_tanq_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::revalue_tank(double Q_in, double Q_out, double tank_value,double tank_max)
{
    double tank_revalue;

    if(Q_in - Q_out <= tank_max - tank_value){
        tank_revalue = ( tank_value + Q_in - Q_out);
    }
    else {
        tank_revalue = tank_max;

    }
    return tank_revalue;
}

void MainWindow::Vec_File(bool estado)
{
    if(estado == true){
        V[0] = ui->Dial_Q_in_tanq_princ->value();
        V[1] = ui->Q_in_max_tanq_prin->value();
        V[2] = ui->tanque_principal->value();
        V[3] = ui->tank_max_princ->value();
        V[4] = ui->Dial_Q_out_tanq_princ->value();
        V[5] = ui->Q_out_Max_tanq_princ->value();
        V[6] = ui->tanque_1->value();
        V[7] = ui->tank_max_1->value();
        V[8] = ui->tanque_2->value();
        V[9] = ui->tank_max_2->value();
        V[10] = ui->Dial_Q_out_tanq_1->value();
        V[11] = ui->Q_out_tanq_1->value();
        V[12] = ui->Dial_Q_out_tanq_2->value();
        V[13] = ui->Q_out_tanq_2->value();
        V[14] = ui->Disp_Q_in_tanq_prin->value();
        V[15] = ui->Disp_Q_out_tanq_prin->value();
        V[16] = ui->Disp_Q_out_tanq_1->value();
        V[17] = ui->Disp_Q_out_tanq_2->value();
        V[18] = ui->check_tanq_1->checkState();
        V[19] = ui->check_tanq_2->checkState();
        V[20] = ui->check_arranque->checkState();

    }
    if(estado == false){

        ui->Dial_Q_in_tanq_princ->setValue(V[0]);
        ui->Q_in_max_tanq_prin->setValue(V[1]);
        ui->tanque_principal->setValue(V[2]);
        ui->tank_max_princ->setValue(V[3]);
        ui->Dial_Q_out_tanq_princ->setValue(V[4]);
        ui->Q_out_Max_tanq_princ->setValue(V[5]);
        ui->tanque_1->setValue(V[6]);
        ui->tank_max_1->setValue(V[7]);
        ui->tanque_2->setValue(V[8]);
        ui->tank_max_2->setValue(V[9]);
        ui->Dial_Q_out_tanq_1->setValue(V[10]);
        ui->Q_out_tanq_1->setValue(V[11]);
        ui->Dial_Q_out_tanq_2->setValue(V[12]);
        ui->Q_out_tanq_2->setValue(V[13]);
        ui->Disp_Q_in_tanq_prin->display(V[14]);
        ui->Disp_Q_out_tanq_prin->display(V[15]);
        ui->Disp_Q_out_tanq_1->display(V[16]);
        ui->Disp_Q_out_tanq_2->display(V[17]);
        if(V[18] == 2){
            ui->check_tanq_1->setChecked(true);}
        if(V[18] == 0){
            ui->check_tanq_1->setChecked(false);}
        if(V[19] == 2){
            ui->check_tanq_2->setChecked(true);}
        if(V[19] == 0){
            ui->check_tanq_2->setChecked(false);}
        if(V[20] == 2){
            ui->check_arranque->setChecked(true);
            start();}
        if(V[20] == 0){
            ui->check_arranque->setChecked(false);
            start();}
    }
}
// https://github.com/mateoislas21/ejercicio-evaluativo.git
void MainWindow::start()
{
    if(ui->check_arranque->isChecked() == true){
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTankLevel()));
    connect(&timer_2,SIGNAL(timeout()),this,SLOT(caudal_animation()));
    double Q_in_max_princ = ui->Q_in_max_tanq_prin->value();
    double Q_out_max_princ = ui->Q_out_Max_tanq_princ->value();
    double Q_out_max_1 = ui->Q_out_tanq_1->value();
    double Q_out_max_2 = ui->Q_out_tanq_2->value();
    ui->Dial_Q_in_tanq_princ->setMaximum(Q_in_max_princ);
    ui->Dial_Q_out_tanq_princ->setMaximum(Q_out_max_princ);
    ui->Dial_Q_out_tanq_1->setMaximum(Q_out_max_1);
    ui->Dial_Q_out_tanq_2->setMaximum(Q_out_max_2);
    ui->Dial_Q_in_tanq_princ->setEnabled(true);
    ui->Dial_Q_out_tanq_princ->setEnabled(true);
    ui->Dial_Q_out_tanq_1->setEnabled(true);
    ui->Dial_Q_out_tanq_2->setEnabled(true);
    ui->check_arranque->setText("turn off");
    }
    else{
        ui->Dial_Q_in_tanq_princ->setEnabled(false);
        ui->Dial_Q_out_tanq_princ->setEnabled(false);
        ui->Dial_Q_out_tanq_1->setEnabled(false);
        ui->Dial_Q_out_tanq_2->setEnabled(false);
        disconnect(&timer,SIGNAL(timeout()),this,SLOT(updateTankLevel()));
        disconnect(&timer_2,SIGNAL(timeout()),this,SLOT(caudal_animation()));
        ui->check_arranque->setText("turn on");
        ui->barra_1->setValue(0);
        ui->barra_2->setValue(0);
        ui->barra_3->setValue(0);
        ui->barra_4->setValue(0);
        ui->barra_5->setValue(0);
        ui->barra_6->setValue(0);
        ui->barra_ver_1->setValue(0);
        ui->barra_ver_2->setValue(0);
    }
}

void MainWindow::Tank_princ_max_value(double value)
{
    ui->tanque_principal->setMaximum(value);
}

void MainWindow::Tank_1_max_value(double value)
{
    ui->tanque_1->setMaximum(value);
}

void MainWindow::Tank_2_max_value(double value)
{
    ui->tank_max_2->setMaximum(value);
}

void MainWindow::Q_in_max_value(double value)
{
    ui->Dial_Q_in_tanq_princ->setMaximum(value);
}

void MainWindow::Q_out_prin_max_value(double value)
{
    ui->Dial_Q_out_tanq_princ->setMaximum(value);
}

void MainWindow::Q_out_1_max_value(double value)
{
    ui->Dial_Q_out_tanq_1->setMaximum(value);
}

void MainWindow::Q_out_2_max_value(double value)
{
    ui->Dial_Q_out_tanq_2->setMaximum(value);
}

void MainWindow::uptdateQoutindicator_princ(int value)
{
    ui->Disp_Q_out_tanq_prin->display(value);
}

void MainWindow::uptdateQinindicator_princ(int value)
{
    ui->Disp_Q_in_tanq_prin->display(value);

}

void MainWindow::uptdateQoutindicator_1(int value)
{
    ui->Disp_Q_out_tanq_1->display(value);
}

void MainWindow::uptdateQoutindicator_2(int value)
{
    ui->Disp_Q_out_tanq_2->display(value);
}

void MainWindow::updateTankLevel()
{
    double Q_in_princ =  ui->Dial_Q_in_tanq_princ->value()/3600;
    double Q_Out_princ = ui->Dial_Q_out_tanq_princ->value()/3600;
    double Tank_value_princ = ui->tanque_principal->value();
    double tank_max = ui->tanque_principal->maximum();
    double Q_out_1 = ui->Dial_Q_out_tanq_1->value()/3600;
    double Q_out_2 = ui->Dial_Q_out_tanq_2->value()/3600;
    double Tank_value_1 = ui->tanque_1->value();
    double Tank_value_2 = ui->tanque_2->value();
    int bandera_1 = 0, bandera_2 = 0;
//tanque 1
    if(ui->check_tanq_1->isChecked() == false && ui->tanque_1->value() != ui->tanque_1->maximum()){
        if(ui->check_tanq_2->isChecked() == false && ui->tanque_2->value() != ui->tanque_2->maximum()){
        Tank_value_1 = revalue_tank(Q_Out_princ/2,Q_out_1,Tank_value_1,tank_max/2);
            if(Tank_value_1 <= ui->tank_max_1->value()*0.1){
                ui->Dial_Q_out_tanq_1->setValue(0);
                ui->Disp_Q_out_tanq_1->display(0);
                ui->Dial_Q_out_tanq_1->setEnabled(false);
            }
            else{
                ui->Dial_Q_out_tanq_1->setEnabled(true);
            }
            ui->tanque_1->setValue(Tank_value_1);

        }
        else{
            Tank_value_1 = revalue_tank(Q_Out_princ,Q_out_1,Tank_value_1,tank_max/2);
            if(Tank_value_1 <= ui->tank_max_1->value()*0.1){
                ui->Dial_Q_out_tanq_1->setValue(0);
                ui->Dial_Q_out_tanq_1->setEnabled(false);
                ui->Disp_Q_out_tanq_1->display(0);
            }
            else{
                ui->Dial_Q_out_tanq_1->setEnabled(true);
            }
            ui->tanque_1->setValue(Tank_value_1);

        }
        ui->check_tanq_1->setText("on");
    }else{
        Tank_value_1 = revalue_tank(0,Q_out_1,Tank_value_1,tank_max/2);
        if(Tank_value_1 <= ui->tank_max_1->value()*0.1){
            ui->Dial_Q_out_tanq_1->setValue(0);
            ui->Dial_Q_out_tanq_1->setEnabled(false);
            ui->Disp_Q_out_tanq_1->display(0);
        }
        else{
                ui->Dial_Q_out_tanq_1->setEnabled(true);
        }
        ui->tanque_1->setValue(Tank_value_1);
        ui->check_tanq_1->setText("off");
        bandera_1 = 1;
    }
//tanque 2
    if(ui->check_tanq_2->isChecked() == false && ui->tanque_2->value() != ui->tanque_2->maximum()){
        if(ui->check_tanq_1->isChecked() == false && ui->tanque_1->value() != ui->tanque_1->maximum()){
            Tank_value_2 = revalue_tank(Q_Out_princ/2,Q_out_2,Tank_value_2,tank_max/2);
            if(Tank_value_2 <= ui->tank_max_2->value()*0.1){
                ui->Dial_Q_out_tanq_2->setValue(0);
                ui->Dial_Q_out_tanq_2->setEnabled(false);
                ui->Disp_Q_out_tanq_2->display(0);
            }
            else{
                ui->Dial_Q_out_tanq_2->setEnabled(true);
            }
            ui->tanque_2->setValue(Tank_value_2);
        }
        else{
            Tank_value_2 = revalue_tank(Q_Out_princ,Q_out_2,Tank_value_2,tank_max/2);
            if(Tank_value_2 <= ui->tank_max_2->value()*0.1){
                ui->Dial_Q_out_tanq_2->setValue(0);
                ui->Dial_Q_out_tanq_2->setEnabled(false);
                ui->Disp_Q_out_tanq_2->display(0);
            }
            else{
                ui->Dial_Q_out_tanq_2->setEnabled(true);
            }
            ui->tanque_2->setValue(Tank_value_2);
        }
           ui->check_tanq_2->setText("on");
    }else{
    Tank_value_2 = revalue_tank(0,Q_out_2,Tank_value_2,tank_max/2);
        if(Tank_value_2 <= ui->tank_max_2->value()*0.1){
          ui->Dial_Q_out_tanq_2->setValue(0);
            ui->Dial_Q_out_tanq_2->setEnabled(false);
          ui->Disp_Q_out_tanq_2->display(0);
        }
        else{
            ui->Dial_Q_out_tanq_2->setEnabled(true);
        }
    ui->tanque_2->setValue(Tank_value_2);
       ui->check_tanq_2->setText("off");
    bandera_2 = 1;
    }
    if (bandera_1 == 1 && bandera_2 == 1){
        ui->Dial_Q_out_tanq_princ->setValue(0);
        ui->Dial_Q_out_tanq_princ->setEnabled(false);
        ui->Disp_Q_out_tanq_prin->display(0);
        Q_Out_princ = 0;
    }
    else{
        ui->Dial_Q_out_tanq_princ->setEnabled(true);
    }

    //tanque principal
    Tank_value_princ = revalue_tank(Q_in_princ,Q_Out_princ,Tank_value_princ,tank_max);
    if(Tank_value_princ <= tank_max*0.1){
        ui->Dial_Q_out_tanq_princ->setValue(0);
        ui->Dial_Q_out_tanq_princ->setEnabled(false);
        ui->Disp_Q_out_tanq_prin->display(0);
    }
    else{
        ui->Dial_Q_in_tanq_princ->setEnabled(true);
    }
    if(Tank_value_princ == tank_max){
        ui->Dial_Q_in_tanq_princ->setValue(0);
        ui->Dial_Q_in_tanq_princ->setEnabled(false);
        ui->Disp_Q_in_tanq_prin->display(0);
    }
    else{
        ui->Dial_Q_in_tanq_princ->setEnabled(true);

    }
    ui->tanque_principal->setValue(Tank_value_princ);
}
void MainWindow::caudal_animation()
{
    if(ui->Dial_Q_in_tanq_princ->value() != 0 &&  ui->barra_1->value() != ui->barra_1->maximum()){
        ui->barra_1->setValue(ui->barra_1->value() + 5);
    }
    else{
        ui->barra_1->setValue(0);
    }


    if(ui->Dial_Q_out_tanq_princ->value() != 0 &&  ui->barra_2->value() != ui->barra_2->maximum()){
        ui->barra_2->setValue(ui->barra_2->value() + 5);
    }
    else{
        ui->barra_2->setValue(0);
    }


    if(ui->Dial_Q_out_tanq_princ->value() != 0 &&  ui->barra_ver_1->value() != ui->barra_ver_1->maximum() && ui->check_tanq_1->isChecked() != true &&
        ui->tanque_1->value() != ui->tanque_1->maximum() && ui->barra_2->value() == ui->barra_2->maximum()){
        ui->barra_ver_1->setValue(ui->barra_ver_1->value() + 10);
    }
    else{
        ui->barra_ver_1->setValue(0);
    }

    if(ui->Dial_Q_out_tanq_princ->value() != 0 &&  ui->barra_ver_2->value() != ui->barra_ver_2->maximum() && ui->check_tanq_2->isChecked() != true &&
        ui->tanque_2->value() != ui->tanque_1->maximum() && ui->barra_2->value() == ui->barra_2->maximum()){
        ui->barra_ver_2->setValue(ui->barra_ver_2->value() + 10);
    }
    else{
        ui->barra_ver_2->setValue(0);
    }

    if(ui->Dial_Q_out_tanq_princ->value() != 0 &&  ui->barra_3->value() != ui->barra_3->maximum() && ui->check_tanq_1->isChecked() != true &&
        ui->tanque_1->value() != ui->tanque_1->maximum() && ui->barra_ver_1->value() == ui->barra_ver_1->maximum()){
        ui->barra_3->setValue(ui->barra_3->value() + 10);
    }
    else{
        ui->barra_3->setValue(0);
    }

    if(ui->Dial_Q_out_tanq_princ->value() != 0 &&  ui->barra_4->value() != ui->barra_4->maximum() && ui->check_tanq_2->isChecked() != true &&
        ui->tanque_2->value() != ui->tanque_1->maximum() && ui->barra_ver_2->value() == ui->barra_ver_2->maximum()){
        ui->barra_4->setValue(ui->barra_4->value() + 10);
    }
    else{
        ui->barra_4->setValue(0);
    }

    if(ui->Dial_Q_out_tanq_1->value() != 0 &&  ui->barra_5->value() != ui->barra_5->maximum()){
        ui->barra_5->setValue(ui->barra_5->value() + 5);
    }
    else{
        ui->barra_5->setValue(0);
    }

    if(ui->Dial_Q_out_tanq_2->value() != 0 &&  ui->barra_6->value() != ui->barra_6->maximum()){
        ui->barra_6->setValue(ui->barra_6->value() + 5);
    }
    else{
        ui->barra_6->setValue(0);
    }
}

void MainWindow::safe_arch()
{
    Vec_File(true);
    FILE *F = fopen("arch_tank.dat","wb");
    if(F == nullptr){
        std::cout << "error en guardar archivo";
    }else{
    fwrite(this->V,sizeof(double),size,F);
    fclose(F);
    }
}

void MainWindow::load_arch()
{

    FILE *F = fopen("arch_tank.dat","rb");
    if(F == nullptr){
        std::cout << "error al leer el archivo";
        Vec_File(true);
    }else{
    fread(V,sizeof(double),size,F);
    fclose(F);
    Vec_File(false);
    }

}


