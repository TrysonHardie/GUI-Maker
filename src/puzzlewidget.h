#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QWidget>

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PuzzleWidget(int imageSize, QWidget *parent = 0);
    void clear();

    int pieceSize() const;
    int imageSize() const;

signals:
    void puzzleCompleted();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int findPiece(const QRect &pieceRect) const;
    const QRect targetSquare(const QPoint &position) const;

    QList<QPixmap> piecePixmaps;
    QList<QRect> pieceRects;
    QList<QPoint> pieceLocations;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
};

#endif // PUZZLEWIDGET_H
