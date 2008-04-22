#include <QApplication>
#include <QTabBar.h>
#include <QStyleOptionTab>
#include <QPainter>
#include <QPolygon>
#include <QPixmapCache>

#include "seStyle.h"


static bool UsePixmapCache = true;
static const bool AnimateBusyProgressBar = true;
static const bool AnimateProgressBar = false;
// #define QPlastique_MaskButtons
static const int ProgressBarFps = 25;
static const int blueFrameWidth =  2;  // with of line edit focus frame

static const char * const qt_simple_toolbarhandle[] = {
    "3 3 4 1",
    "       c None",
    ".      c #C5C5C5",
    "+      c #EEEEEE",
    "@      c #FAFAFA",
    ".. ",
    ".+@",
    " @@"};

static const char *Maximize_xpm[] = {
/* width height num_colors chars_per_pixel */
"22 22 13 2",
/* colors */
"`` c None",
"`. c #595858",
"`# c #737b85",
"`a c #747b86",
"`b c #767e89",
"`c c #7b848f",
"`d c #828c98",
"`e c #000000",
"`f c #87919e",
"`g c #8993a0",
"`h c #88929f",
"`i c #8a95a3",
"`j c #8b96a4",
/* pixels */
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"`````````.`.`.`.`.`.`.`.`.`.`.`.`.``````````",
"`````````#`#`#`#`#`#`#`#`#`#`#`#`#``````````",
"`````````a`a`a`a`a`a`a`a`a`a`a`a`a``````````",
"`````````b`b`b`b`b`b`b`b`b`b`b`b`b``````````",
"`````````c`c`c`c`c`c`c`c`c`c`c`c`c``````````",
"`````````d`d`e`e`e`e`e`e`e`e`e`d`d``````````",
"`````````f`f```````````````````f`f``````````",
"`````````g`g```````````````````g`g``````````",
"`````````h`h```````````````````h`h``````````",
"`````````i`i`i`i`i`i`i`i`i`i`i`i`i``````````",
"`````````j`j`j`j`j`j`j`j`j`j`j`j`j``````````",
"`````````e`e`e`e`e`e`e`e`e`e`e`e`e``````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````"
};

static const char *Office2007_FrameCaptionClose23_xpm[] = {
/* width height num_colors chars_per_pixel */
"    22    22      11            2",
/* colors */
"`` c None",
"`. c #6d6c6c",
"`# c #737b85",
"`a c #767e89",
"`b c #f0f0f0",
"`c c #7b838e",
"`d c #7f8894",
"`e c #838d9a",
"`f c #88929f",
"`g c #141414",
"`h c #8b96a4",
/* pixels */
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"`````````.`.`.`.```````````.`.`.`.``````````",
"```````````#`#`#`.```````.`#`#`#````````````",
"`````````````a`#`#`.```.`#`#`a``````````````",
"```````````````a`a`a`.`a`a`a````````````````",
"`````````````````c`c`c`c`c``````````````````",
"`````````````````d`d`d`d`d``````````````````",
"```````````````e`e`e`e`e`e`e````````````````",
"`````````````f`f`f`f`g`f`f`f`f``````````````",
"```````````h`h`h`h`g```g`h`h`h`h````````````",
"`````````h`h`h`h`g```````g`h`h`h`h``````````",
"```````g`g`g`g`g```````````g`g`g`g`g````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````",
"````````````````````````````````````````````"
};

static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}

static void qt_plastique_draw_gradient(QPainter *painter, const QRect &rect, const QColor &gradientStart,
                                       const QColor &gradientStop)
{
    QString gradientName;
    gradientName.sprintf("%dx%d-%x-%x", rect.width(), rect.height(), gradientStart.rgba(), gradientStop.rgba());
    QPixmap cache;
    if (!UsePixmapCache || !QPixmapCache::find(gradientName, cache)) {
        cache = QPixmap(rect.size());
        cache.fill(Qt::white);
        QPainter cachePainter(&cache);
        QRect pixmapRect(0, 0, rect.width(), rect.height());
        int x = pixmapRect.center().x();
        QLinearGradient gradient(x, pixmapRect.top(), x, pixmapRect.bottom());
        gradient.setColorAt(0, gradientStart);
        gradient.setColorAt(1, gradientStop);
        cachePainter.fillRect(pixmapRect, gradient);
        cachePainter.end();
        if (UsePixmapCache)
            QPixmapCache::insert(gradientName, cache);
    }
    painter->drawPixmap(rect, cache);
}

