#ifndef TEST_EDITOR_HPP
#define TEST_EDITOR_HPP

#include <QWidget>

namespace Ui {
class pagedlod_editor;
}

namespace osg {
    class PagedLOD;
}

class pagedlod_editor : public QWidget
{
    Q_OBJECT
    
public:
    explicit pagedlod_editor(osg::PagedLOD* node, QWidget *parent = 0);
    ~pagedlod_editor();

public slots:
    void accept();
    void reject();
    
private:
    Ui::pagedlod_editor *ui;
    osg::PagedLOD* _node;
};

#endif // TEST_EDITOR_HPP
