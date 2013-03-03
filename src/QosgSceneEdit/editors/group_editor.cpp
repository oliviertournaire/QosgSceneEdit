#include "group_editor.hpp"
#include "ui_group_editor.h"

#include <osg/Group>

#include <iostream>

using namespace osg;
using namespace std;

group_editor::group_editor(Group* node, QWidget *parent) :
    _node(node),
    QWidget(parent),
    ui(new Ui::group_editor)
{
    ui->setupUi(this);

    ui->_label_num_children->setText( QString::number(_node->getNumChildren()) );
}

group_editor::~group_editor()
{
    delete ui;
}

void group_editor::accept()
{
    cout << __FUNCTION__ << endl;
}

void group_editor::reject()
{
    cout << __FUNCTION__ << endl;
}
