#ifndef ADVANCED_H
#define ADVANCED_H

#include <QDialog>
#include "grizzlypp.h"

namespace Ui {
class Advanced;
}

class Advanced : public QDialog
{
    Q_OBJECT
    Grizzly** current;
    QTimer* timer;
    static unsigned char drive_mode[3];
    static unsigned char pid_mode[2];

public:
    explicit Advanced(QWidget *parent = 0, Grizzly** curr = NULL);
    unsigned char dmode;
    unsigned char cmode;
    ~Advanced();

private slots:
    void show_feedback();

    void on_dmodeselect_activated(int index);

    void on_pidmodeselect_activated(int index);

    void on_writesend_clicked();

    void on_readsend_clicked();

    void on_closebtn_clicked();

protected:

private:
    Ui::Advanced *ui;
};

#endif // ADVANCED_H
