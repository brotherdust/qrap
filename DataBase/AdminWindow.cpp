 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : AdminWindow.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Roeland van Nieuwkerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that inherits QTabWidget and is responsible for
 *                  providing administration features to manage User and Group priviledges
 *                  for the Qrap system.
 *
 **************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#include <QMessageBox>
#include "Config.h"

#ifdef QRAP_SERVER_EDITION

#include "AdminWindow.h"

using namespace std;
using namespace Qrap;

//********************************************************************
AdminWindow::AdminWindow (QWidget* parent) : QDialog(parent)
{
	// Set the window title
	setWindowTitle("QRAP Administration");
	setWindowIcon(QIcon(":images/logo_icon.png"));
	setModal(true);
	
	horizontalLayout = new QWidget(this);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	horizontalLayout->setGeometry(QRect(10, 10, 681, 531));
	hboxLayout = new QHBoxLayout(horizontalLayout);
	hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
	hboxLayout->setContentsMargins(0, 0, 0, 0);
	mAdminTree = new QTreeWidget(horizontalLayout);
	mAdminTree->setObjectName(QString::fromUtf8("mAdminTree"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mAdminTree->sizePolicy().hasHeightForWidth());
	mAdminTree->setSizePolicy(sizePolicy);
	
	hboxLayout->addWidget(mAdminTree);
	
	mAdminStack = new QStackedWidget(horizontalLayout);
	mAdminStack->setObjectName(QString::fromUtf8("mAdminStack"));
	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy1.setHorizontalStretch(2);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(mAdminStack->sizePolicy().hasHeightForWidth());
	mAdminStack->setSizePolicy(sizePolicy1);
	mUserAdminPage = new QWidget();
	mUserAdminPage->setObjectName(QString::fromUtf8("mUserAdminPage"));
	mUserAdminLabel = new QLabel(mUserAdminPage);
	mUserAdminLabel->setObjectName(QString::fromUtf8("mUserAdminLabel"));
	mUserAdminLabel->setGeometry(QRect(10, 0, 106, 26));
	mUserAdminLabel->setText(tr("User Administration"));
	QFont font;
	font.setPointSize(14);
	mUserAdminLabel->setFont(font);
	line = new QFrame(mUserAdminPage);
	line->setObjectName(QString::fromUtf8("line"));
	line->setGeometry(QRect(10, 20, 431, 16));
	line->setFrameShadow(QFrame::Plain);
	line->setFrameShape(QFrame::HLine);
	gridLayout = new QWidget(mUserAdminPage);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	gridLayout->setGeometry(QRect(10, 40, 431, 252));
	gridLayout1 = new QGridLayout(gridLayout);
	gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
	gridLayout1->setContentsMargins(0, 0, 0, 0);
	mUsernameLabel = new QLabel(gridLayout);
	mUsernameLabel->setObjectName(QString::fromUtf8("mUsernameLabel"));
	mUsernameLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mUsernameLabel->setText(tr("Username"));
	
	gridLayout1->addWidget(mUsernameLabel, 0, 0, 1, 1);
	
	mPasswordLabel = new QLabel(gridLayout);
	mPasswordLabel->setObjectName(QString::fromUtf8("mPasswordLabel"));
	mPasswordLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mPasswordLabel->setText(tr("Password"));
	
	gridLayout1->addWidget(mPasswordLabel, 1, 0, 1, 1);
	
	mConfirmLabel = new QLabel(gridLayout);
	mConfirmLabel->setObjectName(QString::fromUtf8("mConfirmLabel"));
	mConfirmLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mConfirmLabel->setText(tr("Confirm password"));
	
	gridLayout1->addWidget(mConfirmLabel, 2, 0, 1, 1);
	
	mFullnamesLabel = new QLabel(gridLayout);
	mFullnamesLabel->setObjectName(QString::fromUtf8("mFullnamesLabel"));
	mFullnamesLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mFullnamesLabel->setText(tr("Full names"));
	
	gridLayout1->addWidget(mFullnamesLabel, 3, 0, 1, 1);
	
	mIDLabel = new QLabel(gridLayout);
	mIDLabel->setObjectName(QString::fromUtf8("mIDLabel"));
	mIDLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mIDLabel->setText(tr("ID number"));
	
	gridLayout1->addWidget(mIDLabel, 4, 0, 1, 1);
	
	mEmailLabel = new QLabel(gridLayout);
	mEmailLabel->setObjectName(QString::fromUtf8("mEmailLabel"));
	mEmailLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mEmailLabel->setText(tr("E-mail"));
	
	gridLayout1->addWidget(mEmailLabel, 5, 0, 1, 1);
	
	mGroupLabel = new QLabel(gridLayout);
	mGroupLabel->setObjectName(QString::fromUtf8("mGroupLabel"));
	mGroupLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mGroupLabel->setText(tr("Group"));
	
	gridLayout1->addWidget(mGroupLabel, 6, 0, 1, 1);
	
	mUsernameEdit = new QLineEdit(gridLayout);
	mUsernameEdit->setObjectName(QString::fromUtf8("mUsernameEdit"));
	
	gridLayout1->addWidget(mUsernameEdit, 0, 1, 1, 1);
	
	mPasswordEdit = new QLineEdit(gridLayout);
	mPasswordEdit->setObjectName(QString::fromUtf8("mPasswordEdit"));
	mPasswordEdit->setEchoMode(QLineEdit::Password);
	
	gridLayout1->addWidget(mPasswordEdit, 1, 1, 1, 1);
	
	mConfirmEdit = new QLineEdit(gridLayout);
	mConfirmEdit->setObjectName(QString::fromUtf8("mConfirmEdit"));
	mConfirmEdit->setEchoMode(QLineEdit::Password);
	
	gridLayout1->addWidget(mConfirmEdit, 2, 1, 1, 1);
	
	mFullnamesEdit = new QLineEdit(gridLayout);
	mFullnamesEdit->setObjectName(QString::fromUtf8("mFullnamesEdit"));
	
	gridLayout1->addWidget(mFullnamesEdit, 3, 1, 1, 1);
	
	mIDEdit = new QLineEdit(gridLayout);
	mIDEdit->setObjectName(QString::fromUtf8("mIDEdit"));
	
	gridLayout1->addWidget(mIDEdit, 4, 1, 1, 1);
	
	mEmailEdit = new QLineEdit(gridLayout);
	mEmailEdit->setObjectName(QString::fromUtf8("mEmailEdit"));
	
	gridLayout1->addWidget(mEmailEdit, 5, 1, 1, 1);
	
	mGroupCombo = new QComboBox(gridLayout);
	mGroupCombo->setObjectName(QString::fromUtf8("mGroupCombo"));
	
	gridLayout1->addWidget(mGroupCombo, 6, 1, 1, 1);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	gridLayout1->addItem(spacerItem, 7, 0, 1, 1);
	
	horizontalLayout_2 = new QWidget(mUserAdminPage);
	horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
	horizontalLayout_2->setGeometry(QRect(10, 290, 431, 41));
	hboxLayout1 = new QHBoxLayout(horizontalLayout_2);
	hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
	hboxLayout1->setContentsMargins(0, 0, 0, 0);
	mAddUserButton = new QPushButton(horizontalLayout_2);
	mAddUserButton->setObjectName(QString::fromUtf8("mAddUserButton"));
	mAddUserButton->setText(tr("Add"));
	
	hboxLayout1->addWidget(mAddUserButton);
	
	mDeleteUserButton = new QPushButton(horizontalLayout_2);
	mDeleteUserButton->setObjectName(QString::fromUtf8("mDeleteUserButton"));
	mDeleteUserButton->setText(tr("Delete"));
	
	hboxLayout1->addWidget(mDeleteUserButton);
	
	mCommitUserButton = new QPushButton(horizontalLayout_2);
	mCommitUserButton->setObjectName(QString::fromUtf8("mCommitUserButton"));
	mCommitUserButton->setText(tr("Commit"));
	
	hboxLayout1->addWidget(mCommitUserButton);
	
	mCloseUserButton = new QPushButton(horizontalLayout_2);
	mCloseUserButton->setObjectName(QString::fromUtf8("mCloseUserButton"));
	mCloseUserButton->setText(tr("Close"));
	
	hboxLayout1->addWidget(mCloseUserButton);
	
	mAdminStack->addWidget(mUserAdminPage);
	mGroupAdminPage = new QWidget();
	mGroupAdminPage->setObjectName(QString::fromUtf8("mGroupAdminPage"));
	line_2 = new QFrame(mGroupAdminPage);
	line_2->setObjectName(QString::fromUtf8("line_2"));
	line_2->setGeometry(QRect(10, 20, 431, 16));
	line_2->setFrameShadow(QFrame::Plain);
	line_2->setFrameShape(QFrame::HLine);
	mGroupAdminLabel = new QLabel(mGroupAdminPage);
	mGroupAdminLabel->setObjectName(QString::fromUtf8("mGroupAdminLabel"));
	mGroupAdminLabel->setGeometry(QRect(10, 0, 198, 26));
	mGroupAdminLabel->setFont(font);
	mGroupAdminLabel->setText(tr("Group Administration"));
	horizontalLayout_3 = new QWidget(mGroupAdminPage);
	horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
	horizontalLayout_3->setGeometry(QRect(10, 40, 431, 31));
	hboxLayout2 = new QHBoxLayout(horizontalLayout_3);
	hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
	hboxLayout2->setContentsMargins(0, 0, 0, 0);
	mGroupNameLabel = new QLabel(horizontalLayout_3);
	mGroupNameLabel->setObjectName(QString::fromUtf8("mGroupNameLabel"));
	mGroupNameLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
	mGroupNameLabel->setText(tr("Group name"));
	
	hboxLayout2->addWidget(mGroupNameLabel);
	
	mGroupNameEdit = new QLineEdit(horizontalLayout_3);
	mGroupNameEdit->setObjectName(QString::fromUtf8("mGroupNameEdit"));
	
	hboxLayout2->addWidget(mGroupNameEdit);
	
	mLimiterGroupBox = new QGroupBox(mGroupAdminPage);
	mLimiterGroupBox->setObjectName(QString::fromUtf8("mLimiterGroupBox"));
	mLimiterGroupBox->setGeometry(QRect(10, 80, 431, 391));
	mLimiterGroupBox->setTitle(tr("Limiters"));
	mLimitersTable = new QTableWidget(mLimiterGroupBox);
	mLimitersTable->setObjectName(QString::fromUtf8("mLimitersTable"));
	mLimitersTable->setGeometry(QRect(20, 220, 391, 151));
	mExistingLimitersLabel = new QLabel(mLimiterGroupBox);
	mExistingLimitersLabel->setObjectName(QString::fromUtf8("mExistingLimitersLabel"));
	mExistingLimitersLabel->setGeometry(QRect(20, 200, 97, 18));
	mExistingLimitersLabel->setText(tr("Existing limiters"));
	gridLayout_2 = new QWidget(mLimiterGroupBox);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	gridLayout_2->setGeometry(QRect(20, 20, 391, 168));
	gridLayout2 = new QGridLayout(gridLayout_2);
	gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
	gridLayout2->setContentsMargins(0, 0, 0, 0);
	mTableCombo = new QComboBox(gridLayout_2);
	mTableCombo->setObjectName(QString::fromUtf8("mTableCombo"));
	
	gridLayout2->addWidget(mTableCombo, 1, 0, 1, 1);
	
	mTableLabel = new QLabel(gridLayout_2);
	mTableLabel->setObjectName(QString::fromUtf8("mTableLabel"));
	mTableLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
	mTableLabel->setText(tr("Table"));
	
	gridLayout2->addWidget(mTableLabel, 0, 0, 1, 1);
	
	mCommandLabel = new QLabel(gridLayout_2);
	mCommandLabel->setObjectName(QString::fromUtf8("mCommandLabel"));
	mCommandLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
	mCommandLabel->setText(tr("Command"));
	
	gridLayout2->addWidget(mCommandLabel, 0, 1, 1, 1);
	
	mLimiterLabel = new QLabel(gridLayout_2);
	mLimiterLabel->setObjectName(QString::fromUtf8("mLimiterLabel"));
	mLimiterLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
	mLimiterLabel->setText(tr("Limiter clause"));
	
	gridLayout2->addWidget(mLimiterLabel, 2, 0, 1, 1);
	
	mDoLabel = new QLabel(gridLayout_2);
	mDoLabel->setObjectName(QString::fromUtf8("mDoLabel"));
	mDoLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
	mDoLabel->setText(tr("Do clause"));
	
	gridLayout2->addWidget(mDoLabel, 2, 1, 1, 1);
	
	mLimiterEdit = new QLineEdit(gridLayout_2);
	mLimiterEdit->setObjectName(QString::fromUtf8("mLimiterEdit"));
	
	gridLayout2->addWidget(mLimiterEdit, 3, 0, 1, 1);
	
	mCommandCombo = new QComboBox(gridLayout_2);
	mCommandCombo->setObjectName(QString::fromUtf8("mCommandCombo"));
	
	gridLayout2->addWidget(mCommandCombo, 1, 1, 1, 1);
	
	mDoEdit = new QLineEdit(gridLayout_2);
	mDoEdit->setObjectName(QString::fromUtf8("mDoEdit"));
	
	gridLayout2->addWidget(mDoEdit, 3, 1, 1, 1);
	
	hboxLayout3 = new QHBoxLayout();
	hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
	mCommitLimiterButton = new QPushButton(gridLayout_2);
	mCommitLimiterButton->setObjectName(QString::fromUtf8("mCommitLimiterButton"));
	mCommitLimiterButton->setText(tr("Commit limiter"));
	
	hboxLayout3->addWidget(mCommitLimiterButton);
	
	/*mResetLimiterButton = new QPushButton(gridLayout_2);
	mResetLimiterButton->setObjectName(QString::fromUtf8("mResetLimiterButton"));
	mResetLimiterButton->setText(tr("Reset"));
	
	hboxLayout3->addWidget(mResetLimiterButton);*/
	
	
	gridLayout2->addLayout(hboxLayout3, 5, 1, 1, 1);
	
	hboxLayout4 = new QHBoxLayout();
	hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
	mAddLimiterButton = new QPushButton(gridLayout_2);
	mAddLimiterButton->setObjectName(QString::fromUtf8("mAddLimiterButton"));
	mAddLimiterButton->setText(tr("Add Limiter"));
	
	hboxLayout4->addWidget(mAddLimiterButton);
	
	mDeleteLimiterButton = new QPushButton(gridLayout_2);
	mDeleteLimiterButton->setObjectName(QString::fromUtf8("mDeleteLimiterButton"));
	mDeleteLimiterButton->setText(tr("Delete Limiter"));
	
	hboxLayout4->addWidget(mDeleteLimiterButton);
	
	
	gridLayout2->addLayout(hboxLayout4, 5, 0, 1, 1);
	
	spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	gridLayout2->addItem(spacerItem1, 4, 0, 1, 1);
	
	horizontalLayout_6 = new QWidget(mGroupAdminPage);
	horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
	horizontalLayout_6->setGeometry(QRect(10, 480, 431, 41));
	hboxLayout5 = new QHBoxLayout(horizontalLayout_6);
	hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
	hboxLayout5->setContentsMargins(0, 0, 0, 0);
	mAddGroupButton = new QPushButton(horizontalLayout_6);
	mAddGroupButton->setObjectName(QString::fromUtf8("mAddGroupButton"));
	mAddGroupButton->setText(tr("Add"));
	
	hboxLayout5->addWidget(mAddGroupButton);
	
	/*mDuplicateGroupButton = new QPushButton(horizontalLayout_6);
	mDuplicateGroupButton->setObjectName(QString::fromUtf8("mDuplicateGroupButton"));
	mDuplicateGroupButton->setText(tr("Duplicate"));
	
	hboxLayout5->addWidget(mDuplicateGroupButton);*/
	
	mDeleteGroupButton = new QPushButton(horizontalLayout_6);
	mDeleteGroupButton->setObjectName(QString::fromUtf8("mDeleteGroupButton"));
	mDeleteGroupButton->setText(tr("Delete"));
	
	hboxLayout5->addWidget(mDeleteGroupButton);
	
	mCommitGroupButton = new QPushButton(horizontalLayout_6);
	mCommitGroupButton->setObjectName(QString::fromUtf8("mCommitGroupButton"));
	mCommitGroupButton->setText(tr("Commit"));
	
	hboxLayout5->addWidget(mCommitGroupButton);
	
	mCloseButton = new QPushButton(horizontalLayout_6);
	mCloseButton->setObjectName(QString::fromUtf8("mCloseButton"));
	mCloseButton->setText(tr("Close"));
	
	hboxLayout5->addWidget(mCloseButton);
	mAdminStack->addWidget(mGroupAdminPage);
	hboxLayout->addWidget(mAdminStack);
	mAdminStack->setCurrentIndex(0);
	
	setSizeGripEnabled(false);
	
	/*
	 * Populate the tree view with the list of users.
	 */
	QTreeWidgetItem* treeItem = new QTreeWidgetItem;
	treeItem->setText(0, tr("Administration"));
	mAdminTree->setHeaderItem(treeItem);
	
	// add the "Users" item
	mUsersTreeItem = new QTreeWidgetItem;
	mUsersTreeItem->setText(0, tr("Users"));
	mUsersTreeItem->setData(0, Qt::UserRole, QVariant(1));
	mAdminTree->addTopLevelItem(mUsersTreeItem);
	
	
	/*
	 * Populate the tree view with the list of groups.
	 */
	 
	// add the "Groups" item
	mGroupsTreeItem = new QTreeWidgetItem;
	mGroupsTreeItem->setText(0, tr("Groups"));
	mGroupsTreeItem->setData(0, Qt::UserRole, QVariant(2));
	mAdminTree->addTopLevelItem(mGroupsTreeItem);
	
	// select the "Users" item
	mUsersTreeItem->setSelected(true);
	
	// create the header item for the "Existing limiters" table
	mLimitersTable->setColumnCount(4);
	QTableWidgetItem* limitersHeader = new QTableWidgetItem;
	limitersHeader->setData(Qt::DisplayRole, QVariant(tr("Table")));
	mLimitersTable->setHorizontalHeaderItem(0, limitersHeader);
	
	limitersHeader = new QTableWidgetItem;
	limitersHeader->setData(Qt::DisplayRole, QVariant(tr("Command")));
	mLimitersTable->setHorizontalHeaderItem(1, limitersHeader);
	
	limitersHeader = new QTableWidgetItem;
	limitersHeader->setData(Qt::DisplayRole, QVariant(tr("Limiter Clause")));
	mLimitersTable->setHorizontalHeaderItem(2, limitersHeader);
	
	limitersHeader = new QTableWidgetItem;
	limitersHeader->setData(Qt::DisplayRole, QVariant(tr("Do Clause")));
	mLimitersTable->setHorizontalHeaderItem(3, limitersHeader);
	
	mLimitersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	mLimitersTable->setSelectionMode(QAbstractItemView::SingleSelection);

	PopulateGroups();
	PopulateUsers();
	PopulateTables();
	
	// make the thing switch between user and group view modes
	connect(mAdminTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(AdminTreeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(mLimitersTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(LimitersListItemClicked(QTableWidgetItem*)));
	// user-related buttons
	connect(mAddUserButton, SIGNAL(clicked()), this, SLOT(AddUserClicked()));
	connect(mDeleteUserButton, SIGNAL(clicked()), this, SLOT(DeleteUserClicked()));
	connect(mCommitUserButton, SIGNAL(clicked()), this, SLOT(CommitUserClicked()));
	connect(mCloseUserButton, SIGNAL(clicked()), this, SLOT(accept()));
	// limiter buttons
	connect(mAddLimiterButton, SIGNAL(clicked()), this, SLOT(AddLimiterClicked()));
	connect(mDeleteLimiterButton, SIGNAL(clicked()), this, SLOT(DeleteLimiterClicked()));
	connect(mCommitLimiterButton, SIGNAL(clicked()), this, SLOT(CommitLimiterClicked()));
	// group-related buttons
	connect(mAddGroupButton, SIGNAL(clicked()), this, SLOT(AddGroupClicked()));
	connect(mDeleteGroupButton, SIGNAL(clicked()), this, SLOT(DeleteGroupClicked()));
	connect(mCommitGroupButton, SIGNAL(clicked()), this, SLOT(CommitGroupClicked()));
	connect(mCloseButton, SIGNAL(clicked()), this, SLOT(accept()));
}

//*********************************************************************************
void AdminWindow::AddUserClicked ()
{
	SetAddUserMode();
}

//************************************************************************************
void AdminWindow::DeleteUserClicked ()
{
	if ((mUsernameEdit->text().length() == 0) || (mUsernameEdit->isReadOnly() == false))
	{
		QMessageBox::information(this, tr("Info"), tr("You must first select an existing user to delete"));
		return;
	}
	
	// delete the user from the database
	if (!gDb.RemoveUser(mUsernameEdit->text().toStdString()))
		return;
	
	// repopulate the list of users
	PopulateUsers();
	
	// select the "Users" element
	mAdminTree->setCurrentItem(mUsersTreeItem);
}

//*******************************************************************************************
void AdminWindow::CommitUserClicked ()
{
	// check that there's a username
	if (mUsernameEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must supply a username"));
		return;
	}
	
	if (mPasswordEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must supply a password for this user"));
		return;
	}
	
	if (mConfirmEdit->text() != mPasswordEdit->text())
	{
		QMessageBox::information(this, tr("Info"), tr("The password and confirmation password do not match"));
		return;
	}
	
	if (mFullnamesEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must supply this user's full names"));
		return;
	}
	
	if (mIDEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must supply this user's ID number"));
		return;
	}
	
	if (mEmailEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must supply this user's e-mail address"));
		return;
	}
	
	if (mGroupCombo->currentIndex() == -1)
	{
		QMessageBox::information(this, tr("Info"), tr("This user must belong to a group"));
		return;
	}
	
	// add the user
	if (!gDb.AddUser(mUsernameEdit->text().toStdString(), mPasswordEdit->text().toStdString(),
		mGroupCombo->currentText().toStdString(), mFullnamesEdit->text().toStdString(),
		mIDEdit->text().toStdString(), mEmailEdit->text().toStdString()))
		return;
	
	// if we're in add mode
	if (!mUsernameEdit->isReadOnly())
	{
		QTreeWidgetItem* treeItem = new QTreeWidgetItem;
		treeItem->setText(0, mUsernameEdit->text());
		treeItem->setData(0, Qt::UserRole, 1);
		// add the user to the tree
		mUsersTreeItem->addChild(treeItem);
		mAdminTree->setCurrentItem(treeItem);
	}
}

//*******************************************************************************************
void AdminWindow::AdminTreeItemChanged (QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	// if it's in the "Users" item
	if (current->data(0, Qt::UserRole).toInt() == 1)
	{
		mAdminStack->setCurrentIndex(0);
		if (current->text(0) == "Users")
		{
			// go into "Add" mode
			SetAddUserMode();
		} else
		{
			// go into "Modify" mode
			SetModifyUserMode(current->text(0));
		}
	} else
	// if it's the "Groups" item
	if (current->data(0, Qt::UserRole).toInt() == 2)
	{
		// switch to groups view
		mAdminStack->setCurrentIndex(1);
		if (current->text(0) == "Groups")
		{
			// go into "Add" mode
			SetAddGroupMode();
		} else
		{
			// go into "Modify" mode for this group
			SetModifyGroupMode(current->text(0));
		}
	}
}

//***************************************************************************
void AdminWindow::SetAddUserMode (bool selectUsers)
{
	mUsernameEdit->setText(tr(""));
	// make sure it's editable
	mUsernameEdit->setReadOnly(false);
	mPasswordEdit->setText(tr(""));
	mConfirmEdit->setText(tr(""));
	mFullnamesEdit->setText(tr(""));
	mIDEdit->setText(tr(""));
	mEmailEdit->setText(tr(""));
	mGroupCombo->setCurrentIndex(-1);
	// select the "Users" tree item
	if (selectUsers)
	{
		mAdminTree->clearSelection();
		mAdminTree->setCurrentItem(mUsersTreeItem);
	}
}

//*****************************************************************************
void AdminWindow::SetAddGroupMode (bool selectGroups)
{
	mGroupNameEdit->setText(tr(""));
	mGroupNameEdit->setReadOnly(false);
	ClearLimiterFields();
	// remove all the items in the table
	mLimitersTable->setRowCount(0);
	// select the "Groups" tree item
	if (selectGroups)
	{
		mAdminTree->clearSelection();
		mAdminTree->setCurrentItem(mGroupsTreeItem);
	}
}

//**********************************************************************************
void AdminWindow::PopulateGroups ()
{
	StringArray      groups;
	int              i, count;
	QTreeWidgetItem* treeItem;
	
	// clear the groups combo box
	mGroupCombo->clear();
	// remove the groups tree item's children
	mGroupsTreeItem->takeChildren();
	
	// get the groups' names
	gDb.GetGroups(groups);
	count = groups.size();
	for (i=0;i<count;i++)
	{
		// add this group's name
		mGroupCombo->addItem(tr(groups[i].c_str()));
		treeItem = new QTreeWidgetItem;
		treeItem->setText(0, tr(groups[i].c_str()));
		treeItem->setData(0, Qt::UserRole, QVariant(2));
		// add the group to the groups tree item
		mGroupsTreeItem->addChild(treeItem);
	}
}

//*****************************************************************************************
void AdminWindow::SetModifyUserMode (const QString& username)
{
	string uname = username.toStdString();
	
	// clear the controls first
	SetAddUserMode(false);
	
	if (!gDb.PerformRawSql("select * from qrap_users where username='"+uname+"';"))
		return;
	
	// disable editing in the username field
	mUsernameEdit->setReadOnly(true);
	
	pqxx::result lastResult;
	
	gDb.GetLastResult(lastResult);
	if (lastResult.size() > 0)
	{
		mUsernameEdit->setText(tr(lastResult[0]["username"].c_str()));
		mFullnamesEdit->setText(tr(lastResult[0]["fullnames"].c_str()));
		mIDEdit->setText(tr(lastResult[0]["idno"].c_str()));
		mEmailEdit->setText(tr(lastResult[0]["email"].c_str()));
		mGroupCombo->setCurrentIndex(mGroupCombo->findText(tr(lastResult[0]["groupname"].c_str())));
	}
}

//****************************************************************************************
void AdminWindow::SetModifyGroupMode (const QString& groupname)
{
	string gname = groupname.toStdString();
	
	// clear the controls first
	SetAddGroupMode(false);
	
	// get the group's details
	if (!gDb.PerformRawSql("select * from qrap_groups where groupname='"+gname+"' order by tablename asc;"))
		return;
	
	mGroupNameEdit->setReadOnly(true);
	
	pqxx::result lastResult;
	int          i, count;
	string       ruleName;
	
	gDb.GetLastResult(lastResult);
	if (lastResult.size() > 0)
	{
		mGroupNameEdit->setText(tr(lastResult[0]["groupname"].c_str()));
		count = lastResult.size();
		// add the limiters to the limiters table
		for (i=0;i<count;i++)
		{
			mLimitersTable->setRowCount(i+1);
			mLimitersTable->setItem(i, 0, new QTableWidgetItem(tr(lastResult[i]["tablename"].c_str())));
			mLimitersTable->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			// store the limiter's rule name in the User Role data
			ruleName = lastResult[i]["rulename"].c_str();
			// if it's merely a privilege
			if (ruleName.length() == 0)
				ruleName = "privilege_"+string(lastResult[i]["cmd"].c_str());
			mLimitersTable->item(i, 0)->setData(Qt::UserRole, QVariant(ruleName.c_str()));
			mLimitersTable->setItem(i, 1, new QTableWidgetItem(tr(lastResult[i]["cmd"].c_str())));
			mLimitersTable->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			mLimitersTable->setItem(i, 2, new QTableWidgetItem(tr(lastResult[i]["limiter"].c_str())));
			mLimitersTable->item(i, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			mLimitersTable->setItem(i, 3, new QTableWidgetItem(tr(lastResult[i]["docmd"].c_str())));
			mLimitersTable->item(i, 3)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
	}
}

//*********************************************************************************************
void AdminWindow::PopulateUsers ()
{
	int              i, count;
	StringArray      usernames;
	QTreeWidgetItem* treeItem;
	
	// remove any children of the "Users" tree item
	mUsersTreeItem->takeChildren();
	
	gDb.GetUsers(usernames);
	count = usernames.size();
	for (i=0;i<count;i++)
	{
		treeItem = new QTreeWidgetItem;
		treeItem->setText(0, tr(usernames[i].c_str()));
		treeItem->setData(0, Qt::UserRole, QVariant(1));
		mUsersTreeItem->addChild(treeItem);
	}
}

//**********************************************************************************************
void AdminWindow::PopulateTables ()
{
	DbStructure dbs;
	
	gDb.GetDbStructure(dbs);
	// clear the combo box
	mTableCombo->clear();
	// add all the tables' names
	for (DbStructureIterator table=dbs.begin(); table != dbs.end(); table++)
		if (table->second.mVisible)
			mTableCombo->addItem(tr(table->first.c_str()));
	
	mCommandCombo->clear();
	mCommandCombo->addItem(tr("select"));
	mCommandCombo->addItem(tr("insert"));
	mCommandCombo->addItem(tr("update"));
	mCommandCombo->addItem(tr("delete"));
}

//*******************************************************************************************
void AdminWindow::LimitersListItemClicked (QTableWidgetItem* item)
{
	int curRow = mLimitersTable->currentRow();
	if (curRow == -1) return;
	// populate the limiter controls with the currently selected limiter's details
	mTableCombo->setCurrentIndex(mTableCombo->findText(mLimitersTable->item(curRow, 0)->text()));
	mCommandCombo->setCurrentIndex(mCommandCombo->findText(mLimitersTable->item(curRow, 1)->text()));
	mLimiterEdit->setText(mLimitersTable->item(curRow, 2)->text());
	mDoEdit->setText(mLimitersTable->item(curRow, 3)->text());
}

//*******************************************************************************************
void AdminWindow::ClearLimiterFields ()
{
	mTableCombo->setCurrentIndex(-1);
	mCommandCombo->setCurrentIndex(-1);
	mLimiterEdit->setText(tr(""));
	mDoEdit->setText(tr(""));
}

//****************************************************************************************
void AdminWindow::AddLimiterClicked ()
{
	mLimitersTable->setCurrentCell(-1, -1);
	mLimitersTable->clearSelection();
	ClearLimiterFields();
}

//*********************************************************************************************
void AdminWindow::DeleteLimiterClicked ()
{
	int curRow = mLimitersTable->currentRow();
	if (curRow == -1)
	{
		QMessageBox::information(this, tr("Info"), tr("You must first select a limiter to delete it"));
		return;
	}
	
	// try to remove the limiter
	if (!gDb.RemoveRule(mLimitersTable->item(curRow, 0)->data(Qt::UserRole).toString().toStdString(),
		mLimitersTable->item(curRow, 0)->text().toStdString(),
		mGroupNameEdit->text().toStdString()))
		return;
	
	// delete the item from the list
	mLimitersTable->removeRow(curRow);
}

//********************************************************************************************
void AdminWindow::CommitLimiterClicked ()
{
	int curRow = mLimitersTable->currentRow();
	
	if (mGroupNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must first enter a group name to which these limiters will belong"));
		return;
	}
	
	if (mTableCombo->currentIndex() == -1)
	{
		QMessageBox::information(this, tr("Info"), tr("You must first select a table for which to create a limiter"));
		return;
	}
	
	if (mCommandCombo->currentIndex() == -1)
	{
		QMessageBox::information(this, tr("Info"), tr("You must first select a command for which to create a limiter"));
		return;
	}
	
	cDatabase::SqlRule rule;
	
	rule.mLimiter = mLimiterEdit->text().toStdString();
	rule.mDo = mDoEdit->text().toStdString();
	
	// if we're in Add mode
	if (curRow == -1)
	{
		// if the user is adding a select query
		if (mCommandCombo->currentIndex() == mCommandCombo->findText(tr("select")))
		{
			// if a select query already exists for this table
			if (!gDb.PerformRawSql("select id from qrap_groups where cmd='select' and tablename='"+
				mTableCombo->currentText().toStdString()+"' and groupname='"+
				mGroupNameEdit->text().toStdString()+"' and rulename is not null;"))
				return;
			pqxx::result lastResult;
			gDb.GetLastResult(lastResult);
			// if there is a result, refuse the addition
			if (lastResult.size() > 0)
			{
				QMessageBox::information(this, tr("Info"), tr("You may have only one select limiter per table per group"));
				return;
			}
		}
	} else
	// if we're in update mode
	{
		// if the user has modified the command type
		if (mCommandCombo->currentText() != mLimitersTable->item(curRow, 1)->text())
		{
			// if the user has modified it to a select command
			if (mCommandCombo->currentIndex() == mCommandCombo->findText(tr("select")))
			{
				// if a select query already exists for this table
				if (!gDb.PerformRawSql("select id from qrap_groups where cmd='select' and tablename='"+
					mTableCombo->currentText().toStdString()+"' and groupname='"+
					mGroupNameEdit->text().toStdString()+"' and rulename is not null;"))
					return;
				pqxx::result lastResult;
				gDb.GetLastResult(lastResult);
				// if there is a result, refuse the addition
				if (lastResult.size() > 0)
				{
					QMessageBox::information(this, tr("Info"), tr("You may have only one select limiter per table per group"));
					return;
				}
			}
		}
		
		// remove the existing rule
		if (!gDb.RemoveRule(mLimitersTable->item(curRow, 0)->data(Qt::UserRole).toString().toStdString(),
			mLimitersTable->item(curRow, 0)->text().toStdString(),
			mGroupNameEdit->text().toStdString()))
			return;
	}
	
	// attempt to add the limiter
	if (!gDb.CreateRule(mCommandCombo->currentText().toStdString(), mTableCombo->currentText().toStdString(),
		rule, mGroupNameEdit->text().toStdString()))
		return;
	
	// now refresh the list of limiters
	SetModifyGroupMode(mGroupNameEdit->text());
}

//*********************************************************************************************
void AdminWindow::AddGroupClicked ()
{
	SetAddGroupMode();
}

//***********************************************************************************************
void AdminWindow::DeleteGroupClicked ()
{
	if ((mGroupNameEdit->text().length() == 0) || (!mGroupNameEdit->isReadOnly()))
	{
		QMessageBox::information(this, tr("Info"), tr("You must first select an existing group before you can delete it"));
		return;
	}
	
	// delete the group
	if (!gDb.RemoveGroup(mGroupNameEdit->text().toStdString()))
		return;
	
	// repopulate the list of groups
	PopulateGroups();
	
	// select the "Groups" tree item
	mAdminTree->setCurrentItem(mGroupsTreeItem);
}

//******************************************************************************************
void AdminWindow::CommitGroupClicked ()
{
	if (mGroupNameEdit->text().length() == 0)
	{
		QMessageBox::information(this, tr("Info"), tr("You must first enter a group name"));
		return;
	}
	// if there is whitespace in the group's name or any other special characters, disallow it
	string           groupName = mGroupNameEdit->text().toStdString();
	int              i, len = groupName.length();
	char             ch;
	QTreeWidgetItem* treeItem;
	pqxx::result     lastResult;
	
	if (groupName == "Groups")
	{
		QMessageBox::information(this, tr("Info"), tr("Invalid group name"));
		return;
	}
	
	for (i=0;i<len;i++)
	{
		ch = tolower(groupName[i]);
		if (((ch < 'a') || (ch > 'z')) &&
			((ch < '0') || (ch > '9')))
		{
			QMessageBox::information(this, tr("Info"), tr("Group names may only contain alphanumeric characters"));
			return;
		}
	}
	
	// if the group already exists
	if (!gDb.PerformRawSql("select distinct(groupname) from qrap_groups where groupname='"+groupName+"';"))
		return;
	
	gDb.GetLastResult(lastResult);
	if (lastResult.size() > 0)
	{
		QMessageBox::information(this, tr("Info"), tr("A group with that name already exists"));
		return;
	}
	
	// if we're in Add mode
	if ((mAdminTree->currentItem()->data(0, Qt::UserRole).toInt() == 2) &&
		(mAdminTree->currentItem()->text(0) == "Groups"))
	{
		// add the group
		cDatabase::GroupPrivileges priv;
		DbStructure                dbs;
		
		gDb.GetDbStructure(dbs);
		
		for (DbStructureIterator table=dbs.begin(); table != dbs.end(); table++)
		{
			// add this table to the privileges structure
			if (table->second.mVisible)
			{
				// simply access a member to create all its privileges
				priv.mTablePrivileges[table->first].mSelect = true;
			}
		}
		
		// create the group
		if (!gDb.AddGroup(groupName, priv))
			return;
		
		treeItem = new QTreeWidgetItem;
		treeItem->setText(0, tr(groupName.c_str()));
		treeItem->setData(0, Qt::UserRole, 2);
		// add the group to the tree
		mGroupsTreeItem->addChild(treeItem);
		mAdminTree->setCurrentItem(treeItem);
	}
}

#endif
