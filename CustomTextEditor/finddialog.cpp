#include "finddialog.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>


/* Initializes this FindDialog object.
 */
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    // Initialize all members
    findLabel = new QLabel(tr("Find what:    "));
    replaceLabel = new QLabel(tr("Replace with:"));
    findLineEdit = new QLineEdit();
    replaceLineEdit = new QLineEdit();
    findNextButton = new QPushButton(tr("&Find next"));
    replaceButton = new QPushButton(tr("&Replace"));
    replaceAllButton = new QPushButton(tr("&Replace all"));
    caseSensitiveCheckBox = new QCheckBox(tr("&Match case"));
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    queryText = "";

    // Ensures that the line edit gets the focus whenever the dialog is the active window
    setFocusProxy(findLineEdit);

    // Set up all the  widgets and layouts
    findHorizontalLayout = new QHBoxLayout();
    replaceHorizontalLayout = new QHBoxLayout();
    optionsLayout = new QHBoxLayout();
    verticalLayout = new QVBoxLayout();

    verticalLayout->addLayout(findHorizontalLayout);
    verticalLayout->addLayout(replaceHorizontalLayout);
    verticalLayout->addLayout(optionsLayout);

    findHorizontalLayout->addWidget(findLabel);
    findHorizontalLayout->addWidget(findLineEdit);
    replaceHorizontalLayout->addWidget(replaceLabel);
    replaceHorizontalLayout->addWidget(replaceLineEdit);

    optionsLayout->addWidget(caseSensitiveCheckBox);
    optionsLayout->addWidget(wholeWordsCheckBox);
    optionsLayout->addWidget(findNextButton);
    optionsLayout->addWidget(replaceButton);
    optionsLayout->addWidget(replaceAllButton);

    setLayout(verticalLayout);
    setWindowTitle(tr("Find and Replace"));

    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(on_replaceButton_clicked()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(on_replaceAllButton_clicked()));
}


/* Performs all required memory cleanup operations.
 */
FindDialog::~FindDialog()
{
    delete findLabel;
    delete replaceLabel;
    delete findLineEdit;
    delete replaceLineEdit;
    delete findNextButton;
    delete replaceButton;
    delete replaceAllButton;
    delete caseSensitiveCheckBox;
    delete wholeWordsCheckBox;
    delete findHorizontalLayout;
    delete replaceHorizontalLayout;
    delete verticalLayout;
    delete optionsLayout;
}


/* Called when the user clicks the Find Next button. If the query is empty, it informs the user.
 * Otherwise, it emits an appropriate signal for queryTextReady with all relevant search criteria.
 */
void FindDialog::on_findNextButton_clicked()
{
    QString query = findLineEdit->text();
    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    queryText = query;

   emit(queryTextReady(queryText, caseSensitive, wholeWords));
}


/* TODO document
 */
void FindDialog::on_replaceButton_clicked()
{
    // First find and highlight a result, if any
    on_findNextButton_clicked();

    // Then broadcast the replacement
    // TODO this works, but change replaceAllCanContinue to something more intuitive, like canReplace
    if(replaceAllCanContinue)
    {
        QString replacementText = replaceLineEdit->text();
        emit(replacementTextReady(replacementText));
    }
}


/* TODO document
 */
void FindDialog::on_replaceAllButton_clicked()
{    
    replaceAllCanContinue = true;

    // TODO what if we just hand off this logic to Editor, who has replaceAll routine?
    // and change editor's find routine to return a boolean, such that replaceAll is able to check if a match was found

    // See editor.cpp for when this gets set to false
    while(replaceAllCanContinue)
    {
        on_replaceButton_clicked();
    }
}


/* TODO document
 */
void FindDialog::addToSearchHistory(QString term, int positionPriorToSearch, int firstMatchPosition)
{
    if(!previouslyFound(term))
    {
        searchHistory.clear();
    }
    QPair<int,int> locations;
    locations.first = positionPriorToSearch;
    locations.second = firstMatchPosition;
    searchHistory.insert(term, locations);
}
