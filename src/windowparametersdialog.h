#ifndef WINDOWPARAMETERSDIALOG_H
#define WINDOWPARAMETERSDIALOG_H

#include <QDialog>


class QLineEdit;
class QSpinBox;
class ArrayOfColorsEditorButton;

// Dialog for changing some parameters of main window

class WindowParametersDialog : public QDialog
{
    QLineEdit* m_WindowTitle;
    QLineEdit* m_WindowVarName;

    QSpinBox* m_WindowWidth;
    QSpinBox* m_WindowHeight;

    ArrayOfColorsEditorButton* m_WindowBackgroundColor;

public:
    WindowParametersDialog(QWidget *parent);

public slots:
    void ok();

};

#endif // WINDOWPARAMETERSDIALOG_H
