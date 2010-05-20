 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapFormBuilder.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *		  : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 custom widget that will display a mTable view 
 *                  in the one tab and a edit form on the other tab.
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
 ***************************************************************************/

#ifndef Qrap_cRapFormBuilder_h
#define Qrap_cRapFormBuilder_h

// Include global headers
#include <QDataWidgetMapper>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGridLayout>
#include <QMap>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QComboBox>
#include <QRegExp>
#include <QHeaderView>
#include <math.h>
// Include local headers
#include "MainWindow.h"


namespace Qrap
{
	/**
	* The cRapFormBuilder class, this class inherits the QWidget class and is responsible for containing
	* an edit form that will allow the user to edit the data in the table view.
	*/
	class cRapFormBuilder : public QWidget
	{
		Q_OBJECT
		
	signals:
		void InsertComplete (int index);
		
	public:
		/**
		* This constructor is responsible for creating the form from the database structure.
		* 
		* @param parent A pointer to the parent widget.
		* @param tableName The name of the database table that this form is related to.
		* @param tableWidget A pointer to the QTableWidget to which this form will be mapped.
		* @param insert A boolean that specifies if an insert query is being called.
		*/	
		cRapFormBuilder (QWidget* parent=0,
				QString tableName = "NULL",
				QTableWidget* tableWidget=NULL,
			        bool insert=false);
			        
		/**
		* This constructor is responsible for creating the form from the database structure.
		* 
		* @param parent A pointer to the parent widget.
		* @param tableName The name of the database table that this form is related to.
		* @param tableWidget A pointer to the QTableWidget to which this form will be mapped.
		* @param insert A boolean that specifies if an insert query is being called.
		*/	
		cRapFormBuilder (StringMap ref, QWidget* parent=0,
				QString tableName = "NULL",
				QTableWidget* tableWidget=NULL,
			        bool insert=false);
			        

			        
		/**
		 * This sets the refereces to cerain fields
		 * 
		 * @param reference A set of references
		 */
		 void SetReferences(StringMap reference) {mReferences = reference;}
		
	private slots:
		/**
		 * Private SLOT - This will update the buttons' enabled property depending which row is selected within the table view.
		 * 
		 * @param row The currently selected row.
		 */
		void ImportAntennaFromFile ();
		
		/**
		 * Private SLOT - This slot will emit a RowSelectionChanged signal that will tell the formMapper to update it's form data.
		 */
		void TableItemSelectionChanged();
		
		/**
		 * Private SLOT - Clear the form widgets.
		 */
		void Clear ();
		
		/**
		 * Private SLOT - This slot is responisible for handling the insert query of the new data.
		 */
		void Insert ();
		
		/**
		 * Private SLOT - This slot is responsible for inserting the current form data and then clearing the form data for another insert query to take place.
		 */
		void InsertNext ();
		
		/**
		 * Private SLOT - This slot is responsibe for inserting the antenna device and pattern data.
		 */
		void InsertAntenna ();
		
		/**
		 * Private SLOT - This slot is responsible for inserting the current antenna data and then clears the form for a next round.
		 */
		void InsertNextAntenna ();
		
		/**
		 * Private SLOT - This slot is responsible for updating the content of a particular column that was changed in the form.
		 */
		void Update ();
		
		/**
		 * Private SLOT - This slot is responsible for changing the selection of the table view to the next row and then updating the form's contents.
		 */
		void NextRecord ();
		
		/**
		 * Private SLOT - This slot is responsible for changing the selection of the table view to the previous row and then updating the form's contents.
		 */
		void PreviousRecord ();
		
		/**
		 * Private SLOT - This slot is responsible for enabling the update button whenever the content of a form widget has changed.
		 * 
		 * @param text The text string that changed.
		 */
		void ContentChanged (const QString& text);
		
		/**
		 * Private SLOT - This slot is responsible for enabling the update button whenever the content of a form widget has changed.
		 * 
		 * @param val The number that was changed.
		 */
		void ContentChanged (int val);
		
		/**
		 * Private SLOT - This slot is responsible for enabling the update button whenever the content of a form widget has changed.
		 * 
		 * @param val The number that changed.
		 */
		void ContentChanged (double val);
		
		/**
		 * Private SLOT - This slot is responsible for enabling the update button whenever the content of a form widget has changed.
		 * 
		 * @param dateTime The datetime that changed.
		 */
		void ContentChanged (QDateTime& dateTime);
		
		/**
		 * Private SLOT - This slot is responsible for enabling the update button whenever the content of a form widget has changed.
		 * 
		 * @param date The date that changed
		 */
		void ContentChanged (QDate& date);

		void FormatToDDdddd();
		void FormatToDDMMmm();
		void FormatToDDMMSS();

		void FindGroundHeight();

		void CloseBySite();
	
		/**
		 * Private SLOT - This slot will enable the antenna device form widgets and disable the browseButton.
		 */
		void AntennaDeviceFromManualInput (bool checked);
		
		/**
		 * Private SLOT - This slot will disable the antenna device form widget and enabled the browseButton.
		 */
		void AntennaDeviceFromFile (bool checked);
		
		/**
		 * Private SLOT - This function checks if the last cell in the table has been changed and if so a new row is created.
		 */
		void CreateAzimuthRow(int row, int col);
		
