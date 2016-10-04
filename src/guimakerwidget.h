#ifndef GUIMAKERWIDGET_H
#define GUIMAKERWIDGET_H

#include <QGraphicsScene>
#include <QMenu>



class GuiMakerWidget : public QGraphicsScene
{
    Q_OBJECT

public:
    GuiMakerWidget(QMenu *itemMenu, QObject *parent);

    void clear();

    void addNewItem(QGraphicsItem *item);

    QString getText(bool richText = false);
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
//    void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    QMenu *myItemMenu;

    int quantization() const
    {
        return 5;
    }

};
#endif // GUIMAKERWIDGET_H