static void qt_plastique_draw_handle(QPainter *painter, const QStyleOption *option,
                                     const QRect &rect, Qt::Orientation orientation,
                                     const QWidget *widget)
{
    QColor borderColor = option->palette.background().color().darker(178);
    QColor alphaCornerColor;
    if (widget) {
        // ### backgroundrole/foregroundrole should be part of the style option
        alphaCornerColor = mergedColors(option->palette.color(widget->backgroundRole()), borderColor);
    } else {
        alphaCornerColor = mergedColors(option->palette.background().color(), borderColor);
    }

    QImage handle(qt_simple_toolbarhandle);
    handle.setColor(1, alphaCornerColor.rgba());
    handle.setColor(2, mergedColors(alphaCornerColor, option->palette.base().color()).rgba());
    handle.setColor(3, option->palette.base().color().rgba());

    const int spacing = 2;

    if (orientation == Qt::Vertical) {
        int nchunks = rect.width() / (handle.width() + spacing);
        for (int i = 0; i < nchunks; ++i)
            painter->drawImage(QPoint(rect.left() + i * (handle.width() + spacing), rect.top()), handle);
    } else {
        int nchunks = rect.height() / (handle.height() + spacing);
        for (int i = 0; i < nchunks; ++i)
            painter->drawImage(QPoint(rect.left(), rect.top() + i * (handle.height() + spacing)), handle);
    }
}

static void qt_plastique_draw_mdibutton(QPainter *painter, const QStyleOptionTitleBar *option, const QRect &tmp, bool hover, bool sunken)
{
    if (tmp.isNull())
        return;
    bool active = (option->titleBarState & QStyle::State_Active);

    // ### use palette colors instead
    QColor mdiButtonGradientStartColor;
    QColor mdiButtonGradientStopColor;

    if (active) {
        mdiButtonGradientStartColor = QColor((hover || sunken) ? 0x7d8bb1 : 0x55689a);
        mdiButtonGradientStopColor = QColor((hover || sunken) ? 0x939ebe : 0x7381ab);
    } else {
        mdiButtonGradientStartColor = QColor((hover || sunken) ? 0x9e9e9e : 0x818181);
        mdiButtonGradientStopColor = QColor((hover || sunken) ? 0xababab : 0x929292);
    }

    qt_plastique_draw_gradient(painter, tmp.adjusted(1, 1, -1, -1),
                               mdiButtonGradientStartColor, mdiButtonGradientStopColor);

    QColor mdiButtonBorderColor;
    if (active) {
        mdiButtonBorderColor = (hover || sunken) ? QColor(0x627097) : QColor(0x324577);
    } else {
        mdiButtonBorderColor = (hover || sunken) ? QColor(0x838383) : QColor(0x5e5e5e);
    }
    painter->setPen(QPen(mdiButtonBorderColor, 1));

    const QLine lines[4] = {
        QLine(tmp.left() + 2, tmp.top(), tmp.right() - 2, tmp.top()),
        QLine(tmp.left() + 2, tmp.bottom(), tmp.right() - 2, tmp.bottom()),
        QLine(tmp.left(), tmp.top() + 2, tmp.left(), tmp.bottom() - 2),
        QLine(tmp.right(), tmp.top() + 2, tmp.right(), tmp.bottom() - 2) };
    painter->drawLines(lines, 4);

    const QPoint points[4] = {
        QPoint(tmp.left() + 1, tmp.top() + 1),
        QPoint(tmp.right() - 1, tmp.top() + 1),
        QPoint(tmp.left() + 1, tmp.bottom() - 1),
        QPoint(tmp.right() - 1, tmp.bottom() - 1) };
    painter->drawPoints(points, 4);
}

seStyle::seStyle()
{
}

seStyle::~seStyle()
{
}