		/**
		 * Private SLOT - This function checks if the last cell in the table has been changed and if so a new row is created.
		 */
		void CreateElevationRow(int row, int col);
		
		/**
		 * Private SLOT - This function checks if the last cell in the table has been changed and if so a new row is created.
		 */
		void CreateEnvelopeRow(int row, int col);
		
	private:
		/**
		 * This function will create the form input widget and label based on the data type.
		 * 
		 * @param fieldName The name of the field that this widget repesents.
		 * @param fieldLabel The text description of this field.
		 * @param widgets This is the relevant widget map the contains all the widgets.
		 * @param type The data type of the field.
		 * @param val The data that needs to be converted and inserted to the form.
		 * @param layoutRow This is a counter variable keeping track of the position of the widget in the grid layout.
		 * @param readOnly Determines whether a widget is enabled or not.
		 * @param inserting Determines whether this form is for inserting or not.
		 */
		void CreateFormWidget (QString fieldName,
				       QString fieldLabel,
				       QMap<QString,QWidget*> &widgets,
				       cDatabase::FieldDataType type,
				       QVariant val,
					int &layoutRow,
				       bool readOnly);
		
		/**
		 * This function will populate the current form's widgets with the next row's data from the table view.
		 */
		void PopulateForm ();
		
		/**
		 * This is the actual function that will insert the data from the form into the database.
		 * 
		 * @param tableName The name of the table that the data of the form needs to be written to.
		 * @param widgets The map containing the widgets with the data.
		 * @param update Indicates whether an update must be performed rather than an insert.
		 * @param antennaDevice Indicates whether an antenna device is being inserted which means that the pattern values cannot be sent to the tableView.
		 * @param antdevicekey This holds the primarykey of the new antenna device that was inserted, this will become the foreign key value for the respective antenna pattern.
		 */
		int InsertData (const QString& tableName,
				 QMap<QString,QWidget*> &widgets,
				 bool antennaDevice=false,
				 int antdevicekey=-1);
		
		/**
		 * This function will create the form that is used for inserting the data.
		 */
		void CreateGenericInsertForm ();
		
		/**
		 * This function will create the update form which is used for updating the current information in the table.
		 */
		void CreateGenericUpdateForm ();
		
		/**
		 * This function will create the antenna device form.
		 */
		void CreateAntennaDeviceForm ();

		void FindCurrentLocation ();
		
		QMap<QString,QWidget*>	mFormWidgets;		///< A map that contains all the widgets in the form.
		QMap<QString,QWidget*>  mPatternWidgets;		///< A map that contains all the widgets in the antenna pattern form
		QMap<QString,QLabel*>	mFormLabels;			///< A map that contains all the labels in the form.
		QDataWidgetMapper*	mFormMapper;				///< A Qt4 pointer to a mapper class that will map the data from the table to the form.
		QTableWidget*		mTableView;				///< A pointer to a Qt4 to the table widget that this form represents.
		QTableWidget*		mAzimuthTable;		///< A pointer to a Qt4 to the table widget that will contain all the azimuth angles and pattern values, this is only applicable for the antennadevice and antennapattern forms and not the generic forms.
		QTableWidget*		mElevationTable;		///< A pointer to a Qt4 to the table widget that will contain all the elevation angles and pattern values, this is only applicable for the antennadevice and antennapattern forms and not the generic forms.
		QTableWidget*		mEnvelopeTable;		///< A pointer to a Qt4 to the table widget that will contain all the offsets and values for the spectral envelope, this is only used when editing the spectral envelopes.
		QPushButton*		mNextButton;		///< A pointer to a Qt4 push button widget that when pushed allows the user to navigate to the next row.
		QPushButton*		mPreviousButton;	///< A pointer to a Qt4 push button widget that when pushed allows the user to navigate to the previous row.
		QPushButton*		mCommitButton;		///< A pointer to a Qt4 push button widget that when pushed will commit all the information in the form widgets to the database.
		QPushButton*		mCommitAddNextButton;	///< A pointer to a Qt4 push button widget that when pushed will commit all the information to the database and build another form.
		QPushButton*		mBrowseButton;		///< A pointer to a Qt4 push button widget that when pushed will load the import antennas function.
		QPushButton*		mUpdateButton;		///< A pointer to a Qt4 push button widget that when pushed will update the changed made to the content of the form.
		QPushButton*		mGroundHeightButton;

		QPushButton*		mCloseByButton;

		StringMap		mDefaults;		///< This contains the default values for all the fields.
		QString			mTable;			///< This QString holds the name of the table that this form builder represents.
		QGridLayout*		mFormLayout;		///< A pointer to a Qt4 grid layout that will contain all the form widgets.
		int			mLayoutRow;
		int			mButtonRow;
		int			mButtonPosY;
		int			mButtonPosX0;
		int			mButtonPosX1;
		int			mButtonPosX2;
		int			mLayoutDelta;
		DbStructure		mDbs;			///< This contains the database structure.
		StringMap		mReferences;
		bool			mUpdate;
		bool			mInserting;
		int			mCurrentRecordID;
		double 			mLat, mLon;		///<Coordinates of the current site
		bool			mFindNewHeight;
		DegreeFormat		mDegreeFormat;
	};
}
#endif
