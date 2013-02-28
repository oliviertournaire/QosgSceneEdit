#include "pagedlod_editor.hpp"
#include "ui_pagedlod_editor.h"

#include <osg/PagedLOD>

using namespace osg;

pagedlod_editor::pagedlod_editor(PagedLOD* node, QWidget *parent) :
    _node(node),
    QWidget(parent),
    ui(new Ui::pagedlod_editor)
{
    ui->setupUi(this);

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
}

pagedlod_editor::~pagedlod_editor()
{
    delete ui;
}
