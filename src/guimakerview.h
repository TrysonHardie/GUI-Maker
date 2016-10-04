#ifndef GUIMAKERVIEW_H
#define GUIMAKERVIEW_H


#include "guimakerscene.h"

#include <QGraphicsView>



class GuiMakerView: public QGraphicsView
{
    Q_OBJECT
public:
    GuiMakerView(GuiMakerScene * scene);
    GuiMakerView(QGraphicsScene * scene);
protected:
    void dragEnterEvent ( QDragEnterEvent * event );
//    virtual void dragLeaveEvent ( QDragLeaveEvent * event );
    virtual void dragMoveEvent ( QDragMoveEvent * event );
    void dropEvent ( QDropEvent * event );

private:
};

#endif // GUIMAKERVIEW_H
