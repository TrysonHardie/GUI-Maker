#include "arrayofcolorseditorbutton.h"
#include "langstrategy.h"
#include "mainwindow.h"
#include "windowparametersdialog.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QSpinBox>

WindowParametersDialog::WindowParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    m_WindowTitle = new QLineEdit(this); QLabel* label_WindowTitle = new QLabel("Window Title", this);
    m_WindowVarName = new QLineEdit(this); QLabel* label_WindowVarName = new QLabel("Variable Name", this);

    m_WindowWidth = new QSpinBox(this);
    m_WindowHeight = new QSpinBox(this);

//    m_WindowBackgroundColor = new ColorEditorButton(this);
    m_WindowBackgroundColor = new ArrayOfColorsEditorButton(this); QLabel* label_WindowBackgroundColor = new QLabel("Background Color", this);


    // --- set parameters
    m_WindowWidth->setMinimum(64); m_WindowHeight->setMinimum(64);
    m_WindowWidth->setMaximum(4000); m_WindowHeight->setMaximum(2000); // 4k. 8k resolution == 7680×4320
    m_WindowWidth->setSuffix(" px");    m_WindowHeight->setSuffix(" px");

    QRegExp rx("[a-zA-Z][a-zA-Z0-9_]*"); // variable name must starts with alphabetic character and then contains only [a-zA-Z0-9_]
    QValidator *validator = new QRegExpValidator(rx, this);
    m_WindowVarName->setValidator(validator);


    //  --- set values
    MainWindow *pMainWindow = MainWindow::getInstance();
    AbstractLangStrategy* lang = pMainWindow->getCurrLangStrategy();

    m_WindowTitle->setText(lang->m_WindowTitle);
    m_WindowVarName->setText(lang->m_WindowVarName);

    m_WindowWidth->setValue(pMainWindow->subWindow1->size().width());
    m_WindowHeight->setValue(pMainWindow->subWindow1->size().height());

    m_WindowBackgroundColor->setText(lang->m_WindowBackgroundColor);


//    ---
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &WindowParametersDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_WindowVarName, 0, 0);    layout->addWidget(label_WindowVarName, 0, 1);
    layout->addWidget(m_WindowTitle, 1, 0);    layout->addWidget(label_WindowTitle, 1, 1);

    layout->addWidget(m_WindowWidth, 2, 0); layout->addWidget(m_WindowHeight, 2, 1);

    layout->addWidget(m_WindowBackgroundColor, 3, 0);    layout->addWidget(label_WindowBackgroundColor, 3, 1);

    layout->addWidget(buttonBox);

}

void WindowParametersDialog::ok()
{
    MainWindow *pMainWindow = MainWindow::getInstance();
    AbstractLangStrategy* lang = pMainWindow->getCurrLangStrategy();

    lang->m_WindowVarName = m_WindowVarName->text();

    if(m_WindowTitle->text().isEmpty() == false)
        lang->m_WindowTitle = m_WindowTitle->text();

    // setWindowSize
    pMainWindow->subWindow1->resize(m_WindowWidth->value(), m_WindowHeight->value());

    // setWindowBackgroundColor
    lang->m_WindowBackgroundColor = m_WindowBackgroundColor->getText();

    QDialog::accept();
}
