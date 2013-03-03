#ifndef GROUP_EDITOR_HPP
#define GROUP_EDITOR_HPP

#include <QWidget>

namespace Ui {
    class group_editor;
}

namespace osg {
    class Group;
}

class group_editor : public QWidget
{
    Q_OBJECT
    
public:
    explicit group_editor(osg::Group* node, QWidget *parent = 0);
    ~group_editor();

public slots:
    void accept();
    void reject();
    
private:
    Ui::group_editor *ui;
    osg::Group* _node;
};

#endif // GROUP_EDITOR_HPP
