#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStyleFactory>
#include <QMainWindow>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    double revalue_tank(double Q_in,double Q_out,double tank_value,double tank_max);
    void Vec_File(bool estado);

public slots:
    void uptdateQoutindicator_princ(int value);
    void uptdateQinindicator_princ(int value);
    void uptdateQoutindicator_1(int value);
    void uptdateQoutindicator_2(int value);
    void updateTankLevel();
    void start();
    void caudal_animation();
    void safe_arch();
    void load_arch();
    void Tank_princ_max_value(double value);
    void Tank_1_max_value(double value);
    void Tank_2_max_value(double value);
    void Q_in_max_value (double value);
    void Q_out_prin_max_value (double value);
    void Q_out_1_max_value (double value);
    void Q_out_2_max_value (double value);
private:

private:
    Ui::MainWindow *ui;
    QTimer timer;
    QTimer timer_2;
    double V[21];
    int size = 21;
    double Tank_value_prin;
    double Tank_value_1;
    double Tank_value_2;
};
#endif // MAINWINDOW_H
