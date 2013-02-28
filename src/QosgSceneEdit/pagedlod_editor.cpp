#include "pagedlod_editor.hpp"
#include "ui_pagedlod_editor.h"

#include <osg/PagedLOD>
#include <osg/BoundingBox>

#include <iostream>

using namespace osg;
using namespace std;

pagedlod_editor::pagedlod_editor(PagedLOD* node, QWidget *parent) :
    _node(node),
    QWidget(parent),
    ui(new Ui::pagedlod_editor)
{
    ui->setupUi(this);

    connect(ui->_buttonbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->_buttonbox, SIGNAL(rejected()), this, SLOT(reject()));

    // Name
    QString name = QString::fromStdString(_node->getName());
    ui->_label_name->setText( name == "" ? "<unknow name>" : name );

    // Children
    if (_node->getNumChildren() == 2)
    {
        QString name1 = QString::fromStdString(_node->getFileName(0));
        ui->_label_children_1->setText( name1 == "" ? "<unknow children name>" : name1 );
        QString name2 = QString::fromStdString(_node->getFileName(1));
        ui->_label_children_2->setText( name2 == "" ? "<unknow children name>" : name2 );
    }
    else
    {
        ui->_label_children_1->setText("Unable to display if PagedLOD has less or more than 2 children ...");
        ui->_label_children_2->setText("Unable to display if PagedLOD has less or more than 2 children ...");
    }

    // Ranges
    PagedLOD::RangeList ranges = _node->getRangeList();

    ui->_lineedit_rangemin1->setText( QString::number(ranges[0].first) );
    ui->_lineedit_rangemax1->setText( QString::number(ranges[0].second) );
    ui->_lineedit_rangemin2->setText( QString::number(ranges[1].first) );
    ui->_lineedit_rangemax2->setText( QString::number(ranges[1].second) );

    // Bounds
    // 1) Radius
    ui->_lineedit_radius->setText( QString::number(_node->getRadius()) );
    // 2) Center Mode
     LOD::CenterMode centermode = _node->getCenterMode();
     if ( centermode == osg::LOD::USE_BOUNDING_SPHERE_CENTER )
         ui->_combobox_centermode->setCurrentIndex(0);
     else if ( centermode == osg::LOD::USER_DEFINED_CENTER )
         ui->_combobox_centermode->setCurrentIndex(1);
     // 3) Center coordinates
     osg::LOD::vec_type center = _node->getCenter();
     ui->_lineedit_center_x->setText( QString::number(center[0]) );
     ui->_lineedit_center_y->setText( QString::number(center[1]) );
     ui->_lineedit_center_z->setText( QString::number(center[2]) );

     // Bounding box
     BoundingBox bbox;
     bbox.expandBy(node->getBound());
     if (bbox.valid())
     {
         ui->_lineedit_bb_min_x->setText( QString::number(bbox._min.x(), 'g', 10) );
         ui->_lineedit_bb_min_y->setText( QString::number(bbox._min.y(), 'g', 10) );
         ui->_lineedit_bb_min_z->setText( QString::number(bbox._min.z(), 'g', 10) );
         ui->_lineedit_bb_max_x->setText( QString::number(bbox._max.x(), 'g', 10) );
         ui->_lineedit_bb_max_y->setText( QString::number(bbox._max.y(), 'g', 10) );
         ui->_lineedit_bb_max_z->setText( QString::number(bbox._max.z(), 'g', 10) );

         ui->_lineedit_bb_size_x->setText( QString::number(bbox._max.x()-bbox._min.x(), 'g', 10) );
         ui->_lineedit_bb_size_y->setText( QString::number(bbox._max.y()-bbox._min.y(), 'g', 10) );
         ui->_lineedit_bb_size_z->setText( QString::number(bbox._max.z()-bbox._min.z(), 'g', 10) );
     }
}

pagedlod_editor::~pagedlod_editor()
{
    delete ui;
}

void pagedlod_editor::accept()
{
    cout << __FUNCTION__ << endl;
}

void pagedlod_editor::reject()
{
    cout << __FUNCTION__ << endl;
}
