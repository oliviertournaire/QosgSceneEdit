#include "matrixtransform_editor.hpp"
#include "ui_matrixtransform_editor.h"

#include <osg/MatrixTransform>

#include <iostream>

using namespace osg;
using namespace std;

matrixtransform_editor::matrixtransform_editor(MatrixTransform* node, QWidget *parent) :
    _node(node),
    QWidget(parent),
    ui(new Ui::matrixtransform_editor)
{
    ui->setupUi(this);

    connect(ui->_buttonbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->_buttonbox, SIGNAL(rejected()), this, SLOT(reject()));

    const Matrix& m = _node->getMatrix();

    ui->_lineedit_00->setText( QString::number(m(0,0), 'g', 10) );
    ui->_lineedit_01->setText( QString::number(m(0,1), 'g', 10) );
    ui->_lineedit_02->setText( QString::number(m(0,2), 'g', 10) );
    ui->_lineedit_03->setText( QString::number(m(0,3), 'g', 10) );
    ui->_lineedit_10->setText( QString::number(m(1,0), 'g', 10) );
    ui->_lineedit_11->setText( QString::number(m(1,1), 'g', 10) );
    ui->_lineedit_12->setText( QString::number(m(1,2), 'g', 10) );
    ui->_lineedit_13->setText( QString::number(m(1,3), 'g', 10) );
    ui->_lineedit_20->setText( QString::number(m(2,0), 'g', 10) );
    ui->_lineedit_21->setText( QString::number(m(2,1), 'g', 10) );
    ui->_lineedit_22->setText( QString::number(m(2,2), 'g', 10) );
    ui->_lineedit_23->setText( QString::number(m(2,3), 'g', 10) );
    ui->_lineedit_30->setText( QString::number(m(3,0), 'g', 10) );
    ui->_lineedit_31->setText( QString::number(m(3,1), 'g', 10) );
    ui->_lineedit_32->setText( QString::number(m(3,2), 'g', 10) );
    ui->_lineedit_33->setText( QString::number(m(3,3), 'g', 10) );
}

matrixtransform_editor::~matrixtransform_editor()
{
    delete ui;
}

void matrixtransform_editor::accept()
{
    cout << __FUNCTION__ << endl;
}

void matrixtransform_editor::reject()
{
    cout << __FUNCTION__ << endl;
}
