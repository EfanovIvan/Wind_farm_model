#include <QPixmap>

#include "ClickableLabel.h"
#include "tabs/TabIndex.h"

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::ClickableLabel(QWidget *parent):QLabel(parent)
{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::setIndexType(TabIndices indexType)
{
    mIndexType = indexType;
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
//    if(mIndexType >= TabIndices::Line1)
//    {
//        emit clicked(static_cast<int>(mIndexType), this->objectName());
//    }
//    else
    {
        emit clicked(static_cast<int>(mIndexType));
    }

}

void ClickableLabel::resizeEvent(QResizeEvent *)
{
//    const QPixmap * сp; // load pixmap
//    QPixmap * p; // load pixmap
//    // get label dimensions
//    сp = this->pixmap();
//    p = const_cast<QPixmap *>(сp);
    if(!mPixmap.isNull())
    {
        QPixmap copy = mPixmap;
        int w = this->width();
        int h = this->height();


        // set a scaled pixmap to a w x h window keeping its aspect ratio

            //this->setPixmap(p->scaled(mSize));
            this->setPixmap(copy.scaled(w,h,Qt::KeepAspectRatio));
     }



}