void seStyle::polish(QApplication *application)
{
	QPlastiqueStyle::polish(application);

	_oldPalette = application->palette();

	QPalette pal(_oldPalette);
	pal.setColor(QPalette::Window, QColor(0x47, 0x47, 0x47));
	pal.setColor(QPalette::Base, QColor(0x57, 0x57, 0x57));
	pal.setColor(QPalette::Text, QColor(0xe4, 0xe4, 0xe4));
	pal.setColor(QPalette::HighlightedText, QColor(0x26, 0x26, 0x27));
	pal.setColor(QPalette::ButtonText, QColor(0xe4, 0xe4, 0xe4));
	pal.setColor(QPalette::Highlight, QColor(0xa0, 0xa0, 0xa0));
	pal.setColor(QPalette::WindowText, QColor(0xdc, 0xdc, 0xdc));
	pal.setColor(QPalette::Mid, QColor(0x32, 0x32, 0x32));
	pal.setColor(QPalette::Midlight, QColor(0x55, 0x55, 0x55));
	pal.setColor(QPalette::Button, QColor(0x43, 0x43, 0x43));
	pal.setColor(QPalette::Dark, QColor(0xa, 0xa, 0xa));
	pal.setColor(QPalette::Light, QColor(0x7a, 0x7a, 0x7a));
	pal.setColor(QPalette::Shadow, QColor(0x26, 0x26, 0x26));

	pal.setColor(QPalette::Disabled, QPalette::Text, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Highlight, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Shadow, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Light, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Midlight, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Dark, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Mid, pal.color(QPalette::Active, QPalette::Text).dark());

	application->setPalette(pal);
}

void seStyle::unpolish(QApplication *application)
{
	QPlastiqueStyle::unpolish(application);

	application->setPalette(_oldPalette);
}

void seStyle::drawControl(ControlElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
{
	switch (element)
	{
		/*
	case CE_TabBarTab:
		{
			const QTabBar *tabbar = (const QTabBar*) widget;
			const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab*>(option);
			bool selected = tabOption->state & QStyle::State_Selected;
			const QRect& r = option->rect;

			if (selected)
			{
				painter->fillRect(r.x(), r.y(), r.width(), r.height()-1, option->palette.shadow());

				const int INSET = 6;
				QPolygon points(6);
				points.putPoints(0, 6, 
                              r.left() + 1, r.bottom(),
                              r.left() + INSET, r.bottom(),
                              r.left() + INSET, r.top() + INSET,
                              r.right() - 3*r.height()/5, r.top() + INSET,
                              r.right() - INSET, r.bottom(),
                              r.right(), r.bottom() );
				painter->setBrush(option->palette.button());
				painter->setPen(option->palette.button().color());
				painter->drawPolygon(points);

				painter->setPen(QPen(option->palette.light().color()));
				painter->drawLine( points[0], points[1] );
				painter->drawLine( points[1], QPoint( points[2].x(), points[2].y() + 1 ) );
				painter->drawLine( QPoint( points[2].x() + 1, points[2].y() ), QPoint( points[3].x() - 2, points[3].y() ) ); // this is the right hand top corner
				painter->drawLine( QPoint( points[3].x() - 1, points[3].y() + 1 ), QPoint( points[3].x(), points[3].y() + 2 ) );
				painter->drawLine( QPoint( points[3].x(), points[3].y() + 2 ), points[4] );
				painter->drawLine( points[4], points[5] );
			}
			else
			{
				painter->setPen(option->palette.color(QPalette::Current, QPalette::Shadow));
				painter->setBrush(option->palette.brush(QPalette::Current, QPalette::Shadow));
				painter->drawRect(r.x(), r.y(), r.width(), r.height()-1);

				painter->setPen(option->palette.color(QPalette::Current, QPalette::Light));
				painter->drawLine(r.left()+1, r.bottom(), r.right(), r.bottom());
			}
		}
		break;

	case CE_TabBarTabLabel:
		{
			const QTabBar *tabbar = (const QTabBar*) widget;
			const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab*>(option);
			bool selected = tabOption->state & QStyle::State_Selected;
			const QRect& r = option->rect;

			if (selected)
				painter->setPen(option->palette.text().color());
			else
				painter->setPen(option->palette.midlight().color());

			painter->setPen(QColor(Qt::white));

			QFontMetrics fm(painter->fontMetrics());
			int textHeight = fm.height();
			int textWidth = fm.width(tabOption->text);
			int xoffset = ( r.width() - textWidth ) / 2;
			int yoffset = r.height() - textHeight;

			if ( xoffset < 0 ) xoffset = 0;
			if ( yoffset < 0 ) yoffset = 0;

			if ( selected )
				painter->drawText( r.x() + xoffset, r.bottom(), tabOption->text);
			else
				painter->drawText( r.x() + xoffset, r.bottom() - yoffset, tabOption->text  );
		}
		break;
		*/

	case CE_Splitter:
        if ((option->state & State_Enabled) && (option->state & State_MouseOver))
            painter->fillRect(option->rect, option->palette.background().color().lighter(105));
        if (option->state & State_Horizontal) {
            int height = option->rect.height() / 3;
            QRect rect(option->rect.left() + (option->rect.width() / 2 - 1),
                       option->rect.center().y() - height / 2, 3, height);
            qt_plastique_draw_handle(painter, option, rect, Qt::Horizontal, widget);
        } else {
            int width = option->rect.width() / 3;
            QRect rect(option->rect.center().x() - width / 2,
                       option->rect.top() + (option->rect.height() / 2) - 1, width, 3);
            qt_plastique_draw_handle(painter, option, rect, Qt::Vertical, widget);
        }
        break;

	default:
		QPlastiqueStyle::drawControl(element, option, painter, widget);
	}
}

void seStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                    QPainter *painter, const QWidget *widget) const
{
    Q_ASSERT(option);

	switch (element)
	{
	case PE_IndicatorDockWidgetResizeHandle:
        if ((option->state & State_Enabled) && (option->state & State_MouseOver))
            painter->fillRect(option->rect, option->palette.background().color().lighter(105));
        if (option->state & State_Horizontal) 
		{
            int width = option->rect.width() / 3;
            QRect rect(option->rect.center().x() - width / 2,
                       option->rect.top() + (option->rect.height() / 2) - 1, width, 3);
            qt_plastique_draw_handle(painter, option, rect, Qt::Vertical, widget);
        } 
		else 
		{
            int height = option->rect.height() / 3;
            QRect rect(option->rect.left() + (option->rect.width() / 2 - 1),
                       option->rect.center().y() - height / 2, 3, height);
            qt_plastique_draw_handle(painter, option, rect, Qt::Horizontal, widget);
        }
		break;

	default:
		QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
		break;
	}
}

void seStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                 QPainter *painter, const QWidget *widget) const
{
	switch (control)
	{
#if 0
	case CC_TitleBar:
        if (const QStyleOptionTitleBar *titleBar = qstyleoption_cast<const QStyleOptionTitleBar *>(option)) 
		{
            painter->save();
            bool active = (titleBar->titleBarState & State_Active);
            QRect fullRect = titleBar->rect;

            // ### use palette colors instead
            //QColor titleBarGradientStart(active ? 0x3b508a : 0x6e6e6e);
            //QColor titleBarGradientStop(active ? 0x5d6e9e : 0x818181);
            //QColor titleBarFrameBorder(0x393939);
            //QColor titleBarAlphaCorner(active ? 0x4b5e7f : 0x6a6a6a);
            //QColor titleBarInnerTopLine(active ? 0x8e98ba : 0xa4a4a4);
            //QColor titleBarInnerInnerTopLine(active ? 0x57699b : 0x808080);
            //QColor leftCorner(active ? 0x6f7ea8 : 0x8e8e8e);
            //QColor rightCorner(active ? 0x44537d : 0x676767);
            QColor textColor(active ? 0x282e40 : 0x282e40);
            QColor textAlphaColor(active ? 0x3f4862 : 0x3f4862);


            // max button
            if ((titleBar->subControls & SC_TitleBarMaxButton)
                    && (titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint)
                    && !(titleBar->titleBarState & Qt::WindowMaximized)) {
                bool hover = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_Sunken);

                QRect maxButtonRect = subControlRect(CC_TitleBar, titleBar, SC_TitleBarMaxButton, widget);
				QImage image(Maximize_xpm);
                //qt_plastique_draw_mdibutton(painter, titleBar, maxButtonRect, hover, sunken);
				painter->drawImage(maxButtonRect, image);

                //int xoffset = maxButtonRect.width() / 3;
                //int yoffset = maxButtonRect.height() / 3;

                //QRect maxButtonIconRect(maxButtonRect.left() + xoffset, maxButtonRect.top() + yoffset,
                //                        maxButtonRect.width() - xoffset * 2, maxButtonRect.height() - yoffset * 2);

                //painter->setPen(textColor);
                //painter->drawRect(maxButtonIconRect.adjusted(0, 0, -1, -1));
                //painter->drawLine(maxButtonIconRect.left() + 1, maxButtonIconRect.top() + 1,
                //                  maxButtonIconRect.right() - 1, maxButtonIconRect.top() + 1);
                //painter->setPen(textAlphaColor);
                //const QPoint points[4] = {
                //    maxButtonIconRect.topLeft(), maxButtonIconRect.topRight(),
                //    maxButtonIconRect.bottomLeft(), maxButtonIconRect.bottomRight() };
                //painter->drawPoints(points, 4);
            }

            // close button
            if (titleBar->subControls & SC_TitleBarCloseButton && titleBar->titleBarFlags & Qt::WindowSystemMenuHint) {
                bool hover = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_Sunken);

                QRect closeButtonRect = subControlRect(CC_TitleBar, titleBar, SC_TitleBarCloseButton, widget);
				QImage image(Office2007_FrameCaptionClose23_xpm);
                //qt_plastique_draw_mdibutton(painter, titleBar, closeButtonRect, hover, sunken);
				painter->drawImage(closeButtonRect, image);

                //int xoffset = closeButtonRect.width() / 3;
                //int yoffset = closeButtonRect.height() / 3;

                //QRect closeIconRect(closeButtonRect.left() + xoffset, closeButtonRect.top() + yoffset,
                //                    closeButtonRect.width() - xoffset * 2, closeButtonRect.height() - yoffset * 2);

                //painter->setPen(textAlphaColor);
                //{
                //    const QLine lines[4] = {
                //        QLine(closeIconRect.left() + 1, closeIconRect.top(),
                //              closeIconRect.right(), closeIconRect.bottom() - 1),
                //        QLine(closeIconRect.left(), closeIconRect.top() + 1,
                //              closeIconRect.right() - 1, closeIconRect.bottom()),
                //        QLine(closeIconRect.right() - 1, closeIconRect.top(),
                //              closeIconRect.left(), closeIconRect.bottom() - 1),
                //        QLine(closeIconRect.right(), closeIconRect.top() + 1,
                //              closeIconRect.left() + 1, closeIconRect.bottom()) };
                //    painter->drawLines(lines, 4);
                //    const QPoint points[4] = {
                //        closeIconRect.topLeft(), closeIconRect.topRight(),
                //        closeIconRect.bottomLeft(), closeIconRect.bottomRight() };
                //    painter->drawPoints(points, 4);
                //}
                //painter->setPen(textColor);
                //{
                //    const QLine lines[2] = {
                //        QLine(closeIconRect.left() + 1, closeIconRect.top() + 1,
                //              closeIconRect.right() - 1, closeIconRect.bottom() - 1),
                //        QLine(closeIconRect.left() + 1, closeIconRect.bottom() - 1,
                //              closeIconRect.right() - 1, closeIconRect.top() + 1) };
                //    painter->drawLines(lines, 2);
                //}
            }

            // from qwindowsstyle.cpp
            if ((titleBar->subControls & SC_TitleBarSysMenu) && (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
                bool hover = (titleBar->activeSubControls & SC_TitleBarSysMenu) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarSysMenu) && (titleBar->state & State_Sunken);

                QRect iconRect = subControlRect(CC_TitleBar, titleBar, SC_TitleBarSysMenu, widget);
                if (hover)
                    qt_plastique_draw_mdibutton(painter, titleBar, iconRect, hover, sunken);

                if (!titleBar->icon.isNull()) {
                    titleBar->icon.paint(painter, iconRect);
                } else {
                    QStyleOption tool(0);
                    tool.palette = titleBar->palette;
                    QPixmap pm = standardPixmap(SP_TitleBarMenuButton, &tool, widget);
                    tool.rect = iconRect;
                    painter->save();
                    drawItemPixmap(painter, iconRect, Qt::AlignCenter, pm);
                    painter->restore();
                }
            }

			painter->restore();
		}
		break;
#endif

	default:
		QPlastiqueStyle::drawComplexControl(control, option, painter, widget);
		break;
	}
}