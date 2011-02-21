#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "../../src/engine/dataengine.h"

using namespace DataEngine;

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void changeEvent(QEvent *e);

private slots:
    void selectInputDB();
    void selectOutputDB();

private:
    void connectSignalsAndSlots();
    void autoGenerateOutputPath();

private:
    Ui::Widget * const ui;
    Task * const task;
};

#endif // WIDGET_H
