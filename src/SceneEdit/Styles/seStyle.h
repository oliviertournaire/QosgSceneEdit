#include <QPlastiqueStyle>
#include <QPalette>

class seStyle : public QPlastiqueStyle
{
public:

	seStyle();
	virtual ~seStyle();

	virtual void polish(QApplication *application);
	virtual void unpolish(QApplication *application);
	virtual void drawControl(ControlElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0) const;
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;
	virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = 0) const;

private:

	QPalette _oldPalette;
};