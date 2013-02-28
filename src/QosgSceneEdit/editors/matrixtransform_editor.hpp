#ifndef MATRIXTRANSFORM_EDITOR_HPP
#define MATRIXTRANSFORM_EDITOR_HPP

#include <QWidget>

namespace Ui {
class matrixtransform_editor;
}

namespace osg {
    class MatrixTransform;
}

class matrixtransform_editor : public QWidget
{
    Q_OBJECT
    
public:
    explicit matrixtransform_editor(osg::MatrixTransform* node, QWidget *parent = 0);
    ~matrixtransform_editor();

public slots:
    void accept();
    void reject();
    
private:
    Ui::matrixtransform_editor *ui;
    osg::MatrixTransform* _node;
};

#endif // MATRIXTRANSFORM_EDITOR_HPP
