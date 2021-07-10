#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>

#include "tabs/TabIndex.h"

class ClickableLabel : public QLabel
{
    Q_OBJECT
    public:
        explicit ClickableLabel( const QString& text="", QWidget* parent = nullptr );
        ClickableLabel( QWidget* parent = nullptr );
        ~ClickableLabel() override;
        void setIndexType(TabIndices indexType);
       void setPixmapPath(const QString & path) { mPixmap.load(path);}
    signals:
        void clicked(int index);
        void clicked(int index, QString nameObject);
    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void resizeEvent(QResizeEvent *) override;
    private:
       TabIndices mIndexType;
       QPixmap    mPixmap;
};
#endif // CLICKABLELABEL_H
